#if !defined(AFX_SZ_SGBWMAININFODLG_H__80CFFAC2_3C12_11D6_A5ED_0080C8F60823__INCLUDED_)
#define AFX_SZ_SGBWMAININFODLG_H__80CFFAC2_3C12_11D6_A5ED_0080C8F60823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SZ_SgbwMainInfoDlg.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSZ_SgbwMainInfoDlg dialog
#include "canvas/SZ_SgbwLink.h"
#include "canvas/SZListCtrl.h"

class CSZ_SgbwMainInfoDlg : public CDialog
{
// Construction
public:
	void GetInfo(SZ_S_SGBWMAININFO* MainInfo,SZ_S_SGBWLINEBASE* LineInfo,int* LineCount,char* name);
	void SetInfo(SZ_S_SGBWMAININFO MainInfo,SZ_S_SGBWLINEBASE* LineInfo,int LineCount,char* name);
	CSZ_SgbwMainInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSZ_SgbwMainInfoDlg)
	enum { IDD = IDD_SZ_SGBWMAININFO };
	CEdit	m_imin_c;
	CEdit	m_imax_c;
	CButton	m_zbgt;
	CSZListCtrl	m_grid;
	CStatic	m_umin_s;
	CStatic	m_imin_s;
	CStatic	m_imax_s;
	CStatic	m_umax_s;
	CButton	m_uandi;
	CButton	m_i;
	CButton	m_u;
	CString	m_sgbwname;
	CString	m_rtuname;
	float	m_imax;
	float	m_imin;
	float	m_umax;
	float	m_umin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSZ_SgbwMainInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSZ_SgbwMainInfoDlg)
	virtual void OnOK();
	afx_msg void OnSzBwadd();
	afx_msg void OnSzBwdel();
	afx_msg void OnSzBwedit();
	afx_msg void OnSzBnrtu();
	afx_msg void OnSzUandi();
	afx_msg void OnSzZbgt();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ControlUandI();
	void CheckData();
	void SetColor();
	void DelLine(int LineNo);
	void EditLine(int LineNo,SZ_S_SGBWLINEBASE LineInfo);
	void AddLine(SZ_S_SGBWLINEBASE LineInfo);
	void WriteGridData();
	void WriteGridTitle();
	SZ_S_SGBWMAININFO m_sMainInfo;
	SZ_S_SGBWLINEBASE m_sLineInfo[SZ_SGBW_LINENUM];
	int m_nLineCount;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SZ_SGBWMAININFODLG_H__80CFFAC2_3C12_11D6_A5ED_0080C8F60823__INCLUDED_)
