#ifndef _VM_TYPE_DATA_DEFINE_H_
#define _VM_TYPE_DATA_DEFINE_H_

#include <math.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////
//视频联动优先级
#define  VIDEO_LINKAGE_INFO_UNDEFINE_PRIORITY 0
#define  VIDEO_STATE_LINKAGE_PRIORITY 1
#define  VIDEO_MANUAL_LINKAGE_PRIORITY 2
#define  VIDEO_WEATHER_FORECAST_LINKAGE_PRIORITY 3
#define  VIDEO_WEATHER_WARNING_LINKAGE_PRIORITY 4
#define  VIDEO_ALARM_LINKAGE_PRIORITY 5
#define  VIDEO_LINKAGE_MAX_PRIORITY 6

//////////////////////////////////////////////////////////////////////////
//摄像头状态定义
#define  VM_CAMERA_STATUS_NOTAG_OFFLINE 0  //未标注  不在线
#define  VM_CAMERA_STATUS_NOTAG_ONLINE 1   //未标注   在线
#define  VM_CAMERA_STATUS_TAG_ONLINE 2         //标注      在线
#define  VM_CAMERA_STATUS_TAG_OFFLINE 3        //标注      不在线
#define  VM_CAMERA_STATUS_NO_TAG_BY_STATUS   4   //按状态取消标注
#define  VM_CAMERA_STATUS_TAG_BY_STATUS  5           //按状态标注
#define  VM_CAMERA_STATUS_OFFLINE_BY_STATUS   6   //按状态设置不在线
#define  VM_CAMERA_STATUS_ONLINE_BY_STATUS  7     //按状态设置在线
#define  VM_CAMERA_STATUS_MAX_VALUE 8        //状态最大值

//////////////////////////////////////////////////////////////////////////
//ViewOut摄像头呼叫状态定义
#define  VM_VIEW_OUT_CAMERA_CALL_STATUS_IDLE 0               //空闲
#define  VM_VIEW_OUT_CAMERA_CALL_STATUS_CALLING  1       //正在呼叫
#define  VM_VIEW_OUT_CAMERA_CALL_STATUS_TALKING  2       //呼叫
#define  VM_VIEW_OUT_CAMERA_CALL_STATUS_TERMINATE 3   //呼叫结束
#define  VM_VIEW_OUT_CAMERA_CALL_STATUS_MAX_VALUE 4  //状态最大值

//箭头线信息
struct  VMArrowLine
{
public:
	VMArrowLine()
	{
		m_nArrowStartX = 0;
		m_nArrowStartY = 0;
		m_nArrowStopX = 0;
		m_nArrowStopY =0;
		m_bShowFlag = FALSE;
		memset(m_szInfo,0,sizeof(m_szInfo));
	}

	~VMArrowLine()
	{
		m_nArrowStartX = 0;
		m_nArrowStartY = 0;
		m_nArrowStopX = 0;
		m_nArrowStopY =0;
		m_bShowFlag = FALSE;
		memset(m_szInfo,0,sizeof(m_szInfo));
	}

	BOOL ShowArrowLine(BOOL bFlag)
	{
		m_bShowFlag = bFlag;
		return TRUE;
	}

	BOOL SetArrowInfo(char *pInfo)
	{
		if (pInfo != NULL)
		{
			strncpy(m_szInfo,pInfo,sizeof(m_szInfo)-1);
			m_szInfo[sizeof(m_szInfo)-1] = 0;
			return TRUE;
		}
		return FALSE;
	}

	int GetLineLength()
	{
		int nLength = 0;
		int nX = 0;
		int nY =0;
		double d = 0.0;

		if (m_nArrowStartX == m_nArrowStopX&&m_nArrowStartY == m_nArrowStopY)
			return 0;

		nX = m_nArrowStopX - m_nArrowStartX;
		nY = m_nArrowStopY - m_nArrowStartY;

		d = nX*nX + nY*nY;

		nLength = sqrt(d);

		return nLength;
	}

	BOOL ClearLineInfo()
	{
		m_nArrowStartX = 0;
		m_nArrowStartY = 0;
		m_nArrowStopX = 0;
		m_nArrowStopY =0;
		m_bShowFlag = FALSE;
		memset(m_szInfo,0,sizeof(m_szInfo));
		return TRUE;
	}

public:
	int m_nArrowStartX;
	int m_nArrowStartY;
	int m_nArrowStopX;
	int m_nArrowStopY;
	BOOL m_bShowFlag;
	char   m_szInfo[64];
};

struct VM_VIDEO_LINKAGE_PRIORITY_INFO
{
public:
	VM_VIDEO_LINKAGE_PRIORITY_INFO()
	{
		InitializeCriticalSection(&m_cs);
		EnterCriticalSection(&m_cs);
		m_nVideoLinkageType = VIDEO_LINKAGE_INFO_UNDEFINE_TYPE;
		m_nVideoLinkagePriority = VIDEO_LINKAGE_INFO_UNDEFINE_PRIORITY;
		m_VideoLinkagetime = time(NULL);
		LeaveCriticalSection(&m_cs);
	}

	~VM_VIDEO_LINKAGE_PRIORITY_INFO()
	{
		EnterCriticalSection(&m_cs);
		m_nVideoLinkageType = VIDEO_LINKAGE_INFO_UNDEFINE_TYPE;
		m_nVideoLinkagePriority = VIDEO_LINKAGE_INFO_UNDEFINE_PRIORITY;
		m_VideoLinkagetime = time(NULL);
		LeaveCriticalSection(&m_cs);
		DeleteCriticalSection(&m_cs);
	}

	BOOL DecideAndSetVideoLinkagePriorityInfo(int nType,int nPriority,time_t timeLink)
	{
		BOOL bResult = FALSE;
		EnterCriticalSection(&m_cs);
		if (nPriority > m_nVideoLinkagePriority||timeLink - m_VideoLinkagetime > 1||timeLink - m_VideoLinkagetime < -1)
		{
			m_nVideoLinkageType = nType;
			m_nVideoLinkagePriority = nPriority;
			m_VideoLinkagetime = timeLink;
			bResult = TRUE;
		}
		LeaveCriticalSection(&m_cs);
		return bResult;
	}

	BOOL SetVideoLinkagePriorityInfo(int nType,int nPriority,time_t timeLink)
	{
		EnterCriticalSection(&m_cs);
		m_nVideoLinkageType = nType;
		m_nVideoLinkagePriority = nPriority;
		m_VideoLinkagetime = timeLink;
		LeaveCriticalSection(&m_cs);
		return TRUE;
	}

	BOOL GetVideoLinkagePriorityInfo(int &nType,int &nPriority,time_t &timeLink)
	{
		EnterCriticalSection(&m_cs);
		nType = m_nVideoLinkageType;
		nPriority = m_nVideoLinkagePriority;
		timeLink = m_VideoLinkagetime;
		LeaveCriticalSection(&m_cs);
		return TRUE;
	}

	BOOL Lock()
	{
		EnterCriticalSection(&m_cs);
		return TRUE;
	}

	BOOL Unlock()
	{
		LeaveCriticalSection(&m_cs);
		return TRUE;
	}

public:
	CRITICAL_SECTION m_cs;
	int m_nVideoLinkageType;
	int m_nVideoLinkagePriority;
	time_t m_VideoLinkagetime;
};


struct VM_THTEAD_VIDEO_OPERATE_NUMBER_INFO
{
public:
	VM_THTEAD_VIDEO_OPERATE_NUMBER_INFO()
	{
		InitializeCriticalSection(&m_cs);
		EnterCriticalSection(&m_cs);
		m_nDeviceVideoInNumber = 0;
		m_nDeviceVideoOutNumber = 0;
		LeaveCriticalSection(&m_cs);
	}

	~VM_THTEAD_VIDEO_OPERATE_NUMBER_INFO()
	{
		EnterCriticalSection(&m_cs);
		m_nDeviceVideoInNumber = 0;
		m_nDeviceVideoOutNumber = 0;
		LeaveCriticalSection(&m_cs);
		DeleteCriticalSection(&m_cs);
	}

	BOOL Lock()
	{
		EnterCriticalSection(&m_cs);
		return TRUE;
	}

	BOOL Unlock()
	{
		LeaveCriticalSection(&m_cs);
		return TRUE;
	}

	void DeviceVideoInNumerAdd()
	{
		EnterCriticalSection(&m_cs);
		
		if (m_nDeviceVideoInNumber < 0)
		{
			m_nDeviceVideoInNumber = 0;
		}

		m_nDeviceVideoInNumber++;

		LeaveCriticalSection(&m_cs);
	}

	void DeviceVideoOutNumerAdd()
	{
		EnterCriticalSection(&m_cs);

		if (m_nDeviceVideoOutNumber < 0)
		{
			m_nDeviceVideoOutNumber = 0;
		}

		m_nDeviceVideoOutNumber++;

		LeaveCriticalSection(&m_cs);
	}

	int GetThreadDeviceVideoNumer()
	{
		int nNumber = 0;

		EnterCriticalSection(&m_cs);
		
		if (m_nDeviceVideoInNumber < m_nDeviceVideoOutNumber)
		{
			m_nDeviceVideoInNumber = m_nDeviceVideoOutNumber;
		}

		nNumber = m_nDeviceVideoInNumber-m_nDeviceVideoOutNumber;

		LeaveCriticalSection(&m_cs);

		return nNumber;
	}

public:
	CRITICAL_SECTION m_cs;
	int m_nDeviceVideoInNumber;
	int m_nDeviceVideoOutNumber;
};


struct VM_VIDEO_VIEW_OUT_INFO
{
public:
	CRITICAL_SECTION m_cs;
	int m_nVideoViewType; //呼叫类型，0表示未定义，1表示通过南瑞平台呼叫，2表示直接连接前端设备
	int m_nCameraStatus;   //摄像头状态，在线，不在线，标注
	int m_nCameraStreamless;//摄像头流量，0表示有流量，1表示没有流量
	int m_nCameraCallStatus; //状态，0为空闲,1为正在呼叫,2表示正在通话,3表示结束
	char m_szCameraName[CAMERANAMELEN];//摄像头名称
	char m_szStationName[256];//站点信息
	char m_szNotes[256];//备注信息
	
public:
	//SIP呼叫信息
	int m_nCallid;    //呼叫id
	int m_nDecodeTag;//解码标签
	char m_szCameraCallNum[CAMERACALLNUMLEN];//摄像头呼叫号码
	
public:
	//摄像头DVR信息
	int      m_nVideoSdkId;                      //直连ID
	int      m_nDvrType;                           //DVR类型
	int      m_nDvrId;                               //DVR序号
	char   m_szDvrNum[DVRNUMLEN];   //DVR号码
	char   m_szDvrIP[DVRIPLEN];             //DVR的IP地址
	int      m_nDvrPort;                           //DVR的端口号
	int      m_nCameraChannel;              //摄像头通道号
	char   m_szDvrUserName[DVRUSERNAMELEN]; //DVR登陆用户名
	char   m_szDvrPassword[DVRPASSWORDLEN];   //DVR登陆密码

public:
	//流量
	int             m_nVideoFlow;
	int             m_nRealVideoFlow;
	DWORD    m_dwVideoFlowTime;
	int             m_nVideoFlowCount;//流量检测计数
	BOOL        m_bVideoFlowProblemFlag;//流量检测标志

public:
	int      m_nDeviceVideoFlag;//SDK状态标志
	BOOL m_bSetNrDecodeProblemFlag;//南瑞解码器初始化出错标志
	int      m_nNrDecodeProblemRestartCount;//南瑞解码器初始化出错重试次数
	int      m_nSetProblemCameraFlag;//出现问题标志
	int      m_nProblemRestartCount;//当出现问题时，重新打开次数

public:
	char	   m_preset_name[64];
	int	   m_preset_id;
	int       m_line1_from_x;
	int       m_line1_from_y;
	int       m_line1_to_x;
	int       m_line1_to_y;
	int       m_line2_from_x;
	int       m_line2_from_y;
	int       m_line2_to_x;
	int       m_line2_to_y;

public:
	VM_VIDEO_VIEW_OUT_INFO()
	{
		InitializeCriticalSection(&m_cs);
		Clear();
	}

	~VM_VIDEO_VIEW_OUT_INFO()
	{
		Clear();
		DeleteCriticalSection(&m_cs);
	}

	BOOL Lock()
	{
		EnterCriticalSection(&m_cs);
		return TRUE;
	}

	BOOL UnLock()
	{
		LeaveCriticalSection(&m_cs);
		return TRUE;
	}

	BOOL Copy(VM_VIDEO_VIEW_OUT_INFO &ViewOutInfo)
	{
		BOOL bReturn = FALSE;

		EnterCriticalSection(&m_cs);

		try
		{
			if (m_nVideoViewType > 0 )
			{
				ViewOutInfo.m_nVideoViewType = m_nVideoViewType;
				ViewOutInfo.m_nCameraStatus = m_nCameraStatus;
				ViewOutInfo.m_nCameraCallStatus = m_nCameraCallStatus;
				ViewOutInfo.m_nCameraStreamless = m_nCameraStreamless;
				ViewOutInfo.m_nCallid = m_nCallid;
				ViewOutInfo.m_nDecodeTag = m_nDecodeTag;
				ViewOutInfo.m_nVideoSdkId = m_nVideoSdkId;
				ViewOutInfo.m_nDvrType = m_nDvrType;
				ViewOutInfo.m_nDvrId = m_nDvrId;
				ViewOutInfo.m_nDvrPort = m_nDvrPort;
				ViewOutInfo.m_nCameraChannel = m_nCameraChannel;
				ViewOutInfo.m_nVideoFlow = m_nVideoFlow;
				ViewOutInfo.m_nRealVideoFlow = m_nRealVideoFlow;
				ViewOutInfo.m_dwVideoFlowTime = m_dwVideoFlowTime;
				ViewOutInfo.m_nVideoFlowCount = m_nVideoFlowCount;
				ViewOutInfo.m_bVideoFlowProblemFlag = m_bVideoFlowProblemFlag;
				ViewOutInfo.m_nDeviceVideoFlag = m_nDeviceVideoFlag;
				ViewOutInfo.m_bSetNrDecodeProblemFlag = m_bSetNrDecodeProblemFlag;
				ViewOutInfo.m_nNrDecodeProblemRestartCount = m_nNrDecodeProblemRestartCount;
				ViewOutInfo.m_nSetProblemCameraFlag = m_nSetProblemCameraFlag;
				ViewOutInfo.m_nProblemRestartCount = m_nProblemRestartCount;

				//预置位
				ViewOutInfo.m_preset_id = m_preset_id;
				ViewOutInfo.m_line1_from_x = m_line1_from_x;
				ViewOutInfo.m_line1_from_y = m_line1_from_y;
				ViewOutInfo.m_line1_to_x = m_line1_to_x;
				ViewOutInfo.m_line1_to_y = m_line1_to_y;
				ViewOutInfo.m_line2_from_x = m_line2_from_x;
				ViewOutInfo.m_line2_from_y = m_line2_from_y;
				ViewOutInfo.m_line2_to_x = m_line2_to_x;
				ViewOutInfo.m_line2_to_y = m_line2_to_y;
				strcpy_s(ViewOutInfo.m_preset_name,sizeof(ViewOutInfo.m_preset_name),m_preset_name);

				strcpy_s(ViewOutInfo.m_szCameraName,sizeof(ViewOutInfo.m_szCameraName),m_szCameraName);
				strcpy_s(ViewOutInfo.m_szStationName,sizeof(ViewOutInfo.m_szStationName),m_szStationName);
				strcpy_s(ViewOutInfo.m_szNotes,sizeof(ViewOutInfo.m_szNotes),m_szNotes);
				strcpy_s(ViewOutInfo.m_szCameraCallNum,sizeof(ViewOutInfo.m_szCameraCallNum),m_szCameraCallNum);
				strcpy_s(ViewOutInfo.m_szDvrNum,sizeof(ViewOutInfo.m_szDvrNum),m_szDvrNum);
				strcpy_s(ViewOutInfo.m_szDvrIP,sizeof(ViewOutInfo.m_szDvrIP),m_szDvrIP);
				strcpy_s(ViewOutInfo.m_szDvrUserName,sizeof(ViewOutInfo.m_szDvrUserName),m_szDvrUserName);
				strcpy_s(ViewOutInfo.m_szDvrPassword,sizeof(ViewOutInfo.m_szDvrPassword),m_szDvrPassword);
				bReturn = TRUE;
			}

			LeaveCriticalSection(&m_cs);
			return bReturn;
		}
		catch(...)
		{

		}
		LeaveCriticalSection(&m_cs);
		return FALSE;
	}

	BOOL Clear()
	{
		EnterCriticalSection(&m_cs);

		try
		{
			m_nVideoViewType = 0;
			m_nCameraStatus = 0;
			m_nCameraCallStatus = VM_VIEW_OUT_CAMERA_CALL_STATUS_IDLE;
			m_nCameraStreamless = 0;
			m_nCallid = -1;
			m_nDecodeTag = 0;
			m_nVideoSdkId = -1;
			m_nDvrType = 0; 
			m_nDvrId = -1;
			m_nDvrPort = 0;
			m_nCameraChannel = -1;
			m_nVideoFlow = 0;
			m_nRealVideoFlow = 0;
			m_dwVideoFlowTime = GetTickCount();
			m_nVideoFlowCount = 0;
			m_bVideoFlowProblemFlag = FALSE;
			m_nDeviceVideoFlag = 0;
			m_bSetNrDecodeProblemFlag = FALSE;
			m_nNrDecodeProblemRestartCount = 2;
			m_nSetProblemCameraFlag = FALSE;
			m_nProblemRestartCount = 1;
			m_preset_id = -1;
			m_line1_from_x = 0;
			m_line1_from_y = 0;
			m_line1_to_x = 0;
			m_line1_to_y = 0;
			m_line2_from_x = 0;
			m_line2_from_y = 0;
			m_line2_to_x = 0;
			m_line2_to_y = 0;
			memset(m_preset_name,0,sizeof(m_preset_name));
			memset(m_szCameraName,0,sizeof(m_szCameraName));
			memset(m_szStationName,0,sizeof(m_szStationName));
			memset(m_szNotes,0,sizeof(m_szNotes));
			memset(m_szCameraCallNum,0,sizeof(m_szCameraCallNum));
			memset(m_szDvrNum,0,sizeof(m_szDvrNum));
			memset(m_szDvrIP,0,sizeof(m_szDvrIP));
			memset(m_szDvrUserName,0,sizeof(m_szDvrUserName));
			memset(m_szDvrPassword,0,sizeof(m_szDvrPassword));
			LeaveCriticalSection(&m_cs);
			return TRUE;
		}
		catch(...)
		{

		}
		LeaveCriticalSection(&m_cs);
		return FALSE;
	}

	BOOL Clear2()
	{
		EnterCriticalSection(&m_cs);

		try
		{
			m_nVideoViewType = 0;
			m_nCameraStatus = 0;
			m_nCameraCallStatus = VM_VIEW_OUT_CAMERA_CALL_STATUS_IDLE;
			m_nCameraStreamless = 0;
			m_nCallid = -1;
			m_nDecodeTag = 0;
			m_nVideoSdkId = -1;
			m_nDvrType = 0; 
			m_nDvrId = -1;
			m_nDvrPort = 0;
			m_nCameraChannel = -1;
			m_nVideoFlow = 0;
			m_nRealVideoFlow = 0;
			m_dwVideoFlowTime = GetTickCount();
			m_nDeviceVideoFlag = 0;
			m_preset_id = -1;
			m_line1_from_x = 0;
			m_line1_from_y = 0;
			m_line1_to_x = 0;
			m_line1_to_y = 0;
			m_line2_from_x = 0;
			m_line2_from_y = 0;
			m_line2_to_x = 0;
			m_line2_to_y = 0;
			memset(m_preset_name,0,sizeof(m_preset_name));
			memset(m_szCameraName,0,sizeof(m_szCameraName));
			memset(m_szStationName,0,sizeof(m_szStationName));
			memset(m_szNotes,0,sizeof(m_szNotes));
			memset(m_szCameraCallNum,0,sizeof(m_szCameraCallNum));
			memset(m_szDvrNum,0,sizeof(m_szDvrNum));
			memset(m_szDvrIP,0,sizeof(m_szDvrIP));
			memset(m_szDvrUserName,0,sizeof(m_szDvrUserName));
			memset(m_szDvrPassword,0,sizeof(m_szDvrPassword));
			LeaveCriticalSection(&m_cs);
			return TRUE;
		}
		catch(...)
		{

		}
		LeaveCriticalSection(&m_cs);
		return FALSE;
	}
};

//////////////////////////////////////////////////////////////////////////
//在线统计---读历史数据--与摄像机报表导出一致
#define EXCEL_STATION_MAX_CAMERA_NUM 256

struct VIDEO_VIEW_REPORT_STATUS_CAMERA_STREAM_INFO
{
	int    m_nCameraId;
	char m_szCameraNum[CAMERACALLNUMLEN];//摄像头号码
	int    m_nStreamless;
};

struct VIDEO_VIEW_REPORT_STATUS_STATION_STREAM_INFO
{
	int m_nStationId;
	VIDEO_VIEW_REPORT_STATUS_CAMERA_STREAM_INFO m_CameraInfo[EXCEL_STATION_MAX_CAMERA_NUM];
	int m_nCameraCount;
};

struct VIDEO_VIEW_REPORT_STATUS_CAMERA_INFO
{
public:
	int    m_nCameraId;
	char m_szCameraName[CAMERANAMELEN];//摄像头名称
	char m_szCameraNum[CAMERACALLNUMLEN];//摄像头号码
	int    m_nCameraStatus;
	int    m_nStreamless;
	int    m_nCameraOfflineCount;
};

struct VIDEO_VIEW_REPORT_STATUS_STATION_DAILY_INFO
{
public:
	int    m_nStationId;//站点id号
	VIDEO_VIEW_REPORT_STATUS_CAMERA_INFO m_CameraInfo[EXCEL_STATION_MAX_CAMERA_NUM];
	int m_nCameraCount;
};

//////////////////////////////////////////////////////////////////////////






#endif