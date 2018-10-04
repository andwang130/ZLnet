//
// Created by wj on 18-9-26.
//

#include "Acceptor.h"
#include "Channel.h"
#include "Eventloop.h"
#include "inetAddress.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace ZL;
using namespace ZL::Net;
Acceptor::Acceptor(Eventloop *loop,inetAddress &address, bool reuseport): socket_(address.addr),
_loop(loop),
acceptChannel_(new Channel(loop,socket_.get_fd())),
listenning_(false),
idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC))

{

    socket_.setReuseAddr(true);
    socket_.setReusePort(reuseport);
    socket_.bind();
    acceptChannel_->setreadCallbck(std::bind(&Acceptor::handleread,this,std::placeholders::_1));
}
void Acceptor::handleread(int m)
{
    inetAddress inetaddr;
    int coonfd=socket_.accept(inetaddr);
    if(coonfd>=0)
    {
        if(newConnectionCallback_)
        {
            newConnectionCallback_(coonfd,inetaddr);
        }
        else
        {
            ::close(socket_.get_fd());
        }

    }
    else
    {

            //LOG_SYSERR << "in Acceptor::handleRead";
            // Read the section named "The special problem of
            // accept()ing when you can't" in libev's doc.
            // By Marc Lehmann, author of libev.
            if (errno == EMFILE)
            {
                ::close(idleFd_);
                idleFd_ = ::accept(socket_.get_fd(), NULL, NULL);
                ::close(idleFd_);
                idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
            }

    }
}
void Acceptor::listen()
{
    listenning_ = true;
    socket_.listen();
    //开启可读事件监听
    acceptChannel_->enableReading();

}

void Acceptor::setNewConnectionCallback(const NewConnectionCallback &cb)
{
    newConnectionCallback_=cb;
}
bool Acceptor::listenning() const
{ return listenning_;

}
Acceptor::~Acceptor()
{
    acceptChannel_->enableReading();
    acceptChannel_->remove();
    ::close(idleFd_);
}
