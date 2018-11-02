#include "pubsub.h"
#include "codec.h"
#include <boost/bind.hpp>
#include <iostream>

using namespace muduo;
using namespace muduo::net;
using namespace pubsub;


PubSubClient::PubSubClient(muduo::net::EventLoop *loop, const muduo::net::InetAddress &hubAddr, const string &name) : client_(loop, hubAddr, name)
{
    client_.setConnectionCallback(boost::bind(&PubSubClient::onConnection, this, _1));
    client_.setMessageCallback(boost::bind(&PubSubClient::onMessage, this, _1, _2, _3));
}


void PubSubClient::start()
{
    client_.connect();
}

void PubSubClient::stop()
{
    client_.disconnect();
}

void PubSubClient::onConnection(const TcpConnectionPtr &conn)
{
    //如果是建立连接
    //注意，建立链接的时候，是不会主动发送消息的
    if (conn->connected())
    {
        conn_ = conn;
        // FIXME: re-sub
    }
    else
    //如果是断开连接
    {
        conn_.reset();
    }
    //如果设置了回调函数，那么就执行一下
    if (connectionCallback_)
    {
        connectionCallback_(this);
    }
}

void PubSubClient::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp receiveTime)
{
    //如果收到消息
    ParseResult result = kSuccess;
    while (result == kSuccess)
    {
        string cmd;
        string topic;
        string content;
        result = parseMessage(buf, &cmd, &topic, &content);
        if (result == kSuccess)
        {
            std::cout << "cmd : " << cmd << " , topic : " << topic << " , content : " << content << std::endl;
            if (cmd == "pub" && subscribeCallback_)
            {
                std::cout << "exec subscribeCallback_" << std::endl;
                subscribeCallback_(topic, content, receiveTime);
            }
        }
        else if (result == kError)
        {
            conn->shutdown();
        }
    }
}

bool PubSubClient::send(const string &message)
{
    bool succeed = false;
    if (conn_ && conn_->connected())
    {
        conn_->send(message);
        succeed = true;
    }
    return succeed;
}

bool PubSubClient::subscribe(const string &topic, const SubscribeCallback &cb)
{
    string message = "sub " + topic + "\r\n";
    subscribeCallback_ = cb;
    return send(message);
}

bool PubSubClient::connected() const
{
    return conn_ && conn_->connected();
}