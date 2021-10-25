#ifndef _TOKEN_OPERATOR_H_
#define _TOKEN_OPERATOR_H_
#include "token.h"
#include "token_number.h"
namespace calculater {	
class TokenOperator:public CToken
{
public:
	explicit TokenOperator();
	virtual ~TokenOperator();
	TokenType getTokenType() override final;
	virtual std::string toString() override;
	virtual OperatorType getOperatorType()=0;
	virtual OperatorPriority getOperatorPriority()=0;
	virtual TokenNumber compute(const TokenNumber& lhs,const TokenNumber& rhs)=0;
};
}
#endif
