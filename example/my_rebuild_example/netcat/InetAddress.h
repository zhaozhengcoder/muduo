#pragma once
include <netinet/in.h>

class InetAddress
{
    public:
        InetAddress(int port)
        {
            saddr_.sin_family = AF_INET;
            saddr_.sin_addr.s_addr = INADDR_ANY;
            saddr_.sin_port = htons(port);
        }

    private:
        struct sockaddr_in saddr_;
};