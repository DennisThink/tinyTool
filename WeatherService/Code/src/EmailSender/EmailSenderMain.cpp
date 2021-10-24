#include "SendEmail.h"
#include "CMySql.h"
#include "CDate.h"
#include "CommonConfig.h"
#include "CommonConfigStruct.h"
#include "CommonFunction.h"
#include <iostream>
#include <map>
using namespace DennisThink;
using Email_CityCode_MAP_T = std::map<std::string,std::vector<std::string>>;
class CEmailSender
{
public:
    explicit CEmailSender(const CMySqlConfig& dbConfig)
    {
        std::cout<<"Connect "<<m_mysqlUtil.Connect(dbConfig)<<std::endl;
    }


    virtual ~CEmailSender(){
        m_mysqlUtil.Close();
    }

    Email_CityCode_MAP_T GetEmailSubList()
    {
        C_T_SUBSCRIBE_EMAIL_BEAN_VECTOR_T allBean;
        m_mysqlUtil.GetAllEmailList(allBean);
        Email_CityCode_MAP_T result;
        for(const auto item:allBean)
        {
            if(result.find(item.m_F_EMAIL) != result.end())
            {
                result[item.m_F_EMAIL].push_back(item.m_F_CITY_CODE);
            }
            else
            {
                result.insert(std::pair<std::string,std::vector<std::string>>(item.m_F_EMAIL,std::vector<std::string>{item.m_F_CITY_CODE}));
            }
        }
        return result;
    }

    std::string GetWeatherForEmail(const std::string& cityCode)
    {
        C_T_WEATHER_BEAN oneDayWeather;
        C_T_CITY_CODE_ADDR cityBean;
        cityBean.m_F_CITY_CODE = cityCode;
        m_mysqlUtil.GetCityCodeAddr(cityCode,cityBean);
        Date dateUtil;
        m_mysqlUtil.GetOneDayWeatherByCityCode(cityCode,dateUtil.ToString(),oneDayWeather);
        if(oneDayWeather.Valid() && cityBean.Valid())
        {
            return "城市: "+cityBean.GetAddress()+"\r\n"+oneDayWeather.ToMailContext();
        }
        else
        {
            return "";
        }
        
    }

    void SendEmail()
    {
        auto allEmailList = GetEmailSubList();
        std::string strEmailContext;
        for(const auto email : allEmailList)
        {
           
            strEmailContext.clear();
            for(const auto cityCode : email.second)
            {
                strEmailContext+=GetWeatherForEmail(cityCode);
            }
            SendMail(email.first,"天气预报",strEmailContext);
            std::cout<<email.first<<"  "<<strEmailContext<<std::endl;
        }
    }
protected:
    CMysql m_mysqlUtil;
};
void GenerateCfgFile(const std::string cfgFileName)
{
    if(cfgFileName.empty())
    {
        return ;
    }
    std::string strConfig=R"({
   "LogDir":"/home/test/Log/EmailSender/",
   "MySql":{
        "IP":"127.0.0.1",
        "Port":3306,
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
    std::cout<<g_SERVER_VERSION<<std::endl;
    CEmailSender sender(mysqlCfg);
    sender.SendEmail();
    return 0;
}