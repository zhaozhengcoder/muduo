#include "pubsub.h"
#include <muduo/base/ProcessInfo.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>

#include <iostream>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;
using namespace pubsub;

EventLoop *g_loop = NULL;
string g_topic;
string g_content;

void connection(PubSubClient *client)
{
    if (client->connected())
    {
        client->publish(g_topic, g_content);
        client->stop();
    }
    else
    {
        g_loop->quit();
    }
}

int main(int argc, char *argv[])
{
    if (argc == 4)
    {
        string hostport = argv[1];
        size_t colon = hostport.find(':');
        if (colon != string::npos)
        {
            string hostip = hostport.substr(0, colon);
            uint16_t port = static_cast<uint16_t>(atoi(hostport.c_str() + colon + 1));
            g_topic = argv[2];
            g_content = argv[3];

            string name = ProcessInfo::username() + "@" + ProcessInfo::hostname();
            name += ":" + ProcessInfo::pidString();

            if (g_content == "-")
            {
                std::cout<<"branch 1"<<std::endl;
                //一个线程负责loop
                EventLoopThread loopThread;
                g_loop = loopThread.startLoop();
                PubSubClient client(g_loop, InetAddress(hostip, port), name);
                client.start();

                //主线程负责处理输入
                string line;
                while (getline(std::cin, line))
                {
                    client.publish(g_topic, line);
                }
                client.stop();
                CurrentThread::sleepUsec(1000 * 1000);
            }
            else
            {
                std::cout<<"branch 2"<<std::endl;
                EventLoop loop;
                g_loop = &loop;
                PubSubClient client(g_loop, InetAddress(hostip, port), name);
                // 传递一个回调函数
                client.setConnectionCallback(connection);
                client.start();
                loop.loop();
            }
        }
        else
        {
            printf("Usage: %s hub_ip:port topic content\n", argv[0]);
        }
    }
    else
    {
        printf("Usage: %s hub_ip:port topic content\n"
               "Read contents from stdin:\n"
               "  %s hub_ip:port topic -\n",argv[0], argv[0]);
    }
}
