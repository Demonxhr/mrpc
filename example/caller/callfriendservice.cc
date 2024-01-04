#include <iostream>
#include "mrpcapplication.h"
#include "friend.pb.h"


int main(int argc,char **argv)
{
    //整个程序启动以后，想使用mrpc框架，要先调用初始化
    MrpcApplication::Init(argc,argv);

    //调用远程发布的rpc方法Login
    fixbug::FriendServiceRpc_Stub stub(new MrpcChannel());
    fixbug::GetFriendListRequest request;
    request.set_userid(1);

    fixbug::GetFriendListResponse response;

    // rpcchannel->rpcchannel::callmethod  集中做所有处理
    // 发起rpc方法调用  同步rpc调用过程
    MrpcController controller;
    stub.GetFriendList(&controller,&request,&response,nullptr);


    if(controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        // 一次rpc调用完成，读调用结果
        if(0 == response.result().errcode())
        {
            std::cout << "rpc login response success!"  << std::endl;
            int size = response.friends_size();
            for (int i = 0; i < size ; ++i)
            {
                std::cout << "index:" << (i+1) <<" name:" << response.friends(i) << std::endl;
            }
        }
        else
        {
            std::cout << "rpc login response error:" << response.result().errmsg() << std::endl;
        }
    }
    

    return 0;
}