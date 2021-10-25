#ifndef _CALC_TYPE_DEFINE_H_
#define _CALC_TYPE_DEFINE_H_
namespace calculater {

enum class TokenType
{
    OPERATOR,
    NUMBER,
    UNKNOWN,
};

enum class OperatorType 
{
	UNKNOWN_OPERATOR=0,
    ADD_OPERATOR=1,
    SUB_OPERATOR=2,
    MUL_OPERATOR=3,
    DIV_OPERATOR=4,
    LEFT_BRACKETS=5,
    RIGHT_BRACKETS=6,
	OTHER_OPERATOR=7,
};

enum class OperatorPriority
{
	UNKNOWN_LEVEL=0,
    ADD_LEVEL=1,
    MUL_LEVEL=2,
    LEFT_BRACKETS_LEVEL=3,
    RIGHT_BRACKETS_LEVEL=4,
};

}
#endif