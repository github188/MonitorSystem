#if !defined(AFX_CUREDITDIALOG_H__56314F22_FABE_11D5_B808_0050BABDFD68__INCLUDED_)
#define AFX_CUREDITDIALOG_H__56314F22_FABE_11D5_B808_0050BABDFD68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurEditDialog.h : header file
//
//#include "sp_draw.h"
#include "../../resource.h"
#include "canvas/LineProp.h"

/////////////////////////////////////////////////////////////////////////////
// CCurEditDialog dialog
typedef BOOL _stdcall ShowFindDotDlg(int kind,char * ename,char * cname);

class CCurEditDialog : public CDialog
{
// Construction
private:
	void UpdateColor();
public:
	COLORREF m_color;
	CStaticColor  m_ctrlColor;
	CCurEditDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCurEditDialog)
	enum { IDD = IDD_CUREDITDIALOG };
	CComboBox	m_referencecombo;
	CButton	m_enamebutton;
	BOOL	m_compare;
	CString	m_ename;
	float	m_scale;
	int		m_precission;
	int		m_width;
	int		m_swing;
	CString	m_cname;
	CString	m_reference;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurEditDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCurEditDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnColor();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUREDITDIALOG_H__56314F22_FABE_11D5_B808_0050BABDFD68__INCLUDED_)
