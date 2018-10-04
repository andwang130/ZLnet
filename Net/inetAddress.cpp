//
// Created by wj_de on 18-9-27.
//

#include "inetAddress.h"
#include <arpa/inet.h>
#include "assert.h"
using namespace ZL;
using namespace ZL::Net;
inetAddress::inetAddress(const char *ip, int port)
{
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ip);
//    if (::inet_pton(AF_INET, ip,addr.sin_addr) <= 0)
//    {
//        //LOG_SYSERR << "sockets::fromIpPort";
//    }
}
inetAddress::inetAddress(const sockaddr_in addr_)
{
    addr = addr_;
}
inetAddress::inetAddress(void)
{

}
void inetAddress::set_addr(const sockaddr_in addr_)
{
    addr=addr_;
}
std::string inetAddress::get_ip() const
{
    char buf[64] = "";
    size_t size=64;
    ::inet_ntop(AF_INET, &addr.sin_addr, buf, static_cast<socklen_t>(size));
    return buf;
}
uint16_t inetAddress::get_port() const
{
    return be16toh(addr.sin_port);
}
sockaddr * inetAddress::getSockAddr()
{
    return (struct sockaddr*)&addr;
}

void inetAddress::toIp(char* buf, size_t size,
          const struct sockaddr_in* addr)
{
//

}
sockaddr_in inetAddress::get_addr()
{
    return addr;
}