/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: ItemSet.h
* 摘    要: 项目集类的头文件  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月13日
************************************************************************/

#ifndef _ITEM_SET_H_
#define _ITEM_SET_H_


#include <vector>
#include "Item.h"
using namespace std;
class ItemSet
{
public:
	/*得到项目集Id*/
	int GetId();
	/*拷贝构造函数*/
	void SetId(int id);
	/*判断某个项目是否在在项目集中*/
	bool IsInItemSet(Item elem);
	/*返回项目集中共有多少个项目*/
	int GetItemCount();
	/*根据索引得到项目集中的项目*/
	Item GetItem(int index) ;
	/*得到以非终结符ch为左端的项目集*/
	ItemSet GetItemSet(char ch);
	/*构造函数*/
	ItemSet();
	/*向项目集中添加项目*/
	void AddItem(Item elem);
	/*打印所有项目*/
	void PrintAllData();
	/*析构函数*/
	virtual ~ItemSet();
private:
	/*记录项目的数据*/
	vector<Item> data;
	/*项目集中所含项目的总数*/
	int index;
	/*项目集编号*/
	int id;
};

#endif // !defined(AFX_ITEMSET_H__E61D789E_2EA3_4D27_A589_050B29B9F75A__INCLUDED_)
