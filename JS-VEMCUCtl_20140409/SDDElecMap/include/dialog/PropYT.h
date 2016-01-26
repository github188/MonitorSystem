#if !defined(AFX_PROPYT_H__4848EDD4_8D70_4D17_A9F0_AF35C3EF42AF__INCLUDED_)
#define AFX_PROPYT_H__4848EDD4_8D70_4D17_A9F0_AF35C3EF42AF__INCLUDED_
#include "../../resource.h"
#include "canvas/SysLinkTy.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PropYT.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropYT dialog

class CPropYT : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropYT)

// Construction
public:
	void Update_BS_UI();
	void SetYTParam(char text[],char Ykname[],int bVT,char Yxname_FK[],tagBS_Array& BS_Array);
	void GetYTParam(char text[],char Ykname[],int& bVT,char Yxname_FK[],tagBS_Array& BS_Array);
	CPropYT();
	~CPropYT();

// Dialog Data
	//{{AFX_DATA(CPropYT)
	enum { IDD = IDD_YT };
	CButton	m_mod;
	CButton	m_del;
	CButton	m_add;
	CListBox	m_list_bs;
	CString	m_BS_name;
	CString	m_YK_name;
	CString	m_FK_name;
	int		m_value_type;
	int		m_BS_VALUE;
	CString	m_YK_Caption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropYT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropYT)
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnMod();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	tagBS_Array m_BS_Array;
	void Copy_BS_Array(tagBS_Array& Src_BS_Array,tagBS_Array& Dst_BS_Array);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPYT_H__4848EDD4_8D70_4D17_A9F0_AF35C3EF42AF__INCLUDED_)
