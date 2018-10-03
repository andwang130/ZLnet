//
// Created by wj on 18-10-3.
//

#ifndef NET_EVENTTHREADLOOP_H
#define NET_EVENTTHREADLOOP_H

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include "Callbacks.h"
namespace ZL{
namespace Net{
class EventloopThread;

class EventloopThreadpool {

public:
    EventloopThreadpool(Eventloop *loop,std::string name);
    ~EventloopThreadpool();
    void set_Threadnumbre(int num);

    void start(const ThreadInitCallback &cb);

    //Eventloop返回下一个
    Eventloop* get_Nextloop();
    //通过hash获得Eventloop
    Eventloop* get_hashloop(size_t code);
    //返回全部的通过hash获得Eventloop
    std::vector<Eventloop *> get_allloop();

    std::string get_name();

    bool get_started();
private:
    bool started_;//线程池是否启动的状态
    Eventloop *baseloop;//主线程的loop,当程序时但线程的时候返回这个loop
    std::string name_;//线程名字，所有线程会在这个名字后面加上一个数字
    int Threadnumbre; //线程数
    int Next_;//当前分配的Eventloop的集合
    std::vector<EventloopThread *> EventloopThreads_; //线程的集合
    std::vector<Eventloop *> EVentloops_;//Eventloop的集合
};
}
}


#endif //NET_EVENTTHREADLOOP_H
