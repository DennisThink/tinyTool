/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: LabelSet.cpp
* 摘    要: LabelSet的实现  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/
#pragma warning(disable:4786)
#include <iostream>
#include <algorithm>
#include "LabelSet.h"
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/* 
 * 返回值: null 
 * 功  能: 构造函数
 */
LabelSet::LabelSet()
{
	index = 0;
}

/* 
 * 返回值: null 
 * 功  能: 析构函数
 */
LabelSet::~LabelSet()
{
}

/* 
 * 返回值: void 
 * 功  能: 通过给定的字符串和字符串长度构造符号集
 */
void LabelSet::ReadLabelSet(const char* str, int length)
{
	for (int i = 0 ; i < length ; i++)
	{
		data.push_back(str[i]);
		convert.insert(pair<char, int>(str[i], index));
		index++;
	}
}

/* 
 * 返回值: int 
 * 功  能: 得到ch对应的数字
 */
int LabelSet::GetCharConvertNum(char ch)
{
	return convert.find(ch)->second;
}

/* 
 * 返回值: bool 
 * 功  能: 判断ch是否在集合中
 */
bool LabelSet::IsInLabelSet(char ch)
{
	return (data.end() != find(data.begin(), data.end(), ch));
}

/* 
 * 返回值: void 
 * 功  能: 打印所有的数据
 */
void LabelSet::PrintAllData()
{
	cout<<endl;
	cout<<"\t**************************************************************"<<endl;
	cout<<"\t开始输出符号集"<<endl;
	for (vector<char>::iterator it = data.begin() ; it != data.end() ; it++)
	{
		cout <<"\t"<< (*it) << endl;
	}
	cout<<"\t符号集输出完毕"<<endl;
	cout<<"\t**************************************************************"<<endl;
    cout<<endl<<endl;


	cout<<"\t**************************************************************"<<endl;
	cout << "\t开始输出符号集对应的转换表" << endl;
	for (std::size_t i = 0 ; i < data.size() ; i++)
	{
		map<char,int>::iterator it = convert.find(data[i]);
		if (convert.end() != it)
		{
			cout<<"\t字母: "<<it->first<<"\t对应数字:\t"<<it->second<<endl;
		}
	}
	cout << "\t符号对应转换表输出完毕" << endl;
	cout<<"\t**************************************************************"<<endl;
	cout<<endl<<endl;
}

/* 
 * 返回值: char 
 * 功  能: 得到index位置的字符
 */
char LabelSet::GetLabelChar(int index)
{
	if (index <= (this->index))
	{
		return *(data.begin() + index);
	}
	else
	{
		return *(data.end());
	}
}

/* 
 * 返回值: int 
 * 功  能: 得到集合中符号的总数
 */				 
int LabelSet::GetLabelSetCharCount()
{
	return index;
}
/* 
 * 返回值: map<char,int> 
 * 功  能: 得到转换表 
 */
map<char,int> LabelSet::GetConvertMap()
{
	return convert;
}
