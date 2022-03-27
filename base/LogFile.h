#ifndef BASE_LOGFILE_H
#define BASE_LOGFILE_H
//#include "FileUtil.h"
#include "noncopyable.h"
#include <memory>
#include <cstring>
#include <iostream>
#include <mutex>
#include "FileUtil.h"

using namespace std;

namespace eff
{

namespace FileUtil
{
class AppendFile;
}

class LogFile //: noncopyable
{
    public:
        LogFile(const string &basename, //项目名称
                off_t rollSize,         //一次最大刷新字节数
                bool threadSafe = true, //通过对写入操作加锁，来确定是否线程安全
                int flushInterval = 3,  //隔多少毫秒刷新一次
                int checkEveryN = 1024);

        ~LogFile();

        void append(const char *logline, int len);
        void flush();
        bool rollFile();

    private:

        void append_unlocked(const char *logline, int len);
        static string getLogFileName(const string & basename, time_t *now);

        const string basename_; //项目名
        const off_t rollSize_;   //一次刷新的最大字节数
        const int flushInterval_;   //间隔时间
        const int checkEveryN_;

        int count_;

        std::unique_ptr<mutex> mutex_;
        time_t startOfPeriod_;
        time_t lastRoll_;
        time_t lastFlush_;
        std::unique_ptr<FileUtil::AppendFile> file_; 

        const static int kRollPerSeconds_ = 60*60*24;
};
}

#endif