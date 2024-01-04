#include <iostream>
#include <unistd.h>
#include <thread>
using namespace std;

void daemfunc()
{
    int i =0;
    while(++i)
    {
        cout <<"守护线程：" << i << endl;
        sleep(3);
    }
    
}

int main()
{
    thread mt(&daemfunc);
    mt.detach();
    for(int i = 0 ; i<10;++i)
    {
        cout << "主线程：" << i <<endl;
        sleep(3);
    }
    cout << "主线程over" << endl;
    return 0;
}