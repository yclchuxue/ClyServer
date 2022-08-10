#include "../net/Buffer.h"
#include "HttpContext.h"
#include "../net/Buffer.h"

using namespace eff;
using namespace eff::net;
// using namespace eff::http;

bool HttpContext::processRequestLine(const char *begin, const char * end)
{
    bool succeed = false;
    const char *start = begin;
    const char *space = std::find(start, end, ' ');
    LOG_DEBUG << "start process line";
    if(space == end){
        LOG_DEBUG << "space == end";
        string strs(start, end);
        LOG_DEBUG << "strs" << strs;
    }
    
    if(space != end && request_.setMethod(start, space))
    {
        LOG_DEBUG << "the method had set";
        start = space + 1;
        space = std::find(start, end, ' ');
        if(space != end)
        {
            LOG_DEBUG << "start set url";
            const char *question = std::find(start, space, '?');
            if(question != space)
            {
                request_.setPath(start, question);
                request_.setQuery(question, space);
            }else{
                request_.setPath(start, space);
            }
            start = space + 1;
            succeed = end-start == 8 && std::equal(start, end-1, "HTTP/1.");
            LOG_DEBUG << "equal is " << succeed;
            if(succeed)
            {
                if(*(end-1) == '1')
                {
                    request_.setVersion(HttpRequest::kHttp11);
                }else if(*(end-1) == '0'){
                    request_.setVersion(HttpRequest::kHttp10);
                }else{
                    succeed = false;
                }
            }
        }
    }
    return succeed;
}

//解析http报文
bool HttpContext::parseRequest(Buffer *buf, Timestamp receiveTime)
{
    bool ok = true;
    bool hasMore = true;
    while(hasMore)
    {
        if(state_ == kExpectRequestLine)
        {
            const char *crlf = buf->findCRLF();
            LOG_DEBUG << "?????????????????????????????????????????????";
            if(crlf)
            {
                ok = processRequestLine(buf->peek(), crlf);
                LOG_WARN << ">>>>>>>>>>>>>>>>>>>>>" << ok;
                if(ok)
                {
                    request_.setReceiveTime(receiveTime);
                    buf->retrieveUntil(crlf + 2);
                    state_ = kExpectHeaders;
                }else{
                    LOG_WARN << "}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}";
                    hasMore = false;
                }
            }else{
                hasMore = false;
            }
        }else if(state_ == kExpectHeaders){
            const char *crlf = buf->findCRLF();
            if(crlf)
            {
                const char *colon = std::find(buf->peek(), crlf, ':');
                if(colon != crlf)
                {
                    request_.addHeader(buf->peek(), colon, crlf);
                }else{
                    state_ = kGotAll;
                    hasMore = false;
                }
                buf->retrieveUntil(crlf + 2);
            }else{
                hasMore = false;
            }
        }else if(state_ == kExpectBody){
            //FIXME
        }
    }

    std::string str = buf->retrieveAllAsString();
    LOG_DEBUG << "the message is";
    LOG_DEBUG << str;

    LOG_DEBUG << "method" << request_.method_;
    LOG_DEBUG << "version" << request_.version_;
    LOG_DEBUG << "path" << request_.path_;
    LOG_DEBUG << "query" << request_.query_;
    LOG_DEBUG << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << ok;
    for(auto it : request_.headers_)
    {
        LOG_DEBUG << it.first << ":\t" << it.second;
    }

    LOG_DEBUG << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << ok;
    return ok;
}