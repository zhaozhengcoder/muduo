#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <string>
#include <iostream>
#include <map>

//using namespace std;
using namespace muduo;
using namespace muduo::net;

std::map<std::string, std::string> users;

std::string getUser(const std::string & user)
{
    std::string result = "no such user";
    std::map<std::string, std::string>::iterator it = users.find(user);

    if(it != users.end())
    {
        result = it->second;
    }
    return result;
}

void onConnection(const TcpConnectionPtr & conn)
{
    if(conn->connected())
    {
        conn->shutdown();
    }
}


void onMessage(const TcpConnectionPtr & conn, Buffer * buf, Timestamp receiveTime)
{
    // if(buf->findCRLF())
    // {
    //     std::cout<<"shutdown"<<std::endl;
    //     conn->shutdown();
    // }
    const char * crlf = buf->findCRLF();
    if(crlf)
    {
        std::string user(buf->peek(),crlf);
        conn->send(getUser(user) + "\r\n");
        buf->retrieveUntil(crlf + 2);
        conn->shutdown();
    }
}

int main()
{
    users["zhao"] = "happy and well";

    EventLoop loop;
    TcpServer server(&loop, InetAddress(6666), "Finger");
    //server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();
    loop.loop();


    return 0;
}