/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: ItemSetVector.h
* 摘    要: 记录项目集的集合  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月13日
************************************************************************/

#ifndef _ITEM_SET_VECTOR_H_
#define _ITEM_SET_VECTOR_H_
#include <vector>
#include "ItemSet.h"
using namespace std;
class ItemSetVector
{
public:
	int itemCount;
public:
	/*根据索引得到项目集*/
	ItemSet GetItemSet(int index);
	/*得到项目集总数*/
	int GetItemSetCount();
	/*打印所有数据*/
	void PrintAllData();
	/*构造函数*/
	ItemSetVector();
	/*析构函数*/
	virtual ~ItemSetVector();
	/*加入项目集*/
	void AddItemSet(ItemSet itemSet);
private:
	/*记录项目集的数据*/
	vector<ItemSet> data;
};
#endif // !defined(AFX_ITEMSETVECTOR_H__763824D4_BB68_412F_BF91_51A10CDA1AE5__INCLUDED_)
