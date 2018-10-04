//
// Created by wj on 18-10-3.
//

#include "EventloopThreadpool.h"
#include <assert.h>
#include "Eventloop.h"
#include "EventloopThread.h"
using namespace ZL;
using namespace ZL::Net;
EventloopThreadpool::EventloopThreadpool(Eventloop *loop, std::string name):
baseloop(loop),
name_(name),
Threadnumbre(0),
Next_(0),
started_(false)
{

}
EventloopThreadpool::~EventloopThreadpool()
{

}

void EventloopThreadpool::start(const ThreadInitCallback &cb)
{
    //不可以重复执行start
    assert(!started_);
    started_= true;
    for(int i=0;i<Threadnumbre;i++)
    {
        char buf[name_.size()+32];
        snprintf(buf, sizeof(buf),"%s%d",name_.c_str(),i);

        EventloopThread *loopthread=new EventloopThread(cb,buf);
        EventloopThreads_.push_back(loopthread);
        EVentloops_.push_back(loopthread->starLoop());
    }
    if(Threadnumbre==0&&cb)
    {
      cb(baseloop);
    }
}

Eventloop* EventloopThreadpool::get_Nextloop()
{

    Eventloop *loop= nullptr;
    if(!EVentloops_.empty())
    {
        loop=EVentloops_[Next_];
        Next_++;
        if(Next_>=EVentloops_.size())
        {
            Next_=0;
        }
    }
    else
    {
        loop=baseloop;
    }
    return loop;
}
Eventloop* EventloopThreadpool::get_hashloop(size_t code)
{
    if(!EVentloops_.empty())
    {
        return EVentloops_[EVentloops_.size()%code];
    }
    else
    {
        baseloop;
    }
}
std::vector<Eventloop *> EventloopThreadpool::get_allloop()
{
    if(!EVentloops_.empty())
    {
        return EVentloops_;
    }
    else
    {
        return std::vector<Eventloop *>(1,baseloop);
    }
}

std::string EventloopThreadpool::get_name()
{
    return name_;
}
bool EventloopThreadpool::get_started()
{
    return started_;
}
void EventloopThreadpool::set_Threadnumbre(int num)
{
    Threadnumbre=num;
}