#include "mrpcchannel.h"
#include <string>
#include "rpcheader.pb.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mrpcapplication.h"
#include "mrpccontroller.h"
#include "zookeeperutil.h"

/*
header_size + service_name method_name args_size + args
*/
void MrpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method, 
                            google::protobuf::RpcController *controller, 
                            const google::protobuf::Message *request, 
                            google::protobuf::Message *response, google::protobuf::Closure *done)
{
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    //获取参数的序列化字符串长度 args_size
    std::string args_str;
    int args_size = 0;
    if(request->SerializeToString(&args_str))
    {
        args_size = args_str.size();
    }
    else
    {
        //std::cout << "serialize request error!" << std::endl;
        controller->SetFailed("serialize request error!");
        return ;
    }

    //定于rpc的请求header
    mrpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string rpc_header_str;
    if(rpcHeader.SerializeToString(&rpc_header_str))
    {
        header_size = rpc_header_str.size();
    }
    else
    {
        //std::cout << "serialize rpc header error!" << std::endl;
        controller->SetFailed("serialize rpc header error!");
        return ;
    }


    //组织待发送rpc请求的字符串
    std::string send_rpc_str;
    send_rpc_str.insert(0,std::string((char *)&header_size,4));
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;

    // 打印调试信息
    std::cout << "================================================" << std::endl;
    std::cout << "header_size:" << header_size <<std::endl;
    std::cout << "rpc_header_str:" << rpc_header_str <<std::endl;
    std::cout << "service_name:" << service_name <<std::endl;
    std::cout << "method_name:" << method_name <<std::endl;
    std::cout << "args_str:" << args_str <<std::endl;
    std::cout << "================================================" << std::endl;


    // 使用tcp编程
    int clientfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == clientfd)
    {
        //std::cout << "create socket error! errno:" << errno << std::endl;
        char errtext[512] = {0};
        sprintf(errtext,"create socket error! errno:%d",errno);
        controller->SetFailed(errtext);
        return;
        //exit(EXIT_FAILURE);
    }

    // 读取配置文件rpcserver的信息
    // std::string ip = MrpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    // uint16_t port = atoi(MrpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    ZkClient zkCli;
    zkCli.Start();
    std::string method_path = "/" + service_name + "/" + method_name;
    std::string host_data = zkCli.GetData(method_path.c_str());
    if(host_data == "")
    {
        controller->SetFailed(method_path + " is not exist!");
        return;
    }
    int idx = host_data.find(":");
    if(idx == -1)
    {
        controller->SetFailed(method_path + " address is invalid!");
        return ;
    }
    std::string ip = host_data.substr(0,idx);
    uint16_t port = atoi(host_data.substr(idx+1,host_data.size()-idx).c_str());



    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if(-1 == connect(clientfd,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        //std::cout << "connect error! errono:" << errno << std::endl;
        close(clientfd);
        char errtext[512] = {0};
        sprintf(errtext,"connect error! errono:%d",errno);
        controller->SetFailed(errtext);
        return;
        //exit(EXIT_FAILURE);
    }

    if(-1 == send(clientfd,send_rpc_str.c_str(),send_rpc_str.size(),0))
    {
        //std::cout << "send error! errono:" << errno << std::endl;
        close(clientfd);
        char errtext[512] = {0};
        sprintf(errtext,"send error! errono:%d",errno);
        controller->SetFailed(errtext);
        return;
    }

    //接收rpc请求的响应值
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if(-1 == (recv_size = recv(clientfd,recv_buf,1024,0)))
    {
        //std::cout << "recv error! errono:" << errno << std::endl;
        close(clientfd);
        char errtext[512] = {0};
        sprintf(errtext,"recv error! errono:%d",errno);
        controller->SetFailed(errtext);
        return;
    }

    // recv_buf中存在\0时使用string构造函数会导致字符串在\0处被截断，反序列化失败
    //std::string response_str(recv_buf,0,recv_size);
    //if(!response->ParseFromString(response_str))
    if(!response->ParseFromArray(recv_buf,recv_size))
    {
        //std::cout << "parse error! response_str:" << recv_buf << std::endl;
        close(clientfd);
        char errtext[1088] = {0};
        sprintf(errtext,"parse error! response_str:%s",recv_buf);
        controller->SetFailed(errtext);
        return;
    }
    close(clientfd);
}