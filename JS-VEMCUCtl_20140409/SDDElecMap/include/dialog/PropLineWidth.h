#if !defined(AFX_PROPLINEWIDTH_H__078B7B89_9EFC_4402_9C29_B7A025D76C88__INCLUDED_)
#define AFX_PROPLINEWIDTH_H__078B7B89_9EFC_4402_9C29_B7A025D76C88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PropLineWidth.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPropLineWidth dialog

class CPropLineWidth : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropLineWidth)

// Construction
public:
	int Get_Line_Width();
	CPropLineWidth();
	~CPropLineWidth();

// Dialog Data
	//{{AFX_DATA(CPropLineWidth)
	enum { IDD = IDD_LINE_WIDTH };
	CSpinButtonCtrl	m_lineWidth_Spin;
	int		m_lineWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropLineWidth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropLineWidth)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPLINEWIDTH_H__078B7B89_9EFC_4402_9C29_B7A025D76C88__INCLUDED_)
