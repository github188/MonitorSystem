#ifndef _VM_HISTORY_LOG_H_
#define _VM_HISTORY_LOG_H_
#endif

class CVMHistoryLog
{
public:
	CVMHistoryLog(void);
	virtual ~CVMHistoryLog(void);

public:
	BOOL Lock();//加锁
	BOOL Unlock();//解锁
	BOOL SetVMHistoryLogInfo(char *pLogInUserName,char *pDBServerIP,int nDBServerPort,char *pDBName,char *pDBUserName,char *pDBUserPassword);//成员变量赋值
	BOOL ConnectMySql(void);	// 连接数据库
	BOOL DisConnectMySql(void);	// 断开数据库

	BOOL CreateLinkageHistoryLogTable(char *szHistoryLogTableName);
	BOOL CreateLinkageCameraHistoryLogTable(char *szHistoryLogTableName);

	BOOL WriteManualLinkageLog(VIDEO_LINKAGE_INFO *pVideoManualLinkageInfo);
	BOOL WriteAlarmLinkageLog(VIDEO_ALARM_LINKAGE_INFO *pVideoAlarmLinkageInfo);
	BOOL WriteStateLinkageLog(VIDEO_STATE_LINKAGE_INFO *pVideoStateLinkageInfo);
	BOOL WriteCameraControlNotifyLog(VIDEO_CAMERA_CONTROL_NOTIFY_INFO *pVideoCameraControlNotifyInfo);
	BOOL WriteWeatherWarningLinkageLog(VIDEO_WEATHER_WARNING_INFO *pVideoWeatherWarningLinkageInfo);
	BOOL WriteWeatherForecastLinkageLog(VIDEO_WEATHER_FORECAST_INFO *pVideoWeatherForecastLinkageInfo);
	
public:
	CRITICAL_SECTION m_cs;
	char  m_szLogInUserName[256];//登录用户
	char	 m_szDBServerIP[20];//数据库服务器的地址
	int     m_nDBServerPort;//端口
	char  m_szDBName[256];//数据库名称
	char  m_szDBUserName[256];//用户名
	char  m_szDBUserPassword[256];//密码
	MYSQL *m_pMySql;//mysql数据库访问
};

//////////////////////////////////////////////////////////////////////////
extern CVMHistoryLog g_VMHistoryLog;
