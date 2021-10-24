/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称:Formular.h
* 摘    要:推导式的头文件，记录了文法推导式的数据和主要方法
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月12日
************************************************************************/

#ifndef _FORMULAR_H_
#define _FORMULAR_H_
#include <iostream>

class Formular
{
public:
	/*设置右部*/
	void SetRight(std::string str);
	/*构造函数*/
	Formular();
	/* 通过一个字符串构造表达式，字符串格式为 非终结符-->推导式右部 */
	Formular(const char* source);
	/*根据下标得到推导式右部对应位置的字符*/
	char GetRightOfIndex(int index);

	/* 设置推导式的Id号 */
	void SetId(int id);

	/* 输出所有字段的值 */
	std::string PrintAllField();

	/* 得到右部的字符串 */
	std::string GetRight();

	/* 得到左部的非终结符 */
	char GetLeft();
	
	/* 得到推导式右部字符数，以1开始*/
	int GetRightLength(); 

	/* 设置左部的非终结符 */
	void SetLeft(char ch); 

	/* 格式化为字符串 */
	std::string ToString();

	/*相等比较函数*/
	bool operator ==(Formular& other);

private:
	char left;				//左边的非终结符
	std::string right;		//推导出的右部字符串
	int rightLength;		//右边的字符串长度
	int id;					//推导式编号
};
#endif