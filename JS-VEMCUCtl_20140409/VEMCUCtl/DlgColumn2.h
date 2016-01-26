#pragma once


// CDlgColumn2 对话框

class CDlgColumn2 : public CDialog
{
	DECLARE_DYNAMIC(CDlgColumn2)

public:
	CDlgColumn2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgColumn2();

// 对话框数据
	enum { IDD = IDD_DIALOG_COLUMN2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
