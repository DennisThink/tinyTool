#include "CSessManager.h"
#include "CClientSess.h"
#include "CSourceServer.h"
#include <algorithm>
using namespace SourceServer;
namespace SourceServer{
std::shared_ptr<spdlog::logger> CSessManager::ms_loger;


void CSessManager::start(CSourceServer* pServer,asio::io_service& ioService, std::error_code& ec)
{
	INFO(ms_loger,"Queue Start");
	m_server=pServer;
	srand((int)time(NULL));
	//std::sort(m_config.m_connectCfgVec.begin(),m_config.m_connectCfgVec.end());
	int nSessIndex = 0;
	auto pSess = std::make_shared<CClientSess>(ioService,m_config.m_strServerIp,m_config.m_nPort,this);
	if(pSess)
	{
		INFO(ms_loger,"Add Sess To Map");
		m_SessMap.insert(std::pair<int,std::shared_ptr<CClientSess>>(nSessIndex,pSess));
	}
	nSessIndex++;

	for(auto& item:m_SessMap)
	{
		WARN(ms_loger,"Sess id:{}  Connect:{}",item.first,item.second->GetConnectInfo());
		item.second->StartConnect();
	}
	ec.clear();
}
void CSessManager::CheckSessConn()
{
	for(auto& item:m_SessMap)
	{
		item.second->SendKeepAlive();
	}
}

void CSessManager::OnTimer()
{
	INFO(this->ms_loger,"Queue On Timer");
	CheckSessConn();
}

void CSessManager::OnRecvMessage(const TransBaseMsg_t* pMsg)
{
	if(nullptr != pMsg)
	{
		m_server->OnRecvMessage(pMsg);
	}
}
}