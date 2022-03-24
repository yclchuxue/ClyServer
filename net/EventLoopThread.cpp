#include "EventLoopThread.h"
#include "EventLoop.h"
// #include "../base/Thread.h"

using namespace eff;
using namespace eff::net;

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb, const std::string& name)
    :   loop_(NULL),
        exiting_(false),
        thread_(std::bind(&EventLoopThread::threadFunc, this), name),
        mutex_(),
        callback_(cb)
{
}

EventLoopThread::~EventLoopThread()
{
    exiting_ = true;
    if(loop_ != NULL)
    {   
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop()
{
    //assert(!thread_.started);
    thread_.start();

    EventLoop* loop = NULL;
    {
        std::unique_lock<std::mutex> guard(mutex_);
        while(loop_ == NULL)
        {
            cond_.wait(guard);
        }
        loop = loop_;
    }

    return loop;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;

    if(callback_)
    {
        callback_(&loop);
    }

    {
        std::unique_lock<std::mutex> guard(mutex_);
        loop_ = &loop;
        cond_.notify_one();
    }

    loop.loop();
    std::unique_lock<std::mutex> guard(mutex_);
    loop_ = NULL;
}