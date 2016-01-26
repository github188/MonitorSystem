#include "StdAfx.h"
#include "AssUserOperateLog.h"

//////////////////////////////////////////////////////////////////////////
// 数据库连接
#pragma comment(lib,"lib\\libmySQL.lib")

//////////////////////////////////////////////////////////////////////////
CAssUserOperateLog g_AssUserOperateLog;
Recv_Buf_Queue g_AssUserOperateLogRecvBufQueue;
volatile bool g_bAssUserOperateLogWriteThreadExitFlag = false;
Recv_Buf_Node_Type g_AssUserOperateLogWritEnNode;
Recv_Buf_Node_Type g_ThreadAssUserOperateLogWriteDeNode;
//////////////////////////////////////////////////////////////////////////

CAssUserOperateLog::CAssUserOperateLog(void)
{
	m_bAssUserOperateLogInitFlag = false;
	m_hAssUserOperateLogThread = NULL;
	m_uAssUserOperateLogThreadID = 0;
	memset(m_szClientIp,0,sizeof(m_szClientIp));
	memset(m_szDBServerIp,0,sizeof(m_szDBServerIp));
	m_nDBServerPort = 0;
	memset(m_szDBName,0,sizeof(m_szDBName));
	memset(m_szDBUserName,0,sizeof(m_szDBUserName));
	memset(m_szDBUserPassword,0,sizeof(m_szDBUserPassword));
	m_pMySql = NULL;
	InitAssUserOperateLog();
}

CAssUserOperateLog::~CAssUserOperateLog(void)
{
	StopAssUserOperateLog();
	UnInitAssUserOperateLog();
	m_bAssUserOperateLogInitFlag = false;
	m_hAssUserOperateLogThread = NULL;
	m_uAssUserOperateLogThreadID = 0;
	memset(m_szDBServerIp,0,sizeof(m_szDBServerIp));
	m_nDBServerPort = 0;
	memset(m_szDBName,0,sizeof(m_szDBName));
	memset(m_szDBUserName,0,sizeof(m_szDBUserName));
	memset(m_szDBUserPassword,0,sizeof(m_szDBUserPassword));
	m_pMySql = NULL;
}

//初始化
bool CAssUserOperateLog::InitAssUserOperateLog()
{
	if (m_bAssUserOperateLogInitFlag)
		return true;

	InitializeCriticalSection(&m_csAssUserOperateLogLock);

	InitRecvQueue(g_AssUserOperateLogRecvBufQueue);

	memset(&g_AssUserOperateLogWritEnNode,0,sizeof(g_AssUserOperateLogWritEnNode));
	memset(&g_ThreadAssUserOperateLogWriteDeNode,0,sizeof(g_ThreadAssUserOperateLogWriteDeNode));

	m_bAssUserOperateLogInitFlag = true;

	return true;
}

//反初始化
bool CAssUserOperateLog::UnInitAssUserOperateLog()
{
	if (!m_bAssUserOperateLogInitFlag)
		return true;

	EnterCriticalSection(&m_csAssUserOperateLogLock);

	ClearRecvQueue(g_AssUserOperateLogRecvBufQueue);

	LeaveCriticalSection(&m_csAssUserOperateLogLock);
	DeleteCriticalSection(&m_csAssUserOperateLogLock);

	m_bAssUserOperateLogInitFlag = false;

	return true;
}

bool CAssUserOperateLog::StartAssUserOperateLog()
{
	g_bAssUserOperateLogWriteThreadExitFlag = false;

	m_hAssUserOperateLogThread = (HANDLE)_beginthreadex(NULL, 0,_AssUserOperateLogWriteThread,(void*)this, 0, &m_uAssUserOperateLogThreadID);
	if (m_hAssUserOperateLogThread == NULL)
		return false;

	return true;
}

bool CAssUserOperateLog::StopAssUserOperateLog()
{
	g_bAssUserOperateLogWriteThreadExitFlag = true;

	DWORD dwResult = WaitForSingleObject(m_hAssUserOperateLogThread,6000);
	if (dwResult != WAIT_OBJECT_0)
	{
		TerminateThread(m_hAssUserOperateLogThread,1);
	}

	m_hAssUserOperateLogThread = NULL;
	m_uAssUserOperateLogThreadID = 0;

	DisConnectMySql();

	return true;
}

bool CAssUserOperateLog::Lock()
{
	EnterCriticalSection(&m_csAssUserOperateLogLock);
	return true;
}

bool CAssUserOperateLog::Unlock()
{
	LeaveCriticalSection(&m_csAssUserOperateLogLock);
	return true;
}

bool CAssUserOperateLog::SetAssUserOperateLog( char *pClientIp,char *pDBServerIp,int nDBServerPort,char *pDBName,char *pDBUserName,char *pDBUserPassword )
{
	if (pClientIp != NULL)
	{
		strcpy_s(m_szClientIp,sizeof(m_szClientIp),pClientIp);
	}

	if (pDBServerIp != NULL)
	{
		strcpy_s(m_szDBServerIp,sizeof(m_szDBServerIp),pDBServerIp);
	}

	if (nDBServerPort > 0)
	{
		m_nDBServerPort = nDBServerPort;
	}

	if (pDBName != NULL)
	{
		strcpy_s(m_szDBName,sizeof(m_szDBName),pDBName);
	}

	if (pDBUserName != NULL)
	{
		strcpy_s(m_szDBUserName,sizeof(m_szDBUserName),pDBUserName);
	}

	if (pDBUserPassword != NULL)
	{
		strcpy_s(m_szDBUserPassword,sizeof(m_szDBUserPassword),pDBUserPassword);
	}

	return true;
}

bool CAssUserOperateLog::ConnectMySql()
{
	Lock();

	if (strlen(m_szDBServerIp) == 0||m_nDBServerPort == 0
		||strlen(m_szDBName) == 0||strlen(m_szDBUserName) == 0
		||strlen(m_szDBUserPassword) == 0)
	{
		Unlock();
		return false;
	}

	do 
	{
		try
		{
			if (m_pMySql != NULL)
			{
				mysql_close(m_pMySql);
				m_pMySql = NULL;
				TRACE("断开mysql连接!");
			}

			m_pMySql = mysql_init((MYSQL*)NULL);
			if (m_pMySql == NULL)
			{
				TRACE("初始化mysql失败!\n");
				break;
			}

			if (mysql_real_connect(m_pMySql, m_szDBServerIp, m_szDBUserName, m_szDBUserPassword, m_szDBName, m_nDBServerPort, NULL, 0))
			{
				if (mysql_select_db(m_pMySql, m_szDBName) < 0)
				{
					TRACE("已成功连接mysql, 但选择数据库出错!\n");
					mysql_close(m_pMySql);
					m_pMySql = NULL;
					break;
				}

				int nValue = 1;
				mysql_options(m_pMySql,MYSQL_OPT_RECONNECT,(char *)&nValue);

				mysql_query(m_pMySql,"SET NAMES GB2312");
				TRACE("已成功连接mysql!\n");

				Unlock();
				return true;
			}
			else
			{
				TRACE("无法连接到数据库: Error=%s\n",mysql_error(m_pMySql));
				mysql_close(m_pMySql);
				m_pMySql = NULL;
				TRACE("连接mysql失败!\n");
				break;
			}
		}
		catch(...)
		{
			m_pMySql = NULL;
		}
	} while (false);

	Unlock();
	return false;
}

bool CAssUserOperateLog::DisConnectMySql()
{
	Lock();

	try
	{
		if (m_pMySql != NULL)
		{
			mysql_close(m_pMySql);
			m_pMySql = NULL;
			TRACE("断开mysql连接!");
		}
		Unlock();
		return true;
	}
	catch(...)
	{

	}
	Unlock();
	return false;
}

bool CAssUserOperateLog::CreateAssUserOperateLogTable( char *szAssUserOperateLogTableName )
{
	char sql_buf[1024] = {0};
	int    nResult = 0;
	bool bResult = false;

	sprintf_s(sql_buf," CREATE TABLE IF NOT EXISTS %s ( "
		" `id` int(16) NOT NULL auto_increment, "
		" `user_id` int(5) NOT NULL, "
		" `ip_address` varchar(32) NOT NULL, "
		" `operate_time` varchar(64) NOT NULL, "
		" `operate_action` varchar(128) NOT NULL, "
		" PRIMARY KEY  (`id`) "
		" ) ENGINE=InnoDB DEFAULT CHARSET=utf8; ",
		szAssUserOperateLogTableName
		);

	nResult = mysql_query(m_pMySql, sql_buf);

	bResult = ((nResult == 0)?true:false);

	return bResult;
}

bool CAssUserOperateLog::WriteAssUserOperateLogFromInfoRecvQueue()
{
	DWORD dwCount = 0;

	EnterCriticalSection(&m_csAssUserOperateLogLock);

	do 
	{
		if (RecvQueueEmpty(g_AssUserOperateLogRecvBufQueue))
		{
			break;
		}

		if(RecvDeQueue(g_AssUserOperateLogRecvBufQueue,g_ThreadAssUserOperateLogWriteDeNode) > 0)
		{
			WriteAssUserOperateLogToDB((void*)&g_ThreadAssUserOperateLogWriteDeNode,sizeof(g_ThreadAssUserOperateLogWriteDeNode));
		}

	} while (true);

	LeaveCriticalSection(&m_csAssUserOperateLogLock);

	return true;
}

bool CAssUserOperateLog::WriteAssUserOperateLogToDB( void *pInfo,int nInfoLen )
{
	if (m_pMySql == NULL)
	{
		ConnectMySql();
	}

	if (m_pMySql == NULL)
		return false;

	if (pInfo == NULL||nInfoLen != sizeof(g_ThreadAssUserOperateLogWriteDeNode))
		return false;

	Recv_Buf_Node_Type *pNodeInfo = (Recv_Buf_Node_Type *)pInfo;

	ASS_USER_OPERATE_LOG *pAssUserOperateLog  = (ASS_USER_OPERATE_LOG *)pNodeInfo->Buf;
	int nAssUserOperateLog = pNodeInfo->DataLen;

	if (nAssUserOperateLog != sizeof(ASS_USER_OPERATE_LOG))
		return false;

	SYSTEMTIME sysTm;

	char sql_buf[1024] = {0};
	char szTableName[255] = {0};

	memset(sql_buf,0,sizeof(sql_buf));
	memset(szTableName,0,sizeof(szTableName));

	try
	{
		::GetLocalTime(&sysTm);
		sprintf_s(szTableName,"ass_user_operate_log_%04d",sysTm.wYear);
		sprintf_s(pAssUserOperateLog->operate_time,"%04d-%02d-%02d %02d:%02d:%02d",sysTm.wYear,sysTm.wMonth,sysTm.wDay,sysTm.wHour,sysTm.wMinute,sysTm.wSecond);

		//检查表是否存在,不存在则创建
		CreateAssUserOperateLogTable(szTableName);

		sprintf_s(sql_buf, "INSERT INTO %s (user_id,ip_address,operate_time,operate_action) "
			"VALUES (%d,'%s','%s','%s')",
			szTableName,
			pAssUserOperateLog->user_id,
			pAssUserOperateLog->ip_address,
			pAssUserOperateLog->operate_time,
			pAssUserOperateLog->operate_action
			);

		if (mysql_query(m_pMySql, sql_buf))
		{
			return false;
		}

		return true;
	}
	catch(...)
	{

	}

	return false;
}

bool CAssUserOperateLog::WriteAssUserOperateLog( ASS_USER_OPERATE_LOG *pAssUserOperateLog )
{
	if (pAssUserOperateLog == NULL)
		return false;

	g_AssUserOperateLogWritEnNode.DataLen = sizeof(ASS_USER_OPERATE_LOG);
	memcpy(g_AssUserOperateLogWritEnNode.Buf,pAssUserOperateLog,sizeof(ASS_USER_OPERATE_LOG));

	EnterCriticalSection(&m_csAssUserOperateLogLock);
	RecvEnQueue(g_AssUserOperateLogRecvBufQueue,g_AssUserOperateLogWritEnNode);
	LeaveCriticalSection(&m_csAssUserOperateLogLock);

	return true;
}

unsigned __stdcall _AssUserOperateLogWriteThread(void *arg)
{
	if (arg == NULL)
		return -1;

	CAssUserOperateLog *pAssUserOperateLog = (CAssUserOperateLog *)arg;

	try
	{
		while (1)
		{
			if (g_bAssUserOperateLogWriteThreadExitFlag == true)
			{
				return 0;
			}

			pAssUserOperateLog->WriteAssUserOperateLogFromInfoRecvQueue();

			Sleep(200);
		}
	}
	catch(...)
	{

	}

	return 0;
}