// PropYxBindDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropYxBindDlg.h"


// CPropYxBindDlg 对话框

IMPLEMENT_DYNAMIC(CPropYxBindDlg, CPropertyPage)
CPropYxBindDlg::CPropYxBindDlg()
	: CPropertyPage(CPropYxBindDlg::IDD)
	, m_strName(_T(""))
	, m_strDescript(_T(""))
{
}

CPropYxBindDlg::~CPropYxBindDlg()
{
}

void CPropYxBindDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDV_MaxChars(pDX, m_strName, 17);
	DDX_Text(pDX, IDC_EDIT2, m_strDescript);
}


BEGIN_MESSAGE_MAP(CPropYxBindDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CPropYxBindDlg 消息处理程序

void CPropYxBindDlg::OnBnClickedButton1()//挑点
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true) ;
	char cEname[17] ;
	char cDescribe[33] ;

	HINSTANCE hInstance = LoadLibrary("C:\\SUNPAC\\DLL\\FindDot.dll") ;
	if(!hInstance)
	{
		AfxMessageBox("dll调用失败") ;
		return ;
	}

	pShowFindDot=(ShowFindDotDlg*)GetProcAddress(hInstance,"ShowFindDotDlg");

	if (pShowFindDot==NULL)
	{
		AfxMessageBox("调用函数失败");
		FreeLibrary(hInstance);
		hInstance=NULL;
		return;
	}

	if ((*pShowFindDot)(2,cEname,cDescribe))
	{
		m_strName = cEname ;
	}

	FreeLibrary(hInstance);
	UpdateData(false) ;

}

void CPropYxBindDlg::GetParam(char* ename ,char* des)
{
	strcpy(ename ,m_strName) ;
	strcpy(des ,m_strDescript) ;
}

void CPropYxBindDlg::SetParam(char* ename ,char* des)
{
	m_strName = ename ;
	m_strDescript = des ;
}
