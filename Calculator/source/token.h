
#ifndef _H_CALCULATER_TOKEN_H_
#define _H_CALCULATER_TOKEN_H_
#include "calc_type_define.h"
#include <string>
namespace calculater {	
class CToken
{
public:
	CToken(){};
    virtual TokenType getTokenType()=0;
	virtual std::string toString()=0;
	virtual ~CToken(){};
};
}
#endif
