#ifndef _HTTP_TO_TCP_HTTP_SERVER_H_
#define _HTTP_TO_TCP_HTTP_SERVER_H_
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/high_resolution_timer.hpp>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <functional>
#include <atomic>
#include "json11.hpp"
#include "CommonMsg.h"
#include "Log.h"

#include <served/served.hpp>
#include <served/request_error.hpp>
#include <served/status.hpp>
#include "CMySQLBean.h"
#include "CMySql.h"
#include "CommonConfigStruct.h"
namespace SourceServer
{
    using namespace DennisThink;
class CHttpServer : public std::enable_shared_from_this<CHttpServer>
{
protected:
    //io_service的实例
    asio::io_service &m_ioService;
    std::error_code m_ec;
    std::shared_ptr<served::net::server> m_pServer;
    served::multiplexer mux;
    CMysql m_mySqlUtil;

    std::string m_serverIp;
    int m_port;
  public:
    static std::shared_ptr<spdlog::logger> ms_loger;
    static CMySqlConfig m_mysqlConfig;
    CHttpServer(asio::io_service &io_service) : m_ioService(io_service)
    {
        m_mySqlUtil.Connect(m_mysqlConfig.m_Ip,
                            m_mysqlConfig.m_Port,
                            m_mysqlConfig.m_db,
                            m_mysqlConfig.m_userName,
                            m_mysqlConfig.m_passwd);
        //m_mySqlUtil.Connect(mysqlServerIp,mysqlServerPort,db,userName,passwd);
    }
    virtual ~CHttpServer(){
        m_mySqlUtil.Close();
    }

    /**
     * @brief 启动服务
     * 
     * @param callback,有错误发生的时候，用于回调的函数 
     */
    void start(const std::function<void(const std::error_code &)> &/*callback*/)
    {
        InitHandler();
        m_pServer = std::make_shared<served::net::server>(m_ioService,m_serverIp,std::to_string(m_port),mux);
    }

    //响应定时器
    void OnTimer();

    void SetServerPort(const std::string strIp,const int port){
        m_serverIp = strIp;
        m_port = port;
    }
private:
    void InitHandler();
    void handleGetOneDayWeather(served::response & res, const served::request & req);
    void handleGetOneWeekWeather(served::response & res, const served::request & req);  
    //get_sub_list_by_email
    void handleGetSubListByEmail(served::response & res, const served::request & req);
    //add_sub_city_by_email
    void handlePostAddSubCityByEmail(served::response & res, const served::request & req);
    //del_sub_city_by_email
    void handlePostDelSubCityByEmail(served::response & res, const served::request & req);
    //
    void handleGetCityCode(served::response & res, const served::request & req);  
};
}
#endif