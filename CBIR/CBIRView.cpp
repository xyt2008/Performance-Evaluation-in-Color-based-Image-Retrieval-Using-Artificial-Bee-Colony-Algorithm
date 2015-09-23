/******************************************************************
�ļ�����		CBIRView.cpp
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		�ര����ͼ���Ҳ���ƴ��ڵ�ʵ���ࡣ�Էָ�����Ҳ����ͼ���ڽ��п�������������а�����Ӧ��
���ڵĳ�ʼ�������ڴ��ڵ�OpenGL�豸�Ĵ�����ά����������ͼ�ϵ�һЩUI�����Լ���Ӧ�Ĳ�����Ӧ�������漰��������
���Լ�View��Ľ����ȣ���
******************************************************************/

#include "stdafx.h"
#include "CBIR.h"
#include "CBIRDoc.h"
#include "CBIRView.h"
#include "MainFrm.h"
#include "Image.h"
#include "Canvas.h"
#include <GL/glew.h>
#include "RPResult.h"
#include "Timer.h"

#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCBIRView, CView)

BEGIN_MESSAGE_MAP(CCBIRView, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_SHOWNORMAL, &CCBIRView::OnUpdateShownormal)
	ON_COMMAND(ID_SHOWNORMAL, &CCBIRView::OnShownormal)
	ON_UPDATE_COMMAND_UI(ID_SHOWBEECOLONY, &CCBIRView::OnUpdateShowbeecolony)
	ON_COMMAND(ID_SHOWBEECOLONY, &CCBIRView::OnShowbeecolony)
	ON_COMMAND(ID_SHOWDSR, &CCBIRView::OnShowdsr)
	ON_COMMAND(ID_RETRIEVAL, &CCBIRView::OnRetrieval)
	ON_COMMAND(ID_OUTPUTRESULT, &CCBIRView::OnOutputresult)
END_MESSAGE_MAP()

CCBIRView::CCBIRView()
{
	mpCBIRFormView = NULL;

	mpDstImage = NULL;

	mShowBeeResult = false;

	mpImageDsrDlg = new ImageDsrDlg;
	mpImageDsrDlg->Create(IDD_DSRDLG);

	mpRPResultDlg = new CRPResultDlg;
	mpRPResultDlg->Create(IDD_RPRESULT);

	mIsRetrived = false;
}

CCBIRView::~CCBIRView()
{
	if (mpDstImage)
	{
		delete mpDstImage;
	}

	if (mpImageDsrDlg)
	{
		delete mpImageDsrDlg;
	}

	if (mpRPResultDlg)
	{
		delete mpRPResultDlg;
	}
}

/************************************************************************************
����OpenGL��Ⱦ�豸�����ظ�ʽ��
��OpenGL��صĲ���
************************************************************************************/
bool CCBIRView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;

	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | 
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER |
		PFD_STEREO_DONTCARE;

	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	m_nGLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);
	if(m_nGLPixelIndex==0)
	{
		m_nGLPixelIndex = 1;
		if(DescribePixelFormat(hDC,m_nGLPixelIndex,
			sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
		{
			return FALSE;
		}
	}

	if(SetPixelFormat(hDC,m_nGLPixelIndex,&pixelDesc)==FALSE)
	{
		return FALSE;
	}

	return TRUE;	
}

/************************************************************************************
����OpenGL��Ⱦ�豸
��OpenGL��صĲ���
************************************************************************************/
bool CCBIRView::CreateRenderContext(HDC hDC)
{
	m_hRC = wglCreateContext(hDC);

	if(m_hRC == NULL)
	{
		return FALSE;
	}

	if(wglMakeCurrent(hDC,m_hRC)==FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCBIRView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

#ifdef _DEBUG
void CCBIRView::AssertValid() const
{
	CView::AssertValid();
}

void CCBIRView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/************************************************************************************
���õ�ǰOpenGL��Ⱦ���ڵ�ͶӰģʽ
��OpenGL��صĲ���
************************************************************************************/
void CCBIRView::SetViewOrtho(int width , int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0 , width , 0 , height , -10.0f , 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0 , 0.0f , 100.0f , 0.0 , 0.0 , 0.0 , 0.0 , 1.0 , 0.0);
	glShadeModel(GL_SMOOTH);

	mCanvas.SetSize(width , height);
}

void CCBIRView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	//store the size of the screen
	m_screenWidth = cx;
	m_screenHeight = cy;

	if(cy > 0)
	{
		if((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
		{
			RedrawWindow();
		}

		m_oldRect.right = cx;
		m_oldRect.bottom = cy;

		SetViewOrtho(cx , cy);

		glDrawBuffer(GL_BACK);
		glEnable(GL_DEPTH_TEST);
	}

	if(mpCBIRFormView)
	{
		mpCBIRFormView->InvalidateRect(NULL , false);
	}

	InvalidateRect(NULL , false);
}

int CCBIRView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();

	if(SetWindowPixelFormat(pDC->m_hDC) == FALSE)
	{
		return 0;
	}

	if(CreateRenderContext(pDC->m_hDC) == FALSE)
	{
		return 0;
	}

	CRect rect;
	GetClientRect(rect);

	SetViewOrtho(rect.Width() , rect.Height());

	glewInit();

	return 0;
}

void CCBIRView::OnDraw(CDC* /*pDC*/)
{

}

BOOL CCBIRView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	TRACE("CCBIRView::OnMouseWheel\n");
	mCanvas.Scroll(zDelta);

	InvalidateRect(NULL , false);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

/************************************************************************************
�����뵱ǰ���ڹ���Form���ڵ�ָ��
************************************************************************************/
CCBIRFormView* CCBIRView::GetFormViewPointer()
{
	return NULL;
}

/************************************************************************************
���Ʋ���
��Ҫ����һ��������ͼƬǽ�����Ļ���
************************************************************************************/
void CCBIRView::OnPaint()
{
	CPaintDC dc(this);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.8f , 0.8f , 0.8f , 1.0f);

	glPushMatrix();

	if (mShowBeeResult)
	{
		mCanvas.Draw(mSortedListBeeColony);
	}
	else
	{
		mCanvas.Draw(mSortedListNormal);
	}

	glPopMatrix();

	glFlush();

	SwapBuffers(dc.m_ps.hdc);
}

/************************************************************************************
��ʼ�����ݿ�
Ӧ�ó���ʼִ�к�������Ҫ��������Ҫ��ͼ��⣬��Ҫ�����¼���������
1. ɨ�赱ǰӦ�ó����µ�ͼ���Ŀ¼���������е�ͼ��ȫ����ӵ�ͼ���б�
2. ����ʼ�����ͼ���б���м��أ������ڴ���Դ棩��
3. ��ȡͼ���б��е�����ͼ�������ֵ��
4. ����ʼ����ͼ���ӵ���ʼ�������б��У���δִ�м��������˳����ԭʼ���е�˳��һ�£�
************************************************************************************/
void CCBIRView::InitDatabase(vector<string>& imageList)
{
	int testImageNums = imageList.size();
	int i = 0;
	for (vector<string>::iterator ite = imageList.begin() ; ite != imageList.end() ; ite++)
	{
		Image imageItem;
		imageItem.InitImage(ite->c_str() , i);

		if (i < testImageNums)
		mImageList.AddItem(imageItem);

		i++;
	}
	
	// ����ͼƬ�⵽�ڴ���
	//for(int i = 0 ; i < mImageList.GetListSize() ; ++i)
	for(int i = 0 ; i < testImageNums ; ++i)
	{
		mImageList.GetItemPtr(i)->LoadImage();
	}

	// ��ȡͼƬ����Ӧ������ֵ
	for(int i = 0 ; i < testImageNums ; ++i)
	{
		mImageList.GetItemPtr(i)->ExtractDescriptor();
	}

	// ��ͼƬ���뵽Sorted List��
	for(int i = 0 ; i < testImageNums ; ++i)
	{
		Image* pImage = mImageList.GetItemPtr(i);
		mSortedListNormal.AddItem(pImage);
		mSortedListBeeColony.AddItem(pImage);
	}
}

/************************************************************************************
����Ŀ��ͼ��
1. ���ò�������Ŀ��ͼ�������
2. ����ͼ��ľ������ݣ�����OpenGL����Ĵ���������ֵ����ȡ�ȣ�
************************************************************************************/
void CCBIRView::LoadDstImage(char* fileAddress , int index , bool globalPath)
{
	if(fileAddress)
	{
		if (mpDstImage)
		{
			delete mpDstImage;
		}
		mpDstImage = new Image;

		mpDstImage->InitImage(fileAddress , index);

		if (globalPath)
		{
			mpDstImage->LoadOutImage();
		}
		else
		{
			mpDstImage->LoadImage();
		}
		
		mpDstImage->ExtractDescriptor();

		mpImageDsrDlg->SetSrcImage(mpDstImage);
	}
}

/************************************************************************************
ִ�г���ļ���
1. ����������Ⱦ������
2. ����ͼ����е�ÿ��ͼ�񣬲�������Ŀ��ͼ���������������ƥ�䣻
3. �ڼ�������б��У��������Ƴ����������
������Ϊ��ͳ��Ч�ʣ��ڼ�����ʼǰ���ü�ʱ��������ʼ��ʱ�����������֮��ֹͣ��ʱ�����õ���Ӧ�ĺ�ʱ
************************************************************************************/
double CCBIRView::PerformNormRetrieval()
{
	if (mpDstImage)
	{
		CPerfCounter timeCounter;
		timeCounter.Start();

		// �������������
		mCanvas.Reset();

		// ��Ŀ��ͼƬ�����ݿ��б��е�ͼƬ����һһƥ��
		for (unsigned int i = 0 ; i < mImageList.GetListSize() ; ++i)
		{
			mImageList.GetItemPtr(i)->Match(mpDstImage);
		}

		// ��������б�
		mSortedListNormal.EmptyList();

		// ��ƥ������Distance��С���������������ʹ�ñȽϱ�����N*N����������͹�Է�Ⱥ�㷨���ٶ�����^-^��
		double minDistance;
		int minImageIndex;
		for (unsigned int i = 0 ; i < mImageList.GetListSize() ; ++i)
		{
			minDistance = 999999.0f;
			minImageIndex = 0;

			// ������������ǰ��ƥ�����ߣ�������С����ͼƬ����������б���Ա����ظ�����
			for (unsigned int j = 0 ; j < mImageList.GetListSize() ; ++j)
			{
				if(mImageList.GetItemPtr(j)->IsFlagged() == false)
				{
					if (mImageList.GetItemPtr(j)->GetDistance() < minDistance)
					{
						minImageIndex = j;
						minDistance = mImageList.GetItemPtr(j)->GetDistance();
					}
				}
			}

			// ����ǰ�������ͼƬ���뵽�б��У����޸�����ֵ����ֵ����ÿ��Match����ʱ���ã�
			Image* pImage = mImageList.GetItemPtr(minImageIndex);
			mSortedListNormal.AddItem(pImage);
			mImageList.GetItemPtr(minImageIndex)->SetFlagged();

			TRACE("Distance: %f\n" , minDistance);
		}

		InvalidateRect(NULL , FALSE);

		timeCounter.Stop();

		return timeCounter.GetElapsedTime();
	}

	return 0.0;
}

/************************************************************************************
ִ�г���ļ���
1. ����������Ⱦ������
2. ʹ�÷�Ⱥ�����㷨����BeeColonyAlgo����ִ�м�������
���еļ�������������صĲ������ɷ�Ⱥ�㷨������ά����������Ϊ��ͳ��Ч�ʣ��ڼ�����ʼǰ���ü�ʱ����
����ʼ��ʱ�����������֮��ֹͣ��ʱ�����õ���Ӧ�ĺ�ʱ
************************************************************************************/
double CCBIRView::PerformBeeRetrieval()
{
	if (mpDstImage)
	{
		CPerfCounter timeCounter;
		timeCounter.Start();

		// �������������
		mCanvas.Reset();

		mBeeColony.PerformRetrieval(mpDstImage , mImageList , mSortedListBeeColony);

		InvalidateRect(NULL , FALSE);

		timeCounter.Stop();
		return timeCounter.GetElapsedTime();
	}

	return 0.0;
}

/************************************************************************************
������������RP����
************************************************************************************/
void CCBIRView::ComputeRPResult(RPResult* pRPResult , bool beeColony)
{
	if (pRPResult)
	{
		if (beeColony)
		{
			pRPResult->ComputeRP(mpDstImage->GetImageIndex() , mSortedListBeeColony);
		}
		else
		{
			pRPResult->ComputeRP(mpDstImage->GetImageIndex() , mSortedListNormal);
		}
	}
}

/************************************************************************************
��Ϣ��Ӧ���������³���˵�
************************************************************************************/
void CCBIRView::OnUpdateShownormal(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(!mShowBeeResult);
}

/************************************************************************************
��Ϣ��Ӧ���������³���˵�
************************************************************************************/
void CCBIRView::OnShownormal()
{
	// TODO: Add your command handler code here
	mShowBeeResult = false;

	InvalidateRect(NULL , false);
}

/************************************************************************************
��Ϣ��Ӧ���������³���˵�
************************************************************************************/
void CCBIRView::OnUpdateShowbeecolony(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(mShowBeeResult);
}

/************************************************************************************
��Ϣ��Ӧ���������³���˵�
************************************************************************************/
void CCBIRView::OnShowbeecolony()
{
	// TODO: Add your command handler code here
	mShowBeeResult = true;

	InvalidateRect(NULL , false);
}

/************************************************************************************
��Ϣ��Ӧ���������³���˵�
************************************************************************************/
void CCBIRView::OnShowdsr()
{
	// TODO: Add your command handler code here
	mpImageDsrDlg->ShowWindow(SW_SHOW);
}

/************************************************************************************
������Ӧ����
ִ�м�����ť�������ļ������������ڲ���ͬʱִ����������������
1. �������
2. ��Ⱥ����
************************************************************************************/
void CCBIRView::PerformRetrieval()
{
	if(mpDstImage == NULL)
	{
		MessageBoxA("����ѡ��Ŀ��ͼƬ��" , "��ʾ" , MB_OK);
		return;
	}

	// ���ϱ����жϣ���ֹ�����Ż��㷨��ʧ����������
	bool isValidResult;

	// ִ�г���ı�������
	isValidResult = false;
	while(isValidResult == false)
	{
		mpRPResultDlg->mRPResult.mUsedTime = (float)PerformNormRetrieval();
		ComputeRPResult(&(mpRPResultDlg->mRPResult) , false);
		isValidResult = mpRPResultDlg->mRPResult.mbIsValid;
	}

	// ִ�з�Ⱥ����
	isValidResult = false;
	while(isValidResult == false)
	{
		mpRPResultDlg->mRPResultBee.mUsedTime = (float)PerformBeeRetrieval();
		ComputeRPResult(&(mpRPResultDlg->mRPResultBee) , true);
		isValidResult = mpRPResultDlg->mRPResultBee.mbIsValid;
	}

	mpRPResultDlg->InvalidateRect(NULL , true);

	mIsRetrived = true;
}

/************************************************************************************
��Ϣ��Ӧ����
************************************************************************************/
void CCBIRView::OnRetrieval()
{
	// TODO: Add your command handler code here
	PerformRetrieval();
}

void CCBIRView::OutputResult()
{
	if (mpDstImage == NULL || 
		mIsRetrived == false)
	{
		return;
	}

	ofstream outFile;

	char fileName[32];
	sprintf_s(fileName , sizeof(char) * 32 , "�������_%d.txt" , mpDstImage->GetImageIndex());
	outFile.open(fileName);

	/** �����������Ľ�� **/
	outFile<<"�������"<<endl;
	{
		// �����������е�ģ��˳�����ǰ100��ͼ�񼴿�
		outFile<<"Index: ";
		for(int i = 0 ; i < 100 ; ++i)
		{
			outFile<<(*mSortedListNormal.GetItemPtr(i))->GetImageIndex()<<" ";
		}
		outFile<<endl;

		// �����������е�PR���
		mpRPResultDlg->mRPResult.OutputResult(outFile);
	}
	
	/** �����Ⱥ�����Ľ�� **/
	outFile<<"��Ⱥ����"<<endl;
	{
		// �����������е�ģ��˳�����ǰ100��ͼ�񼴿�
		outFile<<"ͼ���б�";
		for(int i = 0 ; i < 100 ; ++i)
		{
			outFile<<(*mSortedListBeeColony.GetItemPtr(i))->GetImageIndex()<<" ";
		}
		outFile<<endl;

		// �����������е�PR���
		mpRPResultDlg->mRPResultBee.OutputResult(outFile);
	}

	outFile.close();
}


void CCBIRView::OnOutputresult()
{
	// TODO: Add your command handler code here
	OutputResult();
}
