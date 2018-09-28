//
// Created by wj_de on 18-9-27.
//

#include "Eventloop.h"
#include "Epollpoller.h"
#include "Channel.h"
#include <assert.h>
using namespace ZL;
using namespace ZL::Net;
const int timeOutMS=1000;
Eventloop::Eventloop():poller(new Epollpoller()),
                       quit_(true),
                       currentActiveChannel_(nullptr)
{

}

void Eventloop::loop()
{
    while (quit_)
    {

        channells.clear();
        int timeMs=poller->poll(timeOutMS,channells);
        if(channells.empty())
        {   Handling_= true;
            for(std::vector<Channel *>::iterator ite=channells.begin();ite!=channells.end();ite++)
            {
                currentActiveChannel_=*ite;
                currentActiveChannel_->handleEvent(timeMs);
            }
            currentActiveChannel_= nullptr;
        }
        Handling_= false;
    }
}
void Eventloop::updateChannel(Channel *channel)
{
    pooler->updateChannel(channel);
}
void Eventloop::removeChannel(Channel *channel)
{
    if(Handling_) //在执行相应事件当中，不能移除当前和发生事件的channel
    {
           assert(channel==currentActiveChannel_||
           std::find(channells.begin(),channells.end(),channel)==channells.end());
    }
    poller->removeChannel(channel);

}