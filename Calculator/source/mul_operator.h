#ifndef _MUL_OPERATOR_H_
#define _MUL_OPERATOR_H_
#include "token_operator.h"
namespace calculater {

class MulOperator:public TokenOperator
{
public:
	explicit MulOperator(){};
	virtual ~MulOperator(){}
	virtual OperatorType getOperatorType() override final{ return OperatorType::MUL_OPERATOR;}
	virtual OperatorPriority getOperatorPriority() override final { return OperatorPriority::MUL_LEVEL; }
	virtual TokenNumber compute(const TokenNumber& lhs,const TokenNumber& rhs) override final;
	virtual std::string toString() override final;
};

}
#endif