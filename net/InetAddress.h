#ifndef NET_INETADDRESS_H
#define NET_INETADDRESS_H
#include "../base/noncopyable.h"
// #include "../base/StringPiece.h"

#include <netinet/in.h>
#include <string>

namespace muduo
{
namespace net
{
namespace sockets
{
const struct sockaddr * sockaddr_cast(const struct sockaddr_in6 *addr);
}

// class copyable
// {
//  protected:
//   copyable() = default;
//   ~copyable() = default;
// };

class InetAddress //: public copyable
{
    public:
        explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);

        //InetAddress(std::string ip, uint16_t port, bool ipv6 = false);

        explicit InetAddress(const struct sockaddr_in& addr)
            :   addr_(addr)
        { }

        explicit InetAddress(const struct sockaddr_in6 & addr)
            :   addr6_(addr)
        { }

        sa_family_t family() const { return addr_.sin_family; }
        std::string toIp() const;
        std::string toIpPort() const;
        uint16_t port() const;

        const struct sockaddr * getSockAddr() const { return sockets::sockaddr_cast(&addr6_); }
        void setSockAddrInet6(const struct sockaddr_in6& addr6) { addr6_ = addr6; }

        uint32_t ipv4NetEndian() const;
        uint16_t portNetEndian() const { return addr_.sin_port; }

        // resolve hostname to IP address, not changing port or sin_family
        // return true on success.
        // thread safe
        static bool resolve(std::string hostname, InetAddress* result);
        // static std::vector<InetAddress> resolveAll(const char* hostname, uint16_t port = 0);

        // set IPv6 ScopeID
        void setScopeId(uint32_t scope_id);

    private:
        union 
        {
            struct sockaddr_in addr_;
            struct sockaddr_in6 addr6_;
        };
        
};

}
} // namespace muduo

#endif