//
// Created by wj_de on 18-9-27.
//

#include "Eventloop.h"
#include "Channel.h"
#include <assert.h>
#include "CurrentThread.h"
#include <sys/eventfd.h>

using namespace ZL;
using namespace ZL::Net;
const int timeOutMS=1000;

int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);  //获得一个文件描述符
    if (evtfd < 0)
    {
        //LOG_SYSERR << "Failed in eventfd";
        abort();
    }
    return evtfd;
}

Eventloop::Eventloop():poller(new Epollpoller()),
                       quit_(false),
                       looping_(false),
                       callingPendingFunctors(false),
                       threadId_(CurrentThread::tid()),
                       currentActiveChannel_(nullptr),
                       eventHandling_(false),
                       wakeupFd_(createEventfd()),
                       wakeupChannel_(new Channel(this,wakeupFd_))

{

wakeupChannel_->setreadCallbck(std::bind(&Eventloop::readhanel,this,std::placeholders::_1));//设置可读回调函数
wakeupChannel_->enableReading();//开启可读事件监听

}
Eventloop::~Eventloop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
}
void Eventloop::loop()
{   looping_= true;
    quit_=false;
    while (!quit_)
    {

        channells.clear();
        int timeMs=poller->poll(timeOutMS,channells);
        if(!channells.empty())
        {  printActiveChannels();//输出一下发生的事件
            eventHandling_= true;  //处理事件中
            for(std::vector<Channel *>::iterator ite=channells.begin();ite!=channells.end();ite++)
            {
                currentActiveChannel_=*ite;
                currentActiveChannel_->handleEvent(timeMs);
            }
            currentActiveChannel_= nullptr;
            eventHandling_= false;  //处理完成
            doPendingFunctors();
        }

        looping_ = false;
    }
}
void Eventloop::quit()
{
    quit_= true;
    if(!isInLoopThread())
    {
        wakeup();
    }

}
void Eventloop::updateChannel(Channel *channel)
{
    assert(channel->ownerLoop() == this);
    poller->updateChannel(channel);
}
void Eventloop::removeChannel(Channel *channel)
{   assert(channel->ownerLoop() == this);
    if(eventHandling_) //在执行相应事件当中，不能移除当前和发生事件的channel
    {
           //assert(channel==currentActiveChannel_||std::find(channells.begin(),channells.end(),channel)==channells.end());
    }
    poller->removeChannel(channel);

}
bool Eventloop::isInLoopThread() const
{
    return threadId_==CurrentThread::tid();
}

void Eventloop::runinLoop(const Functor &cb) {
    if(isInLoopThread())
    {
        cb();
    }
    else
    {
        queueInLoop(cb);
    }
}
void Eventloop::queueInLoop(const Functor &cb)
{
    {
        std::lock_guard<std::mutex> lockGuard(MutexLock);

        pendingFunctors_.push_back(cb);//添加任务
    }
    if(!isInLoopThread()||callingPendingFunctors)
    {
        wakeup();
    }

}

void Eventloop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = ::write(wakeupFd_, &one, sizeof one);
    if (n != sizeof one)
    {
        //log
    }
}


void Eventloop::readhanel(int m)
{
    uint64_t one = 1;
    ssize_t n = ::read(wakeupFd_, &one, sizeof one);
    if (n != sizeof one)
    {
        //LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
    }
}

void Eventloop::doPendingFunctors()
{
    //创建一个新的vector来存任务队列
    std::vector<Functor> newpendingFunctors_;
    callingPendingFunctors= true;
    {
        std::lock_guard<std::mutex> lockGuard(MutexLock);
        newpendingFunctors_.swap(pendingFunctors_);//交换值
    }
    for(int i=0;i<newpendingFunctors_.size();i++)
    {  //执行任务队列中的函数
        newpendingFunctors_[i]();
    }
    callingPendingFunctors= false;

}
void Eventloop::printActiveChannels() const
{
    //输出发生的事件
//   for(auto i::channells)
//   {
//       const Channel* ch = *it;
//
//   }
}
bool Eventloop::hasChannel(Channel* channel)
{
    assert(channel->ownerLoop() == this);
    return poller->hasChannel(channel);
}

