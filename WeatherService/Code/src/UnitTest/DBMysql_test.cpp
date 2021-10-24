#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include "CMySql.h"
CMySqlConfig GetMysqlCfg()
{
    CMySqlConfig cfg;
    cfg.m_Ip="127.0.0.1";
    cfg.m_Port=3306;
    cfg.m_userName="test";
    cfg.m_passwd="test";
    cfg.m_db="DB_WEATHER_DEV";
    return cfg;
}

TEST_SUITE_BEGIN("MYSQL_TEST");    
using namespace DennisThink;
CMysql mysqlUtilSrc;
TEST_CASE("CONNECT"){
    CHECK_EQ(true,mysqlUtilSrc.Connect(GetMysqlCfg()));
    CHECK_EQ(true,mysqlUtilSrc.Close());
}

TEST_CASE("GET_CITY_BY_PROVINCE_AND_CITY"){
    CHECK_EQ(true,mysqlUtilSrc.Connect(GetMysqlCfg()));
    std::string strProvince="北京";
    std::string strCity="东城";
    C_T_CITY_CODE_ADDR_ARRAY_T allBean;
    CHECK_EQ(true,mysqlUtilSrc.GetCityCodeByAddr(strProvince,strCity,allBean));
    CHECK_EQ(1,allBean.size());
    CHECK_EQ(true,mysqlUtilSrc.Close());
}
TEST_SUITE_END();

