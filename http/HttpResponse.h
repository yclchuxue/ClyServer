#ifndef HTTP_HTTPRESPONSE_H
#define HTTP_HTTPRESPONSE_H

#include "../base/Types.h"

#include <map>

namespace eff
{
namespace net
{

class Buffer;

class HttpResponse
{
    public:
        enum HttpStatusCode
        {
            kUnknown,
            k2000k = 200,
            k301MovedPermanently = 301,
            k400BadRequest = 400,
            k404NotFound = 404,
        };

        explicit HttpResponse(bool close)
            :   statusCode_(kUnknown),
                closeConnection_(close)
        {
        }

        void setStatusCode(HttpStatusCode code)
        { 
            statusCode_ = code; 
        }

        void setStatusMessage(const string & message)
        {
            statusMessage_ = message;
        }

        void setCloseConnextion(bool on)
        {
            closeConnection_ = on;
        }

        void setContentType(const string & contentType)
        {
            addHeader("Content-Type", contentType);
        }

        void addHeader(const string & key, const string &value)
        {
            headers_[key] = value;
        }

        void setBody(const string &body)
        {
            body_ = body;
        }

        void appendToBuffer(Buffer *output) const;

    private:
        std::map<string, string> headers_;
        HttpStatusCode statusCode_;
        string statusMessage_;
        bool closeConnection_;
        string body_;
};

}
}

#endif