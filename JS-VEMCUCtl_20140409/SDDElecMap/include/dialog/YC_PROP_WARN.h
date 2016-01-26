#if !defined(AFX_YC_PROP_WARN_H__D18140AE_B9CE_4CAF_B1E9_14A0F6265A20__INCLUDED_)
#define AFX_YC_PROP_WARN_H__D18140AE_B9CE_4CAF_B1E9_14A0F6265A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// YC_PROP_WARN.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CYC_PROP_WARN dialog

class CYC_PROP_WARN : public CPropertyPage
{
	DECLARE_DYNCREATE(CYC_PROP_WARN)

// Construction
public:
	void SetPara(BYTE warnupenable,BYTE warnupkind,BYTE warnuplevels,
		char *warnupwave,BYTE warndownenable,BYTE warndownkind,BYTE	warndownlevels,
		char *warndownwave);
	void GetPara(BYTE * warnupenable,BYTE * warnupkind,BYTE * warnuplevels,
		char *warnupwave,BYTE * warndownenable,BYTE* warndownkind,BYTE* warndownlevels,
		char *warndownwave);
	CYC_PROP_WARN();
	~CYC_PROP_WARN();

    BYTE warnupkind; //越上限报警类型 //0位：推图 1位：闪烁 2位：语音
    BYTE warndownkind; //越上限报警类型 //0位：推图 1位：闪烁 2位：语音
// Dialog Data
	//{{AFX_DATA(CYC_PROP_WARN)
	enum { IDD = CZ_IDD_YC_WARN };
	int		m_upradio;
	int		m_downradio;
	BOOL	m_up;
	BOOL	m_down;
	CString	m_wave1;
	CString	m_wave2;
	BOOL	m_uppic;
	BOOL	m_upflash;
	BOOL	m_upwave;
	BOOL	m_downpic;
	BOOL	m_downflash;
	BOOL	m_downwave;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CYC_PROP_WARN)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CYC_PROP_WARN)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckUp1();
	afx_msg void OnIdcButtonupl();
	afx_msg void OnIdcButtondnl();
	afx_msg void OnIdcButtonupp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YC_PROP_WARN_H__D18140AE_B9CE_4CAF_B1E9_14A0F6265A20__INCLUDED_)
