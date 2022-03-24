#ifndef NET_EVENTLOOPTHREAD_H
#define NET_EVENTLOOPTHREAD_H

#include <atomic>
#include <functional>
#include <vector>
#include "../base/Thread.h"
#include <mutex>
#include <condition_variable>


namespace eff
{
namespace net
{
class EventLoop;

// class eff::Thread;

class EventLoopThread
{
    public:
        typedef std::function<void(EventLoop*)> ThreadInitCallback;

        EventLoopThread(const ThreadInitCallback& cb,
                        const std::string & name);

        ~EventLoopThread();

        EventLoop * startLoop();

    private:
        void threadFunc();

        EventLoop * loop_;
        bool exiting_;
        Thread thread_;
        std::mutex mutex_;
        std::condition_variable cond_;
        ThreadInitCallback callback_;
        
};
}
}

#endif 