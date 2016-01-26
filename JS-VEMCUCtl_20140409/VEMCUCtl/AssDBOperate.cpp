#include "stdafx.h"
#include "AssDBOperate.h"
#include "AssCommonFunction.h"

BOOL GetSubscribeInfoById(T_ASS_SUBSCRIBE_INFO *pAssSubscribeInfo, int nId)
{
	MYSQL_RES	* res ;
	MYSQL_ROW	row ;
	char sql_buf[1024] = {0};

	sprintf_s(sql_buf, "select id,user_id,station_id from ass_alarm_subscribe where id=%d ",
		nId);
	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);
		if(row = mysql_fetch_row(res))
		{
			pAssSubscribeInfo->nId = atoi(row[0]);
			pAssSubscribeInfo->nUserId = atoi(row[1]);
			pAssSubscribeInfo->nStatiionId = atoi(row[2]);
		}
		mysql_free_result( res ) ;
	}

	return TRUE;
}

BOOL GetCameraCodeByCameraName(char *szCameraCode, char *szCameraName)
{
	MYSQL_RES	* res ;
	MYSQL_ROW	row ;
	char sql_buf[1024] = {0};

	sprintf_s(sql_buf, "SELECT camera_id FROM video_camera WHERE name ='%s'",szCameraName);
	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);
		if(row = mysql_fetch_row(res))
		{
			sprintf_s(szCameraCode, 32, row[0]);
		}
		mysql_free_result(res);
	}

	return TRUE;
}

BOOL GetCameraNameByCameraCode(ASS_CAMERA_INFO *pAssCameraInfo, char *szCameraCode)
{	
	MYSQL_RES	* res ;
	MYSQL_ROW	row ;
	char sql_buf[1024] = {0};

	sprintf_s(sql_buf, "SELECT camera_id,name FROM video_camera WHERE camera_id ='%s'",szCameraCode);
	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);
		if(row = mysql_fetch_row(res))
		{
			sprintf_s(pAssCameraInfo->szCameraCode, 32, row[0]);
			sprintf_s(pAssCameraInfo->szCameraName, 32, row[1]);
		}
		mysql_free_result(res);
	}

	return TRUE;
}

//判断一个表中是否存在某个记录
bool CheckExists(char *szTable, char *szCondition)
{
	MYSQL_RES	*res ;
	MYSQL_ROW	row ;
	char sql_buf[1024] = {0};
	int nCount = 0;
	sprintf_s(sql_buf, 1024, "select count(*) from %s where %s", szTable, szCondition);

	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);
		if(row = mysql_fetch_row(res))
		{
			nCount = atoi(row[0]);
		}
		mysql_free_result( res ) ;
	}
	return nCount>0;
}

std::vector<_T_ASS_DEVICE_LEDGER_> GetDeviceLedgersFromDB(CString strCondition)
{
	//获取变电站下的所有设备
	CString strSql;
	MYSQL_RES* res;
	MYSQL_ROW row;

	std::vector<_T_ASS_DEVICE_LEDGER_> vecDeviceLedgers;

	strSql.Format("SELECT a.id,a.station_id,a.device_name,a.device_manufacturer,a.device_ip,a.device_port,a.login_username, "
		" a.login_password,a.protocol_type,a.device_type,a.harddisk_capacity,a.analog_video_num,a.net_video_num,b.station_name_videoplant "
		" FROM ass_device_ledger a left join ob_d5000_station b on a.station_id=b.station_id  %s", strCondition);

	if (!mysql_query(g_mySqlData,strSql.GetBuffer()))
	{
		res = mysql_store_result(g_mySqlData);
		while (row = mysql_fetch_row(res))
		{
			_T_ASS_DEVICE_LEDGER_ tDeviceLedger;
			memset(&tDeviceLedger, 0, sizeof(_T_ASS_DEVICE_LEDGER_));
			tDeviceLedger.id = atoi(row[0]);
			tDeviceLedger.station_id = atoi(row[1]);
			sprintf_s(tDeviceLedger.device_name,"%s",row[2]);
			sprintf_s(tDeviceLedger.device_manufacturer,"%s",row[3]);
			sprintf_s(tDeviceLedger.device_ip,"%s",row[4]);
			tDeviceLedger.device_port = atoi(row[5]);
			sprintf_s(tDeviceLedger.login_username,"%s",row[6]);
			sprintf_s(tDeviceLedger.login_password,"%s",row[7]);
			sprintf_s(tDeviceLedger.protocol_type,"%s",row[8]);
			sprintf_s(tDeviceLedger.device_type,"%s",row[9]);
			sprintf_s(tDeviceLedger.harddisk_capacity,"%s",row[10]);
			tDeviceLedger.analog_video_num = atoi(row[11]);
			tDeviceLedger.net_video_num = atoi(row[12]);
			sprintf_s(tDeviceLedger.szStationName, "%s", row[13]);

			vecDeviceLedgers.push_back(tDeviceLedger);
		}
		mysql_free_result(res);
	}

	return vecDeviceLedgers;
}

//根据告警ID更新该条告警为已完成
bool UpdateAlarmInfoToCompleted(int nAlarmID, int nAlarmStatus)
{
	int nYear = GetCurrentYear();
	CString strCurrentTime = GetCurrentProcessAlarmTime();
	CString str;
	str.Format("UPDATE ass_alarm_%d set process_status=%d,process_time='%s',process_person='%s' where id=%d", 
		nYear, nAlarmStatus, strCurrentTime, g_userpower.username, nAlarmID);

	if (mysql_query(g_mySqlData, str))
	{
		return false;
	}
}

bool InsertLinkageConfigInfoToDB(T_ASS_LINKAGE_CONFIG* pAssLinkageConfig)
{
	char sql_buf[1024] = {0};
	sprintf_s(sql_buf,1024,"insert into ass_linkage_config set f_user_id=%d,f_show_type=%d,f_show_video=%d, " 
		" f_change_time=%d,f_security=%d,f_fire=%d,f_door_alarm=%d,f_door_action=%d on duplicate key update "
		" f_show_type=%d,f_show_video=%d,f_change_time=%d,f_security=%d,f_fire=%d,f_door_alarm=%d,f_door_action=%d",
		pAssLinkageConfig->nUserId,
		pAssLinkageConfig->nShowType,
		pAssLinkageConfig->nShowVideo,
		pAssLinkageConfig->nChangeTime,
		pAssLinkageConfig->nSecurity,
		pAssLinkageConfig->nFire,
		pAssLinkageConfig->nDoorAlarm,
		pAssLinkageConfig->nDoorAction,
		pAssLinkageConfig->nShowType,
		pAssLinkageConfig->nShowVideo,
		pAssLinkageConfig->nChangeTime,
		pAssLinkageConfig->nSecurity,
		pAssLinkageConfig->nFire,
		pAssLinkageConfig->nDoorAlarm,
		pAssLinkageConfig->nDoorAction);

	if (mysql_query(g_mySqlData, sql_buf))
	{
		return false;
	}
}

int GetDevIdByDevName(char *szDevName)
{
	if (szDevName == NULL || strcmp(szDevName,"") == 0)
	{
		return -1;
	}

	int nDevId = 0;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql_buf[1024] = {0};
	sprintf_s(sql_buf, "SELECT a.id FROM ass_rvu_sm AS a WHERE a.name='%s'", szDevName);

	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);

		if ( row = mysql_fetch_row( res ) )
		{
			nDevId = atoi(row[0]);
			return nDevId;
		}
		mysql_free_result(res) ;
	}
}

void GetDevInfoByDevId(ACS_DEV_INFO *pDevInfo, int nDevId)
{
	if (nDevId == 0)
		return;

	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql_buf[1024] = {0};
	sprintf_s(sql_buf, "SELECT a.id,a.name,a.relation_video_status,a.rvu_id,a.sm_id FROM ass_rvu_sm AS a WHERE a.id=%d", nDevId);

	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);

		if ( row = mysql_fetch_row( res ) )
		{
			pDevInfo->nDevId = nDevId;
			strcpy_s(pDevInfo->szDevName,sizeof(pDevInfo->szDevName),row[1]);
			pDevInfo->nRelationVideo = atoi(row[2]);
			pDevInfo->nRvuId = atoi(row[3]);
			pDevInfo->nSmId = atoi(row[4]);
		}
		mysql_free_result(res) ;
	}
}

//获取每个变电站发生的告警次数
std::vector<ALARM_STATISTICS> GetAlarmStatisticsInfo()
{
	int nYear = GetCurrentYear();
	std::vector<ALARM_STATISTICS> vecAlarmStatistics;
	CString str;
	str.Format("select count(*),d.station_name,d.rvu_id,d.station_id from "
		"(select a.rvu_id as rvu_id,b.station_id as station_id,c.station_name_videoplant as station_name from ass_alarm_%d a  "
		"LEFT JOIN ass_rvu b on a.rvu_id=b.rvu_id  "
		"LEFT JOIN ob_d5000_station c on b.station_id=c.station_id) as d GROUP BY d.station_id ", nYear);

	int re = mysql_query(g_mySqlData,str);
	if (re!=0){
		TRACE0("execute sql failed.");
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;
	res = mysql_store_result(g_mySqlData);
	while (row = mysql_fetch_row(res)){
		ALARM_STATISTICS tAlarmStatistics;
		tAlarmStatistics.nAlarmCount = atoi(row[0]);
		tAlarmStatistics.nStationId = atoi(row[3]);
		sprintf_s(tAlarmStatistics.szStationName, 64, row[1]);
		vecAlarmStatistics.push_back(tAlarmStatistics);
	}
	mysql_free_result(res) ;

	return vecAlarmStatistics;
}

std::vector<PUSH_DATA_RESPONSE> GetDeviceRealDataInfoBySceneName(CString strSceneName)
{
	std::vector<PUSH_DATA_RESPONSE> vecAlarm;

	CString str;
	str.Format("SELECT a.rvu_id,a.sm_id,a.dev_id,a.`value`,a.`status`,a.enum_type,b.scene_id,c.station_scenename,"
		" c.station_id,a.node_id FROM ass_rvu_sm_realdata AS a "
		" left JOIN ass_rvu_sm_entity AS b ON a.rvu_id = b.rvu_id AND a.sm_id = b.sm_id "
		" left JOIN ass_d5000_station_scene AS c ON b.scene_id = c.scene_id WHERE "
		" c.station_scenename = '%s'", strSceneName);

	int re = mysql_query(g_mySqlData,str);
	if (re!=0){
		TRACE0("execute sql failed.");
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;
	res = mysql_store_result(g_mySqlData);
	while (row = mysql_fetch_row(res)){
		PUSH_DATA_RESPONSE tPushDataResponse;
		tPushDataResponse.nRvuId = atoi(row[0]);
		tPushDataResponse.nSmId = atoi(row[1]);
		tPushDataResponse.fValue = atof(row[3]);
		tPushDataResponse.nStatus = atoi(row[4]);
		tPushDataResponse.nType = atoi(row[5]);
		vecAlarm.push_back(tPushDataResponse);
	}

	mysql_free_result(res) ;

	return vecAlarm;
}

int GetStationIdByStationName(CString strStationName)
{
	int nStationId = 0;

	CString str;
	str.Format("SELECT a.station_id FROM "
		" ob_d5000_station AS a WHERE a.station_name_videoplant='%s'", strStationName);

	int re = mysql_query(g_mySqlData,str);
	if (re!=0){
		TRACE0("execute sql failed.");
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;
	res = mysql_store_result(g_mySqlData);
	if (row = mysql_fetch_row(res)){
		nStationId = atoi(row[0]);
	}

	mysql_free_result(res) ;

	return nStationId;
}

void GetStationCodeByStationName(char *szStationCode, char *szStationName)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql_buf[1024] = {0};
	sprintf_s(sql_buf, "SELECT a.station_code_videoplant FROM "
		" ob_d5000_station AS a WHERE a.station_name_videoplant='%s' ", szStationName);

	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);

		if ( row = mysql_fetch_row( res ) )
		{
			sprintf(szStationCode, row[0]);
		}
		mysql_free_result(res) ;
	}
}

void GetStationInfoByStationId(ACS_STATION_INFO *pStationInfo, int nStationId)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql_buf[1024] = {0};
	sprintf_s(sql_buf, "SELECT a.station_id,a.station_name_videoplant,a.voltage_class FROM "
		" ob_d5000_station AS a WHERE a.station_id=%d ", nStationId);

	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);

		if ( row = mysql_fetch_row( res ) )
		{
			pStationInfo->nStationId = nStationId;
			strcpy_s(pStationInfo->szStationName,sizeof(pStationInfo->szStationName),row[1]);
			pStationInfo->nVolClass = atoi(row[2]);
		}
		mysql_free_result(res) ;
	}
}

void GetLinkageConfig(T_ASS_LINKAGE_CONFIG *pAssLinkageConfig)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql_buf[1024] = {0};
	sprintf_s(sql_buf, "SELECT f_user_id,f_show_type,f_show_video,f_change_time,f_security,f_fire, "
		" f_door_alarm,f_door_action FROM ass_linkage_config "
		" where f_user_id=%d", g_userpower.userID);

	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);

		if ( row = mysql_fetch_row( res ) )
		{
			pAssLinkageConfig->nUserId = atoi(row[0]);
			pAssLinkageConfig->nShowType = atoi(row[1]);
			pAssLinkageConfig->nShowVideo = atoi(row[2]);
			pAssLinkageConfig->nChangeTime = atoi(row[3]);
			pAssLinkageConfig->nSecurity = atoi(row[4]);
			pAssLinkageConfig->nFire = atoi(row[5]);
			pAssLinkageConfig->nDoorAlarm = atoi(row[6]);
			pAssLinkageConfig->nDoorAction = atoi(row[7]);
		}
		mysql_free_result(res) ;
	}
}

void GetCameraInfoByCameraCode(ASS_CAMERA_INFO *pCameraInfo, char *szCameraCode)
{
	char sql_buf[1024] = {0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,1024,"select a.camera_id,b.dvr_ip,b.tcp_port,b.dvr_type,b.adminpasswd,b.adminuser,a.channel,a.name from video_camera as a "
		" left join video_dvr as b on a.dvr_id=b.dvr_id where a.camera_id='%s'",szCameraCode);
	if (!mysql_query(g_mySqlData,sql_buf))
	{
		res = mysql_store_result(g_mySqlData);
		if ( row = mysql_fetch_row( res ) )
		{
			sprintf_s(pCameraInfo->szCameraCode, "%s", row[0]);
			sprintf_s(pCameraInfo->tDvrInfo.szDvrIp,"%s",row[1]);
			pCameraInfo->tDvrInfo.nDvrPort = atoi(row[2]);
			sprintf_s(pCameraInfo->tDvrInfo.szDvrType,"%s",row[3]);
			sprintf_s(pCameraInfo->tDvrInfo.szDvrPassword,"%s",row[4]);
			sprintf_s(pCameraInfo->tDvrInfo.szDvrUserName,"%s",row[5]);
			pCameraInfo->nChannel = atoi(row[6]);
			sprintf_s(pCameraInfo->szCameraName,"%s",row[7]);
			pCameraInfo->tDvrInfo.nDvrType = GetDvrTypeByDvrText(pCameraInfo->tDvrInfo.szDvrType);
		}
		mysql_free_result(res);
	}
}

std::vector<_T_ASS_SUBSYSTEM_ALARM_COUNT> GetSubSystemAlarmCount(CString strCondition)
{
	std::vector<_T_ASS_SUBSYSTEM_ALARM_COUNT> vecSubSystemAlarmCount;

	int nYear = GetCurrentYear();
	CString str;
	str.Format("select station_name, "
		" MAX(CASE sub_system WHEN 2 THEN alarm_count ELSE 0 END) as '门禁告警次数', "
		" MAX(CASE sub_system WHEN 4 THEN alarm_count ELSE 0 END) as '安防告警次数', "
		" MAX(CASE sub_system WHEN 6 THEN alarm_count ELSE 0 END) as '消防告警次数' "
		" from(select e.station_name_videoplant as station_name,c.f_parent_id as sub_system,count(*) as alarm_count from ass_alarm_%d as a  "
		" left join ass_rvu_sm as b on a.dev_id=b.id "
		" left join ass_rvu_sm_device_type as c on b.device_type=c.f_device_type_num "
		" left join ass_rvu as d on a.rvu_id=d.rvu_id "
		" LEFT JOIN ob_d5000_station e on d.station_id=e.station_id "
		" %s "
		" GROUP BY c.f_parent_id,e.station_name_videoplant) as AA  GROUP BY station_name ", nYear, strCondition);

	int re = mysql_query(g_mySqlData,str);
	if (re!=0){
		TRACE0("execute sql failed.");
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;
	res = mysql_store_result(g_mySqlData);
	while (row = mysql_fetch_row(res)){
		_T_ASS_SUBSYSTEM_ALARM_COUNT tAssSubSystemAlarm;
		sprintf_s(tAssSubSystemAlarm.szStationName, 64, row[0]);
		tAssSubSystemAlarm.nDoorCount = atoi(row[1]);
		tAssSubSystemAlarm.nAnFangCount = atoi(row[2]);
		tAssSubSystemAlarm.nFireCount = atoi(row[3]);

		vecSubSystemAlarmCount.push_back(tAssSubSystemAlarm);
	}

	mysql_free_result(res) ;

	return vecSubSystemAlarmCount;
}

//根据变电站和告警类型,获取未处理告警信息
std::vector<ACS_ALARM_INFO> GetAlarmInfoByStationId(CString strCondition)
{
	std::vector<ACS_ALARM_INFO> vecAlarmInfo;

	int nYear = GetCurrentYear();
	CString str;
	str.Format("select a.id,c.station_name_videoplant,e.`name`,f.f_device_type_name,a.start_time,a.alarm_level,a.process_status from ass_alarm_%d as a "
		" left join ass_rvu as b on a.rvu_id=b.rvu_id "
		" left join ob_d5000_station as c on b.station_id=c.station_id "
		" left join ass_rvu_sm as e on a.dev_id=e.id "
		" left join ass_rvu_sm_device_type as f on e.device_type=f.f_device_type_num "
	" where %s  ", nYear, strCondition);

	int re = mysql_query(g_mySqlData,str);
	if (re!=0){
		TRACE0("execute sql failed.");
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;
	res = mysql_store_result(g_mySqlData);
	while (row = mysql_fetch_row(res)){
		ACS_ALARM_INFO tAlarmInfo;
		tAlarmInfo.nId = atoi(row[0]);
		sprintf_s(tAlarmInfo.szStationName, 32, row[1]);
		sprintf_s(tAlarmInfo.szDevName, 32, row[2]);
		sprintf_s(tAlarmInfo.szAlarmTypeName, 32, row[3]);
		sprintf_s(tAlarmInfo.szAlarmStartTime, 32, row[4]);
		tAlarmInfo.nPriorLevel = atoi(row[5]);

		vecAlarmInfo.push_back(tAlarmInfo);
	}

	mysql_free_result(res) ;

	return vecAlarmInfo;
}

