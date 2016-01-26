#if !defined(AFX_FONTDLG_H__4241C445_D36D_11D5_BFC5_0050BABDFD68__INCLUDED_)
#define AFX_FONTDLG_H__4241C445_D36D_11D5_BFC5_0050BABDFD68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../../resource.h"
// FontDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFontDlg dialog

class CFontDlg : public CPropertyPage
{
// Construction
public:
	COLORREF GetTextColor();
	void GetCurLogfont(LOGFONT* logfont);
	void SetFontDlg(LOGFONT* plogfont,BOOL sizeenable,BOOL colorenable,COLORREF textcolor);
	CFontDlg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFontDlg)
	enum { IDD = CZ_IDD_LOGFONTDIALOG };
	CEdit	m_styleeditctr;
	CEdit	m_sizeeditctr;
	CEdit	m_fonteditctr;
	CComboBox	m_colorcombo;
	CListBox	m_sizelist;
	CListBox	m_stylelist;
	CListBox	m_fontlist;
	CString	m_fontedit;
	CString	m_styleedit;
	CString	m_sizeedit;
	CString	m_font;
	CString	m_style;
	CString	m_size;
	BOOL	m_strikeout;
	BOOL	m_underline;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_colorenable;
	COLORREF m_textcolor;
	LOGFONT m_logfont;
	LOGFONT* p_logfont;

	BOOL m_sizeenable;

	// Generated message map functions
	//{{AFX_MSG(CFontDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeFontlist();
	afx_msg void OnSelchangeSizelist();
	afx_msg void OnSelchangeStylelist();
	afx_msg void OnUnderline();
	afx_msg void OnStrikeout();
	afx_msg void OnSelchangeColorcombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTDLG_H__4241C445_D36D_11D5_BFC5_0050BABDFD68__INCLUDED_)
