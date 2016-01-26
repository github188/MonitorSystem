#pragma once

#include "afxwin.h"
#include "Label.h"
#include "SkinButton.h"

//////////////////////////////////////////////////////////////////////////
#define  VIEW_INFO_STATION_TYPE_ALL      1
#define  VIEW_INFO_STATION_TYPE_500KV  2
#define  VIEW_INFO_STATION_TYPE_220KV  3
#define  VIEW_INFO_STATION_TYPE_110KV  4
#define  VIEW_INFO_STATION_TYPE_35KV    5
#define  VIEW_INFO_STATION_TYPE_LINE     6


// CDlgPageViewInfo 对话框

class CDlgPageViewInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgPageViewInfo)

public:
	CDlgPageViewInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPageViewInfo();

public:
	BOOL LoadSkin();

public:
	BOOL OpenFirstPageVideo(HTREEITEM hTreeStationItem);
	BOOL SetPageViewOnePageMaxCameraNum(int nNum);

	bool Lock();//加锁
	bool Unlock();//解锁
	bool SetPageViewInfo(char *pClientIp,char *pDBServerIp,int nDBServerPort,char *pDBName,char *pDBUserName,char *pDBUserPassword);//成员变量赋值
	bool ConnectMySql();	     // 连接数据库
	bool DisConnectMySql();	// 断开数据库

	int OpenPreStationFirstPage();//返回值，-1为没有找到上一个站点，0为打开下一个站点
	int OpenNextStationFirstPage(); //返回值，-1为没有找到下一个站点，0为打开下一个站点

	void StartAutoVideoPageView();//开始人工自动视频巡视
	void PauseAutoVideoPageView();//暂停人工自动视频巡视
	void ResumeAutoVideoPageView();//继续人工自动视频巡视
	void StopAutoVideoPageView();//停止人工自动视频巡视

	BOOL GetStationInfo(int &nAllStationNum,int &nOffStationNum,int nStationType);
	BOOL GetStationCameraInfo(int nStationId,int &nAllCameraNum,int &nProblemCameraNum,int &nStationStatus);//从数据库中得到实时的站点设备在线信息
	BOOL GetAndShowOnlineRateReportInfo(char *szTypeName);
	int GetStationNodeInfo(char *szTypeName,int *nNodeId,int nMaxNum,int nLineNodeId);
	int GetLineNodeInfo(char *szTypeName,int *nNodeId,int nMaxNum,int nLineNodeId);
	int GetLineNodeId();

	//////////////////////////////////////////////////////////////////////////
	//从历史表中得到站点设备的在线信息
	int Get500kVStationCameraInfoOnlineCameraNumFromHistory();
	int Get220kVStationCameraInfoOnlineCameraNumFromHistory();
	int Get110kVStationCameraInfoOnlineCameraNumFromHistory();
	int Get35kVStationCameraInfoOnlineCameraNumFromHistory();

	//从历史表中得到站点设备的在线信息,与报表导出信息一致
	int Get500kVStationCameraInfoOnlineCameraNumFromViewHistory();
	int Get220kVStationCameraInfoOnlineCameraNumFromViewHistory();
	int Get110kVStationCameraInfoOnlineCameraNumFromViewHistory();
	int Get35kVStationCameraInfoOnlineCameraNumFromViewHistory();
	//////////////////////////////////////////////////////////////////////////

	BOOL CheckPageViewTickCount();

	BOOL StartDeviceOnlineRateReportInfo(char *szTypeName);
	BOOL StopDeviceOnlineRateReportInfo();

	//////////////////////////////////////////////////////////////////////////
	BOOL GetStationVideoReportCameraStreamlessStatus(int nStationId);
	BOOL GetStationVideoReportStatusCameraDailyInfo(int nUserId,int nStationId,int nYear,int nMonth,int nDay);
	BOOL GetStationVideoReportStatusCameraDailyInfo2(int nUserId,int nStationId,int nYear,int nMonth,int nDay);

	int  JoinStationReportCameraStatus(int nTagStatus,int nLineStatus);
	BOOL StationReportCameraIsOfflineStatus(int nCameraStatus,int nStreamless);

	//从当天的历史记录中,得到站点设备的在线信息,与报表导出信息一致
	BOOL StatisticsOneStationVideoDailyInfo(int nStationId,int &nCameraCount,int &nOfflineCameraCount,int &nStationStatus);

public:
	CSkinButton m_btnPrePage;
	CSkinButton m_btnNextPage;
	CSkinButton m_btnExportReport;
	CSkinButton m_btnDeviceInfo;
	CSkinButton m_btnOnlineRateInfo;
	CSkinButton m_btnSetTime;
	CSkinButton m_btnPauseVideo;
	CSkinButton m_btnResumeVideo;
	CSkinButton m_btnStopVideo;

	CToolTipCtrl	m_tipTool;

	CBrush m_brush;

public:
	CLabel m_staticStationInfo;
	CLabel m_staticPageInfo;
	HTREEITEM m_hTreeStationItem;
	HTREEITEM m_hTreeCameraItem;
	int m_nPageType;
	int m_nTotalPageNum;
	int m_nCurrentPageIndex;
	int m_nTotalCameraNum;
	int m_nCurrentCameraIndex;
	DWORD m_dwPageViewTickCount;

public:
	bool m_bPageViewInfoInitFlag;
	CRITICAL_SECTION m_csPageViewInfoLock;
	char  m_szClientIp[64];
	char  m_szDBServerIp[64];
	int     m_nDBServerPort;
	char  m_szDBName[256];
	char  m_szDBUserName[256];
	char  m_szDBUserPassword[64];
	MYSQL *m_pMySql;
	bool m_bOnlineRateReportFlag;
	char m_szOnlineRateReportType[64];

	HANDLE m_hDeviceOnlineRateInfoThread;
	UINT m_uDeviceOnlineRateInfoThreadID;
	

// 对话框数据
	enum { IDD = IDD_DIALOG_PAGE_VIEW_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnPrePage();
	afx_msg void OnBnClickedBtnNextPage();
	afx_msg void OnBnClickedButtonSetTime();
	afx_msg void OnBnClickedButtonPauseVideo();
	afx_msg void OnBnClickedButtonResumeVideo();
	afx_msg void OnBnClickedButtonStopVideo();
	afx_msg void OnBnClickedBtnDeviceInfo();
	afx_msg void OnBnClickedBtnOnlineRateInfo();
	afx_msg void OnBnClickedBtnExportReport();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	LRESULT OnDeviceOnlineRateInfoUpdateMessageHandler(WPARAM wParam, LPARAM lParam);
};

//////////////////////////////////////////////////////////////////////////
extern int g_nPageViewOnePageMaxCameraNum;//巡视一页打开的摄像头数

//////////////////////////////////////////////////////////////////////////
unsigned __stdcall _GetAndShowOnlineRateReportInfoThread(void *arg);//得到并显示在线率统计信息
