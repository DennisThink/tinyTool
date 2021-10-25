
#include "sub_operator.h"
namespace calculater {	
TokenNumber SubOperator::compute(const TokenNumber& lhs,const TokenNumber& rhs)
{
	return TokenNumber(lhs.getValue()-rhs.getValue());
}

std::string SubOperator::toString()
{
	return "-";
}
}