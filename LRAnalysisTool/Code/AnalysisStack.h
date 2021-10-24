/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: AnalysisStack.h
* 摘    要: 语法分析的时候用到的栈，栈元素为结构体类型，包含一个字符和
*			一个整数，实现了出栈，入栈，得到栈顶元素等功能  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/
#ifndef _ANALYSIS_STACK_H_
#define _ANALYSIS_STACK_H_
/*栈元素，Elem为字符，state为状态*/
typedef struct stackElem
{
	char Elem;
	int state;
}STACKELEM;

class AnalysisStack
{
private:
	/*栈元素指针*/
	STACKELEM* pStackElem;
	/*当前栈中元素下标*/
	int index;
	/*栈的大小*/
	int length;
public:
	void Clear();
	/*打印栈*/
	void PrintStack();
	/*测试函数*/
	bool Test();
	/*带参数的构造函数，参数为*/
	AnalysisStack(int leng);
	/*不带参数的构造函数*/
	AnalysisStack();
	/*析构函数*/
	~AnalysisStack();
	/*得到栈顶元素*/
	STACKELEM GetTop();
	/*得到栈顶之下的元素*/
	STACKELEM GetSecond();
	/*弹出栈顶元素*/
	STACKELEM Pop();
	/*入栈*/
	void Push(STACKELEM elem);
	/*根据nCount对栈进行规约*/
	void Reduce(int nCount);
};
#endif
