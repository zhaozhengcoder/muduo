#include <iostream>
#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpServer.h>
#include <stdio.h>
#include <unistd.h>
#include <utility>
#include <boost/bind.hpp>
#include <string.h>
#include <set>
#include <memory>

//using namespace std;   // 不能这样，如果这样的化 string 会报错
using namespace muduo;
using namespace muduo::net;

// 给msg前面加上一个int的头部，表示消息的长度 -> tcp 分包
// std::shared_ptr<char> make_msg_buff(string msg)
// {
//     int size = static_cast<int>(msg.size());
//     int total_size = size + static_cast<int>(sizeof(int));

//     std::shared_ptr<char> ptr(new char[total_size]);
//     char *p = ptr.get();

//     memcpy(p, &size, size);
//     memcpy(p + size, msg.c_str(), msg.size());
//     return ptr;
// }

class EchoServer
{
  public:
    EchoServer(EventLoop *loop, const InetAddress &listenAddr)
        : loop_(loop), server_(loop, listenAddr, "EchoServer")
    {
        server_.setConnectionCallback(
            boost::bind(&EchoServer::onConnection, this, _1));
        server_.setMessageCallback(
            boost::bind(&EchoServer::onMessage, this, _1, _2, _3));
    }
    void start() { server_.start(); }

  private:
    void onConnection(const TcpConnectionPtr &conn);
    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time);
    EventLoop *loop_;
    TcpServer server_;
    std::set<TcpConnectionPtr> connections_;
};

void EchoServer::onConnection(const TcpConnectionPtr &conn)
{
    std::cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << std::endl;
    connections_.insert(conn);
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

void EchoServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
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

        for (auto iter = connections_.begin(); iter != connections_.end(); iter++)
        {
            mysend(get_pointer(*iter), message);
        }
    }
}

int main(int argc, char *argv[])
{
    EventLoop loop;
    InetAddress listenAddr(7001);
    EchoServer server(&loop, listenAddr);

    server.start();

    loop.loop();
}
