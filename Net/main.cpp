#include <iostream>
#include "TcpServer.h"
#include "Tcpcoonetion.h"
using namespace std;
using namespace ZL;
using namespace ZL::Net;
class test
{
public:
    test(Eventloop *loop,inetAddress &address,std::string name):tcpServer(new TcpServer(loop,address,name))
    {

    }
    void start()
    {

        tcpServer->start();
    }

private:
    void cooncallback(TcpcoontionPrt &coon)
    {
        cout<<coon->get_name()<<endl;
    }
    void meassgcallback(TcpcoontionPrt &coon,Buffer*buffer,int m)
    {
        cout<<coon->get_name()<<endl;
    }
    void writecallback(TcpcoontionPrt &coon)
    {
        cout<<coon->get_name()<<endl;
    }
    std::shared_ptr<TcpServer> tcpServer;
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    inetAddress address("127.0.0.1",8080);
    Eventloop loop;
    test new_test(&loop,address,"testserver");
    new_test.start();

    return 0;
}