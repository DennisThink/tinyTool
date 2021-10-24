

#ifndef _MEDIUM_SERVER_C_MEDIUM_SERVER_H_
#define _MEDIUM_SERVER_C_MEDIUM_SERVER_H_

#include <functional>

#include <map>

#include "CommonMsg.h"
#include "Log.h"
#include "asio_common.h"
#include "json11.hpp"

#include "CClientSess.h"
#include "CPushQueue.h"
#include "CServerSess.h"
#include "planform.h"
namespace MediumServer
{
using tcp = asio::ip::tcp;
class CAgentSess;
class CMediumServer : public std::enable_shared_from_this<CMediumServer>
{
  protected:
    asio::io_service &m_ioService;

    tcp::socket m_socket;

    tcp::acceptor m_acceptor;

    //服务器的IP端口设置
    IpPortCfg m_serverCfg;

    //所有的客户端的连接客户端的IP端口设置
    std::vector<IpPortCfg> m_clientCfgVec;

    std::vector<std::shared_ptr<CServerSess>> m_listenList; //监听的套接字的列表

    std::shared_ptr<asio::high_resolution_timer> m_timer = nullptr;
    void SetTimer(int nSeconds);
    void OnTimer();
    //推送证书及队列

    // static std::shared_ptr<CPushQueue> m_ios_pushQueue;
    std::shared_ptr<CClientSessManager> m_android_pushQueue = nullptr;
    std::vector<std::shared_ptr<CClientSessManager>> m_queueVec;
    std::error_code m_ec;

    void do_accept();

  public:
    static std::shared_ptr<spdlog::logger> ms_loger;
    inline IpPortCfg &config() { return m_serverCfg; }

    void send_back(const TransBaseMsg_t *msg);
    void check_all_connect();
    void send_to_all_connect(std::shared_ptr<TransBaseMsg_t> msg);
    CMediumServer(asio::io_service &io_service)
        : m_ioService(io_service), m_socket(io_service), m_acceptor(io_service)
    {
        if (!m_timer)
        {
            m_timer = std::make_shared<asio::high_resolution_timer>(m_ioService);
        }
    }

    void start(const std::function<void(const std::error_code &)> &callback)
    {
        if (!m_serverCfg.Valid())
        {
            ERR(ms_loger, "ServerConfig Is Error {}", m_serverCfg.to_string());
            return;
        }
        INFO(ms_loger, "Server Start Service");
        std::error_code ec;
        asio::ip::tcp::endpoint endpoint;
        if (m_serverCfg.m_strServerIp.length() > 0)
        {
            endpoint = asio::ip::tcp::endpoint(
                asio::ip::address::from_string(m_serverCfg.m_strServerIp),
                m_serverCfg.m_nPort);
        }
        else
        {
            endpoint =
                asio::ip::tcp::endpoint(asio::ip::tcp::v4(), m_serverCfg.m_nPort);
        }
        WARN(ms_loger, "Before Open");
        m_acceptor.open(endpoint.protocol());
        WARN(ms_loger, "Before Set Option");
        m_acceptor.set_option(asio::socket_base::reuse_address(true));
        WARN(ms_loger, "Before Bind");
        m_acceptor.bind(endpoint, ec);
        WARN(ms_loger, "Before Listen");
        if (!ec)
        {
            WARN(ms_loger, "Bind To {}:{} Succeed", m_serverCfg.to_string());
            m_acceptor.listen(asio::socket_base::max_connections, ec);
            if (!ec)
            {
                WARN(ms_loger, "Listen To {} Succeed", m_serverCfg.to_string());
            }
            else
            {
                WARN(ms_loger, "Listen To {} Failed, Reason:{} {}",
                     m_serverCfg.to_string(), ec.value(), ec.message());
            }
            SetTimer(5);
            do_accept();
            {
                for (auto item : m_clientCfgVec)
                {
                    if (item.Valid())
                    {
                        auto pQueue = std::make_shared<CClientSessManager>(item);
                        m_queueVec.push_back(pQueue);
                        std::error_code ec;
                        pQueue->start(this, m_ioService, ec);
                        INFO(ms_loger, "ConnectTo: {}", item.to_string());
                    }
                }
            }
        }
        else
        {
            WARN(ms_loger, "Bind To {} Failed", m_serverCfg.to_string());
            callback(ec);
#ifndef WIN32
            exit(BIND_FAILED_EXIT);
#endif
        }
    }

    int push_message();

    void loadConfig(const json11::Json &cfg, std::error_code &ec);
    //获取server的ip和端口,
    std::string getServerIpPort();
};
} // namespace MediumServer

#endif // HTTP2T1_CAGENTSERVER_H
