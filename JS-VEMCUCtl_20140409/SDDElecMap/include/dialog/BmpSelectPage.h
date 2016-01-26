#if !defined(AFX_BMPSELECTPAGE_H__1718D0C1_F4C3_11D5_8B43_444553540000__INCLUDED_)
#define AFX_BMPSELECTPAGE_H__1718D0C1_F4C3_11D5_8B43_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpSelectPage.h : header file
//
#include "canvas/LineProp.h"
#include "../../resource.h"


/////////////////////////////////////////////////////////////////////////////
// CStaticBmp window

class CStaticBmp : public CStatic
{
// Construction
public:
	CStaticBmp();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticBmp)
	public:
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBmpFile(LPCSTR file);
	COLORREF m_translatecolor;
	virtual ~CStaticBmp();

	// Generated message map functions
protected:
	CString m_bmpfile;
	CBitmap m_bmp;
	//{{AFX_MSG(CStaticBmp)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CBmpSelectPage dialog

class CBmpSelectPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CBmpSelectPage)

// Construction
public:
	CString GetContent(BOOL& bTranslate,COLORREF& translatecolor,BOOL* blHScale=NULL,BOOL* blVScale=NULL);
	void SetContent(LPCSTR bmpname,BOOL bTranslate,COLORREF translatecolor,BOOL blHScale=TRUE,BOOL blVScale=TRUE);
	CBmpSelectPage();
	~CBmpSelectPage();

  CStaticColor  m_ctrlColor;
	CStaticBmp    m_ctrlBmp;

// Dialog Data
	//{{AFX_DATA(CBmpSelectPage)
	enum { IDD = IDD_BITMAP };
	CListBox	m_List;
	CString	m_bmpName;
	BOOL	m_bTranslate;
	BOOL	m_bl_Scale_Hor;
	BOOL	m_bl_Scale_Ver;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBmpSelectPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COLORREF m_Color;
	// Generated message map functions
	//{{AFX_MSG(CBmpSelectPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBmpClicked();
	afx_msg void OnSelchangeList1();
	afx_msg void OnPreView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillListBox();
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPSELECTPAGE_H__1718D0C1_F4C3_11D5_8B43_444553540000__INCLUDED_)
