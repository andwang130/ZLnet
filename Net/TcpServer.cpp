//
// Created by wj on 18-9-26.
//

#include "TcpServer.h"
#include "Acceptor.h"
#include "EventloopThreadpool.h"
#include "Eventloop.h"
#include "Tcpcoonetion.h"
#include <assert.h>
using namespace ZL;
using namespace ZL::Net;
TcpServer::TcpServer(Eventloop *loop, inetAddress &addrs, std::string name,
                    Option option):
        loop_(loop),
        acceptor_(new Acceptor(loop,addrs,option==kNoReusePort)),
        eventloopThreadpool_(new EventloopThreadpool(loop,name))
{
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::new_connection,this,std::placeholders::_1,std::placeholders::_2));

}
TcpServer::~TcpServer()
{

}

void TcpServer::start()
{
    eventloopThreadpool_->start(threadInitCallback_);
    acceptor_->listen();
    loop_->loop();
}
void TcpServer::new_connection(int fd,const  inetAddress &addr)
{
    std::cout<<"TCPserver,new_connection"<<std::endl;
    Eventloop *loop=eventloopThreadpool_->get_Nextloop();
    char buf[64];
    snprintf(buf, sizeof(buf),"%s:%d",addr.get_ip().c_str(),addr.get_port());

    TcpcoontionPrt tcpcoontionPrt(new Tcpcoonetion(loop,fd,buf));
    tcpcoontionPrt->set_closecallback(std::bind(&TcpServer::removeConnection,this,std::placeholders::_1));
    tcpcoontionPrt->set_messageCallback(messageCallback_);
    tcpcoontionPrt->set_writecallback(writeCompleteCallback_);
    tcpcoontionPrt->set_coonCallback(connectionCallback_);
    nextConnId_++;
    coonections_[buf]=tcpcoontionPrt;
    //开启监听函数加入任务队列
    loop->runinLoop(std::bind(&Tcpcoonetion::connectEstablished,tcpcoontionPrt));


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
    assert(num>=0);
    threadnumber=num;
    eventloopThreadpool_->set_Threadnumbre(num);
}


void TcpServer::removeConnection(const TcpcoontionPrt &tcprt)
{
    std::cout<<"Tcpsercer,removeConnection"<<std::endl;
    loop_->runinLoop(std::bind(&TcpServer::removeConnectionInLoop,this,tcprt));

}

void TcpServer::removeConnectionInLoop(const TcpcoontionPrt &tcprt)
{
    std::cout<<"Tcpsercer,removeConnectionInLoop"<<std::endl;
    size_t  n=coonections_.erase(tcprt->get_name());
    Eventloop *loop=tcprt->get_loop();
    loop->runinLoop(std::bind(&Tcpcoonetion::connectDestroyed,tcprt));

}
