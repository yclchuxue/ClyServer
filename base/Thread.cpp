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

// using namespace eff;
namespace eff
{
static std::atomic<int> numCreated_;

namespace detail
{
// pid_t gettid()
// {
//     return static_cast<pid_t>(::syscall(SYS_gettid));
// }

class ThreadData
{
    public:
        typedef std::function<void()> ThreadFunc;
        ThreadData(ThreadFunc &func, std::string &name, std::promise<void>& p, pid_t *tid)
            :   func_(func),
                name_(name),
                p_(p),
                tid_(tid)
        { }

        


        ThreadFunc func_;
        std::string name_;
        pid_t *tid_;
        std::promise<void> &p_; 
};

void *runInThread(void *arg)
{
    ThreadData * td = (ThreadData *)arg;
    *td->tid_ = ::syscall(SYS_gettid);
    td->p_.set_value();
    try{
        td->func_();
    }catch(const std::exception &e){
        fprintf(stderr, "%s\n", e.what());
        abort();
    }
    return NULL;
}
}


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
    std::future<void> latch = p_.get_future();
    assert(!started_);
    started_ = true;
    detail::ThreadData* data = new detail::ThreadData(func_, name_, p_, &tid_);
    if(pthread_create(&pthreadId_, NULL, &detail::runInThread, data))
    {
        started_ = false;
        delete data;
        //LOG
    }else{
        latch.get();
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


}