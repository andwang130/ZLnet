#include <iostream>
#include "TcpServer.h"
#include "Tcpcoonetion.h"
#include <assert.h>
using namespace std;
using namespace ZL;
using namespace ZL::Net;
class test
{
public:
    test(Eventloop *loop,inetAddress &address,std::string name):tcpServer(new TcpServer(loop,address,name))
    {
        tcpServer->set_MessageCallback(std::bind(&test::meassgcallback,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        tcpServer->set_ConnectionCallback(std::bind(&test::cooncallback,this,std::placeholders::_1));
        tcpServer->set_ThreadInitCallback(std::bind(&test::theradinback,this,std::placeholders::_1));
    }
    void start()
    {
        tcpServer->set_threadnumber(4);
        tcpServer->start();
    }

private:
    void cooncallback(const TcpcoontionPrt &coon)
    {
        cout<<"cooncallback"<<endl;
        cout<<coon->get_name()<<endl;
    }
    void meassgcallback(const TcpcoontionPrt &coon,Buffer*buffer,int m)
    {
        cout<<"meassgcallback"<<endl;
        cout<<coon->get_name()<<endl;
        std::string to_buuf=buffer->toStringPiece().data();
        Buffer new_buffer;
        buffer->swap(new_buffer);
        coon->sendloop(to_buuf);
    }
    void writecallback(const TcpcoontionPrt &coon)
    {
        cout<<coon->get_name()<<endl;

    }
    void theradinback(const Eventloop *loop)
    {
        cout<<"1"<<endl;
    }
    std::shared_ptr<TcpServer> tcpServer;
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    inetAddress address("0.0.0.0",8080);
    Eventloop loop;
    test new_test(&loop,address,"testserver");
    new_test.start();

    return 0;
}