#if !defined(AFX_BITMAPTABCTRL_H__E9A7DD81_EB67_458B_B34F_BA4067F3B159__INCLUDED_)
#define AFX_BITMAPTABCTRL_H__E9A7DD81_EB67_458B_B34F_BA4067F3B159__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BitmapTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitmapTabCtrl window

class CBitmapTabCtrl : public CTabCtrl
{
// Construction
public:
CBitmapTabCtrl();

// Attributes
public:
enum {
BITMAP_NONE = 0,// do not show bitmap
BITMAP_TILE,// tile the bitmap to fill the dialog
BITMAP_STRETCH,// stretch the bitmap so it fits to the dialog
BITMAP_CENTER// center the bitmap inside the dialog
};

// Operations
public:

// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CBitmapTabCtrl)
//}}AFX_VIRTUAL

// Implementation
public:
BOOL SetBitmap(UINT uResource, int nType);
VOID DrawItemEdge();
VOID DrawItemText();
virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
virtual ~CBitmapTabCtrl();

// Generated message map functions
protected:
int m_nType;// see enum above
CBitmap m_bmp;
//{{AFX_MSG(CBitmapTabCtrl)
afx_msg void OnPaint();  
afx_msg BOOL OnEraseBkgnd(CDC* pDC);
 //}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPTABCTRL_H__E9A7DD81_EB67_458B_B34F_BA4067F3B159__INCLUDED_)
