#pragma once
#include "afxcmn.h"


// CDlgAlarmStatistics 对话框

class CDlgAlarmStatistics : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlarmStatistics)

public:
	CDlgAlarmStatistics(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAlarmStatistics();

// 对话框数据
	enum { IDD = IDD_DIALOG_ALARM_STATISTICS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void InitAlarmStatisticsList();
	afx_msg void OnBnClickedButtonClose();
	void InsertAlarmStatisticsToList(std::vector<ALARM_STATISTICS> vecAlarmStatistics);
	void InitControl();
	CListCtrl m_lstAlarmStatistics;
};
