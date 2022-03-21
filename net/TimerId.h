#include "../base/noncopyable.h"
#include <stdio.h>
#include <iostream>

namespace muduo
{
namespace net
{

class Timer;

class TimerId //: public muduo::copyable
{
    public:
        TimerId()
            :   timer_(NULL),
                sequence_(0)
        {
        }

        TimerId(Timer * timer, int64_t seq)
            :   timer_(timer),
                sequence_(seq)
        {
        }
    
        friend class TimerQueue;

    private:
        Timer * timer_;
        int64_t sequence_;
};


}
}