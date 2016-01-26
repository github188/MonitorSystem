#pragma once
#include "vmlistctrl.h"
#include "afxwin.h"
#include "skinbutton.h"
#include "VMListCtrl.h"


// CDlgHLDevice 对话框

class CDlgHLDevice : public CDialog
{
	DECLARE_DYNAMIC(CDlgHLDevice)

public:
	CDlgHLDevice(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHLDevice();

// 对话框数据
	enum { IDD = IDD_DIALOG_HISTORYLOG_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	BOOL LoadSkin();
	BOOL InitControl();
	BOOL InitControlList();
	CVMListCtrl m_lstHlDevice;
	CComboBox m_comboDevice;
	CTime m_StartDay;
	CTime m_StartTime;
	CTime m_StopDay;
	CTime m_StopTime;
	CSkinButton m_btnSelectStation;
	CSkinButton m_btnQuery;
	afx_msg void OnBnClickedButtonSelectStation();
	void UpdateAlarmReplayDeviceCombox(int nStationId);
public:
	_T_NODE_INFO m_tStationNode;//选择的变电站
	afx_msg void OnBnClickedButtonQuery();
	void SearchDeviceHistoryData();
	void SearchAndAddToListControl(int nStationId, char* strDevName, char* szYear,char* szStartTime,char* szEndTime);
};
