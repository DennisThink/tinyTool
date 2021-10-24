#include "CPushQueue.h"
#include "CClientSess.h"
#include <algorithm>
namespace MediumServer
{
std::shared_ptr<spdlog::logger> CClientSessManager::ms_loger;


//收到消息时,发送给server
void CClientSessManager::send_back(const TransBaseMsg_t* msg)
{
	if(msg)
	{
		INFO(ms_loger,"Good Mesg");
	}
	//m_server->send_back(msg);
}

//TODO:off by one bug check
//处理流程:
//1,在权重队列中查找当前partnerid,
//	1.1 存在,
//	1.2 不存在,则建立
//
//	1.3 根据连接的ip和port查找连接是否存在,
//	1.3.1 存在,
//	1.3.2 不存在,建立连接
//
//	1.4更新权重表
//2,初始化keep-alive的timer和紧急timer
//3,设置keep-alivetimer,检查sess
void CClientSessManager::start(CMediumServer* pServer,asio::io_service& ioService, std::error_code& ec)
{
	m_server=pServer;
	auto pSess = std::make_shared<CClientSess>(ioService,m_config.m_strServerIp,m_config.m_nPort,this);
	m_SessMap.insert(std::pair<int,std::shared_ptr<CClientSess>>(1,pSess));
	pSess->StartConnect();
	ec.clear();
}

void CClientSessManager::SendTo(const TransBaseMsg_t* msg)
{
	if(nullptr != msg)
	{
		return ;
	}
}

void CClientSessManager::OnTimer()
{
	CheckSessConn();
}
void CClientSessManager::CheckSessConn()
{
	for(auto sessItem:m_SessMap)
	{
		if(sessItem.second->IsConnect())
		{
			WARN(ms_loger,"{} Connected",sessItem.second->GetConnectInfo());
			sessItem.second->SendKeepAlive();
		}
		else
		{
			WARN(ms_loger,"{} Not Connected",sessItem.second->GetConnectInfo());
			sessItem.second->StartConnect();
		}
	}
}


}