#ifndef NET_ACCEPTOR_H
#define NET_ACCEPTOR_H
#include <functional>
#include "Channel.h"
#include "Socket.h"
#include "InetAddress.h"
 

namespace eff
{
namespace net
{
class EventLoop;
 
class Acceptor //: noncopyable
{
    public:
        typedef std::function<void (int sockfd, const InetAddress&)> NewConnectionCallback;

        Acceptor(EventLoop* loop, const InetAddress& listenAddr);

        ~Acceptor();

        void setNewConnectionCallback(const NewConnectionCallback &cb)
        {
            newConnectionCallback_ = cb;
        }

        void listen();

        bool listenling() const { return listening_; }
    
    private:
        void handleRead();

        EventLoop *loop_;
        Socket acceptSocket_;
        Channel acceptChannel_;
        NewConnectionCallback newConnectionCallback_;
        bool listening_;
        int idleFd_;
};
}
}

#endif