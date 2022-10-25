#ifndef NET_TIMER_H
#define NET_TIMER_H
#include "../base/Atomic.h"
#include "../base/Timestamp.h"
#include "../net/Callbacks.h"

namespace eff
{
namespace net
{

//internal class for timer event
class Timer // : noncopyable
{

    private:
        const TimerCallback callback_; //定时器回调函数
        Timestamp expiration_;  //绝对的时间
        const double interval_;  //如果有重复属性，超时的时间间隔
        const bool repeat_;  //是否有重复
        const int64_t sequence_;  //定时器序号

        static AtomicInt64 s_numCreated_; //定时器计数

    public:
        Timer(TimerCallback cb, Timestamp when, double interval)
            :   expiration_(when),
                callback_(std::move(cb)),
                interval_(interval),
                repeat_(interval > 0.0),
                sequence_(s_numCreated_.incrementAndGet())
        { }

        void run() const
        {
            callback_();
        }

        Timestamp expiration() const { return expiration_; }
        int64_t sequence() const { return sequence_; }

        void restart(Timestamp now);

        static int64_t numCreated() { return s_numCreated_.get(); }

        bool repeat() const { return repeat_; }
};
}
}

#endif