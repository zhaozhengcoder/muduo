// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "Poller.h"
#include "Channel.h"
#include "logging/Logging.h"
#include <assert.h>
#include <poll.h>
#include <stdio.h>
using namespace muduo;

Poller::Poller(EventLoop* loop)
  : ownerLoop_(loop)
{
}

Poller::~Poller()
{
}

Timestamp Poller::poll(int timeoutMs, ChannelList* activeChannels)
{
  // XXX pollfds_ shouldn't change
  // int nready = poll(pollfdArry,cur_fd_num,-1);
  int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);
  Timestamp now(Timestamp::now());
  if (numEvents > 0) {
    LOG_TRACE << numEvents << " events happended";
    fillActiveChannels(numEvents, activeChannels);   //将活动的事件，添加到fd的activeChannels里面去
  } 
  else if (numEvents == 0) 
  {
    LOG_TRACE << " nothing happended";
  } else {
    LOG_SYSERR << "Poller::poll()";
  }
  return now;
}

void Poller::fillActiveChannels(int numEvents,
                                ChannelList* activeChannels) const
{
  printf("void Poller::fillActiveChannels(int numEvents, ... ) \n");
  for (PollFdList::const_iterator pfd = pollfds_.begin();
      pfd != pollfds_.end() && numEvents > 0; ++pfd)
  {
    if (pfd->revents > 0)  // 事件对应的值 //POLLIN  1 , POLLOUT 4 , POLLERR 8
    {
      --numEvents;
      ChannelMap::const_iterator ch = channels_.find(pfd->fd);    //channels_ 是poller类的数据成员
      assert(ch != channels_.end());
      Channel* channel = ch->second;          // fd对应一个channel的对象
      assert(channel->fd() == pfd->fd);
      channel->set_revents(pfd->revents);
      // pfd->revents = 0;
      activeChannels->push_back(channel);
    }
  }
}

void Poller::updateChannel(Channel* channel)
{
  printf("void Poller::updateChannel(Channel* channel) \n");
  assertInLoopThread();
  LOG_TRACE << "fd = " << channel->fd() << " events = " << channel->events();
  printf("[debug] fd = %d , events = %d index : %d \n ",channel->fd(),channel->events(),channel->index()); 
  //[debug] fd = 3 , events = 3 index : -1

  if (channel->index() < 0) 
  {
    // a new one, add to pollfds_
    assert(channels_.find(channel->fd()) == channels_.end());
    struct pollfd pfd;
    pfd.fd = channel->fd();
    pfd.events = static_cast<short>(channel->events());
    pfd.revents = 0;

    pollfds_.push_back(pfd);
    int idx = static_cast<int>(pollfds_.size())-1;
    channel->set_index(idx);
    channels_[pfd.fd] = channel;
  } 
  else 
  {
    // update existing one
    assert(channels_.find(channel->fd()) != channels_.end());
    assert(channels_[channel->fd()] == channel);
    int idx = channel->index();
    assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
    struct pollfd& pfd = pollfds_[idx];
    assert(pfd.fd == channel->fd() || pfd.fd == -1);
    pfd.events = static_cast<short>(channel->events());
    pfd.revents = 0;
    if (channel->isNoneEvent()) 
    {
      // ignore this pollfd
      pfd.fd = -1;
    }
  }
}



/*
struct pollfd
{
    int fd;             //文件描述符 
    short events;       //等待的事件
    short revents;      //实际发生了的事件
};
*/