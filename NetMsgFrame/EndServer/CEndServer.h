

#ifndef _END_SERVER_C_END_SERVER_H_
#define _END_SERVER_C_END_SERVER_H_

#include "CommonMsg.h"
#include "Log.h"
#include "asio_common.h"
#include "json11.hpp"

namespace EndServer
{

using tcp = asio::ip::tcp;
class CServerSess;

class CEndServer : public std::enable_shared_from_this<CEndServer>
{
protected:
    asio::io_service &m_ioService;
    tcp::acceptor m_acceptor;
    tcp::socket m_socket;
    struct Config
    {
        std::string ip;
        uint16_t port;
        std::shared_ptr<spdlog::logger> logr;
    };
    static Config ms_config;

    std::vector<std::shared_ptr<CServerSess>> m_listenList; //监听的套接字的列表

    std::shared_ptr<asio::high_resolution_timer> m_timer = nullptr;
    std::error_code m_ec;
    void setTimer(int nSeconds);
    void onTimer();
    void doAccept();
    void checkAllConnect();
public:
    static std::shared_ptr<spdlog::logger> ms_loger;
    static inline Config &GetConfig() { return ms_config; }

    CEndServer(asio::io_service &io_service);
    void Start(const std::function<void(const std::error_code &)> &callback);

    void LoadConfig(const json11::Json &cfg, std::error_code &ec);
    //获取server的ip和端口,

    void SendBack(const TransBaseMsg_t *msg);
    static std::string GetServerIpPort();
};
} // namespace EndServer

#endif // HTTP2T1_CAGENTSERVER_H
