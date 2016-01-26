// MemDCX.h: interface for the CMemDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMDC_H__E1FB803B_7C38_4A09_BFD8_035C87938599__INCLUDED_)
#define AFX_MEMDC_H__E1FB803B_7C38_4A09_BFD8_035C87938599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMemDCX : public CDC  
{
private:
	CBitmap* m_oldbitmap; // bitmap originally found in cmemdc
	CRect m_rect; // rectangle of drawing area.
public:
	CBitmap m_bitmap;	// offscreen bitmap

	CSize GetSize();
	void draw(CDC *pDC);
	void SetSize(CDC* pDC,int width,int height);
	void create(CDC *pDC,int width,int height);

	CMemDCX(){m_oldbitmap=NULL;}

	virtual ~CMemDCX(){ if(m_oldbitmap!=NULL) SelectObject(m_oldbitmap);};

};

#endif // !defined(AFX_MEMDC_H__E1FB803B_7C38_4A09_BFD8_035C87938599__INCLUDED_)
