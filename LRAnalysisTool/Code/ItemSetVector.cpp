/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: ItemSetVector.cpp
* 摘    要: ItemSetVector类的实现文件  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/

#include "ItemSetVector.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/* 
 * 返回值: null 
 * 功  能: 构造函数
 */
ItemSetVector::ItemSetVector()
{
	itemCount = 0;
}

/* 
 * 返回值: null 
 * 功  能: 析构函数
 */
ItemSetVector::~ItemSetVector()
{
}

/* 
 * 返回值: void 
 * 功  能: 将ItemSet加入到项目集集合中
 */
void ItemSetVector::AddItemSet(ItemSet itemSet)
{
	itemSet.SetId(itemCount);
	itemCount++;
	data.push_back(itemSet);
}

/* 
 * 返回值: void 
 * 功  能: 打印项目集集合中的所有项目集元素
 */
void ItemSetVector::PrintAllData()
{
	int index = 0;
	cout<<endl;
	cout<<"\t**************************************************************"<<endl;
	cout<<"\t开始输出Closure集中的元素"<<endl<<endl;;
	for (vector<ItemSet>::iterator it = data.begin() ;
		data.end() != it ;
		it++)
	{
		cout<<"\t输出第"<<index<<"个项目集";
		it->PrintAllData();
		index ++;
	}
    cout<<"\tClosure集中的元素输出完毕"<<endl;
	cout<<"\t**************************************************************"<<endl;
}

/* 
 * 返回值: int 
 * 功  能: 得到项目集集合中项目集的个数
 */
int ItemSetVector::GetItemSetCount()
{
	return data.size();
}

/* 
 * 返回值: ItemSet 
 * 功  能: 返回index对应的项目集
 */
ItemSet ItemSetVector::GetItemSet(int index)
{
	return data[index];
}
