#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include <stdio.h>
#include <time.h>


//test7_1.cc echo一个时间
void newConnection(int sockfd, const muduo::InetAddress &peerAddr) 
{
    printf("newConnection(): accepted a new connection from %s\n", peerAddr.toHostPort().c_str());
    
	time_t t;
	struct tm *timeinfo;  //结构体
	time(&t);
    timeinfo = localtime(&t);

    ::write(sockfd, asctime(timeinfo), 13);
    muduo::sockets::close(sockfd);
}

int main() 
{
    printf("main(): pid = %d\n", getpid());

    muduo::InetAddress listenAddr(9981);
    muduo::EventLoop loop;

    // 创建acceptor对象
    muduo::Acceptor acceptor(&loop, listenAddr);
    acceptor.setNewConnectionCallback(newConnection);

    // acceptor里面有一个channel的对象，listen的时候enable了这个channel，如果channel上面有事件了
    // 如果上面有事件可读的时候，会调用它的回调函数
    acceptor.listen();

    // 开始loop
    loop.loop();
}
