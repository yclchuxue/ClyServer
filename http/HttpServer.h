#ifndef HTTP_HTTPSERVER_H
#define HTTP_HTTPSERVER_H

#include "../net/TcpServer.h"
#include "../net/EventLoop.h"


namespace eff
{
namespace net
{
class HttpRequest;
class HttpResponse;

class HttpServer
{
    public:
        typedef std::function<void(const HttpRequest&, HttpResponse *)> HttpCallback;

        HttpServer(EventLoop *loop, const InetAddress &listenAddr,
                    const string &name,
                    TcpServer::Option option = TcpServer::kNoReusePort);

        EventLoop *getLoop() const { return server_.getloop(); }

        void setThreadNum(const HttpCallback &cb)
        { httpCallback_ = cb; }

        void setThreadNum(int numThreads)
        {
            server_.setThreadNum(numThreads);
        }

        void start();

    private:
        void onConnection(const TcpConnectionPtr &conn);

        void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp receiveTime);

        void onRequest(const TcpConnectionPtr &conn, const HttpRequest &req);

        TcpServer server_;
        HttpCallback httpCallback_;
};
}
}

#endif