// Image.cpp: implementation of the CImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../SDDElecMap.h"
#include "canvas/ImageX.h"
#include "canvas/CISBitmap.h"
#include <wingdi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CImageX g_image;
//////////////////////////////////////////////////////////////////////
// CHandleObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHandleObj::CHandleObj(HBITMAP hBmp)
{
  m_count=0;
	m_handle=hBmp;
}

CHandleObj::~CHandleObj()
{
	if (m_handle!=NULL)
		DeleteObject(m_handle);
}


CSize  CHandleObj::GetSize()
{
	CSize sz;
	sz.cx=0;
	sz.cy=0;
	if (m_handle==NULL)  return sz;

	CBitmap bmp;
	bmp.Attach(m_handle);
	BITMAP bm;
	bmp.GetBitmap( &bm );
	sz.cx=bm.bmWidth;
	sz.cy=bm.bmHeight;
	bmp.Detach();
	return sz;
}

void CHandleObj::DrawDIBSection(HDC hDC, int xDest, int yDest)
{
	HPALETTE hPal;

	HDC hDCMem = ::CreateCompatibleDC( hDC );

	// Create a logical palette for the bitmap
	DIBSECTION ds;
	BITMAPINFOHEADER &bmInfo = ds.dsBmih;
	if( ::GetObject(m_handle, sizeof(ds), &ds ) == 0 )
		return;			// Not a DIB Section
	
	HGDIOBJ hBmpOld =  ::SelectObject(hDCMem, m_handle);

	int nColors = bmInfo.biClrUsed ? bmInfo.biClrUsed : 1 << ds.dsBm.bmBitsPixel;
	
	if( ::GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE )
	{
		// Create a halftone palette if colors > 256. 
		if( nColors > 256 )
			hPal = ::CreateHalftonePalette(hDC);
		else
		{
			// Create the palette
			RGBQUAD *pRGB = new RGBQUAD[nColors];
			
			::GetDIBColorTable( hDCMem, 0, nColors, pRGB );
			
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
			LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
			
			pLP->palVersion = 0x300;
			pLP->palNumEntries = nColors;
			
			for( int i=0; i < nColors; i++)
			{
				pLP->palPalEntry[i].peRed = pRGB[i].rgbRed;
				pLP->palPalEntry[i].peGreen = pRGB[i].rgbGreen;
				pLP->palPalEntry[i].peBlue = pRGB[i].rgbBlue;
				pLP->palPalEntry[i].peFlags = 0;
			}
			
			hPal = ::CreatePalette( pLP );
			
			delete[] pLP;
			delete[] pRGB;
		}

		HPALETTE hPalOld = ::SelectPalette(hDC,hPal,FALSE);
		::RealizePalette(hDC);
		BitBlt(hDC,xDest,yDest,bmInfo.biWidth,bmInfo.biHeight,hDCMem,0,0,SRCCOPY);

		::SelectPalette(hDC,hPalOld,FALSE);
		// delete GDI objects
		::DeleteObject(hPal);
	}
	else
		BitBlt(hDC,xDest,yDest,bmInfo.biWidth,bmInfo.biHeight,hDCMem,0,0,SRCCOPY);

	::SelectObject(hDCMem, hBmpOld);
	::DeleteDC(hDCMem);
}

void CHandleObj::DrawTransparent(CDC *pDC, int x, int y, COLORREF crColour)
{
	if (m_handle==NULL) return;
	ASSERT(pDC!=NULL);
	CCISBitmap bmp;
	bmp.Attach(m_handle);
	bmp.DrawTransparent(pDC,x,y,crColour);
	bmp.Detach();
}

void CHandleObj::StretchBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest,BOOL blHScale,BOOL blVScale)
{
	HDC hDCMem = ::CreateCompatibleDC( hdcDest );

	// Create a logical palette for the bitmap
	DIBSECTION ds;
	BITMAPINFOHEADER &bmInfo = ds.dsBmih;
	if( ::GetObject(m_handle, sizeof(ds), &ds ) !=0  )
	{
		SetStretchBltMode(hdcDest,MAXSTRETCHBLTMODE);
	
		HGDIOBJ hBmpOld =::SelectObject(hDCMem, m_handle);
		CSize sz=GetSize();
		
		if (blHScale&&blVScale)
			::StretchBlt(hdcDest,nXOriginDest,nYOriginDest,nWidthDest,nHeightDest,hDCMem,0,0,sz.cx,sz.cy,SRCCOPY);
		else if (blHScale)
		{
			int loop=nHeightDest/sz.cy;
			CRect rt;
			int i;
			for (i=0;i<loop;i++)
			{
				rt.SetRect(nXOriginDest,nYOriginDest+i*sz.cy,nXOriginDest+nWidthDest,nYOriginDest+(i+1)*sz.cy);
				if (RectVisible(hdcDest,rt))  
					::StretchBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,sz.cx,sz.cy,SRCCOPY);
			}

			int mod=nHeightDest%sz.cy;
			if (mod!=0)
			{
			  rt.SetRect(nXOriginDest,nYOriginDest+i*sz.cy,nXOriginDest+nWidthDest,nYOriginDest+i*sz.cy+mod);
			  if (RectVisible(hdcDest,rt))  
					::StretchBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,sz.cx,mod,SRCCOPY);
			}

		} 
		else if (blVScale)
		{
			int loop=nWidthDest/sz.cx;
			CRect rt;
			int i;
			for (i=0;i<loop;i++)
			{
				rt.SetRect(nXOriginDest+i*sz.cx,nYOriginDest,nXOriginDest+(i+1)*sz.cx,nYOriginDest+nHeightDest);
				if (RectVisible(hdcDest,rt))  
					::StretchBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,sz.cx,sz.cy,SRCCOPY);
			}

			int mod=nWidthDest%sz.cx;
			if (mod!=0)
			{
			  rt.SetRect(nXOriginDest+i*sz.cx,nYOriginDest,nXOriginDest+i*sz.cx+mod,nYOriginDest+nHeightDest);
			  if (RectVisible(hdcDest,rt))  
					::StretchBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,mod,sz.cy,SRCCOPY);
			}
		}
		else 
		{
			int loopx=nWidthDest/sz.cx;
			int loopy=nHeightDest/sz.cy;
			int modx=nWidthDest%sz.cx;
			int mody=nHeightDest%sz.cy;
			CRect rt;
			int i,j;
 			for (i=0;i<loopx;i++)
			{
				for (j=0;j<loopy;j++)
				{
					rt.SetRect(nXOriginDest+i*sz.cx,nYOriginDest+j*sz.cy,nXOriginDest+(i+1)*sz.cx,nYOriginDest+(j+1)*sz.cy);
					if (RectVisible(hdcDest,rt))  
						//::StretchBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,sz.cx,sz.cy,SRCCOPY);
						::BitBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,SRCCOPY);
				}

				if(mody!=0)
				{
					rt.SetRect(nXOriginDest+i*sz.cx,nYOriginDest+j*sz.cy,nXOriginDest+(i+1)*sz.cx,nYOriginDest+j*sz.cy+mody);
					if (RectVisible(hdcDest,rt))  
						::BitBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,SRCCOPY);
				}
			}
	    
			if (modx!=0)
			{
				for (j=0;j<loopy;j++)
				{
					rt.SetRect(nXOriginDest+i*sz.cx,nYOriginDest+j*sz.cy,nXOriginDest+i*sz.cx+modx,nYOriginDest+(j+1)*sz.cy);
					if (RectVisible(hdcDest,rt))  
						//::StretchBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,sz.cx,sz.cy,SRCCOPY);
						::BitBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,SRCCOPY);
				}
				
				if(mody!=0)
				{
					rt.SetRect(nXOriginDest+i*sz.cx,nYOriginDest+j*sz.cy,nXOriginDest+i*sz.cx+modx,nYOriginDest+j*sz.cy+mody);
					if (RectVisible(hdcDest,rt))  
						::BitBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,SRCCOPY);
				}
			}
			
		}
		::SelectObject(hDCMem, hBmpOld);
	}

	if (hDCMem!=NULL)
			::DeleteDC(hDCMem);

}

void CHandleObj::TransparentBlt(HDC hdcDest, int x, int y, int width, int height, COLORREF crColour,BOOL blHScale,BOOL blVScale)
{
	HDC hDCMem = ::CreateCompatibleDC( hdcDest );

	// Create a logical palette for the bitmap
	DIBSECTION ds;
	BITMAPINFOHEADER &bmInfo = ds.dsBmih;
	if( ::GetObject(m_handle, sizeof(ds), &ds ) != 0 )
	{
		HGDIOBJ hBmpOld =::SelectObject(hDCMem, m_handle);
  
		CSize sz=GetSize();
		if (blHScale&&blVScale)
		{
			::TransparentBlt(hdcDest,x,y,width,height,hDCMem,0,0,sz.cx,sz.cy,crColour);
		}
		else if (blHScale)
		{
			int loop=height/sz.cy;
			CRect rt;
			int i;
			for (i=0;i<loop;i++)
			{
				rt.SetRect(x,y+i*sz.cy,x+width,y+(i+1)*sz.cy);
				if (RectVisible(hdcDest,rt))  
					::TransparentBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,sz.cx,sz.cy,crColour);
			}

			int mod=height%sz.cy;
			if (mod!=0)
			{
			  rt.SetRect(x,y+i*sz.cy,x+width,y+i*sz.cy+mod);
			  if (RectVisible(hdcDest,rt))  
					::TransparentBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,sz.cx,mod,crColour);
			}


		} 
		else if (blVScale)
		{
			int loop=width/sz.cx;
			CRect rt;
			int i;
			for (i=0;i<loop;i++)
			{
				rt.SetRect(x+i*sz.cx,y,x+(i+1)*sz.cx,y+height);
				if (RectVisible(hdcDest,rt))  
					::TransparentBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,sz.cx,sz.cy,crColour);
			}

			int mod=width%sz.cx;
			if (mod!=0)
			{
			  rt.SetRect(x+i*sz.cx,y,x+i*sz.cx+mod,y+height);
			  if (RectVisible(hdcDest,rt))  
					::TransparentBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,mod,sz.cy,crColour);
			}
		}
		else
		{
			int loopx=width/sz.cx;
			int loopy=height/sz.cy;
			int modx=width%sz.cx;
			int mody=height%sz.cy;
			CRect rt;
			int i,j;
 			for (i=0;i<loopx;i++)
			{
				for (j=0;j<loopy;j++)
				{
					rt.SetRect(x+i*sz.cx,y+j*sz.cy,x+(i+1)*sz.cx,y+(j+1)*sz.cy);
					if (RectVisible(hdcDest,rt))  
						::TransparentBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,sz.cx,sz.cy,crColour);
				}
				if(mody!=0)
				{
					rt.SetRect(x+i*sz.cx,y+j*sz.cy,x+(i+1)*sz.cx,y+j*sz.cy+mody);
					if (RectVisible(hdcDest,rt))  
						::TransparentBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,sz.cx,mody,crColour);
				}
			}
	    
			if (modx!=0)
			{
				for (j=0;j<loopy;j++)
				{
					rt.SetRect(x+i*sz.cx,y+j*sz.cy,x+i*sz.cx+modx,y+(j+1)*sz.cy);
					if (RectVisible(hdcDest,rt))  
						::TransparentBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,modx,sz.cy,crColour);
				}
				if(mody!=0)
				{
					rt.SetRect(x+i*sz.cx,y+j*sz.cy,x+i*sz.cx+modx,y+j*sz.cy+mody);
					if (RectVisible(hdcDest,rt))  
						::TransparentBlt(hdcDest,rt.left,rt.top,rt.Width(),rt.Height(),hDCMem,0,0,modx,mody,crColour);
				}
			}
		}
		
		::SelectObject(hDCMem, hBmpOld);

	}

	if (hDCMem!=NULL)
		DeleteDC(hDCMem);
}   


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CImageX::CImageX()
{

}

CImageX::~CImageX()
{

}


BOOL CImageX::RegisterImage(CString bmpname)
{
	CHandleObj* pObj;
	if (m_maplist.Lookup(bmpname,pObj)) 
	{
		pObj->m_count++;
	}
	else 
	{
		pObj=AddImage(bmpname);
		if (pObj==NULL) return FALSE;
		pObj->m_count=1;
	}
	return TRUE;

}

void CImageX::UnRegisterImage(CString bmpname)
{
	CHandleObj* pObj;
	if (!m_maplist.Lookup(bmpname,pObj))  return;
	ASSERT(pObj!=NULL);
	pObj->m_count--;
	if (pObj->m_count==0)
	{
		m_maplist.RemoveKey(bmpname);
		delete pObj;
	}
}

CHandleObj* CImageX::AddImage(CString bmpname)
{
	CString filepath = theApp.getCurrentAppPath();
	filepath.Format("%sglobal\\bmp\\%s.bmp",theApp.getCurrentAppPath(),bmpname);

	HBITMAP hBmp = (HBITMAP)::LoadImage(NULL, 
			filepath, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	
	if( hBmp == NULL ) 
	{
	  DWORD err=::GetLastError();
		CString str;
		str.Format("该位图不能装载%s,错误码:%d",filepath,err);
		#ifdef _DEBUG
			afxDump << str<< "\n";
		#endif

		//  AfxMessageBox(str);
		return NULL;
	}

	CHandleObj* pobj=new CHandleObj(hBmp);
	m_maplist[bmpname]=pobj;

	return pobj;
}

void CImageX::DrawDIBSection(CString bmpname,HDC hdc, int x0, int y0)
{
	CHandleObj* pObj;
	if (!m_maplist.Lookup(bmpname,pObj)) return ;
	pObj->DrawDIBSection(hdc,x0,y0);
}


void CImageX::DrawTransparent(CString bmpname, CDC *pDC, int x, int y, COLORREF crColour)
{
	CHandleObj* pObj;
	if (!m_maplist.Lookup(bmpname,pObj)) return ;
	ASSERT(pObj!=NULL);
	pObj->DrawTransparent(pDC,x,y,crColour);
}

void CImageX::TransparentBlt(CString bmpname, HDC hDC, int x, int y, int width, int height, COLORREF crColour,BOOL blHScale,BOOL blVScale)
{
	CHandleObj* pObj;
	if (!m_maplist.Lookup(bmpname,pObj)) return ;
	ASSERT(pObj!=NULL);
	pObj->TransparentBlt(hDC,x,y,width,height,crColour,blHScale,blVScale);
}

void CImageX::StretchBlt(LPCSTR bmpname,HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest,BOOL blHScale,BOOL blVScale)
{
	CHandleObj* pObj;
	if (!m_maplist.Lookup(bmpname,pObj)) return ;
	ASSERT(pObj!=NULL);
	pObj->StretchBlt(hdcDest,nXOriginDest,nYOriginDest,nWidthDest,nHeightDest,blHScale,blVScale);
}

