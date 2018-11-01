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
std::shared_ptr<char> make_msg_buff(string msg)
{
    int size = static_cast<int>(msg.size());
    int total_size = size + static_cast<int>(sizeof(int));

    std::shared_ptr<char> ptr(new char[total_size]);
    char *p = ptr.get();

    memcpy(p, &size, size);
    memcpy(p + size, msg.c_str(), msg.size());
    return ptr;
}

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
    string msg(buf->retrieveAllAsString());
    std::cout << conn->name() << " recv " << msg.size() << " bytes at " << time.toString() << std::endl;
    std::cout << msg << std::endl;

    for (auto iter = connections_.begin(); iter != connections_.end(); iter++)
    {
        // std::shared_ptr<char> buff = make_msg_buff(msg);
        // int buff_len = static_cast<int>(msg.size()) + static_cast<int>(sizeof(int));
        // void * p_buff = static_cast<void *>(buff.get());
        // std::cout<<"send size : "<<buff_len<<std::endl;
        // (*iter)->send(p_buff, buff_len);
        mysend(get_pointer(*iter), msg);
    }
    //conn->send(msg);
}

int main(int argc, char *argv[])
{
    EventLoop loop;
    InetAddress listenAddr(7001);
    EchoServer server(&loop, listenAddr);

    server.start();

    loop.loop();
}
