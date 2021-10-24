
#ifndef HTTP2T1_CAGENTSERVER_H
#define HTTP2T1_CAGENTSERVER_H
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
#include "CHttpServer.h"
#include "CommonMsg.h"
#include "Log.h"

using tcp = asio::ip::tcp;

namespace SourceServer{
    /**
     * @brief SourceServer的主类，用于管理CSessManager
     * 
     */
class CSourceServer : public std::enable_shared_from_this<CSourceServer>
{
protected:
    //io_service的实例
    asio::io_service &m_ioService;
    
    //tcp Server
    tcp::acceptor m_acceptor;

    //用来监听的套接字
    tcp::socket m_socket;

    //定时器
    std::shared_ptr<asio::high_resolution_timer> m_timer = nullptr;
    
    //连接数组
    std::vector<IpPortCfg> m_connectVec;

    //连接使用的Manager，如果连接多，可以考虑改为数组
    std::shared_ptr<CHttpServer>  m_pHttpServer=nullptr;


    std::error_code m_ec;

  public:
    static std::shared_ptr<spdlog::logger> ms_loger;
    CSourceServer(asio::io_service &io_service) : m_ioService(io_service),m_acceptor(io_service), m_socket(io_service)
    {
        if (!m_timer)
        {
            m_timer = std::make_shared<asio::high_resolution_timer>(m_ioService);
        }
    }

    /**
     * @brief 启动服务
     * 
     * @param callback,有错误发生的时候，用于回调的函数 
     */
    void start(const std::function<void(const std::error_code &)> &callback)
    {
        SetTimer(5);
        if( (!m_pHttpServer)  && 
            (!m_connectVec.empty())  )
        {     
            m_pHttpServer = std::make_shared<CHttpServer>(m_ioService);
            m_pHttpServer->SetServerPort(m_connectVec[0].m_strServerIp,m_connectVec[0].m_nPort); 
            std::error_code ec;
            m_pHttpServer->start(callback);
            if(ec)
            {
                callback(ec);
            }
          
        }
        else
        {
            ERROR(ms_loger,"No Queue To Start");
        }
    }

    //响应收到的消息，由CSessManager调用
    void OnRecvMessage(const TransBaseMsg_t* pMsg);

    //加载配置
    void loadConfig(const json11::Json &cfg, std::error_code &ec);

    //获取server的ip和端口,
    std::string getServerIpPort();
private:
    //设置定时器
    void SetTimer(int nSeconds);
    
    //响应定时器
    void OnTimer();
};
}
#endif //HTTP2T1_CAGENTSERVER_H
