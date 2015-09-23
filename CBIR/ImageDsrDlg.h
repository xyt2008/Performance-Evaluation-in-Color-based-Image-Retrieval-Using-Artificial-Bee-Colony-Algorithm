/******************************************************************
�ļ�����		ImageDsrDlg.h
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		��Ҫ����ʵ��ͼ������ֵ��MFC��Dialog�ؼ��еĻ��Ʋ�����
******************************************************************/
#pragma once

#include "resource.h"
#include "RPResult.h"
#include "Image.h"

class ImageDsrDlg : public CDialog
{
	DECLARE_DYNAMIC(ImageDsrDlg)

public:
	ImageDsrDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ImageDsrDlg();

	// �Ի�������
	enum { IDD = IDD_DSRDLG };

	Image* mpSrcImage;	// Ŀ��ͼ���ָ��

public:
	/*
	��������	SetSrcImage
	���ܣ�	���õ�ǰ����ֵ���Ƶ�Ŀ��ͼ��
	���룺	����1��Ŀ��ͼ��Ķ���ָ��
	�����	��
	*/
	void SetSrcImage(Image* pSrcImage);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
};
