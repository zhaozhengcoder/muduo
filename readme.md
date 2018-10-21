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


* muduo的reator的核心结构

    主要分成三个类EventLoop，Channel，和Poller类。 EventLoop类的数据成员包括Poller对象，Channel类对象的初始化的时候，需要EventLoop对象。

    **EventLoop类**基本上什么都不做，只负责提供一个loop函数，这里面是一个死循环。但是，EventLoop类提供了很多函数，来保证一个线程最多拥有一个loop对象。

    **Channel类**，每个channel对象始终只属于一个eventloop，每个channel对象从始至终只负责一个文件描述符fd的io事件的分发，但是它不拥有这个fd，也不会析构的时候关闭这个fd。

    **Poller类**是io多路复用的封装。

    这是一个例子：
    https://github.com/zhaozhengcoder/muduo/tree/master/example/recipes-master/reactor/s01

    ```
    void timeout()
    {
        printf("Timeout!\n");
        g_loop->quit();
    }

    int main()
    {
        muduo::EventLoop loop;
        g_loop = &loop;             // 创建一个loop的对象，loop的对象每个线程最多拥有一个loop对象，loop对象里面有一个死循环 loop.loop() 函数

        //创建一个fd 测试
        int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
        
        //chanel 需要loop对象和fd
        muduo::Channel channel(&loop, timerfd);
        channel.setReadCallback(timeout);
        
        //这个里面调用了updateChannel函数
        channel.enableReading();

        //设置fd的属性
        struct itimerspec howlong;
        bzero(&howlong, sizeof howlong);
        howlong.it_value.tv_sec = 5;
        ::timerfd_settime(timerfd, 0, &howlong, NULL);

        //loop.loop
        loop.loop();

        ::close(timerfd);
    }

    ```