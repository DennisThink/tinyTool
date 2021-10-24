
/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: FormularSet.h
* 摘    要: FormulatSet用来记录Formular的集合类，
*			这个文件是FormularSet类的头文件，记录相关的字段和函数
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月12日
************************************************************************/

#ifndef _FORMULAR_SET_H_
#define _FORMULAR_SET_H_
#include "Formular.h"
#include <vector>
using namespace std;
class FormularSet
{
public:
	int GetFormularIndex(Formular formular);
	/* 根据非终结符找到在推导式集合中以该非终结符起始的集合 */
	FormularSet GetFormularSet(char left);
	int curIndex;
	/* 根据下标访问推导式 */
	Formular GetFormular(int index);
	/* 得到推导式的计数，以1开始 */
	int GetFormularCount();
	/* 输出推导式的所有数据 */
	void PrintAllData();
	/* 增加推导式 */
	void AddFormular(Formular fr);
	/* 构造函数 */
	FormularSet();
	/* 析构函数 */
	virtual ~FormularSet();
private:
	vector<Formular> data;  //推导式集合的数据存储
	int index;				//推导式集合中推导式的总数
};

#endif // !defined(AFX_FORMULARSET_H__4F36ADD0_B8E6_45ED_92E9_C4F66D787550__INCLUDED_)
