// XLineDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/XLineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXLineDlg dialog


CXLineProp::CXLineProp(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CXLineProp::IDD)
{
	//{{AFX_DATA_INIT(CXLineDlg)
	m_L1 = 0;
	m_L2 = 0;
	m_L3 = 0;
	m_L4 = 0;
	m_Width = 0;
	m_bTranslate = FALSE;
	//}}AFX_DATA_INIT
}


void CXLineProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXLineDlg)
	DDX_Text(pDX, IDC_L1, m_L1);
	DDV_MinMaxInt(pDX, m_L1, 8, 100);
	DDX_Text(pDX, IDC_L2, m_L2);
	DDV_MinMaxInt(pDX, m_L2, 8, 100);
	DDX_Text(pDX, IDC_L3, m_L3);
	DDV_MinMaxInt(pDX, m_L3, 8, 100);
	DDX_Text(pDX, IDC_L4, m_L4);
	DDV_MinMaxInt(pDX, m_L4, 8, 100);
	DDX_Text(pDX, IDC_WIDTH, m_Width);
	DDV_MinMaxInt(pDX, m_Width, 1, 8);
	DDX_Check(pDX, IDC_CHECK1, m_bTranslate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXLineProp, CPropertyPage)
	//{{AFX_MSG_MAP(CXLineDlg)
	ON_BN_CLICKED(IDC_COLOR1, OnColor1)
	ON_BN_CLICKED(IDC_COLOR2, OnColor2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXLineDlg message handlers

BOOL CXLineProp::OnInitDialog()
{
	CSpinButtonCtrl *pCtrl=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN1);
	ASSERT(pCtrl!=NULL);
	pCtrl->SetRange(1,8);
  pCtrl->SetPos(m_Width);

	pCtrl=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN2);
	ASSERT(pCtrl!=NULL);
	pCtrl->SetRange(8,100);
  pCtrl->SetPos(m_L1);

	pCtrl=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN3);
	ASSERT(pCtrl!=NULL);
	pCtrl->SetRange(8,100);
	pCtrl->SetPos(m_L2);

	pCtrl=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN4);
	ASSERT(pCtrl!=NULL);
	pCtrl->SetRange(8,100);
	pCtrl->SetPos(m_L3);

	pCtrl=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN5);
	ASSERT(pCtrl!=NULL);
	pCtrl->SetRange(8,100);
	pCtrl->SetPos(m_L4);

	m_ctrlColor1.SubclassDlgItem(IDC_COLOR1,this);
	m_ctrlColor2.SubclassDlgItem(IDC_COLOR2,this);

	if (m_ctrlColor1.m_Color!=m_color1)
	{
		m_ctrlColor1.m_Color=m_color1;
		m_ctrlColor1.Invalidate();
	}
	if (m_ctrlColor2.m_Color!=m_color2)
	{
		m_ctrlColor2.m_Color=m_color2;
		m_ctrlColor2.Invalidate();
	}

	CButton *pBtn=(CButton *)GetDlgItem(IDC_CHECK1);
	ASSERT(pBtn!=NULL);
	pBtn->SetCheck(m_bTranslate);

	return TRUE;

}

void CXLineProp::SetXLine(COLORREF color1, COLORREF color2, BOOL blTranslate, int width, int type[])
{
	m_color1=color1;
	m_color2=color2;
	m_bTranslate=blTranslate;
	m_L1=type[0];
	m_L2=type[1];
	m_L3=type[2];
	m_L4=type[3];
	m_Width=width;
}

void CXLineProp::GetXLine(COLORREF & color1,COLORREF & color2, BOOL & blTranslate, int& width, int type[])
{
	color1=m_color1;
	color2=m_color2;
	blTranslate=m_bTranslate?TRUE:FALSE;
	type[0]=m_L1;
	type[1]=m_L2;
	type[2]=m_L3;
	type[3]=m_L4;
	width=m_Width;
}

void CXLineProp::OnColor1() 
{
	// TODO: Add your control notification handler code here
	if (m_color1==m_ctrlColor1.m_Color) return;
 
 	m_color1=m_ctrlColor1.m_Color;
	
}

void CXLineProp::OnColor2() 
{
	// TODO: Add your control notification handler code here
	if (m_color2==m_ctrlColor2.m_Color) return;
 	m_color2=m_ctrlColor2.m_Color;

}
