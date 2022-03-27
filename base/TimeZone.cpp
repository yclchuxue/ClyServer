// #include "TimeZone.h"
// #include <algorithm>
// #include <stdexcept>
// #include <string>
// #include <vector>

// #include <assert.h>
// #include <endian.h>

// #include <stdint.h>
// #include <stdio.h>

// namespace muduo
// {
// namespace detail
// {

// struct Transition
// {
//     time_t gmttime;
//     time_t localtime;
//     int localtimeIdx;

//     Transition(time_t t, time_t l, int localIdx)
//         :   gmttime(t), localtime(l), localtimeIdx(localIdx)
//     {}
// };

// struct Comp
// {
//     bool compareGmt;

//     Comp(bool gmt)
//         :   compareGmt(gmt)
//     { }

//     bool operator()(const Transition & lhs, const Transition & rhs) const
//     {
//         if(compareGmt)
//             return lhs.gmttime < rhs.gmttime;
//         else
//             return lhs.localtime < rhs.localtime;
//     }

//     bool equal(const Transition & lhs, const Transition & rhs) const
//     {
//         if(compareGmt)
//             return lhs.gmttime == rhs.gmttime;
//         else 
//             return lhs.localtime == rhs.localtime;
//     }
// };

// struct Localtime
// {
//     time_t gmtOffset;
//     bool isDst;
//     int arrbIdx;

//     Localtime(time_t offset, bool dst, int arrb)
//         :   gmtOffset(offset), isDst(dst), arrbIdx(arrb)
//     {}
// };

// inline void fillHMS(unsigned seconds, struct tm * utc)
// {
//     utc->tm_sec = seconds % 60;
    
// }

// }
// }