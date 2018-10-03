//
// Created by wj on 18-9-26.
//

#include "TcpServer.h"
#include "Acceptor.h"
#include "EventloopThreadpool.h"
#include "Eventloop.h"
using namespace ZL;
using namespace ZL::Net;
TcpServer::TcpServer(Eventloop *loop, inetAddress &addrs, std::string name,
                     ZL::Net::TcpServer::Option option):
        loop_(loop),
        acceptor_(new Acceptor(loop,addrs,option==kNoReusePort)),
        eventloopThreadpool_(new EventloopThreadpool(loop,name))
{

}
TcpServer::~TcpServer()
{

}

void TcpServer::start()
{

}

void TcpServer::set_MessageCallback(const MessageCallback &cb)
{
    messageCallback_=cb;
}

//void set_CloseCallback(const CloseCallback &cb);

void TcpServer::set_WriteCompleteCallback(const WriteCompleteCallback &cb )
{
    writeCompleteCallback_=cb;
}

void TcpServer::set_ConnectionCallback(const ConnectionCallback &cb)
{
    connectionCallback_=cb;
}

void TcpServer::set_ThreadInitCallback(const ThreadInitCallback &cb)
{
    threadInitCallback_=cb;
}

void TcpServer::set_threadnumber(int num)
{
    threadnumber=num;
}


void TcpServer::removeConnection(const TcpcoontionPrt &tcprt)
{
    loop_->runinLoop(std::bind(&TcpServer::removeConnectionInLoop,this,tcprt));

}

void TcpServer::removeConnectionInLoop(const TcpcoontionPrt &tcprt)
{

}
