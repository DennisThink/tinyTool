
// Created by 王耀华 on 16/10/10.
//

#ifndef _SOURCE_SERVER_C_SOURCE_SERVER_H_
#define _SOURCE_SERVER_C_SOURCE_SERVER_H_
#include <map>
#include <functional>
//#include <atomic>
#include "json11.hpp"
#include "asio_common.h"
#include "CClientSess.h"
#include "CSessManager.h"
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
    std::shared_ptr<CSessManager> m_pSessManager=nullptr;

    std::error_code m_ec;

  public:
    static std::shared_ptr<spdlog::logger> ms_loger;
    CSourceServer(asio::io_service &io_service);

    /**
     * @brief 启动服务
     * 
     * @param callback,有错误发生的时候，用于回调的函数 
     */
    void start(const std::function<void(const std::error_code &)> &callback);

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
