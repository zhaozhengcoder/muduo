#pragma once

#include "socket.h"
#include "Acceptor.h"
#include "TcpStream.h"
#include <sys/socket.h>
#include <memory>

class InetAddress;
class TcpStream;
typedef std::unique_ptr<TcpStream> TcpStreamPtr;

class Acceptor
{
public:
    Acceptor(const InetAddress &listenAddr):listenSock_(Socket::createTCP())
    {
        listenSock_.setReuseAddr(true);
        listenSock_.bindOrDie(listenAddr);
        listenSock_.listenOrDie();
    }

    TcpStreamPtr accept()
    {
        // 接受的fd
        int sockfd = ::accept(listenSock_.fd(),NULL,NULL);
        if(sockfd >= 0)
        {
            return TcpStreamPtr(new TcpStream(Socket(sockfd)));
        }
    }

private:
    Socket listenSock_;
};
