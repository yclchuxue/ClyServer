cmake_minimum_required(VERSION 2.8)

project(server_test)


SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -pthread")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(

    server_test
    ../base/Timestamp.h
    ../base/Timestamp.cpp
    ../base/Logging.h
    ../base/Logging.cpp
    ../base/LogStream.h
    ../base/LogStream.cpp
    ../base/Thread.h
    ../base/Thread.cpp
    ../net/Endian.h
    ../net/InetAddress.h
    ../net/Socket.h
    ../net/Socket.cpp
    ../net/Acceptor.h
    ../net/Acceptor.cpp
    ../net/Buffer.h
    ../net/Buffer.cpp
    ../net/Channel.h
    ../net/Channel.cpp
    ../net/EventLoop.h
    ../net/EventLoop.cpp
    ../net/EventLoopThread.h
    ../net/EventLoopThread.cpp
    ../net/EventLoopThreadPool.h
    ../net/EventLoopThreadPool.cpp
    ../net/PollPoller.h
    ../net/PollPoller.cpp
    ../net/SocketsOps.h
    ../net/SocketsOps.cpp
    ../net/TcpServer.h
    ../net/TcpServer.cpp
    ../net/TcpConnection.h
    ../net/TcpConnection.cpp
    HttpContext.h
    HttpContext.cpp
    HttpRequest.h
    HttpResponse.h
    HttpResponse.cpp
    HttpServer.h
    HttpServer.cpp
    test.cpp
    )

