#include "pubsub.h"
#include <muduo/base/ProcessInfo.h>
#include <muduo/net/EventLoop.h>

#include <boost/bind.hpp>
#include <vector>
#include <stdio.h>
#include <iostream>

using namespace muduo;
using namespace muduo::net;

EventLoop *g_loop = NULL;
std::vector<string> g_topics; 

void subscription(const string &topic, const string &content, Timestamp)
{
    printf("%s: %s\n", topic.c_str(), content.c_str());
}

void connection(PubSubClient *client)
{
    if (client->connected())
    {
        for (std::vector<string>::iterator it = g_topics.begin(); it != g_topics.end(); ++it)
        {
            client->subscribe(*it, subscription);
        }
    }
    else
    {
        g_loop->quit();
    }
}

int main()
{
    g_topics.push_back("mytopic");
    g_topics.push_back("mytopic2");


    EventLoop loop;
    g_loop = &loop;

    string name = ProcessInfo::pidString();
    std::cout<<"sub name : "<<name<<std::endl;

    string hostip = "127.0.0.1";
    uint16_t port = 6667;
    PubSubClient client(&loop, InetAddress(hostip, port), name);
    client.setConnectionCallback(connection);
    client.start();
    loop.loop();
}