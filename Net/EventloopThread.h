//
// Created by wj on 18-10-2.
//

#ifndef NET_EVENTLOOPTHREAD_H
#define NET_EVENTLOOPTHREAD_H

#include "Mboost.h"
#include <thread>
#include "Eventloop.h"

//Eventloop对应的信线程，EventloopThread有一个线程和Evevtloop对象
using namespace ZL;
using namespace ZL::Net;

class EventloopThread:Mboost::noncopyable
{

 EventloopThread();

private:
    Eventloop *loop_ ;
    std::thread thread_;

};


#endif //NET_EVENTLOOPTHREAD_H
