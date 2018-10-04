//
// Created by wj_de on 18-9-27.
//

#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include<iostream>
#include <vector>
#include <memory>
#include <mutex>
#include <functional>
#include "Epollpoller.h"
namespace ZL {
namespace Net {
class Channel;

class Eventloop {
    typedef std::function<void()> Functor;
public:
    Eventloop();
    ~Eventloop();
    //执行事件监听
    void loop();

    //
    void quit();

    void updateChannel(Channel *channel);

    void removeChannel(Channel *channel);

    //向任务队列添加一个任务
    void runinLoop(const Functor &cb);

    //添加队列的具体执行
    void queueInLoop(const  Functor &cb);

    //发送信号激活任务队列
    void wakeup();

    //判断分支预测
    bool isInLoopThread()const ;

    //wakeupChannel_的可读回调函数
    void readhanel(int m);

    bool hasChannel(Channel* channel);

    void printActiveChannels() const;
private:
    bool   quit_ ;
    //执行任务队列函数
    void doPendingFunctors();

    mutable std::mutex MutexLock;  //互斥锁

    std::unique_ptr<Epollpoller> poller; //unique_ptr智能指针。无法复制的指针

    int wakeupFd_;//用来激活队列的文件描述符

    //使用一个文件表述符了来通知任务队列
    std::shared_ptr<Channel> wakeupChannel_;//文件描述符对应的Channel
    bool looping_;//是否处于loop状态
    bool eventHandling_;  //是否在执行回调事件状态
    const pid_t threadId_;
    bool callingPendingFunctors;  //任务队列执行状态

    std::vector<Channel *> channells;
    Channel *currentActiveChannel_;
    std::vector<Functor> pendingFunctors_;//任务队列
};
    }
}


#endif //NET_EVENTLOOP_H
