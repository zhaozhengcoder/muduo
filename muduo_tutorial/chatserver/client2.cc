#include <muduo/base/Logging.h>
#include <muduo/net/Endian.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpClient.h>
#include <muduo/base/Mutex.h>
#include <boost/bind.hpp>

#include <utility>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <muduo/net/EventLoopThread.h>
#include <iostream>

using namespace muduo;
using namespace muduo::net;

class TimeClient : boost::noncopyable
{
public:
  TimeClient(EventLoop *loop, const InetAddress &serverAddr)
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

  void mysend(muduo::net::TcpConnection *conn, const muduo::StringPiece &message)
  {
    muduo::net::Buffer buf;
    buf.append(message.data(), message.size());
    int32_t len = static_cast<int32_t>(message.size());
    int32_t be32 = muduo::net::sockets::hostToNetwork32(len);
    buf.prepend(&be32, sizeof be32);
    conn->send(&buf);
  }

  void write(const StringPiece &message)
  {
    MutexLockGuard lock(mutex_);
    if (connection_)
    {
      mysend(get_pointer(connection_), message);
    }
  }

  void disconnect()
  {
    client_.disconnect();
  }

private:
  EventLoop *loop_;
  TcpClient client_;
  MutexLock mutex_;
  TcpConnectionPtr connection_;

  void onConnection(const TcpConnectionPtr &conn)
  {
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");

    if (!conn->connected())
    {
      loop_->quit();
    }
    else
    {
      connection_ = conn;
    }
  }

  void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp receiveTime)
  {
    std::cout << buf->readableBytes() << std::endl;
    if (buf->readableBytes() >= 4) // kHeaderLen == 4
    {

      const void *data = buf->peek();
      int32_t be32 = *static_cast<const int32_t *>(data); // SIGBUS
      const int32_t len = muduo::net::sockets::networkToHost32(be32);
      std::cout << "[msg len] " << len << std::endl;

      buf->retrieve(4);
      muduo::string message(buf->peek(), len);
      buf->retrieve(len);
      std::cout << "[msg ] " << message << std::endl;
    }
  }
};

int main(int argc, char *argv[])
{
  LOG_INFO << "pid = " << getpid();
  if (argc > 1)
  {
    EventLoopThread loopThread;
    InetAddress serverAddr(argv[1], 7001);

    TimeClient timeClient(loopThread.startLoop(), serverAddr);
    timeClient.connect();
    //loop.loop();

    std::string line;
    while (std::getline(std::cin, line))
    {

      timeClient.write(line);
    }
    timeClient.disconnect();
  }
  else
  {
    printf("Usage: %s host_ip\n", argv[0]);
  }
}
