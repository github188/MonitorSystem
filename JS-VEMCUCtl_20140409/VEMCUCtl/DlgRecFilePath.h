#pragma once

#include "DirDialog.h"

// CDlgRecFilePath 对话框
class CDlgRecFilePath : public CDialog
{
	DECLARE_DYNAMIC(CDlgRecFilePath)

public:
	CDlgRecFilePath(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRecFilePath();

public:
	CString	m_Filepath;

// 对话框数据
	enum { IDD = IDD_DIALOG_RECFILEPATH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnFile();
};
