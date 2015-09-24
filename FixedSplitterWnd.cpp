/******************************************************************
�ļ�����		FixedSplitterWnd.cpp
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		MFC�ж��ڴ��ڷָ����ʵ�֡�����Ҫ���ܼ��Ƕ���MFC�ṹ�е�View��ͼ���зָ
�Ա�õ����Ŀ�������FormView�����Ҳ�Ļ�������View����������������������Ӧ��UI�ؼ���
�����Ҳ������Ӧ��ͼƬ���Ƶ�ʵ�֡�
******************************************************************/

#include "stdafx.h"
#include "FixedSplitterWnd.h"
    
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFixedSplitter::CFixedSplitter()   
{
	m_cxSplitter = 3;
	m_cySplitter = 3;
	m_cxSplitterGap = 3;
	m_cySplitterGap = 3;
}

CFixedSplitter::~CFixedSplitter()   
{

}

BEGIN_MESSAGE_MAP(CFixedSplitter, CSplitterWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SIZE() 
END_MESSAGE_MAP()

void CFixedSplitter::OnMouseMove(UINT nFlags, CPoint point)     
{
	CWnd::OnMouseMove(nFlags, point);   
}

void CFixedSplitter::OnLButtonDown(UINT nFlags, CPoint point)     
{
	CWnd::OnLButtonDown(nFlags, point);   
}

BOOL CFixedSplitter::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnSetCursor(pWnd, nHitTest, message);   
}

void CFixedSplitter::OnSize(UINT nType, int cx, int cy)
{
	CSplitterWnd::OnSize(nType, cx, cy);
}