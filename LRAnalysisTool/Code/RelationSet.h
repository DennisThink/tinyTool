/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: RelationSet.h
* 摘    要: 存储First集和Follow集的类，  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/
#ifndef _RELATION_SET_H_
#define _RELATION_SET_H_

#include "Relation.h"
#include <vector>
class RelationSet
{
public:
	/*得到以ch为左端的集合，即First(ch)*/
	Relation GetRelation(char ch);
	/*打印输出所有数据*/
	void PrintAllData();
	/*构造函数*/
	RelationSet();
	/*析构函数*/
	virtual ~RelationSet();
	/*向集合中增加First或Follow集*/
	void AddRelation(Relation re);
private:
	vector<Relation> data;//用于存储集合的变量
};

#endif // !defined(AFX_RELATIONSET_H__A3C37A1A_3B8D_4E50_9086_E0F0ECD3576C__INCLUDED_)
