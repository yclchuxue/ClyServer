#include "Poller.h"
#include "Channel.h"

using namespace muduo;
using namespace muduo::net;

Poller::Poller(EventLoop * loop)
    :   ownerLoop_(loop)
{
}

Poller::~Poller() = default;


// Timestamp Poller::poll(int timeoutMs, ChannelList * activeChannels)
// {
//     //XXX pollfds_ shouldn't change
//     int numEvents = ::poll(&*pollfds_.beging(), pollfds_.size(), timeoutMs);
//     Timestamp now(Timestamp::now());
//     if(numEvents > 0)
//     {
//         LOG_TRACE << numEvents << " events happended";
//         fillActiveChannels(numEvents, activeChannels);
//     }else if(numEvents == 0){
//         LOG_TRACE << " nothing happended";
//     }else{
//         LOG_SYSERR << "Poller::poll()";
//     }

//     return now;
// }

// void Poller::fillActiveChannels(int numEvents,
//                     ChannelList * activeChannels) const
// {
//     for(PollFdList::const_iterator pfd = pollfds_.begin();
//             pfd != pollfds_.end() && numEvents > 0; ++pfd)
//     {
//         if(pfd->revents > 0)
//         {
//             --numEvents;
//             ChannelMap::const_iterator ch = channels_.find(pfd->fd);
//             assert(ch != channels_.end());
//             Channel * channel = ch->second;
//             assert(channel->fd() == pfd->fd);
//             //pfd->revents = 0;
//             activeChannels->push_back(channel);
//         }
//     }
// }

// void Poller::updateChannel(Channel * channel)
// {
//     assertInLoopThread();
//     LOG_TRACE << "fd = " << channel->fd() << " events = " << channel->events();
//     if(channel->index() < 0)
//     {
//         //a new one, add to pollfds
//         assert(channels_.find(channel->fd()) == channels_.end());
//         struct poolfd pfd;
//         pfd.fd = channel->fd();
//         pfd.events = static_cast<short>(channel->events());
//         pfd.revents = 0;
//         pollfds_.push_back(pfd);
//         int idx = static_cast<int>(pollfds_.size());
//         channel->set_index(idx);
//         channels_[pfd.fd] = channel;
//     }else{
//         //update existing one
//         assert(channels_.find(channel->fd()) != channels_.end());
//         assert(channels_[channel->fd()] == channel);
//         int idx = channel->index();
//         assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
//         struct pollfd& fd = pollfds_[idx];
//         assert(pfd.fd == channel->fd() || pfd.fd == -1);
//         pfd.events = static_cast<short>(channel->events());
//         pfd.revents = 0;
//         if(channel->isNoneEvent())
//         {
//             //ignore this pollfd
//             pfd.fd = -1;
//         }
//     }
// }

// bool Poller::hasChannel(Channel * channel) const 
// {
//     assertInLoopThread();
//     ChannelMap::const_iterator it =  channels_.find(channel->fd());
//     return it != channels_end() && it->second == channle;
// }
