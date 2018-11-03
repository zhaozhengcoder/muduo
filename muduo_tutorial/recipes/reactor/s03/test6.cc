#include "EventLoop.h"
#include "EventLoopThread.h"
#include <stdio.h>

void runInThread()
{
    printf("runInThread(): pid = %d, tid = %d\n", getpid(), muduo::CurrentThread::tid());
}

int main()
{
    // 加入主线程的tid是660
    printf("main(): pid = %d, tid = %d\n", getpid(), muduo::CurrentThread::tid());

    // 这EventLoopThread对象里面创建了一个子线程，子线程拥有一个loop对象，并返还给loop指针
    // 因为loop对象是在子线程里面创建的，加入子线程的id是661，那么loop对象里面的id是661
    muduo::EventLoopThread loopThread;
    muduo::EventLoop *loop = loopThread.startLoop();

    //在主线程里面调用，子线程创建的对象，那么这个任务不会立刻执行，会被插入到子线程的loop里面，被子线程执行
    loop->runInLoop(runInThread);
    sleep(1);

    //这个也是同样的道理
    loop->runAfter(2, runInThread);
    sleep(3);
    loop->quit();

    printf("exit main().\n");
}
