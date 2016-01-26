#include "StdAfx.h"
#include "CameraStatusInfo.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"

//////////////////////////////////////////////////////////////////////////
// 数据库连接
#pragma comment(lib,"lib\\libmySQL.lib")

//////////////////////////////////////////////////////////////////////////
CCameraStatusInfo g_CameraStatusInfo;
Recv_Buf_Queue g_CameraStatusInfoRecvBufQueue;
volatile bool g_bCameraStatusInfoWriteThreadExitFlag = false;
Recv_Buf_Node_Type g_CameraStatusInfoWritEnNode;
Recv_Buf_Node_Type g_ThreadCameraStatusInfoWriteDeNode;
//////////////////////////////////////////////////////////////////////////

CCameraStatusInfo::CCameraStatusInfo(void)
{
	m_bCameraStatusInfoInitFlag = false;
	m_hCameraStatusInfoThread = NULL;
	m_uCameraStatusInfoThreadID = 0;
	memset(m_szClientIp,0,sizeof(m_szClientIp));
	memset(m_szDBServerIp,0,sizeof(m_szDBServerIp));
	m_nDBServerPort = 0;
	memset(m_szDBName,0,sizeof(m_szDBName));
	memset(m_szDBUserName,0,sizeof(m_szDBUserName));
	memset(m_szDBUserPassword,0,sizeof(m_szDBUserPassword));
	m_pMySql = NULL;
	InitCameraStatusInfo();
}

CCameraStatusInfo::~CCameraStatusInfo(void)
{
	StopCameraStatusInfo();
	UnInitCameraStatusInfo();
	m_bCameraStatusInfoInitFlag = false;
	m_hCameraStatusInfoThread = NULL;
	m_uCameraStatusInfoThreadID = 0;
	memset(m_szClientIp,0,sizeof(m_szClientIp));
	memset(m_szDBServerIp,0,sizeof(m_szDBServerIp));
	m_nDBServerPort = 0;
	memset(m_szDBName,0,sizeof(m_szDBName));
	memset(m_szDBUserName,0,sizeof(m_szDBUserName));
	memset(m_szDBUserPassword,0,sizeof(m_szDBUserPassword));
	m_pMySql = NULL;
}

bool CCameraStatusInfo::InitCameraStatusInfo()
{
	if (m_bCameraStatusInfoInitFlag)
		return true;

	InitializeCriticalSection(&m_csCameraStatusInfoLock);

	InitRecvQueue(g_CameraStatusInfoRecvBufQueue);

	memset(&g_CameraStatusInfoWritEnNode,0,sizeof(g_CameraStatusInfoWritEnNode));
	memset(&g_ThreadCameraStatusInfoWriteDeNode,0,sizeof(g_ThreadCameraStatusInfoWriteDeNode));

	m_bCameraStatusInfoInitFlag = true;

	return true;

}

bool CCameraStatusInfo::StartCameraStatusInfo()
{
	g_bCameraStatusInfoWriteThreadExitFlag = false;

	m_hCameraStatusInfoThread = (HANDLE)_beginthreadex(NULL, 0,_CameraStatusInfoWriteThread,(void*)this, 0, &m_uCameraStatusInfoThreadID);
	if (m_hCameraStatusInfoThread == NULL)
		return false;
	return true;
}

bool CCameraStatusInfo::StopCameraStatusInfo()
{
	g_bCameraStatusInfoWriteThreadExitFlag = true;

	DWORD dwResult = WaitForSingleObject(m_hCameraStatusInfoThread,6000);
	if (dwResult != WAIT_OBJECT_0)
	{
		TerminateThread(m_hCameraStatusInfoThread,1);
	}

	m_hCameraStatusInfoThread = NULL;
	m_uCameraStatusInfoThreadID = 0;

	DisConnectMySql();

	return true;
}

bool CCameraStatusInfo::UnInitCameraStatusInfo()
{
	if (!m_bCameraStatusInfoInitFlag)
		return true;

	EnterCriticalSection(&m_csCameraStatusInfoLock);

	ClearRecvQueue(g_CameraStatusInfoRecvBufQueue);

	LeaveCriticalSection(&m_csCameraStatusInfoLock);
	DeleteCriticalSection(&m_csCameraStatusInfoLock);

	m_bCameraStatusInfoInitFlag = false;

	return true;
}

bool CCameraStatusInfo::WriteCameraStatusInfo(CAMERA_STATUS_INFO *pCameraStatusInfo)
{
	if (pCameraStatusInfo == NULL)
		return false;

	g_CameraStatusInfoWritEnNode.DataLen = sizeof(CAMERA_STATUS_INFO);
	memcpy(g_CameraStatusInfoWritEnNode.Buf,pCameraStatusInfo,sizeof(CAMERA_STATUS_INFO));

	EnterCriticalSection(&m_csCameraStatusInfoLock);
	RecvEnQueue(g_CameraStatusInfoRecvBufQueue,g_CameraStatusInfoWritEnNode);
	LeaveCriticalSection(&m_csCameraStatusInfoLock);

	return true;
}

// 连接数据库
bool CCameraStatusInfo::ConnectMySql(void)
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
bool CCameraStatusInfo::DisConnectMySql(void)
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

bool CCameraStatusInfo::Lock()
{
	EnterCriticalSection(&m_csCameraStatusInfoLock);
	return true;
}

bool CCameraStatusInfo::Unlock()
{
	LeaveCriticalSection(&m_csCameraStatusInfoLock);
	return true;
}

//成员变量赋值
bool CCameraStatusInfo::SetCameraStatusInfo(char *pClientIp,char *pDBServerIp,int nDBServerPort,char *pDBName,char *pDBUserName,char *pDBUserPassword)
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

bool CCameraStatusInfo::CreateUserStationViewHistoryTable(char *szUserStationViewHistoryTableName)
{
	char sql_buf[1024] = {0};
	int    nResult = 0;

	try
	{
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

bool CCameraStatusInfo::UpdateUserStationViewHistoryTable(char *szUserStationViewHistoryTableName)
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

bool CCameraStatusInfo::UpdateUserStationViewHistoryTableByStationId(char *szUserStationViewHistoryTableName,int nStationId)
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

//更新站点摄像头状态
bool CCameraStatusInfo::WriteCameraStatusInfoToDB(void *pInfo,int nInfoLen)
{
	if (m_pMySql == NULL)
	{
		ConnectMySql();
	}

	if (m_pMySql == NULL)
		return false;

	if (pInfo == NULL||nInfoLen != sizeof(g_ThreadCameraStatusInfoWriteDeNode))
		return false;

	Recv_Buf_Node_Type *pNodeInfo = (Recv_Buf_Node_Type *)pInfo;

	CAMERA_STATUS_INFO *pCamerStatusInfo  = (CAMERA_STATUS_INFO *)pNodeInfo->Buf;
	int nCamerStatusInfo = pNodeInfo->DataLen;

	if (nCamerStatusInfo != sizeof(CAMERA_STATUS_INFO))
		return false;

	char sql_buf[1024] = {0};
	char szTableName[255] = {0};
	int    nCameraStatus = pCamerStatusInfo->camera_status;

	memset(sql_buf,0,sizeof(sql_buf));
	memset(szTableName,0,sizeof(szTableName));

	if (nCameraStatus != 0&&nCameraStatus != 1)
		return false;

	try
	{
		if (pCamerStatusInfo->type == 0)//更新摄像头状态
		{
			sprintf_s(szTableName,"ob_d5000_camera_station_%d",pCamerStatusInfo->station_id);

			sprintf_s(sql_buf, "UPDATE %s SET camera_status=%d WHERE camera_code='%s' ",
				szTableName,
				nCameraStatus,
				pCamerStatusInfo->camera_code);

			if (mysql_query(m_pMySql, sql_buf))
			{
				return false;
			}
		}
		else if (pCamerStatusInfo->type == 1)//更新站点状态
		{
			sprintf_s(sql_buf, "UPDATE ob_d5000_station SET notes='%d' WHERE station_id='%d' ",
				nCameraStatus,pCamerStatusInfo->station_id);

			if (mysql_query(m_pMySql, sql_buf))
			{
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

//更新用户巡视站点摄像头状态
bool CCameraStatusInfo::WriteCameraStatusInfoToDB2(void *pInfo,int nInfoLen)
{
	if (m_pMySql == NULL)
	{
		ConnectMySql();
	}

	if (m_pMySql == NULL)
		return false;

	if (pInfo == NULL||nInfoLen != sizeof(g_ThreadCameraStatusInfoWriteDeNode))
		return false;

	Recv_Buf_Node_Type *pNodeInfo = (Recv_Buf_Node_Type *)pInfo;

	CAMERA_STATUS_INFO *pCamerStatusInfo  = (CAMERA_STATUS_INFO *)pNodeInfo->Buf;
	int nCamerStatusInfo = pNodeInfo->DataLen;

	if (nCamerStatusInfo != sizeof(CAMERA_STATUS_INFO))
		return false;

	if (pCamerStatusInfo->type != 0)//当为变电站状态返回
		return false;

	char sql_buf[1024] = {0};
	char szTableName[255] = {0};
	char szTime[64] = {0};
	int    nCameraStatus = pCamerStatusInfo->camera_status;

	memset(sql_buf,0,sizeof(sql_buf));
	memset(szTableName,0,sizeof(szTableName));

	SYSTEMTIME sysTm;
	
	try
	{
		GetLocalTime(&sysTm);

		sprintf_s(szTableName,"ct_user_station_view_history_%d_%04d%02d",g_userpower.userID,sysTm.wYear,sysTm.wMonth);

		CreateUserStationViewHistoryTable(szTableName);//建立历史查询表
		UpdateUserStationViewHistoryTable(szTableName);//导入全部摄像头

		sprintf_s(szTime,"%04d-%02d-%02d %02d:%02d:%02d",sysTm.wYear,sysTm.wMonth,sysTm.wDay,sysTm.wHour,sysTm.wMinute,sysTm.wSecond);

		UpdateUserStationViewHistoryTableByStationId(szTableName,pCamerStatusInfo->station_id);//导入指定站点的摄像头

		MYSQL_RES	*res ;
		MYSQL_ROW	row ;
		int nCount = 0;

		sprintf_s(sql_buf, "SELECT COUNT(camera_code) FROM %s WHERE day=%d AND camera_code='%s' AND station_id=%d ",
			szTableName,sysTm.wDay,pCamerStatusInfo->camera_code,pCamerStatusInfo->station_id);
		if (mysql_query(m_pMySql, sql_buf))
		{
			sprintf_s(sql_buf,sizeof(sql_buf),"INSERT INTO %s (station_id,camera_code,day,status,time) VALUES (%d,'%s',%d,%d,'%s') ",szTableName,pCamerStatusInfo->station_id,pCamerStatusInfo->camera_code,sysTm.wDay,pCamerStatusInfo->camera_status,szTime);
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
					szTableName,szTime,pCamerStatusInfo->camera_status,sysTm.wDay,pCamerStatusInfo->camera_code,pCamerStatusInfo->station_id);
				if (mysql_query(m_pMySql, sql_buf))
					return false;
			}
			else
			{
				sprintf_s(sql_buf,sizeof(sql_buf),"INSERT INTO %s (station_id,camera_code,day,status,time) VALUES (%d,'%s',%d,%d,'%s') ",szTableName,pCamerStatusInfo->station_id,pCamerStatusInfo->camera_code,sysTm.wDay,pCamerStatusInfo->camera_status,szTime);
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

bool CCameraStatusInfo::WriteCameraStatusInfoFromInfoRecvQueue()
{
	DWORD dwCount = 0;

	EnterCriticalSection(&m_csCameraStatusInfoLock);

	try
	{
		do 
		{
			if (RecvQueueEmpty(g_CameraStatusInfoRecvBufQueue))
			{
				break;
			}

			if(RecvDeQueue(g_CameraStatusInfoRecvBufQueue,g_ThreadCameraStatusInfoWriteDeNode) > 0)
			{
				WriteCameraStatusInfoToDB((void*)&g_ThreadCameraStatusInfoWriteDeNode,sizeof(g_ThreadCameraStatusInfoWriteDeNode));
				WriteCameraStatusInfoToDB2((void*)&g_ThreadCameraStatusInfoWriteDeNode,sizeof(g_ThreadCameraStatusInfoWriteDeNode));
			}

		} while (true);
	}
	catch(...)
	{

	}

	LeaveCriticalSection(&m_csCameraStatusInfoLock);

	return true;
}

unsigned __stdcall _CameraStatusInfoWriteThread(void *arg)
{
	if (arg == NULL)
		return -1;

	CCameraStatusInfo *pCameraStatusInfo = (CCameraStatusInfo *)arg;

	try
	{
		while (1)
		{
			if (g_bCameraStatusInfoWriteThreadExitFlag == true)
			{
				return 0;
			}

			pCameraStatusInfo->WriteCameraStatusInfoFromInfoRecvQueue();

			Sleep(200);
		}
	}
	catch(...)
	{

	}

	return 0;
}




