#ifndef BASE_THREAD_H
#define BASE_THREAD_H

#include <functional>
#include <memory>
// #include "Atomic.h"
#include <thread>
#include <pthread.h>
#include <future>
#include <unistd.h>
#include <sys/syscall.h>
#include <atomic>
namespace eff
{

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
        ThreadData(ThreadFunc &func, std::string &name, std::promise<void>& p, pid_t tid)
            :   func_(func),
                name_(name),
                p_(p),
                tid_(&tid)
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

class Thread //: noncopyable
{
    public:
        typedef std::function<void ()> ThreadFunc;

        explicit Thread(ThreadFunc func, const std::string & name = std::string());

        ~Thread();

        void start();
        int join();

        bool started() const { return started_; }
        pid_t tid() const { return tid_; }
        const std::string & name() const {return name_; }


        static int numCreated() { return numCreated_.load(); }

        // static int numCreadted()
        // {
        //     return numCreadted_;  //创建了多少线程
        // }
    
    private:
        void setDefaultName();

        bool started_;
        bool joined_;
        pthread_t pthreadId_;
        pid_t tid_;
        ThreadFunc func_;
        std::string name_;
        //CountDownLatch lathc_;
        std::promise<void> p_;

        static std::atomic<int> numCreated_;

        //static AtomicInt32 numCreated_;
};

}

#endif