//
// Created by wj_de on 18-9-27.
//

#ifndef NET_INETADDRESS_H
#define NET_INETADDRESS_H

#include<netinet/in.h>
namespace ZL {
namespace Net {
class inetAddress {
public:
    inetAddress(const char *ip, int port);
    sockaddr_in addr;
};
}
}
#endif //NET_INETADDRESS_H
