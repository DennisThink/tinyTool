﻿/**
 * @file CMediumServer.h
 * @author DennisMi (https://www.dennisthink.com/)
 * @brief 总控类
 * @version 0.1
 * @date 2019-08-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _MEDIUM_SERVER_C_MEDIUM_SERVER_H_
#define _MEDIUM_SERVER_C_MEDIUM_SERVER_H_

#include <functional>

#include <map>

#include "CommonMsg.h"
#include "Log.h"
#include "asio_common.h"
#include "json11.hpp"

#include "CClientSess.h"
#include "CServerSess.h"
#include "CHttpServer.h"
#include "planform.h"
#include "CMsgPersistentUtil.h"
#include "CFileUtil.h"
#include "SMTP_Handler.h"
#include "IMAP_Handler.h"
namespace ClientCore
{
using tcp = asio::ip::tcp;
class CClientSess;
class CMediumServer : public std::enable_shared_from_this<CMediumServer>
{
  protected:
    asio::io_service &m_ioService;

    tcp::socket m_socket;

    tcp::acceptor m_acceptor;

    //服务器的IP端口设置
    IpPortCfg m_serverCfg;

	CFileUtil m_fileUtil;
    //所有的客户端的连接客户端的IP端口设置
    std::vector<IpPortCfg> m_clientCfgVec;

	std::vector<IpPortCfg> m_clientBinCfgVec;

    std::vector<std::shared_ptr<CServerSess>> m_listenList; //监听的套接字的列表

	std::map<std::shared_ptr<CServerSess>, std::shared_ptr<CClientSess>> m_ForwardSessMap;
	std::map<std::shared_ptr<CClientSess>, std::shared_ptr<CServerSess>> m_BackSessMap;

	std::map<std::string, std::shared_ptr<CClientSess>> m_userClientSessMap;

    std::shared_ptr<asio::high_resolution_timer> m_timer = nullptr;

	std::shared_ptr<CHttpServer> m_httpServer;
	CClientSess_SHARED_PTR m_freeClientSess;
	CMsgPersistentUtil_SHARED_PTR m_msgPersisUtil;
    void SetTimer(int nSeconds);
    void OnTimer();

    std::error_code m_ec;

    void do_accept();
	void OnHttpRsp(std::shared_ptr<TransBaseMsg_t> pMsg);
	C_SMTP_Handler_PTR GetSmtpHandler(const std::shared_ptr<CClientSess>& pClientSess);
	C_IMAP_Handler_PTR GetImapHandler(const std::shared_ptr<CClientSess>& pClientSess);

  public:
    static std::shared_ptr<spdlog::logger> ms_loger;
    inline IpPortCfg &config() { return m_serverCfg; }

	void HandleSendEmailReq(const SendEmailReq& reqMsg);
	void HandleUserLoginReq(const UserLoginReqMsg& reqMsg);
	void HandleGetTaskReq(const QueryTaskReq& reqMsg);
	void HandleGetUserEmail(const GetEmailReq& reqMsg);
    void SendBack(const std::shared_ptr<CClientSess>& pClientSess,const TransBaseMsg_t& msg);
	void SendBack(const std::shared_ptr<CClientSess>& pClientSess, const std::string msg);
	void SendFoward(const std::shared_ptr<CServerSess>& pServerSess,const TransBaseMsg_t& msg);

    void CheckAllConnect();

	CClientSess_SHARED_PTR GetClientSess(const std::string strUserName);
	CClientSess_SHARED_PTR GetClientSess(const IpPortCfg& smtpCfg);
	CMsgPersistentUtil_SHARED_PTR GetMsgPersisUtil() {
		return m_msgPersisUtil;
	}
    CMediumServer(asio::io_service &io_service)
        : m_ioService(io_service), m_socket(io_service), m_acceptor(io_service)
    {
        if (!m_timer)
        {
            m_timer = std::make_shared<asio::high_resolution_timer>(m_ioService);
        }
		m_httpServer = std::make_shared<CHttpServer>(m_ioService,this);
		m_msgPersisUtil = std::make_shared<CMsgPersistentUtil>();
		m_msgPersisUtil->InitDataBase();
		m_smtpHandler = std::make_shared<C_SMTP_Handler>();
    }

	void start(const std::function<void(const std::error_code &)> &callback);

    void loadConfig(const json11::Json &cfg, std::error_code &ec);
    //获取server的ip和端口,
    std::string getServerIpPort();
private:
	std::map<std::string, CLIENT_SESS_STATE>  m_userStateMap;
	std::map<std::string, UserLoginReqMsg> m_userLoginMsgMap;
	C_SMTP_Handler_PTR m_smtpHandler;
	std::map<CClientSess_SHARED_PTR, C_SMTP_Handler_PTR> m_clientSessHandlerMap;
	std::map<CClientSess_SHARED_PTR, C_IMAP_Handler_PTR> m_clientSessImapHandlerMap;

	std::map<std::string, C_SMTP_Handler_PTR> m_TaskIdHandlerMap;

};
} // namespace MediumServer

#endif // HTTP2T1_CAGENTSERVER_H
