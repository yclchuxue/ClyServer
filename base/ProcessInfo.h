#include "StringPiece.h"
#include "Types.h"
#include "Timestamp.h"
#include <vector>
#include <sys/types.h>

namespace eff
{

namespace ProcessInfo
{
    pid_t pid();
    string pidString();
    uid_t uid();
    string username();
    uid_t euid();
    Timestamp startTime();
    int clockTicksPerSecond();
    int pageSize();
    bool isDebugBuild();

    string hostname();
    string procname();
    StringPiece procname(const string& stat);

    string procStatus();

    string procStat();

    string threadStat();

    string exePath();

    int openedFiles();

    int maxOpenFiles();

    struct CpuTime
    {
        double userSeconds;
        double systemSeconds;

        CpuTime() : userSeconds(0.0), systemSeconds(0.0) { }

        double total() const {return userSeconds + systemSeconds; }
    };

    int numThreads();
    std::vector<pid_t> threads();
}

}