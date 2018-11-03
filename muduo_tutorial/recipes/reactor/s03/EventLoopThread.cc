// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "EventLoopThread.h"
#include <iostream>
#include "EventLoop.h"

#include <boost/bind.hpp>

using namespace muduo;

EventLoopThread::EventLoopThread()
    : loop_(NULL),
      exiting_(false),
      thread_(boost::bind(&EventLoopThread::threadFunc, this)), //设置线程的回调函数
      mutex_(),
      cond_(mutex_)
{
}

EventLoopThread::~EventLoopThread()
{
    exiting_ = true;
    loop_->quit();
    thread_.join();
}

EventLoop *EventLoopThread::startLoop()
{
    assert(!thread_.started());
    // 启动一个线程 , 执行threadFunc函数
    thread_.start();
    {
        MutexLockGuard lock(mutex_);
        //如果loop_是空的话，就等待
        while (loop_ == NULL)
        {
            std::cout << "wait..." << std::endl;
            cond_.wait();
        }
    }

    return loop_;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;

    {
        // 创建一个loop对象，使loop_指向这个对象
        MutexLockGuard lock(mutex_);
        loop_ = &loop;
        cond_.notify();
    }

    loop.loop();
    //assert(exiting_);
}
