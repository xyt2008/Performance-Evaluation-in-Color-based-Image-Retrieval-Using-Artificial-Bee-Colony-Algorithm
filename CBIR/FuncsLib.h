/******************************************************************
�ļ�����		FuncsLib.h
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		������һЩ���õĺ�����ĵ�ʵ�֡�
******************************************************************/

#ifndef __FuncsLib_H__
#define __FuncsLib_H__

#include <string>
using namespace std;
#include "DataStruct.h"

#pragma warning (disable: 4251)

/*
������������Ҫʵ���˶�ĳһĿ¼�������ļ����б�����ɨ��
*/
class  FuncsLib
{
public:
	static bool     IsFileExist(const char* pFileName , bool throwErrorMsg);
	static bool     IsFolderExistA(const char* pFolderString);
	static void     FindFiles(char* pDstPath , char* pDstFilePostfix , List<string>* pDstStringList , bool isRecursive);
	static void     ListDstFilesInFolder(char* pDstFloderString , char* pDstFilePostfix , List<string>* pDstStrList , bool isRecursive = false);
	static bool     FindDstFile(char* pDstFolder , char* pDstFileName , char* pDstFileFullPath , bool isRecursive);
};

#endif // __FuncsLib_H__