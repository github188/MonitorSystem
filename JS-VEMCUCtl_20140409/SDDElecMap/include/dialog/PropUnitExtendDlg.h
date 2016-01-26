#pragma once

#include "../../resource.h"
// CPropUnitExtendDlg 对话框

class CPropUnitExtendDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropUnitExtendDlg)

public:
	CPropUnitExtendDlg();
	virtual ~CPropUnitExtendDlg();

// 对话框数据
	enum { IDD = IDD_UNIT_EXTEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_strDes;
	CString m_strEname;
	// 召唤事故报文
	BOOL m_bUnExt_SGBW;
public:
	afx_msg void OnBnClickedButton1();
public:
	void GetUnitExtParam(char* des ,char* ename ,bool& bCallSGBW ,bool& bEventSGBW ,bool& bCallMC,bool& bRefresh,bool& bEpd) ;
	void SetUnitExtParam(char* des ,char* ename ,bool bCallSGBW ,bool bEventSGBW ,bool bCallMC,bool bRefresh,bool bEpd) ;
private:
	// 召唤事故报文-事件型
	BOOL m_bUnExt_SGBW_Event;
	// 召唤毛刺曲线
	BOOL m_bUnExt_MC;
	// 发送画面刷新信息
	BOOL m_bRefresh;
public:
	// 是否需要epd查询
	BOOL m_bEpd;
};
