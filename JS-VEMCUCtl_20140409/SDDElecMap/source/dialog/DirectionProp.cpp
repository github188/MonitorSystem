// DirectionProp.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/DirectionProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirectionProp property page

IMPLEMENT_DYNCREATE(CDirectionProp, CPropertyPage)

CDirectionProp::CDirectionProp() : CPropertyPage(CDirectionProp::IDD)
{
	//{{AFX_DATA_INIT(CDirectionProp)
	m_bTranslate = FALSE;
	m_bZeroShow = FALSE;
	m_len = 0;
	m_width = 0;
	m_dotname = _T("");
	//}}AFX_DATA_INIT
}

CDirectionProp::~CDirectionProp()
{
}

void CDirectionProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirectionProp)
	DDX_Check(pDX, IDC_CHECK2, m_bTranslate);
	DDX_Check(pDX, IDC_CHECK1, m_bZeroShow);
	DDX_Text(pDX, IDC_L1, m_len);
	DDV_MinMaxUInt(pDX, m_len, 8, 100);
	DDX_Text(pDX, IDC_WIDTH, m_width);
	DDX_Text(pDX, IDC_EDIT1, m_dotname);
	DDV_MaxChars(pDX, m_dotname, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDirectionProp, CPropertyPage)
	//{{AFX_MSG_MAP(CDirectionProp)
	ON_BN_CLICKED(IDC_COLOR1, OnColor1)
	ON_BN_CLICKED(IDC_COLOR2, OnColor2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirectionProp message handlers

void CDirectionProp::GetProp(int &len, int &width, COLORREF &frontcolor, COLORREF &backcolor, BOOL &bTranslate, char dotname[], BOOL &bZeroShow)
{
	len = m_len;
	width = m_width;
	frontcolor = m_color1;
	backcolor = m_color2;
	bTranslate = m_bTranslate;
	bZeroShow = m_bZeroShow;
	strcpy(dotname,m_dotname);
}

void CDirectionProp::SetProp(int len, int width, COLORREF frontcolor, COLORREF backcolor, BOOL bTranslate, char dotname[], BOOL bZeroShow)
{
	m_len = len;
	m_width = width;
	m_color1 = frontcolor;
	m_color2 = backcolor;
	m_bTranslate = bTranslate;
	m_bZeroShow = bZeroShow;
	m_dotname = dotname;
}





BOOL CDirectionProp::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	CSpinButtonCtrl *pCtrl=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN1);
	ASSERT(pCtrl!=NULL);
	pCtrl->SetRange(1,8);
  pCtrl->SetPos(m_width);

	pCtrl=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN2);
	ASSERT(pCtrl!=NULL);
	pCtrl->SetRange(8,100);
  pCtrl->SetPos(m_len);


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

	CButton *pBtn=(CButton *)GetDlgItem(IDC_CHECK2);
	ASSERT(pBtn!=NULL);
	pBtn->SetCheck(m_bTranslate);

	pBtn=(CButton *)GetDlgItem(IDC_CHECK1);
	ASSERT(pBtn!=NULL);
	pBtn->SetCheck(m_bZeroShow);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDirectionProp::OnColor1() 
{
	// TODO: Add your control notification handler code here
	if (m_color1==m_ctrlColor1.m_Color) return;
 
 	m_color1=m_ctrlColor1.m_Color;
		
}

void CDirectionProp::OnColor2() 
{
	// TODO: Add your control notification handler code here
	if (m_color2==m_ctrlColor2.m_Color) return;
 
 	m_color2=m_ctrlColor2.m_Color;
		
}

void CDirectionProp::OnButton1() 
{

}
