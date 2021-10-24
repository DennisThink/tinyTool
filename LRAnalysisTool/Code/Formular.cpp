/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: Formular.cpp
* 摘    要: Formular.h的实现文件
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月12日
************************************************************************/
#include "Formular.h"
#include <stdlib.h>
#include <string.h>
/* 构造函数 */
Formular::Formular()
{
}

/* 用一个字符串构造推导式 */
Formular::Formular(const char* source)
{
	unsigned int index = 0;
	id = 0;
	left = source[index];
	index += 4;
	while (index < strlen(source) - 1)
	{
		right += source[index];
		index ++;
	}
	rightLength = index - 4;
}

/* 得到推导式右部长度 */
int Formular::GetRightLength()
{
	return rightLength;
}

/* 设置左部的非终结符 */
void Formular::SetLeft(char ch)
{
	left = ch;
}

/* 得到左部的非终结符 */
char Formular::GetLeft()
{
	return left;
}

/* 得到右部的字符串 */
std::string Formular::GetRight()
{
	return right;
}

/* 得到推导式对应的字符串 */
std::string Formular::ToString()
{
	std::string str;
    
	if(id > 10 )
	{
		str += char('0'+id/10);
		str += char('0'+id%10);
	}
	else
	{
		str += char('0'+id%10);
	}

	str += ": ";
	str += left;
	str += "-->";
	str += right;
	return str;
}


/*不知道怎么描述这个函数的功能*/
std::string Formular::PrintAllField()
{
	std::string str;
	str +=left;
	str +="\t:";
	str += right;
	return str;
}

/* 设置推导式id */
void Formular::SetId(int id)
{
	this->id = id;
}

/* 得到推导式右部对应位置的字符 */
char Formular::GetRightOfIndex(int index)
{
	if (index < GetRightLength())
	{
		return right[index];
	}
	else
	{
		return right[0];
	}
}

/*设置推导式右部*/
void Formular::SetRight(std::string str)
{
	right = str;
	rightLength=str.length();
}

bool Formular::operator ==(Formular& other)
{
	return((other.GetLeft()==left)&&( (other.GetRight().compare(right)==0)));
}
