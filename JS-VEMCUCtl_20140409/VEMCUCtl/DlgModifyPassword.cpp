// DlgModifyPassword.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgModifyPassword.h"


// CDlgModifyPassword 对话框

IMPLEMENT_DYNAMIC(CDlgModifyPassword, CDialog)

CDlgModifyPassword::CDlgModifyPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifyPassword::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strUserPassword(_T(""))
	, m_strUserNewPassword(_T(""))
	, m_strUserNewPassword2(_T(""))
{

}

CDlgModifyPassword::~CDlgModifyPassword()
{
}

void CDlgModifyPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strUserPassword);
	DDX_Text(pDX, IDC_EDIT_NEW_PASSWORD, m_strUserNewPassword);
	DDX_Text(pDX, IDC_EDIT_NEW_PASSWORD2, m_strUserNewPassword2);
	DDX_Control(pDX, IDC_BTN_MODIFY, m_btnModify);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_EDIT_USER, m_editUser);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_EDIT_NEW_PASSWORD, m_editNewPassword);
	DDX_Control(pDX, IDC_EDIT_NEW_PASSWORD2, m_editNewPassword2);
}


BEGIN_MESSAGE_MAP(CDlgModifyPassword, CDialog)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CDlgModifyPassword::OnBnClickedBtnModify)
	ON_BN_CLICKED(IDCANCEL, &CDlgModifyPassword::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_EXIT, &CDlgModifyPassword::OnBnClickedExit)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_USER, &CDlgModifyPassword::OnEnChangeEditUser)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, &CDlgModifyPassword::OnEnChangeEditPassword)
	ON_EN_CHANGE(IDC_EDIT_NEW_PASSWORD, &CDlgModifyPassword::OnEnChangeEditNewPassword)
	ON_EN_CHANGE(IDC_EDIT_NEW_PASSWORD2, &CDlgModifyPassword::OnEnChangeEditNewPassword2)
END_MESSAGE_MAP()


// CDlgModifyPassword 消息处理程序

BOOL CDlgModifyPassword::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	m_BgBrush.CreateSolidBrush(RGB(250, 250, 250));         // 背景的颜色

	InitControl();
	LoadSkin();

	return TRUE;
}

BOOL CDlgModifyPassword::LoadSkin()
{
	m_Bmp.LoadBitmap(IDB_BITMAP_MODIFY_PASSWORD_DLG_BG);
	m_Bmp.GetObject(sizeof(BITMAP),(LPVOID)&m_bm);
	m_btnModify.LoadBitmap(IDB_BITMAP_MODIFY_BTN_BK);
	m_btnCancel.LoadBitmap(IDB_BITMAP_CANCEL_BTN_BK);
	return TRUE;
}

BOOL CDlgModifyPassword::InitControl()
{
	return TRUE;
}

BOOL CDlgModifyPassword::OnEraseBkgnd(CDC* pDC)
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

void CDlgModifyPassword::OnBnClickedBtnModify()
{
	UpdateData(TRUE);

	if (m_strUserName.IsEmpty())
	{
		MessageBox("用户名不能为空","修改密码");
		return;
	}

	if (m_strUserPassword.IsEmpty())
	{
		MessageBox("用户密码不能为空","修改密码");
		return;
	}

	if (m_strUserNewPassword.IsEmpty())
	{
		MessageBox("用户新密码不能为空","修改密码");
		return;
	}

	if (m_strUserNewPassword2.IsEmpty())
	{
		MessageBox("用户确认密码不能为空","修改密码");
		return;
	}

	if (m_strUserNewPassword.Compare(m_strUserNewPassword2) != 0)
	{
		MessageBox("用户新密码与确认密码不一致","修改密码");
		return;
	}

	if (g_userpower.userID < 0)
	{
		MessageBox("请先登录系统","修改密码");
		return;
	}

	if (m_strUserName.Compare(g_userpower.username) != 0)
	{
		MessageBox("用户只能修改自己的密码","修改密码");
		return;
	}

	if (m_strUserPassword.Compare(g_userpower.password) != 0)
	{
		MessageBox("用户密码不正确","修改密码");
		return;
	}

	char szUserPassword[256] = {0};
	sprintf_s(szUserPassword,sizeof(szUserPassword),"%s",m_strUserPassword);

	char *pStr = NULL;

	pStr = strtok(szUserPassword,",. \t\"\'\r\n");
	if (pStr != NULL)
	{
		MessageBox("用户密码不能有特殊字符","修改密码");
		return;
	}

	char sql_buf[1024] = {0};

	sprintf_s(sql_buf, "UPDATE ct_user SET pass_word='%s' WHERE user_id=%d ",
		m_strUserNewPassword, g_userpower.userID);

	if (!mysql_query(g_mySqlData, sql_buf))
	{
		MessageBox("用户密码修改成功！","修改密码");
		OnCancel();
	}
	else
	{
		MessageBox("用户密码修改错误！","修改密码");
	}
}

void CDlgModifyPassword::OnBnClickedCancel()
{
	OnCancel();
}

void CDlgModifyPassword::OnBnClickedExit()
{

}

HBRUSH CDlgModifyPassword::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_NEW_PASSWORD)
	{
		// 背景色透明
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(10,10,10));

		// 返回背景色的画刷
		return m_BgBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_NEW_PASSWORD2)
	{
		// 背景色透明
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(10,10,10));

		// 返回背景色的画刷
		return m_BgBrush;
	}

	//////////////////////////////////////////////////////////////////////////
	return hbr;
}

void CDlgModifyPassword::OnEnChangeEditUser()
{
	m_editUser.Invalidate(TRUE);
}

void CDlgModifyPassword::OnEnChangeEditPassword()
{
	m_editPassword.Invalidate(TRUE);
}

void CDlgModifyPassword::OnEnChangeEditNewPassword()
{
	m_editNewPassword.Invalidate(TRUE);
}

void CDlgModifyPassword::OnEnChangeEditNewPassword2()
{
	m_editNewPassword2.Invalidate(TRUE);
}

BOOL CDlgModifyPassword::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)//屏住ESC键
		return	TRUE;
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_RETURN)//屏住ENTER键
		return	TRUE; 
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_SPACE)//屏蔽空格键
		return	TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}
