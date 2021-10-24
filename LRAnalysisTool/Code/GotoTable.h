/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: GotoTable.h
* 摘    要: 状态转换表头文件，记录状态转换表的字段和函数
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月12日
************************************************************************/
#ifndef _GOTO_TABLE_H_
#define _GOTO_TABLE_H_
#include "CommonData.h"
#include <map>
using namespace std;
class GotoTable
{
public:
	/*打印所有数据*/
	void PrintAllData();
	/*构造函数*/
	GotoTable();
	/*拷贝构造函数*/
	GotoTable(GotoTable& gt);
	/*根据x,y初始化数据*/
	void InitializeData(int x, int y);
	/*设置转换表*/
	void SetConvertMap(map<char, int> elem);
	/*根据非终结符ch和项目集编号xIndex设置goto表的对应变量elem*/
	void SetData(char ch, int xIndex, int elem);
	/*得到非终结符ch,项目集编号xIndex对应的状态*/
	int GetData(char ch, int xIndex);
	/*得到xIndex,yInde位置的数*/
	int GetData(int xIndex,int yIndex);
	/*得到xLength*/
	int GetXLength();
	/*得到YLength*/
	int GetYLength();
	/*得到转换表*/
	map<char,int> GetConvertMap();
	/*析构函数，释放内存*/
	virtual ~GotoTable();
private:
	/*得到非终结符对应的列号*/
	int ConvertCharToInt(char ch);
	/*非终结符与列号对应的转换表*/
	map<char, int> convert;
	/*记录数据*/
	int** data;
	/*表格的行数*/
	int xLength;
	/*表格的列数*/
	int yLength;
};

#endif // !defined(AFX_GOTOTABLE_H__7854D208_1AD7_4804_AC8A_92FB9A0C01CA__INCLUDED_)
