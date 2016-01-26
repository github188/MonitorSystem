#include "StdAfx.h"
#include "VMHistoryLog.h"

//////////////////////////////////////////////////////////////////////////
CVMHistoryLog g_VMHistoryLog;

//////////////////////////////////////////////////////////////////////////
CVMHistoryLog::CVMHistoryLog(void)
{
	InitializeCriticalSection(&m_cs);
	m_pMySql = NULL;
	m_nDBServerPort = 0;
	memset(m_szLogInUserName,0,sizeof(m_szLogInUserName));
	memset(m_szDBServerIP,0,sizeof(m_szDBServerIP));
	memset(m_szDBName,0,sizeof(m_szDBName));
	memset(m_szDBUserName,0,sizeof(m_szDBUserName));
	memset(m_szDBUserPassword,0,sizeof(m_szDBUserPassword));
}

CVMHistoryLog::~CVMHistoryLog(void)
{
	DisConnectMySql();

	EnterCriticalSection(&m_cs);
	m_pMySql = NULL;
	m_nDBServerPort = 0;
	memset(m_szLogInUserName,0,sizeof(m_szLogInUserName));
	memset(m_szDBServerIP,0,sizeof(m_szDBServerIP));
	memset(m_szDBName,0,sizeof(m_szDBName));
	memset(m_szDBUserName,0,sizeof(m_szDBUserName));
	memset(m_szDBUserPassword,0,sizeof(m_szDBUserPassword));
	LeaveCriticalSection(&m_cs);
	DeleteCriticalSection(&m_cs);
}

BOOL CVMHistoryLog::Lock()
{
	EnterCriticalSection(&m_cs);
	return TRUE;
}

BOOL CVMHistoryLog::Unlock()
{
	LeaveCriticalSection(&m_cs);
	return TRUE;
}

//成员变量赋值
BOOL CVMHistoryLog::SetVMHistoryLogInfo(char *pLogInUserName,char *pDBServerIP,int nDBServerPort,char *pDBName,char *pUserName,char *pUserPassword)
{
	if (pLogInUserName != NULL)
	{
		strcpy_s(m_szLogInUserName,sizeof(m_szLogInUserName),pLogInUserName);
	}

	if (pDBServerIP != NULL)
	{
		strcpy_s(m_szDBServerIP,sizeof(m_szDBServerIP),pDBServerIP);
	}

	if (nDBServerPort > 0)
	{
		m_nDBServerPort = nDBServerPort;
	}

	if (pDBName != NULL)
	{
		strcpy_s(m_szDBName,sizeof(m_szDBName),pDBName);
	}

	if (pUserName != NULL)
	{
		strcpy_s(m_szDBUserName,sizeof(m_szDBUserName),pUserName);
	}

	if (pUserPassword != NULL)
	{
		strcpy_s(m_szDBUserPassword,sizeof(m_szDBUserPassword),pUserPassword);
	}

	return TRUE;
}

// 连接数据库
BOOL CVMHistoryLog::ConnectMySql(void)
{
	Lock();

	if (strlen(m_szDBServerIP) == 0||m_nDBServerPort == 0
		||strlen(m_szDBName) == 0||strlen(m_szDBUserName) == 0
		||strlen(m_szDBUserPassword) == 0)
	{
		Unlock();
		return FALSE;
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

			if (mysql_real_connect(m_pMySql, m_szDBServerIP, m_szDBUserName, m_szDBUserPassword, m_szDBName, m_nDBServerPort, NULL, 0))
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
				return TRUE;
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
	} while (FALSE);

	Unlock();
	return FALSE;
}

// 断开数据库
BOOL CVMHistoryLog::DisConnectMySql(void)
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
		return TRUE;
	}
	catch(...)
	{

	}
	Unlock();
	return FALSE;
}

BOOL CVMHistoryLog::CreateLinkageHistoryLogTable(char *szHistoryLogTableName)
{
	char sql_buf[1024] = {0};
	int    nResult = 0;
	BOOL bResult = FALSE;

	sprintf_s(sql_buf,"CREATE TABLE IF NOT EXISTS %s ("
		"id int(10) NOT NULL default '0',"
		"linkage_type int(10) default '0',"
		"station_name char(255) default '' ,"
		"device_name char(255) default '',"
		"sub_type char(64) default '',"
		"state char(128) default '',"
		"screen_id int(10) default '0',"
		"linkage_time char(64) default '',"
		"content char(255) default '',"
		"value1 char(64) default '',"
		"value2 char(64) default '',"
		"linkage_station_name char(255) default '',"
		"linkage_num int(10) default '0' ,"
		"PRIMARY KEY (id))",
		szHistoryLogTableName
		);

	nResult = mysql_query(m_pMySql, sql_buf);

	bResult = ((nResult == 0)?TRUE:FALSE);

	return bResult;
}

BOOL CVMHistoryLog::CreateLinkageCameraHistoryLogTable(char *szHistoryLogTableName)
{
	char sql_buf[1024] = {0};
	int    nResult = 0;
	BOOL bResult = FALSE;

	sprintf_s(sql_buf,"CREATE TABLE IF NOT EXISTS %s ("
		"id int(10) NOT NULL auto_increment,"
		"name char(255) default '' ,"
		"code char(64) default '',"
		"decode_tag int(10) default '0',"
		"preset_name char(255) default '',"
		"preset_id int(10) default '0',"
		"from_x1 int(10) default '0',"
		"from_y1 int(10) default '0',"
		"to_x1 int(10) default '0',"
		"to_y1 int(10) default '0',"
		"from_x2 int(10) default '0',"
		"from_y2 int(10) default '0',"
		"to_x2 int(10) default '0',"
		"to_y2 int(10) default '0',"
		"node_id int(10) default '0',"
		"PRIMARY KEY (id))",
		szHistoryLogTableName
		);

	nResult = mysql_query(m_pMySql, sql_buf);

	bResult = ((nResult == 0)?TRUE:FALSE);

	return bResult;
}

BOOL CVMHistoryLog::WriteManualLinkageLog(VIDEO_LINKAGE_INFO *pVideoManualLinkageInfo)
{
	if(pVideoManualLinkageInfo == NULL)
		return FALSE;

	int nNodeId = 0;
	int nCameraNum = 0;
	char sql_buf[1024] = {0};
	char szTableName[255] = {0};

	MYSQL_RES * res = NULL;
	MYSQL_ROW	row ;
	SYSTEMTIME sysTm;
	VIDEO_LINKAGE_CAMERA_INFO *pCameraInfo = NULL;

	memset(sql_buf,0,sizeof(sql_buf));
	memset(szTableName,0,sizeof(szTableName));

	Lock();

	try
	{
		::GetLocalTime(&sysTm);
		sprintf_s(szTableName,"ct_linkage_history_log_%s_%04d",m_szLogInUserName,sysTm.wYear);

		//检查表是否存在,不存在则创建
		CreateLinkageHistoryLogTable(szTableName);

		sprintf_s(sql_buf, "SELECT id FROM %s ORDER BY id DESC LIMIT 0,1",szTableName);

		if (!mysql_query(m_pMySql, sql_buf))
		{
			res = mysql_store_result(m_pMySql);
			while ( row = mysql_fetch_row( res ) )
			{
				nNodeId = atoi(row[0]);
			}
			mysql_free_result( res ) ;
		}

		nNodeId++;

		sprintf_s(sql_buf, "INSERT INTO %s (id,linkage_type,station_name,device_name,sub_type,screen_id,linkage_time,linkage_station_name,linkage_num) VALUES (%d,%d,'%s','%s','%d',%d,'%s','%s',%d)",
			           szTableName,
					   nNodeId,
					   VIDEO_LINKAGE_INFO_TYPE,
					   pVideoManualLinkageInfo->szStationName,
					   pVideoManualLinkageInfo->szDeviceName,
					   pVideoManualLinkageInfo->nType,
					   pVideoManualLinkageInfo->nScreenId,
					   pVideoManualLinkageInfo->szTime,
					   pVideoManualLinkageInfo->szLinkageStationName,
					   pVideoManualLinkageInfo->nLinkNum);

		if (mysql_query(m_pMySql, sql_buf))
		{
			Unlock();
			return FALSE;
		}

		nCameraNum = pVideoManualLinkageInfo->nLinkNum;
		pCameraInfo = pVideoManualLinkageInfo->pLinkageCameraInfo;
		if (nCameraNum < 0||pCameraInfo == NULL)
		{
			Unlock();
			return FALSE;
		}

		sprintf_s(szTableName,"ct_linkage_camera_history_log_%s_%04d",m_szLogInUserName,sysTm.wYear);
		
		//检查表是否存在,不存在则创建
		CreateLinkageCameraHistoryLogTable(szTableName);

		for (int i = 0;i < nCameraNum;i++)
		{
			sprintf_s(sql_buf, "INSERT INTO %s (name,code,decode_tag,preset_name,preset_id,from_x1,from_y1,to_x1,to_y1,from_x2,from_y2,to_x2,to_y2,node_id) VALUES ('%s','%s',%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				szTableName,
				pCameraInfo->szName,
				pCameraInfo->szCode,
				pCameraInfo->nDecodeTag,
				pCameraInfo->szPresetName,
				pCameraInfo->nPresetId,
				pCameraInfo->nFromX1,
				pCameraInfo->nFromY1,
				pCameraInfo->nToX1,
				pCameraInfo->nToY1,
				pCameraInfo->nFromX2,
				pCameraInfo->nFromY2,
				pCameraInfo->nToX2,
				pCameraInfo->nToY2,
				nNodeId);

			mysql_query(m_pMySql, sql_buf);

			pCameraInfo++;
		}

		pCameraInfo = NULL;

		Unlock();
		return TRUE;
	}
	catch(...)
	{

	}

	Unlock();
	return FALSE;
}

BOOL CVMHistoryLog::WriteAlarmLinkageLog(VIDEO_ALARM_LINKAGE_INFO *pVideoAlarmLinkageInfo)
{
	if(pVideoAlarmLinkageInfo == NULL)
		return FALSE;

	int nNodeId = 0;
	int nCameraNum = 0;
	char sql_buf[1024] = {0};
	char szTableName[255] = {0};

	MYSQL_RES * res = NULL;
	MYSQL_ROW	row ;
	SYSTEMTIME sysTm;
	VIDEO_LINKAGE_CAMERA_INFO *pCameraInfo = NULL;

	memset(sql_buf,0,sizeof(sql_buf));
	memset(szTableName,0,sizeof(szTableName));

	Lock();

	try
	{
		::GetLocalTime(&sysTm);
		sprintf_s(szTableName,"ct_linkage_history_log_%s_%04d",m_szLogInUserName,sysTm.wYear);
		
		//检查表是否存在,不存在则创建
		CreateLinkageHistoryLogTable(szTableName);
		
		sprintf_s(sql_buf, "SELECT id FROM %s ORDER BY id DESC LIMIT 0,1",szTableName);

		if (!mysql_query(m_pMySql, sql_buf))
		{
			res = mysql_store_result(m_pMySql);
			while ( row = mysql_fetch_row( res ) )
			{
				nNodeId = atoi(row[0]);
			}
			mysql_free_result( res ) ;
		}

		nNodeId++;

		sprintf_s(sql_buf, "INSERT INTO %s (id,linkage_type,station_name,device_name,sub_type,linkage_time,content,linkage_station_name,linkage_num) VALUES (%d,%d,'%s','%s','%s','%s','%s','%s',%d)",
			szTableName,
			nNodeId,
			VIDEO_ALARM_LINKAGE_INFO_TYPE,
			pVideoAlarmLinkageInfo->szStationName,
			pVideoAlarmLinkageInfo->szDeviceName,
			pVideoAlarmLinkageInfo->szAlarmType,
			pVideoAlarmLinkageInfo->szTime,
			pVideoAlarmLinkageInfo->szContent,
			pVideoAlarmLinkageInfo->szLinkageStationName,
			pVideoAlarmLinkageInfo->nLinkNum);

		if (mysql_query(m_pMySql, sql_buf))
		{
			Unlock();
			return FALSE;
		}

		nCameraNum = pVideoAlarmLinkageInfo->nLinkNum;
		pCameraInfo = pVideoAlarmLinkageInfo->pLinkageCameraInfo;
		if (nCameraNum < 0||pCameraInfo == NULL)
		{
			Unlock();
			return FALSE;
		}

		sprintf_s(szTableName,"ct_linkage_camera_history_log_%s_%04d",m_szLogInUserName,sysTm.wYear);

		//检查表是否存在,不存在则创建
		CreateLinkageCameraHistoryLogTable(szTableName);

		for (int i = 0;i < nCameraNum;i++)
		{
			sprintf_s(sql_buf, "INSERT INTO %s (name,code,decode_tag,preset_name,preset_id,from_x1,from_y1,to_x1,to_y1,from_x2,from_y2,to_x2,to_y2,node_id) VALUES ('%s','%s',%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				szTableName,
				pCameraInfo->szName,
				pCameraInfo->szCode,
				pCameraInfo->nDecodeTag,
				pCameraInfo->szPresetName,
				pCameraInfo->nPresetId,
				pCameraInfo->nFromX1,
				pCameraInfo->nFromY1,
				pCameraInfo->nToX1,
				pCameraInfo->nToY1,
				pCameraInfo->nFromX2,
				pCameraInfo->nFromY2,
				pCameraInfo->nToX2,
				pCameraInfo->nToY2,
				nNodeId);

			mysql_query(m_pMySql, sql_buf);

			pCameraInfo++;
		}

		pCameraInfo = NULL;

		Unlock();
		return TRUE;
	}
	catch(...)
	{

	}

	Unlock();
	return FALSE;
}

BOOL CVMHistoryLog::WriteStateLinkageLog(VIDEO_STATE_LINKAGE_INFO *pVideoStateLinkageInfo)
{
	if(pVideoStateLinkageInfo == NULL)
		return FALSE;

	int nNodeId = 0;
	int nCameraNum = 0;
	char sql_buf[1024] = {0};
	char szTableName[255] = {0};

	MYSQL_RES * res = NULL;
	MYSQL_ROW	row ;
	SYSTEMTIME sysTm;
	VIDEO_LINKAGE_CAMERA_INFO *pCameraInfo = NULL;

	memset(sql_buf,0,sizeof(sql_buf));
	memset(szTableName,0,sizeof(szTableName));

	Lock();

	try
	{
		::GetLocalTime(&sysTm);
		sprintf_s(szTableName,"ct_linkage_history_log_%s_%04d",m_szLogInUserName,sysTm.wYear);
		
		//检查表是否存在,不存在则创建
		CreateLinkageHistoryLogTable(szTableName);

		sprintf_s(sql_buf, "SELECT id FROM %s ORDER BY id DESC LIMIT 0,1",szTableName);

		if (!mysql_query(m_pMySql, sql_buf))
		{
			res = mysql_store_result(m_pMySql);
			while ( row = mysql_fetch_row( res ) )
			{
				nNodeId = atoi(row[0]);
			}
			mysql_free_result( res ) ;
		}

		nNodeId++;

		sprintf_s(sql_buf, "INSERT INTO %s (id,linkage_type,station_name,device_name,sub_type,state,linkage_time,content,value1,value2,linkage_station_name,linkage_num) VALUES (%d,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s',%d)",
			szTableName,
			nNodeId,
			VIDEO_STATE_LINKAGE_INFO_TYPE,
			pVideoStateLinkageInfo->szStationName,
			pVideoStateLinkageInfo->szDeviceName,
			pVideoStateLinkageInfo->szDeviceType,
			pVideoStateLinkageInfo->szState,
			pVideoStateLinkageInfo->szTime,
			pVideoStateLinkageInfo->szContent,
			pVideoStateLinkageInfo->szValue1,
			pVideoStateLinkageInfo->szValue2,
			pVideoStateLinkageInfo->szLinkageStationName,
			pVideoStateLinkageInfo->nLinkNum);

		if (mysql_query(m_pMySql, sql_buf))
		{
			Unlock();
			return FALSE;
		}

		nCameraNum = pVideoStateLinkageInfo->nLinkNum;
		pCameraInfo = pVideoStateLinkageInfo->pLinkageCameraInfo;
		if (nCameraNum < 0||pCameraInfo == NULL)
		{
			Unlock();
			return FALSE;
		}

		sprintf_s(szTableName,"ct_linkage_camera_history_log_%s_%04d",m_szLogInUserName,sysTm.wYear);
		
		//检查表是否存在,不存在则创建
		CreateLinkageCameraHistoryLogTable(szTableName);

		for (int i = 0;i < nCameraNum;i++)
		{
			sprintf_s(sql_buf, "INSERT INTO %s (name,code,decode_tag,preset_name,preset_id,from_x1,from_y1,to_x1,to_y1,from_x2,from_y2,to_x2,to_y2,node_id) VALUES ('%s','%s',%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				szTableName,
				pCameraInfo->szName,
				pCameraInfo->szCode,
				pCameraInfo->nDecodeTag,
				pCameraInfo->szPresetName,
				pCameraInfo->nPresetId,
				pCameraInfo->nFromX1,
				pCameraInfo->nFromY1,
				pCameraInfo->nToX1,
				pCameraInfo->nToY1,
				pCameraInfo->nFromX2,
				pCameraInfo->nFromY2,
				pCameraInfo->nToX2,
				pCameraInfo->nToY2,
				nNodeId);

			mysql_query(m_pMySql, sql_buf);

			pCameraInfo++;
		}

		pCameraInfo = NULL;

		Unlock();
		return TRUE;
	}
	catch(...)
	{

	}

	Unlock();
	return FALSE;
}

BOOL CVMHistoryLog::WriteCameraControlNotifyLog(VIDEO_CAMERA_CONTROL_NOTIFY_INFO *pVideoCameraControlNotifyInfo)
{
	if(pVideoCameraControlNotifyInfo == NULL)
		return FALSE;

	char sql_buf[1024] = {0};
	char szTableName[255] = {0};

	MYSQL_RES * res = NULL;
	MYSQL_ROW	row ;
	SYSTEMTIME sysTm;

	int nNodeId = 0;
	char szTime[64] = {0};
	
	memset(sql_buf,0,sizeof(sql_buf));
	memset(szTableName,0,sizeof(szTableName));

	Lock();

	try
	{
		::GetLocalTime(&sysTm);
		sprintf_s(szTableName,"ct_linkage_history_log_%s_%04d",m_szLogInUserName,sysTm.wYear);

		//检查表是否存在,不存在则创建
		CreateLinkageHistoryLogTable(szTableName);
		
		sprintf_s(sql_buf, "SELECT id FROM %s ORDER BY id DESC LIMIT 0,1",szTableName);

		if (!mysql_query(m_pMySql, sql_buf))
		{
			res = mysql_store_result(m_pMySql);
			while ( row = mysql_fetch_row( res ) )
			{
				nNodeId = atoi(row[0]);
			}
			mysql_free_result( res ) ;
		}

		nNodeId++;

		sprintf_s(szTime,sizeof(szTime),"%04d-%02d-%02dT%02d:%02d:%02dZ",sysTm.wYear,sysTm.wMonth,sysTm.wDay,sysTm.wHour,sysTm.wMinute,sysTm.wSecond);

		sprintf_s(sql_buf, "INSERT INTO %s (id,linkage_type,station_name,device_name,sub_type,linkage_time,value1,value2,linkage_station_name,linkage_num) VALUES (%d,%d,'%s','%s','%s','%s','%s','%s','%s',%d)",
			szTableName,
			nNodeId,
			VIDEO_CAMERA_CONTROL_NOTIFY_INFO_TYPE,
			pVideoCameraControlNotifyInfo->szStationName,
			pVideoCameraControlNotifyInfo->szDeviceName,
			"云镜控制通知",
			szTime,
			pVideoCameraControlNotifyInfo->szDeviceCode,
			pVideoCameraControlNotifyInfo->szUserName,
			pVideoCameraControlNotifyInfo->szStationName,
			0);

		if (mysql_query(m_pMySql, sql_buf))
		{
			Unlock();
			return FALSE;
		}

		Unlock();
		return TRUE;
	}
	catch(...)
	{

	}

	Unlock();
	return FALSE;
}

BOOL CVMHistoryLog::WriteWeatherWarningLinkageLog(VIDEO_WEATHER_WARNING_INFO *pVideoWeatherWarningLinkageInfo)
{
	if(pVideoWeatherWarningLinkageInfo == NULL)
		return FALSE;

	int nNodeId = 0;
	int nCameraNum = 0;
	char sql_buf[1024] = {0};
	char szTableName[255] = {0};

	MYSQL_RES * res = NULL;
	MYSQL_ROW	row ;
	SYSTEMTIME sysTm;
	VIDEO_LINKAGE_CAMERA_INFO *pCameraInfo = NULL;

	memset(sql_buf,0,sizeof(sql_buf));
	memset(szTableName,0,sizeof(szTableName));

	Lock();

	try
	{
		::GetLocalTime(&sysTm);
		sprintf_s(szTableName,"ct_linkage_history_log_%s_%04d",m_szLogInUserName,sysTm.wYear);

		//检查表是否存在,不存在则创建
		CreateLinkageHistoryLogTable(szTableName);

		sprintf_s(sql_buf, "SELECT id FROM %s ORDER BY id DESC LIMIT 0,1",szTableName);

		if (!mysql_query(m_pMySql, sql_buf))
		{
			res = mysql_store_result(m_pMySql);
			while ( row = mysql_fetch_row( res ) )
			{
				nNodeId = atoi(row[0]);
			}
			mysql_free_result( res ) ;
		}

		nNodeId++;

		sprintf_s(sql_buf, "INSERT INTO %s (id,linkage_type,station_name,device_name,sub_type,state,linkage_time,content,value1,value2,linkage_station_name,linkage_num) VALUES (%d,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s',%d)",
			szTableName,
			nNodeId,
			VIDEO_WEATHER_WARNING_INFO_TYPE,
			pVideoWeatherWarningLinkageInfo->szStationID,
			pVideoWeatherWarningLinkageInfo->szLinkageStationName,
			pVideoWeatherWarningLinkageInfo->szWeatherType,
			"",
			pVideoWeatherWarningLinkageInfo->szTime,
			"",
			pVideoWeatherWarningLinkageInfo->szTypeName,
			pVideoWeatherWarningLinkageInfo->szWarnValue,
			pVideoWeatherWarningLinkageInfo->szLinkageStationName,
			pVideoWeatherWarningLinkageInfo->nLinkNum);

		if (mysql_query(m_pMySql, sql_buf))
		{
			Unlock();
			return FALSE;
		}

		nCameraNum = pVideoWeatherWarningLinkageInfo->nLinkNum;
		pCameraInfo = pVideoWeatherWarningLinkageInfo->pLinkageCameraInfo;
		if (nCameraNum < 0||pCameraInfo == NULL)
		{
			Unlock();
			return FALSE;
		}

		sprintf_s(szTableName,"ct_linkage_camera_history_log_%s_%04d",m_szLogInUserName,sysTm.wYear);

		//检查表是否存在,不存在则创建
		CreateLinkageCameraHistoryLogTable(szTableName);

		for (int i = 0;i < nCameraNum;i++)
		{
			sprintf_s(sql_buf, "INSERT INTO %s (name,code,decode_tag,preset_name,preset_id,from_x1,from_y1,to_x1,to_y1,from_x2,from_y2,to_x2,to_y2,node_id) VALUES ('%s','%s',%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				szTableName,
				pCameraInfo->szName,
				pCameraInfo->szCode,
				pCameraInfo->nDecodeTag,
				pCameraInfo->szPresetName,
				pCameraInfo->nPresetId,
				pCameraInfo->nFromX1,
				pCameraInfo->nFromY1,
				pCameraInfo->nToX1,
				pCameraInfo->nToY1,
				pCameraInfo->nFromX2,
				pCameraInfo->nFromY2,
				pCameraInfo->nToX2,
				pCameraInfo->nToY2,
				nNodeId);

			mysql_query(m_pMySql, sql_buf);

			pCameraInfo++;
		}

		pCameraInfo = NULL;

		Unlock();
		return TRUE;
	}
	catch(...)
	{

	}

	Unlock();
	return FALSE;
}

BOOL CVMHistoryLog::WriteWeatherForecastLinkageLog(VIDEO_WEATHER_FORECAST_INFO *pVideoWeatherForecastLinkageInfo)
{
	if(pVideoWeatherForecastLinkageInfo == NULL)
		return FALSE;

	int nNodeId = 0;
	int nCameraNum = 0;
	char sql_buf[1024] = {0};
	char szTableName[255] = {0};

	MYSQL_RES * res = NULL;
	MYSQL_ROW	row ;
	SYSTEMTIME sysTm;
	VIDEO_LINKAGE_CAMERA_INFO *pCameraInfo = NULL;

	memset(sql_buf,0,sizeof(sql_buf));
	memset(szTableName,0,sizeof(szTableName));

	Lock();

	try
	{
		::GetLocalTime(&sysTm);
		sprintf_s(szTableName,"ct_linkage_history_log_%s_%04d",m_szLogInUserName,sysTm.wYear);

		//检查表是否存在,不存在则创建
		CreateLinkageHistoryLogTable(szTableName);

		sprintf_s(sql_buf, "SELECT id FROM %s ORDER BY id DESC LIMIT 0,1",szTableName);

		if (!mysql_query(m_pMySql, sql_buf))
		{
			res = mysql_store_result(m_pMySql);
			while ( row = mysql_fetch_row( res ) )
			{
				nNodeId = atoi(row[0]);
			}
			mysql_free_result( res ) ;
		}

		nNodeId++;

		sprintf_s(sql_buf, "INSERT INTO %s (id,linkage_type,station_name,device_name,sub_type,state,linkage_time,content,value1,value2,linkage_station_name,linkage_num) VALUES (%d,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s',%d)",
			szTableName,
			nNodeId,
			VIDEO_WEATHER_FORECAST_INFO_TYPE,
			pVideoWeatherForecastLinkageInfo->szID,
			pVideoWeatherForecastLinkageInfo->szLinkageStationName,
			pVideoWeatherForecastLinkageInfo->szWeatherType,
			pVideoWeatherForecastLinkageInfo->szCityID,
			pVideoWeatherForecastLinkageInfo->szStartTime,
			pVideoWeatherForecastLinkageInfo->szContent,
			pVideoWeatherForecastLinkageInfo->szTypeName,
			pVideoWeatherForecastLinkageInfo->szWeatherLevel,
			pVideoWeatherForecastLinkageInfo->szLinkageStationName,
			pVideoWeatherForecastLinkageInfo->nLinkNum);

		if (mysql_query(m_pMySql, sql_buf))
		{
			Unlock();
			return FALSE;
		}

		nCameraNum = pVideoWeatherForecastLinkageInfo->nLinkNum;
		pCameraInfo = pVideoWeatherForecastLinkageInfo->pLinkageCameraInfo;
		if (nCameraNum < 0||pCameraInfo == NULL)
		{
			Unlock();
			return FALSE;
		}

		sprintf_s(szTableName,"ct_linkage_camera_history_log_%s_%04d",m_szLogInUserName,sysTm.wYear);

		//检查表是否存在,不存在则创建
		CreateLinkageCameraHistoryLogTable(szTableName);

		for (int i = 0;i < nCameraNum;i++)
		{
			sprintf_s(sql_buf, "INSERT INTO %s (name,code,decode_tag,preset_name,preset_id,from_x1,from_y1,to_x1,to_y1,from_x2,from_y2,to_x2,to_y2,node_id) VALUES ('%s','%s',%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				szTableName,
				pCameraInfo->szName,
				pCameraInfo->szCode,
				pCameraInfo->nDecodeTag,
				pCameraInfo->szPresetName,
				pCameraInfo->nPresetId,
				pCameraInfo->nFromX1,
				pCameraInfo->nFromY1,
				pCameraInfo->nToX1,
				pCameraInfo->nToY1,
				pCameraInfo->nFromX2,
				pCameraInfo->nFromY2,
				pCameraInfo->nToX2,
				pCameraInfo->nToY2,
				nNodeId);

			mysql_query(m_pMySql, sql_buf);

			pCameraInfo++;
		}

		pCameraInfo = NULL;

		Unlock();
		return TRUE;
	}
	catch(...)
	{

	}

	Unlock();
	return FALSE;
}