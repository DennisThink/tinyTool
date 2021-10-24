#include "CMySql.h"
#include <mysql/mysql.h>
#include <iostream>
#include <mysql/my_global.h>
#include <stdlib.h>
#define FMT_STRING_ALIAS 1
#include <fmt/format.h>

namespace DennisThink
{
bool CMysql::Init()
{
    if (!m_pConnect)
    {
        m_pConnect = mysql_init(NULL);
        mysql_set_character_set(m_pConnect, "utf8");
    }
    return true;
}
CMysql::CMysql()
{
    Init();
    SetLogFunc([](const std::string strLog)->void{
        std::cout<<strLog<<std::endl;
    });
}

CMysql::~CMysql()
{
    if (m_pConnect)
    {
        mysql_close(m_pConnect);
        m_pConnect = nullptr;
    }
}

bool CMysql::Connect(const CMySqlConfig& cfg)
{
    return Connect(cfg.m_Ip,
                   cfg.m_Port,
                   cfg.m_db,
                   cfg.m_userName,
                   cfg.m_passwd);
}
bool CMysql::Connect(const std::string serverIp, const int port, const std::string strDb, const std::string userName, const std::string passed)
{
    Init();
    MYSQL *result = mysql_real_connect(m_pConnect,
                                       serverIp.c_str(),
                                       userName.c_str(),
                                       passed.c_str(),
                                       strDb.c_str(),
                                       (unsigned int)(port), 0, 0);
    if (nullptr != result)
    {
        return true;
    }
    else
    {
        m_logFunc("Connect To DB Error "+std::string(__FILE__)+"  "+std::to_string(__LINE__));
        return false;
    }
}

bool CMysql::GetOneDayWeatherByCityCode(const std::string &cityCode, const std::string &strDate, C_T_WEATHER_BEAN &weatherBean)
{
    if (cityCode.empty())
    {
        return false;
    }
    if (!m_pConnect)
    {
        return false;
    }
    std::string SQL_TEMPLATE =
        "SELECT F_WEATHER,\
F_HIGH_TEMP,\
F_LOW_TEMP,\
F_WIND_LEVEL,\
F_WIND_DIRECT \
FROM T_WEATHER WHERE F_CITY_CODE='" +
        cityCode + "' AND F_DATE='" + strDate + "';";
    if (mysql_query(m_pConnect, SQL_TEMPLATE.c_str()))
    {
        m_logFunc(SQL_TEMPLATE);
        return false;
    }

    MYSQL_RES *result = mysql_store_result(m_pConnect);

    if (result == NULL)
    {
        m_logFunc(SQL_TEMPLATE);
        return false;
    }

    int num_fields = mysql_num_fields(result);
    std::string Fileds[18] = {""};
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            Fileds[i] = std::string(row[i]);
        }
    }
    weatherBean.m_F_DATE = strDate;
    weatherBean.m_F_CITY_CODE = cityCode;
    weatherBean.m_F_WEATHER = Fileds[0];
    //std::cout << weatherBean.m_F_WEATHER.length() << std::endl;
    weatherBean.m_F_HIGH_TEMP = std::atoi(Fileds[1].c_str());
    weatherBean.m_F_LOW_TEMP = std::atoi(Fileds[2].c_str());
    weatherBean.m_F_WIND_LEVEL = Fileds[3];
    weatherBean.m_F_WIND_DIRECT = Fileds[4];

    mysql_free_result(result);
    return weatherBean.Valid();
}

bool CMysql::UpdateOneDayWeather(const C_T_WEATHER_BEAN &weatherBean)
{
    if (!weatherBean.Valid())
    {
        m_logFunc("Param Error");
        return false;
    }
    if (!m_pConnect)
    {
        m_logFunc("Connect Failes");
        return false;
    }

#define SQL_ONE_DAY_WEATHER_UPDATE R"(UPDATE T_WEATHER SET F_WEATHER='{}',F_HIGH_TEMP={},F_LOW_TEMP={},F_WIND_LEVEL='{}',F_WIND_DIRECT='{}',F_UPDATE_TIME=now() WHERE F_CITY_CODE='{}' AND F_DATE='{}';)"
    std::string strSql = format(fmt(SQL_ONE_DAY_WEATHER_UPDATE), weatherBean.m_F_WEATHER,
                                weatherBean.m_F_HIGH_TEMP,
                                weatherBean.m_F_LOW_TEMP,
                                weatherBean.m_F_WIND_LEVEL,
                                weatherBean.m_F_WIND_DIRECT,
                                weatherBean.m_F_CITY_CODE,
                                weatherBean.m_F_DATE);
    m_logFunc(strSql);
    if (mysql_query(m_pConnect, strSql.c_str()))
    {
        return false;
    }

    return true;
}

bool CMysql::InsertOneDayWeather(const C_T_WEATHER_BEAN &weatherBean)
{
    if (!weatherBean.Valid())
    {
        return false;
    }
    if (!m_pConnect)
    {
        return false;
    }

#define SQL_TEMPLATE_INSERT R"(INSERT INTO T_WEATHER(F_CITY_CODE,F_DATE,F_WEATHER,F_HIGH_TEMP,F_LOW_TEMP,F_WIND_LEVEL,F_WIND_DIRECT) VALUES('{}','{}','{}',{},{},'{}','{}');)"

    std::string strSql = format(fmt(SQL_TEMPLATE_INSERT), weatherBean.m_F_CITY_CODE,
                                weatherBean.m_F_DATE,
                                weatherBean.m_F_WEATHER,
                                weatherBean.m_F_HIGH_TEMP,
                                weatherBean.m_F_LOW_TEMP,
                                weatherBean.m_F_WIND_LEVEL,
                                weatherBean.m_F_WIND_DIRECT);
    m_logFunc(strSql);
    if (mysql_query(m_pConnect, strSql.c_str()))
    {
        return false;
    }

    MYSQL_RES *result = mysql_store_result(m_pConnect);
    mysql_free_result(result);
    return true;
}

bool CMysql::GetCityCodeAddr(const std::string &cityCode, C_T_CITY_CODE_ADDR &addrBean)
{
    if (cityCode.empty())
    {
        return false;
    }
    if (!m_pConnect)
    {
        return false;
    }

    std::string SQL_TEMPLATE =
        "SELECT F_CITY_CODE,\
F_COUNTRY,\
F_PROVINCE,\
F_CITY,\
F_AREA,\
F_WEATHER_CODE \
FROM T_CITY_CODE_ADDR WHERE F_CITY_CODE='" +
        cityCode + "';";
        std::cout<<SQL_TEMPLATE<<std::endl;
    if (mysql_query(m_pConnect, SQL_TEMPLATE.c_str()))
    {
        return false;
    }

    MYSQL_RES *result = mysql_store_result(m_pConnect);

    if (result == NULL)
    {
        return false;
    }

    int num_fields = mysql_num_fields(result);
    std::string Fileds[18] = {""};
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            Fileds[i] = std::string(row[i]);
        }
    }
    addrBean.m_F_CITY_CODE = Fileds[0];
    addrBean.m_F_COUNTRY = Fileds[1];
    addrBean.m_F_PROVINCE = Fileds[2];
    addrBean.m_F_CITY = Fileds[3];
    addrBean.m_F_AREA = Fileds[4];
    addrBean.m_F_WEATHER_CODE = Fileds[5];

    mysql_free_result(result);
    return true;
}


bool CMysql::GetCityCodeByAddr(const std::string& strProvince,const std::string& strCity,C_T_CITY_CODE_ADDR_ARRAY_T& allBeans)
{
    if (!m_pConnect)
    {
        return false;
    }

    if(strProvince.empty())
    {
        return false;
    }

    if(strCity.empty())
    {
        return false;
    }
    std::string SQL_TEMPLATE =
        "SELECT F_CITY_CODE,\
F_COUNTRY,\
F_PROVINCE,\
F_CITY,\
F_AREA,\
F_WEATHER_CODE \
FROM T_CITY_CODE_ADDR WHERE F_PROVINCE LIKE '"+strProvince+"' AND ( F_CITY LIKE '"+strCity+"' OR F_AREA LIKE '"+strCity+"');";
    if (mysql_query(m_pConnect, SQL_TEMPLATE.c_str()))
    {
        std::cout << "SQL: " << SQL_TEMPLATE << std::endl;
        return false;
    }

    MYSQL_RES *result = mysql_store_result(m_pConnect);

    if (result == NULL)
    {
        return false;
    }
    int num_fields = mysql_num_fields(result);
    std::string Fileds[18] = {""};
    MYSQL_ROW row;
    C_T_CITY_CODE_ADDR addrBean;
    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            Fileds[i] = std::string(row[i]);
        }
        addrBean.m_F_CITY_CODE = Fileds[0];
        addrBean.m_F_COUNTRY = Fileds[1];
        addrBean.m_F_PROVINCE = Fileds[2];
        addrBean.m_F_CITY = Fileds[3];
        addrBean.m_F_AREA = Fileds[4];
        addrBean.m_F_WEATHER_CODE = Fileds[5];
        allBeans.push_back(addrBean);
    }
    mysql_free_result(result);
    return true;
}


bool CMysql::GetAllCityCodeAddr(C_T_CITY_CODE_ADDR_ARRAY_T &beanArray)
{
    if (!m_pConnect)
    {
        return false;
    }

    std::string SQL_TEMPLATE =
        "SELECT F_CITY_CODE,\
F_COUNTRY,\
F_PROVINCE,\
F_CITY,\
F_AREA,\
F_WEATHER_CODE \
FROM T_CITY_CODE_ADDR;";
    if (mysql_query(m_pConnect, SQL_TEMPLATE.c_str()))
    {
        std::cout << "SQL: " << SQL_TEMPLATE << std::endl;
        return false;
    }

    MYSQL_RES *result = mysql_store_result(m_pConnect);

    if (result == NULL)
    {
        return false;
    }
    int num_fields = mysql_num_fields(result);
    std::string Fileds[18] = {""};
    MYSQL_ROW row;
    C_T_CITY_CODE_ADDR addrBean;
    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            Fileds[i] = std::string(row[i]);
        }
        addrBean.m_F_CITY_CODE = Fileds[0];
        addrBean.m_F_COUNTRY = Fileds[1];
        addrBean.m_F_PROVINCE = Fileds[2];
        addrBean.m_F_CITY = Fileds[3];
        addrBean.m_F_AREA = Fileds[4];
        addrBean.m_F_WEATHER_CODE = Fileds[5];
        beanArray.push_back(addrBean);
    }
    mysql_free_result(result);
    return true;
}

bool CMysql::InsertCityCodeAddr(const C_T_CITY_CODE_ADDR &addrBean)
{
    if (!addrBean.Valid())
    {
        return false;
    }
    if (!m_pConnect)
    {
        return false;
    }

#define SQL_CITY_CODE_ADDR_INSERT R"(INSERT INTO T_CITY_CODE_ADDR(F_CITY_CODE,F_COUNTRY,F_PROVINCE,F_CITY,F_AREA,F_WEATHER_CODE) VALUES('{}','{}','{}','{}','{}','{}');)"

    std::string strSql = format(fmt(SQL_CITY_CODE_ADDR_INSERT), addrBean.m_F_CITY_CODE,
                                addrBean.m_F_COUNTRY,
                                addrBean.m_F_PROVINCE,
                                addrBean.m_F_CITY,
                                addrBean.m_F_AREA,
                                addrBean.m_F_WEATHER_CODE);
    std::cout << "SQL " << strSql << std::endl;
    if (mysql_query(m_pConnect, strSql.c_str()))
    {
        return false;
    }

    MYSQL_RES *result = mysql_store_result(m_pConnect);
    mysql_free_result(result);
    return true;
}
bool CMysql::InsertSubEmailList(const C_T_SUBSCRIBE_EMAIL_BEAN& emailCodeBean)
{
    if (!emailCodeBean.Valid())
    {
        return false;
    }
    if (!m_pConnect)
    {
        return false;
    }

#define SQL_SUB_EMAIL_LIST_INSERT R"(INSERT INTO T_SUBSCRIBE_EMAIL_LIST(F_EMAIL,F_CITY_CODE,F_START_DATE,F_END_DATE) VALUES('{}','{}','{}','{}');)"

    std::string strSql = format(fmt(SQL_SUB_EMAIL_LIST_INSERT), emailCodeBean.m_F_EMAIL,
                                emailCodeBean.m_F_CITY_CODE,
                                emailCodeBean.m_F_START_DATE,
                                emailCodeBean.m_F_END_DATE);
    std::cout << "SQL " << strSql << std::endl;
    if (mysql_query(m_pConnect, strSql.c_str()))
    {
        return false;
    }

    MYSQL_RES *result = mysql_store_result(m_pConnect);
    mysql_free_result(result);
    return true;
}
bool CMysql::DeleteSubEmailList(const std::string &strEmail, const std::string &strCityCode)
{
    if(strEmail.empty())
    {
        return false;
    }
    if(strCityCode.empty())
    {
        return false;
    }

#define SQL_SUB_EMAIL_LIST_DELETE R"(DELETE FROM  T_SUBSCRIBE_EMAIL_LIST WHERE F_EMAIL='{}' AND F_CITY_CODE='{}';)"

    std::string strSql = format(fmt(SQL_SUB_EMAIL_LIST_DELETE),strEmail,strCityCode);
    std::cout << "SQL " << strSql << std::endl;
    if (mysql_query(m_pConnect, strSql.c_str()))
    {
        return false;
    }

    MYSQL_RES *result = mysql_store_result(m_pConnect);
    mysql_free_result(result);
    return true;
}

bool CMysql::GetAllEmailList(C_T_SUBSCRIBE_EMAIL_BEAN_VECTOR_T& emailCodeBean)
{
    if(!emailCodeBean.empty())
    {
        return false;
    }
    if (!m_pConnect)
    {
        return false;
    }

    std::string SQL_TEMPLATE =
        "SELECT F_EMAIL,\
F_CITY_CODE,\
F_START_DATE,\
F_END_DATE \
FROM T_SUBSCRIBE_EMAIL_LIST;";
    if (mysql_query(m_pConnect, SQL_TEMPLATE.c_str()))
    {
        std::cout << "SQL: " << SQL_TEMPLATE << std::endl;
        return false;
    }

    MYSQL_RES *result = mysql_store_result(m_pConnect);

    if (result == NULL)
    {
        return false;
    }
    int num_fields = mysql_num_fields(result);
    std::string Fileds[18] = {""};
    MYSQL_ROW row;
    C_T_SUBSCRIBE_EMAIL_BEAN addrBean;
    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            Fileds[i] = std::string(row[i]);
        }
        addrBean.m_F_EMAIL = Fileds[0];
        addrBean.m_F_CITY_CODE = Fileds[1];
        addrBean.m_F_START_DATE = Fileds[2];
        addrBean.m_F_END_DATE = Fileds[3];
        emailCodeBean.push_back(addrBean);
    }
    mysql_free_result(result);
    return true;
}
bool CMysql::SelectSubEmailList(const std::string& strEmail,C_T_SUBSCRIBE_EMAIL_BEAN_VECTOR_T& emailCodeBean)
{
    if(strEmail.empty())
    {
        return false;
    }
    if(!emailCodeBean.empty())
    {
        return false;
    }
    if (!m_pConnect)
    {
        return false;
    }

    std::string SQL_TEMPLATE =
        "SELECT F_EMAIL,\
F_CITY_CODE,\
F_START_DATE,\
F_END_DATE \
FROM T_SUBSCRIBE_EMAIL_LIST WHERE F_EMAIL='"+strEmail+"';";
    if (mysql_query(m_pConnect, SQL_TEMPLATE.c_str()))
    {
        std::cout << "SQL: " << SQL_TEMPLATE << std::endl;
        return false;
    }

    MYSQL_RES *result = mysql_store_result(m_pConnect);

    if (result == NULL)
    {
        return false;
    }
    int num_fields = mysql_num_fields(result);
    std::string Fileds[18] = {""};
    MYSQL_ROW row;
    C_T_SUBSCRIBE_EMAIL_BEAN addrBean;
    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            Fileds[i] = std::string(row[i]);
        }
        addrBean.m_F_EMAIL = Fileds[0];
        addrBean.m_F_CITY_CODE = Fileds[1];
        addrBean.m_F_START_DATE = Fileds[2];
        addrBean.m_F_END_DATE = Fileds[3];
        emailCodeBean.push_back(addrBean);
    }
    mysql_free_result(result);
    return true;
}
bool CMysql::Close()
{
    if (m_pConnect)
    {
        mysql_close(m_pConnect);
        m_pConnect = nullptr;
        return true;
    }
    else
    {
        return false;
    }
}
} // namespace DennisThink