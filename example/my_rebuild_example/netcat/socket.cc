#include "Socket.h"
#include "InetAddress.h"

#include <stdio.h>
#include <assert.h>
#include <strings.h>  // bzero
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <stdlib.h>

// 匿名的namespace ，取消在文件外的可见性，约等于在前面加上 static
namespace
{
  typedef struct sockaddr SA;
  // 下面两个函数，想做的事情就是 sockaddr_in 到 sockaddr 的类型转化 （使用c++的方式）
  const SA* sockaddr_cast(const struct sockaddr_in* addr)
  {
    return static_cast<const SA*>(static_cast<const void *>(addr));
    //return static_cast<const SA*>(implicit_cast<const void*>(addr));          // 下面这行是原来的代码，上面的一行是我修改之后的代码
  }

  SA* sockaddr_cast(struct sockaddr_in* addr)
  {
    return static_cast<SA*>(static_cast<void *>(addr));
    //return static_cast<SA*>(implicit_cast<void*>(addr));                     // 下面这行是原来的代码，上面的一行是我修改之后的代码
  }
}

Socket::Socket(int sockfd)
  : sockfd_(sockfd)
{
  assert(sockfd_ >= 0);
}

Socket::~Socket()
{
  if (sockfd_ >= 0)
  {
    int ret = ::close(sockfd_);
    assert(ret == 0); (void)ret;
  }
}

void Socket::bindOrDie(const InetAddress& addr)
{
  const struct sockaddr_in& saddr = addr.getSockAddrInet();
  int ret = ::bind(sockfd_, sockaddr_cast(&saddr), sizeof saddr);
  if (ret)
  {
    perror("Socket::bindOrDie");
    abort();
  }
}

void Socket::listenOrDie()
{
  int ret = ::listen(sockfd_, SOMAXCONN);   // 内核参数 ，定义了系统中每一个端口最大的监听队列的长度
  if (ret)
  {
    perror("Socket::listen");
    abort();
  }
}

int Socket::connect(const InetAddress& addr)
{
  const struct sockaddr_in& saddr = addr.getSockAddrInet();
  return ::connect(sockfd_, sockaddr_cast(&saddr), sizeof saddr);
}

// shutdown 函数
void Socket::shutdownWrite()
{
  if (::shutdown(sockfd_, SHUT_WR) < 0)
  {
    perror("Socket::shutdownWrite");
  }
  /*
  int shutdown(int sockfd, int how);
  a) SHUT_RD，关闭连接的读这一半，套接字中不再有数据可接收，而且套接字接收缓冲区中的现有数据都被丢弃，进程不能再对这样的套接字调用任何读函数。该套接字接收的来自对端的任何数据都被确认，然后悄然丢弃。
  b) SHUT_WR，关闭连接的写这一半，当前留在套接字发送缓冲区的数据都将被发送掉，后跟TCP的正常连接终止序列，进程不能再对这样的套接字调用任何写函数。
  c) SHUT_RDWR，连接的读半部与写半部都关闭。
  */
}


void Socket::setReuseAddr(bool on)
{
  int optval = on ? 1 : 0;
  if (::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
                   &optval, sizeof optval) < 0)
  {
    perror("Socket::setReuseAddr");
  }
}

void Socket::setTcpNoDelay(bool on)
{
  int optval = on ? 1 : 0;
  if (::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, 
    &optval, static_cast<socklen_t>(sizeof optval)) < 0)
  {
    perror("Socket::setTcpNoDelay");
  }
}

InetAddress Socket::getLocalAddr() const
{
  struct sockaddr_in localaddr;
  bzero(&localaddr, sizeof localaddr);
  socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
  if (::getsockname(sockfd_, sockaddr_cast(&localaddr), &addrlen) < 0)
  {
    perror("Socket::getLocalAddr");
  }
  return InetAddress(localaddr);
}

InetAddress Socket::getPeerAddr() const
{
  struct sockaddr_in peeraddr;
  bzero(&peeraddr, sizeof peeraddr);
  socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
  if (::getpeername(sockfd_, sockaddr_cast(&peeraddr), &addrlen) < 0)
  {
    perror("Socket::getPeerAddr");
  }
  return InetAddress(peeraddr);
}

int Socket::read(void* buf, int len)
{
  // FIXME: EINTR
  return ::read(sockfd_, buf, len);
}

int Socket::write(const void* buf, int len)
{
  // FIXME: EINTR
  return ::write(sockfd_, buf, len);
}

Socket Socket::createTCP()
{
  int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);
  assert(sockfd >= 0);
  return Socket(sockfd);
}

Socket Socket::createUDP()
{
  int sockfd = ::socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
  assert(sockfd >= 0);
  return Socket(sockfd);
}

