#ifndef __XML_API__
#define __XML_API__


#define   CODE_LEN	20
#define	TIME_LEN	22

#define MAX_ALARM_NOTIFY_NUM		100
#define MAX_STATUS_NOTIFY_NUM		300


#define SIP_DEFAULT					0
#define SIP_NOTIFY_ALARM			1
#define SIP_NOTIFY_STATUS			2


// ======订阅通知部分======
// 通知告警
typedef struct _notify_alarm 
{
	char	code[CODE_LEN];
	char	name[64];
	int	type;
	char	StartTime[TIME_LEN];
	char	stationname[64];
	int	flag;
}NOTIFY_ALARM;

// 通知状态
typedef struct _notify_status 
{
	char	code[CODE_LEN];
	char	name[64];
	int	status;
}NOTIFY_STATUS;


// 解析告警通知
extern int ParseNotifyAlarm(NOTIFY_ALARM notify_alarm[], int num, const char *strXML);

// 解析状态通知
extern int ParseNotifyStatus(NOTIFY_STATUS notify_status[], int num, const char *strXML);

// 获取请求XML类型和子项数
extern int GetXmlTypeNum(const char *strXML, int *type, int *num);

/************************************************************************
*视频联动信息
************************************************************************/
#define  LINKAGE_DEVICE_CODE_LEN 64
#define  LINKAGE_DEVICE_NAME_LEN 256
#define  LINKAGE_STATION_NAME_LEN 256
#define  LINKAGE_TIME_LEN 64
#define  LINKAGE_ALARM_CONTENT_LEN 512
#define  LINKAGE_STATE_INFO_LEN 128
#define  LINKAGE_VALUE_INFO_LEN 64
#define  LINKAGE_TYPE_LEN  64
#define  LINKAGE_TYPE_NAME_LEN  256
#define  LINKAGE_TYPE_VALUE_LEN  256
#define  VIDEO_USER_NAME_LEN 64
#define  VIDEO_STATION_NAME_LEN 256
#define  VIDEO_DEVICE_NAME_LEN 256
#define  VIDEO_PRESET_NAME_LEN 256
#define  VIDEO_DEVICE_CODE_LEN 64
#define  VIDEO_SCENE_NAME_LEN 256
#define  VIDEO_LINKAGE_INFO_LEN 64
#define  LINKAGE_CONTENT_LEN 512
#define  LINKAGE_ID_LEN 64

//////////////////////////////////////////////////////////////////////////
//视频联动信息类型
#define  VIDEO_LINKAGE_INFO_UNDEFINE_TYPE 0
#define  VIDEO_LINKAGE_INFO_TYPE 1
#define  VIDEO_ALARM_LINKAGE_INFO_TYPE 2
#define  VIDEO_STATE_LINKAGE_INFO_TYPE 3
#define  VIDEO_CAMERA_CONTROL_NOTIFY_INFO_TYPE  4
#define  VIDEO_WEATHER_WARNING_INFO_TYPE 5
#define  VIDEO_WEATHER_FORECAST_INFO_TYPE 6

//////////////////////////////////////////////////////////////////////////
//摄像头信息--http--xml-接收--解析
typedef struct _VIDEO_LINKAGE_CAMERA_INFO_ 
{
	char	szCode[LINKAGE_DEVICE_CODE_LEN];
	char	szName[LINKAGE_DEVICE_NAME_LEN];
	int	nDecodeTag;
	char szPresetName[VIDEO_PRESET_NAME_LEN];
	int    nPresetId;
	int    nFromX1;
	int    nFromY1;
	int    nToX1;
	int    nToY1;
	int    nFromX2;
	int    nFromY2;
	int    nToX2;
	int    nToY2;
	char szSceneName[VIDEO_SCENE_NAME_LEN];
}VIDEO_LINKAGE_CAMERA_INFO;

//手动和巡检联动--http--xml-接收--解析
typedef struct _VIDEO_LINKAGE_INFO_ 
{
	char	szStationName[LINKAGE_STATION_NAME_LEN];
	char	szDeviceName[LINKAGE_DEVICE_NAME_LEN];
	int	nType; //1:手动 2:巡检
	int    nScreenId;
	char szTime[LINKAGE_TIME_LEN];
	char szLinkageStationName[LINKAGE_STATION_NAME_LEN];
	int    nLinkNum;
	VIDEO_LINKAGE_CAMERA_INFO *pLinkageCameraInfo;
}VIDEO_LINKAGE_INFO;


//告警联动--http--xml--接收--解析
typedef struct _VIDEO_ALARM_LINKAGE_INFO_ 
{
	char	szStationName[LINKAGE_STATION_NAME_LEN];
	char	szDeviceName[LINKAGE_DEVICE_NAME_LEN];
	char szAlarmType[LINKAGE_TYPE_LEN];//告警、异常、变位、越限、告知和保护动作
	char szTime[LINKAGE_TIME_LEN];
	char szContent[LINKAGE_ALARM_CONTENT_LEN];
	char szLinkageStationName[LINKAGE_STATION_NAME_LEN];
	int    nLinkNum;
	VIDEO_LINKAGE_CAMERA_INFO *pLinkageCameraInfo;
}VIDEO_ALARM_LINKAGE_INFO;


//状态联动--http--xml--接收--解析
typedef struct _VIDEO_STATE_LINKAGE_INFO_ 
{
	char	szStationName[LINKAGE_STATION_NAME_LEN];
	char	szDeviceName[LINKAGE_DEVICE_NAME_LEN];
	char	szDeviceType[LINKAGE_TYPE_LEN];//变压器、开关、刀闸、容抗器、线路、母线、发电机
	char szState[LINKAGE_STATE_INFO_LEN];//正常变位、异常变位、越限、告知、保护动作等动作类型
	char szTime[LINKAGE_TIME_LEN];
	char szContent[LINKAGE_ALARM_CONTENT_LEN];
	char szValue1[LINKAGE_VALUE_INFO_LEN];
	char szValue2[LINKAGE_VALUE_INFO_LEN];
	char szLinkageStationName[LINKAGE_STATION_NAME_LEN];
	int    nLinkNum;
	VIDEO_LINKAGE_CAMERA_INFO *pLinkageCameraInfo;
}VIDEO_STATE_LINKAGE_INFO;

//////////////////////////////////////////////////////////////////////////
//云镜控制--tcp--发送
typedef struct _VIDEO_CAMERA_CONTROL_INFO_ 
{
	char szUserName[VIDEO_USER_NAME_LEN];
	char	szStationName[VIDEO_STATION_NAME_LEN];
	char	szDeviceName[VIDEO_DEVICE_NAME_LEN];
	char	szDeviceCode[VIDEO_DEVICE_CODE_LEN];
	int    nCommand;
	int    nCommandPara1;
	int    nCommandPara2;
	int    nCommandPara3;
}VIDEO_CAMERA_CONTROL_INFO;

//////////////////////////////////////////////////////////////////////////
//云镜控制通知信息--http--xml--接收--解析
typedef struct _VIDEO_CAMERA_CONTROL_NOTIFY_INFO_ 
{
	char	szStationName[VIDEO_STATION_NAME_LEN];
	char	szDeviceName[VIDEO_DEVICE_NAME_LEN];
	char	szDeviceCode[VIDEO_DEVICE_CODE_LEN];
	char szUserName[VIDEO_USER_NAME_LEN];
}VIDEO_CAMERA_CONTROL_NOTIFY_INFO;

//气象告警
typedef struct _VIDEO_WEATHER_WARNING_INFO_
{
	char  szStationID[LINKAGE_ID_LEN];
	char szWeatherType[LINKAGE_TYPE_LEN];
	char szTypeName[LINKAGE_TYPE_NAME_LEN];
	char szWarnValue[LINKAGE_TYPE_VALUE_LEN];
	char szTime[LINKAGE_TIME_LEN];
	char szLinkageStationName[LINKAGE_STATION_NAME_LEN];
	int    nLinkNum;
	VIDEO_LINKAGE_CAMERA_INFO *pLinkageCameraInfo;
}VIDEO_WEATHER_WARNING_INFO;


//气象预警
typedef struct _VIDEO_WEATHER_FORECAST_INFO_
{
	char  szID[LINKAGE_ID_LEN];
	char szWeatherType[LINKAGE_TYPE_LEN];
	char szTypeName[LINKAGE_TYPE_NAME_LEN];
	char szWeatherLevel[VIDEO_LINKAGE_INFO_LEN];
	char szAreaType[VIDEO_LINKAGE_INFO_LEN];
	char szCityID[VIDEO_LINKAGE_INFO_LEN];
	char szCountyID[VIDEO_LINKAGE_INFO_LEN];
	char szContent[LINKAGE_CONTENT_LEN];
	char szStartTime[LINKAGE_TIME_LEN];
	char szStopTime[LINKAGE_TIME_LEN];
	char szLinkageStationName[LINKAGE_STATION_NAME_LEN];
	int    nLinkNum;
	VIDEO_LINKAGE_CAMERA_INFO *pLinkageCameraInfo;
}VIDEO_WEATHER_FORECAST_INFO;


//得到Xml信息类型
int GetXmlInfoType(char *strXml,int nLen);

// 解析手动和巡检联动
int ParseLinkageInfo(VIDEO_LINKAGE_INFO *pVideoLinkageInfo,const char *strXML);

// 解析告警联动
int ParseAlarmLinkageInfo(VIDEO_ALARM_LINKAGE_INFO *pVideoLinkageInfo,const char *strXML);

// 解析状态联动
int ParseStateLinkageInfo(VIDEO_STATE_LINKAGE_INFO *pVideoLinkageInfo,const char *strXML);

// 解析云镜控制通知信息
int ParseCameraControlInfo(VIDEO_CAMERA_CONTROL_NOTIFY_INFO *pCameraControlNotifyInfo,const char *strXML);

// 解析气象告警联动
int ParseWeatherWarningLinkageInfo(VIDEO_WEATHER_WARNING_INFO *pVideoLinkageInfo,const char *strXML);

// 解析气象预警联动
int ParseWeatherForecastLinkageInfo(VIDEO_WEATHER_FORECAST_INFO *pVideoLinkageInfo,const char *strXML);



#endif
