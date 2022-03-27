// #include "CurrentThread.h"
// #include <assert.h>
// #include <pthread.h>

// #include "noncopyable.h"

// #define MCHECK(ret) ({__typeof__ (ret) errnum = (ret); \
//                             assert(errnum == 0); (void)errnum;})

// namespace muduo
// {
// class MutexLock
// {
//     public:
//         MutexLock()
//             :   holder_(0)
//         {
//             MCHECK(pthread_mutex_init(&mutex_, NULL));
//         }

//         ~MutexLock()
//         {
//             assert(holder_ == 0);
//             MCHECK(pthread_mutex_destroy(&mutex_));
//         }

        


//     private:
        

//         pthread_mutex_t mutex_;
//         pid_t holder_;
// };
// }