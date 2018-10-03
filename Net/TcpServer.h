//
// Created by wj on 18-9-26.
//

#ifndef NET_TCPSERVER_H
#define NET_TCPSERVER_H

#include <iostream>
#include <string>
#include "Callbacks.h"
#include <memory>
namespace ZL {
namespace Net {
class Eventloop;
class inetAddress;
class Acceptor;
class EventloopThreadpool;
class TcpServer {
    enum Option
    {
        kNoReusePort,
        kReusePort,
    };
public:
    TcpServer(Eventloop *loop,inetAddress &addrs,std::string name,Option option);
    ~TcpServer();

    void start();
    void set_MessageCallback(const MessageCallback &cb);

    //void set_CloseCallback(const CloseCallback &cb);

    void set_WriteCompleteCallback(const WriteCompleteCallback &cb );

    void set_ConnectionCallback(const ConnectionCallback &cb);

    void set_ThreadInitCallback(const ThreadInitCallback &cb);

    void set_threadnumber(int num);

    //CloseCallback调用
    void removeConnection(const TcpcoontionPrt &tcprt);

    void removeConnectionInLoop(const TcpcoontionPrt &tcprt);
private:

    //acceptor_监听到有新的连接调用
    void new_connection(int fd,inetAddress &addr);
    //主线程的loop;
    Eventloop *loop_;
    //服务器的名字
    std::string name_;
    //ip和和端口
    std::string ipProt;

    //线程数
    int threadnumber;

    //acceptor_分发器，监听和建立连接分发到线程池，让线程池的epoll监听读写
    std::shared_ptr<Acceptor> acceptor_;

    //线程池。事件的处理
    std::shared_ptr<EventloopThreadpool> eventloopThreadpool_;

    //回调函数
    MessageCallback messageCallback_;
    CloseCallback closeCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    ConnectionCallback connectionCallback_;
    ThreadInitCallback threadInitCallback_;

    //连接数
    int nextConnId_;

};
}
}

#endif //NET_TCPSERVER_H
