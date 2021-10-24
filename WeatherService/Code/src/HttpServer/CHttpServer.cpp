

#include "CHttpServer.h"
#include "HttpReqRspStruct.h"
#include "CDate.h"
namespace SourceServer
{
	using namespace DennisThink;
    std::shared_ptr<spdlog::logger> CHttpServer::ms_loger;
    CMySqlConfig CHttpServer::m_mysqlConfig;
    void CHttpServer::OnTimer()
    {
        INFO(ms_loger, "{} {} On Timer at CHttpServer", __FILE__, __LINE__);
    }

	void CHttpServer::handleGetOneDayWeather(served::response & res, const served::request & req)
	{
		INFO(ms_loger,"URL:{} Params:{}  Body:{} ",req.url().URI(),req.url().query(),req.body());
		auto params = ParseGetUrlParams(req.url().query());
		std::string strCityCode = params["CityCode"];
		std::string strToken = params["Token"];

		if(strCityCode.empty())
		{
			HttpRspBase rspMsg(EMPTY_CITY_CODE);
			res<<rspMsg.ToString();
			INFO(ms_loger,"RSP:{}",rspMsg.ToString());
			return ;
		}
		if(strToken.empty())
		{
			HttpRspBase rspMsg(EMPTY_TOKEN);
			res<<rspMsg.ToString();
			INFO(ms_loger,"RSP:{}",rspMsg.ToString());
			return ;
		}
		C_T_WEATHER_BEAN weatherBean;
		
		Date dateUtil;
		m_mySqlUtil.GetOneDayWeatherByCityCode(strCityCode,dateUtil.ToString(),weatherBean);

		C_T_CITY_CODE_ADDR addrBean;
		m_mySqlUtil.GetCityCodeAddr(strCityCode,addrBean);

		HttpGetOneDayWeatherRsp rspMsg;
		OneDayWeatherElem_t elem;
		elem.m_cityName = addrBean.m_F_COUNTRY+addrBean.m_F_PROVINCE+addrBean.m_F_CITY+addrBean.m_F_AREA;
		elem.m_date = dateUtil.ToString();
		elem.m_highTemp = weatherBean.m_F_HIGH_TEMP;
		elem.m_lowTemp = weatherBean.m_F_LOW_TEMP;
		elem.m_weather = weatherBean.m_F_WEATHER;
		elem.m_windDirect = weatherBean.m_F_WIND_DIRECT;
		elem.m_windLevel = weatherBean.m_F_WIND_LEVEL;
		res<<rspMsg.ToString();
		INFO(ms_loger,"RSP:{}",rspMsg.ToString());
		return ;
	}

	void CHttpServer::handleGetOneWeekWeather(served::response & res, const served::request & req)
	{
		INFO(ms_loger,"URL:{} Params:{}  Body:{} ",req.url().URI(),req.url().query(),req.body());
		auto params = ParseGetUrlParams(req.url().query());
		std::string strCityCode = params["CityCode"];
		std::string strToken = params["Token"];
		if(strCityCode.empty())
		{
			HttpRspBase rspMsg(EMPTY_CITY_CODE);
			res<<rspMsg.ToString();
			INFO(ms_loger,"RSP:{}",rspMsg.ToString());
			return ;
		}
		if(strToken.empty())
		{
			HttpRspBase rspMsg(EMPTY_TOKEN);
			res<<rspMsg.ToString();
			INFO(ms_loger,"RSP:{}",rspMsg.ToString());
			return ;
		}
		C_T_WEATHER_BEAN weatherBean;
		
		C_T_CITY_CODE_ADDR addrBean;
		m_mySqlUtil.GetCityCodeAddr(strCityCode,addrBean);
		Date dateUtil;
		HttpGetOneWeekWeatherRsp rspMsg;
		for(int i = 0 ; i < 7 ; i++)
		{
			OneDayWeatherElem_t elem;
			m_mySqlUtil.GetOneDayWeatherByCityCode(strCityCode,dateUtil.ToString(),weatherBean);
			elem.m_cityName = addrBean.m_F_COUNTRY+addrBean.m_F_PROVINCE+addrBean.m_F_CITY+addrBean.m_F_AREA;
			elem.m_date = dateUtil.ToString();
			elem.m_highTemp = weatherBean.m_F_HIGH_TEMP;
			elem.m_lowTemp = weatherBean.m_F_LOW_TEMP;
			elem.m_weather = weatherBean.m_F_WEATHER;
			elem.m_windDirect = weatherBean.m_F_WIND_DIRECT;
			elem.m_windLevel = weatherBean.m_F_WIND_LEVEL;
			rspMsg.AddElem(elem);
			dateUtil=dateUtil+1;
		}
		res<<rspMsg.ToString();
		INFO(ms_loger,"RSP:{}",rspMsg.ToString());
		return ;
	}


	//get_sub_list_by_email
    void CHttpServer::handleGetSubListByEmail(served::response & res, const served::request & req){
		INFO(ms_loger,"URL:{} Params:{}  Body:{} ",req.url().URI(),req.url().query(),req.body());
		auto params = ParseGetUrlParams(req.url().query());
		std::string strEmail = params["email"];
		std::string strToken = params["token"];
		if(strEmail.empty())
		{
			HttpRspBase rspMsg(EMPTY_EMAIL);
			res<<rspMsg.ToString();
			INFO(ms_loger,"RSP:{}",rspMsg.ToString());
			return ;
		}
		if(strToken.empty())
		{
			HttpRspBase rspMsg(EMPTY_TOKEN);
			res<<rspMsg.ToString();
			INFO(ms_loger,"RSP:{}",rspMsg.ToString());
			return ;
		}
		C_T_SUBSCRIBE_EMAIL_BEAN_VECTOR_T emailList;
		m_mySqlUtil.SelectSubEmailList(strEmail,emailList);
		HttpGetEmailSubRsp rspMsg;
		EmailSubElem_t subElem;
		for(const auto item:emailList)
		{
			subElem.m_CityCode = item.m_F_CITY_CODE;
			subElem.m_StartDate = item.m_F_START_DATE;
			subElem.m_EndDate = item.m_F_END_DATE;
			rspMsg.AddElem(subElem);
		}
		res<<rspMsg.ToString();
		INFO(ms_loger,"RSP:{}",rspMsg.ToString());
		return ;
	}
    
	//add_sub_city_by_email
    void CHttpServer::handlePostAddSubCityByEmail(served::response & res, const served::request & req)
	{
		INFO(ms_loger,"URL:{} Params:{}  Body:{} ",req.url().URI(),req.url().query(),req.body());
		HttpAddSubCityReq reqMsg;
		if(reqMsg.FromString(req.body()))
		{
			if(m_mySqlUtil.DeleteSubEmailList(reqMsg.m_Email,reqMsg.m_CityCode))
			{

			}
			C_T_SUBSCRIBE_EMAIL_BEAN emailBean;
			emailBean.m_F_CITY_CODE = reqMsg.m_CityCode;
			emailBean.m_F_EMAIL = reqMsg.m_Email;
			emailBean.m_F_START_DATE = reqMsg.m_StartDate;
			emailBean.m_F_END_DATE = reqMsg.m_EndDate;
			m_mySqlUtil.InsertSubEmailList(emailBean);
			HttpRspBase rspMsg(SUCCEED);
			res<<rspMsg.ToString();
			INFO(ms_loger,"RSP:{}",rspMsg.ToString());
			return ;
		}
		else
		{
			HttpRspBase rspMsg(UNKNOWN_ERROR);
			res<<rspMsg.ToString();
			INFO(ms_loger,"RSP:{}",rspMsg.ToString());
			return ;
		}
	}
    //del_sub_city_by_email
    void CHttpServer::handlePostDelSubCityByEmail(served::response & res, const served::request & req)
	{
		INFO(ms_loger,"URL:{} Params:{}  Body:{} ",req.url().URI(),req.url().query(),req.body());
		HttpDelSubCityReq reqMsg;
		if(reqMsg.FromString(req.body()))
		{
			if(m_mySqlUtil.DeleteSubEmailList(reqMsg.m_Email,reqMsg.m_CityCode))
			{
				HttpRspBase rspMsg(SUCCEED);
				res<<rspMsg.ToString();
				INFO(ms_loger,"RSP:{}",rspMsg.ToString());
				return ;
			}
			else
			{
				HttpRspBase rspMsg(UNKNOWN_ERROR);
				INFO(ms_loger,"RSP:{}",rspMsg.ToString());
				res<<rspMsg.ToString();
				return ;
			}
			
		}
		else
		{
			HttpRspBase rspMsg(UNKNOWN_ERROR);
			res<<rspMsg.ToString();
			INFO(ms_loger,"RSP:{}",rspMsg.ToString());
			return ;
		}

	}
    //get_city_code
    void CHttpServer::handleGetCityCode(served::response & res, const served::request & req){
		INFO(ms_loger,"{}",UrlDecode(req.url().URI()));
		auto params = ParseGetUrlParams(UrlDecode(req.url().URI()));
		std::string strCountry = params["Country"];
		std::string strProvince = params["Province"];
		std::string strCity  = params["City"]; 
		INFO(ms_loger,"Country {} Province:{} City:{}",strCountry,strProvince,strCity);
		if(strCountry.empty())
		{
			HttpRspBase rspMsg(EMPTY_COUNTRY);
			res<<rspMsg.ToString();
			INFO(ms_loger,"RSP:{}",rspMsg.ToString());
			return ;
		}
		if(strCity.empty())
		{
			HttpRspBase rspMsg(EMPTY_CITY);
			res<<rspMsg.ToString();
			INFO(ms_loger,"RSP:{}",rspMsg.ToString());
			return ;
		}
		C_T_CITY_CODE_ADDR_ARRAY_T allBean;
		m_mySqlUtil.GetCityCodeByAddr(strProvince,strCity,allBean);

		HttpGetCityCodeRsp rspMsg;
		for(auto& item:allBean)
		{
			rspMsg.m_elemVec.push_back(CityCodeAddrElem_t{item.m_F_CITY_CODE,item.GetAddress()});
		}
		res<<rspMsg.ToString();
		INFO(ms_loger,"RSP:{}",rspMsg.ToString());
		return ;
	}  

	void CHttpServer::InitHandler()
    {
        auto self = shared_from_this();

        mux.handle("/weather/v1/get_one_day_base_weather")
		.get([self,this](served::response & res, const served::request & req) {
			handleGetOneDayWeather(res,req);
		});

		mux.handle("/weather/v1/get_one_week_base_weather")
		.get([self,this](served::response & res, const served::request & req) {
			handleGetOneWeekWeather(res,req);
		});

		mux.handle("/weather/v1/get_sub_list_by_email")
		.get([self,this](served::response & res, const served::request & req) {
			handleGetSubListByEmail(res,req);
		});

		mux.handle("/weather/v1/add_sub_city_by_email")
		.post([self,this](served::response & res, const served::request & req) {
			handlePostAddSubCityByEmail(res,req);
		});


		mux.handle("/weather/v1/del_sub_city_by_email")
		.post([self,this](served::response & res, const served::request & req) {
			handlePostDelSubCityByEmail(res,req);
			// read customer req.params["id"]
		});

		
		mux.handle("/weather/v1/get_city_code")
		.get([self,this](served::response & res, const served::request & req) {
			handleGetCityCode(res,req);
			// read customer req.params["id"]
		});
    }
} // namespace SourceServer