#include <iostream>
using namespace std;

int main(int argc,char ** argv)
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