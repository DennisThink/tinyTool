#define DOCTEST_CONFIG_IMPLEMENT
//#include "../HttpClient/http_parse/http_req_rsp.h"
#include <doctest/doctest.h>
#include "HttpReqRspStruct.h"
TEST_CASE("ParseParam"){
    std::string strQuery="CityCode=22222&Token=2222";
    auto result = ParseGetUrlParams(strQuery);
}

TEST_CASE("ParseParam2"){
    std::string strQuery="CityCode=22222";
    auto result = ParseGetUrlParams(strQuery);
}

TEST_CASE("URL_DECODE_1"){
    std::string strUrl="/weather/v1/get_city_code?Country=%E4%B8%AD%E5%9B%BD&Province=%E5%8C%97%E4%BA%AC&City=%E5%9F%8E%E5%8C%BA&Province=%E5%8C%97%E4%BA%AC Params:Country=%E4%B8%AD%E5%9B%BD&Province=%E5%8C%97%E4%BA%AC&City=%E5%9F%8E%E5%8C%BA&Province=%E5%8C%97%E4%BA%AC";
    std::cout<<UrlDecode(strUrl)<<std::endl;
    auto result = ParseGetUrlParams(UrlDecode(strUrl));
    for(auto item:result)
    {
        std::cout<<item.first<<"  "<<item.second<<std::endl;
    }

}