#if !defined(AFX_TYANIMATE_PROP_H__055E9499_B988_42F2_90C2_9E6402051D21__INCLUDED_)
#define AFX_TYANIMATE_PROP_H__055E9499_B988_42F2_90C2_9E6402051D21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// TyAnimate_Prop.h : header file
//
#include "../../resource.h"
#include "canvas/TyAnimate.h"
#include "canvas/ColorListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTyAnimate_Prop dialog

class CTyAnimate_Prop : public CPropertyPage
{
	DECLARE_DYNCREATE(CTyAnimate_Prop)

// Construction
public:
	Stu_TyAnimate myanimate;
	void GetPara(Stu_TyAnimate * animate);
	void SetPara(Stu_TyAnimate * animate);
	CTyAnimate_Prop();
	~CTyAnimate_Prop();

// Dialog Data
	//{{AFX_DATA(CTyAnimate_Prop)
	enum { IDD = CZ_IDD_ANIMATE_INFO };
	CColorListCtrl	m_listvec;
	CColorListCtrl	m_listbmp;
	CString	m_ename;
	float	m_high;
	float	m_width;
	float	m_x0;
	float	m_y0;
	int		m_diskind;
	int		m_pickind;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTyAnimate_Prop)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTyAnimate_Prop)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnIdcButtonFinddot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TYANIMATE_PROP_H__055E9499_B988_42F2_90C2_9E6402051D21__INCLUDED_)
