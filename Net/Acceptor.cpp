//
// Created by wj on 18-9-26.
//

#include "Acceptor.h"
#include "Channel.h"
#include "Eventloop.h"

using namespace ZL;
using namespace ZL::Net;
Acceptor::Acceptor(Eventloop *loop,inetAddress &inetAddress, bool reuseport): socket_(inetAddress.addr),
_loop(loop),
acceptChannel_(new Channel(loop,socket_.get_fd())),
listenning_(false)
{

    socket_.setReuseAddr(true);
    socket_.setReusePort(reuseport);
    socket_.bind();
    acceptChannel_->setreadCallbck(std::bind(&Acceptor::handleread,this,_1));
}
void Acceptor::handleread(int m)
{

}
void Acceptor::listen()
{
    socket_.listen();

}

void Acceptor::setNewConnectionCallback(NewConnectionCallback &cb)
{

}
bool Acceptor::listenning() const
{ return listenning_;

}

