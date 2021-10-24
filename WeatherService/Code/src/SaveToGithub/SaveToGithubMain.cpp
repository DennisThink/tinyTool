#include "SendEmail.h"
#include "CMySql.h"
#include "CDate.h"
#include "CommonConfig.h"
#include "CommonConfigStruct.h"
#include "CommonFunction.h"
#include <iostream>
#include <fstream>
#include <map>
using namespace DennisThink;
using Email_CityCode_MAP_T = std::map<std::string,std::vector<std::string>>;
class CSaveToGitHub
{
public:
    explicit CSaveToGitHub(const CMySqlConfig& dbConfig)
    {
        std::cout<<"Connect "<<m_mysqlUtil.Connect(dbConfig)<<std::endl;
    }


    virtual ~CSaveToGitHub(){
        m_mysqlUtil.Close();
    }

    void SaveWeatherToFile(const std::string strFileName)
    {
        C_T_CITY_CODE_ADDR_ARRAY_T allCityCode;
        m_mysqlUtil.GetAllCityCodeAddr(allCityCode);
      
        std::ofstream weatherFile;
        weatherFile.open(strFileName);
        if(!weatherFile)
        {
            return ;
        }
        C_T_WEATHER_BEAN oneDayWeather;
        std::string strLine;
        weatherFile<<"城市,天气,最高气温,最低气温,风力,风向"<<"\r\n";
        Date dateUtil;
        for(auto cityItem:allCityCode)
        {
            
            m_mysqlUtil.GetOneDayWeatherByCityCode(cityItem.m_F_CITY_CODE,dateUtil.ToString(),oneDayWeather);
            if(oneDayWeather.Valid() && cityItem.Valid())
            {
                strLine = cityItem.GetAddress()+","+oneDayWeather.ToExcelString();
            }
            weatherFile<<strLine<<"\r\n";
        }
        weatherFile.close();
    }

    void SaveToGithub(const std::string strFolder)     
    {
        Date dateUtil;
        std::string strFileName = strFolder+dateUtil.ToString()+".xlsx";
        SaveWeatherToFile(strFileName);
        std::string strShellCmd="cd "+strFolder+" && ";
        strShellCmd = strShellCmd + "git add ./" + dateUtil.ToString() +".xlsx && ";
        strShellCmd = strShellCmd + " git commit -m \""+ dateUtil.ToString()+"\" && "; 
        strShellCmd = strShellCmd + " git push ";
        std::cout<<strShellCmd<<std::endl;
        system(strShellCmd.c_str());
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
      },
    "Github":""
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

    auto gitHub = jsonCfg["Github"].string_value();
    if(gitHub.empty())
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
    CSaveToGitHub sender(mysqlCfg);
    sender.SaveToGithub(gitHub);
    return 0;
}