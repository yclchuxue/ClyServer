#include <mutex>

#include <pthread.h>

namespace muduo
{

class Condition : noncopyable
{
    public:
        explicit Condition(MutexLock & mutex)
            :   mutex_(mutex)
        {
            MCHECK(pthread_cond_init(&pcond_, NULL));
        }

    private:
        MutexLock & mutex_;
        pthread_cond_t pcond_;
};

}