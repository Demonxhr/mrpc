#pragma once


#include "mrpcconfig.h"
#include "mrpcchannel.h"
#include "mrpccontroller.h"
//mrpc框架的初始化类,基础类，负载框架的一些初始化操作

class MrpcApplication
{
public:
    static void Init(int argc,char **argv);
    static MrpcApplication& GetInstance();
    static MrpcConfig& GetConfig();
private:
    static MrpcConfig m_config;

    //单例对象
    //禁用构造函数
    MrpcApplication(){}
    //禁用拷贝构造函数
    MrpcApplication(const MrpcApplication&) = delete;
    //禁用移动构造函数
    MrpcApplication(const MrpcApplication&&) = delete;
};