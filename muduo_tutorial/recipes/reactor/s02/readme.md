# s02 的例子里面添加了定时器的实现 

定时器的思想是依赖于一个根据时间排序的队列，队列的首部是最早要执行的时间。
另外一个成员timerfd，记录了第一个事件要执行的时间。 等到了这个时间，这个timerfd会变得可读。于是这个timerfd也可以看作是一个channel的类的对象。


核心时间的代码：
```
void handleRead();
void reset(xxxx);
bool insert(xxxx);

const int timerfd;
```