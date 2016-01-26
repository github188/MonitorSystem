#if !defined(AFX_SZ_SGBWLINEINFODLG_H__469B5BC3_3CA7_11D6_A5ED_0080C8F60823__INCLUDED_)
#define AFX_SZ_SGBWLINEINFODLG_H__469B5BC3_3CA7_11D6_A5ED_0080C8F60823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SZ_SgbwLineInfoDlg.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSZ_SgbwLineInfoDlg dialog
#include "canvas/LineProp.h"

class CSZ_SgbwLineInfoDlg : public CDialog
{
// Construction
public:
	void GetInfo(char *name, int* TdNo, float* Prop, int* Enum, BYTE* Zbgt, BYTE* UorI,COLORREF* Color);
	void SetInfo(char* name,int TdNo,float Prop,int Enum,BYTE Zbgt,BYTE UorI,COLORREF Color,bool bZbgt,bool bHaveUandI);
	CSZ_SgbwLineInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSZ_SgbwLineInfoDlg)
	enum { IDD = IDD_SZ_SGBWLINEINFO };
	CButton	m_zb;
	CButton	m_u;
	CButton	m_i;
	CButton	m_gt;
	CStaticColor	m_color;
	CString	m_name;
	int		m_enum;
	float	m_prop;
	int		m_tdno;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSZ_SgbwLineInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSZ_SgbwLineInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRSzSgbwzb();
	afx_msg void OnRSzSgbwu();
	afx_msg void OnRSzSgbwi();
	afx_msg void OnRSzSgbwgt();
	virtual void OnOK();
	afx_msg void OnSzBndot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetRadio();
	BYTE m_nZbgt;
	BYTE m_nUorI;
	char m_cDotname[17];
	COLORREF m_nColor;
	float m_nProp;
	int m_nEnum;
	int m_nTdNo;
	bool m_bZbgt;
	bool m_bHaveUandI;
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SZ_SGBWLINEINFODLG_H__469B5BC3_3CA7_11D6_A5ED_0080C8F60823__INCLUDED_)
