
#include "Timestamp.h"

#include <sys/time.h>
#include <chrono>
#include <stdio.h>
#include <inttypes.h>

using namespace eff; 

static_assert(sizeof(Timestamp) == sizeof(int64_t),
                        "Timestamp shoud be same size as int64_t");

std::string Timestamp::toString() const
{
    char buf[32] = {0};
    int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
    int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return buf;
}

std::string Timestamp::toFormattedString() const
{
    char buf[64] = {0};
    int64_t milli = microSecondsSinceEpoch_ / 1000 + (int64_t)8 * 60 * 60 * 1000;
    auto mTime = std::chrono::milliseconds(milli);
    auto tp = std::chrono::time_point<std::chrono::system_clock,
                                      std::chrono::milliseconds>(mTime);
    auto tt = std::chrono::system_clock::to_time_t(tp);
    std::tm *now = std::gmtime(&tt);

    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
                    now->tm_year + 1900, now->tm_mon + 1,  now->tm_mday,
                    now->tm_hour, now->tm_min, now->tm_sec);

    return buf;
}

Timestamp Timestamp::now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

