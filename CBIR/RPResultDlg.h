/******************************************************************
�ļ�����		RPResultDlg.h
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		��������Ļ���ʵ���ࡣ���к���RPResult��ĳ�Ա���󣬲�ʹ�����е�ֵ
��ÿ�μ����Ľ���Զ�ά���ߵ���ʽ���л��Ʋ�������
******************************************************************/
#pragma once

#include "resource.h"
#include "RPResult.h"

class CRPResultDlg : public CDialog
{
	DECLARE_DYNAMIC(CRPResultDlg)

public:
	CRPResultDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRPResultDlg();

	// �Ի�������
	enum { IDD = IDD_RPRESULT };

	RPResult mRPResult;		// ��������µ�RP�������
	RPResult mRPResultBee;	// ��Ⱥ�㷨�����µ�RP�������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
};
