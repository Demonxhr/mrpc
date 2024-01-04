#include <iostream>


class singleton_test
{
public:
    void get_aa()
    {
        std::cout << aa << std::endl;
        aa++;
    }
    static singleton_test& GetInstance(){
        static singleton_test test;
        return test;
    }
private:
    int aa = 1;
    singleton_test(){}
    singleton_test(const singleton_test &) = delete;
    singleton_test(const singleton_test &&) = delete;
};

int main()
{
    singleton_test& aa = singleton_test::GetInstance();
    aa.get_aa();
    singleton_test& bb = singleton_test::GetInstance();
    bb.get_aa();
    return 0;
}