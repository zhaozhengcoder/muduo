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
