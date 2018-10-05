//
// Created by wj_de on 18-9-28.
//

#include "Tcpcoonetion.h"
#include "Channel.h"
#include <unistd.h>
#include <assert.h>
using  namespace ZL;
using namespace Net;
Tcpcoonetion::Tcpcoonetion(Eventloop *loop ,int fd,std::string name):loop_(loop),
                                                                name_(name),
                                                    channel_(new Channel(loop,fd)),
                                                    socketfd(fd),
                                                    scoektprt(new Socket(fd)),
                                                    state_(kConnecting)
{
    channel_->setcloseCallbck(std::bind(&Tcpcoonetion::handleColse,this));
    channel_->setwriteCallbck(std::bind(&Tcpcoonetion::handlewrite,this));
    channel_->setreadCallbck(std::bind(&Tcpcoonetion::handlerread,this,std::placeholders::_1));
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
               channel_->disableWriting();
               if(writecallback)
               {
                   loop_->queueInLoop(std::bind(writecallback,shared_from_this()));//回调函数传入线程队列
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
void Tcpcoonetion::sendloop( const StringPiece &message)
{
    sendInLoop(message.data(),message.size());
}
void Tcpcoonetion::sendloop(const  std::string &message)
{
    sendInLoop(message.data(),message.size());
}



void Tcpcoonetion::sendInLoop(const void* data, size_t len)
{
    ssize_t nwrote = 0;
    size_t remaining = len;
    bool faultError = false;
    if (state_ ==kDisconnected)
    {
        //LOG_WARN << "disconnected, give up writing";
        return;
    }
    // if no thing in output queue, try writing directly

    //没有在监听可读事件和缓冲区的数据为空，
    if(!channel_->isWriting()&&ouptBuffer.readableBytes()==0)
    {
         nwrote=::write(channel_->get_fd(),data,len);
         if (nwrote>=0) //发送成功
         {
             remaining=len-nwrote;
             if(remaining==0&&writecallback) //数据全部发送完成
             {

                 loop_->runinLoop(std::bind(writecallback,shared_from_this()));

             }


         }
         else  //发送失败了
         {
             nwrote=0;
             if (errno != EWOULDBLOCK)
             {
                 //LOG_SYSERR << "TcpConnection::sendInLoop";
                 if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
                 {
                     faultError = true;
                 }
             }
         }
    }


    assert(remaining <= len);
    if (!faultError && remaining > 0) //发送失败了，把数据存了缓冲区，下次发送

    {
        size_t oldLen = ouptBuffer.readableBytes();

//        if (oldLen + remaining >= highWaterMark_
//            && oldLen < highWaterMark_
//            && highWaterMarkCallback_)
//        {
//            loop_->queueInLoop(boost::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
//        }
        ouptBuffer.append(static_cast<const char*>(data)+nwrote, remaining);
        if (!channel_->isWriting())
        {
            channel_->enableWriting();
            //注册一个可写事件的监听，在对方读取数据的时候会触发
            //然后调用handlewrite继续写入，直到写完为止
        }
    }
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
std::string Tcpcoonetion::get_name()
{
    return name_;
}
Eventloop * Tcpcoonetion::get_loop()
{
    return loop_;
}