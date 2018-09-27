//
// Created by wj on 18-9-26.
//

#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include <netinet/in.h>
#include "Mboost.h"

//socket类，封装了操作的socket类

class Socket:Mboost::noncopyable{

public:
    Socket(const char *ip,int port);
    ~Socket();
    void bind();
    void Connect();
    void listen();
    int accept();
    //关闭套节字的写
    void shutdownwrit();

    //关闭读
    void shutdownread();

    //关闭读和写
    void shudownRDWR();

    //设置TCP_NODELAY
    void setTcpNoDelay(bool on);


    //设置端口可以复用
    void setReuseAddr(bool on);

    void setReusePort(bool on);

    void setKeepAlive(bool on);

private:
     int socketfd;
     sockaddr_in server_in;
};


#endif //NET_SOCKET_H
