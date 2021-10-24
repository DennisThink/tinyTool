/************************************************************************
* copyright (c) 2013,计科09级02班 
* all rights reserved.
* 
* 文件名称: SourceCodeStream.h
* 摘    要: 读取分析串的类  
*            
*            
* 
* 当前版本：1.0
* 作    者：米彦辉
* 完成日期：2013年5月15日
************************************************************************/
#ifndef _SOURCE_CODE_STREAM_H_
#define _SOURCE_CODE_STREAM_H_
class SourceCodeStream
{
public:
	/*清空当前缓冲区的内容*/
	void Clear();
	/*构造函数*/
	SourceCodeStream();
	/*读取要分析的字符串*/
	bool ReadSourceCode();
	/*得到当前字符*/
	char GetCurrentChar();
	/*判断分析串是否结束*/
	bool isEnd();
private:
	char codeBuffer[100];//储存分析串的变量
	int length;			 //分析串的长度
	int index;			 //当前字符的下标
};
#endif