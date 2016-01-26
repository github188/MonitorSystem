#pragma once
#include "../../resource.h"
#include "afxcmn.h"


// CPropQkDlg 对话框

class CPropQkDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropQkDlg)

public:
	CPropQkDlg();
	virtual ~CPropQkDlg();

// 对话框数据
	enum { IDD = IDD_PROP_QK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 控制说明
	CString m_strDes;
	CString m_strName;
public:
	virtual BOOL OnInitDialog();
private:
	void InitColum(void);
	CListCtrl m_ctlList;
	void FillList(void);
public:
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	void GetParam(char* ename, char* des);
	void SetParam(char* ename, char* des);
};
