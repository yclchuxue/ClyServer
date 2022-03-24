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


        //static int numCreated() { return numCreated_.load(); }

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

        //static std::atomic<int> numCreated_;

        //static AtomicInt32 numCreated_;
};

}

#endif