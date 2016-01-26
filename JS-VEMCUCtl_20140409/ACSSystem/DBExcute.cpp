#include "stdafx.h"
#include "DBExcute.h"
#include "ACSSystemDlg.h"
#include <Windows.h>


MYSQL mysql_cameralist;

int connectDB()
{
	CString strError;

	if (mysql_init(&mysql_cameralist) == NULL) 
	{
		strError.Format("mysqlCtrl.cpp %i mysql_init mysql_cameralist is error.\n", __LINE__);
		MessageBox(NULL,strError,"数据库初始化",MB_OK);
		return -1;
	}	

	char value1 = 1;
	mysql_options(&mysql_cameralist,MYSQL_OPT_RECONNECT,(char*)&value1);

	if (!mysql_real_connect(&mysql_cameralist,g_pConfigInfo->szDBServerIp,"mdcs","mdcs2008",
		g_pConfigInfo->szDBName,g_pConfigInfo->nDBServerPort,NULL,0)) 
	{
		strError.Format("mysqlCtrl.cpp %i mysql_cameralist mysql_real_connect: %s\n", __LINE__, mysql_error(&mysql_cameralist));
		MessageBox(NULL,strError,"数据库初始化",MB_OK);
		return -1;
	}

	mysql_query(&mysql_cameralist,"SET NAMES 'GBK'");


	return 0;
}

int disconnectDB()
{
	if (&mysql_cameralist)
	{
		mysql_close(&mysql_cameralist);
	}
	return 0;
}

int GetRvuIDByStationID(int nStationId)
{
	int nRvuId = 0;
	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;
	sprintf_s(sql_buf,"select rvu_id from ass_rvu where station_id=%d ",nStationId);
	if (!mysql_query(&mysql_cameralist,sql_buf))
	{
		res = mysql_store_result(&mysql_cameralist);
		while (row = mysql_fetch_row(res))
		{
			nRvuId = atoi(row[0]);
		}
		mysql_free_result(res);
	}
	return nRvuId;
}

int GetStationIdByRvuId(int nRvuId)
{
	int nStationId = 0;
	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;
	sprintf_s(sql_buf,"select station_id from ass_rvu where rvu_id=%d ",nRvuId);
	if (!mysql_query(&mysql_cameralist,sql_buf))
	{
		res = mysql_store_result(&mysql_cameralist);
		while (row = mysql_fetch_row(res))
		{
			nStationId = atoi(row[0]);
		}
		mysql_free_result(res);
	}
	return nStationId;
}

//根据devId获取关联的cameraCode
void GetCameraCodeByDevId(int nStationId, int nDevId, T_LINKAGE_CAMERA_INFO *tCameraCode, int *nCount)
{
	char sql_buf[1024] = {0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;
	int nCameraCount = 0;

	sprintf_s(sql_buf,1024,"select camera_code,dev_id,camera_preset_id,preset_name from ass_camerapreset where dev_id=%d",nDevId);
	if (!mysql_query(&mysql_cameralist,sql_buf))
	{
		res = mysql_store_result(&mysql_cameralist);
		while (row = mysql_fetch_row(res))
		{
			sprintf_s(tCameraCode[nCameraCount].szCameraCode,32,"%s",row[0]);
			tCameraCode[nCameraCount].nDevId = atoi(row[1]);
			tCameraCode[nCameraCount].nPresetId = atoi(row[2]);
			sprintf_s(tCameraCode[nCameraCount].szPresetName,64,"%s",row[3]);

			nCameraCount++;

			if (nCameraCount == 16)
			{
				break;
			}
		}
		mysql_free_result(res);
	}
	
	*nCount = nCameraCount;
}

//通过推送来的实时数据获取该设备的其他信息
void GetSMRealDataInfoByPushDataResponse(T_SM_REALDATA *tSmRealData, PUSH_DATA_RESPONSE *pTSmRealData)
{
	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;
	sprintf_s(sql_buf,"SELECT a.id,a.`name`,a.relation_video_status,b.station_id,a.rvu_id FROM "
		"ass_rvu_sm AS a LEFT JOIN ass_rvu AS b ON a.rvu_id = b.rvu_id "
		"where a.rvu_id=%d and a.sm_id=%d", pTSmRealData->nRvuId, pTSmRealData->nSmId);
	if (!mysql_query(&mysql_cameralist,sql_buf))
	{
		res = mysql_store_result(&mysql_cameralist);
		if (row = mysql_fetch_row(res))
		{
			tSmRealData->station_id = atoi(row[3]);
			tSmRealData->dev_id = atoi(row[0]);
			tSmRealData->rvu_id = pTSmRealData->nRvuId;
			tSmRealData->sm_id = pTSmRealData->nSmId;
			tSmRealData->node_id = pTSmRealData->nNodeId;
			tSmRealData->type = pTSmRealData->nType;
			tSmRealData->real_value = pTSmRealData->fValue;
			tSmRealData->status = pTSmRealData->nStatus;
			sprintf_s(tSmRealData->dev_name,"%s",row[1]);
			tSmRealData->relation_video = atoi(row[2]);
		}
		mysql_free_result(res);
	}
}