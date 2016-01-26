#if !defined(AFX_YX_PROP_INFO_H__054AFE47_484D_4994_8916_2F00724F64E6__INCLUDED_)
#define AFX_YX_PROP_INFO_H__054AFE47_484D_4994_8916_2F00724F64E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// YX_PROP_INFO.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CYX_PROP_INFO dialog

typedef BOOL _stdcall ShowFindDotDlg(int kind,char * ename,char * cname);
class CYX_PROP_INFO : public CPropertyPage
{
	DECLARE_DYNCREATE(CYX_PROP_INFO)

// Construction
public:
	void SetPara(char * ename,BYTE handit);
	void GetPara(char * ename,BYTE * handit);
	CYX_PROP_INFO();
	~CYX_PROP_INFO();

// Dialog Data
	//{{AFX_DATA(CYX_PROP_INFO)
	enum { IDD = CZ_IDD_YX_INFO };
	CString	m_edit;
	BOOL	m_check;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CYX_PROP_INFO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CYX_PROP_INFO)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFinddot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YX_PROP_INFO_H__054AFE47_484D_4994_8916_2F00724F64E6__INCLUDED_)
