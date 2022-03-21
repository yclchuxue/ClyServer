#include "LogFile.h"
#include "Thread.h"

#include <mutex>
#include <assert.h>
#include <stdio.h>
#include <time.h>

using namespace muduo;

LogFile::LogFile(const string &basename,
                    off_t rollSize,
                    bool threadSafe,
                    int flushInterval,
                    int checkEveryN)
    :   basename_(basename),
        rollSize_(rollSize),
        flushInterval_(flushInterval),
        checkEveryN_(checkEveryN),
        count_(0),
        mutex_(threadSafe ? new mutex : NULL),
        startOfPeriod_(0),
        lastRoll_(0),
        lastFlush_(0)
{
    assert(basename.find('/') == string::npos);
    rollFile();
}


bool LogFile::rollFile()
{
    time_t now = 0;
    string filename = getLogFileName(basename_, &now);
    time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;

    if (now > lastRoll_)
    {
        lastRoll_ = now;
        lastFlush_ = now;
        startOfPeriod_ = start;
        file_.reset(new FileUtil::AppendFile(filename));
        return true;
    }
    return false;
}

string LogFile::getLogFileName(const string& basename, time_t* now)
{
    string filename;
    filename.reserve(basename.size() + 64);
    filename = basename;

    char timebuf[32];
    struct tm tm;
    *now = time(NULL);
    gmtime_r(now, &tm); // FIXME: localtime_r ?
    strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S.", &tm);
    filename += timebuf;

    filename += ProcessInfo::hostname();

    char pidbuf[32];
    snprintf(pidbuf, sizeof pidbuf, ".%d", ProcessInfo::pid());
    filename += pidbuf;

    filename += ".log";

    return filename;
}

void LogFile::append(const char* logline, int len)
{
  if (mutex_)
  {
    std::lock_guard<std::mutex> guard(*mutex_);
    append_unlocked(logline, len);
  }
  else
  {
    append_unlocked(logline, len);
  }
}

void LogFile::append_unlocked(const char* logline, int len)
{
    file_->append(logline, len);

    if (file_->writtenBytes() > rollSize_) //已写字节数是否大于规定的一次最大写入数
    {
        rollFile();
    }
    else
    {
        ++count_;//记录写入的日志条数
        if (count_ >= checkEveryN_)
        {
        count_ = 0;
        time_t now = ::time(NULL);
        time_t thisPeriod_ = now / kRollPerSeconds_ * kRollPerSeconds_;
        if (thisPeriod_ != startOfPeriod_) //天数不相同 重新生成文件
        {
            rollFile();
        }
        else if (now - lastFlush_ > flushInterval_) //如果现在和上次刷新缓冲区的时间差超过flushinterval的话 进行刷新
        {
            lastFlush_ = now;
            file_->flush();
        }
        }
    }
}
