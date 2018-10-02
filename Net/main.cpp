#include <iostream>
#include <thread>
#include <functional>
int func_text(int n)
{
    std::cout<<n<<std::endl;
}
class Ctext{

public:
    int i;
    void start()
    {
        t1=std::thread(std::bind(&Ctext::func,this,1));

    }
    void func(int x)
    {
        std::cout<<i<<std::endl;
    }
    ~Ctext(){
        t1.join();
    }
private:
    std::thread t1;
};
int main() {
    std::cout << "Hello, World!" << std::endl;

    Ctext ctext;
    ctext.i=12;
    ctext.start();

    return 0;
}