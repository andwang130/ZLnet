//
// Created by wj_de on 18-9-27.
//

#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H

#include <iostream>
#include <functional>
namespace ZL {
namespace Net {
class Channel {
    typedef std::function<void()> EventCallback;//定义一个发生事件时的回调函数
public:
    //设置可读事件的回调函数
    void setreadCallbck(const EventCallback &cb);

    //设置可写事件的回调函数
    void setwriteCallbck(const EventCallback &cb);

    //设置关闭事件的回调函数
    void closeCallbck(const EventCallback &cb);

    //设置发生错误事件的回调函数
    void seteeorCallbck(const EventCallback &cb);

private:
    int events_; //事件类型
    const int sockefd;//对应的socket描述符
    EventCallback readCallback;
    EventCallback writeCallbck;
    EventCallback eeorCallbck;
    EventCallback closeCallbck;

    };
}

}
#endif //NET_CHANNEL_H
