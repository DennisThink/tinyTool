/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: Manager.cpp
* 摘    要: 分析程序总控类的实现  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/
#pragma warning(disable:4786)
#include "Manager.h"
//#include <conio.h>
#include <iostream>
using namespace std;

/* 
* 返回值: void 
* 功  能: 根据相应的动作做出反应
*/
void Manager::Action()
{
	switch (curDirect)
	{
	case SHIFT_IN:
		//移进
		{
			STACKELEM elem;
			elem.Elem = curChar;
			elem.state = curState;
			stack.Push(elem);
			curChar = sourceCode.GetCurrentChar();
			curDirect = ChangeDirection();
			std::cout<<"移进"<<std::endl;
		}
		break;
	case REDUCE:
		//归约
		{
			Formular formular = formularSet.GetFormular(formularIndex);
            STACKELEM elem;
			elem.Elem=formular.GetLeft();
			elem.state = formular.GetRightLength();
			stack.Reduce(elem.state);
			curState = gotoT.GetData(elem.Elem, stack.GetTop().state);
			elem.state = curState;
			stack.Push(elem);
			curDirect = ChangeDirection();
			std::cout<<"归约"<<std::endl;
			
		}break;
	case SUCCEED:
		{
			cout<<endl;
			cout<<"\t分析结果:匹配成功"<<endl;
			bContinue = false;
		}break;
	case FAILED:
		{
			cout<<endl;
			cout<<"\t分析结果: 匹配失败"<<endl;
			bContinue = false;
		}break;
	default:
		{
			cout<<endl;
			cout<<"\t发生未知错误，程序退出"<<endl;
		}break;
	}
}

/* 
* 返回值: void 
* 功  能: 构造函数，完成了推导式集，动作表和状态转换表的初始化
*/
Manager::Manager()
{
	bPrepared = false;
	bContinue = true;

	//输出屏幕的设置
	system("mode con:cols=80 lines=2000");
	system("COLOR 0A");
}

Manager::~Manager()
{
	
}
/* 
* 返回值: int 
* 功  能: 根据相关信息，改变分析方向
*/
int Manager::ChangeDirection()
{
	TableElem elem = actionT.GetData(curState, curChar);
	char ch = elem.Elem;
	switch (ch)
	{
	case 'S':
		{
			curState = elem.state;
			return SHIFT_IN;
		}break;
	case 'R':
		{
			formularIndex = elem.state;
			return REDUCE ;
		}break;
	case 'A':
		{
			return SUCCEED;
		}break;
	case 'E':
		{
			return FAILED;
		}break;
	default:
		{
			return -1;
		}break;
	}
}

/* 
* 返回值: void 
* 功  能: 分析函数
*/
void Manager::Analysis()
{
	if (bPrepared)
	{
		cout<<endl;
		cout<<"\t请输入要分析的字符串，并以#开头和结尾:";
		sourceCode.ReadSourceCode();
		stack.Clear();
		curChar = sourceCode.GetCurrentChar();
		curState = stack.GetTop().state;
		curDirect = ChangeDirection();
		while (!sourceCode.isEnd() && bContinue)
		{
			Action();
		}
		cout<<endl;
		cout<<"\t字符串分析完成"<<endl;
		bContinue = true;
	} 
	else
	{
		cout<<endl;
		cout<<"\t请读入推导式，终结符，非终结符之后再进行分析"<<endl;
	}
	PressAnyKeyToReturn();
}

int Manager::PrintMenu()
{
	cout<<endl;
	int choice=0;
	cout<<"\t**************************************************************"<<endl;
	cout<<"\t*     这是一个简单的语法分析程序，以下是选项，请选择:        *"<<endl;
	cout<<"\t**************************************************************"<<endl;
	cout<<"\t* 1   读入推导式，终结符，非终结符                           *"<<endl;
	cout<<"\t* 2   保存项目集，Closure,以及Action和Goto表                 *"<<endl;
	cout<<"\t* 3   输出中间数据                                           *"<<endl;
	cout<<"\t* 4   分析字符串                                             *"<<endl;
	cout<<"\t* 5   程序声明，联系作者                                     *"<<endl;
	cout<<"\t* 6   清屏                                                   *"<<endl;
	cout<<"\t* 7   退出程序                                               *"<<endl;
	cout<<"\t**************************************************************"<<endl;
	while((choice < 1) || (choice > 7))
	{
		cout<<endl<<"\t请输入您的选择:";
		cin >> choice;
		if (choice == 0)
		{
			cout<<"\t亲，您输入的不是整数，下次注意噢"<<endl;
			cin.clear();
			cin.ignore();
		}
	}
	return choice;
}

void Manager::PrintDeclaration()
{
	cout<<endl<<endl;
	cout<<"\t**************************************************************"<<endl;
	cout<<"\t*                     程序声明与联系作者                     *"<<endl;
	cout<<"\t**************************************************************"<<endl;
	cout<<"\t*  本程序由西安工程大学计算机科学与技术09级02班的米彦辉编写    *"<<endl;
	cout<<"\t*   算法思路参考西安电子科技大学胡元义编著<编译原理>第二版     *"<<endl;
	cout<<"\t*  程序完成于2013年05月24是毕业设计的题目  指导老师:王春玲     *"<<endl;
	cout<<"\t*  程序更新于2019年03月14于北京昌平                          *"<<endl;
	cout<<"\t*          程序完全是由本人编写，绝无抄袭，特此声明            *"<<endl;                        
    cout<<"\t*   如果您对我的程序有任何的建议或者意见，希望您可以联系我     *"<<endl;
	cout<<"\t*              QQ:2475027682  TEL:18301413762             *"<<endl;
	cout<<"\t*     博客地址  https://www.dennisthink.com                *"<<endl;
	cout<<"\t*              最后，谢谢您的使用，谢谢您的支持              *"<<endl;
	cout<<"\t*           谨以此纪念我四年的大学生活和我的青春岁月          *"<<endl;
	cout<<"\t**************************************************************"<<endl;
	cout<<endl<<endl;
	PressAnyKeyToReturn();
}

/* 
* 返回值: void 
* 功  能: 读取推导式，终结符，非终结符，以及生成action和goto
*/

void Manager::ReadRelatedFile()
{
	CFileManager fileManager;
	formularSet=fileManager.ReadFormularSet();
	
	if (formularSet.GetFormularCount()==0)
	{
		return ;
	} 
	else
	{
		//读取非终结符号
		cout<<"\t请输入非终结符号集文件名:";
		char noTerFile[50];
		cin>>noTerFile;
    	noTerminal = fileManager.ReadLabelSet(noTerFile);
		if (noTerminal.GetLabelSetCharCount()==0)
		{
			return;
		}
		else
		{
			//读取终结符号
			cout<<"\t请输入终结符号集文件名:";
			char terFile[50];
			cin>>terFile;
        	terminal = fileManager.ReadLabelSet(terFile);
		}
	}

	//开始准备ActionTable与GotoTable
	PreManager preManager;
	preManager.FormularSetToItemSet(formularSet,itemSet);
	itemSetVector = preManager.GetItemSetClosure(itemSet,noTerminal);
	//得到actionTable和GotoTable
	preManager.ClosureToTable(formularSet,itemSetVector,terminal,noTerminal,actionT,gotoT);
	
	//设置准备好变量为ture
	if ((formularSet.GetFormularCount() == 0) ||(terminal.GetLabelSetCharCount()==0)||(noTerminal.GetLabelSetCharCount()==0))
	{
		bPrepared = false;
	} 
	else
	{
		bPrepared = true;
	}
	PressAnyKeyToReturn();
}

void Manager::OnUserChoice()
{
	bool bNotExit = true;
	int times = 0;
	while(bNotExit)
	{
		int choice = PrintMenu();
		switch(choice)
		{
		case READFILE:
			{
				ReadRelatedFile();
			}break;
		case SAVEDATA:
			{
				SaveAnalysisData();
			}break;
		case PRINTDATA:
			{
				OnPrintDataChoice();
			}break;
		case ANALYSIS:
			{
				Analysis();
			}break;
		case DECLARETION:
			{
				PrintDeclaration();
			}break;
		case CLEARSCREEN:
			{
				system("cls");
				PressAnyKeyToReturn();
			}break;
		case QUIT:
			{
				bNotExit = false;
				QuitProgram();
				PressAnyKeyToReturn();
			}break;
		default:
			{
				choice = 0;
				PressAnyKeyToReturn();
			}break;
		}
	}
}

/* 
* 返回值: void 
* 功  能: 保存相关的数据
*/
void Manager::SaveAnalysisData()
{
	CFileManager fileManager;
	fileManager.WriteItemSet(itemSet);
	fileManager.WriteItemSetVector(itemSetVector);
	PressAnyKeyToReturn();
}

/*打印所有数据*/
void Manager::PrintAllData()
{
	
}

/*打印数据的菜单选项*/
int Manager::PrintDataChoiceMenu()
{
	int choice = 0;
	cout<<"\t**************************************************************"<<endl;
	cout<<"\t*        这是输出中间数据子菜单，请选择要输出的数据：        *"<<endl;
	cout<<"\t**************************************************************"<<endl;
	cout<<"\t*  1  输出推导式集合                                         *"<<endl;
	cout<<"\t*  2  输出终结符集                                           *"<<endl;
	cout<<"\t*  3  输出非终结符集                                         *"<<endl;
	cout<<"\t*  4  输出项目集                                             *"<<endl;
	cout<<"\t*  5  输出Closure集                                          *"<<endl;
	cout<<"\t*  6  输出ActionTable                                        *"<<endl;
	cout<<"\t*  7  输出GotoTable                                          *"<<endl;
	cout<<"\t*  8  返回上级菜单                                           *"<<endl;
	cout<<"\t**************************************************************"<<endl;
	while ((choice < 1)||( choice > 8))
	{
		cout<<"\t请输入您的选择:";
		cin >> choice;
		if (choice ==0)
		{
			cout<<"\t亲，您输入的不是整数，下次注意噢"<<endl;
			cin.clear();
			cin.ignore();
		}
	}
	return choice;
}

void Manager::OnPrintDataChoice()
{
	if (!bPrepared)
	{
		cout<<endl;
		cout<<"\t请先读入推导式，终结符和非终结符"<<endl;
		return ;
	} 
	else
	{
		bool bGoOn = true;
		while (bGoOn)
		{
			int choice = PrintDataChoiceMenu();
			switch(choice)
			{
			case 1:
				{
					formularSet.PrintAllData();
				}break;
			case 2:
				{
					terminal.PrintAllData();
				}break;
			case 3:
				{
					noTerminal.PrintAllData();
				}break;
			case 4:
				{
					itemSet.PrintAllData();
				}break;
			case 5:
				{
					itemSetVector.PrintAllData();
				}break;
			case 6:
				{
					actionT.PrintAllData();
				}break;
			case 7:
				{
					gotoT.PrintAllData();
				}break;
			case 8:
				{
					bGoOn = false;
				}break;
			default:
				{
					bGoOn = false;
				}break;
			}//swith end
			PressAnyKeyToReturn();
		}//while end
	}//else end
	PressAnyKeyToReturn();
}

/* 
 * 返回值: void 
 * 功  能: 程序退出
 */
void Manager::QuitProgram()
{
	cout<<endl;
	cout<<"\t**************************************************************"<<endl;
	cout<<"\t*                   感谢您的使用，再见                       *"<<endl;
	cout<<"\t**************************************************************"<<endl;
}

void Manager::PressAnyKeyToReturn()
{
	cout<<"\t请按任意键返回:";
	getchar();
	cout<<endl<<endl;
}
