//
// Created by wj_de on 18-9-28.
//

#ifndef NET_TCPCOONTION_H
#define NET_TCPCOONTION_H

#include <iostream>
#include <functional>
#include <memory>
#include "Socket.h"
#include "Buffer.h"
namespace ZL{
namespace Net {
class Channel;
class Eventloop;

//继承enable_shared_from_this类,可以使用shared_from_this函数来获取this的shared_ptr指针
class Tcpcoonetion:std::enable_shared_from_this<Tcpcoonetion>
        {
    typedef std::shared_ptr<Tcpcoontion> TcpcoontionPrt;
    typedef std::function<void (const TcpcoontionPrt&)> ConnectionCallback;
    typedef std::function<void(const TcpcoontionPrt&)> CloseCallback;
    typedef std::function<void (const TcpcoontionPrt&)> WriteCompleteCallback;
    typedef std::function<void(const TcpcoontionPrt&,Buffer *buffer,int timeMs)> MessageCallback;
public:
    Tcpcoonetion(Eventloop *loop,int fd);
    void send();
    //设置回调函数
    void set_coonCallback(const ConnectionCallback &cb);

    void set_closecallback(const CloseCallback &cb);

    void set_writecallback(const WriteCompleteCallback & cb);

    void set_messageCallback(const MessageCallback &cb);

    //启动监听可读事件
    void connectEstablished();

    //关闭监听事件，从eventloop中移除Channel
    void connectDestroyed();

private:

    enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };
    void setstate(StateE st);
    //关闭事件回调函数，注册到Channel当中
    void handleColse();

    //可写事件回掉函数
    void handlewrite();

    //可读事件回调函数
    void handlerread(int timeMS);

    //错误事件回调函数
    void handleeeor();


    void shutdownInLoop();

    //每一个coonet都有一个Channel来连接Eventloop和epool
    std::shared_ptr<Channel> channel_;
    //该连接所属的Eventloop
    Eventloop *loop_;

    //建立连接的回掉函数.tcpserver注册
    ConnectionCallback coonCallback;

    //关闭事件回调函数
    CloseCallback closecallback;

    //写入事件
    WriteCompleteCallback writecallback;

    //读取事件
    MessageCallback messageCallback;

    //该链接的文件描述符
    const int socketfd;
    StateE state_;  //连接所处的状态
    //Socket
    std::shared_ptr<Socket>scoektprt;
    Buffer inputBuffer;  //接受缓存区
    Buffer ouptBuffer;  //发送缓存区
};
}}


#endif //NET_TCPCOONTION_H
