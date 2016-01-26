#pragma once

#include "../../resource.h"
// CSZ_TimeycDlg 对话框

class CSZ_TimeycDlg : public CDialog
{
	DECLARE_DYNAMIC(CSZ_TimeycDlg)

public:
	CSZ_TimeycDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSZ_TimeycDlg();

// 对话框数据
	enum { IDD = SZ_TIMEYCDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_info;
	char m_cinfo[33];
public:
	virtual BOOL OnInitDialog();
	void SetData(char* newVal);
	void GetData(char* retVal);
	afx_msg void OnBnClickedOk();
};
