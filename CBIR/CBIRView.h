/******************************************************************
�ļ�����		CBIRView.cpp
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		�ര����ͼ���Ҳ���ƴ��ڵ�ʵ���ࡣ�Էָ�����Ҳ����ͼ���ڽ��п�������������а�����Ӧ��
���ڵĳ�ʼ�������ڴ��ڵ�OpenGL�豸�Ĵ�����ά����������ͼ�ϵ�һЩUI�����Լ���Ӧ�Ĳ�����Ӧ�������漰��������
���Լ�View��Ľ����ȣ���
******************************************************************/

#pragma once

#include "CBIRDoc.h"
#include <GL/glew.h>
#include "Image.h"
#include "FuncsLib.h"
#include "Canvas.h"
#include "RPResult.h"
#include "BeeColonyAlgo.h"
#include "ImageDsrDlg.h"
#include "RPResultDlg.h"

#include <vector>
using namespace std;

class CCBIRFormView;

class CCBIRView : public CView
{
protected:
	CCBIRView();
	DECLARE_DYNCREATE(CCBIRView)

public:
	CCBIRDoc* GetDocument() const;

public:
	HGLRC m_hRC;						// OpenGL�豸�����Ķ���			
	int m_nGLPixelIndex;				// OpenGL����ģʽ����
	CRect m_oldRect;					// ��Ⱦ�����Ƕ���

	CCBIRFormView* mpCBIRFormView;		// ��ǰ��ͼ������Form��ͼ����

	int m_screenWidth , m_screenHeight;	// ���ڵĿ���

	List<Image> mImageList;				// ͼ����Ӧ��ͼƬ�б�

	bool mShowBeeResult;				// ��ʾ������
	List<Image*> mSortedListNormal;		// �����������б�
	List<Image*> mSortedListBeeColony;	// ��Ⱥ�㷨��������б�

	Image* mpDstImage;					// Ŀ��ͼ��ָ��

	Canvas mCanvas;						// ����ͼƬǽ��������

	BeeColonyAlgo mBeeColony;			// ��Ⱥ�㷨����

	ImageDsrDlg* mpImageDsrDlg;			// ͼ������ֵ���ƶԻ������

	CRPResultDlg* mpRPResultDlg;		// �������RP���߻��ƶ���

	bool mIsRetrived;					// ��ǵ�ǰ�Ƿ�����˼�������

public:
	/*
	��������	SetWindowPixelFormat
	���ܣ�	����OpenGL��Ⱦ�����ĵ�����ģʽ
	���룺	����1����ǰ���ڵ��豸DC���
	�����	bool�����ý���ɹ����
	*/
	bool SetWindowPixelFormat(HDC hDC);

	/*
	��������	CreateRenderContext
	���ܣ�	����OpenGL��Ⱦ�豸
	���룺	����1����ǰ���ڵ��豸DC���
	�����	bool����������ɹ����
	*/
	bool CreateRenderContext(HDC hDC);

	/*
	��������	GetFormViewPointer
	���ܣ�	�õ��뵱ǰ���ڹ�����Form���ڵ�ָ��
	���룺	��
	�����	CCBIRFormView*��������Form��ͼ�����ָ��
	*/
	CCBIRFormView* GetFormViewPointer();

	/*
	��������	SetViewOrtho
	���ܣ�	���õ�ǰOpenGL���ڵ�ͶӰģʽ
	���룺	����1��2����ǰ���ڵĿ��
	�����	��
	*/
	void SetViewOrtho(int width , int height);

	/*
	��������	PerformNormRetrieval
	���ܣ�	�Ե�ǰ��ͼ�����ݿ�ִ�г���ļ�������
	���룺	��
	�����	double��ִ�м��������ĺ�ʱ
	*/
	double PerformNormRetrieval();

	/*
	��������	PerformBeeRetrieval
	���ܣ�	�Ե�ǰ��ͼ�����ݿ�ִ�з�Ⱥ�㷨�ļ�������
	���룺	��
	�����	double��ִ�м��������ĺ�ʱ
	*/
	double PerformBeeRetrieval();

public:
	/*
	��������	InitDatabase
	���ܣ�	��ʼ��ͼ�����ݿ�
	���룺	����1��ͼ�������б�
	�����	��
	*/
	void InitDatabase(vector<string>& imageList);

	/*
	��������	LoadDstImage
	���ܣ�	����ָ����ͼ����Ϊ��ǰ������Ŀ��ͼ��
	���룺	����1��Ŀ��ͼ��ĵ�ַ������2��Ŀ��ͼ�������������3�����ȫ�ֻ�ֲ�·��
	�����	��
	*/
	void LoadDstImage(char* fileAddress , int index = 0 , bool globalPath = false);

	/*
	��������	PerformRetrieval
	���ܣ�	�Ե�ǰ��Ŀ��ͼ�������ݿ���ִ�м���������������������ͷ�Ⱥ������
	���룺	��
	�����	��
	*/
	void PerformRetrieval();

	/*
	��������	ComputeRPResult
	���ܣ�	���㵱ǰ�������RP���ߣ����������������
	���룺	����1��Ҫ���صļ�������Ķ���ָ�룻����2��ͳ�Ʒ�Ⱥ�㷨�򳣹��㷨
	�����	��
	*/
	void ComputeRPResult(RPResult* pRPResult , bool beeColony);

	/*
	*/
	void OutputResult();

	/*
	����Ϊ��Ӧ��MFC�¼�������
	*/
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual ~CCBIRView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnPaint();
protected:
	virtual void OnDraw(CDC* /*pDC*/);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
	afx_msg void OnUpdateShownormal(CCmdUI *pCmdUI);
	afx_msg void OnShownormal();
	afx_msg void OnUpdateShowbeecolony(CCmdUI *pCmdUI);
	afx_msg void OnShowbeecolony();
	afx_msg void OnShowdsr();
	afx_msg void OnRetrieval();
	afx_msg void OnOutputresult();
};

struct threadInfo
{
	bool completed;
	CCBIRView* wndPraent;
};

#ifndef _DEBUG
inline CCBIRDoc* CCBIRView::GetDocument() const
   { return reinterpret_cast<CCBIRDoc*>(m_pDocument); }
#endif

