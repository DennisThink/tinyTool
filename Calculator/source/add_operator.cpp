#include "add_operator.h"
namespace calculater {	
TokenNumber AddOperator::compute(const TokenNumber& lhs,const TokenNumber& rhs)
{
	return TokenNumber(lhs.getValue()+rhs.getValue());
}

std::string AddOperator::toString()
{
	return "+";
}
}