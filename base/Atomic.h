#include <stdio.h>
#include <stdint.h>
// #include <iostream>
// using namespace std;
namespace eff
{

namespace detail
{

template<typename T> class AtomicIntegerT //: noncopyable
{
    public:
        AtomicIntegerT()
            :   value_(0)
        {
        }

        T get()
        {                                          //old,new
            return _sync_val_compare_and_swap(&value_, 0, 0);
            //if(*value_ == old){*value_ = new; return true;}else(return false;)
        }

        T getAndAdd(T x)
        {
            return __sync_fetch_and_add(&value_, x); ////返回*value未加x之前, value现在为*value + x
        }

        T addAndGet(T x)
        {
            return getAndAdd(x) + x;     //返回value_ + x;
        }

        T incrementAndGet()
        {
            return addAndGet(1);         //返回value_ + 1;
        }

        T decrementAndGet()
        {
            return addAndGet(-1);         //返回value_ - 1;
        }

        void add(T x)
        {
            getAndAdd(x);              //对value_ + x，不返回
        }

        void decrement()
        {
            decrementAndGet();          //对value_ -1, 不返回
        }

        T getAndSet(T newValue)
        {
            //cout << "bbb" << value_ << "ccc" << newValue << endl;

            return __sync_lock_test_and_set(&value_, newValue);
        }

    private:
        volatile T value_;
};
}

typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;

}