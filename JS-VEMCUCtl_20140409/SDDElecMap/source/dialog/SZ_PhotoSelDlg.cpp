// SZ_PhotoSelDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/SZ_PhotoSelDlg.h"
#include "dialog/BmpSelectPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSZ_PhotoSelDlg dialog


CSZ_PhotoSelDlg::CSZ_PhotoSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSZ_PhotoSelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSZ_PhotoSelDlg)
	m_photoname = _T("");
	m_photoinfo = _T("");
	//}}AFX_DATA_INIT
	strcpy(m_sPhotoInfo,"");
	strcpy(m_sPhotoName,"");
}


void CSZ_PhotoSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSZ_PhotoSelDlg)
	DDX_Text(pDX, IDC_SZ_PHOTONAME, m_photoname);
	DDX_Text(pDX, IDC_SZ_PHOTOINFO, m_photoinfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSZ_PhotoSelDlg, CDialog)
	//{{AFX_MSG_MAP(CSZ_PhotoSelDlg)
	ON_BN_CLICKED(IDC_SZ_BNBMP, OnSzBnbmp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSZ_PhotoSelDlg message handlers

void CSZ_PhotoSelDlg::SetInfo(char *PhotoInfo, char *PhotoName)
{
	strcpy(m_sPhotoInfo,PhotoInfo);
	strcpy(m_sPhotoName,PhotoName);
}

void CSZ_PhotoSelDlg::GetInfo(char *PhotoInfo, char *PhotoName)
{
	strcpy(PhotoInfo,m_sPhotoInfo);
	strcpy(PhotoName,m_sPhotoName);
}

void CSZ_PhotoSelDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);

	strcpy(m_sPhotoInfo,m_photoinfo);
	strcpy(m_sPhotoName,m_photoname);

	char* p = strtok (m_sPhotoName, ".") ;
	lstrcpyn(m_sPhotoName,p,32) ;

	CDialog::OnOK();
}

void CSZ_PhotoSelDlg::OnSzBnbmp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CPropertySheet sheet( _T("»­Ãæ´úÀí.."));
	CBmpSelectPage dlg;
	BOOL m_bTranslate=true;
	COLORREF m_TranslateColor=RGB(255,255,255);
	dlg.SetContent(m_photoname,m_bTranslate,m_TranslateColor);
	sheet.AddPage((CPropertyPage*)&dlg);
	if (sheet.DoModal() == IDOK)
	{
		m_photoname=dlg.GetContent(m_bTranslate,m_TranslateColor);
		UpdateData(FALSE);
	}
}

BOOL CSZ_PhotoSelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_photoinfo=m_sPhotoInfo;
	m_photoname=m_sPhotoName;
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
