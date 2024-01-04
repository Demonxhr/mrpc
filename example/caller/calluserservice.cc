#include <iostream>
#include "mrpcapplication.h"
#include "user.pb.h"
#include "mrpcchannel.h"
int main(int argc,char **argv)
{
    //整个程序启动以后，想使用mrpc框架，要先调用初始化
    MrpcApplication::Init(argc,argv);

    //调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new MrpcChannel());
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");

    fixbug::LoginResponse response;

    // rpcchannel->rpcchannel::callmethod  集中做所有处理
    // 发起rpc方法调用  同步rpc调用过程
    stub.Login(nullptr,&request,&response,nullptr);

    // 一次rpc调用完成，读调用结果
    if(0 == response.result().errcode())
    {
        std::cout << "rpc login response success:" << response.sucess() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error:" << response.result().errmsg() << std::endl;
    }

    fixbug::RegisterRequest Rrequest;
    Rrequest.set_id(1);
    Rrequest.set_name("li si");
    Rrequest.set_pwd("123456");

    fixbug::RegisterResponse Rresponse;

    stub.Register(nullptr,&Rrequest,&Rresponse,nullptr);
        // 一次rpc调用完成，读调用结果
    if(0 == Rresponse.result().errcode())
    {
        std::cout << "rpc register response success:" << Rresponse.success() << std::endl;
    }
    else
    {
        std::cout << "rpc register response error:" << Rresponse.result().errmsg() << std::endl;
    }

    return 0;
}