
#ifndef _ADD_OPERATOR_H_
#define _ADD_OPERATOR_H_
#include "token_operator.h"
namespace calculater {	

class AddOperator:public TokenOperator
{
public:
	explicit AddOperator(){};
	virtual ~AddOperator(){}
	virtual OperatorType getOperatorType() override final{ return OperatorType::ADD_OPERATOR;}
	virtual OperatorPriority getOperatorPriority() override final { return OperatorPriority::ADD_LEVEL; }
	TokenNumber compute(const TokenNumber& lhs,const TokenNumber& rhs) override final;
	virtual std::string toString() override final;
};

}
#endif