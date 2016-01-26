#pragma once

#include "afxwin.h"
#include "SkinButton.h"
#include "VMListCtrl.h"

// CDlgHistoryLogSystem 对话框

class CDlgHistoryLogSystem : public CDialog
{
	DECLARE_DYNAMIC(CDlgHistoryLogSystem)

public:
	CDlgHistoryLogSystem(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHistoryLogSystem();

public:
	BOOL LoadSkin();
	BOOL InitControl();
	BOOL SearchAndSetHistoryListInfo(char *szStartTime,char *szStopTime);

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
	CVMListCtrl m_ListCtrl_SystemLog;
	// 报警类型
	CComboBox m_ComboType;
	// 报警等级
	CComboBox m_ComboSubType;

	CSkinButton m_btnSelectNode;
	CSkinButton m_btnQuery;

public:
	CBrush m_Brush;

// 对话框数据
	enum { IDD = IDD_DIALOG_HISTORYLOG_SYSTEM };

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
