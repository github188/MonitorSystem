#pragma once


// CDlgPicFilePath 对话框

class CDlgPicFilePath : public CDialog
{
	DECLARE_DYNAMIC(CDlgPicFilePath)

public:
	CDlgPicFilePath(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPicFilePath();

public:
	CString m_FilePath;

// 对话框数据
	enum { IDD = IDD_DIALOG_PICFILEPATH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedFile();
};
