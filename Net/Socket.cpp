//
// Created by wj on 18-9-26.
//

#include "Socket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
Socket::Socket(const char *ip,int port)
{

    server_in.sin_family=AF_INET;
    server_in.sin_addr.s_addr=inet_addr(ip);
    server_in.sin_port=htons(port);
    socketfd=socket(AF_INET,SOCK_STREAM,0);
}

Socket::~Socket()
{
    ::close(socketfd);

}

void Socket::bind()
{
 int ret=::bind(socketfd,(struct  sockaddr*)&server_in, sizeof(server_in));

 if(ret<0)
 {

 }
}
void Socket::Connect()
{

}
void Socket::listen()
{
    int ret=::listen(socketfd,10);
    if(ret<0)
    {
        //出现错误以后执行的代码
    }
}
int Socket::accept()
{
    sockaddr_in addr;
    socklen_t addrlen = static_cast<socklen_t>(sizeof addr);
#if VALGRIND || defined (NO_ACCEPT4)
    int connfd = ::accept(socketfd, (struct sockaddr *)&addr, &addrlen);
  setNonBlockAndCloseOnExec(connfd);
#else
    int connfd = ::accept4(socketfd, (struct sockaddr *)&addr,&addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif
    if (connfd < 0)
    {
        int savedErrno = errno;
        //LOG_SYSERR << "Socket::accept";
        switch (savedErrno)
        {
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPROTO: // ???
            case EPERM:
            case EMFILE: // per-process lmit of open file desctiptor ???
                // expected errors
                errno = savedErrno;
                break;
            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
                // unexpected errors
                //LOG_FATAL << "unexpected error of ::accept " << savedErrno;
                break;
            default:
               // LOG_FATAL << "unknown error of ::accept " << savedErrno;
                break;
        }
    }
    return connfd;
}
void Socket::shutdownwrit()
{
    //shutdown()是指禁止在一个套接口上进行数据的接收与发送。
    int ret=::shutdown(socketfd,SHUT_WR);
    if(ret<0)
    {

    }
}
void Socket::shutdownread()
{   int ret=::shutdown(socketfd,SHUT_RD);
    if(ret<0)
    {

    }

}
void Socket::shudownRDWR()
{
    int ret=::shutdown(socketfd,SHUT_RDWR);
    if(ret<0)
    {

    }
}
void Socket::setTcpNoDelay(bool on)
{

    int poval=on?1:0; //如果on为true poval赋值为1,flase赋值为0
    ::setsockopt(socketfd,IPPROTO_TCP,TCP_NODELAY,&poval, static_cast<socklen_t >(sizeof(poval)));
}

void Socket::setReuseAddr(bool on)
{

    int poval=on?1:0;
    ::setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,&poval, static_cast<socklen_t >(sizeof(poval)));
}

void Socket::setReusePort(bool on)
{
    int poval=on?1:0;
    int ret=::setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,&poval, static_cast<socklen_t >(sizeof(poval)));
    if(ret<0)
    {

    }
}

void Socket::setKeepAlive(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(socketfd, SOL_SOCKET, SO_KEEPALIVE,
                 &optval, static_cast<socklen_t>(sizeof optval));
}