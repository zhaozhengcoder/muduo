#pragma once
#include "socket.h"
#include <memory>

class TcpStream;
typedef std::unique_ptr<TcpStream> TcpStreamPtr;

class TcpStream
{
public:
    TcpStream(Socket && sock):sock_(std::move(sock)){}

    // 
    static TcpStreamPtr connect(const InetAddress& serverAddr);

    // NOT thread safe
    int receiveAll(void* buf, int len);  // read len bytes, unless error happens
    int receiveSome(void* buf, int len); // read len or less bytes

    int sendAll(const void* buf, int len);  // send len bytes, unless error happens
    int sendSome(const void* buf, int len); // send len or less bytes

    // thread safe
    void setTcpNoDelay(bool on);
    void shutdownWrite();

private:
    static TcpStreamPtr connectInternal(const InetAddress& serverAddr, const InetAddress* localAddr);
    Socket sock_;         //这个socket类表示监听到的fd
};