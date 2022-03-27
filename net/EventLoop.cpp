#include "EventLoop.h"
#include <algorithm>
#include <signal.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include "PollPoller.h"

using namespace eff;
using namespace eff::net;

// namespace
// {

__thread EventLoop * t_loopInThisThread = 0;

// const int kPollTimeMs = 10000;

int createEventfd()
{
    int evtfd = :: eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(evtfd < 0)
    {
        LOG_SYSERR << "Failed in eventfd";
        abort();
    }
    return evtfd;
}

// #pragma GCC diagnostic ignored "-Wold-style-cast"
// class IgnoreSigPipe
// {
//  public:
//   IgnoreSigPipe()
//   {
//     ::signal(SIGPIPE, SIG_IGN);
//     // LOG_TRACE << "Ignore SIGPIPE";
//   }
// };
// #pragma GCC diagnostic error "-Wold-style-cast"

// IgnoreSigPipe initObj;

// }
EventLoop::EventLoop()
    :   looping_(false),
        quit_(false),
        eventHandling_(false),
        iteration_(0),
        threadId_(syscall(SYS_gettid)),
        callingPendingFunctors_(false),
        wakeupFd_(createEventfd()),
        wakeupChannel_(new Channel(this, wakeupFd_)),
        poller_(new PollPoller(this)),
        currentActiveChannel_(NULL)
{
    LOG_TRACE << "Eventloop created " << this << " in thread " << threadId_;
    // if(t_loopInThisThread)
    // {
    //     LOG_FATAL << "Another EventLoop " << t_loopInThisThread
    //               << " exists in this thread " << threadId_;
    // }else{
    //     t_loopInThisThread = this;
    // }

    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));

    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
    LOG_DEBUG << "EventLoop " << this << " of thread " << threadId_
              << " destructs in thread " << syscall(SYS_gettid);
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
    t_loopInThisThread = NULL;
}

// EventLoop*EventLoop::getEventLoopOfCurrentThread()
// {
//     //return t_loopInThisThread;
// }

const int kPollTimeMs = 10000;

void EventLoop::loop()
{
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;
    LOG_DEBUG << "in loop";
    LOG_TRACE << "EventLoop " << this << " start looping";
    //LOG_DEBUG << " loop";
    while(!quit_)
    {
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        //sleep(3);
        LOG_DEBUG << activeChannels_.size();
        // ++iteration_;
        // if(Logger::logLevel() <= Logger::TRACE)
        // {
        //     printActiveChannels();
        // }

        eventHandling_ = true;
        for(Channel * channel : activeChannels_)
        {
            currentActiveChannel_ = channel;
            currentActiveChannel_->handleEvent(pollReturnTime_);
        }
        LOG_DEBUG << "";
        currentActiveChannel_ = NULL;
        eventHandling_ = false;
        doPendingFunctors();
    }   

    LOG_TRACE << "EventLoop " << this << " stop looping";
    looping_ = false;
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "EventLoop::abortNotLoopThread - EventLoop " << this
              << " was created in threadId_ = " << threadId_ 
              << ", current thread id = " << syscall(SYS_gettid); 
}

bool EventLoop::hasChannel(Channel* channel)
{
  // assert(channel->ownerLoop() == this);
  // assertInLoopThread();
  return poller_->hasChannel(channel);
}

void EventLoop::updateChannel(Channel* channel)
{
  //assert(channel->ownerLoop() == this);
  //assertInLoopThread();
  LOG_DEBUG << "CHANNEL";
  poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
  //assert(channel->ownerLoop() == this);
  //assertInLoopThread();
  if (eventHandling_)
  {
    assert(currentActiveChannel_ == channel ||
        std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());
  }
  poller_->removeChannel(channel);
}


void EventLoop::quit()
{
    quit_ = true;
    // if(!isInLoopThread())
    // {
    //     wakeup();
    // }    
    //wakeup
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof one);
    if(n != sizeof one)
    {
        LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
    }
}

void EventLoop::runInLoop(const Functor& cb)
{   
  LOG_DEBUG << "in runInLoop : " << syscall(SYS_gettid) << "threadId = " << threadId_;
    if(isInLoopThread())
    {
        cb();
    }else{
        LOG_DEBUG << "AAAA";
        queueInLoop(std::move(cb));
    }
}

void EventLoop::queueInLoop(Functor cb)
{

  LOG_DEBUG << syscall(SYS_gettid);
  {
  std::lock_guard<std::mutex> guard(mutex_);
  pendingFunctors_.push_back(std::move(cb));
  }

  if (!isInLoopThread() || callingPendingFunctors_)
  {
    wakeup();
  }
}

void EventLoop::handleRead()
{
  uint64_t one = 1;
  ssize_t n = read(wakeupFd_, &one, sizeof one);
  if(n != sizeof one)
  {
    // LOG_ERROR 
  }
}

// size_t EventLoop::queueSize() const
// {
//   std::lock_guard<std::mutex> guard(mutex_);
//   return pendingFunctors_.size();
// }

// TimerId EventLoop::runAt(Timestamp time, TimerCallback cb)
// {
//   return timerQueue_->addTimer(std::move(cb), time, 0.0);
// }

// TimerId EventLoop::runAfter(double delay, TimerCallback cb)
// {
//   Timestamp time(addTime(Timestamp::now(), delay));
//   return runAt(time, std::move(cb));
// }

// TimerId EventLoop::runEvery(double interval, TimerCallback cb)
// {
//   Timestamp time(addTime(Timestamp::now(), interval));
//   return timerQueue_->addTimer(std::move(cb), time, interval);
// }

// void EventLoop::cancel(TimerId timerId)
// {
//   return timerQueue_->cancel(timerId);
// }

// void EventLoop::wakeup()
// {
//   uint64_t one = 1;
//   ssize_t n = sockets::write(wakeupFd_, &one, sizeof one);
//   if (n != sizeof one)
//   {
//     LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
//   }
// }

// TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
// {
//     return TimerQueue_->addTimer(cb, time, 0.0);
// }
// TimerId EventLoop::runAfter(double delay, const TimerCallback& cb)
// {
//     Timestamp time(addTime(Timestamp::now(), delay));
//     return runAt(time, cb);
// }    
// TimerId EventLoop::runEvery(double interval, const TimerCallback & cb)
// {
//     Timestamp time(addTime(Timestamp::now(), interval));
//     return timerQueue_->addTimer(cb, time, interval);
// }

/*
void threadFunc()
{
    printf("threadFunc(): pid = %d, tid = %d\n",
            getpid(), muduo::CurrentThread::tid());
    
    EventLoop loop;
    loop.loop();
}
*/

void EventLoop::printActiveChannels() const
{
  for (const Channel* channel : activeChannels_)
  {
    LOG_TRACE << "{" << channel->reventsToString() << "} ";
  }
}

void EventLoop::doPendingFunctors()
{
  std::vector<Functor> functors;
  callingPendingFunctors_ = true;

  {
    std::lock_guard<std::mutex> guard(mutex_);
    functors.swap(pendingFunctors_);
  }

  for (const Functor& functor : functors)
  {
    functor();
  }
  callingPendingFunctors_ = false;
}