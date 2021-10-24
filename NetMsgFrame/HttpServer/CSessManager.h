#ifndef _CPUSHQUEUE_H_
#define _CPUSHQUEUE_H_

#include <atomic>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <functional>
#include <vector>

#include "json11.hpp"
#include "Log.h"
#include "CommonMsg.h"
#include "asio_common.h"
namespace SourceServer
{
class CAPNsSess;
class CSourceServer;
} // namespace SourceServer

//管理链接的第二级控制类
namespace SourceServer
{
class CSessManager : public std::enable_shared_from_this<CSessManager>
{
  protected:
    //TCP连接的IP和端口配置
    IpPortCfg m_config;

    std::map<int, std::shared_ptr<CAPNsSess>> m_SessMap;

    //主要的控制类
    CSourceServer *m_server;

  public:
    //用于记录日志
    static std::shared_ptr<spdlog::logger> ms_loger;

  public:
    //检查Sess的连接情况
    void CheckSessConn();
    CSessManager(const IpPortCfg &cfg) : m_config(cfg)
    {
    }

    //执行定时任务
    void OnTimer();
    
    //启动队列
    void start(CSourceServer *pServer, asio::io_service &ioService, std::error_code &ec);
    
    //响应Sess收到的消息
    void OnRecvMessage(const TransBaseMsg_t* pMsg);
};
} // namespace SourceServer
#endif
