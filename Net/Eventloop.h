//
// Created by wj_de on 18-9-27.
//

#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include<iostream>
namespace ZL {
namespace Net {
class Epoolpoller;

class Eventloop {
public:
    Eventloop();

private:
    std::unique_ptr<Epoolpoller> pooler; //unique_ptr智能指针。无法复制的指针


};
    }
}


#endif //NET_EVENTLOOP_H
