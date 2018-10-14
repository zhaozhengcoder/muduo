#pragma once

#include "Common.h"
#include <utility>  // swap

class InetAddress;

class Socket
{
private:
    int _sockfd;
public:
    explicit Socket(int sockfd);
    ~Socket();

    // 移动构造函数
    Socket(Socket && rhs):Socket(rhs._sockfd)
    {
        rhs._sockfd = -1;
    }

    Socket& operator=(Socket&& rhs)
    {
        swap(rhs);
        return *this;
    }

    void swap(Socket& rhs)
    {
        std::swap(_sockfd, rhs._sockfd);
    }

    // socket api 
    void bindOrDie(const InetAddress& addr);
    void listenOrDie();
    // return 0 on success
    int connect(const InetAddress& addr);
    void shutdownWrite();

    void setReuseAddr(bool on);
    void setTcpNoDelay(bool on);  

    InetAddress getLocalAddr() const;
    InetAddress getPeerAddr() const;

    int read(void* buf, int len);
    int write(const void* buf, int len);  

    // factory methods
    static Socket createTCP();
    static Socket createUDP();    
};