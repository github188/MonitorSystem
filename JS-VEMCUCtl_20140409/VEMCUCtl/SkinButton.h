#if !defined(AFX_SKINBUTTON_H__2A579B8E_CF46_4C1D_BCE3_DCBD018233AB__INCLUDED_)
#define AFX_SKINBUTTON_H__2A579B8E_CF46_4C1D_BCE3_DCBD018233AB__INCLUDED_
// Download by http://www.codefans.net
#include "EnBitmap.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinButton.h : header file
//
#define STYLE_TOOLBAR 0x0001
#define STYLE_BITMAP 0x0002
/////////////////////////////////////////////////////////////////////////////
// CSkinButton window

class CSkinButton : public CButton
{
// Construction
public:
	CSkinButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	COLORREF m_crText;
	void SetTextColor(COLORREF cr);
	void SetStyle(int nStyle){m_nStyle = nStyle;}
	HBITMAP SetBitmap(HBITMAP hBitmap );
	CEnBitmap m_bmpButton[3];
	void LoadBitmap(LPCTSTR szImagePath,int index=0);
	BOOL LoadBitmap(UINT uIDRes,int index=0,COLORREF crBack=0);
	BOOL DrawText(CDC *pDC ,CRect rc,COLORREF crText);
	int  SetIndex(int index);
	virtual ~CSkinButton();
	
	BOOL	m_bMouseIn;
	BOOL	m_bDown;
	BOOL	m_bFocus;
	BOOL	m_bEnable;
	int		m_nStyle;
	int        m_index;

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINBUTTON_H__2A579B8E_CF46_4C1D_BCE3_DCBD018233AB__INCLUDED_)
