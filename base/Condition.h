#include <mutex>

#include <pthread.h>

namespace eff
{

class Condition //: noncopyable
{
    public:
        explicit Condition(std::mutex & mutex)
            :   mutex_(mutex)
        {
            pthread_cond_init(&pcond_, NULL);
        }

    
    private:
        std::mutex & mutex_;
        pthread_cond_t pcond_;
};

}