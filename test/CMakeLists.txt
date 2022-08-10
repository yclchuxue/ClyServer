cmake_minimum_required(VERSION 2.8)

project(server_test)


SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -pthread")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(

    server_test
    ../base/AsyncLogging.h
	../base/AsyncLogging.cpp
	../base/FileUtil.h
	../base/FileUtil.cpp
	../base/LogFile.h
	../base/LogFile.cpp
	../base/LogStream.h
	../base/LogStream.cpp
	../base/Logging.h
	../base/Logging.cpp
	../base/Thread.h
	../base/Thread.cpp
	../base/Timestamp.h
	../base/Timestamp.cpp
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
	../net/Socket.h
	../net/Socket.cpp
	../net/SocketsOps.h
	../net/SocketsOps.cpp
	../net/TcpConnection.h
	../net/TcpConnection.cpp
	../net/TcpServer.h
	../net/TcpServer.cpp
	echo.h
	echo.cpp
	test.cpp
    )

