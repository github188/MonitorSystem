#if !defined(AFX_YDX_PROP_INFO_H__48BB5C63_55B0_462D_985E_284FE01E032E__INCLUDED_)
#define AFX_YDX_PROP_INFO_H__48BB5C63_55B0_462D_985E_284FE01E032E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// YDX_PROP_INFO.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CYDX_PROP_INFO dialog

typedef BOOL _stdcall ShowFindDotDlg(int kind,char * ename,char * cname);
class CYDX_PROP_INFO : public CPropertyPage
{
	DECLARE_DYNCREATE(CYDX_PROP_INFO)

// Construction
public:
	CYDX_PROP_INFO();
	~CYDX_PROP_INFO();

	void SetPara(char * mainename,char * slaveename,BYTE handit);
	void GetPara(char * mainename,char * slaveename,BYTE * handit);
// Dialog Data
	//{{AFX_DATA(CYDX_PROP_INFO)
	enum { IDD = CZ_IDD_YDX_INFO };
	CString	m_edit1;
	CString	m_edit2;
	BOOL	m_check;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CYDX_PROP_INFO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CYDX_PROP_INFO)
	virtual BOOL OnInitDialog();
	afx_msg void OnIdcButtonFinddot();
	afx_msg void OnIdcButtonFinddot2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YDX_PROP_INFO_H__48BB5C63_55B0_462D_985E_284FE01E032E__INCLUDED_)
