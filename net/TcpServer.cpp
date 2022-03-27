#include "TcpServer.h"

#include "../base/Logging.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "SocketsOps.h"
//#include "EventLoopThreadPool.h"
#include "InetAddress.h"
#include <stdio.h>

using namespace eff;
using namespace eff::net;

TcpServer::TcpServer(EventLoop * loop,
                    const InetAddress& listenAddr,
                    const std::string & nameArg)
    :   loop_(loop),
        addr_(listenAddr),
        name_(nameArg),
        acceptor_(new Acceptor(loop, listenAddr)),
        threadPool_(new EventLoopThreadPool(loop, name_)),
        nextConnId_(1)
{
    //LOG_DEBUG << "构造完tcpserver" << loop_->threadId_ << "\t" << loop_->wakeupFd_;
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
    loop_->assertInLoopThread();
    LOG_TRACE << "TcpServer::Tcpserver [" << name_ << "] destructing";

    for(auto& item : connections_)
    {
        TcpConnectionPtr conn(item.second);
        item.second.reset();
        conn->getLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
    }
}

void TcpServer::setThreadNum(int numThreads)
{
    assert(0 <= numThreads);
    threadPool_->setThreadNum(numThreads);
}

void TcpServer::start()
{
    LOG_DEBUG << "Start";
    threadPool_->start(threadInitCallback_);

    assert(!acceptor_->listenling());
    loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
    loop_->assertInLoopThread();
    EventLoop* ioloop = threadPool_->getNextLoop();
    //std::string connName = "test_connection";

    char buf[64];
    snprintf(buf, sizeof buf, "connection-%d", nextConnId_);

    std::string connName = name_ + buf;

    ++nextConnId_;



    //InetAddress localAddr(sockets::getLocalAddr(sockfd));
    TcpConnectionPtr conn(new TcpConnection(ioloop,
                                            connName,
                                            sockfd,
                                            addr_,
                                            peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, _1));
    ioloop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    LOG_DEBUG << "front runInLoop : " << syscall(SYS_gettid);
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr & conn)
{
    //loop_->assertInLoopThread();
    LOG_DEBUG << syscall(SYS_gettid);
    LOG_INFO << "TcpServer::removeConnectionInLoop [" << name_
             << "] - connection " << conn->name();

    size_t n = connections_.erase(conn->name());
    (void)n;
    assert(n == 1);
    //EventLoop* ioLoop = conn->getLoop();
    loop_->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}