/******************************************************************
�ļ�����		CBIR.cpp
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		MFC���̵Ŀ�����࣬ ��VS�Զ����ɡ���Ҫ���������������������ڵ�ά�������
******************************************************************/
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"

class CCBIRApp : public CWinApp
{
public:
	CCBIRApp();

public:
	/*
	�������� InitInstance
	���ܣ�	��ʼ��Ӧ�ó���
	���룺	��
	�����	��
	*/
	virtual BOOL InitInstance();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCBIRApp theApp;