#pragma once

#include "dialog/propkxdlg.h"
#include "afxwin.h"

// CPropKXArea 对话框

class CPropKXArea : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropKXArea)

public:
	CPropKXArea();
	virtual ~CPropKXArea();

// 对话框数据
	enum { IDD = IDD_PROP_KXAREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	int m_nCur;
	int m_nCur2;

	char m_biaoshi[4] ;
	char m_qdename[4] ;

	CString m_strDes;
	CString m_strQD;

	CComboBox m_ctlCombox;
	CComboBox m_QDComBox;
	CListBox m_ctlList;

	CArray<TY_KX,TY_KX&> m_KXArr;
	CArray<TY_QD,TY_QD&> m_QDArr;
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
public:
	void InitData(void);
	void GetCurSel1(char* ename);
	void GetCurSel2(char* qdename);
	void GetParam(char* ename ,char* cname ,char* qdename ,char* qdcname ,char* des);
	void SetParam(char* ename ,char* qdename ,char* des);
	virtual BOOL OnInitDialog();
	
};
