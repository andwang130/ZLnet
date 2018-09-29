//
// Created by wj_de on 18-9-28.
//

#include "Tcpcoonetion.h"
#include "Channel.h"
#include <unistd.h>
using  namespace ZL;
using namespace Net;
Tcpcoonetion::Tcpcoonetion(Eventloop *loop ,int fd):loop_(loop),
                                                    channel_(new Channel(loop,fd)),
                                                    socketfd(fd),
                                                    scoektprt(new Socket(fd)),
                                                    state_(kConnecting)
{
    channel_->setcloseCallbck(std::bind(&Tcpcoonetion::handleColse,this));
    channel_->setwriteCallbck(std::bind(&Tcpcoonetion::handlewrite,this));
    channel_->setreadCallbck(std::bind(&Tcpcoonetion::handlerread,this,_1));
    channel_->seteeorCallbck(std::bind(&Tcpcoonetion::handleeeor,this));
    scoektprt->setKeepAlive(true);
}
void Tcpcoonetion::set_closecallback(const CloseCallback &cb)
{
    closecallback=cb;
}
void Tcpcoonetion::set_coonCallback(const ConnectionCallback &cb)
{
    coonCallback=cb;
}
void Tcpcoonetion::set_writecallback(const WriteCompleteCallback &cb)
{
    writecallback=cb;
}
void Tcpcoonetion::set_messageCallback(const MessageCallback &cb)
{
    messageCallback=cb;
}
void Tcpcoonetion::handleColse()
{
    channel_->disableAll();

    TcpcoontionPrt tcpPrt=shared_from_this();

    coonCallback(tcpPrt);

    closecallback(tcpPrt);

}
void Tcpcoonetion::handlewrite()
{


    if(channel_->isWriting())
    { //注册了可写事件
       ssize_t  n=write(channel_->get_fd(),ouptBuffer.peek(),ouptBuffer.readableBytes());
       if(n>0)
       {   //写入了N个字节，缓存区的下标向后挪动N个字节
           ouptBuffer.retrieve(n);
           //readableBytes返回剩余的字节
           if(ouptBuffer.readableBytes()==0)
           {    //缓冲区的数据全部发送完了,关闭可写事件的监听
               channel_->disableWriting()
               if(writecallback)
               {
                   loop_->queueInLoop(std::bind(writecallback,shared_from_this()))//回调函数传入线程队列
               }
               if(state_==kDisconnecting)
               {
                   shutdownInLoop();
               }
           }
       }
       else
       {
         //log
       }


    }
    else
    {
        //log
    }

}


//Channel接受到可读取事件后调用
void Tcpcoonetion::handlerread(int timeMS)
{

    int savedErrno = 0;

    //Buffer类的readFd函数读取数据。存入缓存区
    ssize_t n = inputBuffer.readFd(channel_->get_fd(), &savedErrno);

    if(n>0)//读取字节大于0，有数据，执行读取事件的回调函数
    {
        if(messageCallback)
        {
            messageCallback(shared_from_this(),&inputBuffer,timeMS);
        }

    }
    else if(n==0) //返回0，连接已经关闭，调用关闭函数
    {
        handleColse();
    }
    else  //小于0发生错误,执行错误函数
    {
        errno = savedErrno;
        handleeeor();
    }

}
void Tcpcoonetion::handleeeor()
{
    //int err = getSocketError(channel_->fd());

}
void Tcpcoonetion::shutdownInLoop()
{

    if(!channel_->isWriting())//
    {
        scoektprt->shutdownwrit();
    }

}
void sendloop()
{

}
void Tcpcoonetion::setstate(StateE st)
{

        state_=st;

}
void Tcpcoonetion::connectEstablished()
{

    setstate(kConnected); //状态设置为监听种
    //开启可读事件监听
    channel_->enableReading();
    if(coonCallback)
    {
        coonCallback(shared_from_this());
    }


}
void Tcpcoonetion::connectDestroyed()
{
    if(state_==kConnected)
    {
        setstate(kDisconnected);
        channel_->disableAll();
        coonCallback(shared_from_this());
    }
    channel_->remove();
}