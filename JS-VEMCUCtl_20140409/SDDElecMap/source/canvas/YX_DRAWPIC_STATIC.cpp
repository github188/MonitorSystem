// YX_DRAWPIC_STATIC.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/YX_DRAWPIC_STATIC.h"
#include "canvas/ImageX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYX_DRAWPIC_STATIC

CYX_DRAWPIC_STATIC::CYX_DRAWPIC_STATIC()
{
	m_kind=-1;
}

CYX_DRAWPIC_STATIC::~CYX_DRAWPIC_STATIC()
{
}


BEGIN_MESSAGE_MAP(CYX_DRAWPIC_STATIC, CStatic)
	//{{AFX_MSG_MAP(CYX_DRAWPIC_STATIC)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYX_DRAWPIC_STATIC message handlers

void CYX_DRAWPIC_STATIC::SetDrawBmp(CString name,COLORREF color,int trans)
{
	if ((m_kind & 0x0f)==0)
	{
		g_image.UnRegisterImage(m_name);
	}
	m_kind=-1;
	if (name!="")
	{
		g_image.RegisterImage(name);
		m_name=name;
		m_color=color;
		if (trans==0) {m_kind=0;}else{m_kind=0x10;}
	}
	Invalidate();
}
void CYX_DRAWPIC_STATIC::SetDrawVec(CString name,COLORREF color,int allcolor)
{
	if ((m_kind & 0x0f)==0)
	{
		g_image.UnRegisterImage(m_name);
	}
	m_kind=-1;
	if (name!="")
	{
		m_name=name;
		m_kind=1;
		m_color=color;
		if (allcolor==0) {m_kind=1;}else{m_kind=0x11;}
	}
	Invalidate();
}

void CYX_DRAWPIC_STATIC::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetWindowRect(rect);

	CBrush brush(COLORREF(0xffffff));
	CBrush * oldbrush=dc.SelectObject(&brush);
	dc.FillRect(CRect(0,0,rect.Width()-4,rect.Height()-4),&brush);
	dc.SelectObject(&brush);
	
	// TODO: Add your message handler code here
	if (m_kind==0)
	{
		g_image.TransparentBlt(m_name,dc.m_hDC,0,0,rect.Width()-4,rect.Height()-4,0xffffff);
	}
	if (m_kind==0x10)
	{
		g_image.TransparentBlt(m_name,dc.m_hDC,0,0,rect.Width()-4,rect.Height()-4,m_color);
	}
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CYX_DRAWPIC_STATIC::ClearIt()
{
	m_kind=-1;
	Invalidate();
}

void CYX_DRAWPIC_STATIC::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// TODO: Add your message handler code here
	if ((m_kind & 0x0f)==0)
	{
		g_image.UnRegisterImage(m_name);
	}
	
}
