#pragma once

#include "InfoRecvQueue.h"
#include ".\include\mysql\mysql.h"				//连接数据库头文件

typedef struct _video_error_info_
{
	char node_name[256];
	char station_name[256];
	char station_code[64];
	char camera_name[256];
	char camera_code[64];
	int    state;
	int    reason;
	int    flow;
	char time[64];
}VIDEO_ERROR_INFO;

class CVideoErrorInfo
{
public:
	CVideoErrorInfo(void);
	~CVideoErrorInfo(void);

public:
	bool InitVideoErrorInfo();
	bool StartVideoErrorInfo();
	bool StopVideoErrorInfo();
	bool UnInitVideoErrorInfo();
	bool WriteVideoErrorInfo(VIDEO_ERROR_INFO *pVideoErrorInfo);

public:
	bool Lock();//加锁
	bool Unlock();//解锁
	bool SetVideoErrorInfo(char *pClientIp,char *pDBServerIp,int nDBServerPort,char *pDBName,char *pDBUserName,char *pDBUserPassword);//成员变量赋值
	bool ConnectMySql();	// 连接数据库
	bool DisConnectMySql();	// 断开数据库
	bool CreateVideoErrorInfoTable(char *szVideoErrorInfoTableName);
	bool WriteVideoErrorInfoFromInfoRecvQueue();
	bool WriteVideoErrorInfoToDB(void *pInfo,int nInfoLen);

public:
	bool m_bVideoErrorInfoInitFlag;
	CRITICAL_SECTION m_csVideoErrorInfoLock;
	HANDLE m_hVideoErrorInfoThread;
	UINT m_uVideoErrorInfoThreadID;
	char  m_szClientIp[64];
	char  m_szDBServerIp[64];
	int     m_nDBServerPort;
	char  m_szDBName[256];
	char  m_szDBUserName[256];
	char  m_szDBUserPassword[64];
	MYSQL *m_pMySql;
};


//////////////////////////////////////////////////////////////////////////
unsigned __stdcall _VideoErrorInfoWriteThread(void *arg);

//////////////////////////////////////////////////////////////////////////
extern CVideoErrorInfo g_VideoErrorInfo;