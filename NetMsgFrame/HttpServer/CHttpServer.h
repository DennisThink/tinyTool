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
namespace SourceServer
{
class CHttpServer : public std::enable_shared_from_this<CHttpServer>
{
protected:
    //io_service的实例
    asio::io_service &m_ioService;
    
    std::error_code m_ec;
    std::shared_ptr<served::net::server> m_pServer;
    served::multiplexer mux;
  public:
    static std::shared_ptr<spdlog::logger> ms_loger;
    CHttpServer(asio::io_service &io_service) : m_ioService(io_service)
    {

    }

    /**
     * @brief 启动服务
     * 
     * @param callback,有错误发生的时候，用于回调的函数 
     */
    void start(const std::function<void(const std::error_code &)> &/*callback*/)
    {
        m_pServer = std::make_shared<served::net::server>(m_ioService,"127.0.0.1","8123",mux);
    }

    //获取server的ip和端口,
    std::string getServerIpPort();
    
    //响应定时器
    void OnTimer();

private:
    void InitHandler();    
};
}
#endif