#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include "restclient-cpp/restclient.h"
#include "HttpReqRspStruct.h"
//std::string strURL_HEAD="https://api.dennisthink.com";
TEST_SUITE_BEGIN("HTTP_NET_TEST");
std::string strURL_HEAD="http://127.0.0.1:8123";
TEST_CASE("HttpTest1"){
    RestClient::Response res = RestClient::get("https://www.baidu.com/");
    std::cout<<res.code<<std::endl;
    std::cout<<res.body<<std::endl;
}

TEST_CASE("GET_ONE_DAY_EMPTY_CITY_CODE"){
    std::string strUrl=strURL_HEAD+"/weather/v1/get_one_day_base_weather";
    RestClient::Response res = RestClient::get(strUrl);
    HttpRspBase baseRsp;
    CHECK_EQ(200,res.code);
    CHECK_EQ(true,baseRsp.FromString(res.body));
    CHECK_EQ(EMPTY_CITY_CODE,baseRsp.GetCode());
}

TEST_CASE("GET_ONE_DAY_EMPTY_TOKEN"){
    std::string strUrl=strURL_HEAD+"/weather/v1/get_one_day_base_weather?CityCode=123455";
    RestClient::Response res = RestClient::get(strUrl);
    HttpRspBase baseRsp;
    CHECK_EQ(200,res.code);
    CHECK_EQ(true,baseRsp.FromString(res.body));
    CHECK_EQ(EMPTY_TOKEN,baseRsp.GetCode());
}


TEST_CASE("GET_ONE_WEEK_EMPTY_CITY_CODE"){
    std::string strUrl=strURL_HEAD+"/weather/v1/get_one_week_base_weather?";
    RestClient::Response res = RestClient::get(strUrl);
    HttpRspBase baseRsp;
    CHECK_EQ(200,res.code);
    CHECK_EQ(true,baseRsp.FromString(res.body));
    CHECK_EQ(EMPTY_CITY_CODE,baseRsp.GetCode());
}


TEST_CASE("GET_ONE_WEEK_EMPTY_TOKEN"){
    std::string strUrl=strURL_HEAD+"/weather/v1/get_one_week_base_weather?CityCode=203-02-08-01-001";
    RestClient::Response res = RestClient::get(strUrl);
    HttpRspBase baseRsp;
    CHECK_EQ(200,res.code);
    CHECK_EQ(true,baseRsp.FromString(res.body));
    CHECK_EQ(EMPTY_TOKEN,baseRsp.GetCode());
}


TEST_CASE("GET_ONE_DAY_GOOD"){
    std::string strUrl=strURL_HEAD+"/weather/v1/get_one_day_base_weather?CityCode=203-02-08-01-001&Token=1111";
    RestClient::Response res = RestClient::get(strUrl);
    HttpRspBase baseRsp;
    CHECK_EQ(200,res.code);
    CHECK_EQ(true,baseRsp.FromString(res.body));
    CHECK_EQ(SUCCEED,baseRsp.GetCode());
}
TEST_CASE("GET_CITY_CODE"){
    std::string strUrl=strURL_HEAD+"/weather/v1/get_city_code";
    RestClient::Response res = RestClient::get(strUrl);
    HttpRspBase baseRsp;
    CHECK_EQ(200,res.code);
    CHECK_EQ(true,baseRsp.FromString(res.body));
    CHECK_EQ(EMPTY_COUNTRY,baseRsp.GetCode());
}
TEST_SUITE_END();

TEST_SUITE_BEGIN("HTTP_LOCAL");
TEST_CASE("GET_ONE_DAY_RSP"){
std::string strRsp=R"({
    "code":0,
    "message":"Succeed",
    "data":{
        "date":"2019-02-01",
        "name":"北京市房山区",
        "weather":"晴天",
        "high_temp":23,
        "low_temp":10,
        "wind_level":"4",
        "wind_direct":"东南风"
    }
})";
HttpGetOneDayWeatherRsp rsp;
CHECK_EQ(true,rsp.FromString(strRsp));
}
TEST_SUITE_END();
/*
TEST_CASE("GET_ONE_WEEK_GOOD"){
    std::string strUrl=strURL_HEAD+"/weather/v1/get_one_week_base_weather?CityCode=123455&Token=1111";
    RestClient::Response res = RestClient::get(strUrl);
    HttpRspBase baseRsp;
    CHECK_EQ(200,res.code);
    CHECK_EQ(true,baseRsp.FromString(res.body));
    CHECK_EQ(SUCCEED,baseRsp.GetCode());WW
}

TEST_CASE("GET_ONE_WEEK_GOOD"){
    std::string strUrl=strURL_HEAD+"/weather/v1/get_one_week_base_weather?CityCode=123455&Token=1111";
    RestClient::Response res = RestClient::get(strUrl);
    HttpRspBase baseRsp;
    CHECK_EQ(200,res.code);
    CHECK_EQ(true,baseRsp.FromString(res.body));
    CHECK_EQ(SUCCEED,baseRsp.GetCode());
}

TEST_CASE("GET_ONE_WEEK_GOOD"){
    std::string strUrl=strURL_HEAD+"/weather/v1/get_one_week_base_weather?CityCode=123455&Token=1111";
    RestClient::Response res = RestClient::get(strUrl);
    HttpRspBase baseRsp;
    CHECK_EQ(200,res.code);
    CHECK_EQ(true,baseRsp.FromString(res.body));
    CHECK_EQ(SUCCEED,baseRsp.GetCode());
}

TEST_CASE("GET_ONE_WEEK_GOOD"){
    std::string strUrl=strURL_HEAD+"/weather/v1/get_one_week_base_weather?CityCode=123455&Token=1111";
    RestClient::Response res = RestClient::get(strUrl);
    HttpRspBase baseRsp;
    CHECK_EQ(200,res.code);
    CHECK_EQ(true,baseRsp.FromString(res.body));
    CHECK_EQ(SUCCEED,baseRsp.GetCode());
}

TEST_CASE("GET_ONE_WEEK_GOOD"){
    std::string strUrl=strURL_HEAD+"/weather/v1/get_one_week_base_weather?CityCode=123455&Token=1111";
    RestClient::Response res = RestClient::get(strUrl);
    HttpRspBase baseRsp;
    CHECK_EQ(200,res.code);
    CHECK_EQ(true,baseRsp.FromString(res.body));
    CHECK_EQ(SUCCEED,baseRsp.GetCode());
}*/



