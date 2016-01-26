#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "BtnST.h"


// CDlgStationAlarm 对话框

class CDlgStationAlarm : public CDialog
{
	DECLARE_DYNAMIC(CDlgStationAlarm)

public:
	CDlgStationAlarm(CWnd* pParent = NULL);   // 标准构造函数
	CDlgStationAlarm(int nStationId, CWnd* pParent = NULL);
	virtual ~CDlgStationAlarm();

// 对话框数据
	enum { IDD = IDD_DIALOG_STATION_ALARM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void InitControl();
	CString SpellCondition(int nStationId);
	void AddAlarmInfoToList(std::vector<ACS_ALARM_INFO> vecAlarmInfo);
	void InitListData(int nStationId);
private:
	int m_nStationId;
public:
	CListCtrl m_lstAlarm;
	CButton m_checkAll;
	CButtonST m_btnQuery;
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedCheckAnfang();
	afx_msg void OnBnClickedCheckFire();
	afx_msg void OnBnClickedCheckDoor();
	afx_msg void OnClickDeleteSelectedItems();
	CButton m_checkAnFang1;
	CButton m_checkFire1;
	CButton m_checkDoor1;
	afx_msg void OnBnClickedButtonProcessAll();
	CButtonST m_btnProcessAll;
	afx_msg void OnNMRClickListAlarm(NMHDR *pNMHDR, LRESULT *pResult);
	void PopRightMenu();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
