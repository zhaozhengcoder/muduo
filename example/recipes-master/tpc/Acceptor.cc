#include "Acceptor.h"

#include "TcpStream.h"

#include <stdio.h>
#include <sys/socket.h>


// 创建一个socket，并且完成绑定和监听 
Acceptor::Acceptor(const InetAddress& listenAddr)
  : listenSock_(Socket::createTCP())
{
  listenSock_.setReuseAddr(true);
  listenSock_.bindOrDie(listenAddr);
  listenSock_.listenOrDie();
}


// 对于收到建立的socket建立 TcpStream 类，提供read 和write 的方法
TcpStreamPtr Acceptor::accept()
{
  // FIXME: use accept4
  int sockfd = ::accept(listenSock_.fd(), NULL, NULL);
  if (sockfd >= 0)
  {
    return TcpStreamPtr(new TcpStream(Socket(sockfd)));
  }
  else
  {
    perror("Acceptor::accept");
    return TcpStreamPtr();
  }
}

