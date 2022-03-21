// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

// #ifndef MUDUO_NET_POLLER_POLLPOLLER_H
// #define MUDUO_NET_POLLER_POLLPOLLER_H

#include "../base/Timestamp.h"
#include "Channel.h"

#include <vector>
#include <map>
#include "EventLoop.h"

struct pollfd;

namespace muduo
{
namespace net
{

///
/// IO Multiplexing with poll(2).
///
class PollPoller //: public Poller
{
 public:

      typedef std::vector<Channel*> ChannelList;

      PollPoller(EventLoop* loop);
      ~PollPoller();// override;

      Timestamp poll(int timeoutMs, ChannelList* activeChannels);//override;
      void updateChannel(Channel* channel);// override;
      void removeChannel(Channel* channel);// override;

      void assertInLoopThread() const { ownerLoop_->assertInLoopThread(); }

      bool hasChannel(Channel * channel) const 
      {
            assertInLoopThread();
            ChannelMap::const_iterator it =  channels_.find(channel->fd());
            return it != channels_.end() && it->second == channel;
      }


 private:
  void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;

  typedef std::vector<struct pollfd> PollFdList;
  PollFdList pollfds_;

  EventLoop* ownerLoop_;

  protected:
        typedef std::map<int, Channel*> ChannelMap;
        ChannelMap channels_;
};

}  // namespace net
}  // namespace muduo
// #endif  // MUDUO_NET_POLLER_POLLPOLLER_H
