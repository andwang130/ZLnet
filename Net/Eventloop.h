//
// Created by wj_de on 18-9-27.
//

#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include<iostream>
#include <vector>
namespace ZL {
namespace Net {
class Epollpoller;
class Channel;

class Eventloop {
public:
    Eventloop();
    void loop();
    void updateChannel(Channel *channel);
private:
    std::unique_ptr<Epollpoller> poller; //unique_ptr智能指针。无法复制的指针
    bool quit_;
    std::vector<Channel *> channells;

};
    }
}


#endif //NET_EVENTLOOP_H
