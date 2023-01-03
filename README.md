## 介绍
ClyServer是一个运行于GNU/Linux平台的HTTP服务器，采用C++11编写，目前支持处理HTTP/1.1的静态GET，HEAD，OPTIONS请求。
## 特点
* 采用基于 one loop per thread + thread pool 并发模型
* 支持TCP和UDP协议，可选者epoll和poll两种IO复用机制
* 封装高性能 buffer 类进行数据发送和接收
* 实现多缓冲区异步日志库,支持设立日志级别
* 使用 Linux 内核提供的 timerfd 将定时事件和 IO 时间统一处理
## 安装
1.首先需要保证你有cmake
```
sudo apt-get install cmake          //debian/ubuntu
```
2.拉取代码并编译
```
git clone git@github.com:yclchuxue/ClyServer.git
cd ClyServer
mkdir build
cd build
cmake ..
make
```