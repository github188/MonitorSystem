// DlgMenu.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgMenu.h"
#include "VEMCUCtlDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDlgMenu 对话框

IMPLEMENT_DYNAMIC(CDlgMenu, CDialog)

CDlgMenu::CDlgMenu(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMenu::IDD, pParent)
{

}

CDlgMenu::~CDlgMenu()
{

}

void CDlgMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MEMU_ALARMSET, m_MenuAlarmSet);
	DDX_Control(pDX, IDC_MENU_EMAP, m_MenuEmap);
	DDX_Control(pDX, IDC_MENU_VIEW, m_MenuView);
	DDX_Control(pDX, IDC_MENU_TUNECYCLE, m_MenuTuneCycle);
	DDX_Control(pDX, IDC_MENU_EVENTLOG, m_MenuEventlog);
	DDX_Control(pDX, IDC_MENU_REALTALK, m_MenuRealTalk);
	DDX_Control(pDX, IDC_MENU_TEMPFAVORITE, m_MenuTempFavorite);
	DDX_Control(pDX, IDC_MENU_SYSTEM, m_MenuSystem);
	DDX_Control(pDX, IDC_MENU_REPLAY, m_MenuReplay);
	DDX_Control(pDX, IDC_MENU_MANAGE, m_MenuManage);
	DDX_Control(pDX, IDC_MENU_EXIT, m_MenuQuit);
	DDX_Control(pDX, IDC_MENU_MIN, m_MenuMin);
	DDX_Control(pDX, IDC_MENU_ALARMREALLOG, m_MenuAlarmLog);
	DDX_Control(pDX, IDC_MENU_NETSTRUCT, m_MenuNetStruct);
	DDX_Control(pDX,IDC_MENU_LOGIN,m_MenuLogin);
	DDX_Control(pDX, IDC_BTN_SHOW_CONTROL, m_btnShowControl);
	DDX_Control(pDX, IDC_WEATHER_SHOW, m_staticShowWeather);
	DDX_Control(pDX, IDC_USER_INFO_SHOW, m_staticUserInfo);
	DDX_Control(pDX, IDC_BTN_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_VERSION_INFO_SHOW, m_staticVersionInfo);
	DDX_Control(pDX, IDC_MENU_TRAY, m_MenuTray);

	DDX_Control(pDX, IDC_MENU_ELECMAP, m_MenuElecMap);
	DDX_Control(pDX, IDC_MENU_DEVICE_LEDGER, m_MenuDeviceLedger);
	DDX_Control(pDX, IDC_MENU_LINKAGE_SETTING, m_MenuLinkageSetting);	
	DDX_Control(pDX, IDC_MENU_ASSISTANCE, m_MenuAssistance);
	DDX_Control(pDX, IDC_BUTTON_ALARM_PREVIEW, m_btnAlarmPreview);
}


BEGIN_MESSAGE_MAP(CDlgMenu, CDialog)
	ON_WM_SIZE()
	ON_COMMAND_RANGE(1000, 2000, OnControlButton)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_MENU_MIN, &CDlgMenu::OnBnClickedMenuMin)
	ON_BN_CLICKED(IDC_BTN_SHOW_CONTROL, &CDlgMenu::OnBnClickedBtnShowControl)
	ON_BN_CLICKED(IDC_MENU_EMAP, &CDlgMenu::OnBnClickedMenuEmap)
END_MESSAGE_MAP()


// CDlgMenu 消息处理程序

void CDlgMenu::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//////////////////////////////////////////////////////////////////////////
	CRect winrc;
	CRect rc;	
	CWnd *pWnd;

	GetClientRect(winrc);

	winrc.bottom = winrc.bottom - 10;

	// 退出按钮
	pWnd = GetDlgItem(IDC_MENU_EXIT);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		rc.right=winrc.right - 2;
		rc.left=winrc.right - 1 - 16;			//调整控件大小
		rc.top=winrc.top + 2;
		rc.bottom=winrc.top + 16 + 2;

		pWnd->MoveWindow(rc);		//设置控件大小
	}

	// 最小化到托盘按钮
	pWnd = GetDlgItem(IDC_MENU_TRAY);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		rc.right=rc.left -1;
		rc.left=rc.right - 16;			//调整控件大小
		rc.top=winrc.top + 2;
		rc.bottom=winrc.top + 16 + 2;

		pWnd->MoveWindow(rc);		//设置控件大小
	}
	
	// 最小化按钮
	pWnd = GetDlgItem(IDC_MENU_MIN);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		rc.right=rc.left -1;
		rc.left=rc.right - 16;			//调整控件大小
		rc.top=winrc.top + 2;
		rc.bottom=winrc.top + 16 + 2;
		
		pWnd->MoveWindow(rc);		//设置控件大小
	}

	//显示版本信息
	pWnd = GetDlgItem(IDC_VERSION_INFO_SHOW);
	if (pWnd)
	{
		rc.top = winrc.top + 40;
		rc.bottom = rc.top + 46;

#if VM_SJ_CLIENT_VERSION
		rc.right = winrc.right - 2;
#else
		rc.right = winrc.right - 20;
#endif

		rc.left = rc.right - 80;

		pWnd->MoveWindow(rc);
	}

	// 重新登录
	pWnd = GetDlgItem(IDC_MENU_LOGIN);
	if (pWnd)
	{
		rc.right = winrc.right - 45;
		rc.left = rc.right - 70;
		rc.top = winrc.bottom - 70 ;
		rc.bottom = winrc.bottom;

		pWnd->MoveWindow(rc);
	}

	// 帮助
	pWnd = GetDlgItem(IDC_BTN_HELP);
	if (pWnd)
	{
		rc.right = rc.left - 3;
		rc.left = rc.right - 70;
		rc.top = winrc.bottom - 70;
		rc.bottom = winrc.bottom;

		pWnd->MoveWindow(rc);
	}

	// 日志
	pWnd = GetDlgItem(IDC_MENU_EVENTLOG);
	if (pWnd)
	{
		rc.right = rc.left - 3;
		rc.left = rc.right - 70;
		rc.top = winrc.bottom - 70;
		rc.bottom = winrc.bottom;

		pWnd->MoveWindow(rc);
	}

	// 管理按钮
	pWnd = GetDlgItem(IDC_MENU_MANAGE);
	if (pWnd)
	{
		rc.right = rc.left - 3;
		rc.left = rc.right - 70;
		rc.top = winrc.bottom - 70;
		rc.bottom = winrc.bottom;

		pWnd->MoveWindow(rc);
	}

	// 回放按钮
	pWnd = GetDlgItem(IDC_MENU_REPLAY);
	if (pWnd)
	{
		rc.right = rc.left - 3;
		rc.left = rc.right - 70;
		rc.top = winrc.bottom - 70;
		rc.bottom = winrc.bottom;

		pWnd->MoveWindow(rc);
	}

	//设备台账
	pWnd = GetDlgItem(IDC_MENU_DEVICE_LEDGER);
	if (pWnd)
	{
		rc.right = rc.left - 3;
		rc.left = rc.right - 70;
		rc.top = winrc.bottom - 70 ;
		rc.bottom = winrc.bottom;

		pWnd->MoveWindow(rc);
	}

	//告警预览
	pWnd = GetDlgItem(IDC_BUTTON_ALARM_PREVIEW);
	if (pWnd)
	{
		rc.right = rc.left - 3;
		rc.left = rc.right - 70;
		rc.top = winrc.bottom - 70 ;
		rc.bottom = winrc.bottom;

		pWnd->MoveWindow(rc);
	}

	//辅助系统
	pWnd = GetDlgItem(IDC_MENU_ASSISTANCE);
	if (pWnd)
	{
		rc.right = rc.left - 3;
		rc.left = rc.right - 70;
		rc.top = winrc.bottom - 70 ;
		rc.bottom = winrc.bottom;

		pWnd->MoveWindow(rc);
	}

	// 多画面
	pWnd = GetDlgItem(IDC_MENU_VIEW);
	if (pWnd)
	{
		rc.right = rc.left - 3;
		rc.left = rc.right - 70;
		rc.top = winrc.bottom - 70 ;
		rc.bottom = winrc.bottom;

		pWnd->MoveWindow(rc);
	}

	//电子地图
	pWnd = GetDlgItem(IDC_MENU_ELECMAP);
	if (pWnd)
	{
		rc.right = rc.left - 3;
		rc.left = rc.right - 70;
		rc.top = winrc.bottom - 70 ;
		rc.bottom = winrc.bottom;

		pWnd->MoveWindow(rc);
	}

	if (g_nShowManagerClient == 1)
	{
		//辅助联动配置按钮
		pWnd = GetDlgItem(IDC_MENU_LINKAGE_SETTING);
		if (pWnd)
		{
			rc.right = rc.left - 3;
			rc.left = rc.right - 70;
			rc.top = winrc.bottom - 70 ;
			rc.bottom = winrc.bottom;

			pWnd->MoveWindow(rc);
		}
	}
	else
	{
		pWnd = GetDlgItem(IDC_MENU_LINKAGE_SETTING);
		if (pWnd)
		{
			pWnd->ShowWindow(FALSE);
		}
	}

	//显示用户信息
	pWnd = GetDlgItem(IDC_USER_INFO_SHOW);
	if (pWnd)
	{
		rc.right = rc.left - 3;
		rc.left = rc.right - 100;
		rc.top = winrc.bottom - 50;
		rc.bottom = winrc.bottom;

		pWnd->MoveWindow(rc);
	}

	// 显示控制按钮
	pWnd = GetDlgItem(IDC_BTN_SHOW_CONTROL);
	if (pWnd)
	{
		rc.left = (winrc.right+winrc.left)/2 - 36;
		rc.right = rc.left + 90;
		rc.top = winrc.bottom;
		rc.bottom = winrc.bottom+10;

		pWnd->MoveWindow(rc);
	}

	// 气象显示数据
	pWnd = GetDlgItem(IDC_WEATHER_SHOW);
	if (pWnd)
	{
		rc.left = winrc.left + 315;
		rc.right = rc.left + 500;
		rc.top = winrc.top + 20;
		rc.bottom = winrc.bottom + 6;

		pWnd->MoveWindow(rc);
	}

	//// 收藏夹
	//pWnd = GetDlgItem(IDC_MENU_TEMPFAVORITE);
	//if (pWnd)
	//{
	//	rc.left = winrc.right - 72*2;
	//	rc.right = winrc.right - 72*1;
	//	rc.top = winrc.bottom - 54;
	//	rc.bottom = winrc.bottom;

	//	pWnd->MoveWindow(rc);
	//}

	//// 轮巡
	//pWnd = GetDlgItem(IDC_MENU_TUNECYCLE);
	//if (pWnd)
	//{
	//	rc.left = winrc.right - 72*3;
	//	rc.right = winrc.right - 72*2;
	//	rc.top = winrc.bottom - 54;
	//	rc.bottom = winrc.bottom;

	//	pWnd->MoveWindow(rc);
	//}

	//// 网络结构
	//pWnd = GetDlgItem(IDC_MENU_EMAP);
	//if (pWnd)
	//{
	//	rc.left = winrc.right - 72*5;
	//	rc.right = winrc.right - 72*4;
	//	rc.top = winrc.bottom - 54;
	//	rc.bottom = winrc.bottom;

	//	pWnd->MoveWindow(rc);
	//}

	//// 语音对讲
	//pWnd = GetDlgItem(IDC_MENU_REALTALK);
	//if (pWnd)
	//{
	//	rc.left = winrc.right - 72*6;
	//	rc.right = winrc.right - 72*5;
	//	rc.top = winrc.bottom - 54;
	//	rc.bottom = winrc.bottom;

	//	pWnd->MoveWindow(rc);
	//}

	//// 报警设置
	//pWnd = GetDlgItem(IDC_MEMU_ALARMSET);
	//if (pWnd)
	//{
	//	rc.left = winrc.right - 72*7;
	//	rc.right = winrc.right - 72*6;
	//	rc.top = winrc.bottom - 54;
	//	rc.bottom = winrc.bottom;

	//	pWnd->MoveWindow(rc);
	//}

	//// 报警日志
	//pWnd = GetDlgItem(IDC_MENU_ALARMREALLOG);
	//if (pWnd)
	//{
	//	rc.left = winrc.right - 72*8;
	//	rc.right = winrc.right - 72*7;
	//	rc.top = winrc.bottom - 54;
	//	rc.bottom = winrc.bottom;

	//	pWnd->MoveWindow(rc);
	//}

	//// 系统结构
	//pWnd = GetDlgItem(IDC_MENU_SYSTEM);
	//if (pWnd)
	//{
	//	rc.left = winrc.right - 72*9;
	//	rc.right = winrc.right - 72*8;
	//	rc.top = winrc.bottom - 54;
	//	rc.bottom = winrc.bottom;

	//	pWnd->MoveWindow(rc);
	//}

	//// 网络结构
	//pWnd = GetDlgItem(IDC_MENU_NETSTRUCT);
	//if (pWnd)
	//{
	//	rc.left = winrc.right - 72*10;
	//	rc.right = winrc.right - 72*9;
	//	rc.top = winrc.bottom - 54;
	//	rc.bottom = winrc.bottom;

	//	pWnd->MoveWindow(rc);
	//}

	//pWnd = GetDlgItem(IDC_MENU_EVENTLOG);
	//if (pWnd)
	//{
	//	rc.left = winrc.right - 72*11;
	//	rc.right = winrc.right - 72*10;
	//	rc.top = winrc.bottom - 54;
	//	rc.bottom = winrc.bottom;

	//	pWnd->MoveWindow(rc);
	//}
}

BOOL CDlgMenu::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==0x100   &&   pMsg->wParam==27)//屏住ESC
		return	TRUE;
	if(pMsg->message==0x100   &&   pMsg->wParam==13)//屏住ENTER   
		return	TRUE; 
	if(pMsg->message==0x100   &&   pMsg->wParam==VK_SPACE)
		return	TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

//将相关控制按钮的消息发送到主对话框进行处理
void CDlgMenu::OnControlButton(UINT nID)
{
	AfxGetMainWnd()->PostMessage(OM_CONTROLBUTTON, nID,VEM_CONTROL_BUTTON_MESSAGE_TYPE_MENU);
}

BOOL CDlgMenu::OnEraseBkgnd(CDC* pDC)
{
	CRect   rect;
	GetClientRect(&rect);

	CDC   dcMem;
	CBitmap*   pOldBitmap;

	dcMem.CreateCompatibleDC(pDC); 
	
	// 左边  
	pOldBitmap = dcMem.SelectObject(&m_LeftBmp);   
	
	pDC->BitBlt(rect.left,
		rect.top,
		m_LeftBm.bmWidth,
		rect.bottom-rect.top,
		&dcMem,
		0,
		m_LeftBm.bmHeight-(rect.bottom-rect.top),
		SRCCOPY);

	// 右边
	dcMem.SelectObject(&m_RightBmp);   
	
	pDC->BitBlt(
		rect.right - m_RightBm.bmWidth,
		rect.top,
		m_RightBm.bmWidth,
		rect.bottom-rect.top,
		&dcMem,
		0,
		m_LeftBm.bmHeight-(rect.bottom-rect.top),
		SRCCOPY);

	//中间
	if (m_LeftBm.bmWidth+m_RightBm.bmWidth < rect.Width())
	{
		dcMem.SelectObject(m_CenterBmp);
		
		pDC->StretchBlt(
			m_LeftBm.bmWidth,
			rect.top,
			rect.Width()-m_LeftBm.bmWidth-m_RightBm.bmWidth,
			rect.bottom-rect.top,
			&dcMem,
			0,
			m_CenterBm.bmHeight-(rect.bottom-rect.top),
			m_CenterBm.bmWidth,
			rect.bottom-rect.top,
			SRCCOPY);
	}

	////名称
	//if (m_NameBm.bmWidth < rect.Width())
	//{
	//	dcMem.SelectObject(m_NameBmp);

	//	pDC->BitBlt(
	//		(rect.Width()+m_LeftBm.bmWidth-m_RightBm.bmWidth-200)/2 - m_NameBm.bmWidth/2,
	//		rect.top,
	//		m_NameBm.bmWidth,
	//		rect.bottom-rect.top,
	//		&dcMem,
	//		0,
	//		m_NameBm.bmHeight-(rect.bottom-rect.top),
	//		SRCCOPY);
	//}

	////顶边
	//dcMem.SelectObject(&m_UpBmp);   

	//pDC->StretchBlt(rect.left,rect.top,
	//	rect.right-rect.left,m_UpBm.bmHeight,
	//	&dcMem,
	//	0,0,
	//	m_UpBm.bmWidth,m_UpBm.bmHeight,
	//	SRCCOPY);


	//底边
	dcMem.SelectObject(&m_BottomBmp);   

	pDC->StretchBlt(rect.left,rect.bottom-m_BottomBm.bmHeight,
		rect.right-rect.left,m_BottomBm.bmHeight,
		&dcMem,
		0,0,
		m_BottomBm.bmWidth,m_BottomBm.bmHeight,
		SRCCOPY);

	dcMem.SelectObject(pOldBitmap);

	return true;
}

BOOL CDlgMenu::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	m_LeftBmp.LoadBitmap(IDB_BITMAP_MENU_LEFT);
	m_CenterBmp.LoadBitmap(IDB_BITMAP_MENU_CENTER);

#if VM_SJ_CLIENT_VERSION
	{
		m_RightBmp.LoadBitmap(IDB_BITMAP_MENU_SJ_RIGHT);
	}
#else
	{
		m_RightBmp.LoadBitmap(IDB_BITMAP_MENU_RIGHT);
	}
#endif

	m_NameBmp.LoadBitmap(IDB_BITMAP_MENU_NAME);
	m_UpBmp.LoadBitmap(IDB_BITMAP_MENU_UP_BK);
	m_BottomBmp.LoadBitmap(IDB_BITMAP_MENU_BOTTOM_BK);
	m_LeftBmp.GetObject(sizeof(BITMAP),(LPVOID)&m_LeftBm);
	m_CenterBmp.GetObject(sizeof(BITMAP),(LPVOID)&m_CenterBm);
	m_RightBmp.GetObject(sizeof(BITMAP),(LPVOID)&m_RightBm); 
	m_NameBmp.GetObject(sizeof(BITMAP),(LPVOID)&m_NameBm);
	m_BottomBmp.GetObject(sizeof(BITMAP),(LPVOID)&m_BottomBm);
	m_UpBmp.GetObject(sizeof(BITMAP),(LPVOID)&m_UpBm);

	//////////////////////////////////////////////////////////////////////////
	m_MenuQuit.LoadBitmap(IDB_BITMAP_MENU_QUIT, 0);
	m_MenuTray.LoadBitmap(IDB_BITMAP_MENU_TRAY, 0);
	m_MenuMin.LoadBitmap(IDB_BITMAP_MENU_MIN, 0);
	m_MenuEmap.LoadBitmap(IDB_BITMAP_MENU_ELECMAP, 0);
	m_MenuView.LoadBitmap(IDB_BITMAP_MENU_MULTIVIEW, 0);
	m_MenuTuneCycle.LoadBitmap(IDB_BITMAP_MENU_TUNECYCLE, 0);
	m_MenuEventlog.LoadBitmap(IDB_BITMAP_MENU_LOG, 0);
	m_MenuRealTalk.LoadBitmap(IDB_BITMAP_MENU_REALTALK, 0);
	m_MenuTempFavorite.LoadBitmap(IDB_BITMAP_MENU_FAVORITE, 0);
	m_MenuSystem.LoadBitmap(IDB_BITMAP_MENU_SYSSTRUCT, 0);
	m_MenuReplay.LoadBitmap(IDB_BITMAP_MENU_REPLAY, 0);
	m_MenuAlarmSet.LoadBitmap(IDB_BITMAP_MENU_ALARMSET, 0);
	m_MenuAlarmLog.LoadBitmap(IDB_BITMAP_MENU_ALARMLOG, 0);
	m_MenuNetStruct.LoadBitmap(IDB_BITMAP_MENU_NETSTRUCT, 0);
	m_MenuManage.LoadBitmap(IDB_BITMAP_MENU_MANAGE,0);
	m_MenuLogin.LoadBitmap(IDB_BITMAP_MENU_LOGIN,0);
	m_btnShowControl.LoadBitmap(IDB_BITMAP_SHOW_CONTROL,0);
	m_btnHelp.LoadBitmap(IDB_BITMAP_BTN_HELP,0);
	m_btnAlarmPreview.LoadBitmap(IDB_BITMAP_ALARM_PREVIEW,0);

	m_MenuElecMap.LoadBitmap(IDB_BITMAP_MENU_ELECMAP, 0);
	m_MenuDeviceLedger.LoadBitmap(IDB_BITMAP_MENU_DEVICELEDGER, 0);
	m_MenuLinkageSetting.LoadBitmap(IDB_BITMAP_MENU_NETSTRUCT,0);
	m_MenuAssistance.LoadBitmap(IDB_BITMAP_MENU_ASSVIEW,0);
	m_MenuElecMap.ShowWindow(SW_SHOW);
	m_MenuDeviceLedger.ShowWindow(SW_SHOW);
	m_MenuLinkageSetting.ShowWindow(SW_SHOW);
	m_MenuAssistance.ShowWindow(SW_SHOW);

	//m_MenuView.ShowWindow(SW_HIDE);
	//m_MenuReplay.ShowWindow(SW_HIDE);
	//m_MenuManage.ShowWindow(SW_HIDE);
	//m_MenuEventlog.ShowWindow(SW_HIDE);
	//m_MenuLogin.ShowWindow(SW_HIDE);

	m_MenuView.ShowWindow(SW_SHOW);
	m_MenuReplay.ShowWindow(SW_SHOW);
	m_MenuManage.ShowWindow(SW_SHOW);
	m_MenuEventlog.ShowWindow(SW_SHOW);
	m_MenuLogin.ShowWindow(SW_SHOW);

	m_btnShowControl.ShowWindow(SW_SHOW);
	m_MenuQuit.ShowWindow(SW_SHOW);
	m_MenuMin.ShowWindow(SW_SHOW);
	m_MenuEmap.ShowWindow(SW_HIDE);
	m_MenuTuneCycle.ShowWindow(SW_HIDE);
	m_MenuRealTalk.ShowWindow(SW_HIDE);
	m_MenuTempFavorite.ShowWindow(SW_HIDE);
	m_MenuSystem.ShowWindow(SW_HIDE);
	m_MenuAlarmSet.ShowWindow(SW_HIDE);
	m_MenuAlarmLog.ShowWindow(SW_HIDE);
	m_MenuNetStruct.ShowWindow(SW_HIDE);

	//char szWeatherInfo[512] = "窗口1->温度 , 湿度 , 风向 , 风力 , 降水 , 气压\r\n窗口2->温度 , 湿度 , 风向 , 风力 , 降水 , 气压\r\n窗口3->温度 , 湿度 , 风向 , 风力 , 降水 , 气压\r\n窗口4->温度 , 湿度 , 风向 , 风力 , 降水 , 气压\r\n";
	//m_staticShowWeather.SetText(szWeatherInfo,10,RGB(0,106,110));
	m_staticShowWeather.SetText(" ",10,RGB(0,106,110));

	m_staticUserInfo.SetText(" ",6,RGB(0,106,110));

	return TRUE; 
}

void CDlgMenu::OnBnClickedMenuMin()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgMenu::OnBnClickedBtnShowControl()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgMenu::OnBnClickedMenuEmap()
{
	// TODO: 在此添加控件通知处理程序代码
}
