/******************************************************************
�ļ�����		FixedSplitterWnd.cpp
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		MFC�ж��ڴ��ڷָ����ʵ�֡�����Ҫ�����Ƕ���MFC�ṹ�е�View��ͼ���зָ
�Ա�õ����Ŀ�������FormView�����Ҳ�Ļ�������View����������������������Ӧ��UI�ؼ���
�����Ҳ������Ӧ��ͼƬ���Ƶ�ʵ�֡�
******************************************************************/

#if !defined(AFX_FIXEDSPLITTER_H__9DB2BF9B_1C2E_483F_985E_FC38AB087B05__INCLUDED_)   
#define AFX_FIXEDSPLITTER_H__9DB2BF9B_1C2E_483F_985E_FC38AB087B05__INCLUDED_   
    
#if _MSC_VER > 1000   
#pragma once   
#endif
    
class CFixedSplitter : public CSplitterWnd   
{
public:   
	CFixedSplitter();
    
public:   
	virtual ~CFixedSplitter();
protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);   
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);   
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()   
};   

#endif