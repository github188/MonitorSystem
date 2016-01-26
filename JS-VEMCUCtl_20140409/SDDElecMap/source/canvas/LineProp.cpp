// LineProp.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/LineProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboBoxLineStyle

CComboBoxLineStyle::CComboBoxLineStyle()
{
}

CComboBoxLineStyle::~CComboBoxLineStyle()
{
}


BEGIN_MESSAGE_MAP(CComboBoxLineStyle, CComboBox)
	//{{AFX_MSG_MAP(CComboBoxLineStyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboBoxLineStyle message handlers

void CComboBoxLineStyle::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
  CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	CRect rt(lpDrawItemStruct->rcItem);

	COLORREF bk=dc.GetBkColor();
	COLORREF fg=dc.GetTextColor();

	if (lpDrawItemStruct->itemState&ODS_SELECTED)
	{
		bk=::GetSysColor(COLOR_HIGHLIGHT);
		fg=::GetSysColor(COLOR_HIGHLIGHTTEXT);
	}
  
	CBrush brush(bk);
	dc.FillRect(&rt,&brush);
	int nBKMode=dc.SetBkMode(TRANSPARENT);
  
	rt.left+=10;
	rt.right-=10;

	UINT linestyle=GetItemData(lpDrawItemStruct->itemID);
	
	if (linestyle!=PS_NULL)
	{
		CPen pen(linestyle,1,fg);
		CPen * poldpen=dc.SelectObject(&pen);
		dc.MoveTo(rt.left,(int)(rt.top+rt.Height()/2));
		dc.LineTo(rt.right,(int)(rt.top+rt.Height()/2));
		dc.SelectObject(poldpen);
	}
	else 
	{
	  dc.DrawText("空线",&rt,DT_CENTER|DT_VCENTER);
	}

	dc.SetBkMode(nBKMode);
	dc.Detach();
	
}


void CComboBoxLineStyle::InitData()
{
	AddString("实线");
	SetItemData(0,PS_SOLID);

	AddString("虚线");
	SetItemData(1,PS_DASH);

	AddString("点线");
	SetItemData(2,PS_DOT);

	AddString("点虚划线");
	SetItemData(3,PS_DASHDOT);

	AddString("双点虚划线");
	SetItemData(4,PS_DASHDOTDOT);

	AddString("消隐");
	SetItemData(5,PS_NULL);
}

void CComboBoxLineStyle::SetPenStyle(UINT style)
{
	int id=GetCurSel(); 
	DWORD dw;
	if (id>=0)
	{
		dw=GetItemData(id);
		if (dw==style) return;
	}

	for (int i=0;i<=GetCount()-1;i++)
	{
		dw=GetItemData(i);
		if (dw==style)
		{
			SetCurSel(i);
			return;
		}
	}
	
	ASSERT(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// CStaticColor message handlers

BEGIN_MESSAGE_MAP(CStaticColor, CStatic)
	//{{AFX_MSG_MAP(CStaticColor)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStaticColor

CStaticColor::CStaticColor()
{
}

CStaticColor::~CStaticColor()
{
}


void CStaticColor::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CBrush brush;
	brush.CreateSolidBrush(m_Color);

	CRect rt;
	GetClientRect(&rt);
	dc.FillRect(&rt,&brush);

	// Do not call CStatic::OnPaint() for painting messages
}



void CLineProp::OnClickColor() 
{
	// TODO: Add your control notification handler code here
	if (m_Color==m_ctrlColor.m_Color) return;
  
 	m_Color=m_ctrlColor.m_Color;
	UpdateColor();
}


void CStaticColor::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CColorDialog dlg(m_Color);
	if (dlg.DoModal()==IDOK)
	{
		  m_Color=dlg.GetColor();
			Invalidate();
	}
	
	CStatic::OnLButtonDown(nFlags, point);
}




/////////////////////////////////////////////////////////////////////////////
// CLineProp message handlers

BEGIN_MESSAGE_MAP(CLineProp, CPropertyPage)
	//{{AFX_MSG_MAP(CLineProp)
	ON_BN_CLICKED(IDC_COLOR, OnClickColor)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLineProp dialog

CLineProp::CLineProp(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CLineProp::IDD)
//		: CDialog(CLineProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineProp)
	m_nWidth = 0;
	//}}AFX_DATA_INIT
	m_uStyle=PS_SOLID;
}

BOOL CLineProp::OnInitDialog()
{
	CSpinButtonCtrl *pCtrlWidth=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN1);
	ASSERT(pCtrlWidth!=NULL);
	pCtrlWidth->SetRange(1,8);
	pCtrlWidth->SetPos(m_nWidth);

	m_ctrlColor.SubclassDlgItem(IDC_COLOR,this);
	m_ctrlStyle.SubclassDlgItem(IDC_COMBOX_STYLE,this);

	m_ctrlStyle.InitData();
	m_ctrlStyle.SetPenStyle(m_uStyle);	

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

	return TRUE;
}


void CLineProp::UpdateColor()
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

void CLineProp::SetLogPen(LOGPEN *buf)
{
	ASSERT(buf!=NULL);
	m_Color=buf->lopnColor;
	m_nWidth=buf->lopnWidth.x;
	m_uStyle=buf->lopnStyle;
}

void CLineProp::GetLogPen(LOGPEN *buf)
{
	ASSERT(buf!=NULL);
	ZeroMemory(buf,sizeof(LOGPEN));
	buf->lopnColor=m_Color;
	buf->lopnWidth.x=m_nWidth;
	buf->lopnStyle=m_uStyle;
}

void CLineProp::DoDataExchange(CDataExchange* pDX)
{
	int id;
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineProp)
	DDX_Control(pDX, IDC_COMBOX, m_ctrlStyle);
	DDX_Text(pDX, IDC_EDIT1, m_nWidth);
	DDV_MinMaxUInt(pDX, m_nWidth, 1, 8);
	//}}AFX_DATA_MAP
	
	if (pDX->m_bSaveAndValidate)
	{
		id=m_ctrlStyle.GetCurSel();
		ASSERT(id>=0);
		m_uStyle=m_ctrlStyle.GetItemData(id);
	} 
	else 
	{
//		m_ctrlStyle.SetPenStyle(m_uStyle);	
	}
}

void CLineProp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
  
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





void CStaticColor::GetColor(COLORREF *color)
{
  *color=m_Color;
}

void CStaticColor::SetColor(COLORREF color)
{
  m_Color=color;
	Invalidate();
}
