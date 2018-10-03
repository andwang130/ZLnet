//
// Created by wj on 18-10-2.
//

#include "EventloopThread.h"
#include "Eventloop.h"
using namespace ZL;
using namespace ZL::Net;
EventloopThread::EventloopThread(const ThreadInitCallback &cb,std::string name):
        loop_(nullptr),
        callback_(cb),
        name_(name)
{

}
Eventloop* EventloopThread::starLoop()
{

    std::unique_lock<std::mutex> uniquelock(MutexLock);  //lock_guard自动释放的锁，出作用域自动释放
    thread_=std::thread(std::bind(&EventloopThread::theradfun,this)); //线程执行
    //当前线程等待，直到loop_不为空的时候
    cond.wait(uniquelock,[this]{ return this->loop_!= nullptr;});
    return loop_;
}

EventloopThread::~EventloopThread()
{
    if(loop_!= nullptr)
    {
        //结束监听
        loop_->quit();
        thread_.join();

    }
}
void EventloopThread::theradfun()
{
    Eventloop loop;
    if(callback_)
    {
        callback_(&loop);
    }
    {
    std::unique_lock<std::mutex> uniquelock(MutexLock);
    loop_=&loop;
    cond.notify_one();//唤醒starLoop函数
    }
    loop.loop();//启动事件监听
    loop_= nullptr;  //loop生命周期结束，loop_指向空的指针
}