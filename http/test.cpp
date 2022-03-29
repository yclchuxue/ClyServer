#include "HttpServer.h"
#include "../net/EventLoop.h"
#include <arpa/inet.h>
#include "../net/InetAddress.h"
#include <iostream>

using namespace std;
using namespace eff;
using namespace eff::net;


int main()
{
    int port;
    cin >> port;

    EventLoop loop;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    InetAddress address(addr);
    string name = "test";
    HttpServer server(&loop, address, name);
    server.start();
    loop.loop();

}