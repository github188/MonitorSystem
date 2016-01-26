#pragma once

#include "../../resource.h"
// CPropModemDlg 对话框

class CPropModemDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropModemDlg)

public:
	CPropModemDlg();
	virtual ~CPropModemDlg();

// 对话框数据
	enum { IDD = IDD_LINK_MODEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 适配器id
	DWORD m_adptID;
	CString m_strPhone;
	// 拨号时间，秒
	DWORD m_dwtime;
public:
	void SetParam(char* descript ,DWORD adptid ,char *phone ,DWORD time) ;
	void GetParam(char* descript ,DWORD &adptid ,char *phone ,DWORD &time) ;
	virtual BOOL OnInitDialog();
private:
	// 控制说明
	CString m_strDescript;
};
