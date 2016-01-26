#pragma once
#include "afxcmn.h"


// CDlgStationSelect 对话框

class CDlgStationSelect : public CDialog
{
	DECLARE_DYNAMIC(CDlgStationSelect)

public:
	CDlgStationSelect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStationSelect();

// 对话框数据
	enum { IDD = IDD_DIALOG_STATION_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	_T_NODE_INFO m_tStationNode;
public:
	afx_msg void OnBnClickedButtonStationSelect();
	virtual BOOL OnInitDialog();
	int CheckStationCurrentStatus2(int nStationId);
	void SetCameraNodeInfo(HTREEITEM hItem, char* ItemName, char* Itemnum, int StationOrCameraID,int ItemStatus,int Streamless,float ItemLongitude, float ItemLatitude, int ItemDecodeTag, int Itemtype, char* stationname);
	void InitStationSelectTree();
	void SetParentWindow(int nParentWindow);
	void UpdateAlarmReplayDeviceCombox(int nStationId);
	void DeleteCameraTreeList();
	void DeleteCameraChildTreeListInfo(HTREEITEM hItem);
	CTreeCtrl m_treeStationSelect;
	int m_nParentWindow;	//0表示设备台账打开的，1表示告警回放打开的
};
