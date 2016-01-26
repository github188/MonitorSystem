#pragma once

#include "../../Resource.h"

// CPowerNameDlg 对话框

class CPowerNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CPowerNameDlg)

public:
	CPowerNameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPowerNameDlg();

// 对话框数据
	enum { IDD = IDD_POWERNAMEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Name;
	void SetName(char *name,char *id);
	void GetName(char *name,char *id);
	CString m_strID;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
};
