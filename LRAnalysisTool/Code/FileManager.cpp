/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: FileManager.cpp
* 摘    要: FileManager的实现文件  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/

#pragma warning(disable:4786)
#include <iostream>
//#include <windows.h>
#include <fstream>
#include <string.h>
#include "FileManager.h"
#include "FormularSet.h"
#include "LabelSet.h"
#include <unistd.h>
using namespace std;


CFileManager::CFileManager()
{

}

CFileManager::~CFileManager()
{

}

/* 
 * 返回值: FormularSet 
 * 功  能: 读取推导式集合
 */
FormularSet CFileManager::ReadFormularSet()
{
	FormularSet frSet;
	
	std::string strFileName;
	cout<<endl;
	cout<<"\t请输入存储推导式的文件名:";
	cin>>strFileName;
	ifstream file;
	file.open(strFileName);
	if(file)
	{
		cout<<"\t打开文件成功，开始读取......"<<endl;
		char buffer[100]={0};
		while (file.getline(buffer, 100))
		{
			Formular formular(buffer);
			frSet.AddFormular(formular);
			memset(buffer,0,100);
		}
		file.close();
		cout<<"\t";
		for (int i = 0 ; i < 62 ; i++)
		{
			cout<<"*";
			usleep(50);
		}
		cout<<endl<<"\t文件读取完成!"<<endl<<endl;
	} 
	else
	{
		cout<<"\t对不起，程序无法读取文件"<<endl<<endl;
	}
	
	return frSet;

}

/* 
 * 返回值: LabelSet 
 * 功  能: 读取符号集，即终结符集和非终结符集
 */
LabelSet CFileManager::ReadLabelSet(const char* fileName)
{
	LabelSet result;
	
	ifstream file;
	file.open(fileName);
	if(file)
	{
		cout<<"\t打开文件成功，开始读取..."<<endl;
		char buffer[100]={0};
		string str;
		while (file.getline(buffer, 100))
		{
			str += buffer[0];
		}
		result.ReadLabelSet(str.c_str(),str.length());
		file.close();

		cout<<"\t";
		for (int i = 0 ; i < 62 ; i++)
		{
			cout<<"*";
			usleep(50);
		}
		cout<<endl<<"\t文件读取完成!"<<endl<<endl;
	}
	else
	{
		cout <<"\t 对不起，程序无法打开文件" << endl<<endl;
	}
	return result;
	
	/*CStdioFile file;
	if (file.Open(fileName, CFile::modeRead))
	{
		cout<<"\t打开文件成功，开始读取..."<<endl;
		char buffer[100];
		string str;
		while (file.ReadString(buffer, 10))
		{
			str += buffer[0];
		}
		result.ReadLabelSet(str.c_str(),str.length());
		file.Close();

		cout<<"\t";
		for (int i = 0 ; i < 62 ; i++)
		{
			cout<<"*";
			Sleep(50);
		}
		cout<<endl<<"\t文件读取完成!"<<endl<<endl;
	}
	else
	{
		cout <<"\t 对不起，程序无法打开文件" << endl<<endl;
	}
	return result;*/
}

bool CFileManager::WriteItemSet(ItemSet itemSet)
{
	return false;
	char fileName[50];
	ofstream file;
	int choice = 0;
	cout<<"\t请输入保存项目集文件的名字:";
	cin>>fileName;
	file.open(fileName);
	while (!file)
	{
		file.close();
		cout<<"\t请输入保存项目集文件的名字:";
		cin>>fileName;
		file.open(fileName);
	}
	string fileStr(fileName);
	if (fileStr.find_first_of(".txt",0) > fileStr.length())
	{
		fileStr.append(".txt");
	}
	file.open(fileStr.c_str());
	std::string strHead = "这是保存项目集的文件\n\r";
	file.write(strHead.c_str(),strHead.length());
	cout<<"\t开始保存项目集到文件:"<<endl;
	cout<<"\t";
	for (int j = 0 ; j < 62 ; j++)
	{
		cout<<"*";
		usleep(50);
	}
	cout<<endl;
	for(int i = 0 ; i < itemSet.GetItemCount() ; i++)
	{
		string str  = itemSet.GetItem(i).ToString();
		str +="\n\r";
		file.write(str.c_str(),str.length());
	}
	cout<<"\t项目集保存成功"<<endl;
	file.close();
	return true;
}

void CFileManager::WriteItemSetVector(ItemSetVector itemSetVector)
{
	char fileName[50];
	ifstream file;
	cout<<endl;
	cout<<"\t请输入保存Closure文件的名字:";
	cin>>fileName;
	file.open(fileName);
	while (file)
	{
		cout<<"\t请输入保存Closure文件的名字:";
		cin>>fileName;
		file.open(fileName);
	}
	string fileStr(fileName);
	if (fileStr.find_first_of(".txt",0) > fileStr.length())
	{
		fileStr.append(".txt");
	}
	ofstream ofile;
	ofile.open(fileStr);
	std::string strHead = "这是保存Closure集的文件\n\r";
	ofile.write(strHead.c_str(),strHead.length());
	cout<<"\t开始保存CLOSURE集到文件:"<<endl;
	cout<<"\t";
	for (int j = 0 ; j < 62 ; j++)
	{
		cout<<"*";
		usleep(50);
	}
	cout<<endl;
	for (int i = 0 ; i < itemSetVector.GetItemSetCount() ; i++)
	{
		ItemSet iset = itemSetVector.GetItemSet(i);
		string iSetStr;
		iSetStr += 'I';
		iSetStr += char('0'+iset.GetId());
		iSetStr += "\n\r";
		ofile.write(iSetStr.c_str(),iSetStr.length());
		for (int j = 0 ; j < iset.GetItemCount() ; j++)
		{
			string str = iset.GetItem(j).ToString();
			str +="\n\r";
			ofile.write(str.c_str(),str.length());
		}
		ofile.write("\n\r",2);
	}
	ofile.close();
	cout<<"\t保存CLOSURE集成功"<<endl;
	return ;
}
