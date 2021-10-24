#include "SendEmail.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#ifdef WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib,"Ws2_32.lib ")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#endif
#include <regex>
//#include "ProtoUtil.h"
#define MAXLINE 4096
#include <string>

//SEND
const std::string strEHLO="EHLO smtp.cppemail.com\r\n";
const std::string strAUTH_LOGIN="AUTH LOGIN\r\n";
const std::string strSendUserName="c2VuZGVyQGNwcGVtYWlsLmNvbQ==\r\n";
const std::string strSendPassword="c2VuZGVy\r\n";

const std::string strSendData="DATA\r\n";
const std::string strSendEmailEnd=".\r\n";
const std::string strSendQuit="QUIT\r\n";

#if 0
#define DEBUG_MSG std::cout<<__LINE__<<"  "<<strMsg<<std::endl;
#define DEBUG_LINE std::cout<<__LINE__<<std::endl;
#else
#define DEBUG_MSG 
#define DEBUG_LINE 
#endif

int Open(std::string strIp,int port)
{
#ifdef WIN32
//�����׽��ֿ�
WSADATA wsaData;
int iRet = 0;
iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
if (iRet != 0)
{
	return -1;
}
if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
{
	WSACleanup();
	return -1;
}
#endif

    int sockfd;
    struct sockaddr_in servaddr;
    if ((sockfd = static_cast<int>(socket(AF_INET, SOCK_STREAM, 0))) < 0)
    {
        return -1;
    }
   
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

#ifdef WIN32
	servaddr.sin_addr.S_un.S_addr = inet_addr(strIp.c_str());
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
#else

	if (inet_pton(AF_INET, strIp.c_str(), &servaddr.sin_addr) <= 0)
	{
		return -1;
	}
#endif

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        return -1;
    }
    return sockfd;
}

bool Close(int socketFd)
{
#ifdef WIN32
	closesocket(socketFd);
#else
    close(socketFd);
#endif
	return true;
}

bool Send(int sockFd,std::string strMsg)
{
#ifdef WIN32
	std::size_t sendLen = send(sockFd, strMsg.data(), static_cast<int>(strMsg.length()), 0);
#else
    std::size_t sendLen = send(sockFd, strMsg.data(), strMsg.length(), 0);
#endif
	if(sendLen != strMsg.length())
    {
        return false;
    }
    return true;
}

bool Recv(int sockFd,std::string& strMsg)
{
    const int BUF_LEN = 512;
    char buf[BUF_LEN] = {0};
    std::size_t nRecvLen = 0;
    do{
        memset(buf,0,BUF_LEN);
        nRecvLen =  recv(sockFd,buf,BUF_LEN,0);
        strMsg = strMsg+std::string(buf,nRecvLen);
        if(nRecvLen < BUF_LEN-1)
        {
            break;
        }
    }while(nRecvLen>0);
    //std::cout<<strMsg<<std::endl;
    return true;
}

bool SendMail(const std::string& strRecv,const std::string& strSub,const std::string& strContext)
{
    int sockFd = Open("14.17.57.241",25);
    if(sockFd < 0)
    {
        DEBUG_LINE
        return false;
    }
   
    std::string strMsg;

    if(!Recv(sockFd,strMsg))
    {
        DEBUG_MSG
        return false;
    }
    DEBUG_MSG
    
    if(!Send(sockFd,strEHLO))
    {
        DEBUG_MSG
        return false;
    }

    strMsg.clear();
    if(!Recv(sockFd,strMsg))
    {
        DEBUG_MSG
        return false;
    }
    DEBUG_MSG

    
    if(!Send(sockFd,strAUTH_LOGIN))
    {
        DEBUG_MSG
        return false;
    }
    strMsg.clear();
    if(!Recv(sockFd,strMsg))
    {
        DEBUG_MSG
        return false;
    }
    DEBUG_MSG
    //CHECK_EQ(strMsg,strRecvName);

    if( !Send(sockFd,"MjQ3NTAyNzY4MkBxcS5jb20=\r\n"))
    {
        DEBUG_MSG
        return false;
    }

    strMsg.clear();
    if(!Recv(sockFd,strMsg))
    {
        DEBUG_MSG
        return false;
    }
    DEBUG_MSG
    //CHECK_EQ(strMsg,strRecvPassword);
    if(!Send(sockFd,"dWp3b2ZvY2VvZGppZWFlZA==\r\n"))
    {
        DEBUG_MSG
        return false;
    }

    strMsg.clear();
    if(!Recv(sockFd,strMsg))
    {
        DEBUG_MSG
        return false;
    }
    DEBUG_MSG
    if( strMsg != "235 Authentication successful\r\n")
    {
        DEBUG_MSG
        return false;
    }




    if(!Send(sockFd,"MAIL FROM: <2475027682@qq.com>\r\n"))
    {
        DEBUG_MSG
        return false;
    }
    strMsg.clear();
    if(!Recv(sockFd,strMsg))
    {
        DEBUG_MSG
        return false;
    }
    DEBUG_MSG
    //CHECK_EQ(strMsg,"250 Mail OK\r\n");

    std::string strRecvEmail = "RCPT TO:<"+strRecv+">\r\n";
    if(!Send(sockFd,strRecvEmail))
    {
        DEBUG_MSG
        return false;
    }
    strMsg.clear();
    if(!Recv(sockFd,strMsg))
    {
        DEBUG_MSG
        return false;
    }
    DEBUG_MSG

   
    //CHECK_EQ(strMsg,"250 Mail OK\r\n");

    if(!Send(sockFd,strSendData))
    {
        DEBUG_MSG
        return false;
    }
  
    if(!Send(sockFd,"From:DennisThink <2475027682@qq.com>\r\n"))
    {
        DEBUG_MSG
        return false;
    }
    strMsg.clear();
    if(!Recv(sockFd,strMsg))
    {
        DEBUG_MSG
        return false;
    }
    DEBUG_MSG
    std::string strRecvEmail2 = "Subject:"+strSub+"\r\n";
    if(!Send(sockFd,strRecvEmail2))
    {
        DEBUG_MSG
        return false;
    }

    if(!Send(sockFd,"\r\n"+strContext+"\r\n"))
    {
        DEBUG_MSG
        return false;
    }
    
    if(!Send(sockFd,strSendEmailEnd))
    {
        DEBUG_MSG
        return false;
    }
    strMsg.clear();
    if(!Recv(sockFd,strMsg))
    {
        DEBUG_MSG
        return false;
    }
    DEBUG_MSG

    if(!Send(sockFd,strSendQuit))
    {
        DEBUG_MSG
        return false;
    }
    strMsg.clear();
    if(!Recv(sockFd,strMsg))
    {
        DEBUG_MSG
        return false;
    }
    DEBUG_MSG
    //CHECK_EQ(strMsg,strRecvQuit);
    DEBUG_MSG
    return true;
}