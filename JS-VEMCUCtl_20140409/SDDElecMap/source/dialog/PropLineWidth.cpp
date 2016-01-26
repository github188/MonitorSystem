// PropLineWidth.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropLineWidth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropLineWidth property page

IMPLEMENT_DYNCREATE(CPropLineWidth, CPropertyPage)

CPropLineWidth::CPropLineWidth() : CPropertyPage(CPropLineWidth::IDD)
{
	//{{AFX_DATA_INIT(CPropLineWidth)
	m_lineWidth = 0;
	//}}AFX_DATA_INIT
}

CPropLineWidth::~CPropLineWidth()
{
}

void CPropLineWidth::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropLineWidth)
	DDX_Control(pDX, IDC_SPIN1, m_lineWidth_Spin);
	DDX_Text(pDX, IDC_EDIT1, m_lineWidth);
	DDV_MinMaxInt(pDX, m_lineWidth, 1, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropLineWidth, CPropertyPage)
	//{{AFX_MSG_MAP(CPropLineWidth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropLineWidth message handlers

int CPropLineWidth::Get_Line_Width()
{
   return m_lineWidth;
}

BOOL CPropLineWidth::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_lineWidth_Spin.SetRange(1,8);
	m_lineWidth_Spin.SetPos(m_lineWidth);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
