/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: AnalysisStack.cpp
* 摘    要: 分析栈的实现文件  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/
#include "AnalysisStack.h"
#include <iostream>
using namespace std;
/*默认的构造函数，默认栈的大小为10*/
AnalysisStack::AnalysisStack() : length(10), index(-1)
{
	pStackElem=0;
	pStackElem = new STACKELEM[length];
}

/*带参数的构造函数*/
AnalysisStack::AnalysisStack(int leng) : length(leng), index(-1)
{
	pStackElem = new STACKELEM[length];
}

/*析构函数*/
AnalysisStack::~AnalysisStack()
{
	if(0 != pStackElem)
	{
		delete pStackElem;
		length = 0;
	    index = -1;
	}
}

/* 
 * 返回值: STACKELEM 
 * 功  能: 得到栈顶之下的元素
 */
STACKELEM AnalysisStack::GetSecond()
{
	STACKELEM result;
	if (index <= 0)
	{
		result.Elem = '#';
		result.state = 0;
		return result;
	}
	else
	{
		return *(pStackElem + index - 1);
	}
}

/* 
 * 返回值: STACKELEM 
 * 功  能: 得到栈顶元素
 */
STACKELEM AnalysisStack::GetTop()
{
	STACKELEM result;
	if (-1 == index)
	{
		result.Elem = '#';
		result.state = 0;
		return result;
	}
	else
	{
		return *(pStackElem + index);
	}
}

/* 
 * 返回值: STACKELEM 
 * 功  能: 弹出栈顶元素
 */
STACKELEM AnalysisStack::Pop(void)
{
	STACKELEM result;
	if (-1 == index)
	{
		std::cout<<std::endl;
		std::cout << "\t分析栈为空 " << std::endl;
		result.Elem = '#';
		result.state = 0;
	}
	else
	{
		result = *(pStackElem + index);	
		index--;
	}
	return result;
}

/* 
 * 返回值: void 
 * 功  能: 将元素压入栈中
 */
void AnalysisStack::Push(STACKELEM elem)
{
	if (index < length - 1)
	{
		index++;
		*(pStackElem + index) = elem;
	}
	else
	{
		std::cout<<std::endl;
		std::cout << "\t分析栈已满" << std::endl;
	}
}

/* 
 * 返回值: void 
 * 功  能: 根据nCount对栈进行规约
 */
void AnalysisStack::Reduce(int nCount)
{
	for (int x = 0; x < nCount ; x++)
	{
		Pop();
	}
}

/* 
 * 返回值: void 
 * 功  能: 打印堆栈的内容
 */
void AnalysisStack::PrintStack()
{
	std::cout<<std::endl<<"\t";
	for (int x = 0; x < index ; x++)
	{
		std::cout << (*(pStackElem + x)).Elem << (*(pStackElem + x)).state;
	}
}

/* 
 * 返回值: bool 
 * 功  能: 测试栈
 */
bool AnalysisStack::Test()
{
	AnalysisStack stack(10);
	return true;
}

void AnalysisStack::Clear()
{
	while (-1 != index)
	{
		Pop();
	}
	STACKELEM elem;
	elem.Elem='#';
	elem.state=0;
	Push(elem);
}
