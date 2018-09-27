//
// Created by wj on 18-9-26.
//

#include "Acceptor.h"
using namespace ZL;
using namespace ZL::Net;
Acceptor::Acceptor(EventLoop *loop,inetAddress &inetAddress, bool reuseport): socket_(inetAddress.addr),
_loop(loop),
listenning_(false)
{

    socket_.setReuseAddr(true);
    socket_.setReusePort(reuseport);
    socket_.bind();
}