/******************************************************************
�ļ�����		ImageDsrDlg.cpp
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		��Ҫ����ʵ��ͼ������ֵ��MFC��Dialog�ؼ��еĻ��Ʋ�����
******************************************************************/

#include "stdafx.h"
#include "ImageDsrDlg.h"


// CRPResultDlg �Ի���

IMPLEMENT_DYNAMIC(ImageDsrDlg, CDialog)

	ImageDsrDlg::ImageDsrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ImageDsrDlg::IDD, pParent)
{
	mpSrcImage = NULL;
}

ImageDsrDlg::~ImageDsrDlg()
{
}

void ImageDsrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ImageDsrDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CRPResultDlg ��Ϣ�������

BOOL ImageDsrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/************************************************************************************
��X���ϻ��Ƹ���λ�õ�����
************************************************************************************/
inline void drawX(int posX , int posY , CPaintDC* pDC , int value)
{
	char text[32];
	sprintf_s(text , sizeof(char) * 32 , "%d" , value);

	pDC->MoveTo(posX , posY);
	pDC->LineTo(posX , posY - 5);

	if (value == 256)
	{
		pDC->TextOut(posX - 25 , posY , text);
	}
	else
	{
		pDC->TextOut(posX - 10 , posY , text);
	}
}

/************************************************************************************
��Y���ϻ��Ƹ���λ�õ�����
************************************************************************************/
inline void drawY(int posX , int posY , CPaintDC* pDC , float value)
{
	char text[32];
	sprintf_s(text , sizeof(char) * 32 , "%1.1f" , value);

	pDC->MoveTo(posX , posY);
	pDC->LineTo(posX + 5 , posY);

	pDC->TextOut(posX - 20 , posY , text);
}

/************************************************************************************
���ƺ���
�ú�����Ҫ��ɶԻ��Ƹ��²�������Ҫ�������£�
1. ������Ӧ��X����Ϣ
2. ������Ӧ��Y����Ϣ
3. ��ͼ���ԭʼ����ֵ���в�ֵϸ��
4. ���Ʋ�ֵ��ÿ��ά�ȴ�һ����ֵ
************************************************************************************/
void ImageDsrDlg::OnPaint()
{
	if (mpSrcImage == NULL)
	{
		return;
	}

	CPaintDC dc(this);

	CFont font;
	font.CreatePointFont(100, "΢���ź�");
	CFont *pOldFont = (CFont *)dc.SelectObject(&font);

	//���ñ���Ϊ͸��
	dc.SetBkMode(0);

	RECT clientRect;
	GetClientRect(&clientRect);

	POINT originPos;
	originPos.x = clientRect.left + 25;
	originPos.y = clientRect.bottom - 30;

	POINT size;
	size.x = (clientRect.right - clientRect.left) - 25;
	size.y = (clientRect.bottom - clientRect.top) - 80;

	dc.MoveTo(originPos.x , originPos.y);
	dc.LineTo(originPos.x + size.x , originPos.y);
	dc.MoveTo(originPos.x , originPos.y);
	dc.LineTo(originPos.x , clientRect.top);

	drawX(originPos.x , originPos.y , &dc , 0);
	drawX(originPos.x + size.x * 0.5 , originPos.y , &dc , 128);
	drawX(originPos.x + size.x , originPos.y , &dc , 256);
	drawY(originPos.x , clientRect.top + (clientRect.bottom - clientRect.top - 30) * 0.5 , &dc , 0.5);
	drawY(originPos.x , clientRect.top , &dc , 1.0);

	dc.TextOut(clientRect.right - 80 , clientRect.top , "����-ά��");
	dc.TextOut(clientRect.right - 80 , clientRect.top + 20 , "����-����ֵ");

	{
		CPen pen(PS_SOLID , 1 , RGB(0, 0, 255));
		CPen *pOldPen = dc.SelectObject(&pen);

		float dsrValue[256 + 3 + 9];
		int dsrIndex = 0;
		for (int i = 0 ; i < 256 ; ++i)
		{
			dsrValue[dsrIndex++] = mpSrcImage->GetHistogramValue(i);
		}
		for (int i = 0 ; i < 3 ; ++i)
		{
			dsrValue[dsrIndex++] = mpSrcImage->GetHistogramValue(i);
		}
		for (int i = 0 ; i < 9 ; ++i)
		{
			dsrValue[dsrIndex++] = mpSrcImage->GetHistogramValue(i);
		}

		//int intDimSize = 76 * 5 - 4;
		//float intDsrValue[1024];
		//InterpolateDsrValue(dsrValue , intDsrValue , 76);

		int intDimSize = 256 + 3 + 9;
		int step = 1;//size.x / 220;

		int xPos = 2;
		for (int i = 0 ; i < intDimSize ; ++i)
		{
			dc.MoveTo(originPos.x + xPos , originPos.y);
			dc.LineTo(originPos.x + xPos , originPos.y - dsrValue[i] * size.y * 30);
			xPos += step + 1;
		}

		dc.SelectObject(pOldPen);
	}
	dc.SelectObject(pOldFont);
}

/************************************************************************************
����Ŀ��ͼ���ָ��
************************************************************************************/
void ImageDsrDlg::SetSrcImage(Image* pSrcImage)
{
	mpSrcImage = pSrcImage;

	InvalidateRect(NULL , TRUE);
}