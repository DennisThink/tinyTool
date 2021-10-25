
#include "token_operator.h"
namespace calculater {
	TokenOperator::TokenOperator()
	{
		
	}

	TokenOperator::~TokenOperator()
	{
		
	}

	TokenType TokenOperator::getTokenType() 
	{
		return TokenType::OPERATOR;
	}

	std::string TokenOperator::toString()
	{
		return " ";
	}
}