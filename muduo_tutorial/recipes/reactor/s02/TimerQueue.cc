// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#define __STDC_LIMIT_MACROS
#include "TimerQueue.h"

#include "EventLoop.h"
#include "Timer.h"
#include "TimerId.h"
#include "logging/Logging.h"

#include <boost/bind.hpp>

#include <sys/timerfd.h>

namespace muduo
{
namespace detail
{
int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0)
    {
        LOG_SYSFATAL << "Failed in timerfd_create";
    }
    return timerfd;
}

struct timespec howMuchTimeFromNow(Timestamp when)
{
    int64_t microseconds =
        when.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();
    if (microseconds < 100)
    {
        microseconds = 100;
    }
    struct timespec ts;
    ts.tv_sec =
        static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(
        (microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
    return ts;
}

void readTimerfd(int timerfd, Timestamp now)
{
    uint64_t howmany;
    ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
    LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at "
              << now.toString();
    if (n != sizeof howmany)
    {
        LOG_ERROR << "TimerQueue::handleRead() reads " << n
                  << " bytes instead of 8";
    }
}

// 定时的主要逻辑功能实在这里实现的，通过设计一个fd在多少时间之后可读，来实现定时
void resetTimerfd(int timerfd, Timestamp expiration)
{
    // wake up loop by timerfd_settime()
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue, sizeof newValue);
    bzero(&oldValue, sizeof oldValue);
    newValue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    if (ret)
    {
        LOG_SYSERR << "timerfd_settime()";
    }
}
} // namespace detail
} // namespace muduo

using namespace muduo;
using namespace muduo::detail;

TimerQueue::TimerQueue(EventLoop *loop)
    : loop_(loop), timerfd_(createTimerfd()), timerfdChannel_(loop, timerfd_),
      timers_()
{
    // handleRead 是如果timerfd的事件可读的时候的回调函数
    timerfdChannel_.setReadCallback(boost::bind(&TimerQueue::handleRead, this));

    // we are always reading the timerfd, we disarm it with timerfd_settime.
    // 把这个fd和channel建立映射，一旦fd可以读的时候，可以调用fd的回调函数
    // 表示关注这个channel的可读事件
    timerfdChannel_.enableReading();
}

TimerQueue::~TimerQueue()
{
    ::close(timerfd_);
    // do not remove channel, since we're in EventLoop::dtor();
    for (TimerList::iterator it = timers_.begin(); it != timers_.end(); ++it)
    {
        delete it->second;
    }
}

// 这个是核心代码，插入timer
TimerId TimerQueue::addTimer(const TimerCallback &cb, Timestamp when,
                             double interval)
{
    Timer *timer = new Timer(cb, when, interval);
    loop_->assertInLoopThread();
    bool earliestChanged = insert(timer);

    if (earliestChanged)
    {
        // 定时的主要逻辑功能实在这里实现的，通过设计一个fd在多少时间之后可读，来实现定时
        resetTimerfd(timerfd_, timer->expiration());
    }
    return TimerId(timer);
}

// timerfd可读的回调事件
void TimerQueue::handleRead()
{
    loop_->assertInLoopThread();
    Timestamp now(Timestamp::now());
    readTimerfd(timerfd_, now);

    //到期事件的vector
    std::vector<Entry> expired = getExpired(now);

    // safe to callback outside critical section
    // 执行到期的事件
    for (std::vector<Entry>::iterator it = expired.begin(); it != expired.end();
         ++it)
    {
        it->second->run();
    }

    reset(expired, now);
}

// 从tiemrs_里面，返回已经到期的timer，并通过vector返回。
// 从原来的timers_里面，删除到期的事件
std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
    std::vector<Entry> expired;
    Entry sentry = std::make_pair(now, reinterpret_cast<Timer *>(UINTPTR_MAX));
    TimerList::iterator it = timers_.lower_bound(sentry);
    assert(it == timers_.end() || now < it->first);
    std::copy(timers_.begin(), it, back_inserter(expired));
    timers_.erase(timers_.begin(), it);

    return expired;
}

// 给用即将到期的下一个事件的时间，设置timer
void TimerQueue::reset(const std::vector<Entry> &expired, Timestamp now)
{
    Timestamp nextExpire;
    for (std::vector<Entry>::const_iterator it = expired.begin();
         it != expired.end(); ++it)
    {
        if (it->second->repeat())
        {
            it->second->restart(now);
            insert(it->second);
        }
        else
        {
            // FIXME move to a free list
            delete it->second;
        }
    }
    if (!timers_.empty())
    {
        nextExpire = timers_.begin()->second->expiration();
    }
    if (nextExpire.valid())
    {
        resetTimerfd(timerfd_, nextExpire);
    }
}

bool TimerQueue::insert(Timer *timer)
{
    bool earliestChanged = false;
    Timestamp when = timer->expiration();
    TimerList::iterator it = timers_.begin();
    if (it == timers_.end() || when < it->first)
    {
        earliestChanged = true;
    }
    std::pair<TimerList::iterator, bool> result =
        timers_.insert(std::make_pair(when, timer));
    assert(result.second);
    return earliestChanged;
}
