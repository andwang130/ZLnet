//
// Created by wj_de on 18-9-27.
//

#include "Channel.h"
#include <sys/epoll.h>
#include <poll.h>
using namespace ZL;
using namespace ZL::Net;

const int NoneEvent = 0;
const int ReadEvent = EPOLLIN | EPOLLPRI;
const int WriteEvent = EPOLLOUT;
Channel::Channel(Evevtloop *loop_,int fd):loop(loop_),sockefd(fd)
{
    index_=1;
}
void Channel::setreadCallbck(const EventCallback &cb)
{
    readCallback=cb;
}
void Channel::setwriteCallbck(const EventCallback &cb)
{
    writeCallbck=cb;
}
void Channel::seteeorCallbck(const EventCallback &cb)
{
    seteeorCallbck=cb;
}
void Channel::closeCallbck(const EventCallback &cb)
{
    closeCallbck=cb;
}
void Channel::update()
{
    loop->update
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
{
    if(revents_==EPOLLIN)
    {

    }
    else if(revents_==EP)
    {

    }

}