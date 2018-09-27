//
// Created by wj_de on 18-9-27.
//

#include "Channel.h"
using namespace ZL;
using namespace ZL::Net;

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