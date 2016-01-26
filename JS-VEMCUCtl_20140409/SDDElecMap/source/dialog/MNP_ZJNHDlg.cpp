// MNP_ZJNHDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/MNP_ZJNHDlg.h"


// CMNP_ZJNHDlg 对话框

IMPLEMENT_DYNAMIC(CMNP_ZJNHDlg, CDialog)
CMNP_ZJNHDlg::CMNP_ZJNHDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMNP_ZJNHDlg::IDD, pParent)
{
	Event1_Enable=FALSE;
	Event2_Enable=FALSE;
	Event3_Enable=FALSE;
	Event4_Enable=FALSE;
	Event5_Enable=FALSE;
	Event6_Enable=FALSE;
	Event7_Enable=FALSE;
	Event8_Enable=FALSE;
	Event9_Enable=FALSE;
	Event10_Enable=FALSE;
	Event11_Enable=FALSE;
	Event12_Enable=FALSE;
	Event13_Enable=FALSE;
	Event14_Enable=FALSE;
	Event15_Enable=FALSE;
	Event16_Enable=FALSE;
	Event17_Enable=FALSE;
}

CMNP_ZJNHDlg::~CMNP_ZJNHDlg()
{

}

void CMNP_ZJNHDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MNPCHECK1, m_Check1);
	DDX_Control(pDX, IDC_MNPCHECK2, m_Check2);
	DDX_Control(pDX, IDC_MNPCHECK3, m_Check3);
	DDX_Control(pDX, IDC_MNPCHECK4, m_Check4);
	DDX_Control(pDX, IDC_MNPCHECK5, m_Check5);
	DDX_Control(pDX, IDC_MNPCHECK6, m_Check6);
	DDX_Control(pDX, IDC_MNPCHECK7, m_Check7);
	DDX_Control(pDX, IDC_MNPCHECK8, m_Check8);
	DDX_Control(pDX, IDC_MNPCHECK9, m_Check9);
	DDX_Control(pDX, IDC_MNPCHECK10, m_Check10);
	DDX_Control(pDX, IDC_MNPCHECK11, m_Check11);
	DDX_Control(pDX, IDC_MNPCHECK12, m_Check12);
	DDX_Control(pDX, IDC_MNPCHECK13, m_Check13);
	DDX_Control(pDX, IDC_MNPCHECK14, m_Check14);
	DDX_Control(pDX, IDC_MNPCHECK15, m_Check15);
	DDX_Control(pDX, IDC_MNPCHECK16, m_Check16);
	DDX_Control(pDX, IDC_MNPCHECK17, m_Check17);
}


BEGIN_MESSAGE_MAP(CMNP_ZJNHDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CMNP_ZJNHDlg 消息处理程序

void CMNP_ZJNHDlg::SetEvent1Enable(BOOL Enable)
{
	Event1_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent2Enable(BOOL Enable)
{
	Event2_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent3Enable(BOOL Enable)
{
	Event3_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent4Enable(BOOL Enable)
{
	Event4_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent5Enable(BOOL Enable)
{
	Event5_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent6Enable(BOOL Enable)
{
	Event6_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent7Enable(BOOL Enable)
{
	Event7_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent8Enable(BOOL Enable)
{
	Event8_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent9Enable(BOOL Enable)
{
	Event9_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent10Enable(BOOL Enable)
{
	Event10_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent11Enable(BOOL Enable)
{
	Event11_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent12Enable(BOOL Enable)
{
	Event12_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent13Enable(BOOL Enable)
{
	Event13_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent14Enable(BOOL Enable)
{
	Event14_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent15Enable(BOOL Enable)
{
	Event15_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent16Enable(BOOL Enable)
{
	Event16_Enable=Enable;
}

void CMNP_ZJNHDlg::SetEvent17Enable(BOOL Enable)
{
	Event17_Enable=Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent1Enable(void)
{
	return Event1_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent2Enable(void)
{
	return Event2_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent3Enable(void)
{
	return Event3_Enable;
}
BOOL CMNP_ZJNHDlg::GetEvent4Enable(void)
{
	return Event4_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent5Enable(void)
{
	return Event5_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent6Enable(void)
{
	return Event6_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent7Enable(void)
{
	return Event7_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent8Enable(void)
{
	return Event8_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent9Enable(void)
{
	return Event9_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent10Enable(void)
{
	return Event10_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent11Enable(void)
{
	return Event11_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent12Enable(void)
{
	return Event12_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent13Enable(void)
{
	return Event13_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent14Enable(void)
{
	return Event14_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent15Enable(void)
{
	return Event15_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent16Enable(void)
{
	return Event16_Enable;
}

BOOL CMNP_ZJNHDlg::GetEvent17Enable(void)
{
	return Event17_Enable;
}

BOOL CMNP_ZJNHDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Check1.SetCheck(Event1_Enable);
	m_Check2.SetCheck(Event2_Enable);
	m_Check3.SetCheck(Event3_Enable);
	m_Check4.SetCheck(Event4_Enable);
	m_Check5.SetCheck(Event5_Enable);
	m_Check6.SetCheck(Event6_Enable);
	m_Check7.SetCheck(Event7_Enable);
	m_Check8.SetCheck(Event8_Enable);
	m_Check9.SetCheck(Event9_Enable);
	m_Check10.SetCheck(Event10_Enable);
	m_Check11.SetCheck(Event11_Enable);
	m_Check12.SetCheck(Event12_Enable);
	m_Check13.SetCheck(Event13_Enable);
	m_Check14.SetCheck(Event14_Enable);
	m_Check15.SetCheck(Event15_Enable);
	m_Check16.SetCheck(Event16_Enable);
	m_Check17.SetCheck(Event17_Enable);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CMNP_ZJNHDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	Event1_Enable=m_Check1.GetCheck();
	Event2_Enable=m_Check2.GetCheck();
	Event3_Enable=m_Check3.GetCheck();
	Event4_Enable=m_Check4.GetCheck();
	Event5_Enable=m_Check5.GetCheck();
	Event6_Enable=m_Check6.GetCheck();
	Event7_Enable=m_Check7.GetCheck();
	Event8_Enable=m_Check8.GetCheck();
	Event9_Enable=m_Check9.GetCheck();
	Event10_Enable=m_Check10.GetCheck();
	Event11_Enable=m_Check11.GetCheck();
	Event12_Enable=m_Check12.GetCheck();
	Event13_Enable=m_Check13.GetCheck();
	Event14_Enable=m_Check14.GetCheck();
	Event15_Enable=m_Check15.GetCheck();
	Event16_Enable=m_Check16.GetCheck();
	Event17_Enable=m_Check17.GetCheck();
	OnOK();
}
