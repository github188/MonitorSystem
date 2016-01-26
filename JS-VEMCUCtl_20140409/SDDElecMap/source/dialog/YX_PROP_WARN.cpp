// YX_PROP_WARN.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/YX_PROP_WARN.h"
#include "dialog/CZWavSelectPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYX_PROP_WARN property page

IMPLEMENT_DYNCREATE(CYX_PROP_WARN, CPropertyPage)

CYX_PROP_WARN::CYX_PROP_WARN() : CPropertyPage(CYX_PROP_WARN::IDD)
{
	//{{AFX_DATA_INIT(CYX_PROP_WARN)
	m_1pic = FALSE;
	m_1 = FALSE;
	m_1flash = FALSE;
	m_1wave = FALSE;
	m_wave1 = _T("");
	m_wave2 = _T("");
	m_1radio = -1;
	m_0radio = -1;
	m_0 = FALSE;
	m_0pic = FALSE;
	m_0flash = FALSE;
	m_0wave = FALSE;
	//}}AFX_DATA_INIT
}

CYX_PROP_WARN::~CYX_PROP_WARN()
{
}

void CYX_PROP_WARN::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYX_PROP_WARN)
	DDX_Check(pDX, CZ_IDC_CHECK_UP1, m_1pic);
	DDX_Check(pDX, CZ_IDC_CHECK_UP, m_1);
	DDX_Check(pDX, CZ_IDC_CHECK_UP2, m_1flash);
	DDX_Check(pDX, CZ_IDC_CHECK_UP3, m_1wave);
	DDX_Text(pDX, CZ_IDC_EDIT_WAVE1, m_wave1);
	DDV_MaxChars(pDX, m_wave1, 32);
	DDX_Text(pDX, CZ_IDC_EDIT_WAVE2, m_wave2);
	DDV_MaxChars(pDX, m_wave2, 32);
	DDX_Radio(pDX, CZ_IDC_RADIO_UP1, m_1radio);
	DDX_Radio(pDX, CZ_IDC_RADIO_UP4, m_0radio);
	DDX_Check(pDX, CZ_IDC_CHECK2, m_0);
	DDX_Check(pDX, CZ_IDC_CHECK_UP4, m_0pic);
	DDX_Check(pDX, CZ_IDC_CHECK_UP5, m_0flash);
	DDX_Check(pDX, CZ_IDC_CHECK_UP6, m_0wave);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYX_PROP_WARN, CPropertyPage)
	//{{AFX_MSG_MAP(CYX_PROP_WARN)
	ON_BN_CLICKED(CZ_IDC_BUTTON1L, OnIdcButton1l)
	ON_BN_CLICKED(CZ_IDC_BUTTON0L, OnIdcButton0l)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYX_PROP_WARN message handlers

BOOL CYX_PROP_WARN::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	if ((warn1kind&0x01)==0) {m_1pic=0;}else{m_1pic=1;}
	if ((warn1kind&0x02)==0) {m_1flash=0;}else{m_1flash=1;}
	if ((warn1kind&0x04)==0) {m_1wave=0;}else{m_1wave=1;}
	if ((warn0kind&0x01)==0) {m_0pic=0;}else{m_0pic=1;}
	if ((warn0kind&0x02)==0) {m_0flash=0;}else{m_0flash=1;}
	if ((warn0kind&0x04)==0) {m_0wave=0;}else{m_0wave=1;}

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CYX_PROP_WARN::SetPara(BYTE warn1enable,BYTE swarn1kind,BYTE warn1levels,
		char *warn1wave,BYTE warn0enable,BYTE swarn0kind,BYTE warn0levels,
        char *warn0wave)
{
	char s1[33],s2[33];
	memcpy(s1,warn1wave,sizeof(char)*33);
	memcpy(s2,warn0wave,sizeof(char)*33);
	s1[32]='\0';
	s2[32]='\0';

	m_1=warn1enable;
	warn1kind=swarn1kind;
	
	if (warn1kind&0x1)
		  m_1pic=true;
	else
		  m_1pic=false;

	if (warn1kind&0x2)
		  m_1flash=true;
	else
		  m_1flash=false;

	if (warn1kind&0x4)
		  m_1wave=true;
	else
		  m_1wave=false;

	m_1radio=warn1levels;
	m_wave1=s1;
	m_0=warn0enable;
	warn0kind=swarn0kind;
	m_0radio=warn0levels;
	m_wave2=s2;

	if (warn0kind&0x1)
		  m_0pic=true;
	else
		  m_0pic=false;

	if (warn0kind&0x2)
		  m_0flash=true;
	else
		  m_0flash=false;
	
	if (warn0kind&0x4)
		  m_0wave=true;
	else
		  m_0wave=false;


}
void CYX_PROP_WARN::GetPara(BYTE * warn1enable,BYTE * warn1kind,BYTE * warn1levels,
		char *warn1wave,BYTE * warn0enable,BYTE* warn0kind,BYTE* warn0levels,
		char *warn0wave)
{
	*warn1kind=0;
	if (m_1pic) *warn1kind=*warn1kind|0x01;
	if (m_1flash) *warn1kind=*warn1kind|0x02;
	if (m_1wave) *warn1kind=*warn1kind|0x04;
	*warn0kind=0;
	if (m_0pic) *warn0kind=*warn0kind|0x01;
	if (m_0flash) *warn0kind=*warn0kind|0x02;
	if (m_0wave) *warn0kind=*warn0kind|0x04;
	*warn1enable=m_1;
	*warn1levels=m_1radio;

	*warn0enable=m_0;
	*warn0levels=m_0radio;

	char s1[33],s2[33];
	sprintf(s1,"%s",m_wave1);
	s1[32]='\0';
	sprintf(s2,"%s",m_wave2);
	s2[32]='\0';

	memcpy(warn1wave,s1,sizeof(char)*33);
	memcpy(warn0wave,s2,sizeof(char)*33);
}

void CYX_PROP_WARN::OnIdcButton1l() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CPropertySheet sheet( _T("声音代理.."));
	CCZWavSelectPage dlg;
	dlg.SetContent(m_wave1);
    sheet.AddPage((CPropertyPage*)&dlg);
	if (sheet.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		CString tmp=dlg.GetContent();
		m_wave1=tmp.Left(32);
		UpdateData(false);
	}
	
}

void CYX_PROP_WARN::OnIdcButton0l() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CPropertySheet sheet( _T("声音代理.."));
	CCZWavSelectPage dlg;
	dlg.SetContent(m_wave2);
    sheet.AddPage((CPropertyPage*)&dlg);
	if (sheet.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		CString tmp=dlg.GetContent();
		m_wave2=tmp.Left(32);
		UpdateData(false);
	}
	
}
