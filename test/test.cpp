#include "echo.h"
#include "../net/EventLoop.h"
#include "../base/AsyncLogging.h"
#include <arpa/inet.h>
#include "../net/InetAddress.h"
#include <iostream>

using namespace std;
using namespace eff;
using namespace eff::net;

eff::AsyncLogging* g_asyncLog =  NULL;

void asyncOutput(const char* msg, int len)
{
  g_asyncLog->append(msg, len);
}

int main()
{
    int port;
    cin >> port;

    AsyncLogging log("server_test", 500*1000*1000);
    log.start();
    g_asyncLog = &log;

    eff::Logger::setOutput(asyncOutput);

    cout << "log start" << endl;
    
    EventLoop loop;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    InetAddress address(addr);
    string name = "test";
    EchoServer server(&loop, address, name);
    server.start();
    loop.loop();

}