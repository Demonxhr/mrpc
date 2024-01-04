#include "mrpcconfig.h"
#include <iostream>
#include <string>
//负责解析加载配置文件
void MrpcConfig::LoadConfigFile(const char *config_file)
{
    FILE *pf = fopen(config_file,"r");
    if(nullptr == pf)
    {
        std::cout << config_file << " is not exist!" << std::endl;
        exit(EXIT_FAILURE);
    }

    //1.注释 2.正确的配置项  = 3.去掉开头多余的空格
    while(!feof(pf))
    {
        char buf[512] = {0};
        fgets(buf,512,pf);


        std::string read_buf(buf);
        Trim(read_buf);
        // 判断#的注释
        if(read_buf[0] == '#' || read_buf.empty())
        {
            continue;
        }

        //解析配置项
        int idx = read_buf.find('=');
        if(-1 == idx)
        {
            //配置项不合法
            continue;
        }

        std::string key;
        std::string value;
        key = read_buf.substr(0,idx);
        value = read_buf.substr(idx+1,read_buf.size()-idx);
        idx = value.find_first_of('\n');
        if(-1 != idx)
        {
            value = value.substr(0,idx);
        }
        Trim(key);
        Trim(value);
        m_configMap.insert({key,value});
    }
}
//查询配置项信息
std::string MrpcConfig::Load(const std::string &key)
{
    // 不用中括号，因为当map中不存在key时，用中括号会增加这一项，我们希望返回空值
    //return m_configMap[key];
    auto it = m_configMap.find(key);
    if(it == m_configMap.end())
    {
        return "";
    }
    return it->second;
    
}

// 去掉字符串前后空格
void MrpcConfig::Trim(std::string &src_buf)
{
        // 去掉字符串前面多余空格
        int idx = src_buf.find_first_not_of(' ');
        if (-1 !=idx)
        {
            // 说明字符串前面有空格
            src_buf = src_buf.substr(idx,src_buf.size()-idx);
        }

        //去掉字符串后面多余的空格
        idx = src_buf.find_last_not_of(' ');
        if(-1 != idx)
        {
            // 说明字符串后面有空格
            src_buf = src_buf.substr(0,idx+1);
        }
}
