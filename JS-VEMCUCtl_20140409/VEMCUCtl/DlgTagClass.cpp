// DlgTagClass.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgTagClass.h"


// CDlgTagClass 对话框

IMPLEMENT_DYNAMIC(CDlgTagClass, CDialog)

CDlgTagClass::CDlgTagClass(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTagClass::IDD, pParent)
	, m_strStationName(_T(""))
	, m_strStationNum(_T(""))
	, m_strClass(_T(""))
{
	m_hTreeItem = NULL;
}

CDlgTagClass::~CDlgTagClass()
{

}

void CDlgTagClass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STATION_NAME, m_strStationName);
	DDV_MaxChars(pDX, m_strStationName, 255);
	DDX_Text(pDX, IDC_EDIT_CLASS, m_strClass);
	DDV_MaxChars(pDX, m_strClass, 255);
	DDX_Control(pDX, IDC_BTN_MODIFY, m_btnModify);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CDlgTagClass, CDialog)
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CDlgTagClass::OnBnClickedBtnModify)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgTagClass::OnBnClickedBtnCancel)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CDlgTagClass 消息处理程序

BOOL CDlgTagClass::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	LoadSkin();


	return TRUE; 
}

BOOL CDlgTagClass::LoadSkin()
{
	m_Bmp.LoadBitmap(IDB_BITMAP_TAG_CLASS_BG);
	m_btnModify.LoadBitmap(IDB_BITMAP_MODIFY_BTN_BK);
	m_btnCancel.LoadBitmap(IDB_BITMAP_CANCEL_BTN_BK);
	return TRUE;
}

void CDlgTagClass::OnBnClickedBtnModify()
{
	UpdateData(TRUE);
	OnOK();
}

void CDlgTagClass::OnBnClickedBtnCancel()
{
	OnCancel();
}

void CDlgTagClass::OnDestroy()
{
	CDialog::OnDestroy();

	//////////////////////////////////////////////////////////////////////////

}

BOOL CDlgTagClass::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)//屏住ESC键
		return	TRUE;
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_RETURN)//屏住ENTER键
		return	TRUE; 

	//////////////////////////////////////////////////////////////////////////
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgTagClass::OnEraseBkgnd(CDC* pDC)
{
	CRect   rect;
	GetClientRect(&rect);

	CDC   dcMem;
	CBitmap*  pOldBitmap = NULL;

	BITMAP   bm;
	m_Bmp.GetObject(sizeof(BITMAP),(LPVOID)&bm);

	dcMem.CreateCompatibleDC(pDC);   
	pOldBitmap = dcMem.SelectObject(&m_Bmp);   

	pDC->BitBlt(rect.left,rect.top,bm.bmWidth,bm.bmHeight,&dcMem,0,0,SRCCOPY);

	dcMem.SelectObject(pOldBitmap);

	return TRUE;
}

LRESULT CDlgTagClass::OnNcHitTest(CPoint point)
{
	return HTCAPTION;
}
