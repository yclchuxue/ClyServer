// #include <memory>
// #include <time.h>

// namespace muduo
// {

// class TimeZone //: public muduo::copyable
// {
//     public:
//         explicit TimeZone(const char * zonefile);
//         TimeZone(int eastOfUtc, const char * tzname);
//         TimeZone() = default;

//         bool valid() const
//         {
//             return static_cast<bool>(data_);
//         }

//         bool valid() const 
//         {
//             return static_cast<bool>(data_);
//         }

//         struct tm toLocalTime(time_t secondsSinceEpoch, bool yday = false);

//         static time_t fromUtcTime(const struct tm&);

//         static time_t fromUtcTime(int year, int month, int day,
//                                     int hour, int minute, int seconds);

//         struct Data;

//     private:
//         std::shared_ptr<Data> data_;
// };

// }