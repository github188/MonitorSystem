#pragma once

// CDlgViewOut 对话框
#include "Label.h"
#include "ViewChannel.h"
#include "UtilMedia.h"
#include "SkinButton.h"
#include "GWPlayInfoDef.h"
#include "VideoInfoDlg.h"
#include "afxwin.h"
#include "axismediacontrol1.h"


class CDlgViewOut : public CDialog
{
	DECLARE_DYNAMIC(CDlgViewOut)

public:
	CDlgViewOut(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgViewOut();

public:
	int	InitViewChannel();// 初始化实时流播放通道
	void	CloseViewChannel();// 关闭播放通道
	bool	SetDefaultViewParameter();// 设置默认视频参数
	bool	SetCurrentViewParameter(int liangdu,int duibidu, int baohedu, int sedu);// 设置当前视频参数

	//////////////////////////////////////////////////////////////////////////
	void ClearViewChannelInfo();//清空通道信息

public:
	// 回放相关函数
	BOOL	ReplayOpenFileByVideoPlatform(char *szFileName);
	BOOL	ReplayStartFileByVideoPlatform();
	BOOL	ReplayStopFileByVideoPlatform();

	BOOL   ReplayStartStreamByVideoPlatform(int nDecodeType,char *pSps,int nSpsLen);
	BOOL   ReplayStopStreamByVideoPlatform();

	BOOL	ReplayPauseByVideoPlatform();
	BOOL   ReplayResumeByVideoPlatform();
	BOOL	ReplayFastByVideoPlatform();
	BOOL	ReplaySlowByVideoPlatform();
	float  	ReplayGetFilePosByVideoPlatform();
	BOOL   ReplaySetFilePosByVideoPlatform(float fPos);
	int        ReplayGetFileDecodeTypeByVideoPlatform(char *szFileName);

	//////////////////////////////////////////////////////////////////////////
	BOOL	ReplayOpenFileByDirectDevice(char *szFileName);
	BOOL	ReplayStartFileByDirectDevice();
	BOOL	ReplayStopFileByDirectDevice();

	BOOL	ReplayPauseByDirectDevice();
	BOOL   ReplayResumeByDirectDevice();
	BOOL	ReplayFastByDirectDevice();
	BOOL	ReplaySlowByDirectDevice();
	BOOL   ReplaySetFilePosByDirectDevice(int nPos);
	int        ReplayGetFileDvrTypeByDirectDevice(char *szFileName);

	//////////////////////////////////////////////////////////////////////////

	BOOL   OnStartRecord();
	BOOL   OnStopRecord();

	BOOL   OnStartRecordByVideoPlatform();
	BOOL   SaveRecordFile(void *pData,int nDataLen);
	BOOL   OnStopRecordByVideoPlatform();

	BOOL   OnStartRecordByDirectDevice();
	BOOL   OnStopRecordByDirectDevice();

	void	   FlashChannel();
	BOOL  InitControl();
	BOOL  InitInfo();
	BOOL  LoadSkin();

	BOOL SetLineInfo(int line1_from_x,int line1_from_y,int line1_to_x,int line1_to_y,
		                        int line2_from_x,int line2_from_y,int line2_to_x,int line2_to_y);

	BOOL StartYtLock();
	BOOL StopYtLock();
	BOOL IsYtLock();

	void DrawRect(CDC *pDC,RECT &rect);
	void ModifyControlSizeBySelectFlag(BOOL bSelectedFlag,BOOL bForceFlag=FALSE);

	BOOL RecvVideoFlow(int nVideoFlow);
	BOOL ShowCurrentVideoFlow();
	BOOL HideCurrentVideoFlow();
	BOOL ShowCurrentVideoFlowInfo();

public:
	CLabel		m_strStation;
	CLabel		m_strRelaInfo;
	CLabel     m_strVideoFlow;
	CLabel     m_strWeather;
	CSkinButton m_btnVoice;
	CSkinButton m_btnCapture;
	CSkinButton m_btnRecord;
	CSkinButton m_btnTypeFlag;
	CToolTipCtrl m_TPtool;

	 CBitmap  m_TopBmp;
	 BITMAP    m_TopBm;

	HCURSOR		m_hCursor_up;
	HCURSOR		m_hCursor_down;
	HCURSOR		m_hCursor_left;
	HCURSOR		m_hCursor_right;
	HCURSOR		m_hCursor_up_on;
	HCURSOR		m_hCursor_down_on;
	HCURSOR		m_hCursor_left_on;
	HCURSOR		m_hCursor_right_on;

	CBrush m_brush;

public:
	//显示窗口ID
	int m_iWinID;

	//视频显示窗口
	CViewChannel m_Pic;

	//axis窗口
	CAxismediacontrol1 m_AxisView;

	//摄像头设备信息和状态
	VM_VIDEO_VIEW_OUT_INFO m_VideoViewOutInfo;//当前的信息
	VM_VIDEO_VIEW_OUT_INFO m_PreVideoViewOutInfo;//前一次的信息

public:
	//本地播放文件类型标志,1表示通过平台,2表示直接连接前端
	int       m_nReplayLocalFileTypeFlag;
	
	//本地播放文件名称
	char     m_szReplayLocalFileName[256];

	//直接连接前端回放时的ID号
	LONG  m_nReplayLocalFileIDByDirectDevice;

	//直接连接前端回放时的DVR类型
	int  m_nReplayLocalFileDvrTypeyDirectDevice;


	//录像标志位
	BOOL	m_bRecordFlag;

	//本地录像文件句柄
	HANDLE m_hSaveFile;

	// 私有数据包的头
	char		m_Headerbuf[1024];
	int		m_nHeaderLen;

public:
	//////////////////////////////////////////////////////////////////////////
	//预置位画线，线开始、结束值
	int m_nLine1_from_x;
	int m_nLine1_from_y;
	int m_nLine1_to_x;
	int m_nLine1_to_y;
	int m_nLine2_from_x;
	int m_nLine2_from_y;
	int m_nLine2_to_x;
	int m_nLine2_to_y;

public:
	//////////////////////////////////////////////////////////////////////////
	int		m_CallidVoice;       //语音
	bool		m_bYTControl;       //云台控制
	bool		m_bShowVoice;     //是否显示语音
	bool		m_bShowCapture; //是否显示抓图
	bool		m_bShowRecord;   //是否显示录像

	//////////////////////////////////////////////////////////////////////////
	BOOL        m_bSelectedFlag;//画面选择标志

public:
	//云台锁定
	BOOL   m_bYtLockFlag;
	time_t   m_YtLockTime;

	//浮动云台相关
	bool		m_byt_ctl;
	int		m_iyt_pos;

public:
	CRtUtilH264ReFUA* m_pFUA;                                     //对分片进行组包
	GW_PLAY_CHANNEL_INFO *m_pPlayChannelInfo;      //解码器
	HANDLE m_hCallResultNotifyEvent;

public:

// 对话框数据
	enum { IDD = IDD_DIALOG_VIEWOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnBnClickedButtonOpenvoice();
	afx_msg void OnBnClickedButtonCaptureVo();
	afx_msg void OnBnClickedButtonRecordVo();
	afx_msg void OnDestroy();
	DECLARE_EVENTSINK_MAP()
	void OnMouseDownAxismediacontrol1(long nButton, long nShiftState, long fX, long fY);
	void OnDoubleClickAxismediacontrol1(long nButton, long nShiftState, long fX, long fY);
	void OnErrorAxismediacontrol1(long theErrorCode, LPCTSTR theErrorInfo);
	
};
