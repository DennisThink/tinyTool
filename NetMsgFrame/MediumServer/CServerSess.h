#ifndef _MEDIUM_SERVER_C_SERVER_SESS_H_
#define _MEDIUM_SERVER_C_SERVER_SESS_H_
#include "json11.hpp"
#include "CommonMsg.h"
#include "asio_common.h"
#include "Log.h"

using tcp=asio::ip::tcp;
namespace MediumServer
{
class CMediumServer;
class CServerSess : public std::enable_shared_from_this<CServerSess>
{
public:
	static std::shared_ptr<spdlog::logger> ms_loger;
private:
    //套接字
    tcp::socket m_socket;
    
    //需要告知
    CMediumServer* m_server;

    //是否已连接
	bool m_bConnect = false;


    enum { max_length = 16384, msg_max_len=8192 };

    //接收消息的buf    
    char m_recvbuf[max_length];
    
    //已经接收的消息的长度
    uint32_t m_recvpos=0;
public:
    /**
     * @brief 发送消息函数，所有的消息需要转为TransBaseMsg_t的类型来进行发送。
     * 
     * @param msg 待发送的消息
     */
    void SendMessage(std::shared_ptr<TransBaseMsg_t> msg){
		INFO(ms_loger," send message ");
        auto self=shared_from_this();
        asio::async_write(m_socket, asio::buffer(msg->GetData(),msg->GetSize()), [this, self, msg](std::error_code ec, std::size_t length) {
            if(!ec)
            {
                INFO(ms_loger,"Succeed DataLen:{}",length);
            }
            else
            {
				INFO(ms_loger," send message error");
				m_socket.close();
				m_bConnect = false;
            }
        });
    }

    CServerSess(tcp::socket socket, CMediumServer* server) : m_socket(std::move(socket)),m_server(server),m_bConnect(false) { }
    
    virtual ~CServerSess(){
    }
    
    void Start() {
		INFO(ms_loger,"Start Receive Socket ");
        m_recvpos=0;
        do_read();
    }
	
	bool IsConnected()
	{
		return m_bConnect;
	}
private:
    
    /**
     * @brief 接收数据，在Start中被调用，通过asio的回调函数，实现连续接收
     *        非阻塞，不需要修改
     * 
     */
	void do_read();


    /**
     * @brief 处理消息的函数，由do_read在判断消息完整以后调用，在此函数中完成消息的分发处理，
     *        代码的示例中给出了KeepAliveReqMsg和KeepAliveRspMsg的处理示例
     * 
     * @param msg 接收到的一条消息
     */
    void handle_message(const TransBaseMsg_t* msg);


    /**
     * @brief 处理KeepAliveRsp消息，由handle_message 调用
     * 
     * @param rspMsg KeepAliveRsp消息
     */
    void handleKeepAliveRsp(const KeepAliveRspMsg& rspMsg);
    
    /**
     * @brief 处理KeepAliveReq消息
     * 
     * @param rspMsg KeepAliveReq消息
     */
    void handleKeepAliveReq(const KeepAliveReqMsg& reqMsg);
};
}
#endif
