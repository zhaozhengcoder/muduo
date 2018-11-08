# MUDUO 

这个repo是学习c++ 网路库muduo的。里面的代码基本上是从muduo的仓库上面来下来的，一些个别的代码是为了理解和学习，我自己修改的。



* 安装
    ```
    https://github.com/chenshuo/muduo 在这里拉下代码。

    # 第一步，安装cmake和boost

    sudo apt-get install cmake

    # 安装boost
    sudo apt-get install libboost-dev libboost-test-dev

    # 第二步
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


## 前置技能

* 前置技能，看这本书之前，应该熟悉socket基本的api和linux下c++的编程。

    1. 这里有一个ttcp的例子，是用原生的socket api 写的。

        https://github.com/zhaozhengcoder/muduo/tree/master/muduo_tutorial/ttcp

    2. 这是一个netcat的例子，这个稍微封装了一下。很专业一点。

        https://github.com/zhaozhengcoder/muduo/tree/master/example/my_rebuild_example/netcat


* 在看源码之前，先思考几个问题，如何要自己开发一下网络库，对于下面的问题，如何回答？


    1. [muduo的io模型是什么](##muduo的reator的核心结构)

    2. [muduo的多线程体现在什么地方](#muduo的定时器的实现)

    3. [muduo的主线程accepte的fd如何分发给其他线程？]

    4. [muduo的定时器如何实现？]

    5. [muduo如何限制连接的数量？]

    6. [muduo如何设计buffer？]

    7. [muduo的定时器是如何设计的？]

    8. [如何安全的关闭tcp连接，能不能直接close，如何直接close会发生什么？]

    9. [muduo是如何 线程安全的对 对象的声明周期进行管理？]


* 其他的一些关于收获

    1. 使用shard_ptr 和 weak_ptr 进行资源管理

    2. 使用bind注册回调函数，比用虚函数的方式实现，更舒服。




## muduo的核心框架的实现过程

作者很贴心的提供了一个recipes的目录，把核心框架的实现过程拆分了出来。

连接：https://github.com/zhaozhengcoder/muduo/tree/master/muduo_tutorial/recipes/reactor


**最近正在学习muduo中，学习中遇到的问题和收获，后面慢慢更新。**