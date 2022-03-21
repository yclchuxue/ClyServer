#include "LogStream.h"
#include "noncopyable.h"

#include <string>
#include <mutex>
#include <thread>
#include <atomic>
#include <vector>
#include <condition_variable>

namespace muduo
{
class AsyncLogging : noncopyable
{
    public:
        AsyncLogging(const std::string& basename,
                        off_t rollsize, //long int,文件缓冲区中需要刷新时的字节数
                        size_t flushInterval = 3);

        void start();

        void stop();

        void append(const char *line, int len);

        ~AsyncLogging()
        {
            if(running_) {stop();}
        }

    private:
        

        void AsyncFunc();

        typedef muduo::detail::FixedBuffer<muduo::detail::kLargeBuffer> Buffer;
        typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
        typedef BufferVector::value_type BufferPtr;

        const int flushInterval_;
        std::atomic<bool> running_;
        const string basename;
        const off_t rollSize;
        std::thread thread_;
        //muduo::CountDownLatch latch_;
        std::mutex mutex_;
        std::condition_variable cv;
        //muduo::Condition cond_ GUARDED_BY(mutex_);
        BufferPtr currentBuffer_; //GUARDED_BY(mutex_);
        BufferPtr nextBuffer_ ;//GUARDED_BY(mutex_);
        BufferVector buffers_ ;//GUARDED_BY(mutex_);

};
}