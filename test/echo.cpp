#include "echo.h"
#include "../net/InetAddress.h"

using namespace eff;
using namespace eff::net;

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

EchoServer::EchoServer(EventLoop *loop, InetAddress addr, std::string &name)
    :   server_(loop, addr, name)
{
    LOG_DEBUG << "构造EchoServer";

    server_.setConnectionCallback(std::bind(&EchoServer::onConnection, this, _1));

    server_.setMessageCallback(std::bind(&EchoServer::onMessage,this,_1,_2,_3));
}


void EchoServer::start()
{
    server_.start();
}
        

void EchoServer::onMessage(const TcpConnectionPtr& ptr, Buffer*buf, Timestamp time)
{
    std::string str = buf->retrieveAllAsString();
    LOG_DEBUG << "the message is";
    LOG_DEBUG << str;
    ptr->send(str);
}

void EchoServer::onConnection(const TcpConnectionPtr&)
{
    LOG_DEBUG << "echo server get connected***********************";
}