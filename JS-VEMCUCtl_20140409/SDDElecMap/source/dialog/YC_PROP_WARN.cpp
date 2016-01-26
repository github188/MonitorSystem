// YC_PROP_WARN.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/YC_PROP_WARN.h"
#include "dialog/CZWavSelectPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYC_PROP_WARN property page

IMPLEMENT_DYNCREATE(CYC_PROP_WARN, CPropertyPage)

CYC_PROP_WARN::CYC_PROP_WARN() : CPropertyPage(CYC_PROP_WARN::IDD)
{
	//{{AFX_DATA_INIT(CYC_PROP_WARN)
	m_upradio = -1;
	m_downradio = -1;
	m_up = FALSE;
	m_down = FALSE;
	m_wave1 = _T("");
	m_wave2 = _T("");
	m_uppic = FALSE;
	m_upflash = FALSE;
	m_upwave = FALSE;
	m_downpic = FALSE;
	m_downflash = FALSE;
	m_downwave = FALSE;
	//}}AFX_DATA_INIT
}

CYC_PROP_WARN::~CYC_PROP_WARN()
{
}

void CYC_PROP_WARN::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYC_PROP_WARN)
	DDX_Radio(pDX, CZ_IDC_RADIO_UP1, m_upradio);
	DDX_Radio(pDX, CZ_IDC_RADIO_UP4, m_downradio);
	DDX_Check(pDX, CZ_IDC_CHECK_UP, m_up);
	DDX_Check(pDX, CZ_IDC_CHECK2, m_down);
	DDX_Text(pDX, CZ_IDC_EDIT_WAVE1, m_wave1);
	DDV_MaxChars(pDX, m_wave1, 32);
	DDX_Text(pDX, CZ_IDC_EDIT_WAVE2, m_wave2);
	DDV_MaxChars(pDX, m_wave2, 32);
	DDX_Check(pDX, CZ_IDC_CHECK_UP1, m_uppic);
	DDX_Check(pDX, CZ_IDC_CHECK_UP2, m_upflash);
	DDX_Check(pDX, CZ_IDC_CHECK_UP3, m_upwave);
	DDX_Check(pDX, CZ_IDC_CHECK_UP4, m_downpic);
	DDX_Check(pDX, CZ_IDC_CHECK_UP5, m_downflash);
	DDX_Check(pDX, CZ_IDC_CHECK_UP6, m_downwave);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYC_PROP_WARN, CPropertyPage)
	//{{AFX_MSG_MAP(CYC_PROP_WARN)
	ON_BN_CLICKED(CZ_IDC_CHECK_UP1, OnCheckUp1)
	ON_BN_CLICKED(CZ_IDC_BUTTONUPL, OnIdcButtonupl)
	ON_BN_CLICKED(CZ_IDC_BUTTONDNL, OnIdcButtondnl)
	ON_BN_CLICKED(CZ_IDC_BUTTONUPP, OnIdcButtonupp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYC_PROP_WARN message handlers

BOOL CYC_PROP_WARN::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	if ((warnupkind&0x01)==0) {m_uppic=0;}else{m_uppic=1;}
	if ((warnupkind&0x02)==0) {m_upflash=0;}else{m_upflash=1;}
	if ((warnupkind&0x04)==0) {m_upwave=0;}else{m_upwave=1;}
	if ((warndownkind&0x01)==0) {m_downpic=0;}else{m_downpic=1;}
	if ((warndownkind&0x02)==0) {m_downflash=0;}else{m_downflash=1;}
	if ((warndownkind&0x04)==0) {m_downwave=0;}else{m_downwave=1;}

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CYC_PROP_WARN::OnCheckUp1() 
{
	// TODO: Add your control notification handler code here
		
}

void CYC_PROP_WARN::SetPara(BYTE warnupenable,BYTE swarnupkind,BYTE warnuplevels,
		char *warnupwave,BYTE warndownenable,BYTE swarndownkind,BYTE warndownlevels,
        char *warndownwave)
{
	char s1[33],s2[33];
	memcpy(s1,warnupwave,sizeof(char)*33);
	memcpy(s2,warndownwave,sizeof(char)*33);
	s1[32]='\0';
	s2[32]='\0';

	m_up=warnupenable;
	warnupkind=swarnupkind;
	m_upradio=warnuplevels;
	m_wave1=s1;
	m_down=warndownenable;
	warndownkind=swarndownkind;
	m_downradio=warndownlevels;
	m_wave2=s2;

}

void CYC_PROP_WARN::GetPara(BYTE * warnupenable,BYTE * warnupkind,BYTE * warnuplevels,
		char *warnupwave,BYTE * warndownenable,BYTE* warndownkind,BYTE* warndownlevels,
		char *warndownwave)
{
	*warnupkind=0;
	if (m_uppic) *warnupkind=*warnupkind|0x01;
	if (m_upflash) *warnupkind=*warnupkind|0x02;
	if (m_upwave) *warnupkind=*warnupkind|0x04;
	*warndownkind=0;
	if (m_downpic) *warndownkind=*warndownkind|0x01;
	if (m_downflash) *warndownkind=*warndownkind|0x02;
	if (m_downwave) *warndownkind=*warndownkind|0x04;
	*warnupenable=m_up;
	*warnuplevels=m_upradio;

	*warndownenable=m_down;
	*warndownlevels=m_downradio;

	char s1[33],s2[33];
	sprintf(s1,"%s",m_wave1);
	s1[32]='\0';
	sprintf(s2,"%s",m_wave2);
	s2[32]='\0';

	memcpy(warnupwave,s1,sizeof(char)*33);
	memcpy(warndownwave,s2,sizeof(char)*33);

}

void CYC_PROP_WARN::OnIdcButtonupl() 
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

void CYC_PROP_WARN::OnIdcButtondnl() 
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

void CYC_PROP_WARN::OnIdcButtonupp() 
{
	// TODO: Add your control notification handler code here
	
}
