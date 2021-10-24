//
// Created by 王耀华 on 16/10/10.
//

#include "CSourceServer.h"
#include "DaemonSvcApp.h"
#include "json11.hpp"

namespace SourceServer
{
std::shared_ptr<spdlog::logger> CSourceServer::ms_loger;
CSourceServer::CSourceServer(asio::io_service &io_service) : m_ioService(io_service),m_acceptor(io_service), m_socket(io_service)
    {
        if (!m_timer)
        {
            m_timer = std::make_shared<asio::high_resolution_timer>(m_ioService);
        }
    }
void CSourceServer::loadConfig(const json11::Json &cfg, std::error_code &ec)
{
	INFO(ms_loger, "loadConfig");
	ec.clear();
	IpPortCfg config;
	config.m_strServerIp = cfg["ip"].string_value();
	config.m_nPort = (uint16_t)cfg["port"].int_value();
	INFO(ms_loger, "Server   ", config.to_string());

	if (!config.Valid())
	{
		ERR(ms_loger, "Config Error {}", config.to_string());
		return;
	}
	else
	{
		m_connectVec.push_back(config);
	}
}
void CSourceServer::start(const std::function<void(const std::error_code &)> &callback)
    {
        SetTimer(5);
        if( (!m_pSessManager)  && 
            (!m_connectVec.empty())  )
        {     
            m_pSessManager = std::make_shared<CSessManager>(m_connectVec[0]);   
            std::error_code ec;
            m_pSessManager->start(this,m_ioService,ec);
            if(ec)
            {
                callback(ec);
            }
        }
        else
        {
			ERR(ms_loger,"No Queue To Start");
        }
    }
void CSourceServer::OnTimer()
{
	INFO(ms_loger, "{0} {1} On Timer at CSourceServer", __FILE__, __LINE__);
	if (m_pSessManager)
	{
		m_pSessManager->OnTimer();
	}
	else
	{
		ERR(this->ms_loger, "No Push Queue");
	}
}
void CSourceServer::SetTimer(int nSeconds)
{
	if (m_timer)
	{
		m_timer->expires_from_now(std::chrono::seconds(nSeconds));
		m_timer->async_wait([this, nSeconds](const std::error_code &ec) {
			INFO(this->ms_loger, "On Timer at APNsServer");
			this->OnTimer();
			if(!ec)
			{
				this->SetTimer(nSeconds);
			}	
		});
	}
}

void CSourceServer::OnRecvMessage(const TransBaseMsg_t* pMsg)
{
	if(nullptr != pMsg)
	{
		INFO(ms_loger,"Msg:{}",pMsg->to_string());
	}
}
} // namespace SourceServer