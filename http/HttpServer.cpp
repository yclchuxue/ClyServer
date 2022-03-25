#include "../base/Logging.h"
#include "HttpContext.h"
#include "HttpRequest.h"
//#include "HttpResponse.h"
#include "HttpServer.h"

using namespace eff;
using namespace eff::net;
// using namespace eff::http;

namespace eff
{
namespace net
{
namespace detail
{
void defaultHttpCallback(const HttpRequst&, HttpResponse * resp)
{
    
}
} //detail
} //net
} //eff

HttpServer::HttpServer(EventLoop *loop,
                        const InetAddress& listenAddr,
                        const string & name,
                        TcpServer::Option option)
    :   server_(loop, listenAddr, name),
        httpCallback_(detail::defaultHttpCallback)   
{
    server_.setConnectionCallback(std::bind(&HttpServer::onConnection, this, _1));
    server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, _1, _2, _3));
}

void HttpServer::start()
{
    server_.start();
}

void HttpServer::onMessage(const TcpConnectionPtr& conn,
                            Buffer * buf,
                            Timestamp receiveTime)
{
    HttpContext *context = boost::any_cast<HttpConnext>(conn->getMutableContext());

    if(!context->parseRequest(buf, receiveTime))
    {
        conn->send("HTTP/1.1 Bad Request\r\n\r\n");
        conn->shutdown();
    }

    if(context->gotAll())
    {
        onRequest(conn, context->request());
        context->reset();
    }
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest & req)
{
    const string& connection = req.getHeader("Connection");
    bool close = connection == "close" || (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");

    HttpResponse response(close);
    httpCallback_(req, &response);

    Buffer buf;
    response.appendToBuffer(&buf);
    conn->send(&buf);
    if(response.closeConnextion())
    {
        conn->shutdown();
    }
}

