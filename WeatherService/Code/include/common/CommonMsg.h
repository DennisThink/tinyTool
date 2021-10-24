#ifndef _COMMON_MSG_H_
#define _COMMON_MSG_H_
#include <map>
#include <set>
#include <list>
#include <queue>
#include <functional>
#include <atomic>
#include "json11.hpp"
#include <stdlib.h>
#include <signal.h>

//绑定端口失败的退出信号值
const int BIND_FAILED_EXIT = SIGUSR2*2;


//消息的枚举类型
enum class MessageType:uint32_t
{
	Base_Type,
	KeepAliveReq_Type,
	KeepAliveRsp_Type,
};

//消息头
struct Header
{
	MessageType m_type;
	uint32_t    m_length;
};

//传输消息的结构体
struct TransBaseMsg_t
{
public:
	MessageType GetType() const 
	{
		Header* head = (Header*)(m_data);
		return head->m_type;
	}

	std::size_t GetSize() const
	{
		Header* head = (Header*)(m_data);
		return head->m_length;
	}


	std::string to_string() const
	{ 
		return std::string((const char*)(m_data+sizeof(Header)),GetSize()-sizeof(Header));
	}

	const char * GetData() const 
	{
		return m_data;
	}
	
	explicit TransBaseMsg_t(const MessageType& type,const std::string& strMsg)
	{
		int strLen = strMsg.length();
		Header head;
		head.m_type = type;
		head.m_length = strLen+sizeof(head);
		m_data = new char[head.m_length];
		memset(m_data,0,head.m_length);
		memcpy(m_data,&head,sizeof(head));
		memcpy(m_data+sizeof(head),strMsg.c_str(),strLen);
		m_selfData = true;
	}
	
	explicit TransBaseMsg_t(char * data)
	{
		m_data = data;
		m_selfData = false;
	}
	virtual ~TransBaseMsg_t()
	{
		if(m_selfData)
		{
			if(nullptr != m_data)
			{
				delete m_data;
			}
		}
	}
protected:
	char *   m_data;
	bool m_selfData = false;
};

struct HttpTrans_t
{
public:
	std::size_t GetSize() const
	{
		return m_dataLen;
	}

	const char * GetData() const 
	{
		return m_data;
	}

	std::string to_string() const
	{ 
		return std::string(m_data,m_dataLen);
	}
	explicit HttpTrans_t(const std::string& strMsg)
	{
		m_dataLen = strMsg.length();
		m_data = new char[m_dataLen];
		memcpy(m_data,strMsg.c_str(),m_dataLen);
		m_selfData = true;
	}
	virtual ~HttpTrans_t()
	{
		if(m_selfData)
		{
			if(nullptr != m_data)
			{
				delete[] m_data;
			}
		}
	}
protected:
	char *   m_data;
	std::size_t m_dataLen;
	bool m_selfData = false;
};

struct IpPortCfg
{
	std::string m_strServerIp;
	int 		m_nPort;
	std::string to_string()
	{
		return m_strServerIp+":"+std::to_string(m_nPort);
	}

	bool Valid() const 
	{
		if(m_strServerIp.empty())
		{
			return false;
		}		
		if(m_nPort<1024)
		{
			return false;
		}
		return true;
	}
};


struct BaseMsg
{
protected:
	MessageType m_type;
public:
	MessageType GetMsgType()
	{
		return m_type;
	}

	explicit BaseMsg()
	{
		m_type=MessageType::Base_Type;
	}
	
	virtual std::string ToString() const
	{
		return "";
	}

	virtual bool FromString(const std::string& strJson)
	{
		if(strJson.empty())
		{
			return false;
		}
		return true;
	}
};
struct KeepAliveReqMsg final:public BaseMsg
{
	std::string m_strClientId;
public:
	explicit KeepAliveReqMsg()
	{
		m_type = MessageType::KeepAliveReq_Type;
		m_strClientId= "";
	}
	explicit KeepAliveReqMsg(const std::string& clientId)
	{
		m_type = MessageType::KeepAliveReq_Type;
		m_strClientId=clientId;
	}
	virtual std::string ToString() const override
	{
		using namespace json11;
		Json clientObj = Json::object({
				{"ClientID",m_strClientId}
			});
		return clientObj.dump();
	}

	virtual bool FromString(const std::string& strJson) override
	{
		std::string err;
		using namespace json11;
		auto json = Json::parse(strJson, err);
		if(!err.empty())
		{
			return false;
		}
		
		if(json["ClientID"].is_string())
		{
			m_strClientId = json["ClientID"].string_value();
		}
		else 
		{
			return false;
		}
		if(m_strClientId.empty())
		{
			return false;
		}
		return true;
	}
};

class KeepAliveRspMsg final:public BaseMsg
{
private:
	std::string m_strClientId;
public:
	explicit KeepAliveRspMsg()
	{
		m_type = MessageType::KeepAliveRsp_Type;
		m_strClientId="";
	}
	explicit KeepAliveRspMsg(const std::string& clientId)
	{
		m_type = MessageType::KeepAliveRsp_Type;
		m_strClientId=clientId;
	}
	virtual std::string ToString() const override
	{
		using namespace json11;
		Json clientObj = Json::object({
				{"ClientID",m_strClientId}
			});
		return clientObj.dump();
	}

	virtual bool FromString(const std::string& strJson) override
	{
		std::string err;
		using namespace json11;
		auto json = Json::parse(strJson, err);
		if(!err.empty())
		{
			return false;
		}

		if(json["ClientId"].is_string())
		{
			m_strClientId = json["ClientId"].string_value();
		}
		else 
		{
			return false;
		}
		if(m_strClientId.empty())
		{
			return false;
		}
		return true;
	}
};

class CServerConfig
{
public:
	std::vector<IpPortCfg> m_clientCfgVec;
	IpPortCfg              m_serverCfg;
	explicit CServerConfig()
	{
		m_serverCfg.m_strServerIp = "127.0.0.1";
		m_serverCfg.m_nPort = 9000;

		m_clientCfgVec.push_back({"127.0.0.1",8000});
	}
};
#endif
