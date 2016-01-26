
#if !defined(AFX_PROPYK_H__27537D25_5B1E_43E1_949E_DDE3B96C456C__INCLUDED_)
#define AFX_PROPYK_H__27537D25_5B1E_43E1_949E_DDE3B96C456C__INCLUDED_

#include "../../resource.h"
#include "canvas/SysLinkTy.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PropYK.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropYK dialog

class CPropYK : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropYK)

// Construction
public:
	CPropYK();
	~CPropYK();
private:
	void Copy_BS_Array(tagBS_Array& Src_BS_Array,tagBS_Array& Dst_BS_Array);
	void Update_BS_UI();
	tagBS_Array m_BS_Array;			//闭锁条件集合

public:

	void SetYkParam(char text[],char Ykname[],BOOL bZK,char Yxname_Hang[],char Yxname_FX_Close[],
								  char Yxname_FX_Open[],char Yxname_Yk[],tagBS_Array& BS_Array);
	void GetYkParam(char text[],char Ykname[],BOOL& bZK,char Yxname_Hang[],
								  char Yxname_FX_Close[],char Yxname_FX_Open[],char Yxname_Yk[],tagBS_Array& BS_Array);

// Dialog Data
	//{{AFX_DATA(CPropYK)
	enum { IDD = IDD_YK };
	CButton	m_modify;
	CButton	m_del;
	CButton	m_add;
	CListBox	m_list_bs;
	int		m_BS_Value;
	CString	m_BS_name;
	CString	m_YK_caption;
	CString	m_YK_name;
	CString	m_ZG_name;
	int		m_ZK_value;
	CString	m_FK_name;
	CString	m_FX_open_name;
	CString	m_FX_close_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropYK)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropYK)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonMod();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList3();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton9();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPYK_H__27537D25_5B1E_43E1_949E_DDE3B96C456C__INCLUDED_)
