#ifndef _DENNIS_THINK_SMTP_CMD_H_
#define _DENNIS_THINK_SMTP_CMD_H_
#include <string>
enum class SMTP_CMD_TYPE
{
	UnKnown,
	S_C_220_RSP,//Server 220�ظ�
	C_S_HELO_REQ,
	C_S_EHLO_REQ,
	C_S_AUTH_LOGIN_REQ,
	S_C_USER_NAME_REQ,
	S_C_PASS_WORD_REQ,
	S_C_SERVER_QUIT_RSP,
	C_S_RECV_TO_REQ,

};

//SMTP����������Ϣ�ķ�����
enum class SMTP_CODE
{
	CODE_UN_KNOWN,
	CODE_220,
	CODE_250,
	CODE_334,
	CODE_235,
	CODE_354,
	CODE_221,
};

//SMTP�Ĳ���
enum class SMTP_STEP
{
	ON_CONNECT_STEP,
	SEND_HELO_STEP,
	ON_SERVER_250_STEP,
	SEND_AUTH_LOGIN_STEP,
	ON_SERVER_USER_NAME_STEP,
	SEND_USER_NAME_STEP,
	ON_SERVER_PASS_WORD_STEP,
	SEND_USER_PASS_WORD_STEP,
	ON_SERVER_AUTH_SUCCEED_STEP,
	SEND_MAIL_FROM_STEP,
	ON_SERVER_MAIL_FROM_STEP,
	SEND_RCPT_TO_SELF_STEP,
	ON_SERVER_RCPT_TO_SELF_STEP,
	SEND_RCPT_TO_STEP,
	ON_SERVER_RCPT_TO_STEP,
	SEND_DATA_BEGIN_STEP,
	ON_SERVER_SEND_DATA_BEGIN_RSP,
	SEND_DATA_STEP,
	ON_SERVER_SEND_DATA_STEP,
	SEND_QUIT_STEP,
	ON_SERVER_QUIT_STEP,

};

SMTP_CODE GetServerRspCode(const std::string strRsp);
class C_SMTP_CMD_BASE
{
public:
	C_SMTP_CMD_BASE();
	virtual ~C_SMTP_CMD_BASE();
	virtual SMTP_CMD_TYPE GetCmdType();
	virtual std::string ToString();
	virtual bool FromString(const std::string strSmtp);
protected:
	SMTP_CMD_TYPE m_type;
};

class C_SMTP_Server_On_Connect_Rsp final:public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Server_On_Connect_Rsp();
	virtual ~C_SMTP_Server_On_Connect_Rsp();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
};

class C_SMTP_Client_HELO_CmdReq final:public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Client_HELO_CmdReq();
	virtual ~C_SMTP_Client_HELO_CmdReq();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
};

class C_SMTP_Client_EHLO_CmdReq final :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Client_EHLO_CmdReq();
	virtual ~C_SMTP_Client_EHLO_CmdReq();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
};
class C_SMTP_Server_250_Rsp :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Server_250_Rsp();
	virtual ~C_SMTP_Server_250_Rsp();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
	bool IsServerRspFinished() const ;
public:
	bool m_bRspFinished;
};

class C_SMTP_Client_AuthLoginReq :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Client_AuthLoginReq();
	virtual ~C_SMTP_Client_AuthLoginReq();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
};

class C_SMTP_Server_UserNameReq:public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Server_UserNameReq();
	virtual ~C_SMTP_Server_UserNameReq();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
};

class C_SMTP_Server_PassWordReq :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Server_PassWordReq();
	virtual ~C_SMTP_Server_PassWordReq();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
};

class C_SMTP_Client_PassWordRsp :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Client_PassWordRsp(const std::string strPassWord);
	virtual ~C_SMTP_Client_PassWordRsp();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
public:
	std::string m_strPassWord;
};

class C_SMTP_Client_UserNameRsp :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Client_UserNameRsp(const std::string strUserName);
	virtual ~C_SMTP_Client_UserNameRsp();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
protected:
	std::string m_strUserName;
};

class C_SMTP_Server_AuthSuccessRsp :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Server_AuthSuccessRsp();
	virtual ~C_SMTP_Server_AuthSuccessRsp();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
};

class C_SMTP_Client_MailFromReq :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Client_MailFromReq(const std::string strUserEmail);
	virtual ~C_SMTP_Client_MailFromReq();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
protected:
	std::string m_strUserEmail;
};

class C_SMTP_Client_RecvToReq :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Client_RecvToReq(const std::string strRecvEmail);
	virtual ~C_SMTP_Client_RecvToReq();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
protected:
	std::string m_strRecvEmail;
};

class C_SMTP_Client_DataBeginReq :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Client_DataBeginReq();
	virtual ~C_SMTP_Client_DataBeginReq();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
};

class C_SMTP_Server_DataBeginRsp :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Server_DataBeginRsp();
	virtual ~C_SMTP_Server_DataBeginRsp();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
};

class C_SMTP_Client_DataBodyReq :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Client_DataBodyReq();
	virtual ~C_SMTP_Client_DataBodyReq();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
public:
	std::string m_strSubject;
	std::string m_strFrom;
	std::string m_strTo;
	std::string m_strContext;
};

class C_SMTP_Client_DataEndReq :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Client_DataEndReq();
	virtual ~C_SMTP_Client_DataEndReq();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
};

class C_SMTP_Client_QuitReq:public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Client_QuitReq();
	virtual ~C_SMTP_Client_QuitReq();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
};

class C_SMTP_Server_QuitRsp :public C_SMTP_CMD_BASE
{
public:
	C_SMTP_Server_QuitRsp();
	virtual ~C_SMTP_Server_QuitRsp();
	virtual SMTP_CMD_TYPE GetCmdType() override;
	virtual std::string ToString() override;
	virtual bool FromString(const std::string strSmtp) override;
};

#endif