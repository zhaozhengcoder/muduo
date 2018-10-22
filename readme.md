# MUDUO 

学习muduo源码的仓库


* 安装

    https://github.com/chenshuo/muduo 在这里拉下代码。

    第一步，安装cmake和boost
    ```
    sudo apt-get install cmake

    # 安装boost
    sudo apt-get install libboost-dev libboost-test-dev
    ```

    第二步
    ```
    cd muduo
    ./build.sh -j2
    ./build.sh install
    
    #release 版本
    BUILD_TYPE = release ./build.sh -j2
    BUILD_TYPE = release ./build.sh install
    ```


* 目录结构
    ```
    .
    ├── build                               # 编译之后产生的例子，头文件和静态库
    ├── muduo-master                        # 这个是muduo的源码
    ├── muduo_tutorial                      # 自己学习源码的过程中，写的一些代码
    └── readme.md
    ```


**最近正在学习muduo中，学习中遇到的问题和收获，后面慢慢更新**

* 前置技能，看这本书之前，应该熟悉socket基本的api和linux下c++的编程。

    这里有一个ttcp的例子，是用原生的socket api 写的。（里面还有一个nc的例子，也可以看看）

    https://github.com/zhaozhengcoder/muduo/tree/master/muduo_tutorial/ttcp

* 在看源码之前，看几个小demo，熟悉一下muduo的核心代码的逻辑。

    目录：

    1. [muduo的reator的核心结构](#muduo的reator的核心结构)

    2. [muduo的定时器的实现](#muduo的定时器的实现)

    3. muduo的tcp的网络库的实现

    4. muduo的io模型

### muduo的reator的核心结构

主要分成三个类EventLoop，Channel，和Poller类。 EventLoop类的数据成员包括Poller对象，Channel类对象的初始化的时候，需要EventLoop对象。

**EventLoop类**基本上什么都不做，只负责提供一个loop函数，这里面是一个死循环。但是，EventLoop类提供了很多函数，来保证一个线程最多拥有一个loop对象。

**Channel类**，每个channel对象始终只属于一个eventloop，每个channel对象从始至终只负责一个文件描述符fd的io事件的分发，但是它不拥有这个fd，也不会析构的时候关闭这个fd。

**Poller类**是io多路复用的封装。

一个demo的实例：

https://github.com/zhaozhengcoder/muduo/tree/master/example/recipes-master/reactor/s01


### muduo的定时器的实现

muduo定时器的定时的功能是给一个fd，设置一个在多少秒之后可读的一个提醒，来实现tiemr的。

```
// 定时的主要逻辑功能实在这里实现的，通过设计一个fd在多少时间之后可读，来实现定时
void resetTimerfd(int timerfd, Timestamp expiration)
{
    // wake up loop by timerfd_settime()
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue, sizeof newValue);
    bzero(&oldValue, sizeof oldValue);
    newValue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    if (ret)
    {
        LOG_SYSERR << "timerfd_settime()";
    }
}
```

具体细节，可以看一个demo ：

!(链接)[https://github.com/zhaozhengcoder/muduo/tree/master/example/recipes-master/reactor/s02]   下面test4.cc 

