/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: Manager.h
* 摘    要: LR总控程序的实现类  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "AnalysisStack.h"
#include "SourceCodeStream.h"
#include "ActionTable.h"
#include "GotoTable.h"
#include "FormularSet.h"
#include "FileManager.h"
#include "PreManager.h"
/*分析过程的动作类型*/
enum ActionType
{
	SHIFT_IN,//移进
	REDUCE,//归约
	SUCCEED,//成功
	FAILED//失败
};
/*用户选择类型*/
enum UserChoiceType
{
    READFILE=1,//读取推导式，终结符，非终结符
	SAVEDATA,//保存中间数据
	PRINTDATA,//打印中间数据
	ANALYSIS,//分析字符串
	DECLARETION,//程序声明
	CLEARSCREEN,//清屏
	QUIT//退出
};
class Manager
{
public:
	/*程序分析函数*/
	void Analysis();
	/*改变分析动作方向*/
	int ChangeDirection();
	/*构造函数*/
	Manager();
	/*读取分析表*/
	int ReadAnalysisTable();
	/*分析过程动作函数*/
	void Action();
	/*输出所有数据*/
	void PrintAllData();
	/*析构函数*/
	~Manager();
private:
	void ShiftIn();//移进
	void Reduce();//归约
private:
	ActionTable actionT;	//分析程序的动作表
	GotoTable gotoT;		//分析程序状态转换表
	PreManager preManager;	//辅助分析的类成员
	AnalysisStack stack;	//分析栈
	SourceCodeStream sourceCode;//源程序流
	FormularSet formularSet;//推导式集合
	char curChar;//当前字符
	int curState;//当前状态
	int curDirect;//当前行动
	LabelSet noTerminal;//非终结符
	LabelSet terminal;//终结符
	ItemSetVector itemSetVector;//项目集的集合
	ItemSet itemSet;//项目集
public:
	void PressAnyKeyToReturn();
	void QuitProgram();
	void OnPrintDataChoice();
	int PrintDataChoiceMenu();
	void SaveAnalysisData();
	void OnUserChoice();
	bool bPrepared;
	/*读入相关文件，并生成actionh和goto表*/
	void ReadRelatedFile();
	void PrintDeclaration();
	/*打印菜单信息*/
	int PrintMenu();
	bool bContinue;//是否继续
	int formularIndex;//推导式索引
};
#endif