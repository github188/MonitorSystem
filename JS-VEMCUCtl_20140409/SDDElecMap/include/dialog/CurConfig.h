#if !defined(AFX_CURCONFIG_H__0D0869E2_F3D0_11D5_B808_0050BABDFD68__INCLUDED_)
#define AFX_CURCONFIG_H__0D0869E2_F3D0_11D5_B808_0050BABDFD68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CurConfig.h : header file
//
//#include "sp_draw.h"
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CCurConfig dialog

class CCurConfig : public CPropertyPage
{
//	DECLARE_DYNCREATE(CCurConfig)
// Construction
public:
	void SetDoubleCurveEnable(BOOL EnableFlag);
	int m_secondpagecount;
	CPropertyPage* p_page;
	CPropertySheet* p_sheet;
	void GetCurParam(LPSTR content);
	void SetCurParam(LPCSTR content);
	void SetSecondLeftCurEnable(BOOL EnableFlag);
	CCurConfig();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCurConfig)
	enum { IDD = IDD_CURCONFIG };
//	CEdit	m_firstrightintervaledit;
	CEdit	m_firstrightunitedit;
	CEdit	m_firstrightmaxedit;
	CEdit	m_firstrightminedit;
	CStatic	m_firstrightmaxstatic;
	CStatic	m_firstrightminstatic;
	CStatic	m_secondrightminstatic;
	CStatic	m_secondleftminstatic;
	CEdit	m_secondrightminedit;
	CEdit	m_secondleftminedit;
	CEdit	m_secondrightunitedit;
	CEdit	m_secondrightmaxedit;
//	CEdit	m_secondrightintervaledit;
	CEdit	m_secondleftunitedit;
	CEdit	m_secondleftmaxedit;
//	CEdit	m_secondleftintervaledit;
	CStatic	m_secondrightunitstatic;
	CStatic	m_secondrightmaxstatic;
	CStatic	m_secondrightintervalstatic;
	CStatic	m_secondleftunitstatic;
	CStatic	m_secondleftmaxstatic;
	CStatic	m_secondleftintervalstatic;
	CString	m_curgroupname;
	int		m_singlecur;
	CString	m_firstleftunit;
	CString	m_firstrightunit;
	int		m_levelcur;
	CString	m_secondleftunit;
	CString	m_secondrightunit;
	float	m_firstleftinterval;
	float	m_firstleftmax;
	float	m_firstrightinterval;
	float	m_firstrightmax;
	float	m_secondleftinterval;
	float	m_secondleftmax;
	float	m_secondrightinterval;
	float	m_secondrightmax;
	int		m_coordinate;
	float	m_firstleftmin;
	float	m_secondleftmin;
	float	m_secondrightmin;
	float	m_firstrightmin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurConfig)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCurConfig)
	afx_msg void OnShqCurgroupdoublecur();
	afx_msg void OnShqCurgroupsinglecur();
	afx_msg void OnShqSinglecoordinate();
	afx_msg void OnShqDoublecoordinate();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void SetSecondRightCurEnable(BOOL EnableFlag);
	void SetFirstRightCurEnable(BOOL EnableFlag);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURCONFIG_H__0D0869E2_F3D0_11D5_B808_0050BABDFD68__INCLUDED_)
