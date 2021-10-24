/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: Item.h
* 摘    要: 语法分析项目元素头文件，记录字段以及相应的方法，
*			考虑到了LR(0)与LR(1)项目的区别  
*            
*            
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月12日
************************************************************************/

#ifndef _ITEM_H_
#define _ITEM_H_
#include "Formular.h"
class Item
{
public:
	/*得到当前项目对应的推导式，在填ActionTable的时候，要填R后的数字时使用*/
	Formular GetFormular();
	/*设置是否已被加入到过项目集*/
	void SetUsed(bool used);
	/*得到是否已被加入到过项目集*/
	bool IsUsed();
	/*设置Id*/
	void SetId(int ID);
	/*返回项目的字符串表示*/
	std::string ToString();
	/*构造函数*/
	Item();
	/*通过推导式f,点的位置ptPos,构造项目*/
	Item(Formular f, int ptPos);
	/*得到该项目对应的下一个项目，即将点的位置后移*/
	Item GetNext();
	/*析构函数*/
	virtual ~Item();
	/*设置项目左部非终结符*/
	void SetLeft(char lf);
	/*得到项目左部非终结符*/
	char GetLeft(void) const;
    /*设置项目右部字符串*/
	void SetRight(std::string right);
	/*得到项目右部字符串*/
	std::string GetRight(void) const;
	/*设置项目的展望串，仅对LR(1)有效*/
	void SetPreString(std::string str);
	/*得到项目的展望串*/
	std::string GetPreString(void) const;
	/*设置点的位置*/
	void SetPointPos(int pos);
	/*得到点的位置*/
	int GetPointPos(void) const;
	/*重载operator ==*/
	bool operator ==(const Item& other);
private:
	bool bUsed;			//是否已被加入项目集的标志
	char left;			//项目左部
	std::string right;	//项目右部
	int id;				//项目编号
	int pointPos;		//点的位置，最左为0
	std::string preString;//展望串
};

#endif // !defined(AFX_ITEM_H__DBA6FFCA_BC1B_4294_AD02_7FCC8B3A9EE0__INCLUDED_)
