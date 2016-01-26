#if !defined(AFX_SKINSTATIC_H__8008CAA3_1EE3_4F71_A6B7_976421CC0915__INCLUDED_)
#define AFX_SKINSTATIC_H__8008CAA3_1EE3_4F71_A6B7_976421CC0915__INCLUDED_

#include "EnBitmap.h"	

#if _MSC_VER > 1000
#pragma once
#endif

// SkinStatic.h : header file

/////////////////////////////////////////////////////////////////////////////
// CSkinStatic window
class CSkinStatic : public CStatic
{
// Construction
public:
	CSkinStatic();
	virtual ~CSkinStatic();

// Attributes
public:
	CEnBitmap m_bmpStatic;
	CString m_strText;
	int m_nTextStart;
	COLORREF m_crText;
	int m_nX;
	int m_nY;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinStatic)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetText(CString strText,int nStart,COLORREF cr);
	void SetText(char * pszText,int nStart,COLORREF cr);
	void SetExtendPoint(int nX,int nY){m_nX=nX;m_nY=nY;}
	BOOL DrawText(CDC *pDC,CRect rc ,CString strText,UINT uFormat=DT_SINGLELINE|DT_VCENTER);
	void LoadBitmap(LPCTSTR szImagePath);
	CString& GetText();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINSTATIC_H__8008CAA3_1EE3_4F71_A6B7_976421CC0915__INCLUDED_)
