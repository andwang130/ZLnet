//
// Created by wj_de on 18-9-27.
//

#include "inetAddress.h"
#include <arpa/inet.h>
inetAddress::inetAddress(const char *ip, int port)
{
    addr.sin_family=AF_INET;
    addr.sin_port=htonl(port);
    if (::inet_pton(AF_INET, ip,addr.sin_addr) <= 0)
    {
        //LOG_SYSERR << "sockets::fromIpPort";
    }
}