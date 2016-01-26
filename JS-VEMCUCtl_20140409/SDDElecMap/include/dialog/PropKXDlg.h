#pragma once
#include "../../resource.h"
#include "afxwin.h"

typedef struct 
{
	char KXID[4];
	char KXCName[13];
}TY_KX;

typedef struct 
{
	char QDID[4];
	char QDCName[13];
}TY_QD;

typedef struct 
{
	char KGEName[17];
	char KGCName[33];
}TY_KG;

// CPropKXDlg 对话框

class CPropKXDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropKXDlg)

public:
	CPropKXDlg();
	virtual ~CPropKXDlg();

// 对话框数据
	enum { IDD = IDD_PROP_KX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
private:
	int m_nCur;
	int m_nCur2;

	char m_biaoshi[4] ;
	char m_kgename[17] ;

	CString m_strKXKG;
	CString m_strDes;

    CArray<TY_KX,TY_KX&> m_KXArr;
	CArray<TY_KG,TY_KG&> m_KGArr;

	CListBox m_ctlList;	
	CComboBox m_ctlCombox;
	CComboBox m_KGComBox;
public:
	void GetKXName(void);
	void GetParam(char* ename, char* cname ,char* kg ,char* des);
	void SetParam(char* ename ,char* kgename ,char* des) ;
	void GetCurSel1(char* ename);
	void GetCurSel2(char* kgename);
	
};
