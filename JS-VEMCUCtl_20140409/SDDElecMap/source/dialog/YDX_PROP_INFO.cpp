// YDX_PROP_INFO.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/YDX_PROP_INFO.h"
#include "canvas/YDX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYDX_PROP_INFO property page

IMPLEMENT_DYNCREATE(CYDX_PROP_INFO, CPropertyPage)

CYDX_PROP_INFO::CYDX_PROP_INFO() : CPropertyPage(CYDX_PROP_INFO::IDD)
{
	//{{AFX_DATA_INIT(CYDX_PROP_INFO)
	m_edit1 = _T("");
	m_edit2 = _T("");
	m_check = FALSE;
	//}}AFX_DATA_INIT
}

CYDX_PROP_INFO::~CYDX_PROP_INFO()
{
}

void CYDX_PROP_INFO::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYDX_PROP_INFO)
	DDX_Text(pDX, CZ_IDC_EDIT_ENAME, m_edit1);
	DDX_Text(pDX, CZ_IDC_EDIT_ENAME2, m_edit2);
	DDX_Check(pDX, CZ_IDC_HANDCHECK, m_check);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYDX_PROP_INFO, CPropertyPage)
	//{{AFX_MSG_MAP(CYDX_PROP_INFO)
	ON_BN_CLICKED(CZ_IDC_BUTTON_FINDDOT, OnIdcButtonFinddot)
	ON_BN_CLICKED(CZ_IDC_BUTTON_FINDDOT2, OnIdcButtonFinddot2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYDX_PROP_INFO message handlers

BOOL CYDX_PROP_INFO::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CYDX_PROP_INFO::GetPara(char *mainename,char *slaveename, BYTE *handit)
{
	char s[17];
	sprintf(s,"%s",m_edit1);
	s[16]='\0';
	memcpy(mainename,s,17*sizeof(char));
	sprintf(s,"%s",m_edit2);
	s[16]='\0';
	memcpy(slaveename,s,17*sizeof(char));
	*handit=m_check;
}

void CYDX_PROP_INFO::SetPara(char *mainename, char *slaveename,BYTE handit)
{
	char s[17];
	memcpy(s,mainename,17*sizeof(char));
	s[16]='\0';
	m_edit1=s;
	memcpy(s,slaveename,17*sizeof(char));
	s[16]='\0';
	m_edit2=s;
	m_check=handit;
}

void CYDX_PROP_INFO::OnIdcButtonFinddot() 
{
	// TODO: Add your control notification handler code here
//===========
	char dllf[256],s[256],myename[17],mycname[33];
	HINSTANCE hmydll;
	sprintf(dllf,YDX_FINDDOTDLLNAME);
	hmydll=LoadLibrary(dllf);
	if (hmydll==NULL)
	{
		sprintf(s,"%s 调入失败！",dllf);
		AfxMessageBox(s);
		return;
	}
	ShowFindDotDlg * pf;
	pf=(ShowFindDotDlg *)::GetProcAddress(hmydll,"ShowFindDotDlg");
	if (pf==NULL)
	{
		sprintf(s,"%s 调入函数ShowFindDotDlg 失败！",dllf);
		AfxMessageBox(s);
		FreeLibrary(hmydll);
		hmydll=NULL;
		return;
	}
	CWnd * pp=this->GetParent();
	if ((*pf)(2,myename,mycname))
	{
		UpdateData(true);
		m_edit1=myename;
		UpdateData(false);
	}

	FreeLibrary(hmydll);
	
}

void CYDX_PROP_INFO::OnIdcButtonFinddot2() 
{
	// TODO: Add your control notification handler code here
//===========
	char dllf[256],s[256],myename[17],mycname[33];
	HINSTANCE hmydll;
	sprintf(dllf,YDX_FINDDOTDLLNAME);
	hmydll=LoadLibrary(dllf);
	if (hmydll==NULL)
	{
		sprintf(s,"%s 调入失败！",dllf);
		AfxMessageBox(s);
		return;
	}

	ShowFindDotDlg * pf;
	pf=(ShowFindDotDlg *)::GetProcAddress(hmydll,"ShowFindDotDlg");
	if (pf==NULL)
	{
		sprintf(s,"%s 调入函数ShowFindDotDlg 失败！",dllf);
		AfxMessageBox(s);
		FreeLibrary(hmydll);
		hmydll=NULL;
		return;
	}
	CWnd * pp=this->GetParent();
	if ((*pf)(2,myename,mycname))
	{
		UpdateData(true);
		m_edit2=myename;
		UpdateData(false);
	}

	FreeLibrary(hmydll);
	
}