// DlgPageViewSetTime.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"
#include "DlgPageViewSetTime.h"


// CDlgPageViewSetTime 对话框

IMPLEMENT_DYNAMIC(CDlgPageViewSetTime, CDialog)

CDlgPageViewSetTime::CDlgPageViewSetTime(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPageViewSetTime::IDD, pParent)
	, m_nTimeValue(0)
{

}

CDlgPageViewSetTime::~CDlgPageViewSetTime()
{
}

void CDlgPageViewSetTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TIME_VALUE, m_nTimeValue);
	DDX_Control(pDX, IDC_BUTTON_SET, m_btnSet);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_EXIT, m_btnExit);
}

BEGIN_MESSAGE_MAP(CDlgPageViewSetTime, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CDlgPageViewSetTime::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgPageViewSetTime::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDlgPageViewSetTime::OnBnClickedBtnExit)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CDlgPageViewSetTime 消息处理程序

BOOL CDlgPageViewSetTime::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	m_nTimeValue = g_VideoPageViewInfo.nTimeSpan;

	LoadSkin();

	UpdateData(FALSE);

	return TRUE;
}

BOOL CDlgPageViewSetTime::LoadSkin()
{
	m_btnSet.LoadBitmap(IDB_BITMAP_SET_PAGEVIEW_TIME);
	m_btnCancel.LoadBitmap(IDB_BITMAP_CANCEL_PAGEVIEW_TIME);
	m_btnExit.LoadBitmap(IDB_BITMAP_MENU_QUIT);

	m_Bmp.LoadBitmap(IDB_BITMAP_PAGEVIEW_SETTIME);
	m_Bmp.GetObject(sizeof(BITMAP),(LPVOID)&m_bm);   

	return TRUE;
}

void CDlgPageViewSetTime::OnBnClickedButtonSet()
{	
	try
	{
		UpdateData(TRUE);

		if (m_nTimeValue < 5||m_nTimeValue > 60)
		{
			m_nTimeValue = g_VideoPageViewInfo.nTimeSpan;
			UpdateData(FALSE);
			MessageBox("时间间隔错误，正确的时间间隔最小为5秒，最大间隔为60秒!","视频监视");
			return;
		}

		g_VideoPageViewInfo.nTimeSpan = m_nTimeValue;

		char szValue[16] = {0};
		char szPathName [256] = {0};
		memset(szValue,0,sizeof(szValue));
		memset(szPathName,0,sizeof(szPathName));

		sprintf_s(szValue,sizeof(szValue)-1,"%d",g_VideoPageViewInfo.nTimeSpan);

		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\VEMCUCtl.ini");
		WritePrivateProfileString("PAGEVIEWINFO", "PageViewTimeSpan", szValue, szPathName);

		OnOK();
	}
	catch(...)
	{

	}
}

void CDlgPageViewSetTime::OnBnClickedButtonCancel()
{
	OnCancel();
}

void CDlgPageViewSetTime::OnBnClickedBtnExit()
{
	OnCancel();
}

BOOL CDlgPageViewSetTime::OnEraseBkgnd(CDC* pDC)
{
	//////////////////////////////////////////////////////////////////////////
	CRect   rect;
	GetClientRect(&rect);
	CDC   dcMem;

	dcMem.CreateCompatibleDC(pDC);   
	CBitmap* pOldBitmap = dcMem.SelectObject(&m_Bmp);   

	pDC->StretchBlt(rect.left,
		rect.top,
		rect.right,
		rect.bottom,
		&dcMem,
		0,
		0,
		m_bm.bmWidth,
		m_bm.bmHeight,
		SRCCOPY);

	dcMem.SelectObject(pOldBitmap);

	return TRUE;
}
