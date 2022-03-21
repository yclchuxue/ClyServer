#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include "../base/noncopyable.h"
#include <vector>
#include <functional>
#include <atomic>
#include <sys/syscall.h>
#include <sys/eventfd.h>
#include <memory>
#include <unistd.h>
#include <algorithm>
//#include "../base/CurrentThread.h"
#include "../base/Logging.h"
#include <assert.h>
#include "TimerId.h"
#include <mutex>
#include "Callbacks.h"

namespace muduo
{

namespace net
{

class PollPoller;
class Channel;
//class TimeQueue;

class EventLoop : noncopyable
{
    public:
        typedef std::function<void()> Functor;
        EventLoop();
        ~EventLoop();

        void loop();

        void quit();

        Timestamp pollReturnTime() const { return pollReturnTime_;}

        int64_t iteration() const { return iteration_; }

        
        TimerId runAt(const Timestamp& time, const TimerCallback& cb);
        TimerId runAfter(double delay, const TimerCallback& cb);
        TimerId runEvery(double interval, const TimerCallback & cb);
        

        void runInLoop(const Functor&cb);

        void queueInLoop(Functor cb);

        void cancel(TimerId timerId);

        void wakuo();

        void removeChannel(Channel * channel);

        bool hasChannel(Channel * channel);

        void assertInLoopThread()
        {
            if(!isInLoopThread())
            {
                abortNotInLoopThread();
            }
        }

        void updateChannel(Channel* channel);

        bool isInLoopThread() const { return threadId_ == syscall(SYS_gettid); }

        bool eventHandling() const { return eventHandling_; }

        //void setContext(const boost::any& context) { context_ = context; }

        // const boost::any& getContext() const
        // { return context_; }

        // boost::any* getMutableContext()
        // { return &context_; }

        static EventLoop* getEventLoopOfCurrentThread();

    private:
        void abortNotInLoopThread();
        //void handleRead();
        void doPendingFunctors();

        void printActiveChannels() const; //debug

        typedef std::vector<Channel*> ChannelList;

        bool looping_;    //atomic
        std::atomic<bool> quit_; //atomic
        bool eventHandling_;
        bool callingPendingFunctors_;
        int64_t iteration_;
        const pid_t threadId_;
        Timestamp pollReturnTime_;
        std::unique_ptr<PollPoller> poller_;
        //std::unique_ptr<TimeQueue> timerQueue_;
        int wakeupFd_;
        //boost::any context_;
        std::unique_ptr<Channel> wakeupChannel_;
        //boost::any contex_;

        ChannelList activeChannels_;
        Channel * currentActiveChannel_;
    
        mutable std::mutex mutex_;
        std::vector<Functor> pendingFunctors_;
};

}

}

#endif