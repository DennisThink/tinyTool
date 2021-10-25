

#ifndef _PARSE_H_
#define _PARSE_H_
#include "token.h"
#include <memory>
#include <vector>
#include <string>
namespace calculater {
	using TokenPtr = std::shared_ptr<CToken>;
	using TokenPtrVec = std::vector<TokenPtr>;	
class CParser
{
public:
	TokenPtrVec static Parse(const std::string& str); 
	TokenPtr static Calculate(TokenPtrVec tokens);
};

}
#endif