
#include "CEndServer.h"
#include "CServerSess.h"
#include "json11.hpp"
#include "planform.h"
namespace EndServer
{
std::shared_ptr<spdlog::logger> CEndServer::ms_loger;

CEndServer::Config CEndServer::ms_config;
/**
 * @brief 构造函数，使用asio::io_service构造一个CEndServer
 * 
 * @param io_service 
 */
CEndServer::CEndServer(asio::io_service &io_service)
	: m_ioService(io_service), m_acceptor(io_service), m_socket(io_service)
{
	if (!m_timer)
	{
		m_timer = std::make_shared<asio::high_resolution_timer>(m_ioService);
		INFO(ms_loger, "Init Timer");
	}
}

/**
 * @brief 将消息发送回
 * 
 * @param msg 需要发送的消息
 */
void CEndServer::SendBack(const TransBaseMsg_t *msg)
{
	INFO(ms_loger, "MSG {}", msg->to_string());
}

/**
 * @brief 加载json中的配置文件
 * 
 * @param cfg 待加载的配置
 * @param ec 错误信息
 */
void CEndServer::LoadConfig(const json11::Json &cfg, std::error_code &ec)
{
	INFO(ms_loger, "loadConfig");
	ms_config.logr = CEndServer::ms_loger;
	ec.clear();
	ms_config.ip = cfg["ip"].string_value();
	ms_config.port = (uint16_t)cfg["port"].int_value();
	INFO(ms_loger, "ServerIp: {}", ms_config.ip);
	INFO(ms_loger, "ServerPort: {}", ms_config.port);

	if (!ms_config.ip.length() || !ms_config.port)
	{
		ERR(ms_loger, "Config Error {} {}", ms_config.ip, ms_config.port);
		// ec<<;
		return;
	}
	else
	{
	}
}

/**
 * @brief 检查所有的socket连接
 * 
 */
void CEndServer::checkAllConnect()
{
	for (auto item = m_listenList.begin(); item != m_listenList.end(); item++)
	{
		if ((*item)->IsConnected())
		{
			INFO(ms_loger, "Client is Connected");
		}
		else
		{
			INFO(ms_loger, "Client is Closed");
			m_listenList.erase(item);
			break;
		}
	}
}

/**
 * @brief 接收socket连接
 * 
 */
void CEndServer::doAccept()
{
	INFO(ms_loger, "CEndServer do_accept");
	auto self = shared_from_this();
	m_acceptor.async_accept(m_socket, [this, self](std::error_code ec) {
		if (!ec)
		{
			INFO(ms_loger, "Server accept Successed");
			auto agentSess = std::make_shared<CServerSess>(std::move(m_socket), this);
			agentSess->Start();
			m_listenList.push_back(agentSess);
		}
		else
		{
			WARN(ms_loger, "{} {} Expection {}", __FILE__, __LINE__, ec.message());
		}
		doAccept();
	});
}

/**
 * @brief 响应定时器，所有需要定时处理的任务，可以放在此处
 * 
 */
void CEndServer::onTimer() { INFO(this->ms_loger, "On Timer at APNsServer"); }


/**
 * @brief 设置定时器
 * 
 * @param nSeconds 定时器时间，单位秒
 */
void CEndServer::setTimer(int nSeconds)
{
	if (m_timer)
	{
		INFO(ms_loger, "Set Timer {}", nSeconds);
		m_timer->expires_from_now(std::chrono::seconds(nSeconds));
		auto self = shared_from_this();
		m_timer->async_wait([this, nSeconds, self](const std::error_code &ec) {
			INFO(this->ms_loger, "On Timer at APNsServer");
			this->onTimer();
			this->setTimer(nSeconds);
			if (!ec)
			{
			}
			else
			{
				WARN(ms_loger, "Error:{}", ec.message());
			}
		});
	}
	else
	{
		WARN(ms_loger, "Can not Set Timer {}", nSeconds);
	}
}

/**
 * @brief 获取服务器的监听信息表示
 * 
 * @return std::string 返回ip端口的表示，类似于127.0.0.1:80
 */
std::string CEndServer::GetServerIpPort()
{
	return ms_config.ip + ":" + std::to_string(ms_config.port);
}

/**
 * @brief 启动服务器
 * 
 * @param callback，发生错误时的回调函数 
 */
void CEndServer::Start(
	const std::function<void(const std::error_code &)> &callback)
{
	// INFO(ms_loger,"Server Start Service")));
	std::error_code ec;
	asio::ip::tcp::endpoint endpoint;
	if (ms_config.ip.length() > 0)
	{
		endpoint = asio::ip::tcp::endpoint(
			asio::ip::address::from_string(ms_config.ip), ms_config.port);
	}
	else
	{
		endpoint = asio::ip::tcp::endpoint(asio::ip::tcp::v4(), ms_config.port);
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
		WARN(ms_loger, "Bind To {}:{} Succeed", ms_config.ip, ms_config.port);
		m_acceptor.listen(asio::socket_base::max_connections, ec);
		if (!ec)
		{
			WARN(ms_loger, "Listen To {}:{} Succeed", ms_config.ip, ms_config.port);
		}
		else
		{
			WARN(ms_loger, "Listen To {}:{} Failed, Reason:{} {}", ms_config.ip,
				 ms_config.port, ec.value(), ec.message());
		}
		setTimer(5);
		doAccept();
	}
	else
	{
		WARN(ms_loger, "Bind To {}:{} Failed", ms_config.ip, ms_config.port);
		callback(ec);
#ifndef WIN32
		exit(BIND_FAILED_EXIT);
#endif
	}
}
} // namespace EndServer