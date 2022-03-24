#ifndef NET_INETADDRESS_H
#define NET_INETADDRESS_H

#include <arpa/inet.h>

namespace eff
{
namespace net
{
class InetAddress
{
    public:
        //explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);
        InetAddress()
        { }

        InetAddress(const struct sockaddr_in& addr)
            :   addr_(addr)
        { }

        // sa_family_t getfamily() { return addr_.sin_family; }

        // struct sockaddr_in getaddr() { return addr_; }

        void setaddr(struct sockaddr_in addr) { addr_ = addr; }

        struct sockaddr_in addr_;
};
}
}

#endif