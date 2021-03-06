
/**
 * @file CClientSess.cpp
 * @author DennisMi (https://www.dennisthink.com/)
 * @brief 连接服务器的Sess
 * @version 0.1
 * @date 2019-09-12
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "CClientSess.h"
#include "CMediumServer.h"
#include "asio.hpp"
#include "json11.hpp"
#include <iostream>
namespace ClientCore
{
std::shared_ptr<spdlog::logger> CClientSess::ms_loger;

CClientSess::CClientSess(asio::io_service &ioService,const std::string &strIp,
						 int port, CMediumServer *queue)
	: m_ioService(ioService), m_serverIp(strIp), m_serverPort(port),
	  m_queue(queue), m_socket(ioService), m_bConnect(ST_NOT_CONNECT)
{
	m_connectInfo = m_serverIp + ":" + std::to_string(m_serverPort);
	//StartConnect();
}

/**
 * @brief 连接socket
 *
 * @return int 返回0
 */
int CClientSess::StartConnect()
{
	//LOG_INFO(ms_loger, "Try To Connected To :{}", GetConnectInfo());
	if (ST_NOT_CONNECT == m_bConnect)
	{
		m_bConnect = ST_CONNING;
		LOG_WARN(ms_loger, "Start Connect To  IP:{} Port:{}", m_serverIp, m_serverPort);
		tcp::resolver reso(m_ioService);
		std::string strPort = std::to_string(m_serverPort);
		auto endpoint_iterator = reso.resolve({m_serverIp, strPort});
		auto self = shared_from_this();
		asio::async_connect(
			m_socket, endpoint_iterator,
			[this, self](std::error_code ec, asio::ip::tcp::endpoint endPt) {
				if (!ec)
				{
					m_bConnect = ST_CONN_FINISHED;
					{
						m_connectInfo.clear();
						m_connectInfo = m_socket.local_endpoint().address().to_v4().to_string() + ":" + std::to_string(m_socket.local_endpoint().port());
						m_connectInfo += "--->";
						m_connectInfo = m_connectInfo + m_socket.remote_endpoint().address().to_v4().to_string() + ":" + std::to_string(m_socket.remote_endpoint().port());
					}
					do_read();
					LOG_WARN(ms_loger, "Connect Success {}",GetConnectInfo());
				}
				else
				{
					m_bConnect = ST_NOT_CONNECT;
					LOG_WARN(ms_loger, "Connect Failed To {} Code:{} Msg:{}",
						 this->GetConnectInfo(), ec.value(), ec.message());
				}
			});
	}
	return 0;
}

/**
 * @brief 关闭socket
 *
 * @return int 返回0
 */
int CClientSess::StopConnect()
{
	LOG_INFO(ms_loger, "Connect Closed IP:{},port:{}", m_serverIp, m_serverPort);
	m_bConnect = ST_NOT_CONNECT;
	m_socket.close();
	return 0;
}
void CClientSess::handle_message(const std::string strMsg)
{
	LOG_INFO(ms_loger, "S: {} [{} {}]", strMsg,__FILENAME__,__LINE__);
	if (m_queue)
	{
		m_queue->SendBack(shared_from_this(), strMsg);
	}
}
/**
 * @brief
 * 处理消息的总入口，在此处根据消息类型，完成消息的分发,需要增加新消息的，在此处增加分发代码
 *
 * @param hdr TransBaseMsg_t消息体指针
 */
void CClientSess::handle_message(const TransBaseMsg_t& hdr)
{
	LOG_INFO(ms_loger, "UserName: {}  UserId: {} Recv: MsgType:{} MsgContent:{}  [{}]", UserName(), UserId(), MsgType(hdr.GetType()), hdr.to_string(), __LINE__);
	if (m_queue)
	{
		m_queue->SendBack(shared_from_this(),hdr);
	}
	//if (MessageType::KeepAliveReq_Type == hdr.GetType())
	//{
	//	KeepAliveReqMsg reqMsg;
	//	reqMsg.FromString(hdr.to_string());
	//	handleKeepAliveReq(reqMsg);
	//}
}

/**
 * @brief 从socket读取数据
 *
 * @return int 返回0
 */
int CClientSess::do_read()
{
	if (IsConnect())
	{
		auto self = shared_from_this();
		m_socket.async_read_some(
			asio::buffer(m_recvbuf + m_recvpos, max_length - m_recvpos),
			[this, self](std::error_code ec, std::size_t length) {
			auto curlen = m_recvpos + length;
			if (curlen > 2)
			{
				//此处必须为 >=，否则需要等到下一条消息到来的时候，前一条消息才能处理
				std::string strRecv(m_recvbuf, curlen);
				auto nPos = strRecv.find("\r\n");
				while (nPos != std::string::npos && nPos != 0)
				{
					std::string strRecvNew(m_recvbuf, curlen);
					std::string strCur = strRecvNew.substr(0, nPos);
					handle_message(strCur);
					memmove(m_recvbuf, m_recvbuf + nPos+2, curlen - nPos-2);
					curlen = curlen - nPos - 2;
					{
						std::string strRecvNew2(m_recvbuf, curlen);
						nPos = strRecvNew2.find("\r\n");
					}
				}
				m_recvpos = (uint32_t)curlen;
				if (!ec)
				{
					do_read();
				}
				else
				{
					StopConnect();
				}
			}
		});
	}
	else
	{
		LOG_ERR(ms_loger, "DisConnectFrom {} ",GetConnectInfo());
	}
	return 0;
}

/**
 * @brief 发送消息到对端
 *
 * @param pMsg 待发送的消息
 * @return true 发送成功
 * @return false 发送失败
 */
bool CClientSess::SendMsg(std::shared_ptr<TransBaseMsg_t> pMsg)
{
	if (IsConnect())
	{
		LOG_WARN(ms_loger, " {} Send {}", GetConnectInfo(),pMsg->to_string());
		auto self = shared_from_this();
		asio::async_write(
			m_socket, asio::buffer(pMsg->GetData(), pMsg->GetSize()),
			[this, self, pMsg](std::error_code ec, std::size_t length) mutable {
				if (ec)
				{
					LOG_WARN(ms_loger, " {} Send Failed", GetConnectInfo());
					StopConnect();
				}
				else
				{
					LOG_WARN(ms_loger, " {} Send Succeed {}", GetConnectInfo(),
						 length);
				}
			});
	}
	else
	{
		//LOG_WARN(ms_loger, "{} Not Connect", GetConnectInfo());
		StartConnect();
	}
	return true;
}

bool CClientSess::SendMsg(const std::string strMsg)
{
	if (IsConnect())
	{
		LOG_WARN(ms_loger, "C: {} Send {}", GetConnectInfo(), strMsg);
		auto self = shared_from_this();
		memcpy(m_sendbuf, strMsg.data(), strMsg.length());
		asio::async_write(
			m_socket, asio::buffer(m_sendbuf, strMsg.length()),
			[this, self](std::error_code ec, std::size_t length) mutable {
			if (ec)
			{
				LOG_WARN(ms_loger, " {} Send Failed", GetConnectInfo());
				StopConnect();
			}
			else
			{
				LOG_WARN(ms_loger, " {} Send Succeed {}", GetConnectInfo(),
					length);
			}
		});
	}
	else
	{
		//LOG_WARN(ms_loger, "{} Not Connect", GetConnectInfo());
		StartConnect();
	}
	return true;
}

/**
 * @brief 发送心跳请求消息
 * 
 * @return true 发送成功
 * @return false 
 */
bool CClientSess::SendKeepAlive()
{
	KeepAliveReqMsg reqMsg("MediumServer");
	return true;
}

/**
 * @brief 处理心跳请求消息
 * 
 * @param reqMsg 心跳请求消息
 */
void CClientSess::handleKeepAliveReq(const KeepAliveReqMsg &reqMsg)
{
	LOG_INFO(ms_loger, "KeepAliveReq: {}", reqMsg.ToString());
	KeepAliveRspMsg rspMsg(m_strUserId);
	auto pSendMsg = std::make_shared<TransBaseMsg_t>(rspMsg.GetMsgType(), rspMsg.ToString());
	SendMsg(pSendMsg);
}

/**
 * @brief 处理心跳回复消息
 * 
 * @param rspMsg 心跳回复消息
 */
void CClientSess::handleKeepAliveRsp(const KeepAliveRspMsg &rspMsg)
{
	LOG_INFO(ms_loger, "KeepAliveRsp:{}", rspMsg.ToString());
}

} // namespace MediumServer