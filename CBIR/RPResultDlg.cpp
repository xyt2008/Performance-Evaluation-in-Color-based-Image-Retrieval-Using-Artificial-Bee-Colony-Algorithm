/******************************************************************
�ļ�����		RPResultDlg.cpp
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		��������Ļ���ʵ���ࡣ���к���RPResult��ĳ�Ա���󣬲�ʹ�����е�ֵ
��ÿ�μ����Ľ���Զ�ά���ߵ���ʽ���л��Ʋ����û�������
******************************************************************/

#include "stdafx.h"
#include "RPResultDlg.h"


// CRPResultDlg �Ի���

IMPLEMENT_DYNAMIC(CRPResultDlg, CDialog)

	CRPResultDlg::CRPResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRPResultDlg::IDD, pParent)
{

}

CRPResultDlg::~CRPResultDlg()
{
}

void CRPResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRPResultDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CRPResultDlg ��Ϣ�������

BOOL CRPResultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/************************************************************************************
��X���ϻ��Ƹ���λ�õ�����
************************************************************************************/
inline void drawX(int posX , int posY , CPaintDC* pDC , float value)
{
	char valueStr[12];
	char text[4];
	text[3] = 0;
	sprintf(valueStr , "%1.1f" , value);
	text[0] = valueStr[0];
	text[1] = valueStr[1];
	text[2] = valueStr[2];

	pDC->MoveTo(posX , posY);
	pDC->LineTo(posX , posY - 5);

	pDC->TextOut(posX - 10 , posY , text);
}

/************************************************************************************
��Y���ϻ��Ƹ���λ�õ�����
************************************************************************************/
inline void drawY(int posX , int posY , CPaintDC* pDC , float value)
{
	char valueStr[12];
	char text[4];
	text[3] = 0;
	sprintf(valueStr , "%1.1f" , value);
	text[0] = valueStr[0];
	text[1] = valueStr[1];
	text[2] = valueStr[2];

	pDC->MoveTo(posX , posY);
	pDC->LineTo(posX + 5 , posY);

	pDC->TextOut(posX - 25 , posY - 5 , text);
}

/************************************************************************************
���ƺ���
�ú�����Ҫ��ɶԻ��Ƹ��²�������Ҫ�������£�
1. ������Ӧ��X����Ϣ
2. ������Ӧ��Y����Ϣ
3. ���Ƴ�������µ�RP����
4. ���Ʒ�Ⱥ�����µ�RP����
5. ��������һЩ������Ϣ���������ʱ�ȣ�
************************************************************************************/
void CRPResultDlg::OnPaint()
{
	CPaintDC dc(this);

	CFont font;
	font.CreatePointFont(100, "΢���ź�");
	CFont *pOldFont = (CFont *)dc.SelectObject(&font);

	//���ñ���Ϊ͸��
	dc.SetBkMode(0);

	RECT clientRect;
	GetClientRect(&clientRect);

	POINT originPos;
	originPos.x = clientRect.left + 30;
	originPos.y = clientRect.bottom - 70;

	POINT size;
	size.x = (clientRect.right - clientRect.left) - 50;
	size.y = (clientRect.bottom - clientRect.top) - 80;

	dc.MoveTo(originPos.x , originPos.y);
	dc.LineTo(originPos.x + size.x , originPos.y);

	dc.MoveTo(originPos.x , originPos.y);
	dc.LineTo(originPos.x , originPos.y - size.y);

	drawX(originPos.x , originPos.y , &dc , 0.0f);
	drawX(originPos.x + size.x * 0.5 , originPos.y , &dc , 0.5f);
	drawX(originPos.x + size.x , originPos.y , &dc , 1.0f);
	drawY(originPos.x , originPos.y - size.y * 0.5f , &dc , 0.5f);
	drawY(originPos.x , originPos.y - size.y , &dc , 1.0f);

	dc.TextOut(clientRect.right - 100 , clientRect.bottom - 45 , "����-Recall");
	dc.TextOut(clientRect.right - 100 , clientRect.bottom - 25 , "����-Precision");

	// ���Ƴ��������µĽ��
	{
		CPen pen(PS_SOLID , 1 , RGB(255, 0, 0));
		CPen *pOldPen = dc.SelectObject(&pen);

		POINT curPos;
		float lastX , lastY;

		int valueX , valueY;
		// ������X���� 50ά�ȵ�����
		if(mRPResult.mRPDataList.GetListSize() > 0)
		{
			RPData* pRTData = mRPResult.mRPDataList.GetItemPtr(0);
			curPos.x = (LONG)(pRTData->recall * size.x + originPos.x);
			curPos.y = (LONG)(-pRTData->precision * size.y + originPos.y);
			dc.MoveTo(curPos.x , curPos.y);
			lastX = pRTData->recall;
			lastY = pRTData->precision;

			for(int i = 1 ; i < mRPResult.mRPDataList.GetListSize() ; i++)
			{
				if ((i % 20 == 0))
				{
					pRTData = mRPResult.mRPDataList.GetItemPtr(i);
					curPos.x = (int)(pRTData->recall * (float)size.x) + originPos.x;
					curPos.y = -(int)(pRTData->precision * (float)size.y) + originPos.y;
					dc.LineTo(curPos.x , curPos.y);

					valueX = (int)(pRTData->recall * 100.0f);
					valueY = (int)(pRTData->precision * 100.0f);

					dc.MoveTo(curPos.x , curPos.y);

					lastX = pRTData->recall;
					lastY = pRTData->precision;
				}
			}
		}

		dc.MoveTo(75 , clientRect.bottom - 15);
		dc.LineTo(120 , clientRect.bottom - 15);
		
		CString timeText;
		timeText.Format("ʱ�䣺%3.6f ��" , mRPResult.mUsedTime);
		dc.TextOut(160 , clientRect.bottom - 25 , timeText);
		dc.TextOut(15 , clientRect.bottom - 25 , "��������");

		dc.SelectObject(pOldPen);
	}

	// ���Ʒ�Ⱥ�����µĽ��
	{
		CPen pen(PS_SOLID , 1 , RGB(0, 0, 255));
		CPen *pOldPen = dc.SelectObject(&pen);

		POINT curPos;
		float lastX , lastY;

		int valueX , valueY;

		int xStep = mRPResultBee.mRPDataList.GetListSize() / 50;
		if(mRPResultBee.mRPDataList.GetListSize() > 0)
		{
			RPData* pRTData = mRPResultBee.mRPDataList.GetItemPtr(0);
			curPos.x = (LONG)(pRTData->recall * size.x + originPos.x);
			curPos.y = (LONG)(-pRTData->precision * size.y + originPos.y);
			dc.MoveTo(curPos.x , curPos.y);
			lastX = pRTData->recall;
			lastY = pRTData->precision;

			for(int i = 1 ; i < mRPResultBee.mRPDataList.GetListSize() ; i++)
			{
				if ((i % xStep == 0))
				{
					pRTData = mRPResultBee.mRPDataList.GetItemPtr(i);
					curPos.x = (int)(pRTData->recall * (float)size.x) + originPos.x;
					curPos.y = -(int)(pRTData->precision * (float)size.y) + originPos.y;
					dc.LineTo(curPos.x , curPos.y);

					valueX = (int)(pRTData->recall * 100.0f);
					valueY = (int)(pRTData->precision * 100.0f);

					dc.MoveTo(curPos.x , curPos.y);

					lastX = pRTData->recall;
					lastY = pRTData->precision;
				}
			}
		}

		dc.MoveTo(75 , clientRect.bottom - 35);
		dc.LineTo(120 , clientRect.bottom - 35);

		CString timeText;
		timeText.Format("ʱ�䣺%3.6f ��" , mRPResultBee.mUsedTime);
		dc.TextOut(160 , clientRect.bottom - 45 , timeText);
		dc.TextOut(15 , clientRect.bottom - 45 , "��Ⱥ����");

		dc.SelectObject(pOldPen);
	}
	dc.SelectObject(pOldFont);
}