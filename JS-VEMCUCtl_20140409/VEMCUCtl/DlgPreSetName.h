#pragma once


// CDlgPreSetName 对话框

class CDlgPreSetName : public CDialog
{
	DECLARE_DYNAMIC(CDlgPreSetName)

public:
	CDlgPreSetName(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPreSetName();

public:
	CString m_PreSetName;

// 对话框数据
	enum { IDD = IDD_DIALOG_PRESET_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedModifyName();
	afx_msg void OnBnClickedDeleteName();
};
