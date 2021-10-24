

#include "CMediumServer.h"
#include "json11.hpp"
#include "DaemonSvcApp.h"
namespace MediumServer
{
std::shared_ptr<spdlog::logger> CMediumServer::ms_loger;


void CMediumServer::loadConfig(const json11::Json &cfg, std::error_code& ec) 
{
	auto serverCfg = cfg["server"];
	INFO(ms_loger,"loadConfig");
    ec.clear();
    m_serverCfg.m_strServerIp=serverCfg["ip"].string_value();
    m_serverCfg.m_nPort=(uint16_t)serverCfg["port"].int_value();
    INFO(ms_loger,"ServerIp: {}",m_serverCfg.to_string());
    if(!m_serverCfg.Valid())
	{
		ERR(ms_loger,"Config Error {}",m_serverCfg.to_string());
        return;
    }

	auto clientsCfg = cfg["clients"];
	if(!clientsCfg.is_array())
	{
		ERR(ms_loger,"Clients Config Error {}",cfg.string_value());
        return;
	}
	for(auto item:clientsCfg.array_items())
	{
		IpPortCfg clientCfg;
		clientCfg.m_strServerIp = item["ip"].string_value();
		clientCfg.m_nPort = item["port"].int_value();
		INFO(ms_loger,"Client Config: {}",clientCfg.to_string());
		m_clientCfgVec.push_back(clientCfg);
	}
}

void CMediumServer::check_all_connect()
{
	for(auto item = m_listenList.begin() ; item != m_listenList.end() ; item++)
	{
		if((*item)->IsConnected())
		{
			INFO(ms_loger,"Client is Connected");
		}
		else
		{
			INFO(ms_loger,"Client is Closed");
			m_listenList.erase(item);
			break;
		}
	}
}
void CMediumServer::do_accept() 
{
		INFO(ms_loger,"CMediumServer do_accept");
        m_acceptor.async_accept(m_socket, [this](std::error_code ec) {
            if (!ec)
			{
			   INFO(ms_loger,"Server accept Successed");
               auto agentSess= std::make_shared<CServerSess>(std::move(m_socket),this);
			   agentSess->Start();
			   m_listenList.push_back(agentSess);
			}
            do_accept();
        });
}

void CMediumServer::OnTimer()
{
	INFO(this->ms_loger,"On Timer at APNsServer");
	for(auto& item:m_queueVec)
	{
		item->OnTimer();
	}
	check_all_connect();
}
void CMediumServer::SetTimer(int nSeconds)
{
	if(m_timer)
	{
		m_timer->expires_from_now(std::chrono::seconds(nSeconds));
		auto self = shared_from_this();
		m_timer->async_wait([this,nSeconds,self](const std::error_code& ec){
			if(!ec)
			{
				INFO(this->ms_loger,"On Timer at APNsServer");
				this->OnTimer();
				this->SetTimer(nSeconds);
			}
			else
			{
				WARN(this->ms_loger,"On Timer at APNsServer {}",ec.message());
			}
			

		});
	}
}
std::string CMediumServer::getServerIpPort()
{
	return m_serverCfg.to_string();
}
}