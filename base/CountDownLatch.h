#include <Condition.h>
#include <mutex>

namespace mudo
{
class CountDownLatch //: noncopyable
{
    public:
        explicit CountDownLatch(int count);  //倒数几次

        void wait();        //等待计数变为0

        void countDown();   //计数减1

        int getCount() const;

    private:
        mutable MutexLock mutex_;   
        Condition condition;
        int count_;
};

}