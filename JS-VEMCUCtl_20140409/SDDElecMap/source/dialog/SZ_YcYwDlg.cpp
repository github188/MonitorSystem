// SZ_YcYwDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/SZ_YcYwDlg.h"


// CSZ_YcYwDlg 对话框

IMPLEMENT_DYNAMIC(CSZ_YcYwDlg, CPropertyPage)
CSZ_YcYwDlg::CSZ_YcYwDlg()
	: CPropertyPage(CSZ_YcYwDlg::IDD)
	, m_ename(_T(""))
	, m_max(0)
	, m_min(0)
	, m_drawtype(0)
{
}

CSZ_YcYwDlg::~CSZ_YcYwDlg()
{
}

void CSZ_YcYwDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, CZ_IDC_STATIC_C1, m_color);
	DDX_Control(pDX, CZ_IDC_STATIC_C2, m_disablecolor);
	DDX_Control(pDX, CZ_IDC_STATIC_C3, m_hilevelcolor);
	DDX_Control(pDX, CZ_IDC_STATIC_C4, m_lwlevelcolor);
	DDX_Text(pDX, CZ_IDC_EDIT_ENAME, m_ename);
	DDX_Text(pDX, IDC_E_MAX, m_max);
	DDX_Text(pDX, IDC_E_MIN, m_min);
	DDX_Radio(pDX, IDC_RADIO1, m_drawtype);
}


BEGIN_MESSAGE_MAP(CSZ_YcYwDlg, CPropertyPage)
	ON_BN_CLICKED(CZ_IDC_BUTTON_FINDDOT, OnBnClickedIdcButtonFinddot)
END_MESSAGE_MAP()


// CSZ_YcYwDlg 消息处理程序

void CSZ_YcYwDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData();

	m_color.GetColor(&m_pcolor);
	m_disablecolor.GetColor(&m_pdisablecolor);
	m_hilevelcolor.GetColor(&m_philevelcolor);
	m_lwlevelcolor.GetColor(&m_plwlevelcolor);
	if (m_max>m_min)
	{
		m_pmax=m_max;
		m_pmin=m_min;
	}
	else
	{
		m_pmax=m_min;
		m_pmin=m_max;
	}

	m_pdrawtype=m_drawtype;
	m_pename=m_ename;

	CPropertyPage::OnOK();
}

BOOL CSZ_YcYwDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_color.SetColor(m_pcolor);
	m_disablecolor.SetColor(m_pdisablecolor);
	m_hilevelcolor.SetColor(m_philevelcolor);
	m_lwlevelcolor.SetColor(m_plwlevelcolor);
	m_max=m_pmax;
	m_min=m_pmin;
	m_drawtype=m_pdrawtype;
	m_ename=m_pename;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CSZ_YcYwDlg::SetPara(COLORREF color,COLORREF disablecolor,COLORREF hilevelcolor,COLORREF lwlevelcolor,char* ename,float max,float min,BYTE drawtype)
{
	m_pcolor=color;
	m_pdisablecolor=disablecolor;
	m_philevelcolor=hilevelcolor;
	m_plwlevelcolor=lwlevelcolor;
	m_pmax=max;
	m_pmin=min;
	m_pdrawtype=drawtype;
	m_pename=ename;
}

void CSZ_YcYwDlg::GetPara(COLORREF* color,COLORREF* disablecolor,COLORREF* hilevelcolor,COLORREF* lwlevelcolor,char* ename,float* max,float* min,BYTE* drawtype)
{
	*color=m_pcolor;
	*disablecolor=m_pdisablecolor;
	*hilevelcolor=m_philevelcolor;
	*lwlevelcolor=m_plwlevelcolor;
	*max=m_pmax;
	*min=m_pmin;
	*drawtype=m_pdrawtype;
	strcpy(ename,m_pename);
}

void CSZ_YcYwDlg::OnBnClickedIdcButtonFinddot()
{
	// TODO: 在此添加控件通知处理程序代码
 	char dllf[256],s[256],myename[17],mycname[33];
 	HINSTANCE hmydll;
 	sprintf(dllf,YC_FINDDOTDLLNAME);
 	hmydll=LoadLibrary(dllf);
 	if (hmydll==NULL)
	{
 		sprintf(s,"%s 调入失败！",dllf);
 		AfxMessageBox(s);
 		return;
 	}
 	ShowFindDotDlg * pf;
 	pf=(ShowFindDotDlg *)::GetProcAddress(hmydll,"ShowFindDotDlg");
 	if (pf==NULL){
 		sprintf(s,"%s 调入函数ShowFindDotDlg 失败！",dllf);
 		AfxMessageBox(s);
 		FreeLibrary(hmydll);
 		hmydll=NULL;
 		return;
 	}
 	CWnd * pp=this->GetParent();
	BYTE dotkind=1;
 	if ((*pf)(dotkind,myename,mycname))
 	{
 		UpdateData(true);
 		m_ename=myename;
 		UpdateData(false);
 	}
 
 	FreeLibrary(hmydll);
}
