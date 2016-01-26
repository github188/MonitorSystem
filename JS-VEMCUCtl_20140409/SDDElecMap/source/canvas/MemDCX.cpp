// MemDC.cpp: implementation of the CMemDC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/MemDCX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void CMemDCX::create(CDC *pDC, int width, int height)
{
	ASSERT(pDC!=NULL);
	ASSERT(m_oldbitmap==NULL);
	ASSERT(width>0);
	ASSERT(height>0);
	ASSERT(pDC->IsPrinting()!=TRUE);

	// create a memory dc
	CreateCompatibleDC(pDC);
	m_rect.SetRect(0,0,width,height);
	m_bitmap.CreateCompatibleBitmap(pDC, width, height);
	m_oldbitmap = SelectObject(&m_bitmap);
	SetWindowOrg(0,0);
}

void CMemDCX::SetSize(CDC *pDC,int width, int height)
{
	ASSERT(m_oldbitmap!=NULL);
	ASSERT(width>0);
	ASSERT(height>0);

	if ((m_rect.Width()==width)&&(m_rect.Height()==height)) return;

	m_rect.SetRect(0,0,width,height);
	SelectObject(&m_oldbitmap);
	m_bitmap.DeleteObject();
	m_bitmap.CreateCompatibleBitmap(pDC, width, height);
	m_oldbitmap = SelectObject(&m_bitmap);
	SetWindowOrg(0,0);
}

void CMemDCX::draw(CDC *pDC)
{
	ASSERT(pDC!=NULL); 
	ASSERT(m_oldbitmap!=NULL);

	pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
				this, m_rect.left, m_rect.top, SRCCOPY);
}

CSize CMemDCX::GetSize()
{
  return m_rect.Size();
}
