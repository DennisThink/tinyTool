/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: PreManager.cpp
* 摘    要:   
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/

#include "PreManager.h"
#include "ActionTable.h"
#include "GotoTable.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PreManager::PreManager()
{
}

PreManager::~PreManager()
{
}

/* 
 * 返回值: void 
 * 功  能: 对于一个推导式，生成相应的项目集
 */
void PreManager::FormularToItem(Formular formular, ItemSet& itemSet)
{
	for (int i = 0 ; i < formular.GetRightLength() + 1 ; i++)
	{
		Item item;
		item.SetLeft(formular.GetLeft());
		item.SetRight(formular.GetRight());
		item.SetPointPos(i);
		itemSet.AddItem(item);
	}
}

/* 
 * 返回值: void 
 * 功  能: 生成推导式集对应的项目集
 */
void PreManager::FormularSetToItemSet(FormularSet forSet, ItemSet& itemSet)
{
	for (int i = 0 ; i < forSet.GetFormularCount() ; i++)
	{
		Formular formular = forSet.GetFormular(i);
		FormularToItem(formular, itemSet);
	}
}

/*
*	得到项目集的LR(0)闭包
*/
ItemSetVector PreManager::GetItemSetClosure(ItemSet& itemSet, LabelSet& noTerminal)
{
	ItemSetVector itemVector;
	for (int i = 0 ; i < itemSet.GetItemCount() ; i++)
	{
		ItemSet iSet;
		Item item = itemSet.GetItem(i);
		//加入第一个项目
		if (!(item.IsUsed()))
		{
			item.SetUsed(true);
			itemSet.AddItem(item);
			iSet.AddItem(item);
		}
		//如果存在S-->·BB,S-->B·B,则将B-->·b等加入含有S-->·BB,S-->B·B的项目集中
		for (int m = 0 ; m < iSet.GetItemCount() ; m++)
		{
			Item sItem = iSet.GetItem(m);
			if (sItem.GetPointPos() != (sItem.GetRight().length() + 1))//点不在最后
			{
				char afterPoint = sItem.GetRight()[sItem.GetPointPos()];
				if (noTerminal.IsInLabelSet(afterPoint))//点后面的字符为非终结符
				{
					//得到以B开始的项目集
					ItemSet bItemSet = itemSet.GetItemSet(afterPoint);
					//如果·在最前面则加入项目集
					for (int k = 0 ; k < bItemSet.GetItemCount() ; k++)
					{
						Item addItem = bItemSet.GetItem(k);
						if (addItem.GetPointPos() == 0)
						{
							addItem.SetUsed(true);
							itemSet.AddItem(addItem);
							iSet.AddItem(addItem);
						}
					}
				}
			}
		}
		if (iSet.GetItemCount() != 0)
		{
			itemVector.AddItemSet(iSet);
		}
	}
	return itemVector;
}

/*
*	通过推导式，终结符集，和非终结符集构造First集，
*/
void PreManager::CreateFirstSet(FormularSet forSet, RelationSet& firstSet,
	LabelSet terminalSet, LabelSet noTerminal)
{
	for (int index = 0 ; index < forSet.GetFormularCount() ; index ++)
	{
		//对于形如E-->*F的推导式，将*加入first(E)
		for (int i = 0 ; i < noTerminal.GetLabelSetCharCount() ; i++)
		{
			char ch = noTerminal.GetLabelChar(i);
			Relation rela;
			rela.SetStart(ch);

			//得到以ch为左部的推导式的集合
			FormularSet relaSet = forSet.GetFormularSet(ch);
			for (int j = 0 ; j < relaSet.GetFormularCount() ; j++)
			{
				Formular formular = relaSet.GetFormular(j);
				char first = formular.GetRightOfIndex(0);
				if (terminalSet.IsInLabelSet(first))
				{
					rela.AddRightChar(first);
				}
			}
			firstSet.AddRelation(rela);
		}

		//如果E-->F+T,则First(F)加入到first(E),变量以e,f开头的代表的便是推导式中的E,F
		for (int j = 0 ; j < noTerminal.GetLabelSetCharCount() ; j++)
		{
			char eChar = noTerminal.GetLabelChar(j);
			FormularSet eForSet = forSet.GetFormularSet(eChar);
			for (int k = 0 ; k < eForSet.GetFormularCount() ; k++)
			{
				char fChar = eForSet.GetFormular(k).GetRightOfIndex(0);
				if (noTerminal.IsInLabelSet(fChar))
				{
					Relation eRela = firstSet.GetRelation(eChar);
					Relation fRela = firstSet.GetRelation(fChar);
					string fStr = fRela.GetRightString();
					for (std::size_t m = 0 ; m < fStr.length() ; m++)
					{
						eRela.AddRightChar(fStr[m]);
					}
					firstSet.AddRelation(eRela);
				}
			}
		}
	}
}

/*
*	通过推导式，first集，终结符集，非终结符集来构造follow集
*/
void PreManager::CreateFollowSet(FormularSet forSet, RelationSet firstSet,
	RelationSet& followSet, LabelSet terminalSet, LabelSet noTerminal)
{
	//置开始符号的Follow为#
	Relation rela;
	Formular formular = forSet.GetFormular(0);
	rela.SetStart(formular.GetLeft());
	rela.AddRightChar('#');
	followSet.AddRelation(rela);
	//将所有的非终结符加入到follow集中
	for (int i = 0 ; i < noTerminal.GetLabelSetCharCount() ; i++)
	{
		Relation relation;
		relation.SetStart(noTerminal.GetLabelChar(i));
		followSet.AddRelation(relation);
	}
	//完善follow集
	for (int index = 0 ; index < forSet.GetFormularCount() ; index++)
	{
		//若含有F-->(E)的，则将)加入follow(E),以e开头的都代表E
		for (int j = 0 ; j < forSet.GetFormularCount() ; j++)
		{
			Formular formu = forSet.GetFormular(j);
			string forStr = formu.GetRight();
			for (std::size_t k = 0 ; k < forStr.length() - 1 ; k++)
			{
				char eChar = forStr[k];
				char afterE = forStr[k + 1];
				//相邻的两个符号满足前一个为非终结符，且后一个为终结符
				if (noTerminal.IsInLabelSet(eChar) &&
					terminalSet.IsInLabelSet(afterE))
				{
					Relation eRela = followSet.GetRelation(eChar);
					eRela.AddRightChar(afterE);
					followSet.AddRelation(eRela);
				}
			}
		}
		//若有E-->TF将first(F)加入follow(T),以t，f开头的就代表推导式中的T,F
		//对每个非终结符进行遍历
		for (int i = 0 ; i < forSet.GetFormularCount() ; i++)
		{
			Formular formular = forSet.GetFormular(i);
			string forRight = formular.GetRight();
			for (std::size_t j = 0 ; j < forRight.length() - 1 ; j++)
			{
				//若两个相邻的字符均为非终结符，即TF相邻
				char tChar = forRight[j];
				char fChar = forRight[j + 1];

				if (noTerminal.IsInLabelSet(tChar) &&
					noTerminal.IsInLabelSet(fChar))
				{
					Relation tRela = followSet.GetRelation(tChar);
					Relation fRela = firstSet.GetRelation(fChar);
					//将first(F)加入Follow(t)
					string fStr = fRela.GetRightString();
					for (std::size_t k = 0 ; k < fStr.length() ; k++)
					{
						tRela.AddRightChar(fStr[k]);
					}
					followSet.AddRelation(tRela);
				}
			}
		}

		//若含有E-->+TF,则将follow(F)加入到follow(E)
		for (int k = 0 ; k < noTerminal.GetLabelSetCharCount() ; k++)
		{
			char noTerChar = noTerminal.GetLabelChar(k);
			for (int j = 0 ; j < forSet.GetFormularCount() ; j++)
			{
				Formular formul = forSet.GetFormular(noTerChar);
				string forRight = formul.GetRight();
				char tail = forRight[forRight.length()];
				Relation rela = followSet.GetRelation(tail);
				Relation tailRela = followSet.GetRelation(noTerChar);
				for (int m = 0 ; m < rela.GetRightCount() ; m++)
				{
					rela.AddRightChar(tailRela.GetRightString()[m]);
				}
			}
			followSet.AddRelation(rela);
		}
	}
}

/* 
 * 返回值: void 
 * 功  能: 通过推导式集，项目集，终结符集合非终结符集生成LR(0)的action表和goto表
 */
void PreManager::ClosureToTable(FormularSet frSet,ItemSetVector closure,LabelSet terminal,LabelSet noTerminal,
	ActionTable& actionTable, GotoTable& gotoTable)
{
	//初始化actionTable和GotoTable
	int itemSetCount = closure.GetItemSetCount();
	int noTerCount = noTerminal.GetLabelSetCharCount();
	int terCount = terminal.GetLabelSetCharCount();
	actionTable.InitializeTableData(itemSetCount,terCount);
	gotoTable.InitializeData(itemSetCount,noTerCount);
	actionTable.SetConvertMap(terminal.GetConvertMap());
	gotoTable.SetConvertMap(noTerminal.GetConvertMap());

	//构造gotoTable
	//遍历项目集,只向后遍历
	for (int itemSetIndex = 0 ; itemSetIndex < itemSetCount ; itemSetIndex++ )
	{
		//得到要遍历的项目集
		ItemSet curItemSet = closure.GetItemSet(itemSetIndex);
		int itemCount = curItemSet.GetItemCount();
        //对项目集进行遍历，
		for (int itemIndex = 0 ; itemIndex < itemCount ; itemIndex++)
		{
			//得到当前项目，若点之后的为非终结符，则进行gotoTable的填写
			Item curItem = curItemSet.GetItem(itemIndex);
			char afterPoint = curItem.GetRight()[curItem.GetPointPos()];
			//是非终结符
			if (noTerminal.IsInLabelSet(afterPoint))
			{
				//得到下一个项目
				Item nextItem = curItem.GetNext();
				for (int nextItemSetIndex = itemSetIndex ; nextItemSetIndex < itemSetCount ; nextItemSetIndex ++ )
				{
					ItemSet nextItemSet = closure.GetItemSet(nextItemSetIndex);	
					if (nextItemSet.IsInItemSet(nextItem))
					{
						gotoTable.SetData(afterPoint,curItemSet.GetId(),nextItemSet.GetId());
						break;
					}
				}
			}
		}
	}
	//gotoTable构造完毕

	//构造actionTable
	//遍历项目集集合
	for(int curSetIndex = 0 ; curSetIndex < closure.GetItemSetCount() ; curSetIndex++)
	{
		//得到项目集
		ItemSet curItemSet = closure.GetItemSet(curSetIndex);
		int curItemCount = curItemSet.GetItemCount();
		//对项目集进行遍历
		for (int curIndex = 0 ; curIndex < curItemCount ; curIndex++)
		{
			//得到项目
			Item curItem = curItemSet.GetItem(curIndex);
			//得到点后的符号
			char afterPoint = curItem.GetRight()[curItem.GetPointPos()];
			//如果是终结符
			if (terminal.IsInLabelSet(afterPoint))
			{
				//得到下一个项目
				Item nextItem = curItem.GetNext();
				//对剩余项目集进行遍历
				for (int nextSetIndex = curSetIndex ; nextSetIndex < closure.GetItemSetCount() ; nextSetIndex++)
				{
					ItemSet nextSet = closure.GetItemSet(nextSetIndex);
					if (nextSet.IsInItemSet(nextItem))
					{
						TableElem elem;
						elem.Elem='S';
						elem.state=nextSet.GetId();
						actionTable.SetData(curItemSet.GetId(),afterPoint,elem);
						break;
					}
				}
			}
			//如果点在最后,且不是S'-->S
			else if (curItem.GetPointPos() == curItem.GetRight().length() && (curItem.GetLeft() != frSet.GetFormular(0).GetLeft()))
			{
				//得到当前项目对应的推导式
				Formular formular = curItem.GetFormular();
				int forIndex = frSet.GetFormularIndex(formular);
				TableElem elem;
				elem.Elem='R';
				elem.state=forIndex;
				for (int k = 0 ; k < terminal.GetLabelSetCharCount() ; k++)
				{
					actionTable.SetData(curItemSet.GetId(),terminal.GetLabelChar(k),elem);
				}
			}
			//点在最后，且是S'-->S
			else if (curItem.GetPointPos() == curItem.GetRight().length() && (curItem.GetLeft() == frSet.GetFormular(0).GetLeft()))
			{
				TableElem elem;
				elem.Elem='A';
				elem.state=0;
				actionTable.SetData(curItemSet.GetId(),'#',elem);
			}
		}
	}
	//actionTable构造完毕
	//actionTable.PrintAllData();
}
