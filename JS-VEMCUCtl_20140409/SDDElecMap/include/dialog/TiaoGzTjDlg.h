#pragma once
#include "../../resource.h"
#include "afxwin.h"

typedef struct
{
	char ename[17];
	char cname[33];
}S_SGTJ;

// CTiaoGzTjDlg 对话框

class CTiaoGzTjDlg : public CDialog
{
	DECLARE_DYNAMIC(CTiaoGzTjDlg)

public:
	CTiaoGzTjDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTiaoGzTjDlg();

// 对话框数据
	enum { IDD = IDD_TIAOGZTJDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_type;
	CComboBox m_name;
	BYTE m_Type;
	void SetType(BYTE type);
	CArray<S_SGTJ,S_SGTJ> m_PowerTdTj;//电源统计
	CArray<S_SGTJ,S_SGTJ> m_LineTdTj;//回路统计
	void SetTypeBox();
	void SetNameBox();
	void InitData();
	void InitPower();
	void InitLine();
	CString MainPath;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
	char m_id[33];
};
