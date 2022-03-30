#include "../net/TcpServer.h"
#include "../net/EventLoop.h"
#include "../net/Buffer.h"
#include "../base/Timestamp.h"
#include <arpa/inet.h>
#include "../net/InetAddress.h"

using namespace eff;
using namespace eff::net;

class EchoServer
{
    public:
        EchoServer(EventLoop *loop, InetAddress addr, std::string &name);
        void start();
        

    private:
        void onMessage(const TcpConnectionPtr&, Buffer*, Timestamp);

        void onConnection(const TcpConnectionPtr&);

        TcpServer server_;

};