#ifndef BASE_FILEUTIL_H
#define BASE_FILEUTIL_H
// #include "StringPiece.h"
#include <sys/types.h>
#include "noncopyable.h"
#include <string>

namespace eff
{
namespace FileUtil
{

class ReadSmallFile //: noncopyable
{
    public:
        ReadSmallFile(std::string filename);
        ~ReadSmallFile();

        template<typename String>
        int readToString(int maxSize,
                String & content,
                int64_t * fileSize,
                int64_t * modifyTime,
                int64_t * createTime);
            
        int readToBuffer(int * size);

        const char * buffer() const { return buf_; }

        static const int kBufferSize = 64*1024;

    private:
        int fd_;
        int err_;
        char buf_[kBufferSize];
};

class AppendFile //: noncopyable
{
    public:
        explicit AppendFile(std::string filename);

        ~AppendFile();

        void append(const char * logline, size_t len);

        void flush();

        off_t writtenBytes() const {return writtenBytes_;}

    private:

        size_t write(const char *logline, size_t len);

        FILE * fp_;    //打开的文件指针
        char buffer_[64*1024];  //用户态缓冲区，减少文件IO次数
        off_t writtenBytes_; //已写入字节数
};

}
}

#endif