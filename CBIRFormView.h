/******************************************************************
�ļ�����		CBIRFormView.cpp
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		�ര����ͼ������������Form�����ͼ��ʵ���ࡣ�Էָ��������Form���ڽ��п�������������а�����Ӧ��
UI�ؼ�������Ӧ�Ĳ�����Ӧ�������漰�������ݿ��Լ�View��Ľ����ȣ�
******************************************************************/

#pragma once
#include "afxcmn.h"

#include "RPResultDlg.h"

class CCBIRView;
class COperationDlg;

class CCBIRFormView : public CFormView
{
	DECLARE_DYNCREATE(CCBIRFormView)

private:
	int mWidth , mHeight;	// Form��ͼ�Ŀ�͸�

public:	
	HTREEITEM m_allRoot;	// ��״�ؼ����Ӧ�Ķ���
	HTREEITEM m_curRoot;

	CCBIRView* mpCBIRView;	// �������Ҳ������ͼ��ָ��

	CTreeCtrl mImageDBTree;	// ��״MFC�ؼ�����

	LPPICTURE mpImagePPI;	// GDI��ͼ�����������������ͼ�е�Ԥ��ͼ���

	int mSelectedIndex;		// ��ǰѡ�е�ͼ�������������ڽ��м���ʱ������ȡ��Ӧ��Ŀ��ͼ��

public:
	/*
	��������	GetViewPointer
	���ܣ�	��ȡ�뵱ǰForm��ͼ��������Ҳ������ͼ�Ķ���ָ��
	���룺	��
	�����	������ͼ��ָ��
	*/
	CCBIRView* GetViewPointer();

	/*
	��������	AddFile
	���ܣ�	����µ�ͼ����󵽵�ǰ�����ݿ��б���
	���룺	����1���¶����Ӧ��ID�����ⲿ�����������ƣ�������2���¶��������
	�����	��
	*/
	void AddFile(int index , CString& fileName);

private:
	/*
	�������� ScanDatabase
	���ܣ�	�ڳ�ʼ��ʱ���ã�����ɨ�赱ǰӦ�ó���Ŀ¼�µ�ͼ����ļ��У��������е�ͼ��ȫ����ӵ�����ͼ����ڴ���
	���룺	��
	�����	��
	*/
	void ScanDatabase();

	/*
	��������	GetSelectedItemIndex
	���ܣ�	�õ���ǰ�����״����ؼ���ѡ�е�ͼ����������ֵ��������ֵ�������ض�Ӧ��ͼ���Լ�����ͼ������
	���룺	��״�ؼ��б�ѡ�е�����
	�����	ѡ�е�ͼ����������
	*/
	int GetSelectedItemIndex(TVITEM& selectedItem);

protected:
	CCBIRFormView();
	virtual ~CCBIRFormView();

public:
	enum { IDD = IDD_CBIRFORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	/*
	��������	OnInitalUpdate
	���ܣ�	�ڳ��δ���FormView֮����øú���������������Ӧ�ؼ��ĸ��£����������صĻ���
	���룺	��
	�����	��
	*/
	virtual void OnInitialUpdate();

	/*
	���º�����ΪMFC�е���Ϣ��Ӧ��������Ӧ�Ĺ�����ʵ�ֿ��Դ������ж���
	*/
public:
	afx_msg void OnTcnSelchangeOperationtab(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedOpenmodeldb();
public:
	afx_msg void OnTvnSelchangedModeldbtree(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedRetriveimage();
	afx_msg void OnBnClickedPrresult();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnUpdateShownormal(CCmdUI *pCmdUI);
	afx_msg void OnShownormal();
	afx_msg void OnUpdateShowbeecolony(CCmdUI *pCmdUI);
	afx_msg void OnShowbeecolony();
	afx_msg void OnShowdsr();
	afx_msg void OnRetrieval();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOutputresult();
	afx_msg void OnOutputresult();
};


