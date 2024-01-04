#include "test.pb.h"
#include <iostream>
#include <string>
using namespace fixbug;



int main()
{
    // LoginResponse rsp;
    // ResultCode *rc = rsp.mutable_result();
    // rc->set_errcode(0);
    // rc->set_errmsg("");

    GetFriendListsResponse rsp;

    ResultCode *rc = rsp.mutable_result();
    rc->set_errcode(0);

    User *user1 = rsp.add_friend_list();
    user1->set_name("zhang san");
    user1->set_age(20);
    user1->set_sex(User::MAN);

    User *user2 = rsp.add_friend_list();
    user2->set_name("zhang san1");
    user2->set_age(21);
    user2->set_sex(User::WOMAN);

    std::cout << rsp.friend_list_size() << std::endl;

    std::string send_msg;
    if(rsp.SerializeToString(&send_msg))
    {
        std::cout << "msg: " << send_msg << std::endl;
    }

    GetFriendListsResponse rsqb;
    if(rsqb.ParseFromString(send_msg))
    {
        User userlist = rsqb.friend_list(0);
        std::cout << "name : "<<userlist.name() << std::endl;
    }




    return 0;
}

int main1()
{
    // LoginRequest req;
    // req.set_name("zhang san");
    // req.set_pwd("123456");

    // //序列化
    // std::string send_str;
    // if(req.SerializeToString(&send_str))
    // {
    //     std::cout << send_str.c_str() <<std::endl;
    // }

    // //反序列化
    // LoginRequest reqB;
    // if(reqB.ParseFromString(send_str))
    // {
    //     std::cout << reqB.name() << std::endl;
    //     std::cout << reqB.pwd() <<std::endl;
    // }
    return 0;
}