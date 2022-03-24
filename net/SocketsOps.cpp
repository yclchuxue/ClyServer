#include "../base/Logging.h"
#include "../base/Types.h"
#include "SocketsOps.h"
#include "Endian.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace eff;
using namespace eff::net;

namespace
{
typedef struct sockaddr SA;
}

const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in6* addr)
{
    return static_cast<const struct sockaddr*>(static_cast<const void *>(addr));
}

struct sockaddr* sockets::sockaddr_cast(struct sockaddr_in6* addr)
{
    return static_cast<struct sockaddr*>(static_cast<void*>(addr));
}

const struct sockaddr * sockets::sockaddr_cast(const struct sockaddr_in* addr)
{
    return static_cast<const struct sockaddr*>(static_cast<const  void*>(addr));
}

const struct sockaddr_in* sockets::sockaddr_in_cast(const struct sockaddr* addr)
{
    return static_cast<const struct sockaddr_in*>(static_cast<const void*>(addr));
}

const struct sockaddr_in6* sockets::sockaddr_in6_cast(const struct sockaddr* addr)
{
    return static_cast<const struct sockaddr_in6*>(static_cast<const void*>(addr));
}

int sockets::createNonblockingOrDie(sa_family_t family)
{
  //int sockfd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
  int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
  LOG_DEBUG << sockfd;
  if (sockfd < 0)
  {
    LOG_SYSFATAL << "sockets::createNonblockingOrDie";
  }

  return sockfd;
}

void sockets::bindOrDie(int sockfd, const struct sockaddr* addr)
{

    // struct sockaddr_in ser;
    // memset(&ser, 0, sizeof(ser));
    // ser.sin_family = AF_INET;
    // ser.sin_port = htons(6000);                         //端口号
    // ser.sin_addr.s_addr = inet_addr("127.0.0.1");       //ID
    //LOG_DEBUG << "sofd = " << sockfd;
    int ret = ::bind(sockfd, reinterpret_cast<const struct sockaddr*>(addr),
           sizeof(struct sockaddr_in));
    if(ret < 0)
    {
        LOG_SYSFATAL << "sockets::bindOrDie";
    }
}

void sockets::listenOrDie(int sockfd)
{
    LOG_DEBUG << " ";
    int ret = ::listen(sockfd, SOMAXCONN);
    if(ret < 0)
    {
        LOG_SYSFATAL << "sockets::listenOrDie";
    }else
    {
        LOG_DEBUG << "listening";
    }
}

int sockets::accept(int sockfd, struct sockaddr_in * addr)
{
    socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);

    int connfd = ::accept4(sockfd, (struct sockaddr*)&addr,
                         &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);

    if(connfd < 0)
    {
        int savedErrno = errno;
        LOG_SYSERR << "Socket::accept";
        switch (savedErrno)
        {
        case EAGAIN:
        case ECONNABORTED:
        case EINTR:
        case EPROTO:
        case EPERM:
        case EMFILE:
            errno = savedErrno;
            break;
        case EBADF:
        case EFAULT:
        case EINVAL:
        case ENFILE:
        case ENOBUFS:
        case ENOMEM:
        case ENOTSOCK:
        case EOPNOTSUPP:
            LOG_FATAL << "unexpected error of ::accept " << savedErrno;
            break;
        default:
            LOG_FATAL << "unknown err of ::accept " << savedErrno;
            break; 
        }
    }
    return connfd;
}

int sockets::connect(int sockfd, const struct sockaddr * addr)
{
    return ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
}

ssize_t sockets::read(int sockfd, void * buf, size_t count)
{
    return ::read(sockfd, buf, count);
}

ssize_t sockets::readv(int sockfd, const struct iovec * iov, int iovcnt)
{
    return ::readv(sockfd, iov, iovcnt);
}

ssize_t sockets::write(int sockfd, const void * buf, size_t count)
{
    return ::write(sockfd, buf, count);
}

void sockets::close(int sockfd)
{
    if(::close(sockfd) < 0)
    {
        LOG_SYSERR << "sockets::close";
    }
}

void sockets::shutdownWrite(int sockfd)
{
    if(::shutdown(sockfd, SHUT_WR) < 0)
    {
        LOG_SYSERR << "sockets::shutdownWrite";
    }
}

void sockets::toIpPort(char * buf, size_t size, const struct sockaddr * addr)
{
  if (addr->sa_family == AF_INET6)
  {
    buf[0] = '[';
    toIp(buf+1, size-1, addr);
    size_t end = ::strlen(buf);
    const struct sockaddr_in6* addr6 = sockaddr_in6_cast(addr);
    uint16_t port = sockets::networkToHost16(addr6->sin6_port);
    assert(size > end);
    snprintf(buf+end, size-end, "]:%u", port);
    return;
  }
  toIp(buf, size, addr);
  size_t end = ::strlen(buf);
  const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
  uint16_t port = sockets::networkToHost16(addr4->sin_port);
  assert(size > end);
  snprintf(buf+end, size-end, ":%u", port);
}


void sockets::toIp(char* buf, size_t size,
                   const struct sockaddr* addr)
{
  if (addr->sa_family == AF_INET)
  {
    assert(size >= INET_ADDRSTRLEN);
    const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
    ::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
  }
  else if (addr->sa_family == AF_INET6)
  {
    assert(size >= INET6_ADDRSTRLEN);
    const struct sockaddr_in6* addr6 = sockaddr_in6_cast(addr);
    ::inet_ntop(AF_INET6, &addr6->sin6_addr, buf, static_cast<socklen_t>(size));
  }
}

void sockets::fromIpPort(const char* ip, uint16_t port,
                         struct sockaddr_in* addr)
{
  addr->sin_family = AF_INET;
  addr->sin_port = hostToNetwork16(port);
  if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
  {
    LOG_SYSERR << "sockets::fromIpPort";
  }
}

// void sockets::fromIpPort(const char* ip, uint16_t port, struct sockaddr_in6* addr)
// {
//   addr->sin6_family = AF_INET6;
//   addr->sin6_port = hostToNetwork16(port);
//   if (::inet_pton(AF_INET6, ip, &addr->sin6_addr) <= 0)
//   {
//     LOG_SYSERR << "sockets::fromIpPort";
//   }
// }

int sockets::getSocketError(int sockfd)
{
  int optval;
  socklen_t optlen = static_cast<socklen_t>(sizeof optval);

  if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
  {
    return errno;
  }
  else
  {
    return optval;
  }
}

struct sockaddr_in6 sockets::getLocalAddr(int sockfd)
{
  struct sockaddr_in6 localaddr;
  memset(&localaddr, 0, sizeof localaddr);
  socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
  if (::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen) < 0)
  {
    LOG_SYSERR << "sockets::getLocalAddr";
  }
  return localaddr;
}

struct sockaddr_in6 sockets::getPeerAddr(int sockfd)
{
  struct sockaddr_in6 peeraddr;
  memset(&peeraddr, 0, sizeof peeraddr);
  socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
  if (::getpeername(sockfd, sockaddr_cast(&peeraddr), &addrlen) < 0)
  {
    LOG_SYSERR << "sockets::getPeerAddr";
  }
  return peeraddr;
}

bool sockets::isSelfConnect(int sockfd)
{
  struct sockaddr_in6 localaddr = getLocalAddr(sockfd);
  struct sockaddr_in6 peeraddr = getPeerAddr(sockfd);
  if (localaddr.sin6_family == AF_INET)
  {
    const struct sockaddr_in* laddr4 = reinterpret_cast<struct sockaddr_in*>(&localaddr);
    const struct sockaddr_in* raddr4 = reinterpret_cast<struct sockaddr_in*>(&peeraddr);
    return laddr4->sin_port == raddr4->sin_port
        && laddr4->sin_addr.s_addr == raddr4->sin_addr.s_addr;
  }
  else if (localaddr.sin6_family == AF_INET6)
  {
    return localaddr.sin6_port == peeraddr.sin6_port
        && memcmp(&localaddr.sin6_addr, &peeraddr.sin6_addr, sizeof localaddr.sin6_addr) == 0;
  }
  else
  {
    return false;
  }
}

