// PropPositionText.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropPositionText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPositionText property page

IMPLEMENT_DYNCREATE(CPropPositionText, CPropertyPage)

CPropPositionText::CPropPositionText() : CPropertyPage(CPropPositionText::IDD)
{
	//{{AFX_DATA_INIT(CPropPositionText)
	m_x0 = 0.0f;
	m_y0 = 0.0f;
	//}}AFX_DATA_INIT
}

CPropPositionText::~CPropPositionText()
{
}

void CPropPositionText::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPositionText)
	DDX_Text(pDX, IDC_EDIT1, m_x0);
	DDV_MinMaxFloat(pDX, m_x0, 0.f, 65536.f);
	DDX_Text(pDX, IDC_EDIT2, m_y0);
	DDV_MinMaxFloat(pDX, m_y0, 0.f, 65536.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPositionText, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPositionText)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPositionText message handlers

void CPropPositionText::SetContent(float x0, float y0)
{
	m_x0=x0;
	m_y0=y0;
}

void CPropPositionText::GetContent(float &x0, float &y0)
{
	x0=m_x0;
	y0=m_y0;
}
