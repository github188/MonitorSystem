// PropUnitExtendDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropUnitExtendDlg.h"


// CPropUnitExtendDlg 对话框

IMPLEMENT_DYNAMIC(CPropUnitExtendDlg, CPropertyPage)
CPropUnitExtendDlg::CPropUnitExtendDlg()
	: CPropertyPage(CPropUnitExtendDlg::IDD)
	, m_strDes(_T(""))
	, m_strEname(_T(""))
	, m_bUnExt_SGBW(FALSE)
	, m_bUnExt_SGBW_Event(FALSE)
	, m_bUnExt_MC(FALSE)
	, m_bRefresh(FALSE)
	, m_bEpd(FALSE)
{
}

CPropUnitExtendDlg::~CPropUnitExtendDlg()
{
}

void CPropUnitExtendDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strDes);
	DDX_Text(pDX, IDC_EDIT2, m_strEname);
	DDX_Check(pDX, IDC_CHECK1, m_bUnExt_SGBW);
	DDV_MaxChars(pDX, m_strDes, 17);
	DDV_MaxChars(pDX, m_strEname, 17);
	DDX_Check(pDX, IDC_CHECK2, m_bUnExt_SGBW_Event);
	DDX_Check(pDX, IDC_CHECK3, m_bUnExt_MC);
	DDX_Check(pDX, IDC_CHECK4, m_bRefresh);
	DDX_Check(pDX, IDC_CHECK7, m_bEpd);
}


BEGIN_MESSAGE_MAP(CPropUnitExtendDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CPropUnitExtendDlg 消息处理程序

void CPropUnitExtendDlg::OnBnClickedButton1()//挑点
{
	
}

void CPropUnitExtendDlg::GetUnitExtParam(char* des ,char* ename ,bool& bCallSGBW ,bool& bEventSGBW ,bool& bCallMC,bool& bRefresh,bool& bEpd)
{
	strcpy(des ,m_strDes) ;
	strcpy(ename ,m_strEname) ;
	bCallSGBW = m_bUnExt_SGBW ;
	bEventSGBW = m_bUnExt_SGBW_Event ;
	bCallMC = m_bUnExt_MC ;
	bRefresh = m_bRefresh ;
	bEpd=m_bEpd;
}

void CPropUnitExtendDlg::SetUnitExtParam(char* des ,char* ename ,bool bCallSGBW ,bool bEventSGBW ,bool bCallMC,bool bRefresh,bool bEpd) 
{
	m_strDes = des ;
	m_strEname = ename ;
	m_bUnExt_SGBW = bCallSGBW ;
	m_bUnExt_SGBW_Event = bEventSGBW ;
	m_bUnExt_MC = bCallMC ;
	m_bRefresh = bRefresh ;
	m_bEpd=bEpd;
}
