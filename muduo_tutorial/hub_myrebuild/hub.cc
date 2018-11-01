#include "codec.h"
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <boost/bind.hpp>
#include <map>
#include <set>
#include <stdio.h>
#include <iostream>

using namespace muduo;
using namespace muduo::net;

typedef std::set<string> ConnectionSubscription;

class Topic
{
  private:
    std::set<TcpConnectionPtr> audiences_;
    string topic_;
    Timestamp lastPubTime_;

  public:
    Topic(string topic)
    {
        topic_ = topic;
    }

    void publish(string content, Timestamp time)
    {
        lastPubTime_ = time;
        string message = "pub " + topic_ + "\r\n" + content + "\r\n";
        for (std::set<TcpConnectionPtr>::iterator iter = audiences_.begin(); iter != audiences_.end(); iter++)
        {
            (*iter)->send(message);
            std::cout<<"[debug ] pub send "<<message<<std::endl;
        }
    }

    void add(const TcpConnectionPtr &conn)
    {
        audiences_.insert(conn);
        // 这里不理解  
        // muduo 的设计就是这样，对于一个新来的订阅者，给他们发一条消息（原来的代码是把上一条消息发给新来的订阅者）
        string message = "pub " + topic_ + "\r\n" + "hello " + "\r\n";
        conn->send(message);
    }

    void remove(const TcpConnectionPtr &conn)
    {
        audiences_.erase(conn);
    }
};

class PubSubServer
{
  public:
    PubSubServer(EventLoop *loop, const muduo::net::InetAddress &listenAddr) : loop_(loop), server_(loop, listenAddr, "PubSubServer")
    {
        server_.setConnectionCallback(boost::bind(&PubSubServer::onConnection, this, _1));
        server_.setMessageCallback(boost::bind(&PubSubServer::onMessage, this, _1, _2, _3));
        // loop_->runEvery(1.0, boost::bind(&PubSubServer::timePublish, this));
    }

    void start()
    {
        server_.start();
    }

  private:
    // 根据topic的name获得topic对象，如果不存在，就创建一个对象
    Topic &getTopic(const string &topic)
    {
        std::map<string, Topic>::iterator it = topics_.find(topic);
        if (it == topics_.end())
        {
            it = topics_.insert(make_pair(topic, Topic(topic))).first;
        }
        return it->second;
    }

    void doPublish(string name, string topic, string content, Timestamp receiveTime)
    {
        //根据topic的name获得topic对象
        getTopic(name).publish(content, receiveTime);
    }

    void doSubscribe(const TcpConnectionPtr &conn, string name)
    {
        ConnectionSubscription *connSub = boost::any_cast<ConnectionSubscription>(conn->getMutableContext());
        connSub->insert(name);
        getTopic(name).add(conn);
    }

    void doUnsubscribe(const TcpConnectionPtr &conn, string name)
    {
        getTopic(name).remove(conn);
        ConnectionSubscription *connSub = boost::any_cast<ConnectionSubscription>(conn->getMutableContext());
        connSub->erase(name);
    }

    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            conn->setContext(ConnectionSubscription());
        }
        else
        {
            // const ConnectionSubscription &connSub = boost::any_cast<const ConnectionSubscription &>(conn->getContext());
            // // subtle: doUnsubscribe will erase *it, so increase before calling.
            // std::cout << "debug size : " << connSub.size() << std::endl;
            // // 一个连接 也会有多个topic的情况
            // for (ConnectionSubscription::const_iterator it = connSub.begin(); it != connSub.end();)
            // {
            //     std::cout << "debug : " << *it << std::endl;
            //     doUnsubscribe(conn, *it++); //*it++ 表示的是topic ，是一个string
            // }
        }
    }

    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp receiveTime)
    {
        // 判断一下是sub，还是pub，unsub
        pubsub::ParseResult result = pubsub::kSuccess;
        while (result == pubsub::kSuccess)
        {
            string cmd;
            string topic;
            string content;
            result = pubsub::parseMessage(buf, &cmd, &topic, &content);

            if (result == pubsub::kSuccess)
            {
                if (cmd == "pub")
                {
                    //发布
                    doPublish(conn->name(), topic, content, receiveTime);
                }
                else if (cmd == "sub")
                {
                    //订阅
                    LOG_INFO << conn->name() << " subscribes " << topic;
                    doSubscribe(conn, topic);
                }
                else if (cmd == "unsub")
                {
                    //取消订阅
                    doUnsubscribe(conn, topic);
                }
                else
                {
                    //关闭这个连接
                    conn->shutdown();
                    result = pubsub::kError;
                }
            }
            else if (result == pubsub::kError)
            {
                conn->shutdown();
            }
        }
    }

    EventLoop *loop_;
    TcpServer server_;
    std::map<string, Topic> topics_;
};

int main()
{
    EventLoop loop;
    uint16_t port = 6666;

    PubSubServer server(&loop, InetAddress(port));
    server.start();
    loop.loop();

    return 0;
}