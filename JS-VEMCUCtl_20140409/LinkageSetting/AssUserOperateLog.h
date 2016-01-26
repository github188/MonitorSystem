#pragma once

#include "InfoRecvQueue.h"
#include "include/mysql.h"				//连接数据库头文件

typedef struct _ass_user_operate_log
{
	int user_id;
	char ip_address[32];
	char operate_time[64];
	char operate_action[128];
}ASS_USER_OPERATE_LOG;

class CAssUserOperateLog
{
public:
	CAssUserOperateLog(void);
	~CAssUserOperateLog(void);

public:
	bool InitAssUserOperateLog();
	bool StartAssUserOperateLog();
	bool StopAssUserOperateLog();
	bool UnInitAssUserOperateLog();
	bool WriteAssUserOperateLog(ASS_USER_OPERATE_LOG *pAssUserOperateLog);

public:
	bool Lock();//加锁
	bool Unlock();//解锁
	bool SetAssUserOperateLog(char *pClientIp,char *pDBServerIp,int nDBServerPort,char *pDBName,char *pDBUserName,char *pDBUserPassword);//成员变量赋值
	bool ConnectMySql();	// 连接数据库
	bool DisConnectMySql();	// 断开数据库
	bool CreateAssUserOperateLogTable(char *szAssUserOperateLogTableName);
	bool WriteAssUserOperateLogFromInfoRecvQueue();
	bool WriteAssUserOperateLogToDB(void *pInfo,int nInfoLen);
public:
	bool m_bAssUserOperateLogInitFlag;
	CRITICAL_SECTION m_csAssUserOperateLogLock;
	HANDLE m_hAssUserOperateLogThread;
	UINT m_uAssUserOperateLogThreadID;
	char  m_szClientIp[64];
	char  m_szDBServerIp[64];
	int   m_nDBServerPort;
	char  m_szDBName[256];
	char  m_szDBUserName[256];
	char  m_szDBUserPassword[64];
	MYSQL *m_pMySql;
	
};

//////////////////////////////////////////////////////////////////////////
unsigned __stdcall _AssUserOperateLogWriteThread(void *arg);

//////////////////////////////////////////////////////////////////////////
extern CAssUserOperateLog g_AssUserOperateLog;
