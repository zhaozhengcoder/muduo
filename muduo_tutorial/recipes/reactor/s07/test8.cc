#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpServer.h"
#include <stdio.h>

void onConnection(const muduo::TcpConnectionPtr &conn) {
  if (conn->connected()) {
    printf("onConnection(): new connection [%s] from %s\n",
           conn->name().c_str(), conn->peerAddress().toHostPort().c_str());
  } else {
    printf("onConnection(): connection [%s] is down\n", conn->name().c_str());
  }
}

void onMessage(const muduo::TcpConnectionPtr &conn, muduo::Buffer *buf,
               muduo::Timestamp receiveTime) {
  printf("onMessage(): received %zd bytes from connection [%s] at %s\n",
         buf->readableBytes(), conn->name().c_str(),
         receiveTime.toFormattedString().c_str());

  // buf->retrieveAsString() 读取所有的消息
  printf("onMessage(): [%s]\n", buf->retrieveAsString().c_str());
}

int main() {
  printf("main(): pid = %d\n", getpid());

  muduo::InetAddress listenAddr(9981);
  muduo::EventLoop loop;

  muduo::TcpServer server(&loop, listenAddr);
  server.setConnectionCallback(onConnection);
  server.setMessageCallback(onMessage);
  server.start();

  loop.loop();
}

// 阅读
// 这个加入了buffer的实现，也就是相比之前可以处理客户端发来的消息了

// + TcpConnection.cc 文件里面的 void TcpConnection::handleRead(Timestamp
// receiveTime) 函数
// + buffer.cc 文件 关于buffer的实现