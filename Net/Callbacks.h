//
// Created by wj_de on 18-10-3.
//

#ifndef NET_CALLBACKS_H
#define NET_CALLBACKS_H
#include <functional>
#include <memory>
namespace ZL {
namespace Net {
    class Tcpcoonetion;

    class Eventloop;

    class Buffer;

    typedef std::shared_ptr<Tcpcoonetion> TcpcoontionPrt;
    typedef std::function<void(const TcpcoontionPrt &)> ConnectionCallback;
    typedef std::function<void(const TcpcoontionPrt &)> CloseCallback;
    typedef std::function<void(const TcpcoontionPrt &)> WriteCompleteCallback;

    typedef std::function<void(const TcpcoontionPrt &, Buffer *, int)> MessageCallback;

    typedef std::function<void(const Eventloop *)> ThreadInitCallback;
}
}

#endif //NET_CALLBACKS_H
