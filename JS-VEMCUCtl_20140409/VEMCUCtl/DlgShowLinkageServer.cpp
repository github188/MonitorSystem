// DlgShowLinkageServer.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"
#include "DlgShowLinkageServer.h"

// CDlgShowLinkageServer 对话框

IMPLEMENT_DYNAMIC(CDlgShowLinkageServer, CDialog)

CDlgShowLinkageServer::CDlgShowLinkageServer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShowLinkageServer::IDD, pParent)
{

}

CDlgShowLinkageServer::~CDlgShowLinkageServer()
{

}

void CDlgShowLinkageServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgShowLinkageServer, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgShowLinkageServer 消息处理程序

BOOL CDlgShowLinkageServer::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	m_Bitmap.LoadBitmap(IDB_BITMAP_SHOW_LINKAGE_SERVER);
	m_Bitmap.GetBitmap(&m_Bm);

	m_hbmpRgn = m_Bitmap.BitmapToRegion(RGB(255,0,255),RGB(230,200,230));
	SetWindowRgn(m_hbmpRgn,FALSE);

	DeleteObject(m_hbmpRgn);
	m_hbmpRgn = NULL;

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	return TRUE;  
}

void CDlgShowLinkageServer::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//////////////////////////////////////////////////////////////////////////
	CDC MemDC;
	CBitmap *pOldBmp = NULL;

	MemDC.CreateCompatibleDC(&dc);
	pOldBmp = MemDC.SelectObject(&m_Bitmap);
	dc.BitBlt(0,0,m_Bm.bmWidth,m_Bm.bmHeight,&MemDC,0,0,SRCCOPY);

	MemDC.SelectObject(pOldBmp);
}

BOOL CDlgShowLinkageServer::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}

void CDlgShowLinkageServer::OnMouseMove(UINT nFlags, CPoint point)
{
	//////////////////////////////////////////////////////////////////////////
	HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
	GetWindowRgn(hRgn);		

	if (PtInRegion( hRgn, point.x, point.y ))
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_MY_HAND));

		if (!m_bMouseIn)
		{
			m_bMouseIn = TRUE;
			Invalidate();
			UpdateWindow();
			SetCapture();
		}
	}
	else
	{
		if (m_bMouseIn)
		{
			m_bMouseIn = FALSE;

			Invalidate();
			UpdateWindow();

			if (!m_bDown )
				ReleaseCapture();
		}
	}
	DeleteObject(hRgn);

	//////////////////////////////////////////////////////////////////////////
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgShowLinkageServer::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (g_TuneCycle_MehtodInfo.bState == true)//轮巡状态
		return;

	if (g_VideoPageViewInfo.nStatus == AUTOVIDEO_PAGEVIEW_STATUS_RUNNING)//人工自动巡视状态
		return;

	//////////////////////////////////////////////////////////////////////////
	if (g_pMainDlg != NULL && g_pMainDlg->m_pDlgMenu != NULL)
	{
		g_pMainDlg->m_nLinkageWidth = LINKAGEWIDTH;

		switch (g_pMainDlg->m_iPreviewNum)
		{
		case 1:
			{
				g_pMainDlg->SendMessage(OM_CONTROLBUTTON, IDC_BUTTON_PS_FORM1,VEM_CONTROL_BUTTON_MESSAGE_TYPE_PAGESERVER);
			}
			break;
		case 4:
			{
				g_pMainDlg->SendMessage(OM_CONTROLBUTTON, IDC_BUTTON_PS_FORM4,VEM_CONTROL_BUTTON_MESSAGE_TYPE_PAGESERVER);
			}
			break;
		case 6:
			{
				g_pMainDlg->SendMessage(OM_CONTROLBUTTON, IDC_BUTTON_PS_FORM6,VEM_CONTROL_BUTTON_MESSAGE_TYPE_PAGESERVER);
			}
			break;
		case 9:
			{
				g_pMainDlg->SendMessage(OM_CONTROLBUTTON, IDC_BUTTON_PS_FORM9,VEM_CONTROL_BUTTON_MESSAGE_TYPE_PAGESERVER);
			}
			break;
		case  16:
			{
				g_pMainDlg->SendMessage(OM_CONTROLBUTTON, IDC_BUTTON_PS_FORM16,VEM_CONTROL_BUTTON_MESSAGE_TYPE_PAGESERVER);
			}
			break;
		default:
			break;
		}

		CRect LinkageServer_rc;
		CRect	VemCuCtl_rc;

		g_pMainDlg->GetClientRect(VemCuCtl_rc);

		LinkageServer_rc.top = g_pMainDlg->m_nMenuHight;
		LinkageServer_rc.bottom = VemCuCtl_rc.bottom;
		LinkageServer_rc.left = VemCuCtl_rc.right - g_pMainDlg->m_nLinkageWidth;
		LinkageServer_rc.right = VemCuCtl_rc.right ;

		if (g_pMainDlg->m_pDlgLinkageServer != NULL)
		{
			g_pMainDlg->m_pDlgLinkageServer->MoveWindow(LinkageServer_rc);
			g_pMainDlg->m_pDlgLinkageServer->ShowWindow(SW_SHOW);
			g_pMainDlg->m_pDlgLinkageServer->m_bRelationListExtendFlag = FALSE;
		}

		//调整告警展示窗口
		CRect AlarmInfo_rc;
		AlarmInfo_rc.top = LinkageServer_rc.bottom - g_pMainDlg->m_nAlarmHeight;
		AlarmInfo_rc.bottom = LinkageServer_rc.bottom;
		AlarmInfo_rc.left = VemCuCtl_rc.left + g_pMainDlg->m_nControlWidth;
		AlarmInfo_rc.right = VemCuCtl_rc.right - g_pMainDlg->m_nLinkageWidth;

		if (g_pMainDlg->m_pDlgAlarmInfo != NULL)
		{
			g_pMainDlg->m_pDlgAlarmInfo->MoveWindow(AlarmInfo_rc);
		}

		g_pMainDlg->m_bLinkageServerFlag = TRUE;
	
		ShowWindow(SW_HIDE);
	}

	//////////////////////////////////////////////////////////////////////////
	CDialog::OnLButtonDown(nFlags, point);
}
