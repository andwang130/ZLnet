//
// Created by wj on 18-10-2.
//

#ifndef NET_EVENTLOOPTHREAD_H
#define NET_EVENTLOOPTHREAD_H

#include "Mboost.h"
#include <thread>
#include "Eventloop.h"
#include <mutex>
#include <functional>
#include <condition_variable>
//Eventloop对应的信线程，EventloopThread有一个线程和Evevtloop对象
using namespace ZL;
using namespace ZL::Net;

class EventloopThread:Mboost::noncopyable
{
    typedef std::function<void(Eventloop *)> ThreadInitCallback;

 EventloopThread(ThreadInitCallback &cb);
 ~EventloopThread();
 Eventloop * starLoop();
private:
    void theradfun();
    Eventloop *loop_ ;
    std::thread thread_;
    std::mutex MutexLock;

    //condition_variabley用于线程之间的通信
    std::condition_variable cond;
     //线程启动后的回调函数
    ThreadInitCallback callback_;
};


#endif //NET_EVENTLOOPTHREAD_H
