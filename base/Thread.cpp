#include "Thread.h"
#include "Logging.h"

#include <type_traits>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include "Atomic.h"

using namespace eff;

// std::atomic<int> numCreated_;

Thread::Thread(ThreadFunc func, const std::string & name)
    :   started_(false),
        joined_(false),
        pthreadId_(0),
        tid_(0),
        func_(std::move(func)),
        name_(name)
        //latch_(1)
{
    setDefaultName();
}

Thread::~Thread()
{ }

void *runThread(void *arg)
{
    detail::ThreadData *td = (detail::ThreadData *)arg;
}

void Thread::setDefaultName()
{
    ++numCreated_;
    int num = numCreated_;
    if(name_.empty())
    {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread%d", num);
        name_ = buf;
    }
}

void Thread::start()
{
    assert(!started_);
    started_ = true;
    detail::ThreadData* data = new detail::ThreadData(func_, name_, p_, tid_);
    if(pthread_create(&pthreadId_, NULL, &detail::runInThread, data))
    {
        started_ = false;
        delete data;
        //LOG
    }else{
        //latch_.wait();
        assert(tid_ > 0);
    }
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, NULL);
}
