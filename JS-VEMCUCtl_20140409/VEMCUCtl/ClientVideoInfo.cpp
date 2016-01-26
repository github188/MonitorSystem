#include "StdAfx.h"
#include "ClientVideoInfo.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"

//////////////////////////////////////////////////////////////////////////
// 数据库连接
#pragma comment(lib,"lib\\libmySQL.lib")

//////////////////////////////////////////////////////////////////////////
CClientVideoInfo g_ClientVideoInfo;
Recv_Buf_Queue g_ClientVideoInfoRecvBufQueue;
volatile bool g_bClientVideoInfoWriteThreadExitFlag = false;
Recv_Buf_Node_Type g_ClientVideoInfoWritEnNode;
Recv_Buf_Node_Type g_ThreadClientVideoInfoWriteDeNode;
//////////////////////////////////////////////////////////////////////////

CClientVideoInfo::CClientVideoInfo(void)
{
	m_bClientVideoInfoInitFlag = false;
	m_hClientVideoInfoThread = NULL;
	m_uClientVideoInfoThreadID = 0;
	memset(m_szClientIp,0,sizeof(m_szClientIp));
	memset(m_szDBServerIp,0,sizeof(m_szDBServerIp));
	m_nDBServerPort = 0;
	memset(m_szDBName,0,sizeof(m_szDBName));
	memset(m_szDBUserName,0,sizeof(m_szDBUserName));
	memset(m_szDBUserPassword,0,sizeof(m_szDBUserPassword));
	m_pMySql = NULL;
	InitClientVideoInfo();
}

CClientVideoInfo::~CClientVideoInfo(void)
{
	StopClientVideoInfo();
	UnInitClientVideoInfo();
	m_bClientVideoInfoInitFlag = false;
	m_hClientVideoInfoThread = NULL;
	m_uClientVideoInfoThreadID = 0;
	memset(m_szClientIp,0,sizeof(m_szClientIp));
	memset(m_szDBServerIp,0,sizeof(m_szDBServerIp));
	m_nDBServerPort = 0;
	memset(m_szDBName,0,sizeof(m_szDBName));
	memset(m_szDBUserName,0,sizeof(m_szDBUserName));
	memset(m_szDBUserPassword,0,sizeof(m_szDBUserPassword));
	m_pMySql = NULL;
}

bool CClientVideoInfo::InitClientVideoInfo()
{
	if (m_bClientVideoInfoInitFlag)
		return true;

	InitializeCriticalSection(&m_csClientVideoInfoLock);

	InitRecvQueue(g_ClientVideoInfoRecvBufQueue);

	memset(&g_ClientVideoInfoWritEnNode,0,sizeof(g_ClientVideoInfoWritEnNode));
	memset(&g_ThreadClientVideoInfoWriteDeNode,0,sizeof(g_ThreadClientVideoInfoWriteDeNode));

	m_bClientVideoInfoInitFlag = true;

	return true;

}

bool CClientVideoInfo::StartClientVideoInfo()
{
	g_bClientVideoInfoWriteThreadExitFlag = false;

	m_hClientVideoInfoThread = (HANDLE)_beginthreadex(NULL, 0,_ClientVideoInfoWriteThread,(void*)this, 0, &m_uClientVideoInfoThreadID);
	if (m_hClientVideoInfoThread == 0)
		return false;
	return true;
}

bool CClientVideoInfo::StopClientVideoInfo()
{
	g_bClientVideoInfoWriteThreadExitFlag = true;

	DWORD dwResult = WaitForSingleObject(m_hClientVideoInfoThread,6000);
	if (dwResult != WAIT_OBJECT_0)
	{
		TerminateThread(m_hClientVideoInfoThread,1);
	}

	m_hClientVideoInfoThread = NULL;
	m_uClientVideoInfoThreadID = 0;

	DisConnectMySql();

	return true;
}

bool CClientVideoInfo::UnInitClientVideoInfo()
{
	if (!m_bClientVideoInfoInitFlag)
		return true;

	EnterCriticalSection(&m_csClientVideoInfoLock);

	ClearRecvQueue(g_ClientVideoInfoRecvBufQueue);

	LeaveCriticalSection(&m_csClientVideoInfoLock);
	DeleteCriticalSection(&m_csClientVideoInfoLock);

	m_bClientVideoInfoInitFlag = false;

	return true;
}

bool CClientVideoInfo::WriteClientVideoInfo(CLIENT_VIDEO_INFO *pClientVideoInfo)
{
	if (pClientVideoInfo == NULL)
		return false;

	g_ClientVideoInfoWritEnNode.DataLen = sizeof(CLIENT_VIDEO_INFO);
	memcpy(g_ClientVideoInfoWritEnNode.Buf,pClientVideoInfo,sizeof(CLIENT_VIDEO_INFO));

	EnterCriticalSection(&m_csClientVideoInfoLock);
	RecvEnQueue(g_ClientVideoInfoRecvBufQueue,g_ClientVideoInfoWritEnNode);
	LeaveCriticalSection(&m_csClientVideoInfoLock);

	return true;
}

// 连接数据库
bool CClientVideoInfo::ConnectMySql(void)
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

// 断开数据库
bool CClientVideoInfo::DisConnectMySql(void)
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

bool CClientVideoInfo::CreateClientVideoInfoTable(char *szClientVideoInfoTableName)
{
	char sql_buf[1024] = {0};
	int    nResult = 0;
	bool bResult = false;

	sprintf_s(sql_buf,sizeof(sql_buf),"CREATE TABLE IF NOT EXISTS %s ("
		"id int(8) NOT NULL auto_increment,"
		"node_name char(255) NOT NULL default '',"
		"station_name char(255) NOT NULL default '',"
		"station_code char(64) NOT NULL default '',"
		"camera_name char(255) NOT NULL default '',"
		"camera_code char(64) NOT NULL default '',"
		"operate_type int(8) NOT NULL default '0',"
		"operate_result int(8) NOT NULL default '0',"
		"time char(64) NOT NULL default '',"
		"user_name char(64) NOT NULL default '',"
		"client_ip char(64) NOT NULL default '',"
		"PRIMARY KEY  (id))",
		szClientVideoInfoTableName
		);

	nResult = mysql_query(m_pMySql, sql_buf);

	bResult = ((nResult == 0)?true:false);

	return bResult;
}

bool CClientVideoInfo::CreateUserStationViewHistoryTable(char *szUserStationViewHistoryTableName)
{
	char sql_buf[1024] = {0};
	int    nResult = 0;

	try
	{
		memset(sql_buf,0,sizeof(sql_buf));
		sprintf_s(sql_buf,sizeof(sql_buf),"CREATE TABLE IF NOT EXISTS %s ("
			"id int(10) NOT NULL auto_increment,"
			"station_id int(10) NOT NULL default '0',"
			"camera_code char(64) NOT NULL default '',"
			"day int(4) NOT NULL default '1',"
			"status int(4) NOT NULL default '10',"
			"time char(32) NOT NULL default '',"
			"PRIMARY KEY  (id))",
			szUserStationViewHistoryTableName
			);

		nResult = mysql_query(m_pMySql, sql_buf);
		if (nResult != 0)
			return false;

		return true;
	}
	catch(...)
	{

	}

	return false;
}

bool CClientVideoInfo::UpdateUserStationViewHistoryTable(char *szUserStationViewHistoryTableName)
{
	char sql_buf[1024] = {0};
	int    nResult = 0;

	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;
	int nCount = 0;
	int nStationCount = 0;

	try
	{
		SYSTEMTIME sysTm;
		::GetLocalTime(&sysTm);

		int nYear = sysTm.wYear;
		int nMonth = sysTm.wMonth;
		int nDay = sysTm.wDay;

		memset(sql_buf,0,sizeof(sql_buf));

		sprintf_s(sql_buf,sizeof(sql_buf),"SELECT COUNT(*)  FROM %s WHERE day=%d ",szUserStationViewHistoryTableName,nDay);
		nResult = mysql_query(m_pMySql, sql_buf);
		if (nResult != 0)
			return false;

		res = mysql_store_result(m_pMySql);
		if (row = mysql_fetch_row(res))
		{
			nCount = atoi(row[0]);
		}
		mysql_free_result(res) ;

		if (nCount > 0)
			return true;

		char szCameraCode[512][32];
		int    nCameraStatus[512];
		int    nCameraCount = 0;
		memset(szCameraCode,0,sizeof(szCameraCode));
		memset(nCameraStatus,0,sizeof(nCameraStatus));

		for (int j = 0;j < g_nUserStationViewStationCount;j++)//站点
		{
			sprintf_s(sql_buf,sizeof(sql_buf),"SELECT camera_code,camera_status FROM ob_d5000_camera_station_%d ",g_nUserStationViewStationId[j]);
			nResult = mysql_query(m_pMySql, sql_buf);
			if (nResult != 0)
				continue;

			nCameraCount = 0;

			res = mysql_store_result(m_pMySql);
			while(row = mysql_fetch_row(res))
			{
				if (nCameraCount >= 512)
					break;

				strcpy_s(szCameraCode[nCameraCount],sizeof(szCameraCode[nCameraCount]),row[0]);
				nCameraStatus[nCameraCount] = atoi(row[1]);
				nCameraCount++;
			}
			mysql_free_result(res);

			for (int k = 0;k < nCameraCount;k++)
			{
				sprintf_s(sql_buf,sizeof(sql_buf),"INSERT INTO %s (station_id,camera_code,day,status) VALUES (%d,'%s',%d,%d) ",szUserStationViewHistoryTableName,g_nUserStationViewStationId[j],szCameraCode[k],nDay,10);
				mysql_query(m_pMySql, sql_buf);
			}
		}
		return true;
	}
	catch(...)
	{

	}

	return false;
}

bool CClientVideoInfo::UpdateUserStationViewHistoryTableByStationId(char *szUserStationViewHistoryTableName,int nStationId)
{
	char sql_buf[1024] = {0};
	int    nResult = 0;

	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;
	int nCount = 0;

	if (nStationId <= 0)
		return false;

	try
	{
		SYSTEMTIME sysTm;
		::GetLocalTime(&sysTm);

		int nYear = sysTm.wYear;
		int nMonth = sysTm.wMonth;
		int nDay = sysTm.wDay;

		sprintf_s(sql_buf,sizeof(sql_buf),"SELECT COUNT(*)  FROM %s WHERE day=%d AND station_id=%d ",szUserStationViewHistoryTableName,nDay,nStationId);
		nResult = mysql_query(m_pMySql, sql_buf);
		if (nResult != 0)
			return false;

		res = mysql_store_result(m_pMySql);
		if (row = mysql_fetch_row(res))
		{
			nCount = atoi(row[0]);
		}
		mysql_free_result(res) ;

		if (nCount > 0)
			return true;

		char szCameraCode[512][32];
		int    nCameraStatus[512];
		int    nCameraCount = 0;
		memset(szCameraCode,0,sizeof(szCameraCode));
		memset(nCameraStatus,0,sizeof(nCameraStatus));

		sprintf_s(sql_buf,sizeof(sql_buf),"SELECT camera_code,camera_status FROM ob_d5000_camera_station_%d ",nStationId);
		nResult = mysql_query(m_pMySql, sql_buf);
		if (nResult != 0)
			return false;

		nCameraCount = 0;

		res = mysql_store_result(m_pMySql);
		while(row = mysql_fetch_row(res))
		{
			if (nCameraCount >= 512)
				break;

			strcpy_s(szCameraCode[nCameraCount],sizeof(szCameraCode[nCameraCount]),row[0]);
			nCameraStatus[nCameraCount] = atoi(row[1]);
			nCameraCount++;
		}
		mysql_free_result(res);

		for (int k = 0;k < nCameraCount;k++)
		{
			sprintf_s(sql_buf,sizeof(sql_buf),"INSERT INTO %s (station_id,camera_code,day,status) VALUES (%d,'%s',%d,%d) ",szUserStationViewHistoryTableName,nStationId,szCameraCode[k],nDay,10);
			mysql_query(m_pMySql, sql_buf);
		}
		return true;
	}
	catch(...)
	{

	}

	return false;
}

bool CClientVideoInfo::Lock()
{
	EnterCriticalSection(&m_csClientVideoInfoLock);
	return true;
}

bool CClientVideoInfo::Unlock()
{
	LeaveCriticalSection(&m_csClientVideoInfoLock);
	return true;
}

//成员变量赋值
bool CClientVideoInfo::SetClientVideoInfo(char *pClientIp,char *pDBServerIp,int nDBServerPort,char *pDBName,char *pDBUserName,char *pDBUserPassword)
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

bool CClientVideoInfo::WriteClientVideoInfoToDB(void *pInfo,int nInfoLen)
{
	if (m_pMySql == NULL)
	{
		ConnectMySql();
	}

	if (m_pMySql == NULL)
		return false;

	if (pInfo == NULL||nInfoLen != sizeof(g_ThreadClientVideoInfoWriteDeNode))
		return false;

	Recv_Buf_Node_Type *pNodeInfo = (Recv_Buf_Node_Type *)pInfo;

	CLIENT_VIDEO_INFO *pVideoInfo  = (CLIENT_VIDEO_INFO *)pNodeInfo->Buf;
	int nVideoInfo = pNodeInfo->DataLen;

	if (nVideoInfo != sizeof(CLIENT_VIDEO_INFO))
		return false;

	SYSTEMTIME sysTm;

	char sql_buf[1024] = {0};
	char szTableName[255] = {0};

	memset(sql_buf,0,sizeof(sql_buf));
	memset(szTableName,0,sizeof(szTableName));

	try
	{
		::GetLocalTime(&sysTm);
		sprintf_s(szTableName,"ct_history_client_video_log_%04d",sysTm.wYear);
		sprintf_s(pVideoInfo->time,"%04d-%02d-%02d %02d:%02d:%02d",sysTm.wYear,sysTm.wMonth,sysTm.wDay,sysTm.wHour,sysTm.wMinute,sysTm.wSecond);

		//检查表是否存在,不存在则创建
		CreateClientVideoInfoTable(szTableName);

		sprintf_s(sql_buf, "INSERT INTO %s (node_name,station_name,station_code,camera_name,camera_code,operate_type,operate_result,time,user_name,client_ip) "
			"VALUES ('%s','%s','%s','%s','%s',%d,%d,'%s','%s','%s')",
			szTableName,
			pVideoInfo->node_name,
			pVideoInfo->station_name,
			pVideoInfo->station_code,
			pVideoInfo->camera_name,
			pVideoInfo->camera_code,
			pVideoInfo->operate_type,
			pVideoInfo->operate_result,
			pVideoInfo->time,
			pVideoInfo->user_name,
			m_szClientIp);

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

//更新站点摄像头访问时间
bool CClientVideoInfo::UpdateStationCameraVideoInfoToDB(void *pInfo,int nInfoLen)
{
	if (m_pMySql == NULL)
	{
		ConnectMySql();
	}

	if (m_pMySql == NULL)
		return false;

	if (pInfo == NULL||nInfoLen != sizeof(g_ThreadClientVideoInfoWriteDeNode))
		return false;

	Recv_Buf_Node_Type *pNodeInfo = (Recv_Buf_Node_Type *)pInfo;

	CLIENT_VIDEO_INFO *pVideoInfo  = (CLIENT_VIDEO_INFO *)pNodeInfo->Buf;
	int nVideoInfo = pNodeInfo->DataLen;

	if (nVideoInfo != sizeof(CLIENT_VIDEO_INFO))
		return false;

	SYSTEMTIME sysTm;

	char sql_buf[1024] = {0};
	char szTableName[255] = {0};
	char szTime[64] = {0};

	memset(sql_buf,0,sizeof(sql_buf));
	memset(szTableName,0,sizeof(szTableName));

	try
	{
		sprintf_s(szTableName,"ob_d5000_camera_station_%d",pVideoInfo->station_id);
		
		::GetLocalTime(&sysTm);
		sprintf_s(szTime,"%04d-%02d-%02d %02d:%02d:%02d",sysTm.wYear,sysTm.wMonth,sysTm.wDay,sysTm.wHour,sysTm.wMinute,sysTm.wSecond);

		sprintf_s(sql_buf, "UPDATE %s SET notes='%s' WHERE camera_code='%s' ",
			szTableName,
			szTime,
			pVideoInfo->camera_code);

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

//更新用户巡视站点历史信息
bool CClientVideoInfo::UpdateStationCameraVideoInfoToDBTag(void *pInfo,int nInfoLen)
{
	if (m_pMySql == NULL)
	{
		ConnectMySql();
	}

	if (m_pMySql == NULL)
		return false;

	if (pInfo == NULL||nInfoLen != sizeof(g_ThreadClientVideoInfoWriteDeNode))
		return false;

	Recv_Buf_Node_Type *pNodeInfo = (Recv_Buf_Node_Type *)pInfo;

	CLIENT_VIDEO_INFO *pVideoInfo  = (CLIENT_VIDEO_INFO *)pNodeInfo->Buf;
	int nVideoInfo = pNodeInfo->DataLen;

	if (nVideoInfo != sizeof(CLIENT_VIDEO_INFO))
		return false;

	if (g_userpower.userID < 0)
		return false;

	SYSTEMTIME sysTm;

	char sql_buf[1024] = {0};
	char szTableName[255] = {0};
	char szTime[64] = {0};

	memset(sql_buf,0,sizeof(sql_buf));
	memset(szTableName,0,sizeof(szTableName));

	try
	{
		::GetLocalTime(&sysTm);
		sprintf_s(szTableName,"ct_user_station_view_history_%d_%04d%02d",g_userpower.userID,sysTm.wYear,sysTm.wMonth);

		g_VMLog.WriteVmLog("UpdateStationCameraVideoInfoToDBTag---szTableName=%s",szTableName);

		CreateUserStationViewHistoryTable(szTableName);//建立历史查询表
		UpdateUserStationViewHistoryTable(szTableName);//导入全部摄像头

		sprintf_s(szTime,"%04d-%02d-%02d %02d:%02d:%02d",sysTm.wYear,sysTm.wMonth,sysTm.wDay,sysTm.wHour,sysTm.wMinute,sysTm.wSecond);

		int nType = 0;
		switch(pVideoInfo->operate_type)
		{
		case WM_DEVICE_VIDEO_OPERATE_RESULT_NOTAG_OFFLINE_VIDEO:
			nType = 0;
			break;

		case WM_DEVICE_VIDEO_OPERATE_RESULT_NOTAG_ONLINE_VIDEO:
			nType = 1;
			break;

		case WM_DEVICE_VIDEO_OPERATE_RESULT_TAG_ONLINE_VIDEO:
			nType = 2;
			break;

		case WM_DEVICE_VIDEO_OPERATE_RESULT_TAG_OFFLINE_VIDEO:
			nType = 3;
			break;

		default:
			nType = 0;
			break;
		}

		MYSQL_RES	* res ;
		MYSQL_ROW	row ;
		int nCount = 0;

		UpdateUserStationViewHistoryTableByStationId(szTableName,pVideoInfo->station_id);//导入指定站点的摄像头

		sprintf_s(sql_buf, "SELECT COUNT(camera_code) FROM %s WHERE day=%d AND camera_code='%s' AND station_id=%d ",
			szTableName,sysTm.wDay,pVideoInfo->camera_code,pVideoInfo->station_id);
		if (mysql_query(m_pMySql, sql_buf))
		{
			sprintf_s(sql_buf,sizeof(sql_buf),"INSERT INTO %s (station_id,camera_code,day,status,time) VALUES (%d,'%s',%d,%d,'%s') ",szTableName,pVideoInfo->station_id,pVideoInfo->camera_code,sysTm.wDay,nType,szTime);
			if (mysql_query(m_pMySql, sql_buf))
				return false;
		}
		else
		{
			res = mysql_store_result(m_pMySql);
			if(row = mysql_fetch_row(res))
			{
				nCount = atoi(row[0]);
			}
			mysql_free_result( res ) ;

			if (nCount != 0)
			{
				sprintf_s(sql_buf, "UPDATE %s SET time='%s',status=%d WHERE day=%d AND camera_code='%s' AND station_id=%d ",
					szTableName,szTime,nType,sysTm.wDay,pVideoInfo->camera_code,pVideoInfo->station_id);
				if (mysql_query(m_pMySql, sql_buf))
					return false;
			}
			else
			{
				sprintf_s(sql_buf,sizeof(sql_buf),"INSERT INTO %s (station_id,camera_code,day,status,time) VALUES (%d,'%s',%d,%d,'%s') ",szTableName,pVideoInfo->station_id,pVideoInfo->camera_code,sysTm.wDay,nType,szTime);
				if (mysql_query(m_pMySql, sql_buf))
					return false;
			}
		}
		return true;
	}
	catch(...)
	{

	}

	return false;
}

//更新用户巡视站点历史信息
bool CClientVideoInfo::UpdateStationCameraVideoInfoToDBTagDKY(void *pInfo,int nInfoLen)
{
	if (m_pMySql == NULL)
	{
		ConnectMySql();
	}

	if (m_pMySql == NULL)
		return false;

	if (pInfo == NULL||nInfoLen != sizeof(g_ThreadClientVideoInfoWriteDeNode))
		return false;

	Recv_Buf_Node_Type *pNodeInfo = (Recv_Buf_Node_Type *)pInfo;

	CLIENT_VIDEO_INFO *pVideoInfo  = (CLIENT_VIDEO_INFO *)pNodeInfo->Buf;
	int nVideoInfo = pNodeInfo->DataLen;

	if (nVideoInfo != sizeof(CLIENT_VIDEO_INFO))
		return false;

	if (g_userpower.userID < 0)
		return false;

	SYSTEMTIME sysTm;

	char sql_buf[1024] = {0};
	char szTableName[255] = {0};
	char szTime[64] = {0};

	memset(sql_buf,0,sizeof(sql_buf));
	memset(szTableName,0,sizeof(szTableName));

	try
	{
		::GetLocalTime(&sysTm);
		sprintf_s(szTableName,"ct_user_station_view_history_%d_%04d%02d",18,sysTm.wYear,sysTm.wMonth);

		g_VMLog.WriteVmLog("UpdateStationCameraVideoInfoToDBTag---szTableName=%s",szTableName);

		CreateUserStationViewHistoryTable(szTableName);//建立历史查询表
		UpdateUserStationViewHistoryTable(szTableName);//导入全部摄像头

		sprintf_s(szTime,"%04d-%02d-%02d %02d:%02d:%02d",sysTm.wYear,sysTm.wMonth,sysTm.wDay,sysTm.wHour,sysTm.wMinute,sysTm.wSecond);

		int nStatus = 0;
		int nType = 0;
		switch(pVideoInfo->operate_type)
		{
		case WM_DEVICE_VIDEO_OPERATE_RESULT_NOTAG_OFFLINE_VIDEO:
			nType = 0;
			break;

		case WM_DEVICE_VIDEO_OPERATE_RESULT_NOTAG_ONLINE_VIDEO:
			nType = 1;
			break;

		case WM_DEVICE_VIDEO_OPERATE_RESULT_TAG_ONLINE_VIDEO:
			nType = 2;
			break;

		case WM_DEVICE_VIDEO_OPERATE_RESULT_TAG_OFFLINE_VIDEO:
			nType = 3;
			break;

		default:
			nType = 0;
			break;
		}

		MYSQL_RES	* res ;
		MYSQL_ROW	row ;
		int nCount = 0;

		UpdateUserStationViewHistoryTableByStationId(szTableName,pVideoInfo->station_id);//导入指定站点的摄像头

		sprintf_s(sql_buf, "SELECT COUNT(camera_code) FROM %s WHERE day=%d AND camera_code='%s' AND station_id=%d ",
			szTableName,sysTm.wDay,pVideoInfo->camera_code,pVideoInfo->station_id);
		if (mysql_query(m_pMySql, sql_buf))
		{
			sprintf_s(sql_buf,sizeof(sql_buf),"INSERT INTO %s (station_id,camera_code,day,status,time) VALUES (%d,'%s',%d,%d,'%s') ",szTableName,pVideoInfo->station_id,pVideoInfo->camera_code,sysTm.wDay,nType,szTime);
			if (mysql_query(m_pMySql, sql_buf))
				return false;
		}
		else
		{
			res = mysql_store_result(m_pMySql);
			if(row = mysql_fetch_row(res))
			{
				nCount = atoi(row[0]);
			}
			mysql_free_result( res ) ;

			if (nCount != 0)
			{
				sprintf_s(sql_buf,"SELECT status FROM %s WHERE day=%d AND camera_code='%s' AND station_id=%d ",szTableName,sysTm.wDay,pVideoInfo->camera_code,pVideoInfo->station_id);
				if (!mysql_query(m_pMySql, sql_buf))
				{
					res = mysql_store_result(m_pMySql);
					if(row = mysql_fetch_row(res))
					{
						nStatus = atoi(row[0]);
					}
					mysql_free_result( res ) ;
					
					if (nType == 2||nType == 3)
					{
						if (nStatus == 0)
						{
							nType = 3;
						}
						else if (nStatus == 1)
						{
							nType = 2;
						}
						else
						{
							return true;
						}
					}
					else 
					{
						if (nStatus == 2)
						{
							nType = 1;
						}
						else if (nStatus == 3)
						{
							nType = 0;
						}
						else
						{
							return true;
						}
					}
				}

				sprintf_s(sql_buf, "UPDATE %s SET time='%s',status=%d WHERE day=%d AND camera_code='%s' AND station_id=%d ",
					szTableName,szTime,nType,sysTm.wDay,pVideoInfo->camera_code,pVideoInfo->station_id);
				if (mysql_query(m_pMySql, sql_buf))
					return false;
			}
			else
			{
				sprintf_s(sql_buf,sizeof(sql_buf),"INSERT INTO %s (station_id,camera_code,day,status,time) VALUES (%d,'%s',%d,%d,'%s') ",szTableName,pVideoInfo->station_id,pVideoInfo->camera_code,sysTm.wDay,nType,szTime);
				if (mysql_query(m_pMySql, sql_buf))
					return false;
			}
		}
		return true;
	}
	catch(...)
	{

	}

	return false;
}


bool CClientVideoInfo::WriteClientVideoInfoFromInfoRecvQueue()
{
	DWORD dwCount = 0;

	EnterCriticalSection(&m_csClientVideoInfoLock);

	try
	{
		do 
		{
			if (RecvQueueEmpty(g_ClientVideoInfoRecvBufQueue))
			{
				break;
			}

			if(RecvDeQueue(g_ClientVideoInfoRecvBufQueue,g_ThreadClientVideoInfoWriteDeNode) > 0)
			{
				g_VMLog.WriteVmLog("WriteClientVideoInfoFromInfoRecvQueue---------------DataLen=%d",g_ThreadClientVideoInfoWriteDeNode.DataLen);
				WriteClientVideoInfoToDB((void*)&g_ThreadClientVideoInfoWriteDeNode,sizeof(g_ThreadClientVideoInfoWriteDeNode));
				UpdateStationCameraVideoInfoToDB((void*)&g_ThreadClientVideoInfoWriteDeNode,sizeof(g_ThreadClientVideoInfoWriteDeNode));
				UpdateStationCameraVideoInfoToDBTag((void*)&g_ThreadClientVideoInfoWriteDeNode,sizeof(g_ThreadClientVideoInfoWriteDeNode));
				
				if (g_userpower.userID != 18)
				{
					UpdateStationCameraVideoInfoToDBTag((void*)&g_ThreadClientVideoInfoWriteDeNode,sizeof(g_ThreadClientVideoInfoWriteDeNode));
				}
			}

		} while (true);
	}
	catch(...)
	{

	}

	LeaveCriticalSection(&m_csClientVideoInfoLock);

	return true;
}

unsigned __stdcall _ClientVideoInfoWriteThread(void *arg)
{
	if (arg == NULL)
		return -1;

	CClientVideoInfo *pClientVideoInfo = (CClientVideoInfo *)arg;

	try
	{
		while (1)
		{
			if (g_bClientVideoInfoWriteThreadExitFlag == true)
			{
				return 0;
			}

			pClientVideoInfo->WriteClientVideoInfoFromInfoRecvQueue();

			Sleep(200);
		}
	}
	catch(...)
	{

	}

	return 0;
}




