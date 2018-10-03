//
// Created by wj on 18-9-26.
//

#ifndef NET_ACCEPTOR_H
#define NET_ACCEPTOR_H

#include <iostream>
#include "Socket.h"
#include <functional>
//Acceptor类主要监控连接
namespace  ZL {
namespace Net {
class inetAddress; //类的向前申明
class Eventloop;
class Channel;
typedef std::function<void(int socketfd, const inetAddress &)> NewConnectionCallback;

class Acceptor : Mboost::noncopyable
{
public:
    //构造函数
    Acceptor(Eventloop *loop, inetAddress &address, bool reuseport);

    ~Acceptor();

    //设置一个新的连接来到时的回调函数
    void setNewConnectionCallback(const NewConnectionCallback &cb);

    //返回listen的状态
    bool listenning() const ;

    //执行socket的listen函数
    void listen();
private:
    void handleread(int m);

    Eventloop *_loop;
    Socket socket_;  //scoket类，封装的sokcet的系统函数
    Channel *acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listenning_;//是否监听状态。初始化值为flase，在listen之后设置为true
    int idleFd_; //文件描述符


};
    }
}

#endif //NET_ACCEPTOR_H
