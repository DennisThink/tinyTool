#ifndef _SUB_OPERATOR_H_
#define _SUB_OPERATOR_H_
#include "token_operator.h"
namespace calculater {	
class SubOperator:public TokenOperator
{
public:
	explicit SubOperator(){}
	virtual ~SubOperator(){};
	virtual OperatorType getOperatorType() override final{ return OperatorType::SUB_OPERATOR;}
	TokenNumber compute(const TokenNumber& lhs,const TokenNumber& rhs) override final;
	virtual OperatorPriority getOperatorPriority() override final { return OperatorPriority::ADD_LEVEL; }
	virtual std::string toString() override final;
};
}
#endif