#include <muduo/base/Logging.h>
#include <muduo/net/Endian.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpClient.h>

#include <boost/bind.hpp>

#include <utility>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class TimeClient : boost::noncopyable
{
 public:
  TimeClient(EventLoop* loop, const InetAddress& serverAddr)
    : loop_(loop),
      client_(loop, serverAddr, "TimeClient")
  {
    client_.setConnectionCallback(
        boost::bind(&TimeClient::onConnection, this, _1));
    client_.setMessageCallback(
        boost::bind(&TimeClient::onMessage, this, _1, _2, _3));
    // client_.enableRetry();
  }

  void connect()
  {
    client_.connect();
  }

 private:

  EventLoop* loop_;
  TcpClient client_;

  void onConnection(const TcpConnectionPtr& conn)
  {
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");

    string str = "hello world";
    StringPiece message(str);
    muduo::net::Buffer buf;
    buf.append(message.data(), message.size());


    conn->send(&buf);
    if (!conn->connected())
    {
      loop_->quit();
    }
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime)
  {
    // if (buf->readableBytes() >= sizeof(int32_t))
    // {
    //   const void* data = buf->peek();
    //   int32_t be32 = *static_cast<const int32_t*>(data);
    //   buf->retrieve(sizeof(int32_t));
    //   time_t time = sockets::networkToHost32(be32);
    //   Timestamp ts(implicit_cast<uint64_t>(time) * Timestamp::kMicroSecondsPerSecond);
    //   LOG_INFO << "Server time = " << time << ", " << ts.toFormattedString();
    // }
    // else
    // {
    //   LOG_INFO << conn->name() << " no enough data " << buf->readableBytes()
    //            << " at " << receiveTime.toFormattedString();
    // }
    std::cout<<buf->readableBytes()<<std::endl;
    if(buf->readableBytes() >= 4) // kHeaderLen == 4
    {
        const void* data = buf->peek();
        int32_t be32 = *static_cast<const int32_t*>(data); // SIGBUS
        const int32_t len = muduo::net::sockets::networkToHost32(be32);
        std::cout<<"[msg len] "<<len<<std::endl;


        buf->retrieve(4);
        muduo::string message(buf->peek(), len);  
        buf->retrieve(len);
        std::cout<<"[msg ] "<<message<<std::endl;
    }
  }
};

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  if (argc > 1)
  {
    EventLoop loop;
    InetAddress serverAddr(argv[1], 7001);

    TimeClient timeClient(&loop, serverAddr);
    timeClient.connect();
    loop.loop();
  }
  else
  {
    printf("Usage: %s host_ip\n", argv[0]);
  }
}

