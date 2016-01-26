#if !defined(AFX_CZWAVSELECTPAGE_H__DDDFBC97_0F2B_4436_926F_A0771AE0F7F9__INCLUDED_)
#define AFX_CZWAVSELECTPAGE_H__DDDFBC97_0F2B_4436_926F_A0771AE0F7F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CZWavSelectPage.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CCZWavSelectPage dialog

class CCZWavSelectPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCZWavSelectPage)

// Construction
public:
	void SetContent(LPCSTR wavename);
	CString GetContent();
	CCZWavSelectPage();
	~CCZWavSelectPage();

// Dialog Data
	//{{AFX_DATA(CCZWavSelectPage)
	enum { IDD = CZ_IDD_WAV };
	CButton	m_button;
	CListBox	m_list;
	CString	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCZWavSelectPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void FillListBox();
	// Generated message map functions
	//{{AFX_MSG(CCZWavSelectPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CZWAVSELECTPAGE_H__DDDFBC97_0F2B_4436_926F_A0771AE0F7F9__INCLUDED_)
