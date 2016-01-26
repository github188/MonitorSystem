// DlgStationAssistPng.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/DlgStationAssistPng.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CDlgStationAssistPng 对话框

IMPLEMENT_DYNAMIC(CDlgStationAssistPng, CPropertyPage)

CDlgStationAssistPng::CDlgStationAssistPng(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgStationAssistPng::IDD)
	, m_strPngName(_T(""))
	, m_fRotateAngle(0)
	, m_bCheckHorizontalStretch(FALSE)
	, m_bCheckVerticalStretch(FALSE)
{

}

CDlgStationAssistPng::~CDlgStationAssistPng()
{
}

void CDlgStationAssistPng::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PNG_NAME, m_strPngName);
	DDV_MaxChars(pDX, m_strPngName, 256);
	DDX_Text(pDX, IDC_EDIT_ROTATE_ANGLE, m_fRotateAngle);
	DDX_Check(pDX, IDC_CHECK_HORIZONTAL_STRETCH, m_bCheckHorizontalStretch);
	DDX_Check(pDX, IDC_CHECK_VERTICAL_STRETCH, m_bCheckVerticalStretch);
}

BEGIN_MESSAGE_MAP(CDlgStationAssistPng, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_SELECT_PNG, &CDlgStationAssistPng::OnBnClickedBtnSelectPng)
END_MESSAGE_MAP()

// CDlgStationAssistPng 消息处理程序

BOOL CDlgStationAssistPng::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgStationAssistPng::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CDlgStationAssistPng::OnBnClickedBtnSelectPng()
{
	CFileDialog DlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("PNG Files (*.png)|*.png||"), NULL);

	if (DlgFile.DoModal())
	{
		m_strPngName = DlgFile.GetFileName();
		UpdateData(FALSE);
	}
}

void CDlgStationAssistPng::OnOK()
{
	UpdateData(TRUE);

	CPropertyPage::OnOK();
}
