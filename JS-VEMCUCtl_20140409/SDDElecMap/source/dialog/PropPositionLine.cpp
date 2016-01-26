// PropPositionLine.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropPositionLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPositionLine property page

IMPLEMENT_DYNCREATE(CPropPositionLine, CPropertyPage)

CPropPositionLine::CPropPositionLine() : CPropertyPage(CPropPositionLine::IDD)
{
	//{{AFX_DATA_INIT(CPropPositionLine)
	m_x0 = 0.0f;
	m_y0 = 0.0f;
	m_x1 = 0.0f;
	m_y1 = 0.0f;
	//}}AFX_DATA_INIT
}

CPropPositionLine::~CPropPositionLine()
{
}

void CPropPositionLine::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPositionLine)
	DDX_Text(pDX, IDC_EDIT1, m_x0);
	DDV_MinMaxFloat(pDX, m_x0, 0.f, 65536.f);
	DDX_Text(pDX, IDC_EDIT2, m_y0);
	DDV_MinMaxFloat(pDX, m_y0, 0.f, 65536.f);
	DDX_Text(pDX, IDC_EDIT3, m_x1);
	DDV_MinMaxFloat(pDX, m_x1, 0.f, 65536.f);
	DDX_Text(pDX, IDC_EDIT4, m_y1);
	DDV_MinMaxFloat(pDX, m_y1, 0.f, 65536.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPositionLine, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPositionLine)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPositionLine message handlers

void CPropPositionLine::SetPosition(float x0, float y0, float x1, float y1)
{
  m_x0=x0;
	m_y0=y0;
	m_x1=x1;
	m_y1=y1;
}

void CPropPositionLine::GetPosition(float &x0, float &y0, float &x1, float &y1)
{
	x0=m_x0;
	y0=m_y0;
	x1=m_x1;
	y1=m_y1;
}
