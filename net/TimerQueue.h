#include <set>
#include <vector>

#include <mutex>
#include "../base/Timestamp.h"
#include "Channel.h"
#include "Callbacks.h"

namespace muduo
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

    // Schedules the callback to be run at given time,
    // repeats if @c interval > 0.0.
    //
    // Must be thread safe. Usually be called from other threads.
        TimerId addTime(const TimerCallback& cb,
                            Timestamp when,
                            double interval);
        
        //void cancel(TimerId timerID);

    
};

} //namespace net
    
} // namespace muduo

