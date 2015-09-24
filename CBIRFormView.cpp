/******************************************************************
�ļ�����		CBIRFormView.cpp
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		�ര����ͼ������������Form�����ͼ��ʵ���ࡣ�Էָ��������Form���ڽ��п�������������а�����Ӧ��
UI�ؼ�������Ӧ�Ĳ�����Ӧ�������漰�������ݿ��Լ�View��Ľ����ȣ�
******************************************************************/

#include "stdafx.h"
#include "CBIR.h"
#include "CBIRFormView.h"
#include "MainFrm.h"
#include "FuncsLib.h"
#include "RPResultDlg.h"
#include <algorithm>
#include <vector>
#include <fstream>

using namespace std;

IMPLEMENT_DYNCREATE(CCBIRFormView, CFormView)

CCBIRFormView::CCBIRFormView() : CFormView(CCBIRFormView::IDD)
{
	mpImagePPI = NULL;
	mSelectedIndex = 0;
}

CCBIRFormView::~CCBIRFormView()
{
	if(mpImagePPI)
	{
		mpImagePPI->Release();
	}
}

/************************************************************************************
MFC�ؼ�����ĳ�Ա��������Ҫʵ�ֿؼ����ݵĸ���
************************************************************************************/
void CCBIRFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODELDBTREE, mImageDBTree);
}

BEGIN_MESSAGE_MAP(CCBIRFormView, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MODELDBTREE, &CCBIRFormView::OnTvnSelchangedModeldbtree)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RETRIVEIMAGE, &CCBIRFormView::OnBnClickedRetriveimage)
	ON_BN_CLICKED(IDC_PRRESULT, &CCBIRFormView::OnBnClickedPrresult)
	ON_UPDATE_COMMAND_UI(ID_SHOWNORMAL, &CCBIRFormView::OnUpdateShownormal)
	ON_COMMAND(ID_SHOWNORMAL, &CCBIRFormView::OnShownormal)
	ON_UPDATE_COMMAND_UI(ID_SHOWBEECOLONY, &CCBIRFormView::OnUpdateShowbeecolony)
	ON_COMMAND(ID_SHOWBEECOLONY, &CCBIRFormView::OnShowbeecolony)
	ON_COMMAND(ID_SHOWDSR, &CCBIRFormView::OnShowdsr)
	ON_COMMAND(ID_RETRIEVAL, &CCBIRFormView::OnRetrieval)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_OUTPUTRESULT, &CCBIRFormView::OnBnClickedOutputresult)
	ON_COMMAND(ID_OUTPUTRESULT, &CCBIRFormView::OnOutputresult)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CCBIRFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCBIRFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

/************************************************************************************
ʹ��GDI��ͼ�ķ����������ⲿ��JPG���󣬲�����ʵ��ͼ��Ԥ����ʾ��ʵ�֡�
************************************************************************************/
bool LoadMyJpegFile(CString fname , LPPICTURE *lppi)
{
	HANDLE hFile=CreateFile(fname,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);

	if(hFile==INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL , "�޷����ļ�!" , "Error" , MB_OK);
		return FALSE;
	}
	//ȡ���ļ���С
	DWORD dwFileSize=GetFileSize(hFile,NULL);

	if((DWORD)-1==dwFileSize)
	{
		CloseHandle(hFile);

		MessageBoxA(NULL , "ͼ���ļ��ǿյ�!" , "Error" , MB_OK);
		return FALSE;
	}
	//��ȡͼ���ļ�
	LPVOID pvData;

	//���ļ���С�����ڴ�
	HGLOBAL hGlobal=GlobalAlloc(GMEM_MOVEABLE,dwFileSize);

	if(NULL==hGlobal)
	{
		CloseHandle(hFile);

		MessageBoxA(NULL , "�ڴ治�㣬�޷������㹻�ڴ�!" , "Error" , MB_OK);
		return FALSE;
	}

	pvData=GlobalLock(hGlobal);
	if(NULL==pvData)
	{
		GlobalUnlock(hGlobal);
		CloseHandle(hFile);

		MessageBoxA(NULL , "�޷������ڴ�!" , "Error" , MB_OK);
		return FALSE;
	}

	DWORD dwFileRead=0;
	BOOL bRead=ReadFile(hFile,pvData,dwFileSize,&dwFileRead,NULL);
	GlobalUnlock(hGlobal);
	CloseHandle(hFile);
	if(FALSE==bRead)
	{
		MessageBoxA(NULL , "���ļ�����!" , "Error" , MB_OK);
		return FALSE;
	}

	LPSTREAM pstm=NULL;
	//���ѷ����ڴ�����IStream��
	HRESULT hr=CreateStreamOnHGlobal(hGlobal,TRUE,&pstm);

	if(!SUCCEEDED(hr))
	{
		MessageBoxA(NULL , "����������ʧ��!" , "Error" , MB_OK);
		if(pstm!=NULL)
			pstm->Release();
		return FALSE;
	}
	else if(pstm==NULL)
	{
		MessageBoxA(NULL , "����������ʧ��!" , "Error" , MB_OK);
		return FALSE;
	}

	if(lppi != NULL && *lppi != NULL)
	{
		(*lppi)->Release();
	}
	hr=OleLoadPicture(pstm,dwFileSize,FALSE,IID_IPicture,(LPVOID*)&(*lppi));
	pstm->Release();
	if(!SUCCEEDED(hr))
	{
		MessageBoxA(NULL , "���ز���ʧ��!" , "Error" , MB_OK);
		return FALSE;
	}
	else if(*lppi==NULL)
	{
		MessageBoxA(NULL , "���ز���ʧ��!" , "Error" , MB_OK);
		return FALSE;
	}
	return TRUE;
}

/************************************************************************************
Form��ͼ������ɺ�ĸ��²���
��Ҫ�����������ݣ�
1. Form��ͼ�Ͽؼ��Ĵ������ʼ����
2. �����ؼ���С��ֱ����޹ػ�������
3. ͼ����ɨ������أ�ScanDatabase��
************************************************************************************/
void CCBIRFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// ���õ�ǰ��ͼ�Ĵ�С
	SetScrollSizes(MM_TEXT , CSize(10 , 10));

	// ���õ�ǰ��ͼ��MFC���
	DWORD dwStyle = GetWindowLong(mImageDBTree.m_hWnd , GWL_STYLE);
	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;

	SetWindowLong(mImageDBTree.m_hWnd , GWL_STYLE , dwStyle);
	m_allRoot = mImageDBTree.InsertItem("database");
	m_curRoot = m_allRoot;

	// ɨ����Ӧ�ó�����ص�ͼ��⣬���������е�����
	ScanDatabase();

	// Ԥ����ص�������ʼ����������״���Ƶĳ�ʼ����Ԥ��ͼ��ĳ�ʼ������ص�
	mImageDBTree.Expand(m_allRoot , TVE_TOGGLE);

	LoadMyJpegFile("data/image.orig/0.jpg" , &mpImagePPI);

	RECT treeRect;
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_MODELDBTREE);
	pWnd->GetWindowRect(&treeRect);
	pWnd->SetWindowPos(NULL , treeRect.left , treeRect.top , 
		(int)((float)(treeRect.right - treeRect.left) * ((float)mWidth / 250.0f)) , 
		(int)((float)(treeRect.bottom - treeRect.top) * ((float)mHeight / 738.0f)) , SWP_NOMOVE);

	pWnd = GetDlgItem(IDC_RETRIVEIMAGE);
	pWnd->GetWindowRect(&treeRect);
	pWnd->SetWindowPos(NULL , 5 , 700.0f / 738.0f * (float)mHeight , 
		(int)((float)(treeRect.right - treeRect.left) * ((float)mWidth / 250.0f)) , 
		(int)((float)(treeRect.bottom - treeRect.top) * ((float)mHeight / 738.0f)) , SWP_NOZORDER);
	int lastWidth = (int)((float)(treeRect.right - treeRect.left) * ((float)mWidth / 250.0f));

	pWnd = GetDlgItem(IDC_PRRESULT);
	pWnd->GetWindowRect(&treeRect);
	pWnd->SetWindowPos(NULL , 10 + lastWidth , 700.0f / 738.0f * (float)mHeight , 
		(int)((float)(treeRect.right - treeRect.left) * ((float)mWidth / 250.0f)) , 
		(int)((float)(treeRect.bottom - treeRect.top) * ((float)mHeight / 738.0f)) , SWP_NOZORDER);
}

/************************************************************************************
C++ Std��ʽ�ıȽϺ���������ʵ���������и����ַ����ַ����Ĵ�С�Ƚϡ�
ʵ���ϲ������ַ������޳���Ӧ�ĺ�׺�����ַ���ֻ�Թ�ע���ֽ��бȽ�
����10.jpg , 111.jpg���ú�������10.jpg < 111.jpg
************************************************************************************/
bool StringCompareFunc(string a , string b)
{
	char ca[32] , cb[32];
	ZeroMemory(ca , sizeof(char) * 32);
	ZeroMemory(cb , sizeof(char) * 32);
	a._Copy_s(ca , 32 , a.length() - 4);
	b._Copy_s(cb , 32 , b.length() - 4);

	return atoi(ca) < atoi(cb);
}

/************************************************************************************
ɨ�貢����ͼ��⡣
1. ���ú������еķ���ListDstFilesInFloder����᷵����Ŀ���ļ����е������ļ�
2. Ŀ����е������ļ���vector����ʽ�洢
3. ����ͼ���ļ������ֶ�vector�����������
4. ��֯�õ���ǰӦ�ó����ͼ���
************************************************************************************/
void CCBIRFormView::ScanDatabase()
{
	List<string>* pDatabase = new List<string>();
	FuncsLib::ListDstFilesInFolder("data/image.orig" , ".jpg" , pDatabase);

	vector<string> databaseVector;

	for(unsigned int i = 0 ; i < pDatabase->GetListSize() ; ++i)
	{
		databaseVector.push_back(*pDatabase->GetItemPtr(i));
	}

	// Sort the items in the database
	sort(databaseVector.begin() , databaseVector.end() , StringCompareFunc);

	int itemIndex = 0;
	for(vector<string>::iterator ite = databaseVector.begin() ; ite != databaseVector.end() ; ite++)
	{
		CString imageName;
		imageName.Format("%s" , ite->c_str());
		AddFile(itemIndex++ , imageName);
	}

	if (mpCBIRView)
	{
		mpCBIRView->InitDatabase(databaseVector);
	}
	delete pDatabase;
}

/************************************************************************************
�õ��뵱ǰ��Form��ͼ�������Ҳ���Ⱦ����ָ�롣
��ָ����ڴ��������Ӵ���ʱ��MainFrame������г�ʼ����
��ʹ��ʱӦ����ȫ��飬���жϵõ��Ĵ���ָ���Ƿ�Ϸ���
************************************************************************************/
CCBIRView* CCBIRFormView::GetViewPointer()
{
	CMainFrame* parent = ((CMainFrame*)GetParent());
	return (CCBIRView*)parent->m_wndSplitterFrame.GetPane(0 , 1);
}

/************************************************************************************
������ļ�����״�ؼ��У���Ҫ�����ݿ��е�ͼ���ļ���
************************************************************************************/
void CCBIRFormView::AddFile(int index , CString& fileName)
{
	HTREEITEM lastRoot;
	lastRoot = m_curRoot;

	mImageDBTree.InsertItem(fileName , m_curRoot);
	mImageDBTree.SetItemData(m_curRoot , index);

	m_curRoot = lastRoot;
}

/************************************************************************************
MFC�е�UI������Ӧ������
��Ҫ���û��ڲ��������״�ؼ�ʱ�õ���Ӧ�ĸ��±仯���Ա㼰ʱ�޸�Ԥ����Ϣ���仯Ŀ��ͼ�������ֵ�ȡ�
************************************************************************************/
void CCBIRFormView::OnTvnSelchangedModeldbtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM item = pNMTreeView->itemNew;
	HTREEITEM hChild = mImageDBTree.GetChildItem(item.hItem);

	if(!hChild)
	{
		CString imagePath;
		imagePath.Format("data/image.orig/%s" , mImageDBTree.GetItemText(item.hItem));
		LoadMyJpegFile(imagePath , &mpImagePPI);

		InvalidateRect(NULL , true);

		CString imageName = mImageDBTree.GetItemText(item.hItem);

		char imageNameIndex[32];
		sprintf_s(imageNameIndex , sizeof(char) * 32 , "%s" , imageName.GetBuffer());
		imageNameIndex[strlen(imageNameIndex) - 4] = '\0';

		mSelectedIndex = atoi(imageNameIndex);

		char dstImageName[MAX_PATH];
		sprintf_s(dstImageName , sizeof(char) * MAX_PATH , "%d.jpg" , mSelectedIndex);
		mpCBIRView->LoadDstImage(dstImageName , mSelectedIndex , false);
	}

	InvalidateRect(NULL , false);
}

void CCBIRFormView::OnDestroy()
{
	CFormView::OnDestroy();
}

/************************************************************************************
Form��ͼ�Ļ��ƺ���
************************************************************************************/
void CCBIRFormView::OnPaint()
{
	CPaintDC dc(this);

	int xPos = 13 * ((float)mWidth / 250.0f);
	int yPos = 465 * ((float)mHeight / 738.0f);
	float maxWith = 230.0f * ((float)mWidth / 250.0f);
	float maxHeight = 220.0f * ((float)mHeight / 738.0f);

	int halfWidth = (int)(maxWith * 0.5f);
	int haftHeight = (int)(maxHeight * 0.5f);

	if(mpImagePPI)
	{
		CDC *pDC = GetDC();

		long hmWidth = 0;
		long hmHeight = 0;
		mpImagePPI->get_Height(&hmHeight);
		mpImagePPI->get_Width(&hmWidth);
		float ahmWidth = (float)hmWidth / 26.45f;
		float ahmHeight = (float)hmHeight /26.45f;

		float xScale = maxWith / (float)ahmWidth;
		float yScale = maxHeight / (float)ahmHeight;
		float scale = min(xScale , yScale);

		float aWidth = (float)ahmWidth * scale;
		float aHeight = (float)ahmHeight * scale; 

		CRect rc;
		GetClientRect(&rc);
		HRESULT hr = mpImagePPI->Render(pDC->m_hDC , xPos + halfWidth - aWidth * 0.5f , yPos + haftHeight - aHeight * 0.5f , aWidth , aHeight , 0 , hmHeight , hmWidth , -hmHeight , &rc);

		ReleaseDC(pDC);
	}
}

/************************************************************************************
���¾�Ϊ��Ӧ��UI��Ӧ��������
************************************************************************************/
void CCBIRFormView::OnBnClickedRetriveimage()
{
	mpCBIRView->PerformRetrieval();
}

void CCBIRFormView::OnBnClickedPrresult()
{
	// TODO: Add your control notification handler code here
	mpCBIRView->mpRPResultDlg->ShowWindow(SW_SHOW);
}

void CCBIRFormView::OnUpdateShownormal(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(!mpCBIRView->mShowBeeResult);
}

void CCBIRFormView::OnShownormal()
{
	// TODO: Add your command handler code here
	mpCBIRView->mShowBeeResult = false;
	mpCBIRView->InvalidateRect(NULL , false);
}

void CCBIRFormView::OnUpdateShowbeecolony(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(mpCBIRView->mShowBeeResult);
}

void CCBIRFormView::OnShowbeecolony()
{
	// TODO: Add your command handler code here
	mpCBIRView->mShowBeeResult = true;
	mpCBIRView->InvalidateRect(NULL , false);
}

void CCBIRFormView::OnShowdsr()
{
	// TODO: Add your command handler code here
	mpCBIRView->mpImageDsrDlg->ShowWindow(SW_SHOW);
}

void CCBIRFormView::OnRetrieval()
{
	// TODO: Add your command handler code here
	mpCBIRView->PerformRetrieval();
}

void CCBIRFormView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	mWidth = cx;
	mHeight = cy;

	TRACE("Size: %d , %d\n" , mWidth , mHeight);
}


void CCBIRFormView::OnBnClickedOutputresult()
{
	mpCBIRView->OutputResult();
}


void CCBIRFormView::OnOutputresult()
{
	// TODO: Add your command handler code here
	mpCBIRView->OutputResult();
}
