#ifndef BASE_COUNTDOWNLATCH_H
#define BASE_COUNTDOWNLATCH_H
#include <Condition.h>
#include <mutex>

namespace eff
{
class CountDownLatch //: noncopyable
{
    public:
        explicit CountDownLatch(int count);  //倒数几次

        void wait();        //等待计数变为0

        void countDown();   //计数减1

        int getCount() const;

    private:
        mutable std::mutex mutex_;   
        Condition condition;
        int count_;
};

}

#endif