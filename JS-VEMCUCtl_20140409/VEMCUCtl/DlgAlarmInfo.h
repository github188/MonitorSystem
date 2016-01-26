#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SkinButton.h"
#include "BtnST.h"


// CDlgAlarmInfo 对话框
typedef struct _Alarm_Info
{
	int	nVolGrade;	//电压等级
	char szStationName[32];	//变电站名称
	char szDevName[32];	//告警设备名称
	int nAlarmType;	//告警类型
	char szHappenTime[64];	//告警发生时间
	char szDealTime[64];	//告警处理时间
	int nPriority;	//优先级
	BOOL bRelevanceVideo;	//是否关联视频
	int nCurrentStatus;	//新告警，等待处理，已处理
	char szDealPerson;	//处理人员
}ALARM_INFO;

class CDlgAlarmInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlarmInfo)
public:
	BOOL m_bAlarmInfoShow;

public:
	CDlgAlarmInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAlarmInfo();

// 对话框数据
	enum { IDD = IDD_DIALOG_ALARM_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	T_LINKAGE_CAMERA_INFO m_tLinkageCameraInfo[MAX_DEV_RELATION_CAMERA];
	int m_nLinkageCameraInfoCount;
	ACS_ALARM_INFO m_tAlarmInfo;
	_T_NODE_INFO m_pCameraNodeInfo;
	int m_nIndex;
	int m_nSelectedList;
	CBitmap* m_pBackBmp;
private:
	CMenu pPopMenu;	
	CMenu menuMain;
	ASS_LINKAGE_LIST_ITEM m_tLinkageListItem;//联动列表记录项
	T_ASS_LINKAGE_CONFIG m_tAssLinkageConfig;//联动事件配置
	CString m_strLastLinkageEventTime;//上一次发生联动事件的时间
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OnControlButton(UINT nID);	//响应按钮消息
	afx_msg void OnBnClickedBtnShowAlarm();
	int SendLoginRequest();
	int CreateAlarmClient();
	BOOL ReadProtocolTable();
	BOOL SendGetAlarmDataRequest();
	void InitRealAlarmInfoList();
	void InitDealAlarmInfoList();
	void InitLocalDealAlarmInfoList();
	void InitLocalHideAlarmInfoList();
	void InitAllListStyleAndColumn();
	CListCtrl m_lstRealAlarmInfo;
	CListCtrl m_lstDealAlarmInfo;
	CListCtrl m_lstLocalDealAlarmInfo;
	CListCtrl m_lstLocalHideAlarmInfo;
	afx_msg void OnBnClickedButtonRealtimeAlarm();
	afx_msg void OnBnClickedButtonProcessedAlarm();
	afx_msg void OnBnClickedButtonProcessedHistory();
	afx_msg void OnBnClickedButtonHidingAlarm();
	afx_msg void OnBnClickedButtonAlarmSubscribe();
	void AlarmListShowHideControl(int nID);
	void FreeRealAlarmInfoList();
	void GetProcessStatusText(char *szProcessStatusText,int nProcessStatus);
	CString GetProcessStatusText(int nProcessStatus);
	void GetRelationVideoText(char *szRelationVideoText,int nRelationVideo);
	void FreeDealAlarmInfoList();
	void FreeLocalDealAlarmInfoList();
	void FreeLocalHidelAlarmInfoList();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMRClickListRealAlarmInfo(NMHDR *pNMHDR, LRESULT *pResult);
	void GetCameraCodeByDevId(int nDevId, T_LINKAGE_CAMERA_INFO *tCameraCode, int *nCount);
	void PopRightMenu(T_LINKAGE_CAMERA_INFO *tCameraCode, int nCount);
	void GetCameraInfoByCameraCode(_T_NODE_INFO* pNodeInfo,char *szCameraCode);
	afx_msg void OnClickLinkageCamera(UINT nID);
	afx_msg void OnClickProcessAlarm();
	void OpenCamera(char *szCameraCode);
	void ToPreset(T_LINKAGE_CAMERA_INFO *pLinkageCameraInfo);
	bool UpdataDBRecord(ACS_ALARM_INFO *pAlarmInfo);
	void InsertRecordToDealList(ACS_ALARM_INFO *pAlarmInfo);
	void DeleteRecordFromRealList(ACS_ALARM_INFO *pAlarmInfo);
	void InsertRecodeToLocalDealList(ACS_ALARM_INFO *pAlarmInfo);
	void InsertDealListOneRecord(int nIndex,ACS_ALARM_INFO *pAlarmInfo);
	void FreeListOneRecord(int nIndex,CListCtrl *list);
	void InsertLocalDealListOneRecord(int nIndex,ACS_ALARM_INFO *pAlarmInfo);
	CListCtrl m_lstRealExAlarmInfo;
	afx_msg void OnBnClickedButtonRealtimeExAlarm();
	afx_msg void OnClickHideAlarm();
	afx_msg void OnClickRecoverAlarm();
	afx_msg void OnClickCompleteProcessAlarm();
	afx_msg void OnClickCancelHideAlarm();
	afx_msg void OnClickDeleteLinkageRecord();
	afx_msg void OnClickRemoveAllItems();
	afx_msg void OnClickOpenElecMap();
	afx_msg void OnClickOpenVideo();
	bool InsertHideAlarmInfoToDB(int nDevId,int nUserId);
	void SendHideAlarmMessageToServer(int nUserId,int nDevId,int nAction);
	afx_msg void OnNMRClickListRealExAlarmInfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListDealAlarmInfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListLocalDealAlarmInfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListLocalHideAlarmInfo(NMHDR *pNMHDR, LRESULT *pResult);
	void InsertRecodeToRealList(ACS_ALARM_INFO* pAlarmInfo);
	void InsertRealListOneRecord(int nIndex, ACS_ALARM_INFO* pAlarmInfo);
	bool UpdataRecoverAlarmStatusToDB(int nStatus, ACS_ALARM_INFO* pAlarmInfo);
	void DeleteListOneRecord(int nIndex,CListCtrl *list);
	bool UpdataCompleteProcessRecordToDB(ACS_ALARM_INFO *pAlarmInfo);
	void ProcessPushAlarmData(char* pPushAlarm, int nCount);
	void ConvertPushAlarmToAlarmInfo(PUSH_ALARM_RESPONSE *pPushAlarm, ACS_ALARM_INFO *pAlarmInfo);
	void InsertRealExListOneRecord(int nIndex, ACS_ALARM_INFO* pAlarmInfo);
	afx_msg void OnBnClickedButtonLinkage();
	afx_msg void OnBnClickedButtonLinkageConfig();
	void InsertLinkageActionListOneRecord(int nIndex, ASS_LINKAGE_LIST_ITEM *pAssLinkageRealtimeInfo);
	void ProcessLinkageRealtimeInfo(char* pLinkageRealtimeInfo, int nCount);
	void ConvertLinkageRealtimeInfoToLinkageListItem(LINKAGE_REALTIME_INFO_RESPONSE *pLinkageRealtimeInfo, ASS_LINKAGE_LIST_ITEM *pLinkageListItem);
	CListCtrl m_lstLinkageAction;
	afx_msg void OnNMRClickListLinkageAction(NMHDR *pNMHDR, LRESULT *pResult);
	void FreeLinkageActionListOneRecord(int nIndex);
	void FreeLinkageActionList();
	void FreeRealExAlarmInfoList();
	void AutoProcessLinkageAction(ASS_LINKAGE_LIST_ITEM *pLinkageRealtimeInfo);
	void SaveCurrentLinkageTime(char *szCurrentTime);
	void ShowElecMapOnMainDlgAndChangePosition(ASS_LINKAGE_LIST_ITEM *pLinkageRealtimeInfo);
	CButtonST m_btnRealTime;
	CButtonST m_unDealAlarm;
	CButtonST m_btnHaveDealAlarm;
	CButtonST m_btnLocalDealAlarm;
	CButtonST m_btnHideAlarm;
	CButtonST m_btnLinkageInfo;
	CButtonST m_btnLinkageConfig;
	CButtonST m_btnSubscribeAlarm;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CButtonST m_btnShowAlarm;
	afx_msg void OnBnClickedButtonAlarmStatistics();
	CButtonST m_btnAlarmStatistics;
};