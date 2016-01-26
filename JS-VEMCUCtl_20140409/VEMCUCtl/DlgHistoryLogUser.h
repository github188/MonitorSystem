#pragma once

#include "afxwin.h"
#include "SkinButton.h"
#include "VMListCtrl.h"

// CDlgHistoryLogUser 对话框

class CDlgHistoryLogUser : public CDialog
{
	DECLARE_DYNAMIC(CDlgHistoryLogUser)

public:
	CDlgHistoryLogUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHistoryLogUser();

public:
	BOOL LoadSkin();
	BOOL InitControl();
	BOOL InitControlList();
	BOOL InitControlList2();
	BOOL InitControlList3();
	BOOL SearchAndSetHistoryListInfo(char *szStartTime,char *szStopTime,char *szSelectUserName);
	BOOL SearchAndSetHistoryListInfo2(char *szStartTime,char *szStopTime,char *szSelectUserName);
	BOOL SearchAndSetHistoryListInfo3(char *szStartTime,char *szStopTime,char *szSelectUserName);

public:
	// 选择节点的编码
	CString m_strNodeName;
	// 查询条件，开始时间
	CTime m_StartDay;
	// 查询条件，开始时间
	CTime m_StartTime;
	// 查询条件，结束日期
	CTime m_StopDay;
	// 查询条件，结束时间
	CTime m_StopTime;
	// 查询结果文件列表
	CVMListCtrl m_ListCtrl_UserLog;
	// 报警类型
	CComboBox m_ComboType;
	// 报警等级
	CComboBox m_ComboSubType;

	CSkinButton m_btnSelectNode;
	CSkinButton m_btnQuery;

public:
	CBrush m_Brush;

// 对话框数据
	enum { IDD = IDD_DIALOG_HISTORYLOG_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonHlQuery();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDtnDatetimechangeDatetimepickerHlStartday(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeDatetimepickerHlStarttime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeDatetimepickerHlStopday(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeDatetimepickerHlStoptime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonHlChoosenode();
};
