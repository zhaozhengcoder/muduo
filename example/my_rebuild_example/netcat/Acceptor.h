#pragma once


class Acceptor
{
public:
    Acceptor(const InetAddress &listenAddr)
    {
        listenSock_.setReuseAddr(true);
        listenSock_.bindOrDie(listenAddr);
        listenSock_.listenOrDie();
    }
private:
    Socket listenSock_;
};
