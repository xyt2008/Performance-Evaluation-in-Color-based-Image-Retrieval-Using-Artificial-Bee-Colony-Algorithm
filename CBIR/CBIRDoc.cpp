/******************************************************************
�ļ�����		CBIRDoc.cpp
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		MFC���̵��ĵ����࣬ ��VS�Զ����ɡ���Ҫ����Ӧ�ó���������ĵ������л��Ȳ���
******************************************************************/

#include "stdafx.h"
#include "CBIR.h"

#include "CBIRDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCBIRDoc, CDocument)

BEGIN_MESSAGE_MAP(CCBIRDoc, CDocument)
END_MESSAGE_MAP()

CCBIRDoc::CCBIRDoc()
{

}

CCBIRDoc::~CCBIRDoc()
{
}

/************************************************************************************
����MFC�Ŀ⺯�������µ��ĵ���
************************************************************************************/
BOOL CCBIRDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}
/************************************************************************************
����MFC�Ŀ⺯�������л���ǰ����ĵ���
************************************************************************************/
void CCBIRDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}

#ifdef _DEBUG
void CCBIRDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCBIRDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif