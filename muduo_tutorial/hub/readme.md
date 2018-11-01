# hub 


## 使用

```
./hub 9999                                      #hub进程，提供转发

./sub 127.0.0.1:9999 mytopic                    #订阅
./pub 127.0.0.1:9999 mytopic "hello world"      #发布
```