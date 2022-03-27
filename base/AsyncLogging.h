#ifndef BASE_ASYNCLOGGING_H
#define BASE_ASYNCLOGGING_H
#include "LogStream.h"
//#include "noncopyable.h"
#include "Thread.h"
#include <string>
#include <mutex>
#include <thread>
#include <atomic>
#include <vector>
#include <condition_variable>
#include <future>

namespace eff
{

class AsyncLogging //: noncopyable
{
    public:
        AsyncLogging(const std::string& basename,
                        off_t rollsize, //long int,文件缓冲区中需要刷新时的字节数
                        size_t flushInterval = 3);

        void start()
        {
            std::future<void> p = latch_.get_future();
            running_ = true;
            thread_.start();
            p.get();
        }

        void stop()
        {
            running_ = false;
            cond_.notify_one();
            thread_.join();
        }

        void append(const char *logline, int len);

        ~AsyncLogging()
        {
            if(running_) {stop();}
        }

    private:
        

        void threadFunc();

        typedef eff::detail::FixedBuffer<eff::detail::kLargeBuffer> Buffer;
        typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
        typedef BufferVector::value_type BufferPtr;

        const int flushInterval_;
        std::atomic<bool> running_;
        const string basename;
        const off_t rollSize;
        Thread thread_;
        std::promise<void> latch_;
        std::mutex mutex_;
        std::condition_variable cond_;
        BufferPtr currentBuffer_; //GUARDED_BY(mutex_);
        BufferPtr nextBuffer_ ;//GUARDED_BY(mutex_);
        BufferVector buffers_ ;//GUARDED_BY(mutex_);

};
}

#endif