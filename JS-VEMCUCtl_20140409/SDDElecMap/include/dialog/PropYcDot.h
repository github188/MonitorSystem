#if !defined(AFX_PROPYCDOT_H__B7599841_3EA0_11D6_8B44_444553540000__INCLUDED_)
#define AFX_PROPYCDOT_H__B7599841_3EA0_11D6_8B44_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PropYcDot.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPropYcDot dialog

class CPropYcDot : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropYcDot)

// Construction
public:
	void Get_Yc_Dot_Name(char  buf[],DWORD bufsize);
	void Set_Yc_Dot_Name(LPCSTR buf);
	CPropYcDot();
	~CPropYcDot();

// Dialog Data
	//{{AFX_DATA(CPropYcDot)
	enum { IDD = IDD_NET_POWER };
	CString	m_dotName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropYcDot)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropYcDot)
	afx_msg void OnSelectDot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPYCDOT_H__B7599841_3EA0_11D6_8B44_444553540000__INCLUDED_)
