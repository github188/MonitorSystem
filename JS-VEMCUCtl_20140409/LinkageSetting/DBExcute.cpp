#include "stdafx.h"
#include "DBExcute.h"
#include "PtuDlg.h"
#include <Windows.h>


MYSQL *g_LinkageSettingMysql;

//判断一个表中是否存在某个记录
bool CheckExists(char *szTable, char *szCondition)
{
	MYSQL_RES	*res ;
	MYSQL_ROW	row ;
	char sql_buf[1024] = {0};
	int nCount = 0;
	sprintf_s(sql_buf, 1024, "select count(*) from %s where %s", szTable, szCondition);

	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		if(row = mysql_fetch_row(res))
		{
			nCount = atoi(row[0]);
		}
		mysql_free_result( res ) ;
	}
	return nCount>0;
}

BOOL UpdateSubSystemOfDeviceType(CString strDeviceTypeName, int nId)
{
	CString str;
	str.Format("UPDATE ass_rvu_sm_device_type set f_parent_id=%d where f_device_type_name='%s'", 
		nId, strDeviceTypeName);

	if (mysql_query(g_LinkageSettingMysql, str))
	{
		return false;
	}
}

//从数据库中获取子系统类型
std::vector<T_ASS_SUBSYSTEM> GetSubSystemFromDB()
{
	MYSQL_RES *res ;
	MYSQL_ROW row ;
	char sql_buf[1024] = {0};
	std::vector<T_ASS_SUBSYSTEM> vecSubSystem;
	sprintf_s(sql_buf, 1024, "SELECT a.id,a.parent_id,a.device_type,a.node_name FROM "
		" ass_station_ob_node AS a");
	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while(row = mysql_fetch_row(res))
		{
			T_ASS_SUBSYSTEM tSubSystem;
			tSubSystem.nId = atoi(row[0]);
			tSubSystem.nParentId = atoi(row[1]);
			sprintf_s(tSubSystem.szSubSystemName, "%s", row[3]);
			tSubSystem.nSubSystemNum = atoi(row[2]);
			vecSubSystem.push_back(tSubSystem);
		}
		mysql_free_result( res ) ;
	}
	return vecSubSystem;
}

std::vector<T_ASS_RVU_TYPE> GetRvuTypeFromDB()
{
	MYSQL_RES *res ;
	MYSQL_ROW row ;
	char sql_buf[1024] = {0};
	std::vector<T_ASS_RVU_TYPE> vecRvuType;
	sprintf_s(sql_buf, 1024, "SELECT id,rvu_protocol,description "
		" FROM ass_rvu_protocol");
	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while(row = mysql_fetch_row(res))
		{
			T_ASS_RVU_TYPE tRvuType;
			tRvuType.nId = atoi(row[0]);
			sprintf_s(tRvuType.szRvuType, "%s", row[1]);
			sprintf_s(tRvuType.szDescription, "%s", row[2]);
			vecRvuType.push_back(tRvuType);
		}
		mysql_free_result( res ) ;
	}
	return vecRvuType;
}

std::vector<T_ASS_DEVICE_TYPE> GetDeviceTypeByParentId(int nParentId)
{
	MYSQL_RES *res ;
	MYSQL_ROW row ;
	char sql_buf[1024] = {0};
	std::vector<T_ASS_DEVICE_TYPE> vecDeviceType;
	sprintf_s(sql_buf, 1024, "SELECT a.f_id,a.f_parent_id,a.f_device_type_name,a.f_device_type_num "
		" FROM ass_rvu_sm_device_type AS a where a.f_parent_id=%d ", nParentId);
	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while(row = mysql_fetch_row(res))
		{
			T_ASS_DEVICE_TYPE tDeviceType;
			tDeviceType.nId = atoi(row[0]);
			tDeviceType.nParentId = atoi(row[1]);
			sprintf_s(tDeviceType.szDeviceTypeName, "%s", row[2]);
			tDeviceType.nDeviceTypeNum = atoi(row[3]);
			vecDeviceType.push_back(tDeviceType);
		}
		mysql_free_result( res ) ;
	}
	return vecDeviceType;
}

//从数据库中获取设备类型
std::vector<T_ASS_DEVICE_TYPE> GetDeviceTypeFromDB()
{
	MYSQL_RES *res ;
	MYSQL_ROW row ;
	char sql_buf[1024] = {0};
	std::vector<T_ASS_DEVICE_TYPE> vecDeviceType;
	sprintf_s(sql_buf, 1024, "SELECT a.f_id,a.f_parent_id,a.f_device_type_name,a.f_device_type_num "
		" FROM ass_rvu_sm_device_type AS a");
	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while(row = mysql_fetch_row(res))
		{
			T_ASS_DEVICE_TYPE tDeviceType;
			tDeviceType.nId = atoi(row[0]);
			tDeviceType.nParentId = atoi(row[1]);
			sprintf_s(tDeviceType.szDeviceTypeName, "%s", row[2]);
			tDeviceType.nDeviceTypeNum = atoi(row[3]);
			vecDeviceType.push_back(tDeviceType);
		}
		mysql_free_result( res ) ;
	}
	return vecDeviceType;
}