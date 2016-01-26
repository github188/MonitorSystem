// RTUColorStatic.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/RTUColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRTUColorStatic

CRTUColorStatic::CRTUColorStatic()
{
}

CRTUColorStatic::~CRTUColorStatic()
{
}


BEGIN_MESSAGE_MAP(CRTUColorStatic, CStatic)
	//{{AFX_MSG_MAP(CRTUColorStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRTUColorStatic message handlers

void CRTUColorStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush brush(m_color);
	CBrush * oldbrush=dc.SelectObject(&brush);
	CRect rect;
	GetWindowRect(rect);
	dc.Rectangle(0,0,rect.Width()-2,rect.Height()-2);
	dc.SelectObject(oldbrush);

	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CRTUColorStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CColorDialog dlg(m_color);
	if (dlg.DoModal()==IDOK)
	{
		SetColor(dlg.GetColor());
	}
	
	CStatic::OnLButtonDown(nFlags, point);
}

void CRTUColorStatic::SetColor(COLORREF color)
{
	m_color=color;
	Invalidate();
}

void CRTUColorStatic::GetColor(COLORREF *color)
{
	*color=m_color;
}
