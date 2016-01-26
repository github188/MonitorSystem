// YX_PROP_INFO.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/YX_PROP_INFO.h"
#include "canvas/YX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYX_PROP_INFO property page

IMPLEMENT_DYNCREATE(CYX_PROP_INFO, CPropertyPage)

CYX_PROP_INFO::CYX_PROP_INFO() : CPropertyPage(CYX_PROP_INFO::IDD)
{
	//{{AFX_DATA_INIT(CYX_PROP_INFO)
	m_edit = _T("");
	m_check = FALSE;
	//}}AFX_DATA_INIT
}

CYX_PROP_INFO::~CYX_PROP_INFO()
{
}

void CYX_PROP_INFO::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYX_PROP_INFO)
	DDX_Text(pDX, CZ_IDC_EDIT_ENAME, m_edit);
	DDV_MaxChars(pDX, m_edit, 16);
	DDX_Check(pDX, CZ_IDC_HANDCHECK, m_check);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYX_PROP_INFO, CPropertyPage)
	//{{AFX_MSG_MAP(CYX_PROP_INFO)
	ON_BN_CLICKED(CZ_IDC_BUTTON_FINDDOT, OnButtonFinddot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYX_PROP_INFO message handlers

void CYX_PROP_INFO::GetPara(char *ename, BYTE *handit)
{
	char s[17];
	sprintf(s,"%s",m_edit);
	s[16]='\0';
	memcpy(ename,s,17*sizeof(char));
	*handit=(BYTE)m_check;
}

void CYX_PROP_INFO::SetPara(char *ename, BYTE handit)
{
	char s[17];
	memcpy(s,ename,17*sizeof(char));
	s[16]='\0';
	m_edit=s;
	m_check=handit;
}

BOOL CYX_PROP_INFO::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CYX_PROP_INFO::OnButtonFinddot() 
{
	// TODO: Add your control notification handler code here
//===========
	char dllf[256],s[256],myename[17],mycname[33];
	HINSTANCE hmydll;
	sprintf(dllf,YX_FINDDOTDLLNAME);
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
		m_edit=myename;
		UpdateData(false);
	}

	FreeLibrary(hmydll);
}
