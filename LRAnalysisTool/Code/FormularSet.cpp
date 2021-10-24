// FormularSet.cpp: implementation of the FormularSet class.
//
//////////////////////////////////////////////////////////////////////
/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: FormularSet.cpp
* 摘    要: 对项目集的实现
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月12日
************************************************************************/

#include "FormularSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/* 推导式集构造函数 */
FormularSet::FormularSet()
{
	curIndex = -1;
}

/* 推导式集析构函数 */
FormularSet::~FormularSet()
{
}

/* 向集合中添加推导式*/
void FormularSet::AddFormular(Formular fr)
{
	curIndex++;
	fr.SetId(curIndex);
	data.push_back(fr);
}

/*输出推导式集合中所有元素*/
void FormularSet::PrintAllData()
{
	cout<<endl<<endl;
	cout<<"\t**************************************************************"<<endl;
	cout<<"\t现在开始输出推导式集合"<<endl;
	for (std::size_t i = 0 ; i < data.size() ; i++)
	{
		cout<<"\t";
		cout<<data[i].ToString().c_str()<<endl;
	}
	cout<<"\t推导式集合输出完毕"<<endl;
	cout<<"\t**************************************************************"<<endl;
	cout<<endl<<endl;
}

/* 
 * 返回值: Formular 
 * 功  能: 根据下标返回对应的推导式 
 */
Formular FormularSet::GetFormular(int index)
{
	if (index <= curIndex)
	{
		return (data[index]);
	}
	else
	{
		return (data[0]);
	}
}

/* 
 * 返回值: int 
 * 功  能: 返回推导式集合中推导式的个数
 */
int FormularSet::GetFormularCount()
{
	return (curIndex + 1);
}

/* 
 * 返回值: FormularSet 
 * 功  能: 返回左部为left的推导式的集合 
 */
FormularSet FormularSet::GetFormularSet(char left)
{
	FormularSet result;
	for (vector<Formular>::iterator it = data.begin() ;
		data.end() != it ;
		it++)
	{
		if (left == (*it).GetLeft())
		{
			result.AddFormular(*(it));
		}
	}
	return result;
}

int FormularSet::GetFormularIndex(Formular formular)
{
	for (int i = 0 ; i < GetFormularCount() ; i++)
	{
		if (GetFormular(i)==formular)
		{
			return i;
		} 
	}
	return -1;
}
