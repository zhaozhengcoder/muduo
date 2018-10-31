# 实现muduo里面的chatserver的例子


这个是利用muduo实现一个应用层的广播。另外，就是加上了**tcp的分包和拆包机制**。

client1.cc 和 server1.cc 是一个半成品，理解起来更快一点。

client2.cc 和 server2.cc 是写完成的东西。


---

## 收获

* 拆包的时候，有一个写法很好 
```
const void *data = buf->peek();  //拿到的数据
int32_t be32 = *static_cast<const int32_t *>(data);  //获得头部的int
```