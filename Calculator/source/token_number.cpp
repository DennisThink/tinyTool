#include "token_number.h"
#include <stdlib.h>
namespace calculater {	
TokenNumber::TokenNumber(const std::string& str)
{
	m_orgStr = str;
	m_value = atof(str.c_str());
}

double TokenNumber::getValue() const 
{
    return m_value;
}

TokenType TokenNumber::getTokenType() {
	return TokenType::NUMBER;
}

TokenNumber::TokenNumber(const double value)
{
	m_value = value;
}

std::string TokenNumber::toString()
{
	if(m_orgStr.length() > 0)
	{
		return m_orgStr;
	}
	else
	{
		char buff[128]={0};
		sprintf(buff,"%f",m_value);
		return std::string(buff);
	}
}
}