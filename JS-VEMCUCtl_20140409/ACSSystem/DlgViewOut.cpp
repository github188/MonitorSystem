// DlgViewOut.cpp : 实现文件
//

#include "stdafx.h"
#include "ACSSystem.h"
#include "DlgViewOut.h"

#define TITLE_HEIGHT  20
// CDlgViewOut 对话框

IMPLEMENT_DYNAMIC(CDlgViewOut, CDialog)

CDlgViewOut::CDlgViewOut(CWnd* pParent /*=NULL*/)
: CDialog(CDlgViewOut::IDD, pParent)
{
	m_bSelected = FALSE;
	m_nLoginID = -1;
	m_nPlayID = -1;
	m_nDvrType = -1;
	m_strLightCode = _T("");
	m_bLightRelating = FALSE;
	m_brush.CreateSolidBrush(RGB(77, 188, 176)); 
}

CDlgViewOut::~CDlgViewOut()
{
}

void CDlgViewOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CAMERA_NAME, m_strCameraName);
}


BEGIN_MESSAGE_MAP(CDlgViewOut, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgViewOut 消息处理程序

void CDlgViewOut::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd* pWnd = NULL;
	DrawSelectedWindowFrame(m_bSelected);
}

BOOL CDlgViewOut::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect Rect(400, 400, 400, 400);
	m_ViewChannel.Create(NULL, NULL, WS_TABSTOP|WS_CHILD|WS_VISIBLE, Rect, this, VIEW_CHANNEL_WINDOW_CONTROLID);

	m_titleBk.LoadBitmap(IDB_BITMAP_TITLE);
	m_titleBk.GetObject(sizeof(BITMAP),(LPVOID)&m_bmBk);

	memset(&m_camera_info,0,sizeof(CAMERA_INFO));

	m_strCameraName.SetTransparent(TRUE);
	m_strCameraName.SetTextColor(RGB(23, 92, 85));
	m_strCameraName.SetText("");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgViewOut::DrawSelectedWindowFrame(BOOL bSelected)
{
	RECT rc,rc1;
	CWnd *pWnd;

	GetClientRect(&rc);

	pWnd = GetDlgItem(VIEW_CHANNEL_WINDOW_CONTROLID);
	if (pWnd != NULL)
	{


		rc1.top = rc.top;
		rc1.left = rc.left;
		rc1.bottom = rc.bottom;
		rc1.right = rc.right;

		m_bSelected = bSelected;
		InvalidateRect(&rc1);

		CRect rect;

		if(m_bSelected != FALSE)
		{
			rect.left=rc.left+1;
			rect.right=rc.right-1;
			rect.top=rc.top+TITLE_HEIGHT;
			rect.bottom=rc.bottom-1;
		}
		else
		{
			rect.left=rc.left;
			rect.right=rc.right;
			rect.top=rc.top+TITLE_HEIGHT;
			rect.bottom=rc.bottom;
		}

		m_ViewChannel.MoveWindow(rect);
	}

	pWnd = GetDlgItem(IDC_STATIC_CAMERA_NAME);	//区域名称
	if (pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;

		if(bSelected != FALSE)
		{
			rect.left = rc.left+1;
			rect.right = rc.right-20;
			rect.top = rc.top+1;
			rect.bottom = rc.top + TITLE_HEIGHT;
		}
		else
		{
			rect.left = rc.left;
			rect.right = rc.right-20;
			rect.top = rc.top;
			rect.bottom = rc.top + TITLE_HEIGHT;
		}

		pWnd->MoveWindow(rect);
	}

	//告警详细信息
	/*
	pWnd = GetDlgItem(IDC_STATIC_ALARM_INFO);
	if (pWnd)
	{
		CRect rect;

		if(bSelected != FALSE)
		{
			rect.left = rc.right-20;
			rect.right = rc.right;
			rect.top = rc.top+1;
			rect.bottom = rc.top + TITLE_HEIGHT;
		}
		else
		{
			rect.left = rc.right-20;
			rect.right = rc.right;
			rect.top = rc.top;
			rect.bottom = rc.top + TITLE_HEIGHT;
		}

		pWnd->MoveWindow(rect);
	}*/

}

void CDlgViewOut::DrawRect(CDC *pDC,RECT &rect)
{
	if (pDC == NULL)
		return;

	CPen Pen(PS_SOLID, 1, RGB(255,0,0));
	CPen *pOldPen = pDC->SelectObject(&Pen);

	pDC->MoveTo(rect.left,rect.top);
	pDC->LineTo(rect.right-1,rect.top);
	pDC->LineTo(rect.right-1,rect.bottom-1);
	pDC->LineTo(rect.left,rect.bottom-1);
	pDC->LineTo(rect.left,rect.top);
	pDC->SelectObject(pOldPen);
}

BOOL CDlgViewOut::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect,recttop;
	GetClientRect (&rect);

	//

	if (m_bSelected)
	{
		DrawRect(pDC,rect);
		recttop.top = rect.top+1;
		recttop.left = rect.left+1;
		recttop.right = rect.right-1;
		recttop.bottom = rect.top+TITLE_HEIGHT;
	}
	else
	{
		recttop.top = rect.top;
		recttop.left = rect.left;
		recttop.right = rect.right;
		recttop.bottom = rect.top+TITLE_HEIGHT;
	}

	pDC->FillRect (recttop, &m_brush);

	CDC   dcMem;
	CBitmap*   pOldBitmap;

	dcMem.CreateCompatibleDC(pDC);

	// 上部
	pOldBitmap = dcMem.SelectObject(&m_titleBk);   

	pDC->StretchBlt(rect.left,rect.top,
		rect.right-rect.left,m_bmBk.bmHeight,
		&dcMem,
		0,0,
		m_bmBk.bmWidth,m_bmBk.bmHeight,
		SRCCOPY);

	dcMem.SelectObject(pOldBitmap);

	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}
