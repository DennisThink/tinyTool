#ifndef _TOKEN_NUMBER_H_
#define _TOKEN_NUMBER_H_
#include "token.h"
namespace calculater {	
class TokenNumber:public CToken
{
public:
    explicit TokenNumber(const std::string& str);
	explicit TokenNumber(const double value );
    double   getValue() const;
	virtual  TokenType getTokenType() override final;
	virtual  std::string toString() override final;
private:
    double   m_value;
	std::string m_orgStr;
};
}
#endif