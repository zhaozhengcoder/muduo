#include "Channel.h"
#include "EventLoop.h"

#include <stdio.h>
#include <sys/timerfd.h>

muduo::EventLoop *g_loop;

void timeout()
{
    printf("Timeout!\n");
    g_loop->quit();
}

int main()
{
    // 创建一个loop的对象，loop的对象每个线程最多拥有一个loop对象，loop对象里面有一个死循环 loop.loop() 函数
    // loop对象在初始化的时候，自己的数据成员有一个是Poller类的对象，在这个时候完成了初始化
    muduo::EventLoop loop;
    g_loop = &loop;

    // 创建一个fd测试
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

    // chanel 需要loop对象和fd
    muduo::Channel channel(&loop, timerfd);
    channel.setReadCallback(timeout);

    // 这个里面调用了updateChannel函数
    // 通过这个enablereading，将自己注册到poller对象里面
    channel.enableReading();

    // 设置fd的属性
    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    // loop.loop
    loop.loop();

    ::close(timerfd);
}
