/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: LabelSet.h
* 摘    要: 用来记录终结符和非终结符的类  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/

#ifndef _LABEL_SET_H_
#define _LABEL_SET_H_
#include <vector>
#include <map>
using namespace std;
/************************************************************************/
/* 用于记录终结符和非终结符 																
/************************************************************************/
class LabelSet
{
public:
	/*得到根据符号构造出的转换map*/
	map<char,int> GetConvertMap();
	/*得到索引index对应的字符*/
	char GetLabelChar(int index);
	/*打印所有数据*/
	void PrintAllData();
	/*构造函数*/
	LabelSet();
	/*析构函数*/
	virtual ~LabelSet();
	/*通过传入字符串，确定符号集*/
	void ReadLabelSet(const char* str, int length);
	/*判断是否在符号集中*/
	bool IsInLabelSet(char ch);
	/*得到字符映射的那个数字*/
	int GetCharConvertNum(char ch);
	/*得到字符总数*/
	int GetLabelSetCharCount();
private:
	int index;//用作最初的非终结符计数
	vector<char> data;//用作记录字符的数据
	map<char, int> convert;//字符到数字的转换表
};

#endif // !defined(AFX_LabelSet_H__0D824701_686F_4465_9347_ADE881409D28__INCLUDED_)
