// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "TcpServer.h"

#include "logging/Logging.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "SocketsOps.h"

#include <boost/bind.hpp>

#include <stdio.h>  // snprintf

using namespace muduo;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr)
  : loop_(CHECK_NOTNULL(loop)),
    name_(listenAddr.toHostPort()),
    acceptor_(new Acceptor(loop, listenAddr)),
    started_(false),
    nextConnId_(1)
{
  acceptor_->setNewConnectionCallback(boost::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
  if (!started_)
  {
    started_ = true;
  }

  if (!acceptor_->listenning())
  {
    // 调用listen函数，作为任务函数
    loop_->runInLoop(boost::bind(&Acceptor::listen, get_pointer(acceptor_)));
  }
}

// 这个是acceptor_对象的回调函数，这个对象在监听socket，如果有事件上来就会调用这个回调函数 
// 建立连接的回调函数
// 这个函数里面最核心要干的事情就是创建一个conn ， 然后 建立一个映射 connections_[connName] = conn; 
void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
  loop_->assertInLoopThread();
  char buf[32];
  snprintf(buf, sizeof buf, "#%d", nextConnId_);
  ++nextConnId_;
  std::string connName = name_ + buf;

  LOG_INFO << "TcpServer::newConnection [" << name_
           << "] - new connection [" << connName
           << "] from " << peerAddr.toHostPort();
  
  InetAddress localAddr(sockets::getLocalAddr(sockfd));
  // FIXME poll with zero timeout to double confirm the new connection
  // 创建conn
  TcpConnectionPtr conn(new TcpConnection(loop_, connName, sockfd, localAddr, peerAddr));
  
  //建立映射
  connections_[connName] = conn;
  conn->setConnectionCallback(connectionCallback_);
  conn->setMessageCallback(messageCallback_);
  
  // 这一行代码很重要 
  // 对于已经建立的连接，那么就要关注这个连接上面的可读和可写事件了
  conn->connectEstablished();
}

