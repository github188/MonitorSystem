#if !defined(AFX_LINKCLUB_PROP1_H__BF177ADB_08A8_4768_8679_AEC7D960EC1C__INCLUDED_)
#define AFX_LINKCLUB_PROP1_H__BF177ADB_08A8_4768_8679_AEC7D960EC1C__INCLUDED_

#include "canvas/linkclub.h"
#include "../../resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinkClub_Prop1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLinkClub_Prop1 dialog
#include "canvas/PropertyList.h"

class CLinkClub_Prop1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CLinkClub_Prop1)

// Construction
public:
	STRUCT_CLUB myclub;
	void GetPara(char * name,STRUCT_CLUB * club);
	void SetPara(char * name,STRUCT_CLUB club);
	CLinkClub_Prop1();
	~CLinkClub_Prop1();
    
	STRUCT_PROPERTYITEM myitems[64];
	char myname[33];
  
// Dialog Data
	//{{AFX_DATA(CLinkClub_Prop1)
	enum { IDD = CZ_IDD_CLUB1 };
	CPropertyList	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLinkClub_Prop1)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLinkClub_Prop1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKCLUB_PROP1_H__BF177ADB_08A8_4768_8679_AEC7D960EC1C__INCLUDED_)
