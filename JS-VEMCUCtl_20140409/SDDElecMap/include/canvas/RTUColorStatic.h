#if !defined(AFX_RTUCOLORSTATIC_H__8D671D42_DC00_4C43_85A8_57CE969A5900__INCLUDED_)
#define AFX_RTUCOLORSTATIC_H__8D671D42_DC00_4C43_85A8_57CE969A5900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// RTUColorStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRTUColorStatic window

class CRTUColorStatic : public CStatic
{
// Construction
public:
	CRTUColorStatic();
	COLORREF m_color;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRTUColorStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColor(COLORREF color);
	void GetColor(COLORREF *color);
	virtual ~CRTUColorStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRTUColorStatic)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTUCOLORSTATIC_H__8D671D42_DC00_4C43_85A8_57CE969A5900__INCLUDED_)
