// EnBitmap.h: interface for the CEnBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENBITMAP_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_)
#define AFX_ENBITMAP_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEnBitmap : public CBitmap  
{
public:
	HBITMAP SetBitmap(HBITMAP hBitmap);
	CRect GetRect();
	void AlphaDisplay(CDC  *pDC,BYTE bAlpha);
	CEnBitmap();
	virtual ~CEnBitmap();
	BOOL ExtendDraw(CDC *pDC,CRect rc, int nX, int nY);
	BOOL ExtendDrawImage(CEnBitmap &bmp,CRect rc, int nX, int nY);

	BOOL LoadImage(LPCTSTR szImagePath, COLORREF crBack = 0);
	BOOL LoadImage(UINT uIDRes, LPCTSTR szResourceType, HMODULE hInst = NULL, COLORREF crBack = 0); 

	// helpers
	static HBITMAP LoadImageFile(LPCTSTR szImagePath, COLORREF crBack = 0);
	static HBITMAP LoadImageResource(UINT uIDRes, LPCTSTR szResourceType, HMODULE hInst = NULL, COLORREF crBack = 0); 
	static BOOL GetResource(LPCTSTR lpName, LPCTSTR lpType, HMODULE hInst, void* pResource, int& nBufSize);
	static IPicture* LoadFromBuffer(BYTE* pBuff, int nSize);
	HRGN BitmapToRegion(COLORREF cTransparentColor = 0,
					COLORREF cTolerance = 0x101010);
	BOOL Draw(CDC *pDC, LPRECT r);
	BOOL DrawImage(CEnBitmap &bmp,int nX,int nY,int nCol,int nRow);
	void TransparentBlt(CDC &dc, CRect rc,UINT crTransparent );	
	int	 GetWidth()
	{
		BITMAP bm;
		memset( &bm, 0, sizeof(bm) );
		GetBitmap(&bm);
		return bm.bmWidth;
	}

	int	 GetHeight()
	{
		BITMAP bm;
		memset( &bm, 0, sizeof(bm) );
		GetBitmap(&bm);
		return bm.bmHeight;
	}
	
	int CEnBitmap::Grey( int r, int g, int b )
	{
		return ((( b * 11 ) + ( g * 59 ) + ( r * 30 )) / 100 );
	}
	void DrawGreyScale( CDC *pDC);
	void DitherBlt (HDC hdcDest, int nXDest, int nYDest, int nWidth, 
				int nHeight, HBITMAP hbm, int nXSrc, int nYSrc);							
	HPALETTE CreateReservedPalette(CDC *pDC);
	void FadeColorToGrayScale( CDC *pDC, int xDest, int yDest, int nLoops, 
									int nDelay ) ;
	HRGN InflateRegion(HRGN hRgn, int nXInflate, int nYInflate);
	HRGN CreateRegionExt(DWORD nCount, CONST RGNDATA *pRgnData );
	BOOL StretchDraw(CDC *pDC, LPRECT r, LPRECT sr);
protected:
	static HBITMAP ExtractBitmap(IPicture* pPicture, COLORREF crBack);
	static int GetFileType(LPCTSTR szImagePath);

};

#endif // !defined(AFX_ENBITMAP_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_)
