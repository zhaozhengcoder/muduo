#pragma once

#include "Common.h"
#include <string>
#include <vector>
#include <netinet/in.h>

class InetAddress
{
    public:
        InetAddress(int port)
        {
            saddr_.sin_family = AF_INET;
            saddr_.sin_addr.s_addr = INADDR_ANY;
            saddr_.sin_port = htons(port);
        }

        InetAddress(const struct sockaddr_in& saddr):saddr_(saddr)
        {}

        bool operator==(const InetAddress& rhs) const
        {
            return saddr_.sin_family == rhs.saddr_.sin_family
                && ipNetEndian() == rhs.ipNetEndian()
                && portNetEndian() == rhs.portNetEndian();
        }

        const struct sockaddr_in& getSockAddrInet() const
        {
            return saddr_;
        }

        uint32_t ipNetEndian() const { return saddr_.sin_addr.s_addr; }
        uint16_t portNetEndian() const { return saddr_.sin_port; }
        uint32_t ipHostEndian() const { return ntohl(saddr_.sin_addr.s_addr); }
        uint16_t portHostEndian() const { return ntohs(saddr_.sin_port); }

        static bool resolve(StringArg hostname, InetAddress*);
        std::string toIp() const;
        std::string toIpPort() const;
    private:
        struct sockaddr_in saddr_;
        static bool resolveSlow(const char* hostname, InetAddress*);
};