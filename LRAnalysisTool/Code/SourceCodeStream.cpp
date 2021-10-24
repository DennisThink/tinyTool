/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: SourceCodeStream.cpp
* 摘    要: 分析串读取类SourceCodeStream的实现文件  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/
#include "SourceCodeStream.h"
#include <iostream>
#include <string.h>
#include <memory.h>
/* 
 * 返回值: null 
 * 功  能: 构造函数
 */
SourceCodeStream::SourceCodeStream() : index(0), length(0)
{
}

/* 
 * 返回值: bool 
 * 功  能: 读取分析串
 */
bool SourceCodeStream::ReadSourceCode()
{
	Clear();
	index = 0;
	char buffer[100];
	std::cin >> buffer;
	length = strlen(buffer);
	for (int i = 0 ; i < length ; i++)
	{
		codeBuffer[i] = buffer[i];
	}
	return true;
}

/* 
 * 返回值: char 
 * 功  能: 得到当前字符
 */
char SourceCodeStream::GetCurrentChar()
{
	index++;
	return codeBuffer[index];
}

/* 
 * 返回值: bool 
 * 功  能: 判断分析串是否结束
 */
bool SourceCodeStream::isEnd()
{
	return (index == length);
}

void SourceCodeStream::Clear()
{
	for (int i = 0 ; i < 100 ; i++)
	{
		codeBuffer[i]='#';
	}
	index = 0 ;
	length = 0 ;
}
