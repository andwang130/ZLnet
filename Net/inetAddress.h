//
// Created by wj_de on 18-9-27.
//

#ifndef NET_INETADDRESS_H
#define NET_INETADDRESS_H

#include<netinet/in.h>
#include <iostream>
namespace ZL {
namespace Net {
class inetAddress {
public:
    inetAddress(void);
    inetAddress(const char *ip, int port);
    inetAddress(const sockaddr_in addr_);

    std::string get_ip() const ;
    uint16_t get_port() const ;
    void set_addr(const sockaddr_in addr_);
    sockaddr_in get_addr();
    sockaddr_in addr;
private:
    sockaddr * getSockAddr();

    void toIp(char* buf, size_t size, const struct sockaddr_in * addr);

};
}
}
#endif //NET_INETADDRESS_H
