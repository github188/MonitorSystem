// PropYZTYDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropYZTYDlg.h"


// CPropYZTYDlg 对话框

IMPLEMENT_DYNAMIC(CPropYZTYDlg, CPropertyPage)
CPropYZTYDlg::CPropYZTYDlg()
	: CPropertyPage(CPropYZTYDlg::IDD)
	, m_nUp_action(0)
	, m_nDown_action(0)
	, m_nStop_action(0)
	, m_nZK(0)
	, m_strName(_T(""))
	, m_strUpName(_T(""))
	, m_strDownName(_T(""))
	, m_strStopName(_T(""))
	, m_strFK(_T(""))
	, m_nType(0)
{
}

CPropYZTYDlg::~CPropYZTYDlg()
{
}

void CPropYZTYDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nUp_action);
	DDX_Radio(pDX, IDC_RADIO3, m_nDown_action);
	DDX_Radio(pDX, IDC_RADIO5, m_nStop_action);
	DDX_Radio(pDX, IDC_RADIO7, m_nZK);

	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDV_MaxChars(pDX, m_strName, 17);

	DDX_Text(pDX, IDC_EDIT2, m_strUpName);
	DDV_MaxChars(pDX, m_strUpName, 17);

	DDX_Text(pDX, IDC_EDIT3, m_strDownName);
	DDV_MaxChars(pDX, m_strDownName, 17);

	DDX_Text(pDX, IDC_EDIT4, m_strStopName);
	DDV_MaxChars(pDX, m_strStopName, 17);

	DDX_Text(pDX, IDC_EDIT5, m_strFK);
	DDV_MaxChars(pDX, m_strFK, 17);
}


BEGIN_MESSAGE_MAP(CPropYZTYDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
END_MESSAGE_MAP()


// CPropYZTYDlg 消息处理程序

void CPropYZTYDlg::SetParam(char text[], char Yknameup[], char Yknamedown[], char Yknamestop[], int bVTup,int bVTdown,int bVTstop, char Yxname_FK[])
{
	m_strName = text ;
	m_strUpName = Yknameup ;
	m_strDownName = Yknamedown ;
	m_strStopName = Yknamestop ;
	m_strFK = Yxname_FK ;

	m_nZK = bVTup >> 1 ;

	m_nUp_action = bVTup&0x01 ;
	m_nDown_action = bVTdown&0x01 ;
	m_nStop_action = bVTstop&0x01 ;

}

void CPropYZTYDlg::GetParam(char text[], char Yknameup[], char Yknamedown[], char Yknamestop[], int &bVTup, int &bVTdown, int &bVTstop, char Yxname_FK[])
{
	strcpy(text,m_strName) ;
	strcpy(Yknameup,m_strUpName) ;
	strcpy(Yknamedown,m_strDownName) ;
	strcpy(Yknamestop,m_strStopName) ;
	strcpy(Yxname_FK,m_strFK) ;

	bVTup = m_nUp_action | (m_nZK << 1) ;
	bVTdown = m_nDown_action | (m_nZK << 1) ;
	bVTstop = m_nStop_action | (m_nZK << 1) ;

}

void CPropYZTYDlg::OnBnClickedButton1()//挑点
{
	// TODO: 在此添加控件通知处理程序代码
	AddName(0) ;
}

void CPropYZTYDlg::OnBnClickedButton2()
{
	AddName(1) ;
}

void CPropYZTYDlg::OnBnClickedButton3()
{
	AddName(2) ;
}

void CPropYZTYDlg::OnBnClickedButton4()
{
	AddName(3) ;
}

void CPropYZTYDlg::AddName(int nType)
{

}
