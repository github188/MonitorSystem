// FontProp.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "../../resource.h"
#include "dialog/FontProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontProp property page

IMPLEMENT_DYNCREATE(CFontProp, CPropertyPage)

CFontProp::CFontProp() : CPropertyPage(CFontProp::IDD)
{
	//{{AFX_DATA_INIT(CFontProp)
	m_TextFont = _T("");
	m_TextHeight = 0.0f;
	m_JqWidth = 0.0f;
	m_TextWidth = 0.0f;
	m_FangXiang = -1;
	//}}AFX_DATA_INIT
}

CFontProp::~CFontProp()
{
}

void CFontProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFontProp)
	DDX_CBString(pDX, IDC_FONTSTYLE, m_TextFont);
	DDX_Text(pDX, IDC_HEIGHT, m_TextHeight);
	DDV_MinMaxFloat(pDX, m_TextHeight, 2.e-002f, 1024.f);
	DDX_Text(pDX, IDC_JQWIDTH, m_JqWidth);
	DDV_MinMaxFloat(pDX, m_JqWidth, 0.f, 1024.f);
	DDX_Text(pDX, IDC_WIDTH, m_TextWidth);
	DDV_MinMaxFloat(pDX, m_TextWidth, 2.e-002f, 1024.f);
	DDX_Radio(pDX, IDC_HOR, m_FangXiang);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFontProp, CPropertyPage)
	//{{AFX_MSG_MAP(CFontProp)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontProp message handlers

BOOL CFontProp::OnInitDialog()
{
	CButton * pBtn=(CButton *)GetDlgItem(IDC_HOR);
	ASSERT(pBtn!=NULL);
	pBtn->SetCheck(m_FangXiang);
 
	pBtn=(CButton *)GetDlgItem(IDC_VER);
	ASSERT(pBtn!=NULL);
	pBtn->SetCheck((m_FangXiang+1)%2);

	CString str;
	CEdit * pEdit=(CEdit *)GetDlgItem(IDC_HEIGHT);
	ASSERT(pEdit!=NULL);
	str.Format("%.2f",m_TextHeight);
	pEdit->SetWindowText(str);

	pEdit=(CEdit *)GetDlgItem(IDC_WIDTH);
	ASSERT(pEdit!=NULL);
	str.Format("%.2f",m_TextWidth);
	pEdit->SetWindowText(str);

	pEdit=(CEdit *)GetDlgItem(IDC_JQWIDTH);
	ASSERT(pEdit!=NULL);
	str.Format("%.2f",m_JqWidth);
	pEdit->SetWindowText(str);

	CComboBox* pBox=(CComboBox*)GetDlgItem(IDC_FONTSTYLE);
	ASSERT(pBox!=NULL);
	pBox->LimitText(15);
	pBox->ResetContent();
	pBox->AddString("ËÎÌå");
	pBox->AddString("·ÂËÎ_GB2312");
	pBox->AddString("ºÚÌå");
	pBox->AddString("¿¬Ìå_GB2312");
	pBox->SelectString(-1,m_TextFont);


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

	UpdateData(FALSE);
	
	return TRUE;
}

void CFontProp::SetFontProp(float height, float width, float jqwidth, BOOL pl, COLORREF color, char font[])
{
	m_TextHeight=height;
	m_TextWidth=width;
	m_JqWidth=jqwidth;
	m_TextColor=color;
	m_TextFont=font;
	
	if (pl)
		m_FangXiang=0;
	else 
		m_FangXiang=1;
}

void CFontProp::GetFontProp(float &height, float &width, float &jqwidth, BOOL &pl, COLORREF &color, char *font)
{
	height=m_TextHeight;
	width=m_TextWidth;
	jqwidth=m_JqWidth;
	color=m_TextColor;
	strcpy(font,m_TextFont);

	pl=(m_FangXiang==0);
}


void CFontProp::UpdateColor()
{
	if (m_ctrlColor.m_Color!=m_TextColor)
	{
		m_ctrlColor.m_Color=m_TextColor;
		m_ctrlColor.Invalidate();
	}

	CSliderCtrl *pCtrlRed=(CSliderCtrl *)GetDlgItem(IDC_SLIDER1);
	ASSERT(pCtrlRed!=NULL);
	pCtrlRed->SetPos(GetRValue(m_TextColor));

	CSliderCtrl *pCtrlGreen=(CSliderCtrl *)GetDlgItem(IDC_SLIDER2);
	ASSERT(pCtrlGreen!=NULL);
	pCtrlGreen->SetPos(GetGValue(m_TextColor));

	CSliderCtrl *pCtrlBlue=(CSliderCtrl *)GetDlgItem(IDC_SLIDER3);
	ASSERT(pCtrlBlue!=NULL);
	pCtrlBlue->SetPos(GetBValue(m_TextColor));

	CString str;

	str.Format("%d",GetRValue(m_TextColor));
	SetDlgItemText(IDC_STATIC_RED,str);

	str.Format("%d",GetGValue(m_TextColor));
	SetDlgItemText(IDC_STATIC_GREEN,str);

	str.Format("%d",GetBValue(m_TextColor));
	SetDlgItemText(IDC_STATIC_BLUE,str);

}


void CFontProp::OnColor() 
{
	if (m_TextColor==m_ctrlColor.m_Color) return;
  
 	m_TextColor=m_ctrlColor.m_Color;
	UpdateColor();
}

void CFontProp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
  
	CSliderCtrl * pSlider=(CSliderCtrl *)pScrollBar;
	CString str;
	
	int pos=pSlider->GetPos();
	str.Format("%d",pos);
	
	byte r,g,b;
	r=GetRValue(m_TextColor);
	g=GetGValue(m_TextColor);
	b=GetBValue(m_TextColor);
	
	

	switch(pSlider->GetDlgCtrlID())
	{
	case IDC_SLIDER1:
		SetDlgItemText(IDC_STATIC_RED,str);
    m_TextColor=RGB((byte)pos,g,b);
		break;
	case IDC_SLIDER2:
		SetDlgItemText(IDC_STATIC_GREEN,str);
    m_TextColor=RGB(r,(byte)pos,b);
		break;
	case IDC_SLIDER3:
		SetDlgItemText(IDC_STATIC_BLUE,str);
    m_TextColor=RGB(r,g,(byte)pos);
		break;
	}

	UpdateColor();

}

void CFontProp::OnOK()
{
	UpdateData(TRUE);
	CPropertyPage::OnOK();
}
