
#include <iostream>
#include "parse.h"
	
int main(int argc,char * argv[])
{
	std::cout<<argc<<argv[0]<<std::endl;
	using namespace calculater;
	{
		std::string strExpress;
		std::cout<<"Please Input the Express:"<<std::endl;
		std::cin>>strExpress;
		//TokenNumber numResult = TokenNumber(-8);
		auto tokens = CParser::Parse(strExpress);
		auto result = CParser::Calculate(tokens);
		std::cout<<"Result "<<result->toString()<<std::endl;
		//auto calcResult = dynamic_cast<TokenNumber*>(result.get());
	}
	if(argc > 9)
	{
	}
	else
	{
		std::cout<<"error"<<std::endl;
	}
		
	int i = 0;
	std::cout<<i<<std::endl;
    return 0;
}