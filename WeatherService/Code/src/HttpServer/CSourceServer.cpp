//
// Created by 王耀华 on 16/10/10.
//

#include "CSourceServer.h"
#include "DaemonSvcApp.h"
#include "json11.hpp"

namespace SourceServer
{
std::shared_ptr<spdlog::logger> CSourceServer::ms_loger;
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
		ERROR(ms_loger, "Config Error {}", config.to_string());
		return;
	}
	else
	{
		m_connectVec.push_back(config);
	}
}

void CSourceServer::OnTimer()
{
	INFO(ms_loger, "{0} {1} On Timer at CSourceServer", __FILE__, __LINE__);
}
void CSourceServer::SetTimer(int nSeconds)
{
	if (m_timer)
	{
		m_timer->expires_from_now(std::chrono::seconds(nSeconds));
		auto self = shared_from_this();
		m_timer->async_wait([this, self,nSeconds](const std::error_code &ec) {
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