#include "Acceptor.h"
#include "../base/Logging.h"
#include "EventLoop.h"
#include "SocketsOps.h"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
using namespace eff;
using namespace eff::net; 
using std::placeholders::_1;

Acceptor::Acceptor(EventLoop* loop, const InetAddress & listenAddr)
    :   loop_(loop),
        acceptSocket_(sockets::createNonblockingOrDie(listenAddr.addr_.sin_family)),
        acceptChannel_(loop, acceptSocket_.fd()),
        listening_(false)
{
    acceptSocket_.bindAddress(listenAddr); //解析addr并绑定
    acceptChannel_.setReadCallback(
                    std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
    acceptChannel_.disableAll();
    acceptChannel_.remove();
    ::close(idleFd_);
}

void Acceptor::listen()
{
    // loop_->assertInLoopThread();
    listening_ = true;
    acceptSocket_.listen();   //listen fd
    acceptChannel_.enableReading(); //设置events
    LOG_DEBUG << "set channel listen";
}

void Acceptor::handleRead()       //listen
{
    //loop_->assertInLoopThread();
    InetAddress peerAddr;

    int connfd = acceptSocket_.accept(&peerAddr);
    LOG_DEBUG << connfd;
    if(connfd >= 0)
    {
        if(newConnectionCallback_)
        {
            newConnectionCallback_(connfd, peerAddr);
        }else{
            sockets::close(connfd);
        }
    }else{
        LOG_SYSERR << "in Acceptor::handleRead";

        if(errno == EMFILE)
        {
            ::close(idleFd_);
            idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);
            ::close(idleFd_);
            idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
}