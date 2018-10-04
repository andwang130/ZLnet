//
// Created by wj_de on 18-9-27.
//

#ifndef NET_EPOOLPOLLER_H
#define NET_EPOOLPOLLER_H

#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <vector>
namespace ZL {
namespace Net{
class Channel;
typedef std::map<int,Channel *> Channel_map;
typedef std::vector<Channel *> ChannelList;
typedef std::vector<struct epoll_event> Eventlist;
class Epollpoller {

public:
    Epollpoller();
    int poll(int timeoutMs,ChannelList &channells);
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel* channel) const;
private:
    void update(int option ,Channel *channel);
    Channel_map channels_;
    int epoolfd;
    Eventlist eventlist;

};
}
}


#endif //NET_EPOOLPOLLER_H
