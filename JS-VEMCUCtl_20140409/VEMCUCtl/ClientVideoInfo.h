#pragma once

#include "InfoRecvQueue.h"
#include ".\include\mysql\mysql.h"				//连接数据库头文件

typedef struct _client_video_info_
{
	char node_name[256];
	int    station_id;
	char station_name[256];
	char station_code[64];
	char camera_name[256];
	char camera_code[64];
	char time[64];
	char user_name[64];
	int    operate_type;
	int    operate_result;
}CLIENT_VIDEO_INFO;

class CClientVideoInfo
{
public:
	CClientVideoInfo(void);
	~CClientVideoInfo(void);

public:
	bool InitClientVideoInfo();
	bool StartClientVideoInfo();
	bool StopClientVideoInfo();
	bool UnInitClientVideoInfo();
	bool WriteClientVideoInfo(CLIENT_VIDEO_INFO *pClientVideoInfo);

public:
	bool Lock();//加锁
	bool Unlock();//解锁
	bool SetClientVideoInfo(char *pClientIp,char *pDBServerIp,int nDBServerPort,char *pDBName,char *pDBUserName,char *pDBUserPassword);//成员变量赋值
	bool ConnectMySql();	// 连接数据库
	bool DisConnectMySql();	// 断开数据库
	bool CreateClientVideoInfoTable(char *szClientVideoInfoTableName);
	bool CreateUserStationViewHistoryTable(char *szUserStationViewHistoryTableName);
	bool UpdateUserStationViewHistoryTable(char *szUserStationViewHistoryTableName);
	bool UpdateUserStationViewHistoryTableByStationId(char *szUserStationViewHistoryTableName,int nStationId);
	bool WriteClientVideoInfoFromInfoRecvQueue();
	bool WriteClientVideoInfoToDB(void *pInfo,int nInfoLen);//更新用户巡视站点历史信息
	bool UpdateStationCameraVideoInfoToDB(void *pInfo,int nInfoLen);//更新站点摄像头访问时间
	bool UpdateStationCameraVideoInfoToDBTag(void *pInfo,int nInfoLen);//更新用户巡视站点历史信息
	bool UpdateStationCameraVideoInfoToDBTagDKY(void *pInfo,int nInfoLen);//更新用户巡视站点历史信息

public:
	bool m_bClientVideoInfoInitFlag;
	CRITICAL_SECTION m_csClientVideoInfoLock;
	HANDLE m_hClientVideoInfoThread;
	UINT m_uClientVideoInfoThreadID;
	char  m_szClientIp[64];
	char  m_szDBServerIp[64];
	int     m_nDBServerPort;
	char  m_szDBName[256];
	char  m_szDBUserName[256];
	char  m_szDBUserPassword[64];
	MYSQL *m_pMySql;
};


//////////////////////////////////////////////////////////////////////////
unsigned __stdcall _ClientVideoInfoWriteThread(void *arg);

//////////////////////////////////////////////////////////////////////////
extern CClientVideoInfo g_ClientVideoInfo;