#if !defined(AFX_YC_PROP_INFO_H__DA9B6CF0_83D0_4205_BFE0_94AB75401178__INCLUDED_)
#define AFX_YC_PROP_INFO_H__DA9B6CF0_83D0_4205_BFE0_94AB75401178__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../resource.h"
#include "canvas/lineprop.h"

// YC_PROP_INFO.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CYC_PROP_INFO dialog

typedef BOOL _stdcall ShowFindDotDlg(int kind,char * ename,char * cname);

class CYC_PROP_INFO : public CPropertyPage
{
	DECLARE_DYNCREATE(CYC_PROP_INFO)

// Construction
public:
	CYC_PROP_INFO(BYTE kind=1);//1 yc 2 ym
	~CYC_PROP_INFO();
	
	CRTUColorStatic m_hintc;
	CRTUColorStatic m_unitc;
	
    COLORREF hintcolor;//提示颜色;
    COLORREF unitcolor;//单位颜色
	BYTE hintenable;
	BYTE itkind;

	void GetPara(char * ename,int *digitlength,char * hint,char * unit,
		COLORREF *unitc,COLORREF *hintc,BYTE * hintena,float *hintjj,float *unitjj);
	void SetPara(char * ename,int digitlength,char * hint,char * unit,
		COLORREF unitc,COLORREF hintc,BYTE hintena,float hintjj,float unitjj);

// Dialog Data
	//{{AFX_DATA(CYC_PROP_INFO)
	enum { IDD = CZ_IDD_YC_INFO };
	CString	m_unit;
	int		m_digitlength;
	CString	m_ename;
	CString	m_hint;
	float	m_edithintdis;
	float	m_editunitdis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CYC_PROP_INFO)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CYC_PROP_INFO)
	virtual BOOL OnInitDialog();
	afx_msg void OnIdcButtonFinddot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStnClickedIdcStaticC5();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YC_PROP_INFO_H__DA9B6CF0_83D0_4205_BFE0_94AB75401178__INCLUDED_)
