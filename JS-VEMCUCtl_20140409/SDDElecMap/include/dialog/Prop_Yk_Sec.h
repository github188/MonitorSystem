#pragma once
#include "../../resource.h"
#include "afxcmn.h"

// CProp_Yk_Sec 对话框

class CProp_Yk_Sec : public CPropertyPage
{
	DECLARE_DYNAMIC(CProp_Yk_Sec)

public:
	CProp_Yk_Sec();
	virtual ~CProp_Yk_Sec();

// 对话框数据
	enum { IDD = IDD_YK_SEC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwFlag;			//控制组选择标记
	BOOL m_bWatchMan;
	BOOL m_bGroup;
	CListCtrl m_CtrlGroup;
	virtual BOOL OnInitDialog();
private:
	// 初始化控制组
	void Init_Ctrl_Group(void);
	void Save_Group_Flag(void);
public:
	void SetData(bool bWatchMan, bool bGroup, DWORD dwFlag);
	void GetData(bool& bWatchMan, bool& bGroup, DWORD& dwFlag);
};
