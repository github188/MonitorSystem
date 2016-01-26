// YC_PROP_INFO.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/YC_PROP_INFO.h"
#include "dialog/FontDlg.h"
#include "canvas/yc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYC_PROP_INFO property page

IMPLEMENT_DYNCREATE(CYC_PROP_INFO, CPropertyPage)

CYC_PROP_INFO::CYC_PROP_INFO(BYTE kind) : CPropertyPage(CYC_PROP_INFO::IDD)
{
	itkind=kind;
	//{{AFX_DATA_INIT(CYC_PROP_INFO)
	m_unit = _T("");
	m_digitlength = 0;
	m_ename = _T("");
	m_hint = _T("");
	m_edithintdis = 0.0f;
	m_editunitdis = 0.0f;
	//}}AFX_DATA_INIT
}

CYC_PROP_INFO::~CYC_PROP_INFO()
{
}

void CYC_PROP_INFO::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYC_PROP_INFO)
	DDX_CBString(pDX, CZ_IDC_COMBO_UNIT, m_unit);
	DDV_MaxChars(pDX, m_unit, 16);
	DDX_Text(pDX, CZ_IDC_EDIT_DIGITLENGTH, m_digitlength);
	DDV_MinMaxInt(pDX, m_digitlength, 4, 10);
	DDX_Text(pDX, CZ_IDC_EDIT_ENAME, m_ename);
	DDV_MaxChars(pDX, m_ename, 16);
	DDX_Text(pDX, CZ_IDC_EDIT_HINT, m_hint);
	DDV_MaxChars(pDX, m_hint, 32);
	DDX_Text(pDX, CZ_IDC_EDITHINTDIS, m_edithintdis);
	DDV_MinMaxFloat(pDX, m_edithintdis, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITUNITDIS, m_editunitdis);
	DDV_MinMaxFloat(pDX, m_editunitdis, 0.f, 32000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYC_PROP_INFO, CPropertyPage)
	//{{AFX_MSG_MAP(CYC_PROP_INFO)
	ON_BN_CLICKED(CZ_IDC_BUTTON_FINDDOT, OnIdcButtonFinddot)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(CZ_IDC_STATIC_C5, OnStnClickedIdcStaticC5)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYC_PROP_INFO message handlers
void CYC_PROP_INFO::SetPara(char * ename,int digitlength,char * hint,char * unit,
		COLORREF unitc,COLORREF hintc,BYTE hintena,float hintj,float unitj)
{
	char tempename[17],temphint[33],tempunit[17];
    memcpy(tempename,ename,sizeof(char)*17);
    memcpy(temphint,hint,sizeof(char)*33);
    memcpy(tempunit,unit,sizeof(char)*17);
	tempename[16]='\0';
	temphint[32]='\0';
	tempunit[16]='\0';
	m_ename=tempename;
	m_digitlength=digitlength;
	m_hint=temphint;
	m_unit=tempunit;
	unitcolor=unitc;
	hintcolor=hintc;
	hintenable=hintena;
	m_edithintdis=hintj;
	m_editunitdis=unitj;

}

void CYC_PROP_INFO::GetPara(char * ename,int *digitlength,char * hint,char * unit,
		COLORREF *unitc,COLORREF *hintc,BYTE * hintena,float *hintjj,float *unitjj)
{
	//UpdateData(true);
    memcpy(ename,m_ename,sizeof(char)*17);
	ename[16]='\0';
	*digitlength=m_digitlength;
	char s1[33],s2[17];
	sprintf(s1,"%s",m_hint);
	s1[32]='\0';
	memcpy(hint,s1,sizeof(char)*33);
	sprintf(s2,"%s",m_unit);
	s2[16]='\0';
	memcpy(unit,s2,sizeof(char)*17);
	*unitc=unitcolor;
	*hintc=hintcolor;
	*hintena=hintenable;
	*hintjj=m_edithintdis;
	*unitjj=m_editunitdis;
}

BOOL CYC_PROP_INFO::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	HWND hwnd;

	GetDlgItem(CZ_IDC_STATIC_C5,&hwnd);
	m_hintc.SubclassWindow(hwnd);
	m_hintc.SetColor(hintcolor);
	
	GetDlgItem(CZ_IDC_STATIC_C6,&hwnd);
	m_unitc.SubclassWindow(hwnd);
	m_unitc.SetColor(unitcolor);

	if (hintenable==0)
	{
		GetDlgItem(CZ_IDC_STATIC_HINT,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(CZ_IDC_EDIT_HINT,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(CZ_IDC_STATIC_HINTCOLOR,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(CZ_IDC_STATIC_C5,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(CZ_IDC_STATICHINTDIS,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(CZ_IDC_EDITHINTDIS,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

 void CYC_PROP_INFO::OnIdcButtonFinddot() 
 {
 	// TODO: Add your control notification handler code here
 //===========
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
 	if (pf==NULL)
	{
 		sprintf(s,"%s 调入函数ShowFindDotDlg 失败！",dllf);
 		AfxMessageBox(s);
 		FreeLibrary(hmydll);
 		hmydll=NULL;
 		return;
 	}
 	CWnd * pp=this->GetParent();
	BYTE dotkind=0;
	if (itkind==1)
	{
		dotkind=1;
	}
	if (itkind==2)
	{
		dotkind=3;
	}
 	if ((*pf)(dotkind,myename,mycname))
 	{
 		UpdateData(true);
 		m_ename=myename;
 		UpdateData(false);
 	}
 
 	FreeLibrary(hmydll);
 	
 }

//DEL void CYC_PROP_INFO::OnButtonFont() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	CPropertySheet sheet("选择字体");
//DEL 	CFontDlg dlg;
//DEL 	dlg.SetFontDlg(&lgfont,false,false,0x000000);
//DEL 	sheet.AddPage(&dlg);
//DEL 	sheet.DoModal();
//DEL 	
//DEL }

//DEL void CYC_PROP_INFO::OnClose() 
//DEL {
//DEL 	// TODO: Add your message handler code here and/or call default
//DEL 	
//DEL 	CPropertyPage::OnClose();
//DEL }

void CYC_PROP_INFO::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
    m_hintc.GetColor(&hintcolor);
    m_unitc.GetColor(&unitcolor);
	
	CPropertyPage::OnOK();
}

void CYC_PROP_INFO::OnStnClickedIdcStaticC5()
{
	// TODO: 在此添加控件通知处理程序代码
}
