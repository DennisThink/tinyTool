/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: FileManager.h
* 摘    要: 用来读取Formular和符号集的类FileManger的头文件  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/

#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_
#include "FormularSet.h"	// Added by ClassView
#include "LabelSet.h"	// Added by ClassView
#include "ItemSet.h"
#include "ItemSetVector.h"

class CFileManager  
{
public:
	/*将Closure集写入文件*/
	void WriteItemSetVector(ItemSetVector itemSetVector);
	/*将项目集写入文件*/
	bool WriteItemSet(ItemSet itemSet);
	/*读取符号集*/
	LabelSet ReadLabelSet(const char* fileName);
	/*读取项目集*/
	FormularSet ReadFormularSet();
	/*构造函数*/
	CFileManager();
	/*析构函数*/
	virtual ~CFileManager();
};

#endif // !defined(AFX_FILEMANAGER_H__896AC32C_B9AD_44AC_9345_D8D7EC99BDA8__INCLUDED_)
