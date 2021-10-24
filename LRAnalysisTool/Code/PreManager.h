/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: PreManager.h
* 摘    要: 分析过程的帮助类，帮助实现action表和goto表，以及first集，follow集  
*           还有从follow到Item的转换
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/

#ifndef _PRE_MANAGER_H_
#define _PRE_MANAGER_H_

#pragma warning(disable:4786)
#include "Formular.h"
#include "FormularSet.h"
#include "ItemSet.h"
#include "Item.h"
#include "LabelSet.h"
#include "RelationSet.h"
#include "ItemSetVector.h"
#include "ActionTable.h"
#include "GotoTable.h"
class PreManager
{
public:
	/*完成从项目集生成LR(0)action和goto表的过程*/
	void ClosureToTable(FormularSet frSet,ItemSetVector closure,LabelSet terminal,LabelSet noTerminal,
	ActionTable& actionTable, GotoTable& gotoTable);

	/*求follow集*/
	void CreateFollowSet(FormularSet forSet, RelationSet firstSet,
		RelationSet& followSet, LabelSet terminalSet, LabelSet noTerminal);

	/*求first集*/
	void CreateFirstSet(FormularSet forSet, RelationSet& firstSet,
		LabelSet terminalSet, LabelSet noTerminal);
	/*从项目集生成LR(0)的闭包*/
	ItemSetVector GetItemSetClosure(ItemSet& itemSet, LabelSet& noTerminal);//求LR(0)的Closure
	/*从推导式集生成项目集*/
	void FormularSetToItemSet(FormularSet forSet, ItemSet& itemSet);
	/*生成推导式对应的项目集，一个推导式对应多个项目，LR(0)版*/
	void FormularToItem(Formular formular, ItemSet& itemSet);
	PreManager();
	virtual ~PreManager();
};

#endif // !defined(AFX_PREMANAGER_H__D499130A_66C7_4953_A5DB_FA8476D1DD69__INCLUDED_)
