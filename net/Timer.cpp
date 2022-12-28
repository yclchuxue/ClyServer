#include "Timer.h"

using namespace eff;
using namespace eff::net;

AtomicInt64 Timer::s_numCreated_;

void Timer::restart(Timestamp now)
{
    if(repeat_)
    {
        expiration_ =  addTime(now, interval_);
    }else{
        expiration_ = Timestamp::invalid();
    }
}