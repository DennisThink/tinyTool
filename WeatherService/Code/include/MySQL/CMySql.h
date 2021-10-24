#ifndef _DENNIS_THINK_C_MYSQL_H_
#define _DENNIS_THINK_C_MYSQL_H_
#include <string>
#include "mysql/mysql.h"
#include "CMySQLBean.h"
#include <functional>
#include "CommonConfigStruct.h"
namespace DennisThink
{
    using LogFunc = std::function<void(const std::string strLog)>;
    class CMysql
    {
    public:
        CMysql();
        void SetLogFunc(LogFunc logFunc){
            m_logFunc = logFunc;
        }
        virtual ~CMysql();
        bool Connect(const CMySqlConfig& cfg);
        bool Connect(const std::string serverIp,const int port,const std::string db,const std::string userName,const std::string passed);
        bool GetOneDayWeatherByCityCode(const std::string& cityCode,const std::string& strDate,C_T_WEATHER_BEAN& weatherBean);
        bool UpdateOneDayWeather(const C_T_WEATHER_BEAN& weatherBean);
        bool InsertOneDayWeather(const C_T_WEATHER_BEAN& weatherBean);
        
        bool GetCityCodeAddr(const std::string& cityCode,C_T_CITY_CODE_ADDR& addrBean);
        bool GetCityCodeByAddr(const std::string& province,const std::string& strCity,C_T_CITY_CODE_ADDR_ARRAY_T& addrBean);
        bool GetAllCityCodeAddr(C_T_CITY_CODE_ADDR_ARRAY_T& beanArray);
        bool InsertCityCodeAddr(const C_T_CITY_CODE_ADDR& addrBean);
        
        bool InsertSubEmailList(const C_T_SUBSCRIBE_EMAIL_BEAN& emailCodeBean);
        bool DeleteSubEmailList(const std::string& strEmail,const std::string& strCityCode);
        bool SelectSubEmailList(const std::string& strEmail,C_T_SUBSCRIBE_EMAIL_BEAN_VECTOR_T& emailCodeBean);
        bool GetAllEmailList(C_T_SUBSCRIBE_EMAIL_BEAN_VECTOR_T& emailCodeBean);
        bool Close();
    private:
        bool Init();
        MYSQL * m_pConnect=nullptr;
        LogFunc m_logFunc;
    };
};
#endif