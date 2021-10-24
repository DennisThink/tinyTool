#include "CWeatherSpider.h"
#include <iostream>
#include "../thirdparty/restclient_cpp/include/restclient-cpp/restclient.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <array>
#include <assert.h>
#include "CDate.h"



C_T_WEATHER_BEAN GetDetailWeather(const std::string strContext)
{
    C_T_WEATHER_BEAN bean;
/*<li class="sky skyid lv1">
<h1>6日（明天）</h1>
<big class="png40 d00"></big>
<big class="png40 n00"></big>
<p title="晴" class="wea">晴</p>
<p class="tem">
<span>11</span>/<i>-2℃</i>
</p>
<p class="win">
<em>
<span title="北风" class="N"></span>
<span title="西南风" class="SW"></span>
</em>
<i>4-5级转<3级</i>
</p>
<div class="slid"></div>*/
    {
        auto h1Start = strContext.find("<h1>");
        auto h1End = strContext.find("</h1>");
        std::string strDate = strContext.substr(h1Start+4,h1End-h1Start-4);
        //bean.m_F_DATE = strDate;
        //std::cout<<"Date : "<<strDate<<std::endl;
    }
    {
        auto weatherStart = strContext.find("class=\"wea\">");
        auto weatherEnd = strContext.find("</p>",weatherStart);
        std::string strDate = strContext.substr(weatherStart+12,weatherEnd-weatherStart-12);
        bean.m_F_WEATHER = strDate;
        //std::cout<<"Weather : "<<strDate<<std::endl;
    }
    {
        auto weatherStart = strContext.find("<span>");
        auto weatherEnd = strContext.find("</span>",weatherStart);
        std::string strDate = strContext.substr(weatherStart+6,weatherEnd-weatherStart-6);
        bean.m_F_HIGH_TEMP = std::atoi(strDate.c_str());
        //std::cout<<"HighTemp : "<<strDate<<std::endl;
    }

    {
        auto weatherStart = strContext.find("<i>");
        auto weatherEnd = strContext.find("</i>",weatherStart);
        std::string strDate = strContext.substr(weatherStart+3,weatherEnd-weatherStart-3);
        bean.m_F_LOW_TEMP = std::atoi(strDate.c_str());
       // std::cout<<"LowTemp : "<<strDate<<std::endl;
    }

    std::size_t WindDirectIndex = 0;
    {
        auto windStart = strContext.find("<span title=\"");
        auto windEnd = strContext.find("\"",windStart+13);
        std::string strDate = strContext.substr(windStart+13,windEnd-windStart-13);
        //std::cout<<"WindLevel : "<<strDate<<std::endl;
        bean.m_F_WIND_DIRECT = strDate;
        WindDirectIndex = windEnd;
    }
    {
        auto windStart = strContext.find("<i>",WindDirectIndex);
        auto windEnd = strContext.find("</i>",windStart+3);
        std::string strDate = strContext.substr(windStart+3,windEnd-windStart-3);
        bean.m_F_WIND_LEVEL = strDate;
        //std::cout<<"WindDirect : "<<strDate<<std::endl;
    }
    return bean;
}
std::string GetWeatherArrayStr(const std::string strContext)
{
    std::string strStartTag="<ul class=\"t clearfix\">";
    std::string strEndTag = "</ul>";
    std::size_t startIndex = strContext.find(strStartTag);
    std::size_t endIndex = strContext.find(strEndTag,startIndex);
    std::string subStr = strContext.substr(startIndex,endIndex-startIndex+strEndTag.length());
    return subStr;
}

C_T_WEATHER_BEAN_ARRAY ParseHtmlGetWeather2(const std::string strContext)
{
    C_T_WEATHER_BEAN_ARRAY beanArray;
    std::string subStr = GetWeatherArrayStr(strContext);
    {
        std::size_t liStart = 0;
        std::size_t liEnd = 0;
       
        while(liEnd != std::string::npos)
        {
            liStart = subStr.find("<li",liEnd);
            liEnd = subStr.find("</li>",liStart);
            if(liEnd != std::string::npos)
            {
                std::string strLi = subStr.substr(liStart+3,liEnd-liStart-7);
                //std::cout<<"------------------------------------"<<std::endl;
               // std::cout<<strLi<<std::endl;
                //std::cout<<"------------------------------------"<<std::endl;
                auto bean = GetDetailWeather(strLi);
                beanArray.push_back(bean);
                //std::cout<<"------------------------------------"<<std::endl;
            }
           
            liStart = liEnd;
        }
    }
    return beanArray;
}


#if 0
#define DEBUG_LINE std::cout<<__LINE__<<std::endl;
#else
#define DEBUG_LINE
#endif

C_T_WEATHER_BEAN_ARRAY CWeatherSpider::GetWeatherByCityWeatherCode(const std::string& strCode)
{
    C_T_WEATHER_BEAN_ARRAY beanArray;
    if(!strCode.empty())
    {
        std::string strWeather="http://www.weather.com.cn/weather/"+strCode+".shtml";
        RestClient::Response res = RestClient::get(strWeather);
        if(res.code == 200)
        {
            beanArray= ParseHtmlGetWeather2(res.body);
        }
    }
    
    if(!beanArray.empty())
    {
        Date date;
        for(auto& item:beanArray)
        {
            item.m_F_DATE = date.ToString();
            date=date+1;
        }

    }
    return beanArray; 
}
