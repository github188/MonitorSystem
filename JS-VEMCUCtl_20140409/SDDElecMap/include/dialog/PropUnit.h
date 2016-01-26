#if !defined(AFX_PROPUNIT_H__3DA0447C_D76D_47E3_81BA_5AE4D80C449D__INCLUDED_)
#define AFX_PROPUNIT_H__3DA0447C_D76D_47E3_81BA_5AE4D80C449D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PropUnit.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPropUnit dialog

class CPropUnit : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropUnit)

// Construction
//////////////////////////////////////////////////////////////////////////
//pang修改12.8增加了下发遥信复归
//////////////////////////////////////////////////////////////////////////

public:
	void GetUnitParam(char text[],char UnitCaption[],BOOL& bUnittq,BOOL& bUnitsoe,BOOL& bUnityc,BOOL& bUnitycspecial,
		BOOL& bUnitdiandu,BOOL& bUnitlb,BOOL& bUnitset,BOOL& bUnitcanshu,BOOL& bUnitpct,BOOL& bUnitYxfg,BOOL& bUnitSGBW,BOOL& bUnitQLB,BOOL& bUnitCKLB);
	
	void SetUnitParam(char text[],char UnitCaption[],BOOL bUnittq,BOOL bUnitsoe,BOOL bUnityc,BOOL bUnitycspecial,
		BOOL bUnitdiandu,BOOL bUnitlb,BOOL bUnitset,BOOL bUnitcanshu,BOOL bUnitpct ,BOOL bUnitYxfg, BOOL bUnitSGBW, BOOL bUnitQLB, BOOL bUnitCKLB);
	CPropUnit();
	~CPropUnit();

// Dialog Data
	//{{AFX_DATA(CPropUnit)
	enum { IDD = IDD_Unit };
	CString	m_YK_Caption;
	BOOL	m_Unit_tq;
	BOOL	m_Unit_soe;
	BOOL	m_Unit_set;
	BOOL	m_Unit_lb;
	BOOL	m_Unit_canshu;
	BOOL	m_Unit_diandu;
	BOOL	m_Unit_yc;
	CString	m_Unit_Caption;
	BOOL	m_Unit_yc_special;
	BOOL	m_Unit_pct;
	BOOL    m_Unit_yxfg;
	BOOL    m_Unit_SGBW;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropUnit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropUnit)
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
public:
	BOOL m_Unit_QLB;
	BOOL m_Unit_CKLB;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPUNIT_H__3DA0447C_D76D_47E3_81BA_5AE4D80C449D__INCLUDED_)
