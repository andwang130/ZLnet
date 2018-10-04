//
// Created by wj_de on 18-9-27.
//

#include "Epollpoller.h"
#include "Channel.h"
#include <assert.h>
#include <string.h>
using namespace ZL;
using namespace ZL::Net;
const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;
const int eventnumber=16;

int Epollpoller::poll(int timeoutMs,ChannelList &channells)
{           //&*eventlist.begin()取vecort第一个值的指针,timeoutMs超时时间


    int ret=epoll_wait(epoolfd,&*eventlist.begin(), static_cast<int>(eventlist.size()),timeoutMs);
    int savedErrno = errno;
    if(ret>0)
    {
        for(int i=0;i<ret;i++)
        {
            Channel *channel= static_cast<Channel*>(eventlist[i].data.ptr);
            channel->set_revents(eventlist[i].events);
            channells.push_back(channel);
        }
        if(static_cast<size_t>(ret)==eventlist.size())
        {
            eventlist.resize(eventlist.size()*2);
        }

    }
    else if(ret==0) //为0，过来超时时间
    {


    }
    else  //小于0,出现错误
    {

    }
    return 1;//暂时返回1
}
void Epollpoller::updateChannel(Channel *channel)
{
    int index=channel->get_index();
    int fd=channel->get_fd();
    if(index==kNew||index==kDeleted) //处于new状态和kdeleted状态，在可以添加到epool当中
    {
        if(index==kNew)
        {
        //Knew状态是一个新的连接，不应该出现在channels_，
        assert(channels_.find(fd)==channels_.end());
        //把这个新的连接添加到channels_当中
        channels_[fd]=channel;

        }
        else //index是 kdeleted状态,不是新的但是被删除的
        {   //kdeleted状态下并没有从channels_中删除，应该还在channels_当中
            assert(channels_.find(fd)!=channels_.end());

        }
        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD,channel);

    }
    else //kadd状态，已经在epool当中，更新状态
    {
        assert(channels_.find(fd)!=channels_.end());
        assert(channels_[fd] == channel);
        if(channel->isNoneEvent())//event状态为NoneEvent
        {
            //从epool当中删除
            update(EPOLL_CTL_DEL,channel);
            //状态设置以及删除
            channel->set_index(kDeleted);
        }
        else
        {
            update(EPOLL_CTL_MOD,channel);

        }
    }

}
void Epollpoller::update(int option,Channel *channel)
{
    epoll_event event;
    memset(&event,0, sizeof(event));//将event的内存清零
    event.data.ptr=channel;
    event.events=channel->get_events();
    int fd=channel->get_fd();
    int ret=epoll_ctl(epoolfd,option,fd,&event);
    if(ret<0)
    {
        //出现错误
    }


}
void Epollpoller::removeChannel(Channel *channel)
{
    int fd=channel->get_fd();
    assert(channels_.find(fd)!=channels_.end());
    int index=channel->get_index();

    //kadd，kdeleted两种状态channel才在channels_当中
    assert(index==kAdded||index==kDeleted);
    size_t n=channels_.erase(fd);
    //删除成功返回1
    assert(n==1);
    if(index==kAdded) //kadd状态，还在epool当种。从epool当中删除
    {
        update(EPOLL_CTL_DEL,channel);
    }
    //设置为Knew状态，不在channels_当中，也不在epool当中
    channel->set_index(kNew);

}
Epollpoller::Epollpoller():eventlist(eventnumber)
{
    epoolfd=epoll_create1(EPOLL_CLOEXEC);

}

bool Epollpoller::hasChannel(Channel* channel) const
{
    Channel_map::const_iterator it = channels_.find(channel->get_fd());
    return it != channels_.end() && it->second == channel;
}