#ifndef MUDUO_EXAMPLES_HUB_PUBSUB_H
#define MUDUO_EXAMPLES_HUB_PUBSUB_H

#include <muduo/net/TcpClient.h>

using muduo::string;
using muduo::Timestamp;

class PubSubClient
{
  private:
    muduo::net::TcpClient client_;
    muduo::net::TcpConnectionPtr conn_;

    void onConnection(const muduo::net::TcpConnectionPtr &conn);
    void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp receiveTime);
    bool send(const string &message);

  public:
    typedef boost::function<void(PubSubClient *)> ConnectionCallback;
    typedef boost::function<void(const string &topic, const string &content, Timestamp)> SubscribeCallback;

    PubSubClient(muduo::net::EventLoop *loop, const muduo::net::InetAddress &hubAddr, const string &name);

    void start();
    void stop();
    bool connected() const;
    bool subscribe(const string &topic, const SubscribeCallback &cb);

    void setConnectionCallback(const ConnectionCallback &cb)
    {
        connectionCallback_ = cb;
    }


    ConnectionCallback connectionCallback_;
    SubscribeCallback subscribeCallback_;
};

#endif