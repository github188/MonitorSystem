
// SDDElecMapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <cstdio>
#include <iostream>
#include "SDDElecMap.h"
#include "SDDElecMapDlg.h"
#include "ElecMapView.h"
#include "ElecMapDoc.h"
#include "ElecMapFrame.h"
#include "canvas/HuaMian.h"
#include "canvas/TyTool.h"

#include "canvas/HMProject.h"
#include "gloable/FileRead.h"

#include "VEMCUCtl/xappany.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSDDElecMapDlg* g_pMainDlg=NULL;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


CSDDElecMapDlg::CSDDElecMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSDDElecMapDlg::IDD, pParent)
	,m_pElecMapDoc(NULL)
	,m_pElecMapView(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSDDElecMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSDDElecMapDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	//menu message
END_MESSAGE_MAP()

BOOL CSDDElecMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//=====================================================
	CRect rectWnd;
	GetClientRect(&rectWnd);
	int width = rectWnd.right - rectWnd.left;
	int height = rectWnd.bottom - rectWnd.top;
	rectWnd.top+=150;  rectWnd.bottom = rectWnd.top+height;
	rectWnd.left+=300;   rectWnd.right  = rectWnd.left+width;
	MoveWindow(rectWnd);

	//get view rect.
	UINT TargetCtrlID = IDC_STATIC_VIEWAREA;
	CWnd *pWnd = this->GetDlgItem(TargetCtrlID);
	pWnd->GetWindowRect(RectTargetCtrl);
	this->ScreenToClient(RectTargetCtrl);

	CCreateContext content;
	CWnd* pFramWnd = this;

	m_pElecMapDoc  = new CElecMapDoc();
	m_pElecMapDoc->OnNewDocument();
	if (!m_pElecMapDoc)
	{
		TRACE0("initial doc plate failed.");
		return FALSE;
	}

	context.m_pCurrentDoc = m_pElecMapDoc;
	context.m_pCurrentFrame = (CFrameWnd *)this;
	context.m_pLastView = NULL;
	context.m_pNewDocTemplate = NULL;
	context.m_pNewViewClass = RUNTIME_CLASS(CElecMapView);

	m_pElecMapView = (CElecMapView*)context.m_pNewViewClass->CreateObject();
	if (m_pElecMapView==NULL) {
		TRACE1("Warning: Dynamic create of view type %hs failed.\n",
			context.m_pNewViewClass->m_lpszClassName);
	}
	ASSERT_KINDOF(CWnd,m_pElecMapView);
	if (!m_pElecMapView->Create(NULL,NULL,AFX_WS_DEFAULT_VIEW,RectTargetCtrl,this,
		AFX_IDW_PANE_FIRST,&context)) 
	{
		TRACE0("Warning: Couldn't create view for frame.\n");
		return FALSE;
	}

	m_pElecMapView->ShowWindow(SW_SHOW);
	m_pElecMapView->MoveWindow(RectTargetCtrl);
	//=====================================================
	return TRUE;
}

CElecMapView* CSDDElecMapDlg::getCurrentViewInstance() const
{
	return m_pElecMapView;
}

CElecMapDoc*  CSDDElecMapDlg::getCurrentDocInstance() const
{
	return m_pElecMapDoc;
}

void CSDDElecMapDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CSDDElecMapDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CSDDElecMapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSDDElecMapDlg::OnDestroy()
{
	delete m_pElecMapDoc;
	if (m_pElecMapDoc)
	{
		m_pElecMapDoc =NULL;
	}

	delete m_pElecMapView;
	if (m_pElecMapView)
	{
		m_pElecMapView =NULL;
	}
}

void CSDDElecMapDlg::OnSaveMapFile()
{
	
}

void CSDDElecMapDlg::OnOpenMapFile()
{

}


BOOL CSDDElecMapDlg::PreTranslateMessage( MSG* pMsg )
{
	return m_pElecMapView->PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BOOL CSDDElecMapDlg::openASceneElecMap(const CString& strSceneName)
{
	if (!CHMProject::getInstance()->OpenHM(FALSE,strSceneName)){
		return FALSE;
	}
	
	Invalidate();
	return TRUE;
}

BOOL CSDDElecMapDlg::openASceneElecMap(const int rvuID, const int smID)
{
	CString strSceneID;
	if (!CFileRead::getSceneIDByRvuIDAndSmID( rvuID, smID, strSceneID )){
		TRACE0("get scene id by rvu_id and sm_id failed.\n");
		return FALSE;
	}

	CString strSceneName;
	if (!CFileRead::getSceneNameBySceneID( strSceneID, strSceneName )){
		TRACE0("get scene name by scene_id failed.\n");
		return FALSE;
	}

	if (!CHMProject::getInstance()->OpenHM(FALSE,strSceneName)){
		return FALSE;
	}

	Invalidate();
	return TRUE;
}

void CSDDElecMapDlg::setDlgPosition( CRect rect )
{
	MoveWindow(&rect);

	//get view rect.
	RectTargetCtrl = rect;
	CWnd *pWnd = this->GetDlgItem(IDC_STATIC_VIEWAREA);
	pWnd->MoveWindow(RectTargetCtrl);
	this->ScreenToClient(RectTargetCtrl);
	m_pElecMapView->MoveWindow(&RectTargetCtrl);
}

void CSDDElecMapDlg::postMessageToOpenVideo( CString strCameraID )
{
	CWnd* pParent = NULL;
	CWnd* pDlg = (CWnd*)g_pMainDlg;
	pParent = pDlg->GetParent();

	char* camera = new char[32];
	sprintf_s(camera,sizeof(char)*32,strCameraID.GetBuffer());

	pParent->PostMessage(WM_ELECMAP_OPEN_CAMERA_MESSAGE,(WPARAM)camera,NULL);
}

BOOL CSDDElecMapDlg::updateEntityInStation( char *pRealData, int nCount )
{
	if (!CHMProject::getInstance()->updateEntityInAHuamian(pRealData, nCount)){
		TRACE0("update entity status failed.\n");
		return FALSE;
	}

	Invalidate();
	return TRUE;
}
