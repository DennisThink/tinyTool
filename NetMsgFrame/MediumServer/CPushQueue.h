#ifndef _CPUSHQUEUE_H_
#define _CPUSHQUEUE_H_
#define ASIO_STANDALONE
#include <map>
#include <set>
#include <list>
#include <queue>
#include <functional>
#include <asio.hpp>
#include <atomic>
#include "json11.hpp"
#include "spdlog/spdlog.h"
#include "CommonMsg.h"
#include <asio/high_resolution_timer.hpp>
#include <vector>
#include <map>
namespace MediumServer
{

class CClientSess;
class CMediumServer;

class CClientSessManager : public std::enable_shared_from_this<CClientSessManager>
{
protected:
	IpPortCfg m_config;

    enum{MAX_QUEUED=5000};


	std::map<int,std::shared_ptr<CClientSess>> m_SessMap;//所有推送连接的map,第一项是连接的id

	CMediumServer * m_server; //主要的控制类
	


	//检查Sess的连接情况
	void CheckSessConn();

protected:
	void send_back(const TransBaseMsg_t* msg);
public:
	//用于记录日志
	static std::shared_ptr<spdlog::logger> ms_loger;
public:
	void SendTo(const TransBaseMsg_t* msg);
	void OnTimer();
    CClientSessManager(const IpPortCfg& cfg):m_config(cfg){}
    //启动队列
    void start(CMediumServer* pServer,asio::io_service& ioService, std::error_code& ec);

};
}
#endif
