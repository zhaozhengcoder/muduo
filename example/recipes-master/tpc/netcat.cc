#include "Acceptor.h"
#include "InetAddress.h"
#include "TcpStream.h"
#include <thread>
#include <string.h>
#include <unistd.h>

#include <stdio.h>
#include <iostream>
using namespace std;

int write_n(int fd, const void* buf, int length)
{
  int written = 0;
  while (written < length)
  {
    int nw = ::write(fd, static_cast<const char*>(buf) + written, length - written);
    if (nw > 0)
    {
      written += nw;
    }
    else if (nw == 0)
    {
      break;  // EOF
    }
    else if (errno != EINTR)
    {
      perror("write");
      break;
    }
  }
  return written;
}

void run(TcpStreamPtr stream)
{
  // Caution: a bad example for closing connection
  // 创建一个子线程 执行里面的逻辑
  std::thread thr([&stream] () {
    char buf[8192];
    int nr = 0;

    // 如果nr的返回值是0，那么退出循环
    while ((nr = stream->receiveSome(buf, sizeof(buf))) > 0)
    {
      int nw = write_n(STDOUT_FILENO, buf, nr);
      // 跳出循环 ，error 收发不一致
      if (nw < nr)
      {
        cout<<"[error] [child thread]: read > write "<<endl;
        break;
      }
    }
    ::exit(0);  // should somehow notify main thread instead
  });

  // 主线程执行下面的逻辑 ,从标准输入读取，然后发送到socket上面
  // 如果读一个文件，读到文件结尾的时候，会出现read的返回值等于0，然后跳出while
  char buf[8192];
  int nr = 0;
  while ( (nr = ::read(STDIN_FILENO, buf, sizeof(buf))) > 0)
  {
    int nw = stream->sendAll(buf, nr);
    if (nw < nr)
    {
       cout<<"[error] [main thread]: read > write "<<endl;
      break;
    }
  }

  //tcp 关闭 （调用shutdown是正确的tcp关闭的姿势）
  //如果调用close 的话，主动方就会直接发送一个rst报文给被动方，然后被动方如果调用read的话，返回值就是-1，错误是 Connection reset by peer
  stream->shutdownWrite();
  thr.join();
}

int main(int argc, const char* argv[])
{
  if (argc < 3)
  {
    printf("Usage:\n  %s hostname port\n  %s -l port\n", argv[0], argv[0]);
    return 0;
  }

  int port = atoi(argv[2]);
  if (strcmp(argv[1], "-l") == 0)
  {
    // 接受tcp连接
    std::unique_ptr<Acceptor> acceptor(new Acceptor(InetAddress(port)));
    TcpStreamPtr stream(acceptor->accept());

    // 收到连接
    if (stream)
    {
      acceptor.reset();  // stop listening  //reset 表示 释放该对象
      //因为tcpstream类，没有提供拷贝构造函数，所以，必须使用move(stream)的方式
      run(std::move(stream)); 
    }
    else
    {
      perror("accept");
    }
  }
  else
  {
    InetAddress addr(port);
    const char* hostname = argv[1];
    if (InetAddress::resolve(hostname, &addr))
    {
      // 建立tcp连接
      TcpStreamPtr stream(TcpStream::connect(addr));
      if (stream)
      {
        run(std::move(stream));
      }
      else
      {
        printf("Unable to connect %s\n", addr.toIpPort().c_str());
        perror("");
      }
    }
    else
    {
      printf("Unable to resolve %s\n", hostname);
    }
  }
}
