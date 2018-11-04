// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "Acceptor.h"

#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include "logging/Logging.h"

#include <boost/bind.hpp>

using namespace muduo;

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr)
    : loop_(loop), acceptSocket_(sockets::createNonblockingOrDie()),
      acceptChannel_(loop, acceptSocket_.fd()), listenning_(false) 
{
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(boost::bind(&Acceptor::handleRead, this));
}

void Acceptor::listen() 
{
    loop_->assertInLoopThread();
    listenning_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}

// 如果对于listen到新的连接，那么调用这个callback
void Acceptor::handleRead() 
{
    loop_->assertInLoopThread();
    InetAddress peerAddr(0);
    // FIXME loop until no more
    int connfd = acceptSocket_.accept(&peerAddr);
    if (connfd >= 0) 
    {
        if (newConnectionCallback_) 
        {
            // 对于新连接，调用这个callback函数，这个是tcpserver.cc
            // 文件里面在构造函数里面设置的
            newConnectionCallback_(connfd, peerAddr);
        } 
        else 
        {
            sockets::close(connfd);
        }
    }
}
