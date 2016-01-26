// PropPositionRect.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropPositionRect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPositionRect property page

IMPLEMENT_DYNCREATE(CPropPositionRect, CPropertyPage)

CPropPositionRect::CPropPositionRect() : CPropertyPage(CPropPositionRect::IDD)
{
	//{{AFX_DATA_INIT(CPropPositionRect)
	m_x0 = 0.0f;
	m_y0 = 0.0f;
	m_x1 = 0.0f;
	m_y1 = 0.0f;
	//}}AFX_DATA_INIT
}

CPropPositionRect::~CPropPositionRect()
{
}

void CPropPositionRect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPositionRect)
	DDX_Text(pDX, IDC_EDIT1, m_x0);
	DDV_MinMaxFloat(pDX, m_x0, 0.f, 65535.f);
	DDX_Text(pDX, IDC_EDIT2, m_y0);
	DDV_MinMaxFloat(pDX, m_y0, 0.f, 65536.f);
	DDX_Text(pDX, IDC_EDIT3, m_x1);
	DDV_MinMaxFloat(pDX, m_x1, 1.e-009f, 65536.f);
	DDX_Text(pDX, IDC_EDIT4, m_y1);
	DDV_MinMaxFloat(pDX, m_y1, 1.e-009f, 65536.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPositionRect, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPositionRect)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPositionRect message handlers

void CPropPositionRect::SetContent(float x0, float y0, float x1, float y1)
{
	m_x0=x0;
	m_y0=y0;
	m_x1=x1-x0;
	m_y1=y1-y0;
}

void CPropPositionRect::GetContent(float &x0, float &y0, float &x1, float &y1)
{
	x0=m_x0;
	y0=m_y0;
	x1=m_x0+m_x1;
	y1=m_y0+m_y1;
}

void CPropPositionRect::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
