#include <map>
#include <vector>

#include "../base/Timestamp.h"
#include "EventLoop.h"

namespace muduo
{
namespace net
{
class Channel;

//IO Multiplexing with poll
class Poller
{
    public:
        typedef std::vector<Channel*> ChannelList;

        Poller(EventLoop * loop);
        ~Poller();

        //Polls the I/O events
        //Must be called in the loop thread
        virtual Timestamp poll(int timeoutMs, ChannelList * activeChannels) = 0;

        //Changes the interested I/O events
        //Must be call int the loop thread
        virtual void updateChannel(Channel * channel) = 0;

        virtual void removeChannel(Channel* channel) = 0;

        virtual bool hasChannel(Channel * channel) const;

        static Poller* newDefaultPoller(EventLoop * loop);

        void assertInLoopThread() { ownerLoop_->assertInLoopThread(); }

    private:
        // void fillActiveChannels(int numEvents,
        //                         ChannelList * activeChannels) const;

        // typedef std::vector<struct pollfd> PollFdList;
        

        EventLoop* ownerLoop_;
    protected:
        typedef std::map<int, Channel*> ChannelMap;
        ChannelMap Channels_;
};


}
}