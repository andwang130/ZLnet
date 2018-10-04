//
// Created by wj_de on 18-9-27.
//

#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H

#include <iostream>
//#include <Eventloop.h>
#include <functional>

//Channel类对应一个文件描述符和他要监听的事件,和监听到的事件事件对应的回调函数
namespace ZL {
namespace Net {
class Eventloop;
class Channel {
    typedef std::function<void()> EventCallback;//定义一个发生事件时的回调函数
    typedef std::function<void(int)> ReadEventback;
public:
    //构造函数，两个参数。一个改对象属于的Eventloop的指针，和socket描述符
    Channel(Eventloop *loop_,int fd);

    ~Channel();
    //设置可读事件的回调函数
    void setreadCallbck(const ReadEventback &cb);

    //设置可写事件的回调函数
    void setwriteCallbck(const EventCallback &cb);

    //设置关闭事件的回调函数
    void setcloseCallbck(const EventCallback &cb);

    //设置发生错误事件的回调函数
    void seteeorCallbck(const EventCallback &cb);
    //有事件发生时执行的函数，在Eventloop当中执行
    void handleEvent(int time);
    bool isNoneEvent();

    //返回注册事件是否是写入事件
    bool isWriting();

    //返回注册事件是否是读取事件
    bool isReading();
    //从epoll删除
    void disableAll();
    void enableReading();
    void disableReading();
    void enableWriting();
    void disableWriting();
    int get_index();
    int get_fd();
    void set_index(int index );
    int get_events();
    void set_revents(int e);

    //返回当前Channel属于的loop
    Eventloop * ownerLoop();
    void remove();
private:
    Eventloop *loop;//这个对象属于的Eventloop
    void update();
    //Channel的状态，3种状态，初始状态为New_(1),表示新的连接还没有添加到Epoolpoller当中,kaddr(2)在poller当中了,kdeltetd(3)不在pooler当中,
    int index_;
    int events_; //监听的事件类型
    int revents_;//响应的事件类型
    const int sockefd;//对应的socket描述符
    bool eventHandling_;
    bool addedToLoop_;
    ReadEventback readCallback;  //可读事件回调函数
    EventCallback writeCallbck;   //可写事件回调函数
    EventCallback eeorCallbck;   //出现错误的回调函数
    EventCallback closeCallbck;  //关闭事件的回调函数

    };
}

}
#endif //NET_CHANNEL_H
