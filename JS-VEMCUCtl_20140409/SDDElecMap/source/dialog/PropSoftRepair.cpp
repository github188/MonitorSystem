// PropSoftRepair.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropSoftRepair.h"


// CPropSoftRepair 对话框

IMPLEMENT_DYNAMIC(CPropSoftRepair, CPropertyPage)
CPropSoftRepair::CPropSoftRepair()
	: CPropertyPage(CPropSoftRepair::IDD)
	, m_strDes(_T(""))
	, m_strEname(_T(""))
{
}

CPropSoftRepair::~CPropSoftRepair()
{
}

void CPropSoftRepair::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strDes);
	DDX_Text(pDX, IDC_EDIT1, m_strEname);
}


BEGIN_MESSAGE_MAP(CPropSoftRepair, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CPropSoftRepair 消息处理程序

void CPropSoftRepair::OnBnClickedButton1()//挑点
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
		m_strEname = cEname ;
	}

	FreeLibrary(hInstance);
	UpdateData(false) ;

}

void CPropSoftRepair::GetParam(char* ename ,char* des)
{
	strcpy(ename ,m_strEname) ;
	strcpy(des ,m_strDes) ;
}

void CPropSoftRepair::SetParam(char* ename ,char* des)
{
	m_strEname = ename ;
	m_strDes = des ;
}
