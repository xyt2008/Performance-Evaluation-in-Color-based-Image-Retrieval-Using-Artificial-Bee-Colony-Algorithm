/******************************************************************
�ļ�����		MainFrm.h
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		�����MFC����࣬��VS�Զ����ɡ�
******************************************************************/

#include "FixedSplitterWnd.h"

#include "CBIRFormView.h"
#include "CBIRView.h"

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	CFixedSplitter m_wndSplitterFrame;	// �Ӵ��ڷָ�������
	int m_formWidth;					// ���Form��ͼ�Ŀ��

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


