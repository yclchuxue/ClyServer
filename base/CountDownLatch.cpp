#include "CountDownLatch.h"

using namespace eff;


CountDownLatch::CountDownLatch(int count)
    :   mutex_(),
        condition_(mutex_),
        count_(count)
{
}

void CountDownLatch::wait()
{
    std::lock_guard<std::mutex> guard(mutex_);
    while(count_ > 0)
    {
        condition_.wait();
    }
}

void CountDownLatch::countDown()
{
    std::lock_guard<std::mutex> guard(mutex_);
    --count;
    if(count_ == 0)
    {
        condition_.notifyAll();
    }
}

int CountDownLatch::getCount()const
{
    std::lock_guard<std::mutex> guard(mutex_);
    return count_;
}