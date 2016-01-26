#if !defined(AFX_PROPNETBMP_H__B7599842_3EA0_11D6_8B44_444553540000__INCLUDED_)
#define AFX_PROPNETBMP_H__B7599842_3EA0_11D6_8B44_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PropNetBmp.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPropNetBmp dialog

class CPropNetBmp : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CPropNetBmp)

// Construction
public:
	void Get_Bmp_Name(char* bmpname,int size);
	void Set_Bmp_Name(LPCSTR bmpname);
	CPropNetBmp();
	~CPropNetBmp();

// Dialog Data
	//{{AFX_DATA(CPropNetBmp)
	enum { IDD = IDD_NET_BMP };
	CString	m_bmp_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropNetBmp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropNetBmp)
	afx_msg void OnBmpSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPNETBMP_H__B7599842_3EA0_11D6_8B44_444553540000__INCLUDED_)
