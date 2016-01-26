
// VEMCUCtlDlg.h : 头文件
//
#pragma once

#include "DlgMenu.h"				// 菜单窗口
#include "DlgLogin.h"
#include "DlgPageServer.h"
#include "DlgViewOut.h"
#include "DlgRecFilePath.h"
#include "DlgPicFilePath.h"
#include "DlgReplay.h"
#include "DlgHistoryLog.h"
#include "DlgSelectDevice.h"
#include "DlgShowControl.h"
#include "DlgShowPageServer.h"
#include "DlgLinkageServer.h"
#include "DlgShowLinkageServer.h"
#include "DlgPageViewInfo.h"
#include "DlgDeviceOnlineInfo.h"
#include "VideoTagInfoDlg.h"
#include "DlgAlarmInfo.h"
#include "DlgShowAlarm.h"
#include "DlgDeviceLedger.h"
#include "DlgStationSelect.h"
#include "DlgAlarmSubscribe.h"

#include "xappany.h"
#include "GWPlay.h"
#include "TCPServerClient.h"
#include "SystemTray.h"
#include "VMTypeDataDefine.h"
#include "WeatherDataWinShowInfo.h"
#include "VideoExcelReportPipeServer.h"
#include "VideoExcelRecordReportPipeServer.h"
#include "ACSSystemClient.h"
#include "DlgColumn1.h"
#include "DlgColumn2.h"
#include "DlgAlarmPreview.h"

//////////////////////////////////////////////////////////////////////////
#define VEM_WIN_ITEM_TYPE_UNDEFINED 0
#define VEM_WIN_ITEM_TYPE_MULTIVIEW 1
#define VEM_WIN_ITEM_TYPE_REPLAY 2
#define VEM_WIN_ITEM_TYPE_MANAGE 3
#define VEM_WIN_ITEM_TYPE_HISTORY_LOG 4
#define VEM_WIN_ITEM_TYPE_ASSISTANCE 5
#define VEM_WIN_ITEM_TYPE_LINKAGE_SETTING 6
#define VEM_WIN_ITEM_TYPE_DEVICE_LEDGER 7
#define VEM_WIN_ITEM_TYPE_ALARM_PREVIEW 8

//////////////////////////////////////////////////////////////////////////
#define  VEM_CONTROL_BUTTON_MESSAGE_TYPE_DEFAULT 0
#define  VEM_CONTROL_BUTTON_MESSAGE_TYPE_MAIN 1
#define  VEM_CONTROL_BUTTON_MESSAGE_TYPE_MENU 2
#define  VEM_CONTROL_BUTTON_MESSAGE_TYPE_PAGESERVER 3
#define  VEM_CONTROL_BUTTON_MESSAGE_TYPE_LINKAGESERVER 4
#define  VEM_CONTROL_BUTTON_MESSAGE_TYPE_ALARM 5
#define  VEM_CONTROL_BUTTON_MESSAGE_TYPE_LINKAGEEVENT 6

//////////////////////////////////////////////////////////////////////////
#define  MAX_STATION_USER_POWER_NUM  (4*1024)
#define  MAX_STATION_USER_POWER_CAMERA_NUM (2*1024)
#define  MAX_STATION_USER_POWER_YWB_NUM (2*1024)

//////////////////////////////////////////////////////////////////////////
#define  VM_HOT_KEY_PRIOR_PAGE   1188
#define  VM_HOT_KEY_NEXT_PAGE 2288
#define  VM_HOT_KEY_EXPORT_REPORT 3688 

//////////////////////////////////////////////////////////////////////////
#define  AUTOVIDEO_PAGEVIEW_STATUS_INIT 0
#define  AUTOVIDEO_PAGEVIEW_STATUS_RUNNING 1
#define  AUTOVIDEO_PAGEVIEW_STATUS_PAUSE 2

//add by wx 2015.11.16  for elecmap
class CDlgTreeElecMap;
class CDlgCameraAndControl;

// CVEMCUCtlDlg 对话框
class CVEMCUCtlDlg : public CDialog
{
// 构造
public:
	CVEMCUCtlDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CVEMCUCtlDlg();

public:
	BOOL DialogInit();
	LRESULT OnDefaultControlButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnMainControlButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnMenuControlButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnPageServerControlButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnLinkageServerControlButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnAlarmControlButton(WPARAM wParam, LPARAM lParam);
	void CheckRelationLinkageListCount();
	void CheckAndSetYtLockState();
	BOOL CheckMenuVideoTagIsDisplay(int iWinID);
	BOOL SearchProcessByName(char * pProcessName);//在进程表中查找进程
	BOOL  CheckAndUpdateSystemSoftware();//检测并更新软件
	BOOL  VideoPageAutoView(); //人工自动视频巡视
	
public:
	BOOL InitVemAllWeatherInfo();
	BOOL UnInitVemAllWeatherInfo();
	BOOL InitVemWeatherInfo(int nIndex);//0:当前气象,1:历史气象,2:历史告警,3:台风告警,4:当前微气象,5:历史微气象
	BOOL StartVemWeatherInfo(int nIndex);
	BOOL StopVemWeatherInfo(int nIndex);
	BOOL UnInitVemWeatherInfo(int nIndex);

	BOOL VemSendCurrentWeatherInfoRequest(int nIndex,char *szUserKey,int nStationID,int nTimeRange);
	BOOL VemSendHistoryWeatherInfoRequest(int nIndex,char *szUserKey,int nStationID,char *szStartTime,char *szStopTime,char *szSortColumn,int nPageRecords,int nPageIndex);
	BOOL VemSendHistoryWarningInfoRequest(int nIndex,char *szUserKey,int nStationID,char *szStartTime,char *szStopTime,char *szSortColumn,int nPageRecords,int nPageIndex);
	BOOL VemSendTyphoonInfoRequest(int nIndex,char *szUserKey,int nTFID,char *szStartTime,char *szStopTime,char *szSortColumn,int nPageRecords,int nPageIndex);
	BOOL VemSendCurrentMicroWeatherInfoRequest(int nIndex,char *szUserKey,int nComCode,char *szComType,int nTimeRange);
	BOOL VemSendHistoryMicroWeatherInfoRequest(int nIndex,char *szUserKey,int nComCode,char *szComType,char *szStartTime,char *szStopTime,char *szSortColumn,int nPageRecords,int nPageIndex);

	void   ShowVideoWinWeatherCurrentInfoOnTimer();//实时气象定时刷新
	void   ShowVideoWinWeatherCurrentInfoOnTimer2();//实时气象定时刷新-在窗口显示
	BOOL StartVideoWinWeatherCurrentInfo(int nIndex,char *szUserKey,int nStationId,int nTimeRange);
	BOOL StopVideoWinWeatherCurrentInfo(int nIndex);

	void   ShowVideoWinMicroWeatherCurrentInfoOnTimer();//实时微气象定时刷新
	void   ShowVideoWinMicroWeatherCurrentInfoOnTimer2();//实时微气象定时刷新-在窗口显示
	BOOL StartVideoWinMicroWeatherCurrentInfo(int nIndex,char *szUserKey,int nComCode,char *szComType,int nTimeRange);
	BOOL StopVideoWinMicroWeatherCurrentInfo(int nIndex);

	void   ShowVideoWinWeatherCurrentInfoOnTimerByFlag();
	BOOL StopVideoWinWeatherCurrentInfoByFlag(int nIndex);

	BOOL StartVideoWinWeatherHistoryInfo(int nIndex,char *szUserKey,int nStationId,char *szStartTime,char *szStopTime);//历史气象
	BOOL StopVideoWinWeatherHistoryInfo(int nIndex);

	int      GetStationIndexByStationName(char *szStationName);
	int      GetStationIndexByStationCode(char *szStationCode);
	int      GetStationIndexByD5000StationName(char *szD5000StationName);
	int      GetStationNodeInfoByNodeId(int nNodeId);

	void    VemStopCurrentVideo();//关闭当前视频
	void    VemStopAllVideo();//关闭全部视频

	void    CheckShowVideoFlow();//显示视频流量

	BOOL StartSubscribeCameraPresenceInfo();//通过订阅方式得到摄像头状态
	BOOL StartSearchCameraPresenceInfo();//通过轮巡方式得到摄像头状态
	BOOL StopSearchCameraPresenceInfo();//通过轮巡方式得到摄像头状态
	BOOL SearchCameraPresenceInfo(int nStationId,char *szStationNum);//通过轮巡方式得到摄像头状态
	
	BOOL StartViewChannelPrevVideoByIndex(int nIndex);
	BOOL QuickStartViewChannelPrevVideoByIndex(int nIndex);

	bool CreateUserStationViewHistoryTable(char *szUserStationViewHistoryTableName);
	bool UpdateUserStationViewHistoryTable(char *szUserStationViewHistoryTableName);
	bool UpdateStationCameraVideoInfo();

	int   GetDeviceIpAddressType(_T_CAMERA_INFO *pCameraInfo);

public:
	BOOL KillProcessByName(char * pProcessName);//根据进程名关闭进程
	BOOL TerminateProcess(char *szFileName);//关闭进程

	//////////////////////////////////////////////////////////////////////////
	BOOL  CheckAndOpenVideoExcelReport();
	BOOL VideoExcelReportPipeInit();
	BOOL VideoExcelReportPipeUnInit();

	BOOL   VideoExcelReportFreshUserInfo(int nUserId);
	BOOL   VideoExcelReportShowReport();
	BOOL   VideoExcelReportHideReport();
	BOOL   VideoExcelReportExitReport();

	//////////////////////////////////////////////////////////////////////////
	BOOL  CheckAndOpenVideoExcelRecordReport();
	BOOL VideoExcelRecordReportPipeInit();
	BOOL VideoExcelRecordReportPipeUnInit();

	BOOL   VideoExcelRecordReportFreshUserInfo(int nUserId);
	BOOL   VideoExcelRecordReportShowReport();
	BOOL   VideoExcelRecordReportHideReport();
	BOOL   VideoExcelRecordReportExitReport();

public:
	HANDLE m_hVideoExcelReportPipe;
	bool m_bVideoExcelReportPipeInitFlag;

	HANDLE m_hVideoExcelRecordReportPipe;
	bool m_bVideoExcelRecordReportPipeInitFlag;

public:
	void	SetFullScreen(void);// 设置窗口全屏
	void	InitScreen(void);// 加载窗口各个组成部分
	bool GetLocalIpAddress(char *szLocalIpAddress);//自动得到本端的IP地址
	bool TestLinkServerIpAddress(IN_ADDR* pInAddr);
	bool	 ReadIniFile(void);// 读取配置文件
	int	SetFilterToInt(char* strValue);// 设置日志等级为整型
	bool	 ConnectMySql(void);// 连接数据库
	bool	DisConnectMySql(void);// 断开数据库
	bool CreateUserStationViewTable();//创建用户巡视权限表
	bool GetUserStationViewPower();
	BOOL GetUserStationViewPower2();
	bool CheckUserYwbViewPower2(int nYwbId);
	bool CheckUserStationViewPower(HTREEITEM hTreeItem,int nType);
	void	SetForms(int num,BOOL bFlag = FALSE);// 设置多画面
	void	SetLinkageForms(int num);
	void	InitPreviewWnd(int iPreviewNum);// 重新设置窗口数目
	void	DrawFocusChannel(int iChannel, BOOL bDraw);// 设置选中视频区域边框颜色
	void	DrawRect(int iWinID, int nHighLight);// 画矩形视频区域外边框

	BOOL LoadSkin();

	BOOL ModifyVideoInfoDlg(int nIndex,RECT *pRect,	BOOL bShowFlag,
		int line1_from_x,int line1_from_y,int line1_to_x,int line1_to_y,
		int line2_from_x,int line2_from_y,int line2_to_x,int line2_to_y);

	BOOL ModifyVideoTagInfoDlg(int nIndex,RECT *pRect,	BOOL bShowFlag);

	BOOL ShowVideoInfoDlgByFlag(int nIndex);
	BOOL ModifyVideoInfoDlgWithView(int nIndex);

	//查找呼叫
	int   SearchIndexCallByCID(int nCid);

	//6分屏
	BOOL SetVideoPreviewWndSix(RECT &rc,BOOL bFullScreenFlag);

	//隐藏视频显示信息
	BOOL HideVideoInfoDlg(int nIndex);
	BOOL HideAllVideoInfoDlg();

	//清除视频显示信息
	BOOL ClearVideoInfoDlgInfo(int nIndex);

	//线程池
	BOOL InitThreadPool();//0号:联动服务，1号:查询气象，2号:打开视频，3号:响应sip服务，4号:主动关闭视频
	BOOL FreeThreadPool();
	BOOL ThreadPoolDispatchTask(void (*pFun)(void *), void *arg,int argsize,int key=0);

	//TCP服务
	BOOL InitTcpServerClient();
	BOOL UnInitTcpServerClient();

	/******************** 厂家接口部分 ***************************/
	void	PlayClose(int win);                                                        // 关闭播放通道
	void	DefaultViewSetting();                                                   // 设置默认视频参数
	void	SetViewSetting();                                                          // 设置当前视频参数
	void	ExitDlg();                                                                        // 客户端退出处理

	/*************************************国网解码SDK*********************************/
	int    GetGWCode(int *pCode);     //得到已经安装的解码SDK
	bool InitGWSDK();                         //初始化国网解码SDK
	bool UninitGWSDK();                    //反初始化国网解码SDK

	/*************************************轮巡处理*********************************/
	void	TuneCycle();             //轮巡处理主线程
	void	TuneCycleThread();  //轮巡处理
	void	TuneCycleStart();     //轮巡开始
	void	TuneCycleEnd();      //轮巡结束
	void	SetPreSet();             // 设置预置点
	int   MoveToPreSet();     // 移到预置点
	void OpenCurPageVideo(HTREEITEM hTreeCameraItem);//打开当前手动轮巡页视频

	/*************************************联动列表显示处理*********************************/
	void	ShowLargeRelationList();// 显示缩小的联动列表
	void	ShowSmallRelationList();// 显示放大的联动列表

	void DisplayLinkageServerRelationListByFlag(BOOL bFlag);//根据标志显示联动列表
	void DisplayShowLinkageServerByFlag(BOOL bFlag);//根据标志显示联动按钮

	BOOL GetProgramAppPath();//得到本程序所在目录
	BOOL GetFileVersionInfoFromFile(char *szVersion,int nLen);//从配置文件中读取版本信息
	BOOL GetFileVersionInfoFromApp(char *szVersion,int nLen);//得到本程序的版本信息
	BOOL GetProgramAppVersionInfo();//得到程序的版本信息

public:
	/*******************各子窗口定义*************************/
	CDlgMenu		*m_pDlgMenu;
	CDlgPageServer	*m_pDlgPageServer;
	CDlgHistoryLog	*m_pDlgHistoryLog;
	CDlgSelectDevice *m_pDlgSelectDevice;
	CDlgShowControl *m_pDlgShowControl;
	CDlgShowPageServer *m_pDlgShowPageServer;
	CDlgLinkageServer *m_pDlgLinkageServer;
	CDlgShowLinkageServer *m_pDlgShowLinkageServer;
	CDlgPageViewInfo *m_pDlgPageViewInfo;
	CDlgRecFilePath	 *m_pDlgRecFilePath;
	CDlgPicFilePath *m_pDlgPicFilePath;
	CDlgShowAlarm *m_pDlgShowAlarm;//显示告警展示窗口的按钮
	CDlgAlarmInfo *m_pDlgAlarmInfo;
	CDlgDeviceLedger *m_pDlgDeviceLedger;
	CDlgStationSelect *m_pDlgStationSelect;
	CDlgAlarmSubscribe *m_pDlgAlarmSubscribe;
	CDlgColumn1 *m_pDlgColumn1;
	CDlgColumn2 *m_pDlgColumn2;
	CDlgAlarmPreview *m_pDlgAlarmPreview;

	CDlgTreeElecMap* m_pDlgTreeElecMap;
	CDlgCameraAndControl *m_pDlgCameraAndControl;

	//////////////////////////////////////////////////////////////////////////
	int		m_FormsNum;					//当前窗口分屏数目
	int		m_iPreviewNum;				//之前窗口分屏数目
	int		m_iFocuseWindowID;		//焦点窗口
	bool	    m_bShowTempFavorite;	//是否显示收藏夹对话框
	bool  	m_bShowRealTalk;
	int		m_nControlWidth;		// 左侧控制栏宽度
	int       m_nMenuHight;         // 菜单栏高度
	int       m_nLinkageWidth;     //右侧联动窗口宽度
	int       m_nPageViewInfoHeight; //手动轮巡导航高度
	int		m_nAlarmHeight;	//告警展示窗口高度

	//显示区域与视频信息关联
	BOOL		  m_bMultiFullScreen;	//多画面全屏
	BOOL		  m_bFullScreen;			//是否全屏
	BOOL          m_bMenuFlag;            //是否显示标题
	BOOL          m_bLinkageServerFlag; //是否显示联动
	BOOL          m_bPageServerFlag;   //是否显示列表、云镜控制
	BOOL          m_bPageViewInfoFlag; //是否显示手动轮巡导航
	int			   m_nFullScreen;				//当前多画面全屏模式
	int                m_nMenuItemIndex; //当前选择子窗口
	WINDOWPLACEMENT m_OldWndpl;				//暂存的整个窗口全屏前的大小
	WINDOWPLACEMENT m_chWndpl;					//暂存的整个窗口全屏前的大小

	//////////////////////////////////////////////////////////////////////////
	CBitmap   m_ViewOutVideoBGBmp;
	BITMAP    m_ViewOutVideoBGBm;

	//////////////////////////////////////////////////////////////////////////
	int m_nYtCommand;
	char m_szYtCommandName[64];
	
// 对话框数据
	enum { IDD = IDD_VEMCUCTL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	LRESULT OnControlButton(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMenuItemExitVideo();
	afx_msg void OnMenuStopAllVideo();
	afx_msg void OnMenuitemStartRec();
	afx_msg void OnMenuitemStopRec();
	afx_msg void OnMenuitemCapture();
	afx_msg void OnMenuitemMultifullScreen();
	afx_msg void OnMenuitemExitmultifullScreen();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMenuItemShow();
	afx_msg void OnMenuItemExit();
	afx_msg void OnMenuitemTagCamera();
	afx_msg void OnMenuitemCancelcamera();
	LRESULT OnChannelChange(WPARAM wParam, LPARAM lParam);
	LRESULT OnDBLChannel(WPARAM wParam, LPARAM lParam);
	LRESULT OnRightClickChannel(WPARAM wParam, LPARAM lParam);
	LRESULT StartYtControl(WPARAM iCommandID, LPARAM iMove);
	afx_msg LRESULT OnExitUsbVideoMSGHandler(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInitUsbVideoMSGHandler(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSubOK(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSubFail(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	LRESULT OnVideoLinkageManualMessageHandler(WPARAM wParam, LPARAM lParam);
	LRESULT OnVideoLinkageAlarmMessageHandler(WPARAM wParam, LPARAM lParam);
	LRESULT OnVideoLinkageStateMessageHandler(WPARAM wParam, LPARAM lParam);
	LRESULT OnVideoCameraControlNotifyMessageHandler(WPARAM wParam, LPARAM lParam);
	LRESULT OnDeviceVideoErrorReasonNotifyMessageHandler(WPARAM wParam, LPARAM lParam);
	LRESULT OnDeviceVideoTalkNotifyMessageHandler(WPARAM wParam, LPARAM lParam);
	LRESULT OnDeviceVideoCloseMessageHandler(WPARAM wParam, LPARAM lParam);
	LRESULT OnDeviceVideoQuickRestartMessageHandler(WPARAM wParam, LPARAM lParam);
	LRESULT OnDeviceVideoStatusOnlineNotifyMessageHandler(WPARAM wParam, LPARAM lParam);
	LRESULT OnVideoLinkageWeatherWarningMessageHandler(WPARAM wParam, LPARAM lParam);
	LRESULT OnVideoLinkageWeatherForecastMessageHandler(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateUserViewCameraStatusMessageHandler(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	void SetDevPlayVideoSdk();
	BOOL InitAssClient();
	//向数据表video_camera中添加一个anfang_flag字段，用来判断摄像头是否是安防摄像头
	BOOL AddAnFangFlagToVideoCamera();
	void ACSSystemConfig();
	void LinkageSettingConfig();
	void HideAllDialog();
	LRESULT OnElecMapOpenCameraMessageHandler(WPARAM wParam, LPARAM lParam);
};

/****************************************************************/
extern CDlgViewOut    g_DlgVideoView[MAXVIEWCH];
extern CVideoInfoDlg  g_VideoInfoDlg[MAXVIEWCH];
extern CVideoTagInfoDlg g_VideoTagInfoDlg[MAXVIEWCH];

/****************************************************************/
//版本配置文件数据
extern char g_FileDataInfo[2*1024];

//表示当前版本信息,初始化为最初版本
extern char g_szCurrentVersion[2*1024];

//程序路径
extern char g_szAppPath[MAX_PATH];

/****************************************************************/
extern CACSSystemClient *g_pACSClient;
extern CGWPlay g_GWPlay;
extern CTcpServerClient g_TcpServerClient;
extern CVEMCUCtlDlg	*g_pMainDlg;
extern int					g_nWin_Height;
extern int					g_nWin_Width;

//客户端视频连接方式,0表示只用南瑞平台,1表示直接访问前端设备,2表示尝试一次后访问设备,3表示尝试两次后访问设备
extern int  g_nClientVideoLinkType;

//视频巡视线程记数
extern VM_THTEAD_VIDEO_OPERATE_NUMBER_INFO g_ThreadVideoOperateNumberInfo;

//////////////////////////////////////////////////////////////////////////
//气象显示类型
extern int g_nCurrentWeatherDataWinInfoTypeFlag;//0:表示显示微气象,1:表示显示气象

//当前气象
extern HANDLE  g_hWeatherCurrentInfoEvent;
extern int            g_nWeatherCurrentInfoType;
extern time_t      g_WeatherCurrentInfoTime;
extern int            g_nWeatherCurrentInfoIndex;

//历史气象
extern HANDLE  g_hWeatherHistoryInfoEvent;
extern int           g_nWeatherHistoryInfoType ;
extern time_t      g_WeatherHistoryInfoTime;
extern int           g_nWeatherHistoryInfoIndex;

//历史告警
extern HANDLE  g_hWeatherHistoryWarningInfoEvent;
extern int           g_nWeatherHistoryWarningInfoType;
extern time_t      g_WeatherHistoryWarningInfoTime;
extern int           g_nWeatherHistoryWarningInfoIndex;

//台风告警
extern HANDLE  g_hWeatherTyphoonWarningInfoEvent;
extern int           g_nWeatherTyphoonWarningInfoType;
extern time_t      g_WeatherTyphoonWarningInfoTime;
extern int           g_nWeatherTyphoonWarningInfoIndex;

//当前微气象
extern HANDLE  g_hMicroWeatherCurrentInfoEvent;
extern int           g_nMicroWeatherCurrentInfoType;
extern time_t      g_MicroWeatherCurrentInfoTime;
extern int           g_nMicroWeatherCurrentInfoIndex;

//历史微气象
extern HANDLE  g_hMicroWeatherHistoryInfoEvent ;
extern int           g_nMicroWeatherHistoryInfoType;
extern time_t      g_MicroWeatherHistoryInfoTime;
extern int           g_nMicroWeatherHistoryInfoIndex;

//抓图路径
extern char   g_PictureSaveFilepath[256];

//////////////////////////////////////////////////////////////////////////
//用户巡视
extern int  g_nUserStationViewStationCount;//用户要巡视站点数
extern int  g_nUserStationViewStationId[MAX_STATION_USER_POWER_NUM];//用户要巡视站点ID号
extern int  g_nUserStationViewStationVoltageClass[MAX_STATION_USER_POWER_NUM];//用户要巡视站点的电压等级
extern char  g_szUserStationViewStationNum[MAX_STATION_USER_POWER_NUM][64];//用户要巡视站点号码
extern char  g_szUserStationViewStationName[MAX_STATION_USER_POWER_NUM][256];//用户要巡视站点名称
extern int  g_nUserStationViewStationCameraCount[MAX_STATION_USER_POWER_NUM];//用户要巡视站点的摄像头数
extern int  g_nUserStationViewStationNoCheckCameraCount[MAX_STATION_USER_POWER_NUM];//用户巡视站点的未巡视的摄像头数
extern char g_szUserStationViewOneStationCameraNum[MAX_STATION_USER_POWER_CAMERA_NUM][64];//临时变量,做摄像头信息同步使用,用户巡视站点的摄像头号码
extern int  g_nUserStationViewOneStationCameraCount;//临时变量,做摄像头信息同步使用,用户巡视站点的摄像头数量
extern char g_szCurrentStationCameraNum[MAX_STATION_USER_POWER_CAMERA_NUM][64];//临时变量,做摄像头信息同步使用,当前站点的摄像头号码
extern int  g_nCurrentStationCameraStatus[MAX_STATION_USER_POWER_CAMERA_NUM];//临时变量,做摄像头信息同步使用,当前站点的摄像头状态
extern int  g_nCurrentStationCameraCount;//临时变量,做摄像头信息同步使用,当前站点的摄像头数量

//////////////////////////////////////////////////////////////////////////

extern T_CURRENT_WEATHER_DATA_WIN_INFO g_CurrentWeatherDataWinInfo[MAXVIEWCH];
extern T_HISTORY_WEATHER_DATA_WIN_INFO g_HistoryWeatherDataWinInfo[MAXVIEWCH];
extern T_CURRENT_MICRO_WEATHER_DATA_WIN_INFO g_CurrentMicroWeatherDataWinInfo[MAXVIEWCH];
extern int   g_nViewOutVideoTypeFlag;//视频按钮显示类型

//////////////////////////////////////////////////////////////////////////
BOOL SendVideoYTControlInfo(char *szInfo,int nInfoLen,int nFlag=0);
BOOL IsCameraVideoLinkByVideoPlatform(int VideoLinkType,int nCameraType);

//////////////////////////////////////////////////////////////////////////
unsigned __stdcall _CameraStatusSearchThread(void *arg);
unsigned __stdcall _CameraStatusSearchThread2(void *arg);

//////////////////////////////////////////////////////////////////////////
extern int  g_nUserStationViewYwbId[MAX_STATION_USER_POWER_YWB_NUM];//用户要巡视运维班ID号
extern int  g_nUserStationViewYwbCount;//用户要巡视运维班数

/////////////////////////////////////////////////////////////////////////
extern CEvent g_AcsSubscribeAlarmEvent;
extern CEvent g_CommonEvent;
