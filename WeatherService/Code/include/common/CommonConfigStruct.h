#ifndef _COMMON_CONFIG_STRUCT_H_
#define _COMMON_CONFIG_STRUCT_H_
#include <string>
#include <json11.hpp>
class CMySqlConfig
{
public:
    bool FromJson(json11::Json jsonObj)
    {
        if(!jsonObj["IP"].is_string())
        {
            return false;
        }
        m_Ip = jsonObj["IP"].string_value();


        if(!jsonObj["Port"].is_number())
        {
            return false;
        }
        m_Port = jsonObj["Port"].int_value();


        if(!jsonObj["UserName"].is_string())
        {
            return false;
        }
        m_userName = jsonObj["UserName"].string_value();

        if(!jsonObj["PassWord"].is_string())
        {
            return false;
        }
        m_passwd = jsonObj["PassWord"].string_value();

        if(!jsonObj["DataBase"].is_string())
        {
            return false;
        }
        m_db = jsonObj["DataBase"].string_value();
        return true;
    }
    json11::Json::object ToJson()
    {
        json11::Json::object result=json11::Json::object({
            {"IP",m_Ip},
            {"Port",m_Port},
            {"UserName",m_userName},
            {"Password",m_passwd},
            {"DataBase",m_db},
        });
        return result;
    }
public:
    std::string m_Ip;
    int         m_Port;
    std::string m_userName;
    std::string m_passwd;
    std::string m_db;
};
#endif