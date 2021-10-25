
#ifndef _DIV_OPERATOR_H_
#define _DIV_OPERATOR_H_
#include "token_operator.h"
namespace calculater {

class DivOperator:public TokenOperator
{
public:
	explicit DivOperator(){};
	virtual ~DivOperator(){}
	virtual OperatorType getOperatorType() override final { return OperatorType::DIV_OPERATOR;}
	virtual OperatorPriority getOperatorPriority() override final { return OperatorPriority::MUL_LEVEL; }
	TokenNumber compute(const TokenNumber& lhs,const TokenNumber& rhs) override final;
	virtual std::string toString() override final;
};

}
#endif