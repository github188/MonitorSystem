#ifndef _CAMERA_VIDEO_OPERATE_H_
#define _CAMERA_VIDEO_OPERATE_H_

//////////////////////////////////////////////////////////////////////////
//NodeType表示结点类型,1:表示server结点,2:表示preset结点,3:表示带预置位preset结点
BOOL MakeCallCameraByViewType(int nWinID,char *szCameraNum,char* szCameraName,int nViewType,int nDecodeTag,int nCameraStatus,int nStreamless,char* szStationName,int NodeType=0,void *pNodeInfo = NULL,char *szNotes = NULL);// 呼叫摄像头

//判断播放窗口是否有视频打开
int      CheckViewVideoIsOpen(int nWinID);

//判断摄像头视频是否被打开
int      CheckCameraVideoIsOpen(char *szCameraNum,int &nWinID);

//关闭指定窗口的视频
BOOL CloseViewVideoByWinID(int nWinID,int nViewType);

//关闭指定窗口通过南瑞平台打开的视频
BOOL CloseViewPlatformVideoByWinID(int nWinID);

//关闭指定窗口通过直连设备打开的视频
BOOL CloseViewDeviceVideoByWinID(int nWinID);

//清除指定窗口的信息
BOOL ClearViewPlatformAndDeviceInfoByWinID(int nWinID);

//关闭录像
BOOL CloseVideoRecordByWinRecordFlag(int nWinID);

//关闭音频
BOOL CloseVideoAudioByWinRecordFlag(int nWinID);

// 得到一个窗口
int GetViewCameraVideoIdleWin();

// 得到当前选择窗口
int GetViewCameraVideoCurrentWin();

// 通过南瑞平台呼叫摄像头
BOOL OpenViewCameraVideoByVideoPlatform(int nWinID,char* szCameraNum,char* szCameraName,int nDecodeTag,int nCameraStatus,int nStreamless,char* szStationName,int NodeType=0,void *pNodeInfo = NULL,char *szNotes = NULL);

// 直接向前端摄像头呼叫
BOOL OpenViewCameraVideoByDirectDevice(int nWinID,char* szCameraNum,char* szCameraName,int nDvrType,int nCameraStatus,int nStreamless,char* szStationName,int NodeType,void *pNodeInfo,char *szNotes = NULL);

// 更新呼叫显示信息
BOOL UpdateViewCameraVideoShowInfo(int nWinID,char* szCameraNum,char* szCameraName,int nCameraStatus,int nStreamless,char* szStationName,int NodeType,void *pNodeInfo,char *szNotes = NULL);

//在指定的窗口打开并显示气象信息
BOOL StartVideoWinCurrentWeather(int nWinID,char *szStationName);

//关闭指定的窗口的气象信息
BOOL StopVideoWinCurrentWeather(int nWinID);

//显示预置位画线信息
BOOL ShowVideoPresetLineInfoDlg(int nWinID,RECT *pRect,void *pNodeInfo);

//清除预置位画线信息
BOOL ClearVideoPresetLineInfoDlg(int nWinID);

//清除标记和中断画线信息
BOOL ClearVideoTagAndOffLineInfoDlg(int nWinID);

//清除预置位画线信息，清除标记和中断画线信息
BOOL ClearVideoPresetLineAndTagInfoDlg(int nWinID);

//显示标注信息
BOOL ShowVideoTagInfoDlg(int nWinID,int nCameraStatus,RECT *pRect);

// 通过南瑞平台发起新视频呼叫
int AppVideoMakeCallByPlatform(int nWinID);

//根据CallId挂断通过南瑞平台发起的呼叫
void AppHangupCallIdByPlatform(int nCallId);

// 直接向前端设备发起新视频呼叫
int AppVideoMakeCallByDirectDevice(int nWinID);

// 挂断直接向前端设备发起的视频呼叫
int AppHangupCallByDirectDevice(int nID);

//根据字符串类型，得到DVR的数字类型
int AppGetCameraDvrType(char *szType);

//取消断线二次重连
BOOL AppCancelRestartCameraVideo(int nWinID);

//设置断线二次重连
BOOL AppSetRestartCameraVideo(int nWinID);

//测试
void AppTestAndDisplayCallThreadIDInfo(char *szInfo);

//测试启动时间
void AppTestAndDisplayTickCountInfo(char *szInfo);

#endif