#ifndef _SEND_EMAIL_H_
#define _SEND_EMAIL_H_
#include <string>
bool SendMail(const std::string& strRecv,const std::string& strSub,const std::string& strContext);
#endif