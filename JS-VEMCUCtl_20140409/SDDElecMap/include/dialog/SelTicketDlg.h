#pragma once

#include <afxwin.h>
#include "../../resource.h"
// CSelTicketDlg 对话框

class CSelTicketDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelTicketDlg)

public:
	CSelTicketDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelTicketDlg();

// 对话框数据
	enum { IDD = IDD_SELTICKETDLG };

protected:
	char m_TicketName[33];
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetTicketName(char* TicketName);
	char* GetTicketName(void);
	virtual BOOL OnInitDialog();
private:
	CComboBox m_TicketNameBox;
public:
	afx_msg void OnBnClickedCreatebutton();
	afx_msg void OnBnClickedOk();

};
