

#include "div_operator.h"

namespace calculater {

	TokenNumber DivOperator::compute(const TokenNumber& lhs,const TokenNumber& rhs)
	{
		return TokenNumber(lhs.getValue()/rhs.getValue());
	}

	std::string DivOperator::toString()
	{
		return "/";
	}

}