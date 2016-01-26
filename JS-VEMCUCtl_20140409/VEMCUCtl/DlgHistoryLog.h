#pragma once

#include "SkinButton.h"
#include "DlgHistoryLogSystem.h"
#include "DlgHistoryLogUser.h"
#include "DlgHistoryLogRelation.h"
#include "DlgHistoryLogVideoError.h"
#include "DlgHistoryLogAlarm.h"
#include "DlgHLDevice.h"
#include "afxwin.h"


// CDlgHistoryLog 对话框
class CDlgHistoryLog : public CDialog
{
	DECLARE_DYNAMIC(CDlgHistoryLog)

public:
	CDlgHistoryLog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHistoryLog();

// 对话框数据
	enum { IDD = IDD_DIALOG_HISTORYLOG };

public:
	CDlgHistoryLogRelation m_DlgHlRelation;//视频联动日志
	CDlgHistoryLogSystem   m_DlgHlSystem;//系统运行日志
	CDlgHistoryLogUser       m_DlgHlUser;//用户操作日志
	CDlgHistoryLogVideoError m_DlgHlVideoError;//视频检测日志
	CDlgHistoryLogAlarm m_DlgHlAlarm;//告警日志
	CDlgHLDevice m_dlgHlDevice;//前端历史告警日志

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CSkinButton	m_btnSelSystem;
	CSkinButton   m_btnSelOperate;
	CSkinButton	m_btnSelRelation;
	CSkinButton   m_btnVideoError;
	CBrush     m_Brush;
	CBitmap   m_UpBmp;
	BITMAP    m_UpBm;

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonHlSystem();
	afx_msg void OnBnClickedButtonHlOperate();
	afx_msg void OnBnClickedButtonHlRelation();
	afx_msg void OnBnClickedButtonHlVideoError();
	afx_msg void OnBnClickedButtonAlarmHistory();
	afx_msg void OnBnClickedButtonHlDevice();
	void SelectBtnControl(CSkinButton* btn);
	void ShowDialog(int nID);
	CSkinButton m_btnAlarmHistory;
	CSkinButton m_btnDeviceData;
};
