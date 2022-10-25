#ifndef NET_TIMEQUEUE_H
#define NET_TIMEQUEUE_H

#include "../net/TimerQueue.h"
#include "../base/Logging.h"
#include "../net/EventLoop.h"
#include "../net/Timer.h"
#include "../net/TimerId.h"

#include <sys/timerfd.h>
#include <unistd.h>

namespace eff
{
namespace net
{
    int createTimerfd(){
        int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    }


}
}

#endif