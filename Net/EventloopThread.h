//
// Created by wj on 18-10-2.
//

#ifndef NET_EVENTLOOPTHREAD_H
#define NET_EVENTLOOPTHREAD_H

#include "Mboost.h"
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include "Callbacks.h"
//Eventloop对应的信线程，EventloopThread有一个线程和Evevtloop对象
namespace ZL {
namespace Net {
class EventloopThread : Mboost::noncopyable {

public:
    EventloopThread(const ThreadInitCallback &cb, std::string name);

    ~EventloopThread();

    Eventloop *starLoop();

private:
    void theradfun();

    Eventloop *loop_;
    std::thread thread_;
    std::mutex MutexLock;
    std::string name_;
    //condition_variabley用于线程之间的通信
    std::condition_variable cond;
    //线程启动后的回调函数
    ThreadInitCallback callback_;
        };
    }
}

#endif //NET_EVENTLOOPTHREAD_H
