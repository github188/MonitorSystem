#pragma once


// CDlgColumn1 对话框

class CDlgColumn1 : public CDialog
{
	DECLARE_DYNAMIC(CDlgColumn1)

public:
	CDlgColumn1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgColumn1();

// 对话框数据
	enum { IDD = IDD_DIALOG_COLUMN1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
