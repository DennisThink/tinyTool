
/************************************************************************
* copyright (c) 2013, 计科09级02班
* all rights reserved.
* 
* 文件名称:ActionTable.h
* 摘    要:该文件为LR分析程序Action表的头文件，说明了动作表的一些常用函数
*		   和常用功能.表格的行数为项目集的个数，列数为终结符的个数，
*          所以终结符会转换为对应的列号
*           
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月26日
************************************************************************/


#ifndef _ACTION_TABLE_H_
#define _ACTION_TABLE_H_

#include <map>
#include "CommonData.h"
using namespace std;

class ActionTable
{
public:
	/*得到对应位置(xIndex,yIndex)的值*/
	TableElem GetData(int xIndex,int yIndex);
	/*得到转换表，供拷贝函数使用*/
	map<char,int> GetConvertMap();
	/*得到列长度*/
	int GetYLength();
	/*得到行长度*/
	int GetXLength();
	/*打印所有数据*/
	void PrintAllData();
	/*构造函数*/
	ActionTable();
	/*拷贝构造函数*/
	ActionTable(ActionTable& ac);
	/*设置转换表格*/
	void SetConvertMap(map<char, int> convertMap);
	/*得到终结符在表中对应的数字*/
	int ConvertCharToInt(char ch);
	/*初始化表格*/
	bool InitializeTableData(int x, int y);
	/*对表格对应位置(xindex,ConvertCharToInt(ch))的数据设置为elem*/
	void SetData(int xIndex, char ch, TableElem elem);
	/*得到表格对应位置(xIndex,ConvertCharToInt(ch))位置的数据*/
	TableElem GetData(int xIndex, char ch);
	/*析构函数，释放表格数据空间*/
	virtual ~ActionTable();
private:
	/*行数*/
	int xLength;
	/*列数*/
	int yLength;
	/*非终结符到列号的转换表*/
	map<char, int> convert;
	/*状态转换表数据*/
	TableElem** data;
};

#endif // !defined(AFX_ACTIONTABLE_H__D3248C29_C492_45BC_A641_788CB8992337__INCLUDED_)
