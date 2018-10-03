//
// Created by wj on 18-10-3.
//

#ifndef NET_CALLBACKS_H
#define NET_CALLBACKS_H

#include <functional>
class Eventloop;
typedef std::function<void(Eventloop *)> ThreadInitCallback;
#endif //NET_CALLBACKS_H
