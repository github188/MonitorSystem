#if !defined(AFX_INPUTTEXT_H__30DA1B28_D86E_11D5_8B43_444553540000__INCLUDED_)
#define AFX_INPUTTEXT_H__30DA1B28_D86E_11D5_8B43_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputText.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CInputText dialog

class CInputText : public CPropertyPage
{
	DECLARE_DYNCREATE(CInputText)

// Construction
public:
	void GetText(CString& str);
	void SetText(LPCSTR str);
	CInputText();
	~CInputText();

// Dialog Data
	//{{AFX_DATA(CInputText)
	enum { IDD = IDD_INPUT_TEXT };
	CString	m_Text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CInputText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CInputText)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTTEXT_H__30DA1B28_D86E_11D5_8B43_444553540000__INCLUDED_)
