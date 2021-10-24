/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: RelationSet.cpp
* 摘    要: RelationSet的实现文件  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/
#pragma warning(disable:4786)
#include "RelationSet.h"
#include <iostream>
#include <algorithm>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/* 
 * 返回值: null 
 * 功  能: 构造函数
 */
RelationSet::RelationSet()
{
}

/* 
 * 返回值: null 
 * 功  能: 析构函数
 */
RelationSet::~RelationSet()
{
}

/* 
 * 返回值: void 
 * 功  能: 向当前的first或者follow集中增加first集或者follow集，只有当要增加的集合右部
 *		   大于当前存在的集合右部时，才进行增加
 */

void RelationSet::AddRelation(Relation re)
{
	vector<Relation>::iterator it = find(data.begin(), data.end(), re);
	if (data.end() == it)
	{
		data.push_back(re);
	}
	else
	{
		if ((it->GetRightCount()) < re.GetRightCount())
		{
			data.erase(it);
			data.push_back(re);
		}
	}
}

/* 
 * 返回值: void 
 * 功  能: 打印集合中的所有数据
 */
void RelationSet::PrintAllData()
{
	for (vector<Relation>::iterator it = data.begin() ;
		data.end() != it ;
		it++)
	{
		cout << (*it).ToString().c_str() << endl;
	}
}

/* 
 * 返回值: Relation 
 * 功  能: 得到非终结符ch所对应的first集或者follow集
 */
Relation RelationSet::GetRelation(char ch)
{
	for (vector<Relation>::iterator it = data.begin() ;
		data.end() != it ;
		it++)
	{
		if (ch == (*it).GetStart())
		{
			return (*it);
		}
	}
	return (*data.begin());
}
