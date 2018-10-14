
#include <assert.h>
#include <strings.h>  // bzero
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

Socket::Socket(int sockfd):_sockfd(sockfd)
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