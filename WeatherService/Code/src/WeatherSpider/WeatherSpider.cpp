#include "CWeatherSpider.h"
#include <iostream>
#include <iostream>
#include "CMySql.h"
#if 0
#define DEBUG_LINE std::cout<<__LINE__<<std::endl;
#else
#define DEBUG_LINE 
#endif
#include "CommonConfig.h"
#include "CommonFunction.h"
#include "CommonConfigStruct.h"

void GenerateCfgFile(const std::string cfgFileName)
{
    if(cfgFileName.empty())
    {
        return ;
    }
    std::string strConfig=R"({
   "LogDir":"/home/test/Log/SourceServer/",
   "MySql":{
        "IP":"127.0.0.1",
        "Port":9000,
        "UserName":"test",
        "PassWord":"test",
        "DataBase":"WEATHER"
      }
    })";
    write_txtfile(cfgFileName.c_str(),strConfig);
}
int main(int argc,char * argv[])
{   
    std::cout<<g_SERVER_VERSION<<std::endl;
    std::cout<<argc<<argv[0]<<std::endl;
    std::cout<<g_SERVER_VERSION<<std::endl;
	if(argc < 2)
	{
		PrintParamOption();
		return 0;
	}
    ParseParamResult result = ParseParam(argc,argv,GenerateCfgFile);
    if(result.m_bReturn)
    {
        return 0;
    }

    using namespace DennisThink;
    std::string strCfgContext;
    load_txtfile(result.m_cfgFile.c_str(),strCfgContext);
    if(strCfgContext.empty())
    {
        std::cout<<"Config File Error"<<std::endl;
        return -1;
    }
    std::string strErr;
    auto jsonCfg = json11::Json::parse(strCfgContext,strErr);
    if(!strErr.empty())
    {
        std::cout<<"Json Format Error"<<strCfgContext<<std::endl;
        return -1;
    }
    auto loger = CreateLogger(jsonCfg);
    if(nullptr == loger)
    {
        std::cout<<"Create Loger Failed"<<strCfgContext<<std::endl;
        return -1;
    }
    CMySqlConfig mysqlCfg;
    if(!mysqlCfg.FromJson(jsonCfg["MySql"]))
    {
        std::cout<<"Parse Mysql Config From Json Failed "<<std::endl;
        return -1;
    }
    CMysql mysqlUtil;

    mysqlUtil.SetLogFunc([loger](const std::string strLog)->void {
        loger->warn("{}",strLog);
    });
  
    std::cout<<"Connect "<<mysqlUtil.Connect(mysqlCfg.m_Ip,mysqlCfg.m_Port,mysqlCfg.m_db,mysqlCfg.m_userName,mysqlCfg.m_passwd)<<std::endl;
    {
        C_T_CITY_CODE_ADDR_ARRAY_T addrVec;
        if(mysqlUtil.GetAllCityCodeAddr(addrVec))
        {
            C_T_WEATHER_BEAN weatherBean;
            for(const auto& item:addrVec)
            {
                auto beanArray = CWeatherSpider::GetWeatherByCityWeatherCode(item.m_F_WEATHER_CODE);
                for(auto& weatherItem:beanArray)
                {
                    weatherItem.m_F_CITY_CODE = item.m_F_CITY_CODE;
                    if(mysqlUtil.GetOneDayWeatherByCityCode(weatherItem.m_F_CITY_CODE,weatherItem.m_F_DATE,weatherBean))
                    {
                        DEBUG_LINE
                        std::cout<<"UPDATE: "<<mysqlUtil.UpdateOneDayWeather(weatherItem)<<std::endl;
                    }
                    else
                    {
                        DEBUG_LINE
                        std::cout<<"INSERT: "<<mysqlUtil.InsertOneDayWeather(weatherItem)<<std::endl;
                    }
                }
            }
        }
        else
        {
            std::cout<<"Error"<<std::endl;
        }
    }
    std::cout<<"Close "<<mysqlUtil.Close()<<std::endl;
    return 0;
}