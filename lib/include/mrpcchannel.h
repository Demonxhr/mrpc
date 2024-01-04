#pragma once

#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
class MrpcChannel : public google::protobuf::RpcChannel
{
public:
  //所有通过stub代理对象调用的rpc方法，都同一送入这个方法
  void CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                          google::protobuf::Message* response, google::protobuf::Closure* done);


};