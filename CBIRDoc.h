/******************************************************************
�ļ�����		CBIRDoc.cpp
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		MFC���̵��ĵ����࣬ ��VS�Զ����ɡ���Ҫ����Ӧ�ó���������ĵ������л��Ȳ�����
******************************************************************/

#pragma once

class CCBIRDoc : public CDocument
{
protected: 
	CCBIRDoc();
	DECLARE_DYNCREATE(CCBIRDoc)

public:

public:

public:
	/*
	��������		OnNewDocument
	���ܣ�		���µ��ⲿ�ļ�
	���룺		��
	�����		BOOL���ͣ���ʼ���Ľ���Ƿ�ɹ�
	*/
	virtual BOOL OnNewDocument();

	/*
	��������		Serialize
	���ܣ�		�����л�����ʽ���浱ǰ���ĵ�
	���룺		��
	�����		��
	*/
	virtual void Serialize(CArchive& ar);

public:
	virtual ~CCBIRDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


