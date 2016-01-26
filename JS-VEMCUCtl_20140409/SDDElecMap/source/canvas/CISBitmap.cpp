// CISBitmap.cpp: implementation of the CCISBitmap class.
// Author:	Paul Reynolds 
// Date:	24/04/1998
// Version:	1.0
//////////////////////////////////////////////////////////////////////

#include <stdafx.h>
#include "canvas/CISBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCISBitmap::CCISBitmap()
{
	m_crBlack = 0;
	m_crWhite = RGB(255,255,255);
}

CCISBitmap::~CCISBitmap()
{

}

int CCISBitmap::Width()
{
	BITMAP bm;
	GetBitmap(&bm);
	return bm.bmWidth;
}

int CCISBitmap::Height()
{
	BITMAP bm;
	GetBitmap(&bm);
	return bm.bmHeight;
}

void CCISBitmap::DrawTransparent(CDC * pDC, int x, int y, COLORREF crColour)
{
	CDC l_BufferDC;
	l_BufferDC.CreateCompatibleDC(pDC);

	CBitmap  l_BufferBitmap;
	l_BufferBitmap.CreateCompatibleBitmap( pDC,Width(),Height());

	CBitmap* l_pOldBufferBitmap = l_BufferDC.SelectObject(&l_BufferBitmap);

	CDC l_MaskDC;
	l_MaskDC.CreateCompatibleDC(pDC);

	CBitmap l_MaskBitmap;

	l_MaskBitmap.CreateBitmap(Width(), Height(), 1, 1, NULL);

	CBitmap* l_pOldMaskBitmap = l_MaskDC.SelectObject(&l_MaskBitmap);

	#define ROP4_TRANSPARENTBLIT		0xCCAA0020
	#define SRCMASK						0x00220326

	////////////////////////////////////////////////////////////////////////////
	// Fill with transparent color
	////////////////////////////////////////////////////////////////////////////

	CRect rt;
	rt.SetRect(0,0,Width(),Height());
	l_BufferDC.FillSolidRect( &rt, crColour) ;

	////////////////////////////////////////////////////////////////////////////
	// Blit the bitmap to the buffer
	////////////////////////////////////////////////////////////////////////////

	CDC l_MemoryDC;
	l_MemoryDC.CreateCompatibleDC(pDC) ;

	CBitmap* l_pOldMemoryBitmap = l_MemoryDC.SelectObject(this) ;

	l_BufferDC.BitBlt( 0, 0, Width(), Height(), &l_MemoryDC, 0, 0, SRCCOPY ) ;

	l_MemoryDC.SelectObject(l_pOldMemoryBitmap) ;

	////////////////////////////////////////////////////////////////////////////
	// Create the mask.
	////////////////////////////////////////////////////////////////////////////

	COLORREF l_crOldBack = l_BufferDC.SetBkColor(crColour) ;

	l_MaskDC.BitBlt(0, 0, Width(),Height(), &l_BufferDC, 0, 0, SRCCOPY) ;

	l_BufferDC.SetBkColor(l_crOldBack) ;

	////////////////////////////////////////////////////////////////////////////
	// Draw the bitmap transparently now, since not all devices support this
	// and Windows 98 does not (duh?), if this fails, we do it the long way.
	////////////////////////////////////////////////////////////////////////////

	if(!pDC->MaskBlt(x,y,Width(),Height(),&l_BufferDC,0,0,l_MaskBitmap,0,0,ROP4_TRANSPARENTBLIT))
	{
		CDC l_CopyDC;
		l_CopyDC.CreateCompatibleDC(pDC);

		CBitmap l_CopyBitmap;
		l_CopyBitmap.CreateCompatibleBitmap(pDC, Width(), Height()) ;

		CBitmap* l_pOldCopyBitmap = l_CopyDC.SelectObject(&l_CopyBitmap);

		l_CopyDC.BitBlt(0, 0, Width(), Height(), pDC,  x, y, SRCCOPY) ;
		l_CopyDC.BitBlt(0, 0, Width(), Height(), &l_MaskDC,   0, 0, SRCAND) ;
		l_BufferDC.BitBlt(0, 0, Width(), Height(), &l_MaskDC,   0, 0, SRCMASK) ;
		l_CopyDC.BitBlt(0, 0, Width(), Height(), &l_BufferDC, 0, 0, SRCPAINT) ;
		pDC->BitBlt(x, y, Width(), Height(), &l_CopyDC,   0, 0, SRCCOPY) ;

		l_CopyDC.SelectObject(l_pOldCopyBitmap) ;
	}

	////////////////////////////////////////////////////////////////////////////
	// Clean up.
	////////////////////////////////////////////////////////////////////////////

	l_MaskDC.SelectObject(l_pOldMaskBitmap) ;
	l_BufferDC.SelectObject(l_pOldBufferBitmap) ;
}
