// ItemSet.cpp: implementation of the ItemSet class.
//
//////////////////////////////////////////////////////////////////////

#include "ItemSet.h"
#include <iostream>
#include <algorithm>


ItemSet::ItemSet()
{
	index = -1;
}

ItemSet::~ItemSet()
{
}

/* 
 * 返回值: void 
 * 功  能: 将项目加入到项目集中，防止了重复加入
 */
void ItemSet::AddItem(Item elem)
{
	vector<Item>::iterator it = find(data.begin(), data.end(), elem);
	if (data.end() == it)
	{
		index++;
		elem.SetId(index);
		data.push_back(elem);
	}
	else//为求Closure做的，设置当前为已经使用
	{
		it->SetUsed(true);
	}
}

void ItemSet::PrintAllData()
{
	cout<<endl;
	cout<<"\t**************************************************************"<<endl;
	for (vector<Item>::iterator it = data.begin(); data.end() != it; it++)
	{
		cout<<"\t";
		std::cout << (*it).ToString().c_str() << std::endl;
	}
	cout<<"\t**************************************************************"<<endl;
}

/*
 *	返回以ch开头的，所有项目
 */
ItemSet ItemSet::GetItemSet(char ch)
{
	ItemSet result;
	for (vector<Item>::iterator it = data.begin() ; data.end() != it; it++)
	{
		if ((it->GetLeft()) == ch)
		{
			result.AddItem(*it);
		}
	}
	return result;
}



Item ItemSet::GetItem(int index)
{
	return *(data.begin() + index);
}

int ItemSet::GetItemCount() 
{
	return (index + 1);
}

bool ItemSet::IsInItemSet(Item elem)
{
	return (data.end() != find(data.begin(), data.end(), elem));
}

void ItemSet::SetId(int id)
{
	this->id = id;
}

int ItemSet::GetId()
{
	return this->id;
}
