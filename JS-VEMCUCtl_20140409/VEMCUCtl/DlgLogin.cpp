// DlgLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgLogin.h"
#include "DlgModifyPassword.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDlgLogin 对话框

IMPLEMENT_DYNAMIC(CDlgLogin, CDialog)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogin::IDD, pParent)
	, m_nUserID(-1)
	, m_Login_UserName(_T("admin"))
	, m_Login_Password(_T(""))
{

}

CDlgLogin::~CDlgLogin()
{

}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USER, m_Login_UserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_Login_Password);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, m_btnLogIn);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_USER, m_editUser);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_BTN_MODIFY_PASSWORD, m_btnModifyPassword);
}

BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CDlgLogin::OnBnClickedLogin)
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_USER, &CDlgLogin::OnEnChangeEditUser)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, &CDlgLogin::OnEnChangeEditPassword)
	ON_BN_CLICKED(IDC_EXIT, &CDlgLogin::OnBnClickedExit)
	ON_BN_CLICKED(IDC_BTN_MODIFY_PASSWORD, &CDlgLogin::OnBnClickedBtnModifyPassword)
	ON_BN_CLICKED(IDCANCEL, &CDlgLogin::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgLogin 消息处理程序
void CDlgLogin::OnBnClickedLogin()
{
	UpdateData(true);

	if (m_Login_UserName.IsEmpty()||m_Login_Password.IsEmpty())
	{
		MessageBox("输入的用户或密码不能为空,请重新输入！","视频监视");
		return;
	}
	
	m_nUserID = -1;

	char sql_buf[1024] = {0};
	
	MYSQL_RES	*res ;
	MYSQL_ROW	row ;

	sprintf_s(sql_buf, "SELECT user_id, user_name, pass_word , user_type, videoview_p, ytcontrol_p, replay_p, historylog_p, devicemanager_p, tunecycle_p, usermanager_p, eventlevel_p, user_level,login_type,group_no,sub_group_no,group_name  FROM ct_user where user_name='%s' and pass_word='%s' order by user_id",
		m_Login_UserName, m_Login_Password);

	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);

		CString i_CallnumState;
		CString i_UserID;
		while ( row = mysql_fetch_row( res ) )
		{
			m_nUserID = g_userpower.userID = atoi(row[0]);
			sprintf(g_userpower.username, "%s", row[1]);
			sprintf(g_userpower.password, "%s", row[2]);
			g_userpower.usertype = atoi(row[3]);
			g_userpower.userCP.videoview = atoi(row[4]);
			g_userpower.userCP.ytcontrol = atoi(row[5]);
			g_userpower.userCP.replay = atoi(row[6]);
			g_userpower.userCP.historylog = atoi(row[7]);
			g_userpower.userCP.devicemanage = atoi(row[8]);
			g_userpower.userCP.tunecycle = atoi(row[9]);
			g_userpower.userCP.usermanage = atoi(row[10]);
			g_userpower.userCP.eventlevel = atoi(row[11]);
			g_userpower.userlevel = atoi(row[12]);
			g_userpower.logintype = atoi(row[13]);
			g_userpower.groupno = atoi(row[14]);
			g_userpower.subgroupno = atoi(row[15]);
			sprintf(g_userpower.groupname,"%s",row[16]);
		}
		mysql_free_result( res ) ;
	}
	else
	{
		//MessageBox("用户数据表错误！","视频监视");
		MessageBox("输入的用户或密码错误,请重新输入！","视频监视");
		return;
	}

	if (m_nUserID == -1)
	{
		MessageBox("输入的用户或密码错误,请重新输入！","视频监视");
		return;
	}

	if (g_userpower.logintype == 2)// 表示按照默认方式，admin
	{
		g_nUserType = 1;
		g_nUserResourceShowType = 1;
	}
	else if (g_userpower.logintype == 1)// 表示按省检管理结构方式，sjadmin
	{
		g_nUserType = 2;
		g_nUserResourceShowType = 2;
	}
	else if (g_userpower.logintype == 0)// 表示省调电压等级方式，sdadmin
	{
		g_nUserType = 3;
		g_nUserResourceShowType = 3;
	}
	else
	{
		g_nUserType = 3;
		g_nUserResourceShowType = 3;
	}

	CDlgLogin::OnOK();
}


BOOL CDlgLogin::LoadSkin()
{
#if VM_SJ_CLIENT_VERSION
	{
		m_Bmp.LoadBitmap(IDB_BITMAP_LOGIN_SJ);
	}
#else
	{
		m_Bmp.LoadBitmap(IDB_BITMA_LOGIN);
	}
#endif
	
	m_Bmp.GetObject(sizeof(BITMAP),(LPVOID)&m_bm);
	m_btnLogIn.LoadBitmap(IDB_BITMAP_USER_LOGIN);
	m_btnCancel.LoadBitmap(IDB_BITMAP_USER_CANCEL);
	m_btnExit.LoadBitmap(IDB_BITMAP_LOGIN_EXIT);
	//m_btnModifyPassword.LoadBitmap(IDB_BITMAP_MODIFY_PASSWORD_BK);
	m_btnModifyPassword.LoadBitmap(IDB_BITMAP_MODIFY_PASSWORD_BK);
	return TRUE;
}

BOOL CDlgLogin::SetDlgRgn()
{
	HRGN hbmpRgn = NULL;
	hbmpRgn = m_Bmp.BitmapToRegion(RGB(255,0,255),RGB(230,200,230));
	SetWindowRgn(hbmpRgn,TRUE);

	if (hbmpRgn != NULL)
	{
		DeleteObject(hbmpRgn);
		hbmpRgn = NULL;
	}

	return TRUE;
}

BOOL CDlgLogin::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==0x100   &&   pMsg->wParam==27)//屏住ESC
		return	TRUE;

	if(pMsg->message==0x100   &&   pMsg->wParam==13)//屏住ENTER
	{
		OnBnClickedLogin();
		return	TRUE;
	}

	if(pMsg->message==0x100   &&   pMsg->wParam==VK_SPACE)
		return	TRUE;


	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgLogin::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	SetWindowPos(&CWnd::wndTopMost, 0, 0, 800, 600, SWP_NOMOVE /*|SWP_NOSIZE*/);

	m_BgBrush.CreateSolidBrush(RGB(250, 250, 250));         // 背景的颜色

	LoadSkin();

	SetDlgRgn();

	if (strlen(g_userpower.username) != 0)
	{
		m_Login_UserName = g_userpower.username;
	}

	UpdateData(FALSE);

	return TRUE;
}

BOOL CDlgLogin::OnEraseBkgnd(CDC* pDC)
{
	CRect   rect;
	GetClientRect(&rect);

	CDC   dcMem;
	CBitmap*  pOldBitmap = NULL;

	dcMem.CreateCompatibleDC(pDC);   
	pOldBitmap = dcMem.SelectObject(&m_Bmp);   

	pDC->BitBlt(rect.left,rect.top,m_bm.bmWidth,m_bm.bmHeight,&dcMem,0,0,SRCCOPY);

	dcMem.SelectObject(pOldBitmap);

	return TRUE;
}

LRESULT CDlgLogin::OnNcHitTest(CPoint pt)
{
	return HTCAPTION;

	//CRect rc;
	//GetClientRect(&rc);
	//ClientToScreen(&rc);
	//return rc.PtInRect(pt) ? HTCAPTION : CDialog::OnNcHitTest(pt);
}

HBRUSH CDlgLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//////////////////////////////////////////////////////////////////////////
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_USER)
	{
		// 背景色透明
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(10,10,10));
		
		// 返回背景色的画刷
		return m_BgBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_PASSWORD)
	{
		// 背景色透明
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(10,10,10));
		
		// 返回背景色的画刷
		return m_BgBrush;
	}

	return hbr;
}

void CDlgLogin::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (GetDlgItem(IDC_BUTTON_LOGIN)->GetSafeHwnd())
	{	
		CWnd *pWnd;

		pWnd = GetDlgItem(IDC_BUTTON_LOGIN);
		if (pWnd)
		{
			CRect rect;

			rect.top = 440;
			rect.left = 470;
			rect.right = 560;
			rect.bottom = 480;

			ScreenToClient(&rect);

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDCANCEL);
		if (pWnd)
		{
			CRect rect;

			rect.top = 440;
			rect.left = 600;
			rect.right = 690;
			rect.bottom = 480;

			ScreenToClient(&rect);

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDC_BTN_MODIFY_PASSWORD);
		if (pWnd)
		{
			CRect rect;

			rect.top = 387;
			rect.left =  726;
			rect.right = 755;
			rect.bottom = 415;

			ScreenToClient(&rect);

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDC_EDIT_USER);
		if (pWnd)
		{
			CRect rect;

			rect.top = 341;
			rect.left = 524;
			rect.right = 714;
			rect.bottom = 369;

			ScreenToClient(&rect);

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDC_EDIT_PASSWORD);
		if (pWnd)
		{
			CRect rect;

			rect.top = 381;
			rect.left =  524;
			rect.right = 714;
			rect.bottom = 409;

			ScreenToClient(&rect);

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDC_EXIT);
		if (pWnd)
		{
			CRect rect;

			rect.top = 6;
			rect.left =  766;
			rect.right = 790;
			rect.bottom = 40;

			ScreenToClient(&rect);

			pWnd->MoveWindow(rect);		
		}
	}

	/*if (GetDlgItem(IDC_BUTTON_LOGIN)->GetSafeHwnd())
	{	
		CWnd *pWnd;

		pWnd = GetDlgItem(IDC_BUTTON_LOGIN);
		if (pWnd)
		{
			CRect rect;

			rect.top = 660;
			rect.left = 860;
			rect.right = 950;
			rect.bottom = 700;

			ScreenToClient(&rect);

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDCANCEL);
		if (pWnd)
		{
			CRect rect;

			rect.top = 660;
			rect.left = 980;
			rect.right = 1070;
			rect.bottom = 700;

			ScreenToClient(&rect);

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDC_BTN_MODIFY_PASSWORD);
		if (pWnd)
		{
			CRect rect;

			rect.top = 596;
			rect.left = 1110;
			rect.right = 1140;
			rect.bottom = 624;

			ScreenToClient(&rect);

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDC_EDIT_USER);
		if (pWnd)
		{
			CRect rect;

			rect.top = 539;
			rect.left = 842;
			rect.right = 1080;
			rect.bottom = 560;

			ScreenToClient(&rect);

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDC_EDIT_PASSWORD);
		if (pWnd)
		{
			CRect rect;

			rect.top = 594;
			rect.left = 842;
			rect.right = 1080;
			rect.bottom = 620;

			ScreenToClient(&rect);

			pWnd->MoveWindow(rect);		
		}*/

	//}

	//////////////////////////////////////////////////////////////////////////
}

void CDlgLogin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	//////////////////////////////////////////////////////////////////////////

}

void CDlgLogin::OnEnChangeEditUser()
{
	m_editUser.Invalidate(TRUE);
}

void CDlgLogin::OnEnChangeEditPassword()
{
	m_editPassword.Invalidate(TRUE);
}

void CDlgLogin::OnBnClickedExit()
{
	OnCancel();
}

void CDlgLogin::OnBnClickedBtnModifyPassword()
{
	UpdateData(TRUE);

	if (g_userpower.userID < 0)
	{
		MessageBox("用户请先登录","视频监视");
		return;
	}

	CDlgModifyPassword DlgModifyPassword;
	DlgModifyPassword.m_strUserName = g_userpower.username;
	
	DlgModifyPassword.DoModal();
}

void CDlgLogin::OnBnClickedCancel()
{
	OnCancel();
}
