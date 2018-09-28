//
// Created by wj_de on 18-9-27.
//

#include "Eventloop.h"
#include "Epollpoller.h"
using namespace ZL;
using namespace ZL::Net;
const int timeOutMS=1000;
Eventloop::Eventloop():poller(new Epollpoller()),quit_(true)
{

}

void Eventloop::loop()
{
    while (quit_)
    {
        channells.clear();
        poller->poll(timeOutMS,channells);
        if(channells.empty())
        {
            for(atuo i::channells)
            {

            }
        }
    }
}
void Eventloop::updateChannel(Channel *channel)
{
    pooler->updateChannel(channel);
}