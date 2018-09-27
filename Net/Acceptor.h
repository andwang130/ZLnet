//
// Created by wj on 18-9-26.
//

#ifndef NET_ACCEPTOR_H
#define NET_ACCEPTOR_H

#include <iostream>
#include "Mboost.h"
#include <functional>
//Acceptor类主要监控连接
namespace  ZL {
    class InetAddress; //类的向前申明
    class EventLoop;
    typedef std::function<void (int socketfd,const InetAddress &)> NewConnectionCallback;
class Acceptor: public Mboost::noncopyable
{

public:
        //构造函数
        Acceptor(EventLoop *loop,InetAddress &inetAddress, bool reuseport);

        ~Acceptor();

        //设置一个新的连接来到时的回调函数
         void setNewConnectionCallback(NewConnectionCallback &cb);

        bool listenning() const { return listenning_; }
private:
        EventLoop *loop;


};
}

#endif //NET_ACCEPTOR_H
