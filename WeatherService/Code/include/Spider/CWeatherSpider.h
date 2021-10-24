#ifndef _WEATHER_SPIDER_C_WEATHER_SPIDER_H_
#define _WEATHER_SPIDER_C_WEATHER_SPIDER_H_
#include "CMySQLBean.h"
#include <vector>
using C_T_WEATHER_BEAN_ARRAY = std::vector<C_T_WEATHER_BEAN>; 
class CWeatherSpider final
{
public:
    explicit CWeatherSpider(){}
    virtual ~CWeatherSpider(){};
    static C_T_WEATHER_BEAN_ARRAY GetWeatherByCityWeatherCode(const std::string& strCode);
};
#endif