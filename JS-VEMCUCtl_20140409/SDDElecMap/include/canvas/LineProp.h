#if !defined(AFX_LINEPROP_H__EA070242_CBD8_11D5_8B43_444553540000__INCLUDED_)
#define AFX_LINEPROP_H__EA070242_CBD8_11D5_8B43_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// LineProp.h : header file
//
//#include "sp_draw.h"
#include "../../resource.h"

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CComboBoxLineStyle window

class CComboBoxLineStyle : public CComboBox
{
// Construction
public:
	CComboBoxLineStyle();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboBoxLineStyle)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetPenStyle(UINT style);
	virtual ~CComboBoxLineStyle();
	void InitData();
	// Generated message map functions
protected:
	//{{AFX_MSG(CComboBoxLineStyle)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CStaticColor window

class CStaticColor : public CStatic
{
// Construction
public:
	CStaticColor();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticColor)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColor(COLORREF color);
	void GetColor(COLORREF* color);
	COLORREF m_Color;
	virtual ~CStaticColor();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticColor)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#define CRTUColorStatic CStaticColor
/////////////////////////////////////////////////////////////////////////////
// CLineProp dialog

class CLineProp :public CPropertyPage	//public CDialog  //
{
// Construction
public:
	void GetLogPen(LOGPEN *buf);
	void SetLogPen(LOGPEN* buf);

	UINT m_uStyle;
	COLORREF m_Color;
	CLineProp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineProp)
	enum { IDD = IDD_LINE };
	//CComboBox	m_ctrlStyle;
	UINT	m_nWidth;
	//}}AFX_DATA
  CStaticColor  m_ctrlColor;
	CComboBoxLineStyle m_ctrlStyle;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog( );

	// Generated message map functions
	//{{AFX_MSG(CLineProp)
	afx_msg void OnClickColor();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateColor();
};



/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEPROP_H__EA070242_CBD8_11D5_8B43_444553540000__INCLUDED_)
