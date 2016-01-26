#pragma once
#include "afxwin.h"
#include "../../resource.h"
// CCreateTicketDlg 对话框

class CCreateTicketDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateTicketDlg)

public:
	CCreateTicketDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateTicketDlg();

// 对话框数据
	enum { IDD = IDD_CREATETICKETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	CComboBox m_TemplateBox;
	CComboBox m_ObjectBox;
	CString m_TicketName;
public:
	CString GetTicketName(void);
	BOOL TestTemplateAndObjects(void);
	void CreateTicket(void);
	afx_msg void OnBnClickedOk();
};
