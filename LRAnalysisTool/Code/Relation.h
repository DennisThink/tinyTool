/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: Relation.h
* 摘    要: 用于存储first集和follow集的类  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/

#ifndef _RELATION_H_
#define _RELATION_H_
#include <set>
#include <string>
using namespace std;
class Relation
{
public:
	/*得到集合中的左部，即First(S)中的S*/
	char GetStart()const;
	/*设置集合的左部，即first(S)与Follow(S)中的S*/
	void SetStart(char ch);
	/*构造函数*/
	Relation();
	/*析构函数*/
	virtual ~Relation();
    /*判断ch是不是集合中的元素*/
	bool IsInRight(char ch);
	/*增加集合右部的元素，即First(S)={x,y}右部的花括号中的元素*/
	void AddRightChar(char ch);
	/*得到集合中First(S)={x,y}右部花括号内的元素个数*/
	int GetRightCount();
	/*得到右部的字符串化表示*/
	string GetRightString();
	/*是否相等的运算符*/
	bool operator ==(const Relation& other);
	/*集合的字符串化表示*/
	string ToString();
private:
	char start;		//First(S)={x,y}中的S
	std::set<char> right;//First(S)={x,y}中的花括号内的元素存储
};

#endif // !defined(AFX_RELATION_H__522A436C_B791_4858_A1ED_EB61FF2ABBF5__INCLUDED_)
