//
// Created by wj_de on 18-9-27.
//

#include "Channel.h"
#include <sys/epoll.h>
#include "Eventloop.h"
#include <assert.h>
using namespace ZL;
using namespace ZL::Net;

const int NoneEvent = 0;
const int ReadEvent = EPOLLIN | EPOLLPRI;
const int WriteEvent = EPOLLOUT;
Channel::Channel(Eventloop *loop_,int fd):
loop(loop_),sockefd(fd),
eventHandling_(false),
addedToLoop_(false)
{
    index_=-1;
}
Channel::~Channel()
{
    assert(!eventHandling_);
    assert(!addedToLoop_);
    if (loop->isInLoopThread())
    {
        assert(!loop->hasChannel(this));
    }
}
void Channel::setreadCallbck(const ReadEventback &cb)
{
    readCallback=cb;
}
void Channel::setwriteCallbck(const EventCallback &cb)
{
    writeCallbck=cb;
}
void Channel::seteeorCallbck(const EventCallback &cb)
{
    eeorCallbck=cb;
}
void Channel::setcloseCallbck(const EventCallback &cb)
{
    closeCallbck=cb;
}
void Channel::update()
{
    addedToLoop_= true;
    loop->updateChannel(this);
}
int Channel::get_index()
{
    return index_;
}
int Channel::get_fd()
{
    return sockefd;
}
void Channel::set_index(int index)
{
    index_=index;
}
int Channel::get_events()
{
    return events_;
}
bool Channel::isNoneEvent()
{
    //返回是否NoneEvent状态
    return events_==NoneEvent;
}
void Channel::set_revents(int e)
{
    revents_=e;
}
void Channel::handleEvent(int time)
{   eventHandling_= true;
    if(revents_&EPOLLHUP&&!(events_&EPOLLIN))  //对方描述符挂起,并且没有可读事件
    {
        if(closeCallbck)
        {
            closeCallbck();
        }
    }
    if(revents_&EPOLLERR)
    {
        if(eeorCallbck)
        {
            eeorCallbck();
        }
    }
    if(revents_&(EPOLLIN|EPOLLPRI|EPOLLRDHUP))
    {
        if(readCallback)
        {
            readCallback(time);
        }
    }
    if(revents_&EPOLLOUT)
    {
        if(writeCallbck)
        {
            writeCallbck();
        }
    }
    eventHandling_= false;
}
void Channel::disableAll()
{
    events_=NoneEvent;  //事件设置为0
    update();
}

void Channel::disableWriting()
{
   events_&=~WriteEvent;//事件取消一个写入事件
   update();
}
void Channel::enableWriting()
{
    events_|=WriteEvent;  //添加一个写入事件
    update();
}

void Channel::disableReading()
{
    //取消一个读取事件
    events_&=~ReadEvent;
    update();
}
void Channel::enableReading()
{
    //添加一个读取事件
    events_|=ReadEvent;
    update();
}
bool Channel::isWriting()
{
    return events_&WriteEvent;
}
bool Channel::isReading()
{
    return events_&ReadEvent;
}

void Channel::remove()
{   addedToLoop_= false;
    loop->removeChannel(this);
}
Eventloop * Channel::ownerLoop()
{
    return loop;
}