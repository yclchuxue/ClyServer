#ifndef NET_TIMEQUEUE_H
#define NET_TIMEQUEUE_H
#include <set>
#include <vector>
#include <mutex>
#include "../base/Timestamp.h"
#include "Channel.h"
#include "Callbacks.h"

namespace eff
{

namespace net
{

class EventLoop;
class Timer;
class TimerId;

// A best efforts timer queue.
// No guarantee that the callback will be on time.
class TimerQueue : noncopyable
{
    public:
        TimerQueue(EventLoop * loop);
        ~TimerQueue();

        TimerId addTime(const TimerCallback& cb,
                            Timestamp when,
                            double interval);
        
        void cancel(TimerId timerID);

    private:
        typedef std::pair<Timestamp, Timer*> Entry;
        typedef std::set<Entry> TimerList;
        typedef std::pair<Timer*, int64_t> ActiveTimer; //定时器和其定时器的序列号
        typedef std::set<ActiveTimer> ActiveTimerSet;

        void addTimerInLoop(Timer* timer);
        void cancelInLoop(TimerId timerId);
        // called when timerfd alarms
        void handleRead();
        // move out all expired timers
        std::vector<Entry> getExpired(Timestamp now); //返回超时的定时器列表
        void reset(const std::vector<Entry>& expired, Timestamp now);

        bool insert(Timer* timer);

        EventLoop* loop_;
        const int timerfd_; //只有一个定时器，防止同时开启多个定时器，占用多余的文件描述符
        Channel timerfdChannel_; //定时器关心的channel对象
        // Timer list sorted by expiration
        TimerList timers_; //定时器集合（有序）

        // for cancel()
        // timers_是按照到期的时间排序的，activeTimerSet_是按照对象地址排序
        ActiveTimerSet activeTimers_; //保存正在活动的定时器（无序）
        bool callingExpiredTimers_; /* atomic */ //是否正在处理超时事件
        ActiveTimerSet cancelingTimers_; //保存的是取消的定时器（无序）
};

} //namespace net
    
} // namespace eff

#endif