/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: Item.cpp
* 摘    要: Item的实现文件  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月13日
************************************************************************/

#include "Item.h"


/* 
 * 返回值: null 
 * 功  能: 通过推导式以及点的位置构造项目
 */
Item::Item(Formular f, int ptPos) : pointPos(ptPos)
{
	this->left = f.GetLeft();
	this->right = f.GetRight();
	this->id = 0;
	this->pointPos = 0;
	this->preString = "";
	this->bUsed = false;
}

/* 
 * 返回值: null 
 * 功  能: 项目的构造函数
 */
Item::Item()
{
	this->left = 'E';
	this->right = "";
	this->id = 0;
	this->pointPos = 0;
	this->preString = "";
	this->bUsed = false;
}

/*项目的析构函数*/
Item::~Item()
{
}

/* 
 * 返回值: Item 
 * 功  能: 得到该项目对应的下一个项目,其实就是点后移一位
 */
Item Item::GetNext()
{
	Item it;
	it.SetLeft(this->left);
	it.SetRight(right);
	if (this->GetPointPos() == (this->GetRight()).length())
	{
		it.SetPointPos(pointPos);
	} 
	else
	{
		it.SetPointPos(pointPos + 1);
	}
	return it;
}

/* 
 * 返回值: char 
 * 功  能: 得到项目的左部非终结符
 */
char Item::GetLeft() const
{
	return left;
}

/* 
 * 返回值: void 
 * 功  能: 设置项目左部的非终结符
 */
void Item::SetLeft(char lf)
{
	this->left = lf;
}

/* 
 * 返回值: std::string 
 * 功  能: 得到项目的右部字符串
 */
std::string Item::GetRight() const
{
	return right;
}

/* 
 * 返回值: void 
 * 功  能: 设置项目的右部字符串
 */
void Item::SetRight(std::string right)
{
	this->right = right;
}

/* 
 * 返回值: int 
 * 功  能: 得到项目点的位置
 */
int Item::GetPointPos(void) const
{
	return pointPos;
}

/* 
 * 返回值: void 
 * 功  能: 设置项目点的位置
 */
void Item::SetPointPos(int pos)
{
	this->pointPos = pos;
}

/* 
 * 返回值: std::string 
 * 功  能: 得到向前推导串
 */
std::string Item::GetPreString(void) const
{
	return preString;
}

/* 
 * 返回值: void 
 * 功  能: 设置向前推导串
 */
void Item::SetPreString(std::string str)
{
	this->preString = str;
}

/* 
 * 返回值: bool 
 * 功  能: 重载==操作符
 */
bool Item::operator ==(const Item& other)
{
	return((other.GetLeft() == left) &&
		(!other.GetRight().compare(right)) &&
		(other.GetPointPos() == pointPos) &&
		(!other.GetPreString().compare(preString)));
}

/* 
 * 返回值: std::string 
 * 功  能: 得到项目对应的字符串
 */
std::string Item::ToString()
{
	std::string str;
	if(id > 10 )
	{
		str += char('0'+id/10);
		str += char('0'+id%10);
	}
	else
	{
		str += char('0'+id%10);
	}
	str += ":\t";
	str += left;
	str += "-->";
	std::string rightCopy(right);
	rightCopy.insert(pointPos, "·");
	str += rightCopy;

	str += ",";
	str += preString;
	return str;
}

/* 
 * 返回值: void 
 * 功  能: 设置项目id
 */
void Item::SetId(int ID)
{
	this->id = ID;
}

/* 
 * 返回值: bool 
 * 功  能: 得到项目是否被加入到项目集中
 */
bool Item::IsUsed()
{
	return bUsed;
}

/* 
 * 返回值: void 
 * 功  能: 设置项目是否被加入到项目集中
 */
void Item::SetUsed(bool used)
{
	bUsed = used;
}

/* 
 * 返回值: Formular 
 * 功  能: 得到项目对应的推导式
 */
Formular Item::GetFormular()
{
	Formular formular;
	formular.SetLeft(left);
	formular.SetRight(right);
	return formular;
}
