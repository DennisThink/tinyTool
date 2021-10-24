/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: Relation.cpp
* 摘    要: 表示First和Follow集合的类的实现  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/
#pragma warning(disable:4786)
#include "Relation.h"


/* 
 * 返回值: null 
 * 功  能: 构造函数
 */
Relation::Relation()
{
}

/* 
 * 返回值: null 
 * 功  能: 析构函数
 */
Relation::~Relation()
{
}

/* 
 * 返回值: void 
 * 功  能: 向集合中添加元素
 */
void Relation::AddRightChar(char ch)
{
	right.insert(ch);
}

/* 
 * 返回值: bool 
 * 功  能: 判断ch是否位于集合的中
 */
bool Relation::IsInRight(char ch)
{
	return (right.end() != right.find(ch));
}

/* 
 * 返回值: std::string 
 * 功  能: 返回集合的字符串化表示
 */
std::string Relation::ToString()
{
	std::string str;
	str += start;
	str += "   ";
	for (set<char>::iterator it = right.begin() ; it != right.end() ; it++)
	{
		str += (*it);
	}
	return str;
}

/* 
 * 返回值: void 
 * 功  能: 设置集合的开始字符，即First(S)中的S
 */
void Relation::SetStart(char ch)
{
	start = ch;
}

/* 
 * 返回值: char 
 * 功  能: 得到集合的起始字符，即First(S)中的S
 */
char Relation::GetStart()const
{
	return start;
}

/* 
 * 返回值: int 
 * 功  能: 得到集合右部元素的个数，即First(S)={x,y},花括号中的元素个数
 */
int Relation::GetRightCount()
{
	return right.size();
}

/* 
 * 返回值: bool
 * 功  能: 重载==操作符
 */
bool Relation::operator ==(const Relation& other)
{
	return (other.GetStart() == (GetStart()));
}

/* 
 * 返回值: std::string 
 * 功  能: 得到集合右部的字符串化表示
 */
string Relation::GetRightString()
{
	string str;
	for (set<char>::iterator it = right.begin() ; right.end() != it ; it++)
	{
		str += *(it);
	}
	return str;
}
