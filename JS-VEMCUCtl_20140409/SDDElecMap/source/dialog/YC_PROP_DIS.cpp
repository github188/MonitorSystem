// YC_PROP_DIS.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/YC_PROP_DIS.h"
#include "dialog/FontDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYC_PROP_DIS PropertyPage

IMPLEMENT_DYNCREATE(CYC_PROP_DIS, CPropertyPage)

CYC_PROP_DIS::CYC_PROP_DIS(BYTE kind) : CPropertyPage(CYC_PROP_DIS::IDD)
{
	itkind=kind;
	//{{AFX_DATA_INIT(CYC_PROP_DIS)
	m_editx0 = 0.0f;
	m_edity0 = 0.0f;
	m_editfh = 0.0f;
	m_editfw = 0.0f;
	m_editjj = 0.0f;
	//}}AFX_DATA_INIT
}

CYC_PROP_DIS::~CYC_PROP_DIS()
{
}

void CYC_PROP_DIS::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYC_PROP_DIS)
	DDX_Text(pDX, CZ_IDC_EDITX0, m_editx0);
	DDV_MinMaxFloat(pDX, m_editx0, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITY0, m_edity0);
	DDV_MinMaxFloat(pDX, m_edity0, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITFH, m_editfh);
	DDV_MinMaxFloat(pDX, m_editfh, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITFW, m_editfw);
	DDV_MinMaxFloat(pDX, m_editfw, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITJJ, m_editjj);
	DDV_MinMaxFloat(pDX, m_editjj, 0.f, 32000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYC_PROP_DIS, CPropertyPage)
	//{{AFX_MSG_MAP(CYC_PROP_DIS)
	ON_BN_CLICKED(CZ_IDC_BUTTON_FONT, OnIdcButtonFont)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYC_PROP_DIS message handlers
void CYC_PROP_DIS::SetPara(COLORREF disablec,COLORREF normalc,COLORREF leveldc,COLORREF leveluc,
		LOGFONT ff,float x0,float y0,float fontw,float fonth,float fontj)
{
	disablecolor=disablec;
	normalcolor=normalc;
	leveldowncolor=leveldc;
	levelupcolor=leveluc;
	lgfont=ff;
	rectx0=x0;
	recty0=y0;
	fontwidth=fontw;
	fontheight=fonth;
	fontjj=fontj;
}

void CYC_PROP_DIS::SetPara2(COLORREF disablec,COLORREF normalc,
		LOGFONT ff,float x0,float y0,float fontw,float fonth,float fontj)
{
	disablecolor=disablec;
	normalcolor=normalc;
	lgfont=ff;
	rectx0=x0;
	recty0=y0;
	fontwidth=fontw;
	fontheight=fonth;
	fontjj=fontj;
}

void CYC_PROP_DIS::GetPara(COLORREF *disablec,COLORREF *normalc,COLORREF *leveldc,COLORREF *leveluc,
		LOGFONT *ff,float *x0,float *y0,float *fontw,float *fonth,float *fontj)
{
	//UpdateData(true);
	*disablec=disablecolor;
	*normalc=normalcolor;
	*leveldc=leveldowncolor;
	*leveluc=levelupcolor;
	*ff=lgfont;
	*x0=rectx0;
	*y0=recty0;
	*fontw=fontwidth;
	*fonth=fontheight;
	*fontj=fontjj;
}

void CYC_PROP_DIS::GetPara2(COLORREF *disablec,COLORREF *normalc,
		LOGFONT *ff,float *x0,float *y0,float *fontw,float *fonth,float *fontj)
{
	//UpdateData(true);
	*disablec=disablecolor;
	*normalc=normalcolor;
	*ff=lgfont;
	*x0=rectx0;
	*y0=recty0;
	*fontw=fontwidth;
	*fonth=fontheight;
	*fontj=fontjj;
}


//DEL void CYC_PROP_DIS::OnClose() 
//DEL {
//DEL 	// TODO: Add your message handler code here and/or call default
//DEL 	
//DEL 	CPropertyPage::OnClose();
//DEL }


/////////////////////////////////////////////////////////////////////////////
// CYC_PROP_DIS message handlers

void CYC_PROP_DIS::OnIdcButtonFont() 
 {
 	// TODO: Add your control notification handler code here
 	CPropertySheet sheet("Ñ¡Ôñ×ÖÌå");
 	CFontDlg dlg;
 	dlg.SetFontDlg(&lgfont,false,false,0x000000);
 	sheet.AddPage(&dlg);
 	if (sheet.DoModal()==IDOK)
	{
		dlg.GetCurLogfont(&lgfont);
	}
 	
 }

void CYC_PROP_DIS::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(true);

    m_upc.GetColor(&levelupcolor);
    m_downc.GetColor(&leveldowncolor);
    m_normalc.GetColor(&normalcolor);
    m_disablec.GetColor(&disablecolor);
	
	rectx0=m_editx0;
	recty0=m_edity0;
	fontwidth=m_editfw;
	fontheight=m_editfh;
	fontjj=m_editjj;
	CPropertyPage::OnOK();
}

BOOL CYC_PROP_DIS::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editx0=rectx0;
	m_edity0=recty0;
	m_editfw=fontwidth;
	m_editfh=fontheight;
	m_editjj=fontjj;

	HWND hwnd;
	GetDlgItem(CZ_IDC_STATIC_C1,&hwnd);
	m_normalc.SubclassWindow(hwnd);
	m_normalc.SetColor(normalcolor);
	
	GetDlgItem(CZ_IDC_STATIC_C2,&hwnd);
	m_disablec.SubclassWindow(hwnd);
	m_disablec.SetColor(disablecolor);
	
	GetDlgItem(CZ_IDC_STATIC_C3,&hwnd);
	m_upc.SubclassWindow(hwnd);
	m_upc.SetColor(levelupcolor);
	
	GetDlgItem(CZ_IDC_STATIC_C4,&hwnd);
	m_downc.SubclassWindow(hwnd);
	m_downc.SetColor(leveldowncolor);
	
//CZ_IDC_DISUPSTATIC
	if (itkind==2)
	{
		m_upc.ShowWindow(SW_HIDE);
		m_downc.ShowWindow(SW_HIDE);
		
		GetDlgItem(CZ_IDC_DISUPSTATIC,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(CZ_IDC_DISDOWNSTATIC,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
	}
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CYC_PROP_DIS::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	
}
