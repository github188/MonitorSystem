#if !defined(AFX_FILLPROP_H__A7A4F681_D5E9_11D5_8B43_444553540000__INCLUDED_)
#define AFX_FILLPROP_H__A7A4F681_D5E9_11D5_8B43_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FillProp.h : header file
//
#include "../../resource.h"
#include "canvas/LineProp.h"

/////////////////////////////////////////////////////////////////////////////
// CFillProp dialog

class CFillProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CFillProp)

// Construction
public:
	void SetFill(BOOL blTranslate, COLORREF color);
	void GetFill(BOOL& blTranslate,COLORREF& color);
	virtual BOOL OnInitDialog();
	CFillProp();
	~CFillProp();

// Dialog Data
	//{{AFX_DATA(CFillProp)
	enum { IDD = IDD_FILL };
	BOOL	m_bFill;
	//}}AFX_DATA

	COLORREF m_Color;
  CStaticColor  m_ctrlColor;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFillProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateColor();
	// Generated message map functions
	//{{AFX_MSG(CFillProp)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILLPROP_H__A7A4F681_D5E9_11D5_8B43_444553540000__INCLUDED_)
