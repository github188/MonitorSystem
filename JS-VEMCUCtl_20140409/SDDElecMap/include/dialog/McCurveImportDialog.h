#pragma once

#include "afxwin.h"
#include "../../resource.h"
// CMcCurveImportDialog 对话框

class CMcCurveImportDialog : public CDialog
{
	DECLARE_DYNAMIC(CMcCurveImportDialog)

public:
	CMcCurveImportDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMcCurveImportDialog();

// 对话框数据
	enum { IDD = IDD_MCCURVEIMPORTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
public:
	CString strfilepath;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_Sectionlist;
	CString m_strSelsection;
	afx_msg void OnLbnSelchangeList1();
	CString m_strinlist;
	LPCTSTR GetSelSection();
	afx_msg void OnLbnDblclkList1();
};
