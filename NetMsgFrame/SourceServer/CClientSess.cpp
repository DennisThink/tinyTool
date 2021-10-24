
//
// Created by 王耀华 on 16/10/11.
// Copyright(c) 2018 DennisMi.
// dennismi1024@gmail.com
// https://www.dennisthink.com/
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#include "CClientSess.h"
#include "json11.hpp"
#include "CSessManager.h"
#include <iostream>
namespace SourceServer
{
std::shared_ptr<spdlog::logger> CClientSess::ms_loger;

CClientSess::CClientSess(asio::io_service& ioService,std::string& strIp,int port, CSessManager* manager):
    m_ioService(ioService),
	m_manager(manager),
	m_serverIp(strIp),
	m_serverPort(port),
	m_socket(ioService),
	m_ConnectState(STATE_E::ST_NOT_CONNECT)
{
		m_connectInfo = m_serverIp+":"+std::to_string(m_serverPort);
		//m_bReciveKeepAlive =true;
		StartConnect();
}



//启动连接
void CClientSess::StartConnect()
{
	DBG(ms_loger,"Try To Connected To :{}",GetConnectInfo());
	if(!IsConnect())
	{
		m_ConnectState = STATE_E::ST_CONNING;
		WARN(ms_loger,"Start Connect To  IP:{} Port:{}",m_serverIp,m_serverPort);
		tcp::resolver reso(m_ioService);
		std::string strPort = std::to_string(m_serverPort);
		auto endpoint_iterator = reso.resolve({m_serverIp,strPort});
		asio::async_connect(m_socket,endpoint_iterator,[this](std::error_code ec,tcp::endpoint endPt)
				{
					if(!ec)
					{
						m_ConnectState = STATE_E::ST_CONN_FINISHED;	
						do_read();
						INFO(ms_loger,"Connect Success IP:{} Port:{}",this->m_serverIp,this->m_serverPort);
						INFO(ms_loger,"ConnectID :{}:{}",endPt.address().to_string(),endPt.port());
					}
					else
					{
						m_ConnectState = STATE_E::ST_NOT_CONNECT;
						WARN(ms_loger,"Connect Failed To {} Code:{} Msg:{}",this->GetConnectInfo(),ec.value(),ec.message());
					}
			});
	}
	else
	{
		INFO(ms_loger,"Connect Has Established IP:{} Port:{}",this->m_serverIp,this->m_serverPort);
	}
}

void CClientSess::StopConnect()
{
	INFO(ms_loger,"Connect Closed IP:{},port:{}",m_serverIp,m_serverPort);
	m_ConnectState = STATE_E::ST_NOT_CONNECT;
	m_socket.close();
}



bool CClientSess::IsConnect()
{
	return (STATE_E::ST_CONN_FINISHED == m_ConnectState);
}

void CClientSess::SendBack(const TransBaseMsg_t* msg)
{
	std::string strValue = msg->to_string();
	if(m_manager)
	{
		m_manager->OnRecvMessage(msg);
	}
}

int CClientSess::do_read()
{
	if(IsConnect())
	{
		INFO(ms_loger,"Client  read data ");
		auto self=shared_from_this();
		m_socket.async_read_some(
				asio::buffer(m_recvbuf+m_recvpos,max_length-m_recvpos),[this,self](std::error_code ec, std::size_t length)
				{
					if(ec)
					{
						m_socket.close();
						m_ConnectState = STATE_E::ST_NOT_CONNECT;
					}
					else
				    {
						TransBaseMsg_t msg(m_recvbuf);
						auto curlen=m_recvpos+length;
						INFO(ms_loger,"Client async_read_some:size:{:#x}  ec:{}",length,ec.message());
						while(curlen>=sizeof(Header) && curlen>=msg.GetSize())
						{
							INFO(ms_loger," read data {} ",curlen);
							SendBack(&msg);
							curlen-=msg.GetSize();
							memmove(m_recvbuf,m_recvbuf+msg.GetSize(),curlen);
						}
						m_recvpos=(uint32_t)curlen;
						if(m_recvpos<max_length && !ec)
						{	
							do_read();
						}
					}
				}
				);
	}
	return 0;
}

bool CClientSess::SendKeepAlive()
{
	DBG(ms_loger,"Send Keep Alive");
	if(IsConnect())
	{
		KeepAliveReqMsg msg("SourceServer");
		auto pMsg = std::make_shared<TransBaseMsg_t>(msg.GetMsgType(),msg.ToString());
		auto self = shared_from_this();
		asio::async_write(m_socket,
		asio::buffer(pMsg->GetData(),pMsg->GetSize()),
		[this,self,pMsg](std::error_code ec, std::size_t length) mutable
		{
			if(ec)
			{
				WARN(ms_loger," {} Send KeepAlive Failed",GetConnectInfo());
				StopConnect();
			}          
			else
			{
				INFO(ms_loger," {} Send KeepAlive Succeed Length:{}",GetConnectInfo(),length); 
			}
		});
		return true;
	}
	else
	{
		WARN(ms_loger,"{} Not Connect",GetConnectInfo());
		StartConnect();
		return false;
	}
}

void CClientSess::send_msg(std::shared_ptr<TransBaseMsg_t> msg){
		INFO(ms_loger," send message ");
        auto self=shared_from_this();
        asio::async_write(m_socket, asio::buffer(msg->GetData(),msg->GetSize()), [this, self, msg](std::error_code ec, std::size_t length) {
            if (ec){
				INFO(ms_loger," send message error");
                //TODO:log
				m_socket.close();
				m_ConnectState = STATE_E::ST_NOT_CONNECT;
            }
            else {
                INFO(ms_loger,"Send Succeed {}",length);
            }
        });
    }



void CClientSess::handleKeepAliveReq(const KeepAliveReqMsg& reqMsg)
{
	INFO(ms_loger,"Recv KeepAliveReq:{}",reqMsg.ToString());
	KeepAliveRspMsg msg("SourceServer");
	auto pMsg = std::make_shared<TransBaseMsg_t>(msg.GetMsgType(),msg.ToString());
	send_msg(pMsg);
}

void CClientSess::handleKeepAliveRsp(const KeepAliveRspMsg& rspMsg)
{
	INFO(ms_loger,"Recv KeepAliveRsp:{}",rspMsg.ToString());
}
}