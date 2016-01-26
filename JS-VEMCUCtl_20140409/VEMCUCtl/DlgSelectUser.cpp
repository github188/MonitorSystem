// DlgSelectUser.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgSelectUser.h"
#include "VMTypeDefine.h"


// CDlgSelectUser 对话框

IMPLEMENT_DYNAMIC(CDlgSelectUser, CDialog)

CDlgSelectUser::CDlgSelectUser(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectUser::IDD, pParent)
{

}

CDlgSelectUser::~CDlgSelectUser()
{
}

void CDlgSelectUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_USER, m_treeUser);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_EXIT2, m_btnExit);
}


BEGIN_MESSAGE_MAP(CDlgSelectUser, CDialog)
	ON_BN_CLICKED(IDC_BTN_OK, &CDlgSelectUser::OnBnClickedBtnOk)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_USER, &CDlgSelectUser::OnNMDblclkTreeUser)
	ON_BN_CLICKED(IDC_BTN_EXIT2, &CDlgSelectUser::OnBnClickedBtnExit2)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgSelectUser 消息处理程序

BOOL CDlgSelectUser::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	InitControl();
	LoadSkin();

	//////////////////////////////////////////////////////////////////////////

	return TRUE; 
}

BOOL CDlgSelectUser::LoadSkin()
{
	m_btnOK.LoadBitmap(IDB_BITMAP_SELECT,0);
	m_btnExit.LoadBitmap(IDB_BITMAP_MENU_QUIT,0);

	m_Bmp.LoadBitmap(IDB_BITMAP_SELECT_USER_BK);
	m_Bmp.GetObject(sizeof(BITMAP),(LPVOID)&m_bm);   

	return TRUE;
}

BOOL CDlgSelectUser::InitControl()
{
	try
	{
		m_strUserName = "全部用户";

		m_treeUser.SetBkColor(RGB(239, 246, 253));
		m_treeUser.SetTextColor(RGB(36, 162, 146));

		m_treeUser.DeleteAllItems();

		int nLoginType = g_userpower.logintype;
		int nUserType = g_userpower.usertype;
		int nUserID = g_userpower.userID;
		
		HTREEITEM hParentItem = m_treeUser.InsertItem("全部用户");
		HTREEITEM hItem = NULL;

		MYSQL_RES	* res ;
		MYSQL_ROW	row ;

		char sql_buf[1024] = {0};
		char szUserName[256] = {0};

		//读取用户信息
		int nCount = 0;

		if (nLoginType == VM_USER_TYPE_SD_LOGIN)//省调
		{
			if (nUserType == VM_USER_TYPE_SD_SJ_LEADER||nUserType == VM_USER_TYPE_SD_SJ_ADMIN)//管理员
			{
				sprintf_s(sql_buf, "SELECT user_name FROM ct_user WHERE user_type!=1 AND login_type=0 ORDER by user_id ASC");
			}
			else //一般用户
			{
				sprintf_s(sql_buf, "SELECT user_name FROM ct_user WHERE user_id=%d AND login_type=0 ORDER by user_id ASC",nUserID);
			}

		}
		else if (nLoginType == VM_USER_TYPE_SJ_LOGIN)//省检
		{
			if (nUserType == VM_USER_TYPE_SD_SJ_LEADER||nUserType == VM_USER_TYPE_SD_SJ_ADMIN)//本部
			{
				sprintf_s(sql_buf, "SELECT user_name FROM ct_user WHERE login_type=1 ORDER by user_id ASC");
			}
			else //分部
			{
				sprintf_s(sql_buf, "SELECT user_name FROM ct_user WHERE login_type=1 AND user_id=%d ORDER by user_id ASC",nUserID);
			}
		}
		else if (nLoginType == VM_USER_TYPE_ADMIN_LOGIN)
		{
			sprintf_s(sql_buf, "SELECT user_name FROM ct_user WHERE user_type!=1 ORDER by user_id ASC");
		}
		else
		{
			return FALSE;
		}
		
		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);
			while (row = mysql_fetch_row(res))
			{
				strcpy_s(szUserName,sizeof(szUserName),row[0]);
				m_treeUser.InsertItem(szUserName,hParentItem);
			}
			mysql_free_result(res) ;
		}

		hItem = m_treeUser.GetChildItem(hParentItem);
		m_treeUser.EnsureVisible(hItem);

		UpdateData(FALSE);

		return TRUE;
	}
	catch(...)
	{

	}

	return FALSE;
}

void CDlgSelectUser::OnBnClickedBtnOk()
{
	UpdateData(TRUE);

	CTreeCtrl *pList = (CTreeCtrl *)GetDlgItem(IDC_TREE_USER);
	if (pList == NULL)
		return;

	HTREEITEM hItem = pList->GetSelectedItem();
	if (hItem == NULL)
		return;

	m_strUserName = m_treeUser.GetItemText(hItem);

	UpdateData(FALSE);

	OnOK();
}

void CDlgSelectUser::OnNMDblclkTreeUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	//////////////////////////////////////////////////////////////////////////
	CTreeCtrl *pList = (CTreeCtrl *)GetDlgItem(IDC_TREE_USER);
	if (pList == NULL)
		return;

	HTREEITEM hItem = pList->GetSelectedItem();
	if (hItem == NULL)
		return;

	m_strUserName = m_treeUser.GetItemText(hItem);

	UpdateData(FALSE);

	//////////////////////////////////////////////////////////////////////////
	*pResult = 0;
}

void CDlgSelectUser::OnBnClickedBtnExit2()
{
	OnCancel();
}

BOOL CDlgSelectUser::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP) 
		&& (pMsg->wParam == VK_ESCAPE || pMsg->wParam== VK_RETURN || pMsg->wParam == VK_SPACE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgSelectUser::OnEraseBkgnd(CDC* pDC)
{
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
