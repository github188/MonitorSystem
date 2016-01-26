// SZ_SbLookDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/SZ_SbLookDlg.h"
//#include "Envir.h"


// CSZ_SbLookDlg 对话框

IMPLEMENT_DYNAMIC(CSZ_SbLookDlg, CDialog)
CSZ_SbLookDlg::CSZ_SbLookDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSZ_SbLookDlg::IDD, pParent)
	, m_sbinfo(_T(""))
	, m_sbbh(_T(""))
{
}

CSZ_SbLookDlg::~CSZ_SbLookDlg()
{
}

void CSZ_SbLookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SZ_SBINFO, m_sbinfo);
	DDX_Text(pDX, IDC_SZ_SBBH, m_sbbh);
	DDV_MaxChars(pDX, m_sbinfo, 32);
	DDV_MaxChars(pDX, m_sbbh, 16);
}


BEGIN_MESSAGE_MAP(CSZ_SbLookDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CSZ_SbLookDlg 消息处理程序

void CSZ_SbLookDlg::SetInfo(char* Info, char* Sbbh)
{
	m_sbinfo=Info;
	m_sbbh=Sbbh;
}

void CSZ_SbLookDlg::GetInfo(char* Info, char* Sbbh)
{
	if (m_sbinfo.GetLength()>32) m_sbinfo=m_sbinfo.Mid(0,32);
	strcpy(Info,m_sbinfo);
	if (m_sbbh.GetLength()>16) m_sbbh=m_sbbh.Mid(0,16);
	strcpy(Sbbh,m_sbbh);
}

void CSZ_SbLookDlg::OnBnClickedButton1()
{

}
