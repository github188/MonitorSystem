#pragma once

#include "InfoRecvQueue.h"
#include ".\include\mysql\mysql.h"				//连接数据库头文件

typedef struct _camera_status_info_
{
	int    station_id;
	int    type;
	char camera_code[64];
	int    camera_status;
}CAMERA_STATUS_INFO;

class CCameraStatusInfo
{
public:
	CCameraStatusInfo(void);
	~CCameraStatusInfo(void);

public:
	bool InitCameraStatusInfo();
	bool StartCameraStatusInfo();
	bool StopCameraStatusInfo();
	bool UnInitCameraStatusInfo();
	bool WriteCameraStatusInfo(CAMERA_STATUS_INFO *pCameraStatusInfo);

public:
	bool Lock();//加锁
	bool Unlock();//解锁
	bool SetCameraStatusInfo(char *pClientIp,char *pDBServerIp,int nDBServerPort,char *pDBName,char *pDBUserName,char *pDBUserPassword);//成员变量赋值
	bool ConnectMySql();	// 连接数据库
	bool DisConnectMySql();	// 断开数据库
	bool WriteCameraStatusInfoFromInfoRecvQueue();
	bool CreateUserStationViewHistoryTable(char *szUserStationViewHistoryTableName);
	bool UpdateUserStationViewHistoryTable(char *szUserStationViewHistoryTableName);
	bool UpdateUserStationViewHistoryTableByStationId(char *szUserStationViewHistoryTableName,int nStationId);
	bool WriteCameraStatusInfoToDB(void *pInfo,int nInfoLen);//更新站点摄像头状态
	bool WriteCameraStatusInfoToDB2(void *pInfo,int nInfoLen);//更新用户巡视站点摄像头状态

public:
	bool m_bCameraStatusInfoInitFlag;
	CRITICAL_SECTION m_csCameraStatusInfoLock;
	HANDLE m_hCameraStatusInfoThread;
	UINT m_uCameraStatusInfoThreadID;
	char  m_szClientIp[64];
	char  m_szDBServerIp[64];
	int     m_nDBServerPort;
	char  m_szDBName[256];
	char  m_szDBUserName[256];
	char  m_szDBUserPassword[64];
	MYSQL *m_pMySql;
};


//////////////////////////////////////////////////////////////////////////
unsigned __stdcall _CameraStatusInfoWriteThread(void *arg);

//////////////////////////////////////////////////////////////////////////
extern CCameraStatusInfo g_CameraStatusInfo;