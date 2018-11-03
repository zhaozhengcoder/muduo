# muduo 的 eventloop 实现思路

在s01的例子里面有有三个类，EventLoop , Poller , Channel 类。

eventloop对象
1. 拥有一个唯一的poller指针，
2. 拥有一个vector的channel指针

poller对象：
1. 拥有一个Evenloop的指针对象（也是唯一的）
2. 拥有一个vector的channel的指针

channel对象
1. 一个channel对象表示一个fd ，表示要关注的事件，同时也给fd事件设置了回调

