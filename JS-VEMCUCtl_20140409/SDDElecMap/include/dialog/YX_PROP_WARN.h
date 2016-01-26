#if !defined(AFX_YX_PROP_WARN_H__1C93F9F0_08C1_4C06_B128_694942846A57__INCLUDED_)
#define AFX_YX_PROP_WARN_H__1C93F9F0_08C1_4C06_B128_694942846A57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// YX_PROP_WARN.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CYX_PROP_WARN dialog

class CYX_PROP_WARN : public CPropertyPage
{
	DECLARE_DYNCREATE(CYX_PROP_WARN)

// Construction
public:
	CYX_PROP_WARN();
	~CYX_PROP_WARN();

	void SetPara(BYTE warn1enable,BYTE warn1kind,BYTE warn1levels,
		char *warn1wave,BYTE warn0enable,BYTE warn0kind,BYTE	warn0levels,
		char *warn0wave);
	void GetPara(BYTE * warn1enable,BYTE * warn1kind,BYTE * warn1levels,
		char *warn1wave,BYTE * warn0enable,BYTE* warn0kind,BYTE* warn0levels,
		char *warn0wave);

    BYTE warn1kind; //0->1报警类型 //0位：推图 1位：闪烁 2位：语音
    BYTE warn0kind; //1->0报警类型 //0位：推图 1位：闪烁 2位：语音

// Dialog Data
	//{{AFX_DATA(CYX_PROP_WARN)
	enum { IDD = CZ_IDD_YX_WARN };
	BOOL	m_1pic;
	BOOL	m_1;
	BOOL	m_1flash;
	BOOL	m_1wave;
	CString	m_wave1;
	CString	m_wave2;
	int		m_1radio;
	int		m_0radio;
	BOOL	m_0;
	BOOL	m_0pic;
	BOOL	m_0flash;
	BOOL	m_0wave;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CYX_PROP_WARN)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CYX_PROP_WARN)
	virtual BOOL OnInitDialog();
	afx_msg void OnIdcButton1l();
	afx_msg void OnIdcButton0l();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YX_PROP_WARN_H__1C93F9F0_08C1_4C06_B128_694942846A57__INCLUDED_)
