// DlgHistoryLog.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgHistoryLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDlgHistoryLog 对话框

IMPLEMENT_DYNAMIC(CDlgHistoryLog, CDialog)

CDlgHistoryLog::CDlgHistoryLog(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHistoryLog::IDD, pParent)
{
	m_Brush.CreateSolidBrush(RGB(210,224,237)); 
	//m_Brush.CreateSolidBrush(RGB(218,255,251)); 
}

CDlgHistoryLog::~CDlgHistoryLog()
{
	m_DlgHlRelation.DestroyWindow();
	m_DlgHlSystem.DestroyWindow();
	m_DlgHlUser.DestroyWindow();
}

void CDlgHistoryLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BUTTON_HL_OPERATE, m_btnSelOperate);
	DDX_Control(pDX,IDC_BUTTON_HL_SYSTEM, m_btnSelSystem);
	DDX_Control(pDX,IDC_BUTTON_HL_RELATION, m_btnSelRelation);
	DDX_Control(pDX, IDC_BUTTON_HL_VIDEO_ERROR, m_btnVideoError);
	DDX_Control(pDX, IDC_BUTTON_ALARM_HISTORY, m_btnAlarmHistory);
	DDX_Control(pDX, IDC_BUTTON_HL_DEVICE, m_btnDeviceData);
}


BEGIN_MESSAGE_MAP(CDlgHistoryLog, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_HL_SYSTEM, &CDlgHistoryLog::OnBnClickedButtonHlSystem)
	ON_BN_CLICKED(IDC_BUTTON_HL_OPERATE, &CDlgHistoryLog::OnBnClickedButtonHlOperate)
	ON_BN_CLICKED(IDC_BUTTON_HL_RELATION, &CDlgHistoryLog::OnBnClickedButtonHlRelation)
	ON_BN_CLICKED(IDC_BUTTON_HL_VIDEO_ERROR, &CDlgHistoryLog::OnBnClickedButtonHlVideoError)
	ON_BN_CLICKED(IDC_BUTTON_ALARM_HISTORY, &CDlgHistoryLog::OnBnClickedButtonAlarmHistory)
	ON_BN_CLICKED(IDC_BUTTON_HL_DEVICE, &CDlgHistoryLog::OnBnClickedButtonHlDevice)
END_MESSAGE_MAP()


// CDlgHistoryLog 消息处理程序

BOOL CDlgHistoryLog::OnInitDialog()
{
	CDialog::OnInitDialog();

	/******************************回放对话框*****************************/
	CRect	winrc;
	GetClientRect(winrc);
	CRect	rc;
	rc.top = winrc.top + 40;
	rc.bottom = winrc.bottom;
	rc.left = winrc.left;
	rc.right = winrc.right;

	m_DlgHlSystem.Create(IDD_DIALOG_HISTORYLOG_SYSTEM, this);
	m_DlgHlSystem.MoveWindow(rc);
	m_DlgHlSystem.ShowWindow(SW_SHOW);

	m_DlgHlUser.Create(IDD_DIALOG_HISTORYLOG_USER, this);
	m_DlgHlUser.MoveWindow(rc);
	m_DlgHlUser.ShowWindow(SW_HIDE);

	m_DlgHlRelation.Create(IDD_DIALOG_HISTORYLOG_RELATION, this);
	m_DlgHlRelation.MoveWindow(rc);
	m_DlgHlRelation.ShowWindow(SW_HIDE);

	m_DlgHlVideoError.Create(IDD_DIALOG_HISTORYLOG_VIDEO_ERROR,this);
	m_DlgHlVideoError.MoveWindow(rc);
	m_DlgHlVideoError.ShowWindow(SW_HIDE);

	m_DlgHlAlarm.Create(IDD_DIALOG_HISTORYLOG_ALARM,this);
	m_DlgHlAlarm.MoveWindow(rc);
	m_DlgHlAlarm.ShowWindow(SW_HIDE);

	m_dlgHlDevice.Create(IDD_DIALOG_HISTORYLOG_DEVICE,this);
	m_dlgHlDevice.MoveWindow(rc);
	m_dlgHlDevice.ShowWindow(SW_HIDE);

	m_btnSelOperate.LoadBitmap(IDB_BITMAP_HL_USER_ON, 0);
	m_btnSelOperate.LoadBitmap(IDB_BITMAP_HL_USER_OFF, 1);
	m_btnSelOperate.SetIndex(1);
	m_btnSelOperate.Invalidate(true);
	m_btnSelSystem.LoadBitmap(IDB_BITMAP_HL_SYSTEM_ON, 0);
	m_btnSelSystem.LoadBitmap(IDB_BITMAP_HL_SYSTEM_OFF, 1);
	m_btnSelSystem.SetIndex(0);
	m_btnSelSystem.Invalidate(true);
	m_btnSelRelation.LoadBitmap(IDB_BITMAP_HL_RELATION_ON, 0);
	m_btnSelRelation.LoadBitmap(IDB_BITMAP_HL_RELATION_OFF, 1);
	m_btnSelRelation.SetIndex(1);
	m_btnSelRelation.Invalidate(true);
	m_btnVideoError.LoadBitmap(IDB_BITMAP_HL_VIDEO_ON,0);
	m_btnVideoError.LoadBitmap(IDB_BITMAP_HL_VIDEO_OFF,1);
	m_btnVideoError.SetIndex(1);
	m_btnVideoError.Invalidate(true);
	m_btnAlarmHistory.LoadBitmap(IDB_BITMAP_ALARM_HISTORY_SEL, 0);
	m_btnAlarmHistory.LoadBitmap(IDB_BITMAP_ALRAM_HISTORY_UNSEL, 1);
	m_btnAlarmHistory.SetIndex(1);
	m_btnAlarmHistory.Invalidate(true);
	m_btnDeviceData.LoadBitmap(IDB_BITMAP_DEVICE_DATA_SEL,0);
	m_btnDeviceData.LoadBitmap(IDB_BITMAP_DEVICE_DATA_UNSEL,1);
	m_btnDeviceData.SetIndex(1);
	m_btnDeviceData.Invalidate(true);

	m_UpBmp.LoadBitmap(IDB_BITMAP_RP_UP_BG);
	m_UpBmp.GetObject(sizeof(BITMAP),(LPVOID)&m_UpBm);   

	return TRUE; 
}

BOOL CDlgHistoryLog::PreTranslateMessage(MSG* pMsg)
{
	//////////////////////////////////////////////////////////////////////////

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgHistoryLog::OnEraseBkgnd(CDC* pDC)
{
	CRect   rect;
	GetClientRect(&rect);

	CDC dcMem;

	CBitmap*   pOldBitmap = NULL;
	pDC->FillRect (rect, &m_Brush);

	//上面业务选择的背景色
	dcMem.CreateCompatibleDC(pDC);   
	pOldBitmap = dcMem.SelectObject(&m_UpBmp);   

	pDC->StretchBlt(
		rect.left,
		rect.top ,
		rect.right,
		32,
		&dcMem,
		0,
		0,
		m_UpBm.bmWidth,
		m_UpBm.bmHeight,
		SRCCOPY);

	dcMem.SelectObject(pOldBitmap);

	return true;
}

void CDlgHistoryLog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect(0,0,0,0);
	CWnd *pWnd;

	// *****************************************最上面的按钮
	pWnd = GetDlgItem(IDC_BUTTON_HL_SYSTEM); //系统日志
	if(pWnd)
	{
		rect.left = 1;
		rect.right = rect.left+90;
		rect.top = 1;
		rect.bottom = 27;

		pWnd->MoveWindow(rect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_HL_OPERATE); //操作日志
	if(pWnd)
	{
		rect.left = rect.right+1;
		rect.right = rect.left+ 90;
		rect.top = 1;
		rect.bottom = 27;

		pWnd->MoveWindow(rect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_HL_VIDEO_ERROR);//联动日志
	if(pWnd)
	{
		rect.left = rect.right+1;
		rect.right = rect.left+ 90;
		rect.top = 1;
		rect.bottom = 27;

		pWnd->MoveWindow(rect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_ALARM_HISTORY);//告警日志
	if(pWnd)
	{
		rect.left = rect.right+1;
		rect.right = rect.left+ 90;
		rect.top = 1;
		rect.bottom = 27;

		pWnd->MoveWindow(rect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_HL_DEVICE);//告警日志
	if(pWnd)
	{
		rect.left = rect.right+1;
		rect.right = rect.left+ 90;
		rect.top = 1;
		rect.bottom = 27;

		pWnd->MoveWindow(rect);
	}

	if (pWnd)
	{
		CRect	winrc;
		GetClientRect(winrc);
		CRect	rc;
		rc.top = winrc.top + 40;
		rc.bottom = winrc.bottom;
		rc.left = winrc.left;
		rc.right = winrc.right;

		m_DlgHlSystem.MoveWindow(rc);

		m_DlgHlUser.MoveWindow(rc);

		m_DlgHlVideoError.MoveWindow(rc);

		m_DlgHlAlarm.MoveWindow(rc);

		m_dlgHlDevice.MoveWindow(rc);
	}
	//pWnd = GetDlgItem(IDC_BUTTON_HL_RELATION);//联动日志
	//if(pWnd)
	//{
	//	rect.left = rect.right+1;
	//	rect.right = rect.left+ 90;
	//	rect.top = 1;
	//	rect.bottom = 27;

	//	pWnd->MoveWindow(rect);
	//}
}

void CDlgHistoryLog::OnBnClickedButtonHlSystem()
{
	SelectBtnControl(&m_btnSelSystem);

	m_DlgHlUser.ShowWindow(SW_HIDE);
	m_DlgHlRelation.ShowWindow(SW_HIDE);
	m_DlgHlVideoError.ShowWindow(SW_HIDE);
	m_DlgHlSystem.ShowWindow(SW_SHOW);
	m_DlgHlAlarm.ShowWindow(SW_HIDE);
	m_dlgHlDevice.ShowWindow(SW_HIDE);
}

void CDlgHistoryLog::OnBnClickedButtonHlOperate()
{
	SelectBtnControl(&m_btnSelOperate);

	m_DlgHlSystem.ShowWindow(SW_HIDE);
	m_DlgHlRelation.ShowWindow(SW_HIDE);
	m_DlgHlVideoError.ShowWindow(SW_HIDE);
	m_DlgHlAlarm.ShowWindow(SW_HIDE);
	m_DlgHlUser.ShowWindow(SW_SHOW);
	m_dlgHlDevice.ShowWindow(SW_HIDE);
}

void CDlgHistoryLog::OnBnClickedButtonHlRelation()
{
	SelectBtnControl(&m_btnSelRelation);

	m_DlgHlUser.ShowWindow(SW_HIDE);
	m_DlgHlSystem.ShowWindow(SW_HIDE);
	m_DlgHlVideoError.ShowWindow(SW_HIDE);
	m_DlgHlAlarm.ShowWindow(SW_HIDE);
	m_DlgHlRelation.ShowWindow(SW_SHOW);
	m_dlgHlDevice.ShowWindow(SW_HIDE);
}

void CDlgHistoryLog::OnBnClickedButtonHlVideoError()
{
	SelectBtnControl(&m_btnVideoError);

	m_DlgHlUser.ShowWindow(SW_HIDE);
	m_DlgHlSystem.ShowWindow(SW_HIDE);
	m_DlgHlRelation.ShowWindow(SW_HIDE);
	m_DlgHlAlarm.ShowWindow(SW_HIDE);
	m_DlgHlVideoError.ShowWindow(SW_SHOW);
	m_dlgHlDevice.ShowWindow(SW_HIDE);
}

void CDlgHistoryLog::SelectBtnControl(CSkinButton* btn)
{
	m_btnSelOperate.SetIndex(1);
	m_btnSelOperate.Invalidate(TRUE);
	m_btnSelSystem.SetIndex(1);
	m_btnSelSystem.Invalidate(TRUE);
	m_btnSelRelation.SetIndex(1);
	m_btnSelRelation.Invalidate(TRUE);
	m_btnVideoError.SetIndex(1);
	m_btnVideoError.Invalidate(TRUE);
	m_btnAlarmHistory.SetIndex(1);
	m_btnAlarmHistory.Invalidate(TRUE);
	m_btnDeviceData.SetIndex(1);
	m_btnDeviceData.Invalidate(TRUE);

	btn->SetIndex(0);
	btn->Invalidate(TRUE);
}

void CDlgHistoryLog::ShowDialog(int nID)
{
	m_DlgHlSystem.ShowWindow(SW_HIDE);
	m_DlgHlRelation.ShowWindow(SW_HIDE);
	m_DlgHlVideoError.ShowWindow(SW_HIDE);
	m_DlgHlUser.ShowWindow(SW_HIDE);
	m_DlgHlAlarm.ShowWindow(SW_HIDE);
	m_dlgHlDevice.ShowWindow(SW_HIDE);

	GetDlgItem(nID)->ShowWindow(SW_SHOW);
}

void CDlgHistoryLog::OnBnClickedButtonAlarmHistory()
{
	SelectBtnControl(&m_btnAlarmHistory);

	//ShowDialog(IDD_DIALOG_HISTORYLOG_ALARM);

	m_DlgHlSystem.ShowWindow(SW_HIDE);
	m_DlgHlRelation.ShowWindow(SW_HIDE);
	m_DlgHlVideoError.ShowWindow(SW_HIDE);
	m_DlgHlUser.ShowWindow(SW_HIDE);
	m_DlgHlAlarm.ShowWindow(SW_SHOW);
	m_dlgHlDevice.ShowWindow(SW_HIDE);
}

void CDlgHistoryLog::OnBnClickedButtonHlDevice()
{
	SelectBtnControl(&m_btnDeviceData);

	//ShowDialog(IDD_DIALOG_HISTORYLOG_DEVICE);

	m_DlgHlSystem.ShowWindow(SW_HIDE);
	m_DlgHlRelation.ShowWindow(SW_HIDE);
	m_DlgHlVideoError.ShowWindow(SW_HIDE);
	m_DlgHlUser.ShowWindow(SW_HIDE);
	m_DlgHlAlarm.ShowWindow(SW_HIDE);
	m_dlgHlDevice.ShowWindow(SW_SHOW);
}
