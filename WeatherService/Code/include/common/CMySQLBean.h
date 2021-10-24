#ifndef _WEATHER_SERVER_C_MYSQL_BEAN_H_
#define _WEATHER_SERVER_C_MYSQL_BEAN_H_
#include <string>
#include <vector>
#include "CommonEnum.h"


class C_T_CITY_CODE_ADDR final
{
public:
    std::string m_F_CITY_CODE;//城市编号
    std::string m_F_COUNTRY;//国家
    std::string m_F_PROVINCE;//省
    std::string m_F_CITY;//市
    std::string m_F_AREA;//县
    std::string m_F_WEATHER_CODE;

    std::string GetAddress() const {
        return m_F_COUNTRY+"-"+m_F_PROVINCE+"-"+m_F_CITY+"-"+m_F_AREA;
    }
    std::string ToString() const
    {
        std::string strResult;
        strResult = strResult + m_F_CITY_CODE+",";
        strResult = strResult + m_F_COUNTRY+",";
        strResult = strResult + m_F_PROVINCE+",";
        strResult = strResult + m_F_CITY+",";
        strResult = strResult + m_F_AREA+",";
        strResult = strResult + m_F_WEATHER_CODE;
        return strResult;
    }

    bool Valid() const 
    {
        if(m_F_CITY_CODE.empty())
        {
            return false;
        }
        if(m_F_COUNTRY.empty())
        {
            return false;
        }
        if(m_F_PROVINCE.empty())
        {
            return false;
        }
        
        if(m_F_CITY.empty())
        {
            return false;
        }
        return true;
    }

    DIFF_LEVEL GetDiffLevel(const C_T_CITY_CODE_ADDR& other)
    {
        if(this->m_F_COUNTRY != other.m_F_COUNTRY)
        {
            return COUNTRY_DIFF;
        }
        
        if(this->m_F_PROVINCE != other.m_F_PROVINCE)
        {
            return PROVINCE_DIFF;
        }
        
        if(this->m_F_CITY != other.m_F_CITY)
        {
            return CITY_DIFF;
        }
        
        if(this->m_F_AREA != other.m_F_AREA)
        {
            return AREA_DIFF;
        }
        return NO_DIFF;
    }
};

using C_T_CITY_CODE_ADDR_ARRAY_T =  std::vector<C_T_CITY_CODE_ADDR>;
class C_T_WEATHER_BEAN final
{
public:
    std::string m_F_CITY_CODE;//城市编号
    std::string m_F_DATE;//日期
    std::string m_F_WEATHER;//天气状况
    int m_F_HIGH_TEMP;//最高气温
    int m_F_LOW_TEMP;//最低气温
    std::string m_F_WIND_LEVEL;//风力
    std::string m_F_WIND_DIRECT;//风向

    std::string ToString() const
    {
        std::string strResult;
        strResult = strResult + "F_CITY_CODE: "+m_F_CITY_CODE;
        strResult = strResult + "\tF_DATE: "+m_F_DATE;
        strResult = strResult + "\tF_WEATHER: "+m_F_WEATHER;
        strResult = strResult + "\tF_HIGH_TEMP: "+std::to_string(m_F_HIGH_TEMP);
        strResult = strResult + "\tF_LOW_TEMP: "+std::to_string(m_F_LOW_TEMP);
        strResult = strResult + "\tF_WIND_LEVEL: "+m_F_WIND_LEVEL;
        strResult = strResult + "\tF_WIND_DIRECT: "+m_F_WIND_DIRECT;
        return strResult;
    }

    std::string ToExcelString() const
    {
        std::string strResult;
        strResult = strResult +m_F_WEATHER;
        strResult = strResult + ","+std::to_string(m_F_HIGH_TEMP);
        strResult = strResult + ","+std::to_string(m_F_LOW_TEMP);
        strResult = strResult + ","+m_F_WIND_LEVEL;
        strResult = strResult + ","+m_F_WIND_DIRECT;
        return strResult;
    }

    std::string ToMailContext() const {
        std::string strResult;
        strResult = strResult + "\n 日期 : "+m_F_DATE;
        strResult = strResult + "\n 天气 : "+m_F_WEATHER;
        strResult = strResult + "\n 最高气温 : "+std::to_string(m_F_HIGH_TEMP)+"℃";
        strResult = strResult + "\n 最低气温 : "+std::to_string(m_F_LOW_TEMP)+"℃";
        strResult = strResult + "\n 风力 : "+m_F_WIND_LEVEL;
        strResult = strResult + "\n 风向 : "+m_F_WIND_DIRECT+"\n";
        strResult += "-----------------------------------------------------";
        return strResult+"\n";
    }

    bool Valid() const 
    {
        if(m_F_CITY_CODE.empty())
        {
            return false;
        }
        if(m_F_DATE.empty())
        {
            return false;
        }

        if(m_F_WEATHER.empty())
        {
            return false;
        }
        if(m_F_WIND_DIRECT.empty())
        {
            return false;
        }
        return true;
    }
};

class C_T_SUBSCRIBE_EMAIL_BEAN final
{
public:
    std::string m_F_EMAIL;//城市编号
    std::string m_F_CITY_CODE;//日期
    std::string m_F_START_DATE;//天气状况
    std::string m_F_END_DATE;//天气状况

    std::string ToString() const
    {
        std::string strResult;
        strResult = strResult + "F_EMAIL: "+m_F_EMAIL;
        strResult = strResult + "\tF_CITY_CODE: "+m_F_CITY_CODE;
        strResult = strResult + "\tF_START_DATE: "+m_F_START_DATE;
        strResult = strResult + "\tF_END_DATE: "+m_F_END_DATE;
        return strResult;
    }

    bool Valid() const 
    {
        if(m_F_EMAIL.empty())
        {
            return false;
        }

        if(m_F_CITY_CODE.empty())
        {
            return false;
        }
       

        if(m_F_START_DATE.empty())
        {
            return false;
        }
        if(m_F_END_DATE.empty())
        {
            return false;
        }
        return true;
    }
};
using C_T_SUBSCRIBE_EMAIL_BEAN_VECTOR_T=std::vector<C_T_SUBSCRIBE_EMAIL_BEAN>;
#endif