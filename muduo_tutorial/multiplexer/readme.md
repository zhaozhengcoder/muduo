# multiplexer 接入服务

启动三个终端
```
# 第一个终端，作为backend
nc -l 9999

# 第二个终端，作为接入服务
./multiplexer  

# 第三个终端，作为client
nc 127.0.0.1:3333 

```