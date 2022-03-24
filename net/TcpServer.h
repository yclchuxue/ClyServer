#ifndef NET_TCPSERVER_H
#define NET_TCPSERVER_H
#include "../base/Atomic.h"
#include "../base/Types.h"
#include "TcpConnection.h"
#include "EventLoopThreadPool.h"
#include <map>

namespace eff
{
namespace net
{
class Acceptor;
class EventLoop;
//class EventLoopThreadPool;

class TcpServer //: noncopyable
{
    public:
        typedef std::function<void(EventLoop*)> ThreadInitCallback;

        enum Option
        {
            kNoReusePort,
            kReusePort,
        };

        TcpServer(EventLoop * loop,
                const InetAddress& listenAddr,
                const std::string & nameArg);
        ~TcpServer();


        const std::string &name()   const { return name_; }
        EventLoop* getloop() const { return loop_; }

        void setThreadNum(int numThreads);
        void setThreadInitCallback(const ThreadInitCallback & cb)
        { threadInitCallback_ = cb; }

        std::shared_ptr<EventLoopThreadPool> threadPoll()
        {
            return threadPool_;
        }

        void start();

        void setConnectionCallback(const ConnectionCallback & cb)
        { connectionCallback_ = cb; }

        void setMessageCallback(const MessageCallback& cb)
        { messageCallback_ = cb; }

        void setWriteCompleteCallback(const WriteCompleteCallback& cb)
        { writeCompleteCallback_ = cb; }

    private:

        void newConnection(int sockfd, const InetAddress& peerAddr);

        void removeConnection(const TcpConnectionPtr & conn);

        void removeConnectionInLoop(const TcpConnectionPtr& conn);

        typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

        EventLoop * loop_;
        const std::string name_;
        std::unique_ptr<Acceptor> acceptor_;
        std::shared_ptr<EventLoopThreadPool> threadPool_;
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
        WriteCompleteCallback writeCompleteCallback_;
        ThreadInitCallback threadInitCallback_;
        //AtomicInt32 started;
        InetAddress addr_;

        int nextConnId_;
        ConnectionMap connections_;
};

}
}

#endif