#if !defined(AFX_LINKCLUB_PROP2_H__4273FA52_26B0_41F0_8870_4D519E32D971__INCLUDED_)
#define AFX_LINKCLUB_PROP2_H__4273FA52_26B0_41F0_8870_4D519E32D971__INCLUDED_

#include "selfwidget/gxListCtrl.h"
#include "canvas/LinkClub.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// LinkClub_Prop2.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CLinkClub_Prop2 dialog

class CLinkClub_Prop2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CLinkClub_Prop2)

// Construction
public:
	void GetPara(BYTE * count,STRUCT_SINGLEBAR * info);
	void SetPara(BYTE count,STRUCT_SINGLEBAR info[64]);
	CLinkClub_Prop2();
	~CLinkClub_Prop2();

	STRUCT_SINGLEBAR myinfo[64];
	BYTE mycount;

// Dialog Data
	//{{AFX_DATA(CLinkClub_Prop2)
	enum { IDD = CZ_IDD_CLUB2 };
	gxListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLinkClub_Prop2)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLinkClub_Prop2)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKCLUB_PROP2_H__4273FA52_26B0_41F0_8870_4D519E32D971__INCLUDED_)
