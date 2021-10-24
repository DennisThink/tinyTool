
#include <iostream>
#include "ActionTable.h"
using namespace std;


/*
 *构造函数，完成数据的初始化
 */
ActionTable::ActionTable()
{
	data = 0;
}

/*
 * 析构函数，用于释放转换表动态申请的空间
 */
ActionTable::~ActionTable()
{
	if (0 != data)
	{
		for (int i = 0 ; i < xLength ; i++)
		{
			delete[] data[i];
		}
		delete data;
	}
}

ActionTable::ActionTable(ActionTable& other)
{
	this->xLength = other.GetXLength();
	this->yLength = other.GetYLength();
	InitializeTableData(xLength,yLength);
	for (int x = 0 ; x < xLength ; x++)
	{
		for (int y = 0; y < yLength ; y++)
		{
			this->data[x][y]=other.GetData(x,y);
		}
	}
	SetConvertMap(other.GetConvertMap());
}
/*
 * 初始化表格内存函数，对存储数据的区域进行动态申请，x为行号，y为列号
 */
bool ActionTable::InitializeTableData(int x, int y)
{
	this->xLength = x;
	this->yLength = y;
	try
	{
		data = new TableElem * [x];
		for (int i = 0 ; i < x ; i++)
		{
			data[i] = new TableElem[y];
		}
		for (int j = 0; j < x ; j++)
		{
			for (int k = 0; k < y ; k++)
			{
				data[j][k].Elem = 'E';
				data[j][k].state = -1;
			}
		}
	}
	catch (std::exception e)
	{
		std::cout<<e.what()<<std::endl;
		return false;
	}

	return true;
}

/*
 * 设置对应位置的数据，
 */
void ActionTable::SetData(int xIndex, char ch, TableElem elem)
{
	int yIndex = ConvertCharToInt(ch);
	data[xIndex][yIndex] = elem;
}

/*
 * 得到对应位置的数据
 */
TableElem ActionTable::GetData(int xIndex, char ch)
{
	int yIndex = ConvertCharToInt(ch);
	if (-1== yIndex)
	{
		TableElem elem;
		elem.Elem='E';
		elem.state=-1;
		return elem;
	} 
	else
	{
		return data[xIndex][yIndex];
	}
}

/************************************************************************/
/* 得到非终结符在表列的索引                                             */
/************************************************************************/
int ActionTable::ConvertCharToInt(char ch)
{
	map<char,int>::iterator it = convert.find(ch);
	if (convert.end() != it)
	{
		return it->second;
	} 
	else
	{
		return -1;
	}
}

void ActionTable::PrintAllData()
{
	cout<<endl;
	cout<<"\t**************************************************************"<<endl;
	cout<<"\t输出动作表的数据"<<endl;
	for (int xIndex = 0 ; xIndex < xLength ; xIndex++ )
	{
		cout<<"\t";
		for (int yIndex = 0; yIndex < yLength ; yIndex++)
		{
			std::cout<<data[xIndex][yIndex].Elem<<data[xIndex][yIndex].state<<"\t";
		}
		std::cout<<std::endl;
	}
	cout<<"\t动作表数据输出完毕"<<endl;
	cout<<"\t**************************************************************"<<endl;
}

void ActionTable::SetConvertMap(map<char,int> convertMap)
{
	this->convert = convertMap;
}

int ActionTable::GetXLength()
{
	return xLength;
}

int ActionTable::GetYLength()
{
	return yLength;
}

map<char,int> ActionTable::GetConvertMap()
{
	return convert;
}

TableElem ActionTable::GetData(int xIndex, int yIndex)
{
	return data[xIndex][yIndex];
}
