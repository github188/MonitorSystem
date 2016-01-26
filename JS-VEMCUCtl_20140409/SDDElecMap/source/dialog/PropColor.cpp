// PropColor.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropColor property page

IMPLEMENT_DYNCREATE(CPropColor, CPropertyPage)

CPropColor::CPropColor() : CPropertyPage(CPropColor::IDD)
{
	//{{AFX_DATA_INIT(CPropColor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPropColor::~CPropColor()
{
}

void CPropColor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropColor)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropColor, CPropertyPage)
	//{{AFX_MSG_MAP(CPropColor)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropColor message handlers

void CPropColor::OnColor() 
{
	// TODO: Add your control notification handler code here
	if (m_Color==m_ctrlColor.m_Color) return;
  
 	m_Color=m_ctrlColor.m_Color;
	UpdateColor();
	
}

void CPropColor::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);

	CSliderCtrl * pSlider=(CSliderCtrl *)pScrollBar;
	CString str;
	int pos=pSlider->GetPos();
	str.Format("%d",pos);
	byte r,g,b;
	r=GetRValue(m_Color);
	g=GetGValue(m_Color);
	b=GetBValue(m_Color);

	switch(pSlider->GetDlgCtrlID())
	{
	case IDC_SLIDER1:
		SetDlgItemText(IDC_STATIC_RED,str);
    m_Color=RGB((byte)pos,g,b);
		break;
	case IDC_SLIDER2:
		SetDlgItemText(IDC_STATIC_GREEN,str);
    m_Color=RGB(r,(byte)pos,b);
		break;
	case IDC_SLIDER3:
		SetDlgItemText(IDC_STATIC_BLUE,str);
    m_Color=RGB(r,g,(byte)pos);
		break;
	}

	UpdateColor();

}

void CPropColor::UpdateColor()
{

	if (m_ctrlColor.m_Color!=m_Color)
	{
		m_ctrlColor.m_Color=m_Color;
		m_ctrlColor.Invalidate();
	}


	CSliderCtrl *pCtrlRed=(CSliderCtrl *)GetDlgItem(IDC_SLIDER1);
	ASSERT(pCtrlRed!=NULL);
	pCtrlRed->SetPos(GetRValue(m_Color));

	CSliderCtrl *pCtrlGreen=(CSliderCtrl *)GetDlgItem(IDC_SLIDER2);
	ASSERT(pCtrlGreen!=NULL);
	pCtrlGreen->SetPos(GetGValue(m_Color));

	CSliderCtrl *pCtrlBlue=(CSliderCtrl *)GetDlgItem(IDC_SLIDER3);
	ASSERT(pCtrlBlue!=NULL);
	pCtrlBlue->SetPos(GetBValue(m_Color));

	CString str;

	str.Format("%d",GetRValue(m_Color));
	SetDlgItemText(IDC_STATIC_RED,str);

	str.Format("%d",GetGValue(m_Color));
	SetDlgItemText(IDC_STATIC_GREEN,str);

	str.Format("%d",GetBValue(m_Color));
	SetDlgItemText(IDC_STATIC_BLUE,str);

}

BOOL CPropColor::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ctrlColor.SubclassDlgItem(IDC_COLOR,this);

	CSliderCtrl *pCtrlRed=(CSliderCtrl *)GetDlgItem(IDC_SLIDER1);
	ASSERT(pCtrlRed!=NULL);
	pCtrlRed->SetRange(0,255);
	pCtrlRed->SetTicFreq(64);
	pCtrlRed->SetLineSize(1);
	pCtrlRed->SetPageSize(64);

	CSliderCtrl *pCtrlGreen=(CSliderCtrl *)GetDlgItem(IDC_SLIDER2);
	ASSERT(pCtrlGreen!=NULL);
	pCtrlGreen->SetRange(0,255);
	pCtrlGreen->SetTicFreq(64);
	pCtrlGreen->SetLineSize(1);
	pCtrlGreen->SetPageSize(64);


	CSliderCtrl *pCtrlBlue=(CSliderCtrl *)GetDlgItem(IDC_SLIDER3);
	ASSERT(pCtrlBlue!=NULL);
	pCtrlBlue->SetRange(0,255);
	pCtrlBlue->SetTicFreq(64);
	pCtrlBlue->SetLineSize(1);
	pCtrlBlue->SetPageSize(64);

	UpdateColor();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropColor::GetColor(COLORREF &color)
{
  color=m_Color;
}

void CPropColor::SetColor(COLORREF color)
{
  m_Color=color;
}
