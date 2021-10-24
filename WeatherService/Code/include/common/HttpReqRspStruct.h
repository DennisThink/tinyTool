#ifndef _WEATHER_SERVER_HTTP_REQ_RSP_STRUCT_H_
#define _WEATHER_SERVER_HTTP_REQ_RSP_STRUCT_H_
#include <string>
#include <map>
#include <vector>
#include "json11.hpp"
enum HTTP_RSP_CODE
{
    SUCCEED=0,
    EMPTY_CITY_CODE,//城市代码为空
    EMPTY_TOKEN,//
    INVALID_CITY_CODE,
    INVALID_TOKEN,
    EMPTY_EMAIL,
    INVALID_EMAIL,

    //
    EMPTY_COUNTRY,
    EMPTY_CITY,
    UNKNOWN_ERROR,
};

std::string CodeToMsg(const HTTP_RSP_CODE& code)
{
    switch (code)
    {
        case SUCCEED:
            {
                return "Success";
            }
            break;
        case EMPTY_CITY_CODE:
            {
                return "Empty City Code Error";
            }break;
        case EMPTY_TOKEN:
            {
                return "Empty Token Error";
            }break;
        case INVALID_CITY_CODE:
            {
                return "Invalid City Code Error";
            }break;
        case INVALID_TOKEN:
            {
                return "Invalid Token";
            }break;
        default:
            {
                return "UnKnown Error"+std::to_string(code);
            }
            break;
    }
}
class HttpReqBase
{
public:
    virtual bool FromString(const std::string& strText)=0;
    virtual std::string ToString() = 0;
};

class HttpRspBase
{
public:
    explicit HttpRspBase()
    {
        m_code = UNKNOWN_ERROR;
        m_message = CodeToMsg(m_code);
    }
    explicit HttpRspBase(const HTTP_RSP_CODE& code)
    {
        m_code = code;
        m_message = CodeToMsg(code);
    }
    explicit HttpRspBase(const HTTP_RSP_CODE& code,const std::string& strMessage)
    {
        m_code = code;
        m_message = strMessage;
    }
    HTTP_RSP_CODE GetCode() const
    {
        return m_code;
    }
    std::string GetMessage() const
    {
        return m_message;
    }
    virtual ~HttpRspBase()=default;
    virtual bool FromString(const std::string& strRsp)
    {
        if(strRsp.empty())
        {
            return false;
        }
        std::string strErr;
        auto rspJson = json11::Json::parse(strRsp,strErr);
        if(!strErr.empty())
        {
            return false;
        }

        if(!rspJson["code"].is_number())
        {
            return false;
        }
        m_code = HTTP_RSP_CODE(rspJson["code"].int_value());

        if(!rspJson["message"].is_string())
        {
            return false;
        }
        m_message = rspJson["message"].string_value();
        return true;
        
    }
    virtual std::string ToString() const
    {
        json11::Json result=json11::Json::object({
            {"code",static_cast<int>(m_code)},
            {"message",m_message}
        });
        return result.dump();
    }
protected:
    HTTP_RSP_CODE m_code;
    std::string m_message;
};

class OneDayWeatherElem_t {
public:
    OneDayWeatherElem_t(){
        m_lowTemp = 0;
        m_highTemp = -1;
    }
    std::string m_date;
    std::string m_cityName;
    std::string m_weather;
    int m_highTemp;
    int m_lowTemp;
    std::string m_windLevel;
    std::string m_windDirect;
public:
    bool Valid()
    {
        if(m_date.empty())
        {
            return false;
        }
        if(m_cityName.empty())
        {
            return false;
        }
        if(m_weather.empty())
        {
            return false;
        }
        if(m_lowTemp>m_highTemp)
        {
            return false;
        }
        if(m_windLevel.empty())
        {
            return false;
        }
        if(m_windDirect.empty())
        {
            return false;
        }
        return true;
    }
};
class HttpGetOneDayWeatherRsp:public HttpRspBase 
{
public:
    explicit HttpGetOneDayWeatherRsp():HttpRspBase(SUCCEED){

    }
    virtual ~HttpGetOneDayWeatherRsp(){

    }

    virtual std::string ToString() const override {
        json11::Json data = json11::Json::object({
            {"date",m_elem.m_date},
            {"name",m_elem.m_cityName},
            {"weather",m_elem.m_weather},
            {"high_temp",m_elem.m_highTemp},
            {"wind_level",m_elem.m_windLevel},
            {"wind_direct",m_elem.m_windDirect},
        });
        json11::Json result=json11::Json::object({
            {"code",static_cast<int>(m_code)},
            {"message",m_message},
            {"data",data},
        });
        return result.dump();
    }

    bool FromString(const std::string& strValue){
        if(strValue.empty())
        {
            return false;
        }
        std::string strErr;
        auto rspJson = json11::Json::parse(strValue,strErr);
        if(!strErr.empty())
        {
            return false;
        }

        if(!rspJson["code"].is_number())
        {
            return false;
        }
        m_code = HTTP_RSP_CODE(rspJson["code"].int_value());

        if(!rspJson["message"].is_string())
        {
            return false;
        }
        m_message = rspJson["message"].string_value();

        if(!rspJson["data"].is_object())
        {
            return false;
        }
        auto jsonElem = rspJson["data"];

        if(!jsonElem["date"].is_string())
        {
            return false;
        }
        m_elem.m_date=jsonElem["date"].string_value();

        if(!jsonElem["name"].is_string())
        {
            return false;
        }
        m_elem.m_cityName=jsonElem["name"].string_value();

         if(!jsonElem["weather"].is_string())
        {
            return false;
        }
        m_elem.m_weather=jsonElem["weather"].string_value();


         if(!jsonElem["high_temp"].is_number())
        {
            return false;
        }
        m_elem.m_highTemp=jsonElem["high_temp"].int_value();

        if(!jsonElem["low_temp"].is_number())
        {
            return false;
        }
        m_elem.m_lowTemp=jsonElem["low_temp"].int_value();

        if(!jsonElem["wind_level"].is_string())
        {
            return false;
        }
        m_elem.m_windLevel=jsonElem["wind_level"].string_value();

        if(!jsonElem["wind_direct"].is_string())
        {
            return false;
        }
        m_elem.m_windDirect=jsonElem["wind_direct"].string_value();
        return true;
    }
    void SetElem(const OneDayWeatherElem_t& elem){
        m_elem = elem;
    }
private:
    OneDayWeatherElem_t m_elem;
};

class HttpGetOneWeekWeatherRsp:public HttpRspBase 
{
public:
    explicit HttpGetOneWeekWeatherRsp():HttpRspBase(SUCCEED){

    }
    virtual ~HttpGetOneWeekWeatherRsp(){

    }

    virtual std::string ToString() const override {
        json11::Json::array elemJsonVec;
        for(const auto& elem:m_elemVec){
            json11::Json data = json11::Json::object({
                {"date",elem.m_date},
                {"name",elem.m_cityName},
                {"weather",elem.m_weather},
                {"high_temp",elem.m_highTemp},
                {"wind_level",elem.m_windLevel},
                {"wind_direct",elem.m_windDirect},
            });
            elemJsonVec.push_back(data);
        }
        json11::Json result=json11::Json::object({
            {"code",static_cast<int>(m_code)},
            {"message",m_message},
            {"data",elemJsonVec},
        });
        return result.dump();
    }

    bool FromString(const std::string& strValue){
        if(strValue.empty())
        {
            return false;
        }
        std::string strErr;
        auto rspJson = json11::Json::parse(strValue,strErr);
        if(!strErr.empty())
        {
            return false;
        }

        if(!rspJson["code"].is_number())
        {
            return false;
        }
        m_code = HTTP_RSP_CODE(rspJson["code"].int_value());

        if(!rspJson["message"].is_string())
        {
            return false;
        }
        m_message = rspJson["message"].string_value();

        if(!rspJson["data"].is_array())
        {
            return false;
        }
        auto jsonElem = rspJson["data"];

        OneDayWeatherElem_t elem;
        for(auto item:jsonElem.array_items())
        {
            if(!item["date"].is_string())
            {
                return false;
            }
            elem.m_date=item["date"].string_value();

            if(!item["name"].is_string())
            {
                return false;
            }
            elem.m_cityName=item["name"].string_value();

            if(!item["weather"].is_string())
            {
                return false;
            }
            elem.m_weather=item["weather"].string_value();


            if(!item["high_temp"].is_number())
            {
                return false;
            }
            elem.m_highTemp=item["high_temp"].int_value();

            if(!item["low_temp"].is_number())
            {
                return false;
            }
            elem.m_lowTemp=item["low_temp"].int_value();

            if(!item["wind_level"].is_string())
            {
                return false;
            }
            elem.m_windLevel=item["wind_level"].string_value();

            if(!item["wind_direct"].is_string())
            {
                return false;
            }
            elem.m_windDirect=item["wind_direct"].string_value();
            m_elemVec.push_back(elem);
        }
        return true;
    }
    void AddElem(const OneDayWeatherElem_t& elem){
        m_elemVec.push_back(elem);
    }
private:
    std::vector<OneDayWeatherElem_t> m_elemVec;
};

struct EmailSubElem_t{
    std::string m_StartDate;
    std::string m_EndDate;
    std::string m_CityCode;
    std::string m_CityName;
};

class HttpGetEmailSubRsp:public HttpRspBase 
{
public:
    explicit HttpGetEmailSubRsp():HttpRspBase(SUCCEED){

    }
    virtual ~HttpGetEmailSubRsp(){

    }

    virtual std::string ToString() const override {
        json11::Json::array elemJsonVec;
        for(const auto& elem:m_elemVec){
            json11::Json data = json11::Json::object({
                {"StartDate",elem.m_StartDate},
                {"EndDate",elem.m_EndDate},
                {"CityCode",elem.m_CityCode},
                {"CityName",elem.m_CityName},
            });
            elemJsonVec.push_back(data);
        }
        json11::Json result=json11::Json::object({
            {"code",static_cast<int>(m_code)},
            {"message",m_message},
            {"data",elemJsonVec},
        });
        return result.dump();
    }

    bool FromString(const std::string& strValue){
        if(strValue.empty())
        {
            return false;
        }
        std::string strErr;
        auto rspJson = json11::Json::parse(strValue,strErr);
        if(!strErr.empty())
        {
            return false;
        }

        if(!rspJson["code"].is_number())
        {
            return false;
        }
        m_code = HTTP_RSP_CODE(rspJson["code"].int_value());

        if(!rspJson["message"].is_string())
        {
            return false;
        }
        m_message = rspJson["message"].string_value();

        if(!rspJson["data"].is_array())
        {
            return false;
        }
        auto jsonElem = rspJson["data"];

        EmailSubElem_t elem;
        for(auto item:jsonElem.array_items())
        {
            if(!item["StartDate"].is_string())
            {
                return false;
            }
            elem.m_StartDate=item["StartDate"].string_value();

            if(!item["EndDate"].is_string())
            {
                return false;
            }
            elem.m_EndDate=item["EndDate"].string_value();

            if(!item["CityCode"].is_string())
            {
                return false;
            }
            elem.m_CityCode=item["CityCode"].string_value();


            if(!item["CityName"].is_number())
            {
                return false;
            }
            elem.m_CityName=item["CityName"].int_value();
            m_elemVec.push_back(elem);
        }
        return true;
    }
    void AddElem(const EmailSubElem_t& elem){
        m_elemVec.push_back(elem);
    }
private:
    std::vector<EmailSubElem_t> m_elemVec;
};

class HttpAddSubCityReq:public HttpReqBase
{
public:
    virtual bool FromString(const std::string& strValue) override
    {
        if(strValue.empty())
        {
            return false;
        }
        std::string strErr;
        auto rspJson = json11::Json::parse(strValue,strErr);
        if(!strErr.empty())
        {
            return false;
        }

        if(!rspJson["Email"].is_string())
        {
            return false;
        }
        m_Email = rspJson["Email"].string_value();

        if(!rspJson["Token"].is_string())
        {
            return false;
        }
        m_Token = rspJson["Token"].string_value();

        if(!rspJson["CityCode"].is_string())
        {
            return false;
        }
        m_CityCode = rspJson["CityCode"].string_value();

        if(!rspJson["StartDate"].is_string())
        {
            return false;
        }
        m_StartDate = rspJson["StartDate"].string_value();

        if(!rspJson["EndDate"].is_string())
        {
            return false;
        }
        m_EndDate = rspJson["EndDate"].string_value();
        return true;
    }

    virtual std::string ToString() override {
        json11::Json result=json11::Json::object({
            {"Email",m_Email},
            {"Token",m_Token},
            {"CityCode",m_CityCode},
            {"StartDate",m_StartDate},
            {"EndDate",m_EndDate},
        });
        return result.dump();
    }
public:
    std::string m_Email;
    std::string m_Token;
    std::string m_StartDate;
    std::string m_EndDate;
    std::string m_CityCode;
};


class HttpDelSubCityReq:public HttpReqBase
{
public:
    virtual bool FromString(const std::string& strValue) override
    {
        if(strValue.empty())
        {
            return false;
        }
        std::string strErr;
        auto rspJson = json11::Json::parse(strValue,strErr);
        if(!strErr.empty())
        {
            return false;
        }

        if(!rspJson["Email"].is_string())
        {
            return false;
        }
        m_Email = rspJson["Email"].string_value();

        if(!rspJson["Token"].is_string())
        {
            return false;
        }
        m_Token = rspJson["Token"].string_value();

        if(!rspJson["CityCode"].is_string())
        {
            return false;
        }
        m_CityCode = rspJson["CityCode"].string_value();
        return true;
    }

    virtual std::string ToString() override {
        json11::Json result=json11::Json::object({
            {"Email",m_Email},
            {"Token",m_Token},
            {"CityCode",m_CityCode},
        });
        return result.dump();
    }
public:
    std::string m_Email;
    std::string m_Token;
    std::string m_CityCode;
};

struct CityCodeAddrElem_t 
{
    std::string m_CityCode;
    std::string m_CityName;
};

class HttpGetCityCodeRsp:public HttpRspBase
{
public:
    explicit HttpGetCityCodeRsp():HttpRspBase(SUCCEED){

    }
    virtual ~HttpGetCityCodeRsp(){

    }
    virtual bool FromString(const std::string& strValue) override
    {
        if(strValue.empty())
        {
            return false;
        }
        std::string strErr;
        auto rspJson = json11::Json::parse(strValue,strErr);
        if(!strErr.empty())
        {
            return false;
        }
        auto jsonElem = rspJson["data"];
        CityCodeAddrElem_t elem;
        for(auto item:jsonElem.array_items())
        {
          
            if(!item["CityCode"].is_string())
            {
                return false;
            }
            elem.m_CityCode=item["CityCode"].string_value();


            if(!item["CityName"].is_number())
            {
                return false;
            }
            elem.m_CityName=item["CityName"].int_value();
            m_elemVec.push_back(elem);
        }
        return true;
    }

    virtual std::string ToString() const override {
        json11::Json::array elemJsonVec;
        for(const auto& elem:m_elemVec){
            json11::Json data = json11::Json::object({
                {"CityCode",elem.m_CityCode},
                {"CityName",elem.m_CityName},
            });
            elemJsonVec.push_back(data);
        }
        json11::Json result=json11::Json::object({
            {"code",static_cast<int>(m_code)},
            {"message",m_message},
            {"data",elemJsonVec},
        });
        return result.dump();
    }
public:
    std::vector<CityCodeAddrElem_t> m_elemVec;
};


using HTTP_GET_PARAMS_t = std::map<std::string,std::string>;
unsigned char ToHex(unsigned char x) 
{ 
    return  x > 9 ? x + 55 : x + 48; 
}
 
unsigned char FromHex(unsigned char x) 
{ 
    unsigned char y;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    return y;
}
 
std::string UrlEncode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) || 
            (str[i] == '-') ||
            (str[i] == '_') || 
            (str[i] == '.') || 
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}
 
std::string UrlDecode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (str[i] == '+') strTemp += ' ';
        else if (str[i] == '%')
        {
            //assert(i + 2 < length);
            unsigned char high = FromHex((unsigned char)str[++i]);
            unsigned char low = FromHex((unsigned char)str[++i]);
            strTemp += high*16 + low;
        }
        else strTemp += str[i];
    }
    return strTemp;
}

HTTP_GET_PARAMS_t ParseGetUrlParams(const std::string& strQueryParams)
{
    std::cout<<strQueryParams<<std::endl;
    std::vector<std::string> strVec;
    std::size_t startIndex = 0;
    std::size_t findIndex = 0;
    findIndex = strQueryParams.find("?",startIndex);
    while(findIndex != std::string::npos)
    {
        auto result = strQueryParams.substr(startIndex+1,findIndex-startIndex-1);
        std::cout<<"Result: "<<result<<std::endl;
        strVec.push_back(result);
        startIndex = findIndex;
        findIndex = strQueryParams.find("&",startIndex+1);
    }
    if(startIndex < strQueryParams.length())
    {
        startIndex == 0 ? 0 : startIndex++;
        strVec.push_back(strQueryParams.substr(startIndex,strQueryParams.length()-startIndex));
    }

    std::map<std::string,std::string> resultMap;
    for(auto item:strVec)
    {
        auto index = item.find("=");
        if(index != std::string::npos)
        {
            auto key =item.substr(0,index);
            auto value = item.substr(index+1,item.length()-index-1);
            std::cout<<"Key :"<<key<<" Value:"<<value<<std::endl;
            resultMap.insert(std::pair<std::string,std::string>(key,value));
        }
    }

    return resultMap;
} 
#endif