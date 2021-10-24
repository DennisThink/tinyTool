/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: GotoTable.cpp
* 摘    要: GotoTable的实现文件  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/
#pragma warning(disable:4786)
#include <iostream>
#include "GotoTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*构造函数*/
GotoTable::GotoTable()
{
	data = 0;
}

GotoTable::GotoTable(GotoTable& gt)
{
	this->xLength = gt.GetXLength();
	this->yLength = gt.GetYLength();
	InitializeData(xLength,yLength);
	for (int x = 0; x < xLength ; x++)
	{
		for (int y = 0; y < yLength ; y++)
		{
			this->data[x][y]=gt.GetData(x,y);
		}
	}
	SetConvertMap(gt.GetConvertMap());

}
/*析构函数，完成内存释放*/
GotoTable::~GotoTable()
{
	if(0 != data)
	{
		for (int x = 0 ; x < xLength ; x++)
		{
			delete[] data[x];
		}
	    delete data;
	}	
}

/*根据x,y初始化数据*/
void GotoTable::InitializeData(int x, int y)
{
	this->xLength = x;
	this->yLength = y;
	data = new int * [x];
	for (int i = 0 ; i < x ; i++)
	{
		data[i] = new int[y];
	}
	for(int k = 0 ; k < x ; k++)
	{
		for(int m = 0 ; m < y ; m++)
		{
			data[k][m]=0;
		}
	}
}

/*
 *	得到对应于非终结符ch,项目集编号xIndex位置的数据
 */
int GotoTable::GetData(char ch, int xIndex)
{
	int yIndex = ConvertCharToInt(ch);
	if (-1 != yIndex)
	{
		return data[xIndex][yIndex];
	} 
	else
	{
		return -1;
	}
}

/*
 *	将非终结符ch转化为对应的数字
 */
int GotoTable::ConvertCharToInt(char ch)
{
	map<char, int>::iterator it = convert.find(ch);
	if (convert.end() == it)
	{
		return -1;
	}
	else
	{
		return (it->second);
	}
}

/*设置转换map*/
void GotoTable::SetConvertMap(map<char, int> elem)
{
	convert = elem;
}

/*置非终结符ch,项目集编号xIndex位置元素的值为elem*/
void GotoTable::SetData(char ch, int xIndex, int elem)
{
	int yIndex = ConvertCharToInt(ch);
	if (-1 != yIndex)
	{
		data[xIndex][yIndex] = elem;
	} 
}

/* 
 * 返回值: void 
 * 功  能: 打印goto表的所有内容
 */

void GotoTable::PrintAllData()
{
    cout<<"\t**************************************************************"<<endl;
	cout<<"\t开始输出goto表中的数据"<<endl;
	for (int x = 0; x < xLength ; x++)
	{
		cout<<"\t";
		for (int y = 0; y < yLength ; y++)
		{
			std::cout<<data[x][y]<<"\t";
		}
		std::cout<<std::endl;
	}
	cout<<"\tgoto表中的数据输出完毕"<<endl;
	cout<<"\t**************************************************************"<<endl;
}

/* 
 * 返回值: int 
 * 功  能: 得到goto表的xLength长度
 */
int GotoTable::GetXLength()
{
	return xLength;
}

/* 
 * 返回值: int 
 * 功  能: 得到Gotob表的yLength长度
 */
int GotoTable::GetYLength()
{
	return yLength;
}

/* 
 * 返回值: map<char,int> 
 * 功  能: 得到Goto表的转换map
 */
map<char,int> GotoTable::GetConvertMap()
{
	return convert;
}

/* 
 * 返回值: int  
 * 功  能: 得到(xIndex,yIndex)位置的值
 */
int GotoTable::GetData(int xIndex,int yIndex)
{
	if ( (xIndex < xLength) && (yIndex < yLength) )
	{
		return data[xIndex][yIndex];
	} 
	else
	{
		return -1;
	}
}
