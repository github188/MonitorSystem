#pragma once
#include "../../resource.h"
#include "canvas/SysLinkTy.h"
#include <afxwin.h>

// CPropYT2 ¶Ô»°¿ò

class CPropYT2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropYT2)

	// Construction
public:
	void Update_BS_UI();
	void SetYTParam(char text[],char Ykname[],int bVT,char Yxname_FK[],tagBS_Array& BS_Array, float fPianCha, int nXiaoShu, int nBiLi);
	void GetYTParam(char text[],char Ykname[],int& bVT,char Yxname_FK[],tagBS_Array& BS_Array, float& fPianCha, int& nXiaoShu, int& nBiLi);
	CPropYT2();
	~CPropYT2();

	// Dialog Data
	//{{AFX_DATA(CPropYT2)
	enum { IDD = IDD_YT2 };
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
	//{{AFX_VIRTUAL(CPropYT2)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropYT2)
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnMod();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnBnClickedvaluetype();
	afx_msg void OnBnClickedRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	tagBS_Array m_BS_Array;
	void Copy_BS_Array(tagBS_Array& Src_BS_Array,tagBS_Array& Dst_BS_Array);
public:
	//float m_fPianCha;
	//int m_nXiaoShu;
	//int m_nBiLi;
	CEdit m_editPianCha;
	CEdit m_editXiaoShu;
	CEdit m_editBiLi;
	CString m_sPianCha;
	CString m_sXiaoShu;
	CString m_sBiLi;
	virtual BOOL OnApply();
};
