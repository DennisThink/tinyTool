
//
// Created by 王耀华 on 16/10/11.
// Copyright(c) 2018 DennisMi.
// dennismi1024@gmail.com
// https://www.dennisthink.com/
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#ifndef _CAPNSSESS_H_
#define _CAPNSSESS_H_
#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif
#include <spdlog/spdlog.h>
#include <asio/high_resolution_timer.hpp>
#include <asio.hpp>
#include "DaemonSvcApp.h"
#include "CSourceServer.h"
#include "CommonMsg.h"
#include "CSessManager.h"


using asio::ip::tcp;

namespace SourceServer
{
	/**
	 * @brief 连接到远程服务器的Sess，
	 * 
	 */
	class CAPNsSess : public std::enable_shared_from_this<CAPNsSess>  {
protected:
	enum STATE
	{
		ST_CONN_FINISHED=0,
		ST_CONNING,
		ST_NOT_CONNECT
	};

	//IoService的引用
    asio::io_service& m_ioService;

	//持有此连接的消息队列
    CSessManager* m_queue;
	
	//服务器的IP地址
	std::string m_serverIp;
	
	//服务器的端口
	int 		m_serverPort;

	//tcp连接的socket
	asio::ip::tcp::socket   m_socket;

	//Sess的连接状态
	int m_bConnect=ST_NOT_CONNECT;

	//bool m_bReciveKeepAlive = true;
	//连接信息,需要多次获取,因此保存到变量中
	std::string m_connectInfo;

	enum { max_length = 16384, msg_max_len=8192 };


	//接收buf
    char m_recvbuf[max_length];
	
	//接收位置
    uint32_t m_recvpos=0;

public:
	static	std::shared_ptr<spdlog::logger> ms_loger;
public:
	//检查是否为同一个连接
	bool is_same_connect(const std::string& strIp,const int nport)
	{
		return (( m_serverIp.compare(strIp)==0) && 
			(nport == m_serverPort ));
	}
	//获取连接的ip地址和端口号,格式为 xx.xx.xx.xx:port
	std::string GetConnectInfo(){return m_connectInfo;}

	//发送KeepAlive消息
	/**
	 * @brief 发送KeepAlive消息
	 * 
	 * @return true 发送成功
	 * @return false 发送失败
	 */
	bool SendKeepAlive();
	
	

	/**
	 * @brief 收到回复信息时,通知Queue
	 * 
	 * @param msg 待处理的消息
	 */
	void SendBack(const TransBaseMsg_t* msg);

	/**
	 * @brief 判断连接状态
	 * 
	 * @return true 已连接
	 * @return false 已断开
	 */
	bool IsConnect();

	/**
	 * @brief Construct a new CAPNsSess object
	 * 
	 * @param ioService asio::io_service实例
	 * @param strIp 远程服务器的IP
	 * @param port  远程服务器的端口
	 * @param queue 保存该链接的Queue
	 */
    CAPNsSess(asio::io_service& ioService,std::string& strIp,int port,CSessManager* queue);

	/**
	 * @brief 开始连接
	 * 
	 */
    void StartConnect();

	/**
	 * @brief 断开与服务器的连接
	 * 
	 */
    void StopConnect();
private:

	/**
	 * @brief 读取数据，连接建立以后开始读取
	 * 
	 * @return int 
	 */
	int do_read();
	
	/**
	 * @brief 处理消息的总入口函数，由do_read在判断消息完整的时候调用
	 * 
	 * @param msg 待处理的消息
	 */
	void handle_message(const TransBaseMsg_t* msg);

	/**
	 * @brief 发送消息到远端服务器
	 * 
	 * @param msg 待发送的消息
	 */
	void send_msg(std::shared_ptr<TransBaseMsg_t> msg);
	
	/**
	 * @brief 处理KeepAliveReq消息,由handle_message调用
	 * 
	 * @param reqMsg KeepAliveReq消息
	 */
	void handleKeepAliveReq(const KeepAliveReqMsg& reqMsg);
	
	/**
	 * @brief 处理KeepAliveRsp消息，由handle_message调用
	 * 
	 * @param rspMsg KeepAliveRsp消息
	 */
	void handleKeepAliveRsp(const KeepAliveRspMsg& rspMsg);
};
}

#endif //CAPNSSESS
