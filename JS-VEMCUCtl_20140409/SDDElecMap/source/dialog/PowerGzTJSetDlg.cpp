// PowerGzTJSetDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PowerGzTJSetDlg.h"
#include "dialog/TiaoGzTjDlg.h"

// CPowerGzTJSetDlg 对话框

IMPLEMENT_DYNAMIC(CPowerGzTJSetDlg, CDialog)
CPowerGzTJSetDlg::CPowerGzTJSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPowerGzTJSetDlg::IDD, pParent)
	, m_Check(FALSE)
	, m_Ename1(_T(""))
	, m_Power1(_T(""))
	, m_Power2(_T(""))
{
}

CPowerGzTJSetDlg::~CPowerGzTJSetDlg()
{
}

void CPowerGzTJSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_Check);
	DDX_Text(pDX, IDC_EDIT1, m_Ename1);
	DDX_Text(pDX, IDC_EDIT2, m_Power1);
	DDX_Text(pDX, IDC_EDIT3, m_Power2);
	DDX_Control(pDX, IDC_STATIC3, m_Static);
	DDX_Control(pDX, IDC_EDIT3, m_Edit);
	DDX_Control(pDX, IDC_BUTTON2, m_Btn);
}


BEGIN_MESSAGE_MAP(CPowerGzTJSetDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()

void CPowerGzTJSetDlg::SetName(char *s1,char *s2,char *s3,BOOL check)
{
	m_Ename1=s1;
	m_Power1=s2;
	m_Power2=s3;
	m_Check=check;
}
void CPowerGzTJSetDlg::GetName(char *s1,char *s2,char *s3,BOOL *check)
{
	//UpdateData(TRUE);
	strcpy(s1,m_Ename1);
	strcpy(s2,m_Power1);
	strcpy(s3,m_Power2);
	*check=m_Check;
}
// CPowerGzTJSetDlg 消息处理程序

BOOL CPowerGzTJSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Static.ShowWindow(0);
	m_Edit.ShowWindow(0);
	m_Btn.ShowWindow(0);
	if(m_Check)
	{
		m_Static.ShowWindow(1);
	    m_Edit.ShowWindow(1);
		m_Btn.ShowWindow(1);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CPowerGzTJSetDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_Check)
	{
		m_Static.ShowWindow(1);
	    m_Edit.ShowWindow(1);
		m_Btn.ShowWindow(1);
	}
	else
	{
		m_Static.ShowWindow(0);
	    m_Edit.ShowWindow(0);
		m_Btn.ShowWindow(0);
	}
}

void CPowerGzTJSetDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CTiaoGzTjDlg dlg;
	dlg.SetType(1);
	if(dlg.DoModal()==IDOK)
	{
		m_Power1=dlg.m_id;
	}
	UpdateData(FALSE);
}

void CPowerGzTJSetDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CTiaoGzTjDlg dlg;
	dlg.SetType(1);
	if(dlg.DoModal()==IDOK)
	{
		m_Power2=dlg.m_id;
	}
	UpdateData(FALSE);
}
