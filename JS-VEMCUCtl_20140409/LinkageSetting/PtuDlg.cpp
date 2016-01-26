// PtuDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "LinkageSetting.h"
#include "PtuDlg.h"
#include "DBExcute.h"
#include "RVUServerParse.h"
#include "common.h"

#define TREE_STATION_LIST_WIDTH (300)

TSM g_t_TSM[MAX_SM_NUM_PER_STATION]={0x0};
int g_nTSM = 0;

TNODE g_t_TNODE[MAX_NODE_NUM_PER_STATION]={0x0};
int g_nTNODE = 0;

RECV_PACKET g_recv_packet;
int g_nDataLen = 0;

COMMONTHREADPOOL g_parse_rvu_thread ;

CPtuDlg* g_pMainDlg = NULL;

VEM_USER_CONFIG_INFO* g_pConfigInfo = NULL;
DEV_PLAY_VIDEO_SDK_FUNS* g_pstruDevPlayVideoSdkFunsEx = NULL;

//////////////////////////////////////////////////////////////////////////
_T_ACS_STATION_OB_NODE_ g_tAcsStationObNode[MAX_STATION_OB_NODE_NUM];
int g_nStationObNodeNum=0;

_T_ASS_STATION_OB_NODE_ g_tAssStationObNode[MAX_STATION_OB_NODE_NUM];
int g_nAssStationObNodeNum=0;

_T_ACS_STATION_OB_ACCESS_DEV_ g_tAcsStationObAccessDev[MAX_STATION_OB_ACCESS_DEV];
int g_nStationObAccessDev=0;

_T_ACS_STATION_OB_CAMERA_ g_tAcsStationObCamera[MAX_STATION_OB_CAMERA];
int g_nStationObCamera=0;

_T_ACS_STATION_LINKAGE_RELATION_ g_tAcsStationLinkageRelation[MAX_STATION_ST_RELATION];
int g_nStationLinkageRelation=0;

_T_ACS_STATION_OB_YK_ g_tAcsStationObYK[MAX_STATION_OB_YK];
int g_nStationObYK=0;

_T_ACS_STATION_OB_YT_ g_tAcsStationObYT[MAX_STATION_OB_YT];
int g_nStationObYT=0;

_T_ACS_STATION_OB_YX_ g_tAcsStationObYX[MAX_STATION_OB_YX];
int g_nStationObYX=0;

_T_ACS_STATION_OB_YC_ g_tAcsStationObYC[MAX_STATION_OB_YC];
int g_nStationObYC=0;

_T_ACS_STATION_OB_CARD_ g_tAcsStationObCard[MAX_STATION_OB_CARD];
int g_nStationObCard=0;

_T_ACS_STATION_CAMERA_PRESET_ g_tAcsStationObPreset[MAX_STATION_CAMERA_PRESET];
int g_nStationObCameraPreset=0;

_T_ACS_STATION_CT_USER_ g_tAcsStationCtUser[MAX_STATION_CT_USER];
int g_nStationCtUser=0;

_T_ACS_RVU_CAMERA_INFO_ g_tAcsRVUCameraInfo[MAX_CAMERA_NUM_PER_USER];
int g_nAcsRVUStation = 0;

_T_ACS_RVU_STATION_INFO_ g_tAcsRVUStationInfo[MAX_STATION_NUM_PER_USER];
int g_nAcsRVUCamera = 0;

_T_ASS_RVU_INFO_ g_tAssRVU[MAX_RVU_NUM_PER_STATION]; 
int g_nAssRVUNum = 0;

_T_ACS_RVU_DVR_INFO_ g_tAcsRVUDVRInfo[MAX_DVR_NUM_PER_USER];
int g_nAcsRVUDVR = 0;

_T_ASS_RVU_DEVICE_INFO_ g_tAssRVUDevice[MAX_RVU_DEVICE_NUM_PER_STATION];
int g_nAssRVUDeviceNum;

//登录用户
T_USER *g_pUser = NULL;

//写日志
void WriteAssUserOperateLogPre(char *szNote)
{
	ASS_USER_OPERATE_LOG assUserOperateLog;
	sprintf_s(assUserOperateLog.ip_address,g_pConfigInfo->szLocalUdpIp);
	sprintf_s(assUserOperateLog.operate_action,szNote);
	assUserOperateLog.user_id = g_pUser->user_id;
	g_AssUserOperateLog.WriteAssUserOperateLog(&assUserOperateLog);
}

// CPtuDlg 对话框

void ReadStationCtUser()
{
	g_nStationCtUser = 0;
	memset(&g_tAcsStationCtUser,0,sizeof(_T_ACS_STATION_CT_USER_)*MAX_STATION_CT_USER);

	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"%s","SELECT user_id,user_name,pass_word,user_type,enablectrl,realtalk_p,eventlog_p,replay_p,alarm_p,tunecycle_p,usermanager_p,devicemanager_p,timerecord_p,elecmap_p FROM acs_station_ct_user");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAcsStationCtUser[g_nStationCtUser].user_id = atoi(row[0]);
			sprintf_s(g_tAcsStationCtUser[g_nStationCtUser].user_name,"%s",row[1]);
			sprintf_s(g_tAcsStationCtUser[g_nStationCtUser].user_password,"%s",row[2]);
			g_tAcsStationCtUser[g_nStationCtUser].user_type = atoi(row[3]);
			g_tAcsStationCtUser[g_nStationCtUser].enablectrl_p = atoi(row[4]);
			g_tAcsStationCtUser[g_nStationCtUser].realtalk_p = atoi(row[5]);
			g_tAcsStationCtUser[g_nStationCtUser].eventlog_p = atoi(row[6]);
			g_tAcsStationCtUser[g_nStationCtUser].replay_p = atoi(row[7]);
			g_tAcsStationCtUser[g_nStationCtUser].alarm_p = atoi(row[8]);
			g_tAcsStationCtUser[g_nStationCtUser].tunecycle_p = atoi(row[9]);
			g_tAcsStationCtUser[g_nStationCtUser].usermanage_p = atoi(row[10]);
			g_tAcsStationCtUser[g_nStationCtUser].devicemanage_p = atoi(row[11]);
			g_tAcsStationCtUser[g_nStationCtUser].videorecord_p = atoi(row[12]);
			g_tAcsStationCtUser[g_nStationCtUser].elecmap_p = atoi(row[13]);

			g_nStationCtUser++;
		}
		mysql_free_result(res);
	}
}

void ReadStationObCard()
{
	g_nStationObCard = 0;
	memset(&g_tAcsStationObCard,0,sizeof(_T_ACS_STATION_OB_CARD_)*MAX_STATION_OB_CARD);

	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,1024,"%s","SELECT id,code,code_sys,user_name,personal_code,department,sex FROM acs_station_ob_card");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAcsStationObCard[g_nStationObCard].id = atoi(row[0]);
			sprintf_s(g_tAcsStationObCard[g_nStationObCard].code_door,"%s",row[1]);
			sprintf_s(g_tAcsStationObCard[g_nStationObCard].code_sys,"%s",row[2]);
			sprintf_s(g_tAcsStationObCard[g_nStationObCard].user_name,"%s",row[3]);
			sprintf_s(g_tAcsStationObCard[g_nStationObCard].personal_code,"%s",row[4]);
			sprintf_s(g_tAcsStationObCard[g_nStationObCard].department,"%s",row[5]);
			g_tAcsStationObCard[g_nStationObCard].sex = atoi(row[6]);

			g_nStationObCard++;
		}
		mysql_free_result(res);
	}
}

void ReadStationCameraPreset()
{
	g_nStationObCameraPreset = 0;
	memset(&g_tAcsStationObPreset,0,sizeof(_T_ACS_STATION_CAMERA_PRESET_)*MAX_STATION_CAMERA_PRESET);

	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"%s","SELECT id,camera_id,preset_id,preset_name FROM acs_station_st_camera_preset");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAcsStationObPreset[g_nStationObCameraPreset].id = atoi(row[0]);
			g_tAcsStationObPreset[g_nStationObCameraPreset].camera_id = atoi(row[1]);
			g_tAcsStationObPreset[g_nStationObCameraPreset].preset_id = atoi(row[2]);
			sprintf_s(g_tAcsStationObPreset[g_nStationObCameraPreset].preset_name,"%s",row[3]);
			g_nStationObCameraPreset++;
		}
		mysql_free_result(res);
	}
}


void ReadStationYK()
{
	g_nStationObYK = 0;
	memset(&g_tAcsStationObYK,0,sizeof(_T_ACS_STATION_OB_YK_)*MAX_STATION_OB_YK);

	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"%s","SELECT id,yk_code_sys,accessdev_code_id,yk_name,yk_addr,normal_state,state,dev_online_state FROM acs_station_ob_yk");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAcsStationObYK[g_nStationObYK].id = atoi(row[0]);
			sprintf_s(g_tAcsStationObYK[g_nStationObYK].yk_code_sys,"%s",row[1]);
			g_tAcsStationObYK[g_nStationObYK].access_code_id = atoi(row[2]);
			sprintf_s(g_tAcsStationObYK[g_nStationObYK].yk_name,"%s",row[3]);
			sprintf_s(g_tAcsStationObYK[g_nStationObYK].yk_addr,"%s",row[4]);
			g_tAcsStationObYK[g_nStationObYK].normal_state = atoi(row[5]);
			g_tAcsStationObYK[g_nStationObYK].current_state = atoi(row[6]);
			g_tAcsStationObYK[g_nStationObYK].dev_online_state = atoi(row[7]);

			g_nStationObYK++;
		}
		mysql_free_result(res);
	}
}

void ReadStationYT()
{
	g_nStationObYT = 0;
	memset(&g_tAcsStationObYT,0,sizeof(_T_ACS_STATION_OB_YT_)*MAX_STATION_OB_YT);

	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"%s","SELECT id,yt_code_sys,accessdev_code_id,yt_name,yt_addr,setting_value,current_value,dev_online_state FROM acs_station_ob_yt");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAcsStationObYT[g_nStationObYT].id = atoi(row[0]);
			sprintf_s(g_tAcsStationObYT[g_nStationObYT].yt_code_sys,"%s",row[1]);
			g_tAcsStationObYT[g_nStationObYT].access_code_id = atoi(row[2]);
			sprintf_s(g_tAcsStationObYT[g_nStationObYT].yt_name,"%s",row[3]);
			sprintf_s(g_tAcsStationObYT[g_nStationObYT].yt_addr,"%s",row[4]);
			g_tAcsStationObYT[g_nStationObYT].setting_value = atoi(row[5]);
			g_tAcsStationObYT[g_nStationObYT].current_value = atoi(row[6]);
			g_tAcsStationObYT[g_nStationObYT].dev_online_state = atoi(row[7]);

			g_nStationObYT++;
		}
		mysql_free_result(res);
	}
}

void ReadStationYX()
{
	g_nStationObYX = 0;
	memset(&g_tAcsStationObYX,0,sizeof(_T_ACS_STATION_OB_YX_)*MAX_STATION_OB_YX);

	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"%s","SELECT id,yx_code_sys,accessdev_code_id,yx_name,yx_addr,normal_state,current_state,dev_online_state FROM acs_station_ob_yx");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAcsStationObYX[g_nStationObYX].id = atoi(row[0]);
			sprintf_s(g_tAcsStationObYX[g_nStationObYX].yx_code_sys,"%s",row[1]);
			g_tAcsStationObYX[g_nStationObYX].access_code_id = atoi(row[2]);
			sprintf_s(g_tAcsStationObYX[g_nStationObYX].yx_name,"%s",row[3]);
			sprintf_s(g_tAcsStationObYX[g_nStationObYX].yx_addr,"%s",row[4]);
			g_tAcsStationObYX[g_nStationObYX].normal_value = atoi(row[5]);
			g_tAcsStationObYX[g_nStationObYX].current_value = atoi(row[6]);
			g_tAcsStationObYX[g_nStationObYX].dev_online_state = atoi(row[7]);

			g_nStationObYX++;
		}
		mysql_free_result(res);
	}
}

void ReadStationYC()
{
	g_nStationObYC = 0;
	memset(&g_tAcsStationObYC,0,sizeof(_T_ACS_STATION_OB_YC_)*MAX_STATION_OB_YC);

	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"%s","SELECT id,yc_code_sys,accessdev_code_id,yc_name,yc_addr,up_up_value,up_value,value,low_value,low_low_value,dev_online_state FROM acs_station_ob_yc");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAcsStationObYC[g_nStationObYC].id = atoi(row[0]);
			sprintf_s(g_tAcsStationObYC[g_nStationObYC].yc_code_sys,"%s",row[1]);
			g_tAcsStationObYC[g_nStationObYC].access_code_id = atoi(row[2]);
			sprintf_s(g_tAcsStationObYC[g_nStationObYC].yc_name,"%s",row[3]);
			sprintf_s(g_tAcsStationObYC[g_nStationObYC].yc_addr,"%s",row[4]);
			g_tAcsStationObYC[g_nStationObYC].up_up_value = atof(row[5]);
			g_tAcsStationObYC[g_nStationObYC].up_value = atof(row[6]);
			g_tAcsStationObYC[g_nStationObYC].current_value = atof(row[7]);
			g_tAcsStationObYC[g_nStationObYC].low_value = atof(row[8]);
			g_tAcsStationObYC[g_nStationObYC].low_low_value = atof(row[9]);
			g_tAcsStationObYC[g_nStationObYC].dev_online_state = atof(row[10]);

			g_nStationObYC++;
		}
		mysql_free_result(res);
	}
}

void ReadStationLinkageRelation()
{
	g_nStationLinkageRelation = 0;
	memset(&g_tAcsStationLinkageRelation,0,sizeof(_T_ACS_STATION_LINKAGE_RELATION_)*MAX_STATION_ST_RELATION);

	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"%s","SELECT id,event_access_dev_id,event_dev_id,event_type,event_reason,event_reason_text,action_access_dev_id,"
		"action_dev_id,action_level,action_type,action_type_text,action_pretime,action_time,action_timeinterval,action_digital,action_yk,action_analog FROM acs_station_st_relation");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].id = atoi(row[0]);
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].event_access_dev_id = atoi(row[1]);
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].event_dev_id = atoi(row[2]);
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].event_type = atoi(row[3]);
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].event_reason = atoi(row[4]);
			sprintf_s(g_tAcsStationLinkageRelation[g_nStationLinkageRelation].event_reason_text,"%s",row[5]);
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].action_access_dev_id = atoi(row[6]);
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].action_dev_id = atoi(row[7]);
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].action_level = atoi(row[8]);
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].action_type = atoi(row[9]);
			sprintf_s(g_tAcsStationLinkageRelation[g_nStationLinkageRelation].action_type_text,"%s", row[10]);
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].action_pretime = atoi(row[11]);
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].action_time = atoi(row[12]);
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].action_timeinterval = atoi(row[13]);
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].action_digital = atoi(row[14]);
			g_tAcsStationLinkageRelation[g_nStationLinkageRelation].action_yk = atoi(row[15]);
			sprintf_s(g_tAcsStationLinkageRelation[g_nStationLinkageRelation].action_analog,"%s",row[16]);

			g_nStationLinkageRelation++;
		}
		mysql_free_result(res);
	}
}

void ReadSubSystemNode()
{
	g_nAssStationObNodeNum = 0;
	memset(&g_tAssStationObNode,0,sizeof(_T_ASS_STATION_OB_NODE_)*MAX_STATION_OB_NODE_NUM);

	char sql_buf[1024]={0x0};

	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"%s","SELECT id,parent_id,node_name FROM ass_station_ob_node");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAssStationObNode[g_nAssStationObNodeNum].id = atoi(row[0]);
			g_tAssStationObNode[g_nAssStationObNodeNum].parent_id = atoi(row[1]);
			sprintf_s(g_tAssStationObNode[g_nAssStationObNodeNum].node_name,"%s",row[2]);

			g_nAssStationObNodeNum++;
		}
		mysql_free_result(res);
	}
}

void ReadStationNode()
{
	g_nStationObNodeNum = 0;
	memset(&g_tAcsStationObNode,0,sizeof(_T_ACS_STATION_OB_NODE_)*MAX_STATION_OB_NODE_NUM);

	char sql_buf[1024]={0x0};

	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"%s","SELECT id,parent_id,node_name,node_sysid,node_type FROM acs_station_ob_node");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAcsStationObNode[g_nStationObNodeNum].id = atoi(row[0]);
			g_tAcsStationObNode[g_nStationObNodeNum].parent_id = atoi(row[1]);
			sprintf_s(g_tAcsStationObNode[g_nStationObNodeNum].node_name,"%s",row[2]);
			sprintf_s(g_tAcsStationObNode[g_nStationObNodeNum].node_sysid,"%s",row[3]);
			g_tAcsStationObNode[g_nStationObNodeNum].node_type = atoi(row[4]);

			g_nStationObNodeNum++;
		}
		mysql_free_result(res);
	}
}

void ReadStationAccessDev()
{
	g_nStationObAccessDev = 0;
	memset(&g_tAcsStationObAccessDev,0,sizeof(_T_ACS_STATION_OB_ACCESS_DEV_)*MAX_STATION_OB_ACCESS_DEV);

	char sql_buf[1024]={0x0};

	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"%s","SELECT id,dev_code_sys,node_code_id,dev_name,connect_type,ip_addr,ip_protocol,ip_port,local_addr,local_port,serial_addr,"
		"serial_port,serial_bandrate,serial_data,serial_stopdata,serial_parity,access_username,access_password,dev_type,factory_type,capacity_yx_num,"
		"capacity_yc_num,capacity_yk_num,capacity_yt_num,capacity_camera_num,dev_online_state FROM acs_station_ob_access_dev");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAcsStationObAccessDev[g_nStationObAccessDev].id = atoi(row[0]);
			sprintf_s(g_tAcsStationObAccessDev[g_nStationObAccessDev].dev_code_sys,"%s",row[1]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].parent_id = atoi(row[2]);
			sprintf_s(g_tAcsStationObAccessDev[g_nStationObAccessDev].dev_name,"%s",row[3]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].connect_type = atoi(row[4]);
			sprintf_s(g_tAcsStationObAccessDev[g_nStationObAccessDev].ipaddr,"%s",row[5]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].ip_protocol = atoi(row[6]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].ipport = atoi(row[7]);
			sprintf_s(g_tAcsStationObAccessDev[g_nStationObAccessDev].local_addr,"%s",row[8]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].local_port = atoi(row[9]);
			sprintf_s(g_tAcsStationObAccessDev[g_nStationObAccessDev].serial_device_addr,"%s",row[10]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].serial_device_port = atoi(row[11]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].bandrate = atoi(row[12]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].data = atoi(row[13]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].stopdata = atoi(row[14]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].parity = atoi(row[15]);
			sprintf_s(g_tAcsStationObAccessDev[g_nStationObAccessDev].username,"%s",row[16]);
			sprintf_s(g_tAcsStationObAccessDev[g_nStationObAccessDev].userpassword,"%s",row[17]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].dev_type = atoi(row[18]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].factory_type = atoi(row[19]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].yx_num = atoi(row[20]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].yc_num = atoi(row[21]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].yk_num = atoi(row[22]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].yt_num = atoi(row[23]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].camera_num = atoi(row[24]);
			g_tAcsStationObAccessDev[g_nStationObAccessDev].dev_online_state = atoi(row[25]);

			g_nStationObAccessDev++;
		}
		mysql_free_result(res);
	}
}


void ReadStationCamera()
{
	g_nStationObCamera = 0;
	memset(&g_tAcsStationObCamera,0,sizeof(_T_ACS_STATION_OB_CAMERA_)*MAX_STATION_OB_CAMERA);

	char sql_buf[1024]={0x0};

	MYSQL_RES* res;
	MYSQL_ROW row; 

	sprintf_s(sql_buf,1024,"%s","SELECT id,accessdev_code_id,camera_code_sys,camera_name,camera_channel,dev_online_state FROM acs_station_ob_camera");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while(row = mysql_fetch_row(res))
		{
			g_tAcsStationObCamera[g_nStationObCamera].id = atoi(row[0]);
			g_tAcsStationObCamera[g_nStationObCamera].accessdev_code_id = atoi(row[1]);
			sprintf_s(g_tAcsStationObCamera[g_nStationObCamera].camera_code,"%s",row[2]);
			sprintf_s(g_tAcsStationObCamera[g_nStationObCamera].camera_name,"%s",row[3]);
			g_tAcsStationObCamera[g_nStationObCamera].camera_channel = atoi(row[4]);
			g_tAcsStationObCamera[g_nStationObCamera].camera_online_state = atoi(row[5]);

			g_nStationObCamera++;
		}
		mysql_free_result(res);
	}
}

void ReadRVUStationByUserID(int user_id)
{
	g_nAcsRVUStation = 0;
	memset(&g_tAcsRVUStationInfo,0,sizeof(_T_ACS_RVU_STATION_INFO_)*MAX_STATION_NUM_PER_USER);


	char sql_buf[1024]={0x0};

	MYSQL_RES* res;
	MYSQL_ROW row; 
	sprintf_s(sql_buf,1024,"SELECT t1.station_name,t1.station_code,t1.station_id FROM ass_rvu_station as t1,ass_rvu_user_station_relation as t2 where t2.user_id=%d and t2.station_id=t1.station_id",user_id);
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			sprintf_s(g_tAcsRVUStationInfo[g_nAcsRVUStation].station_name,64,"%s",row[0]);
			sprintf_s(g_tAcsRVUStationInfo[g_nAcsRVUStation].station_code,32,"%s",row[1]);
			g_tAcsRVUStationInfo[g_nAcsRVUStation].station_id = atoi(row[2]);
			g_tAcsRVUStationInfo[g_nAcsRVUStation].user_id = user_id;
			g_nAcsRVUStation++;
		}
		mysql_free_result(res);
	}
}

void ReadRVUDVR()
{
	g_nAcsRVUDVR = 0;
	memset(&g_tAcsRVUDVRInfo,0,sizeof(_T_ACS_RVU_DVR_INFO_)*MAX_DVR_NUM_PER_USER);

	char sql_buf[1024]={0x0};

	MYSQL_RES* res;
	MYSQL_ROW row; 
	sprintf_s(sql_buf,1024,"%s","SELECT id,station_id,dvr_code,dvr_name,dvr_type,dvr_ip,dvr_port,dvr_username,dvr_password FROM ass_rvu_dvr");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAcsRVUDVRInfo[g_nAcsRVUDVR].dvr_id = atoi(row[0]);
			g_tAcsRVUDVRInfo[g_nAcsRVUDVR].station_id = atoi(row[1]);
			sprintf_s(g_tAcsRVUDVRInfo[g_nAcsRVUDVR].dvr_code,24,"%s",row[2]);
			sprintf_s(g_tAcsRVUDVRInfo[g_nAcsRVUDVR].dvr_name,64,"%s",row[3]);
			sprintf_s(g_tAcsRVUDVRInfo[g_nAcsRVUDVR].dvr_type,16,"%s",row[4]);
			sprintf_s(g_tAcsRVUDVRInfo[g_nAcsRVUDVR].dvr_ip,24,"%s",row[5]);
			g_tAcsRVUDVRInfo[g_nAcsRVUDVR].dvr_port = atoi(row[6]);
			sprintf_s(g_tAcsRVUDVRInfo[g_nAcsRVUDVR].dvr_username,32,"%s",row[7]);
			sprintf_s(g_tAcsRVUDVRInfo[g_nAcsRVUDVR].dvr_password,32,"%s",row[8]);
			g_nAcsRVUDVR++;
		}
		mysql_free_result(res);
	}
}

void ReadRVUInfo()
{
	g_nAssRVUNum = 0;
	memset(&g_tAssRVU,0,sizeof(_T_ASS_RVU_INFO_)*MAX_RVU_NUM_PER_STATION);

	char sql_buf[1024]={0x0};

	MYSQL_RES* res;
	MYSQL_ROW row; 
	sprintf_s(sql_buf,1024,"%s","SELECT rvu_id,station_id,rvu_code,rvu_name,rvu_ip,rvu_port,rvu_username,rvu_password,rvu_protocol FROM ass_rvu");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAssRVU[g_nAssRVUNum].rvu_id = atoi(row[0]);
			g_tAssRVU[g_nAssRVUNum].station_id = atoi(row[1]);
			sprintf_s(g_tAssRVU[g_nAssRVUNum].rvu_code,32,"%s",row[2]);
			sprintf_s(g_tAssRVU[g_nAssRVUNum].rvu_name,64,"%s",row[3]);
			sprintf_s(g_tAssRVU[g_nAssRVUNum].rvu_ip,16,"%s",row[4]);
			g_tAssRVU[g_nAssRVUNum].rvu_port = atoi(row[5]);
			sprintf_s(g_tAssRVU[g_nAssRVUNum].rvu_username,32,"%s",row[6]);
			sprintf_s(g_tAssRVU[g_nAssRVUNum].rvu_userpassword,32,"%s",row[7]);
			sprintf_s(g_tAssRVU[g_nAssRVUNum].rvu_type,32,"%s",row[8]);
			g_nAssRVUNum++;
		}
		mysql_free_result(res);
	}
}

void ReadRVUCamera()
{
	g_nAcsRVUCamera = 0;
	memset(&g_tAcsRVUCameraInfo,0,sizeof(_T_ACS_RVU_CAMERA_INFO_)*MAX_CAMERA_NUM_PER_USER);

	char sql_buf[1024]={0x0};

	MYSQL_RES* res;
	MYSQL_ROW row; 
	sprintf_s(sql_buf,1024,"%s","SELECT id,dvr_id,camera_code,camera_name,channel_id,station_id FROM ass_rvu_camera");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAcsRVUCameraInfo[g_nAcsRVUCamera].camera_id = atoi(row[0]);
			g_tAcsRVUCameraInfo[g_nAcsRVUCamera].dvr_id = atoi(row[1]);
			sprintf_s(g_tAcsRVUCameraInfo[g_nAcsRVUCamera].camera_code,32,"%s",row[2]);
			sprintf_s(g_tAcsRVUCameraInfo[g_nAcsRVUCamera].camera_name,64,"%s",row[3]);
			g_tAcsRVUCameraInfo[g_nAcsRVUCamera].channel_id = atoi(row[4]);
			g_tAcsRVUCameraInfo[g_nAcsRVUCamera].station_id = atoi(row[5]);

			g_nAcsRVUCamera++;
		}
		mysql_free_result(res);
	}
}

void ReadRVUDevice()
{
	g_nAssRVUDeviceNum = 0;
	memset(&g_tAssRVUDevice,0,sizeof(_T_ASS_RVU_DEVICE_INFO_)*MAX_RVU_DEVICE_NUM_PER_STATION);

	char sql_buf[1024] = {0x0};

	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,1024,"%s","select sm_id,name,node_id,device_type,id,rvu_id,enum_type from ass_rvu_sm");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_tAssRVUDevice[g_nAssRVUDeviceNum].sm_id = atoi(row[0]);
			sprintf_s(g_tAssRVUDevice[g_nAssRVUDeviceNum].node_name,NAMELENGTH,"%s",row[1]);
			g_tAssRVUDevice[g_nAssRVUDeviceNum].node_id = atoi(row[2]);
			g_tAssRVUDevice[g_nAssRVUDeviceNum].device_type = atoi(row[3]);
			g_tAssRVUDevice[g_nAssRVUDeviceNum].id = atoi(row[4]);
			g_tAssRVUDevice[g_nAssRVUDeviceNum].rvu_id = atoi(row[5]);
			g_tAssRVUDevice[g_nAssRVUDeviceNum].sm_type = atoi(row[6]);

			g_nAssRVUDeviceNum++;
		}
		mysql_free_result(res);
	}
}

IMPLEMENT_DYNAMIC(CPtuDlg, CDialog)

CPtuDlg::CPtuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPtuDlg::IDD, pParent)
{
	m_pPresetSettingDlg = NULL;
	m_pLinkageConfigDlg = NULL;
	g_pMainDlg = NULL;
	m_pPtuManageDlg = NULL;
	m_pDeviceLedgerDlg = NULL;
	m_pDeviceTypeConfigDlg =  NULL;
	memset(&g_recv_packet,0,sizeof(RECV_PACKET));
	m_pTitleBmp = NULL;
	m_pBackBmp = NULL;
	g_pUser = new T_USER;
}

CPtuDlg::~CPtuDlg()
{
	if (m_pPresetSettingDlg != NULL)
	{
		delete m_pPresetSettingDlg;
		m_pPresetSettingDlg = NULL;
	}
	if (m_pPtuManageDlg != NULL)
	{
		delete m_pPtuManageDlg;
		m_pPtuManageDlg = NULL;
	}
	if (m_pDeviceLedgerDlg != NULL)
	{
		delete m_pDeviceLedgerDlg;
		m_pDeviceLedgerDlg = NULL;
	}
	if (g_pUser != NULL)
	{
		delete g_pUser;
		g_pUser = NULL;
	}
	if (m_pLinkageConfigDlg != NULL)
	{
		delete m_pLinkageConfigDlg;
		m_pLinkageConfigDlg = NULL;
	}
	if (m_pDeviceTypeConfigDlg != NULL)
	{
		delete m_pDeviceTypeConfigDlg;
		m_pDeviceTypeConfigDlg = NULL;
	}
}

void CPtuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_STATION_LIST, m_treeList);
	DDX_Control(pDX, IDC_BUTTON_PTU_CONFIG, m_btnPtuConfig);
}


BEGIN_MESSAGE_MAP(CPtuDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PRESET_CONFIG, &CPtuDlg::OnBnClickedButtonPresetConfig)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_PTU_CONFIG, &CPtuDlg::OnBnClickedButtonPtuConfig)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_STATION_LIST, &CPtuDlg::OnNMRClickTreeStationList)
	ON_COMMAND(ID_MENU_VIEW_PTU_INFO, &CPtuDlg::OnMenuViewPtuInfo)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_STATION_LIST, &CPtuDlg::OnNMDblclkTreeStationList)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_DEVICE_LEDGER_MANAGE, &CPtuDlg::OnBnClickedButtonDeviceLedgerManage)
	ON_BN_CLICKED(IDC_BUTTON_LINKAGE_CONFIG, &CPtuDlg::OnBnClickedButtonLinkageConfig)
	ON_BN_CLICKED(IDC_BUTTON_DEVICE_TYPE_CONFIG, &CPtuDlg::OnBnClickedButtonDeviceTypeConfig)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CPtuDlg::OnBnClickedButtonSearch)
END_MESSAGE_MAP()


// CPtuDlg 消息处理程序
int VideoDirectDeviceGetOpenVideoCB(int id, int nresult)
{
	return 0;
}


BOOL CPtuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_pMainDlg = this;

	//获得窗口在屏幕坐标系中的位置
	ModifyStyle(0, WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	ModifyStyleEx(0,  WS_EX_APPWINDOW);
	SetWindowText(_T("ACSSystem"));

	CRect WindowRect;
	GetWindowRect(&WindowRect);

	//获得客户区在屏幕坐标系中的位置
	CRect ClientRect;
	RepositionBars(0,0xffff,AFX_IDW_PANE_FIRST,reposQuery,&ClientRect);
	ClientToScreen(&ClientRect);
	//获得全屏大小（不包含任务栏）
	CRect rtClient;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rtClient,0);
	int nFullWidth = rtClient.Width();
	int nFullHeight = rtClient.Height();
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);

	m_WndRect.left = 0;
	m_WndRect.right = nWidth;
	m_WndRect.top = 0 + MAIN_TITLE_HEIGHT + 7;
	m_WndRect.bottom = nHeight;

	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof(WINDOWPLACEMENT);
	wndpl.flags = 0;
	wndpl.showCmd = SW_HIDE;
	wndpl.rcNormalPosition = m_WndRect;

	SetWindowPlacement(&wndpl);

	InitDVR();
	
	//辅助系统操作记录
	g_AssUserOperateLog.SetAssUserOperateLog(g_pConfigInfo->szLocalUdpIp,g_pConfigInfo->szDBServerIp,g_pConfigInfo->nDBServerPort,g_pConfigInfo->szDBName,"mdcs","mdcs2008");
	g_AssUserOperateLog.StartAssUserOperateLog();

	//连接数据库
	if (ConnectMySql()<0)
	{
		MessageBox("连接数据库失败，程序将退出！","辅助系统");
		exit(-1);
	}

	//将数据库中的数据放到内存中
	ReadStationCtUser();
	ReadRVUStationByUserID(1);
	ReadRVUInfo();
	ReadRVUDevice();
	ReadSubSystemNode();

	//初始化本地数据库
	if (!InitLocalAllDBTable())
	{
		MessageBox("初始化本地数据库失败，程序将退出！","辅助系统");
		exit(-1);
	}

	//通过用户名获取用户id
	GetUserInfoByUserName(g_pConfigInfo->szUserName);

	////tcp初始化
	//int result = ComInitNet(-1,NET_IOCP,10,100);
	//TRACE("---ComInitNet :result = %d ---\n\n",result);

	//线程池初始化
	ComInitThreadPool(&g_parse_rvu_thread,SINGLEFIFOTHREADPOOL|COPYARG,-1,1,100);

	////初始化每个rvu的连接
	//for (int i=0; i<g_nAssRVUNum; i++)
	//{
	//	g_tAssRVU[i].commnet_handle =  ComOpenNet("172.17.36.61",0,g_tAssRVU[i].rvu_ip,g_tAssRVU[i].rvu_port,NETCLIENT,TCP,RECVBUFSIZE,SOCKETRECVBUFSIZE,SOCKETSENDBUFSIZE,10,-1,10,recv_parse,NULL,0);
	//}

	//初始化各个子窗口
	InitChildWindow();

	//初始化控件皮肤
	InitControlSkin();

	//初始化变电站列表
	InitStationList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CPtuDlg::GetUserInfoByUserName(char *szUserName)
{
	if (szUserName == NULL)
		return FALSE;

	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"select user_id from ct_user where user_name='%s'", szUserName);
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			g_pUser->user_id = atoi(row[0]);
			sprintf_s(g_pUser->szUserName,256,szUserName);
		}
		mysql_free_result(res);
	}

	return TRUE;
}

void CPtuDlg::InitControlSkin()
{
	m_pBackBmp = new CBitmap;
	m_pBackBmp->LoadBitmap(IDB_BITMAP_BACK);

	m_pTitleBmp = new CBitmap;
	m_pTitleBmp->LoadBitmap(IDB_BITMAP_INTERVAL);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_TREE);
	m_ilTreeIcons.Create(16,16,ILC_MASK|ILC_COLOR24,0,1);
	m_ilTreeIcons.Add(&bmp,RGB(255,0,255));

	m_treeList.SetImageList(&m_ilTreeIcons,TVSIL_NORMAL);
}

BOOL CPtuDlg::InitDVR()
{
	bool ret = Init_DevSdk();
	return ret;
}

BOOL CPtuDlg::InitLocalAllDBTable()
{
	char sql_buf[1024] = {0x0};

	MYSQL_RES* res = NULL;
	MYSQL_ROW row = NULL;

	int rnums = 0;

	//设备台账表
	sprintf_s(sql_buf,"%s","SHOW TABLES like 'ass_device_ledger'");
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		rnums = mysql_num_rows(res);
		mysql_free_result( res ) ;
	}

	if (rnums == 0)
	{
		sprintf_s(sql_buf,"%s","CREATE TABLE `ass_device_ledger` ( "
			" `id` int(16) NOT NULL auto_increment, "
			" `station_id` int(16) NOT NULL, "
			" `device_manufacturer` char(32) NOT NULL, "
			" `device_ip` char(32) NOT NULL, "
			" `device_port` int(16) NOT NULL, "
			" `login_username` char(32) NOT NULL, "
			" `login_password` char(32) NOT NULL, "
			" `protocol_type` char(32) NOT NULL, "
			" `device_type` char(32) default NULL, "
			" `harddisk_capacity` char(32) default NULL, "
			" `analog_video_num` int(16) default NULL, "
			" `net_video_num` int(16) default NULL, "
			" PRIMARY KEY  (`id`) "
			" ) ENGINE=InnoDB DEFAULT CHARSET=utf8;");
		if (!mysql_query(g_LinkageSettingMysql, sql_buf))
		{
			res = mysql_store_result(g_LinkageSettingMysql);

			mysql_free_result(res) ;
		}
		else
		{
			MessageBox("创建ass_device_ledger数据表失败","辅助系统");
			return FALSE;
		}
	}

	//rnums = 0;
	//sprintf_s(sql_buf,"%s","SHOW TABLES like 'acs_station_ct_user'");
	//if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	//{
	//	res = mysql_store_result(g_LinkageSettingMysql);
	//	rnums = mysql_num_rows(res);
	//	mysql_free_result( res ) ;
	//}
	//if (rnums == 0)
	//{
	//	sprintf_s(sql_buf,"%s","CREATE TABLE `acs_station_ct_user` (`user_id` smallint(16) NOT NULL auto_increment,`system_id` char(20) NOT NULL default '',"
	//		"`user_name` char(64) NOT NULL default '',`pass_word` char(32) NOT NULL default '',`user_type` smallint(8) NOT NULL default '0',"
	//		"`enablectrl` smallint(8) NOT NULL default '0',`group_no` smallint(8) NOT NULL default '0',`pic_system_id` smallint(8) NOT NULL default '0',"
	//		"`picture_no` smallint(8) NOT NULL default '0',`alias_name` char(64) NOT NULL default '',`realtalk_p` tinyint(4) NOT NULL default '0',"
	//		"`eventlog_p` tinyint(4) NOT NULL default '0',`replay_p` tinyint(4) NOT NULL default '0',`alarm_p` tinyint(4) NOT NULL default '0',"
	//		"`tunecycle_p` tinyint(4) NOT NULL default '0',`usermanager_p` tinyint(4) NOT NULL default '0',`devicemanager_p` tinyint(4) NOT NULL default '0',"
	//		"`timerecord_p` tinyint(4) NOT NULL default '0',`elecmap_p` tinyint(4) NOT NULL default '0',`user_level` tinyint(4) NOT NULL default '0',"
	//		"`node_id` tinyint(4) NOT NULL default '0',`call_num` char(32) NOT NULL default '',	`call_password` char(32) NOT NULL default '',	PRIMARY KEY  (`user_id`)) TYPE=MyISAM;");
	//	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	//	{
	//		res = mysql_store_result(g_LinkageSettingMysql);

	//		mysql_free_result(res) ;
	//	}
	//	else
	//	{
	//		MessageBox("创建acs_station_ct_user数据表失败","辅助系统");
	//		return FALSE;
	//	}
	//}

	//rnums = 0;
	//sprintf_s(sql_buf,"%s","SHOW TABLES like 'acs_station_ob_access_dev'");
	//if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	//{
	//	res = mysql_store_result(g_LinkageSettingMysql);
	//	rnums = mysql_num_rows(res);
	//	mysql_free_result( res ) ;
	//}
	//if (rnums == 0)
	//{
	//	sprintf_s(sql_buf,"%s","CREATE TABLE `acs_station_ob_access_dev` (`id` int(16) NOT NULL auto_increment,`dev_code_sys` varchar(32) NOT NULL default '',"
	//		"`node_code_id` int(16) NOT NULL default '0',`dev_name` varchar(64) NOT NULL default '',`connect_type` smallint(4) NOT NULL default '0',"
	//		"`ip_addr` varchar(32) NOT NULL default '',`ip_protocol` varchar(32) NOT NULL default '',`ip_port` int(16) NOT NULL default '0',`local_addr` varchar(32) NOT NULL default '',`local_port` int(16) NOT NULL default '0',"
	//		"`serial_addr` varchar(32) NOT NULL default '',`serial_port` int(16) NOT NULL default '0',`serial_bandrate` int(16) NOT NULL default '0',"
	//		"`serial_data` int(16) NOT NULL default '0',`serial_stopdata` int(16) NOT NULL default '0',`serial_parity` int(16) NOT NULL default '0',"
	//		"`access_username` varchar(32) NOT NULL default '',`access_password` varchar(32) NOT NULL default '',`dev_type` smallint(4) NOT NULL default '0',"
	//		"`factory_type` smallint(4) NOT NULL default '0',`capacity_yx_num` smallint(4) NOT NULL default '0',`capacity_yc_num` smallint(4) NOT NULL default '0',"
	//		"`capacity_yk_num` smallint(4) NOT NULL default '0',`capacity_yt_num` smallint(4) NOT NULL default '0',`capacity_camera_num` smallint(4) NOT NULL default '0',`dev_online_state` smallint(4) NOT NULL default '0',"
	//		"`notes` varchar(64) NOT NULL default '',`order_num` smallint(8) NOT NULL default '0',	PRIMARY KEY  (`id`)	) TYPE=MyISAM;");
	//	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	//	{
	//		res = mysql_store_result(g_LinkageSettingMysql);
	//		mysql_free_result(res) ;
	//	}
	//	else
	//	{
	//		MessageBox("创建acs_station_ob_access_dev数据表失败","辅助系统");
	//		return FALSE;
	//	}
	//}

	//rnums = 0;
	//sprintf_s(sql_buf,"%s","SHOW TABLES like 'acs_station_ob_camera'");
	//if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	//{
	//	res = mysql_store_result(g_LinkageSettingMysql);
	//	rnums = mysql_num_rows(res);
	//	mysql_free_result( res ) ;
	//}
	//if (rnums == 0)
	//{
	//	sprintf_s(sql_buf,"%s","CREATE TABLE `acs_station_ob_camera` (`id` int(16) NOT NULL auto_increment,`camera_code_sys` varchar(32) NOT NULL default '',"
	//		"`accessdev_code_id` int(16) NOT NULL default '0',`camera_name` varchar(64) NOT NULL default '',`camera_channel` smallint(4) NOT NULL default '0',"
	//		"`dev_online_state` smallint(4) NOT NULL default '0',`notes` varchar(64) NOT NULL default '',`order_num` int(16) NOT NULL default '0',PRIMARY KEY  (`id`)) TYPE=MyISAM;");
	//	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	//	{
	//		res = mysql_store_result(g_LinkageSettingMysql);
	//		mysql_free_result(res) ;
	//	}
	//	else
	//	{
	//		MessageBox("创建acs_station_ob_camera数据表失败","辅助系统");
	//		return FALSE;
	//	}
	//}

	//rnums = 0;
	//sprintf_s(sql_buf,"%s","SHOW TABLES like 'acs_station_ob_card'");
	//if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	//{
	//	res = mysql_store_result(g_LinkageSettingMysql);
	//	rnums = mysql_num_rows(res);
	//	mysql_free_result( res ) ;
	//}
	//if (rnums == 0)
	//{
	//	sprintf_s(sql_buf,"%s","CREATE TABLE `acs_station_ob_card` (`id` smallint(8) NOT NULL auto_increment,`code` varchar(32) NOT NULL default '',"
	//		"`code_sys` varchar(32) NOT NULL default '',`user_name` varchar(32) NOT NULL default '',`personal_code` varchar(32) NOT NULL default '',"
	//		"`department` varchar(32) NOT NULL default '',`sex` smallint(4) NOT NULL default '0',`notes` varchar(64) NOT NULL default '',PRIMARY KEY  (`id`)) TYPE=MyISAM;");
	//	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	//	{
	//		res = mysql_store_result(g_LinkageSettingMysql);
	//		mysql_free_result(res) ;
	//	}
	//	else
	//	{
	//		MessageBox("创建acs_station_ob_card数据表失败","辅助系统");
	//		return FALSE;
	//	}
	//}

	//rnums = 0;
	//sprintf_s(sql_buf,"%s","SHOW TABLES like 'acs_station_ob_node'");
	//if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	//{
	//	res = mysql_store_result(g_LinkageSettingMysql);
	//	rnums = mysql_num_rows(res);
	//	mysql_free_result( res ) ;
	//}
	//if (rnums == 0)
	//{
	//	sprintf_s(sql_buf,"%s","CREATE TABLE `acs_station_ob_node` (`id` smallint(8) NOT NULL auto_increment,`parent_id` smallint(8) NOT NULL default '0',"
	//		"`sys_id` varchar(32) NOT NULL default '',`node_name` varchar(64) NOT NULL default '',`node_sysid` varchar(32) NOT NULL default '',"
	//		"`node_type` smallint(16) NOT NULL default '0',`node_level` smallint(16) NOT NULL default '0',`notes` varchar(64) NOT NULL default '',"
	//		"`order_num` smallint(8) NOT NULL default '0',	PRIMARY KEY  (`id`)) TYPE=MyISAM;");
	//	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	//	{
	//		res = mysql_store_result(g_LinkageSettingMysql);
	//		mysql_free_result(res) ;
	//	}
	//	else
	//	{
	//		MessageBox("创建acs_station_ob_node数据表失败","辅助系统");
	//		return FALSE;
	//	}
	//}

	//rnums = 0;
	//sprintf_s(sql_buf,"%s","SHOW TABLES like 'acs_station_ob_yc'");
	//if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	//{
	//	res = mysql_store_result(g_LinkageSettingMysql);
	//	rnums = mysql_num_rows(res);
	//	mysql_free_result( res ) ;
	//}
	//if (rnums == 0)
	//{
	//	sprintf_s(sql_buf,"%s","CREATE TABLE `acs_station_ob_yc` (`id` smallint(16) NOT NULL auto_increment,`yc_code_sys` varchar(32) NOT NULL default '',"
	//		"`accessdev_code_id` smallint(16) NOT NULL default '0',`yc_type` smallint(4) NOT NULL default '0',`yc_name` varchar(64) NOT NULL default '',"
	//		"`yc_addr` varchar(32) NOT NULL default '',`up_up_value` float(6,2) NOT NULL default '0.00',`up_value` float(6,2) NOT NULL default '0.00',"
	//		"`value` float(6,2) NOT NULL default '0.00',`low_value` float(6,2) NOT NULL default '0.00',`low_low_value` float(6,2) NOT NULL default '0.00',"
	//		"`units` varchar(8) NOT NULL default '',`dec_point` smallint(4) NOT NULL default '0',`algorithm` smallint(4) NOT NULL default '0',"
	//		"`ratio` float(16,3) NOT NULL default '0.000',`offset` float(16,3) NOT NULL default '0.000',`dev_online_state` smallint(4) NOT NULL default '0',"
	//		"`notes` varchar(64) NOT NULL default '',`order_num` smallint(8) NOT NULL default '0',PRIMARY KEY  (`id`)) TYPE=MyISAM;");
	//	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	//	{
	//		res = mysql_store_result(g_LinkageSettingMysql);
	//		mysql_free_result(res) ;
	//	}
	//	else
	//	{
	//		MessageBox("创建acs_station_ob_yc数据表失败","辅助系统");
	//		return FALSE;
	//	}
	//}

	//rnums = 0;
	//sprintf_s(sql_buf,"%s","SHOW TABLES like 'acs_station_ob_yk'");
	//if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	//{
	//	res = mysql_store_result(g_LinkageSettingMysql);
	//	rnums = mysql_num_rows(res);
	//	mysql_free_result( res ) ;
	//}
	//if (rnums == 0)
	//{
	//	sprintf_s(sql_buf,"%s","CREATE TABLE `acs_station_ob_yk` (`id` smallint(16) NOT NULL auto_increment,`yk_code_sys` varchar(32) NOT NULL default '',"
	//		"`accessdev_code_id` smallint(16) NOT NULL default '0',`yk_type` smallint(4) NOT NULL default '0',`yk_name` varchar(64) NOT NULL default '',"
	//		"`yk_addr` varchar(32) NOT NULL default '',`normal_state` smallint(4) NOT NULL default '0',`state` smallint(4) NOT NULL default '0',"
	//		"`algorithm` smallint(4) NOT NULL default '0',`ratio` float(16,3) NOT NULL default '0.000',`offset` float(16,3) NOT NULL default '0.000',"
	//		"`dev_online_state` smallint(4) NOT NULL default '0',`notes` varchar(64) NOT NULL default '',`order_num` smallint(8) NOT NULL default '0',"
	//		"PRIMARY KEY  (`id`)) TYPE=MyISAM;");
	//	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	//	{
	//		res = mysql_store_result(g_LinkageSettingMysql);
	//		mysql_free_result(res) ;
	//	}
	//	else
	//	{
	//		MessageBox("创建acs_station_ob_yk数据表失败","辅助系统");
	//		return FALSE;
	//	}
	//}

	//rnums = 0;
	//sprintf_s(sql_buf,"%s","SHOW TABLES like 'acs_station_ob_yt'");
	//if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	//{
	//	res = mysql_store_result(g_LinkageSettingMysql);
	//	rnums = mysql_num_rows(res);
	//	mysql_free_result( res ) ;
	//}
	//if (rnums == 0)
	//{
	//	sprintf_s(sql_buf,"%s","CREATE TABLE `acs_station_ob_yt` (`id` smallint(16) NOT NULL auto_increment,`yt_code_sys` varchar(32) NOT NULL default '',"
	//		"`accessdev_code_id` smallint(16) NOT NULL default '0',`yt_type` smallint(4) NOT NULL default '0',`yt_name` varchar(64) NOT NULL default '',"
	//		"`yt_addr` varchar(32) NOT NULL default '',`setting_value` float(16,3) NOT NULL default '0.000',`current_value` float(16,3) NOT NULL default '0.000',"
	//		"`algorithm` smallint(4) NOT NULL default '0',`ratio` float(16,0) NOT NULL default '0',`offset` float(16,0) NOT NULL default '0',"
	//		"`dev_online_state` smallint(4) NOT NULL default '0',`notes` varchar(64) NOT NULL default '',`order_num` smallint(16) NOT NULL default '0',"
	//		"PRIMARY KEY  (`id`)) TYPE=MyISAM;");
	//	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	//	{
	//		res = mysql_store_result(g_LinkageSettingMysql);
	//		mysql_free_result(res) ;
	//	}
	//	else
	//	{
	//		MessageBox("创建acs_station_ob_yt数据表失败","辅助系统");
	//		return FALSE;
	//	}
	//}

	//rnums = 0;
	//sprintf_s(sql_buf,"%s","SHOW TABLES like 'acs_station_ob_yx'");
	//if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	//{
	//	res = mysql_store_result(g_LinkageSettingMysql);
	//	rnums = mysql_num_rows(res);
	//	mysql_free_result( res ) ;
	//}
	//if (rnums == 0)
	//{
	//	sprintf_s(sql_buf,"%s","CREATE TABLE `acs_station_ob_yx` (`id` smallint(16) NOT NULL auto_increment,`yx_code_sys` varchar(32) NOT NULL default '',"
	//		"`accessdev_code_id` smallint(16) NOT NULL default '0',`yx_type` smallint(4) NOT NULL default '0',`yx_name` varchar(64) NOT NULL default '',"
	//		"`yx_addr` varchar(32) NOT NULL default '',`normal_state` smallint(4) NOT NULL default '0',`current_state` smallint(4) NOT NULL default '0',"
	//		"`algorithm` smallint(4) NOT NULL default '0',`ratio` float(16,3) NOT NULL default '0.000',`offset` float(16,3) NOT NULL default '0.000',"
	//		"`dev_online_state` smallint(4) NOT NULL default '0',`notes` varchar(64) NOT NULL default '',`order_num` smallint(8) NOT NULL default '0',PRIMARY KEY  (`id`)) TYPE=MyISAM;");
	//	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	//	{
	//		res = mysql_store_result(g_LinkageSettingMysql);
	//		mysql_free_result(res) ;
	//	}
	//	else
	//	{
	//		MessageBox("创建acs_station_ob_yx数据表失败","辅助系统");
	//		return FALSE;
	//	}
	//}

	//rnums = 0;
	//sprintf_s(sql_buf,"%s","SHOW TABLES like 'acs_station_st_camera_preset'");
	//if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	//{
	//	res = mysql_store_result(g_LinkageSettingMysql);
	//	rnums = mysql_num_rows(res);
	//	mysql_free_result( res ) ;
	//}
	//if (rnums == 0)
	//{
	//	sprintf_s(sql_buf,"%s","CREATE TABLE `acs_station_st_camera_preset` (`id` int(32) NOT NULL auto_increment,`camera_id` int(16) NOT NULL default '0',"
	//		"`preset_id` int(16) NOT NULL default '0',`preset_name` varchar(32) NOT NULL default '',`notes` varchar(64) NOT NULL default '',PRIMARY KEY  (`id`)) TYPE=MyISAM;");
	//	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	//	{
	//		res = mysql_store_result(g_LinkageSettingMysql);
	//		mysql_free_result(res) ;
	//	}
	//	else
	//	{
	//		MessageBox("创建acs_station_st_camera_preset数据表失败","辅助系统");
	//		return FALSE;
	//	}
	//}

	//rnums = 0;
	//sprintf_s(sql_buf,"%s","SHOW TABLES like 'acs_station_st_relation'");
	//if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	//{
	//	res = mysql_store_result(g_LinkageSettingMysql);
	//	rnums = mysql_num_rows(res);
	//	mysql_free_result( res ) ;
	//}
	//if (rnums == 0)
	//{
	//	sprintf_s(sql_buf,"%s","CREATE TABLE `acs_station_st_relation` (`id` int(32) NOT NULL auto_increment,`event_access_dev_id` int(32) NOT NULL default '0',"
	//		"`event_dev_id` int(32) NOT NULL default '0',`event_type` smallint(4) NOT NULL default '0',`event_reason` smallint(4) NOT NULL default '0',"
	//		"`event_reason_text` varchar(64) NOT NULL default '',`action_access_dev_id` int(32) NOT NULL default '0',`action_dev_id` int(32) NOT NULL default '0',"
	//		"`action_level` smallint(4) NOT NULL default '0',`action_type` smallint(4) NOT NULL default '0',`action_type_text` varchar(64) NOT NULL default '',"
	//		"`action_pretime` smallint(4) NOT NULL default '0',`action_time` smallint(4) NOT NULL default '0',`action_timeinterval` smallint(4) NOT NULL default '0',"
	//		"`action_digital` smallint(4) NOT NULL default '0',`action_yk` smallint(4) NOT NULL default '0',`action_analog` varchar(32) NOT NULL default '',PRIMARY KEY  (`id`)) TYPE=MyISAM;");
	//	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	//	{
	//		res = mysql_store_result(g_LinkageSettingMysql);
	//		mysql_free_result(res) ;
	//	}
	//	else
	//	{
	//		MessageBox("创建acs_station_st_relation数据表失败","辅助系统");
	//		return FALSE;
	//	}
	//}
	return TRUE;
}

// 连接数据库
bool CPtuDlg::ConnectMySql(void)
{
	int times = 0;

	for (times = 0; times < 3; times ++)
	{
		if ((g_LinkageSettingMysql = mysql_init((MYSQL*)0)) && mysql_real_connect(g_LinkageSettingMysql, g_pConfigInfo->szDBServerIp, "mdcs", "mdcs2008", g_pConfigInfo->szDBName,g_pConfigInfo->nDBServerPort, NULL, times))
		{
			if (mysql_select_db(g_LinkageSettingMysql, g_pConfigInfo->szDBName) < 0)
			{
				TRACE("已成功连接mysql, 但选择数据库出错!\n");
				mysql_close(g_LinkageSettingMysql);
				g_LinkageSettingMysql = NULL;

				if (times >= 2)
					return false;
				else
					continue;
			}

			int nValue = 1;
			mysql_options(g_LinkageSettingMysql,MYSQL_OPT_RECONNECT,(char *)&nValue);

			mysql_query(g_LinkageSettingMysql,"SET NAMES 'GBK'");
			TRACE("已成功连接mysql!\n");

			return true;
		}
		else
		{
			mysql_close(g_LinkageSettingMysql);
			g_LinkageSettingMysql = NULL;

			TRACE("连接mysql失败!");

			if (times >= 2)
				return false;
			else
				continue;
		}
	}
	return true;
}

// 断开数据库
bool CPtuDlg::DisConnectMySql(void)
{
	mysql_close(g_LinkageSettingMysql);
	g_LinkageSettingMysql = NULL;
	return true;
}

void CPtuDlg::InitStationList()
{
	m_treeList.DeleteAllItems();

	char sql_buf[1024]={0x0};

	MYSQL_RES* res;
	MYSQL_ROW row;

	int rnum = 0;
	sprintf_s(sql_buf, "%s","SHOW TABLES LIKE 'ob_d5000_station'");
	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		rnum = mysql_num_rows(res);
		mysql_free_result( res ) ;
	}

	if (rnum == 0)
	{
		MessageBox("没有变电站信息，初始化失败","辅助系统");
		return ;
	}

	HTREEITEM hMainItem = NULL;
	HTREEITEM hItem1 = NULL;
	hMainItem = m_treeList.InsertItem("江苏省电力公司", 1, 1, TVI_ROOT, NULL);
	SetStationNodeInfo(hMainItem,-1,NULL,NULL,0,0);
	

	//1000KV
	hItem1 = m_treeList.InsertItem("1000KV变电站",7,7,hMainItem);
	SetStationNodeInfo(hItem1,-1,NULL,NULL,1,0);
	SetVolateClassStationToTree("1000", hItem1);

	//500kV
	hItem1 = m_treeList.InsertItem("500kV变电站",7,7,hMainItem);
	SetStationNodeInfo(hItem1,-1,NULL,NULL,1,0);
	SetVolateClassStationToTree("500", hItem1);

	//220kV
	hItem1 = m_treeList.InsertItem("220kV变电站",7,7,hMainItem);
	SetStationNodeInfo(hItem1,-1,NULL,NULL,1,0);
	SetVolateClassStationToTree("220", hItem1);

	//110kV
	hItem1 = m_treeList.InsertItem("110kV变电站",7,7,hMainItem);
	SetStationNodeInfo(hItem1,-1,NULL,NULL,1,0);
	SetVolateClassStationToTree("110", hItem1);

	//35kv
	hItem1 = m_treeList.InsertItem("110kV变电站",7,7,hMainItem);
	SetStationNodeInfo(hItem1,-1,NULL,NULL,1,0);
	SetVolateClassStationToTree("35", hItem1);

	//其它
	hItem1 = m_treeList.InsertItem("其他",7,7,hMainItem);
	SetStationNodeInfo(hItem1,-1,NULL,NULL,1,0);
	SetVolateClassStationToTree("", hItem1);

	ExpandAllItems(&m_treeList, NULL);
}

//获取某个电压等级下的变电站，添加到tree下
void CPtuDlg::SetVolateClassStationToTree(char* szVolClass, HTREEITEM hItem)
{
	CString strStationName = "";
	MYSQL_RES* res;
	MYSQL_ROW row;
	HTREEITEM hItemChild;
	char sql_buf[1024] = {0};
	sprintf_s(sql_buf,"SELECT a.station_id,a.station_name_videoplant,a.station_code_videoplant,IFNULL(b.rvu_id,0) FROM ob_d5000_station as a"
		" left join ass_rvu as b on a.station_id=b.station_id where a.voltage_class = '%s'", szVolClass);
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			//如果变电站绑定了rvu，增加(已绑定）字符串
			strStationName = GetStationTreeShowText(atoi(row[3]), row[1]);

			hItemChild = m_treeList.InsertItem(strStationName, 8, 8, hItem);
			SetStationNodeInfo(hItemChild,atoi(row[0]), row[2], row[1], 2, 0);
		}
		mysql_free_result(res);
	}
}

//获取变电站树节点显示文本，如果变电站绑定了rvu，则增加已绑定字符串
CString CPtuDlg::GetStationTreeShowText(int nFlag, CString strStationName)
{
	CString returnText;
	if (nFlag == 0)
		returnText.Format("%s", strStationName);
	else
		returnText.Format("%s(已绑定)", strStationName);

	return returnText;
}

void CPtuDlg::SetStationNodeInfo(HTREEITEM hItem,int station_id,char* station_code,char* station_name,int node_type,int connect_state)
{
	StationNode* pStationNode = NULL;
	pStationNode = new StationNode;
	memset(pStationNode,0,sizeof(StationNode));

	sprintf_s(pStationNode->station_code,"%s",station_code);
	sprintf_s(pStationNode->station_name,"%s",station_name);
	pStationNode->node_type = node_type;
	pStationNode->station_id = station_id;
	pStationNode->connect_ptu = connect_state;

	m_treeList.SetItemData(hItem,(DWORD)pStationNode);
}

void CPtuDlg::InitChildWindow()
{
	CRect winrc;
	GetClientRect(&winrc);

	CRect groupboxRect;
	GetDlgItem(IDC_STATIC_PICTURE)->GetWindowRect(&groupboxRect);

	CRect presetdlg_rect;
	presetdlg_rect.top = 0;
	presetdlg_rect.bottom = (groupboxRect.bottom-groupboxRect.top);
	presetdlg_rect.left = 0;
	presetdlg_rect.right = (groupboxRect.right - groupboxRect.left) ;

	CRect ptumanagedlg_rect;
	ptumanagedlg_rect.top = 0;
	ptumanagedlg_rect.bottom = (groupboxRect.bottom-groupboxRect.top) - 0;
	ptumanagedlg_rect.left = 0;
	ptumanagedlg_rect.right = (groupboxRect.right - groupboxRect.left) - 0;

	CRect deviceledgerdlg_rect;
	deviceledgerdlg_rect.top = 0;
	deviceledgerdlg_rect.bottom = (groupboxRect.bottom-groupboxRect.top) - 0;
	deviceledgerdlg_rect.left = 0;
	deviceledgerdlg_rect.right = (groupboxRect.right - groupboxRect.left) - 0;

	CRect linkageconfigdlg_rect;
	linkageconfigdlg_rect.top = 0;
	linkageconfigdlg_rect.bottom = (groupboxRect.bottom-groupboxRect.top) - 0;
	linkageconfigdlg_rect.left = 0;
	linkageconfigdlg_rect.right = (groupboxRect.right - groupboxRect.left) - 0;

	CRect devicetypeconfigdlg_rect;
	devicetypeconfigdlg_rect.top = 0;
	devicetypeconfigdlg_rect.bottom = (groupboxRect.bottom-groupboxRect.top) - 0;
	devicetypeconfigdlg_rect.left = 0;
	devicetypeconfigdlg_rect.right = (groupboxRect.right - groupboxRect.left) - 0;

	m_pPresetSettingDlg = new CDlgPresetSetting;
	if (m_pPresetSettingDlg != NULL)
	{
		m_pPresetSettingDlg->Create(IDD_DIALOG_PRESET_SETTING,this);
		m_pPresetSettingDlg->SetParent(GetDlgItem(IDC_STATIC_PICTURE));
		m_pPresetSettingDlg->MoveWindow(&presetdlg_rect,TRUE);
		m_pPresetSettingDlg->ShowWindow(SW_HIDE);
	}

	m_pPtuManageDlg = new CDlgPtuManage;
	if (m_pPtuManageDlg != NULL)
	{
		m_pPtuManageDlg->Create(IDD_DIALOG_PTU_MANAGE,this);
		m_pPtuManageDlg->SetParent(GetDlgItem(IDC_STATIC_PICTURE));
		m_pPtuManageDlg->MoveWindow(&presetdlg_rect,TRUE);
		m_pPtuManageDlg->ShowWindow(SW_SHOW);
	}

	m_pDeviceLedgerDlg = new CDlgDeviceLedger;
	if (m_pDeviceLedgerDlg!=NULL)
	{
		m_pDeviceLedgerDlg->Create(IDD_DIALOG_DEVICE_LEDGER,this);
		m_pDeviceLedgerDlg->SetParent(GetDlgItem(IDC_STATIC_PICTURE));
		m_pDeviceLedgerDlg->MoveWindow(&deviceledgerdlg_rect,TRUE);
		m_pDeviceLedgerDlg->ShowWindow(SW_HIDE);
	}

	m_pLinkageConfigDlg = new CDlgLinkageConfig;
	if (m_pLinkageConfigDlg != NULL)
	{
		m_pLinkageConfigDlg->Create(IDD_DIALOG_LINKAGE_CONFIG,this);
		m_pLinkageConfigDlg->SetParent(GetDlgItem(IDC_STATIC_PICTURE));
		m_pLinkageConfigDlg->MoveWindow(&linkageconfigdlg_rect,TRUE);
		m_pLinkageConfigDlg->ShowWindow(SW_HIDE);
	}

	m_pDeviceTypeConfigDlg = new CDlgDeviceTypeConfig;
	if (m_pDeviceTypeConfigDlg != NULL)
	{
		m_pDeviceTypeConfigDlg->Create(IDD_DIALOG_DEVICE_TYPE_CONFIG,this);
		m_pDeviceTypeConfigDlg->SetParent(GetDlgItem(IDC_STATIC_PICTURE));
		m_pDeviceTypeConfigDlg->MoveWindow(&devicetypeconfigdlg_rect,TRUE);
		m_pDeviceTypeConfigDlg->ShowWindow(SW_HIDE);
	}
}

void CPtuDlg::OnBnClickedButtonPresetConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pPresetSettingDlg->ShowWindow(TRUE);
	m_pPtuManageDlg->ShowWindow(SW_HIDE);
	m_pDeviceLedgerDlg->ShowWindow(SW_HIDE);
	m_pLinkageConfigDlg->ShowWindow(SW_HIDE);
	m_pDeviceTypeConfigDlg->ShowWindow(SW_HIDE);
	//ShowOneDlg(IDD_DIALOG_PRESET_SETTING);
}

void CPtuDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd* pWnd = NULL;
	CRect winrc,button_rect;

	GetClientRect(&winrc);

	int nButtonWidth = 150;
	int nButtonHeight = 30;
	int nval = 2;

	int nTreeWidth = 0.15 * cx;
	int nTreeHeight = cy - 30;

	int nSearchEditHeight = 30;
	int nSearchEditWidth = 0.15 * cx * 0.7;

	int nSearchBtnHeight = 30;
	int nSerachBtnWidth = 0.15 * cx * 0.3;

	int nCloumnInterval = 7;

	//pWnd = GetDlgItem(IDC_STATIC_STATION_LIST);
	//if (pWnd != NULL)
	//{
	//	button_rect.top = winrc.top+10;
	//	button_rect.bottom = button_rect.top + nButtonHeight;
	//	button_rect.left = winrc.left + 20;
	//	button_rect.right = button_rect.left + nTreeWidth;
	//	pWnd->MoveWindow(button_rect);
	//	pWnd->ShowWindow(SW_SHOW);
	//}

	pWnd = GetDlgItem(IDC_TREE_STATION_LIST);
	if (pWnd != NULL)
	{
		button_rect.top = winrc.top;
		button_rect.bottom = button_rect.top + nTreeHeight;
		button_rect.left = winrc.left;
		button_rect.right = button_rect.left + nTreeWidth;
		pWnd->MoveWindow(button_rect);
		pWnd->ShowWindow(SW_SHOW);
	}

	//搜索文本框
	pWnd = GetDlgItem(IDC_EDIT_SEARCH);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.bottom;
		button_rect.bottom = button_rect.top + nSearchEditHeight;
		button_rect.left = winrc.left;
		button_rect.right = button_rect.left + nSearchEditWidth;
		pWnd->MoveWindow(button_rect);
		pWnd->ShowWindow(SW_SHOW);
	}

	//搜索按钮
	pWnd = GetDlgItem(IDC_BUTTON_SEARCH);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nSearchBtnHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nSerachBtnWidth;
		pWnd->MoveWindow(button_rect);
		pWnd->ShowWindow(SW_SHOW);
	}

	pWnd = GetDlgItem(IDC_BUTTON_PTU_CONFIG);
	if (pWnd != NULL)
	{
		button_rect.top = winrc.top;
		button_rect.bottom = winrc.top + nButtonHeight;
		button_rect.left = button_rect.right + nCloumnInterval;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
		pWnd->ShowWindow(SW_SHOW);
	}

	pWnd = GetDlgItem(IDC_BUTTON_PRESET_CONFIG);
	if (pWnd != NULL)
	{
		button_rect.top = winrc.top;
		button_rect.bottom = winrc.top + nButtonHeight;
		button_rect.left = button_rect.left + nButtonWidth;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
		pWnd->ShowWindow(SW_SHOW);
	}

	pWnd = GetDlgItem(IDC_BUTTON_DEVICE_LEDGER_MANAGE);
	if (pWnd != NULL)
	{
		button_rect.top = winrc.top;
		button_rect.bottom = winrc.top + nButtonHeight;
		button_rect.left = button_rect.left + nButtonWidth;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
		pWnd->ShowWindow(SW_SHOW);
	}

	pWnd = GetDlgItem(IDC_BUTTON_DEVICE_TYPE_CONFIG);
	if (pWnd != NULL)
	{
		button_rect.top = winrc.top;
		button_rect.bottom = winrc.top + nButtonHeight;
		button_rect.left = button_rect.left + nButtonWidth;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
		pWnd->ShowWindow(SW_SHOW);
	}

	//pWnd = GetDlgItem(IDC_BUTTON_LINKAGE_CONFIG);
	//if (pWnd != NULL)
	//{
	//	button_rect.top = winrc.top;
	//	button_rect.bottom = winrc.top + nButtonHeight;
	//	button_rect.left = button_rect.left + nButtonWidth;
	//	button_rect.right = button_rect.left + nButtonWidth;
	//	pWnd->MoveWindow(button_rect);
	//	pWnd->ShowWindow(SW_SHOW);
	//}

	pWnd = GetDlgItem(IDC_STATIC_PICTURE);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top + nButtonHeight;
		button_rect.bottom = winrc.bottom;
		button_rect.left = winrc.left + nTreeWidth + nCloumnInterval;
		button_rect.right = winrc.right;
		pWnd->MoveWindow(button_rect);
		pWnd->ShowWindow(SW_SHOW);
	}

	//pWnd = GetDlgItem(IDC_STATIC_GROUP_BOX);
	//if (pWnd != NULL)
	//{
	//	button_rect.top = button_rect.top + nButtonHeight;
	//	button_rect.bottom = winrc.bottom;
	//	button_rect.left = winrc.left + nTreeWidth + nCloumnInterval;
	//	button_rect.right = winrc.right;
	//	pWnd->MoveWindow(button_rect);
	//	pWnd->ShowWindow(SW_SHOW);
	//}
}

BOOL CPtuDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_pPresetSettingDlg != NULL)
	{
		if (m_pPresetSettingDlg->m_bVideoOpen == true)
		{
			m_pPresetSettingDlg->PreTranslateMessage(pMsg);
		}
	}

	if(m_pPtuManageDlg != NULL)
	{
		m_pPtuManageDlg->PreTranslateMessage(pMsg);
	}

	if(m_pDeviceLedgerDlg != NULL)
	{
		m_pDeviceLedgerDlg->PreTranslateMessage(pMsg);
	}

	if (m_pLinkageConfigDlg != NULL)
	{
		m_pLinkageConfigDlg->PreTranslateMessage(pMsg);
	}

	if (m_pDeviceTypeConfigDlg != NULL)
	{
		m_pDeviceTypeConfigDlg->PreTranslateMessage(pMsg);
	}
	
	return FALSE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CPtuDlg::OnBnClickedButtonPtuConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	//ShowOneDlg(IDD_DIALOG_PTU_MANAGE);
	m_pPresetSettingDlg->ShowWindow(SW_HIDE);
	m_pPtuManageDlg->ShowWindow(TRUE);
	m_pDeviceLedgerDlg->ShowWindow(SW_HIDE);
	m_pLinkageConfigDlg->ShowWindow(SW_HIDE);
	m_pDeviceTypeConfigDlg->ShowWindow(SW_HIDE);
}

void CPtuDlg::OnNMRClickTreeStationList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POINT pt;
	::GetCursorPos(&pt);//获取鼠标的位置

	UINT hit_flag;
	HTREEITEM h_item = NULL;
	
	m_treeList.ScreenToClient(&pt);

	// 查找点中的树结点项目
	h_item = m_treeList.HitTest(pt, &hit_flag);
	if (h_item == NULL)
		return;

	if ((hit_flag & TVHT_ONITEMICON) == 0 && (hit_flag & TVHT_ONITEMLABEL) == 0)
		return;

	m_treeList.SelectItem(h_item);

	CMenu  popMenu;

	popMenu.LoadMenu(IDR_MENU_PTU_TREELIST);
	CMenu *pMenu = popMenu.GetSubMenu(0); 

	CPoint posMouse;
	GetCursorPos(&posMouse);

	pMenu->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_RIGHTALIGN,  posMouse.x, posMouse.y, this); 
}

void CPtuDlg::OnMenuViewPtuInfo()
{
	// TODO: 在此添加命令处理程序代码
	try
	{
		HTREEITEM hItem = NULL;
		StationNode *pNodeInfo = NULL;

		// 查找点中的树结点项目
		hItem = m_treeList.GetSelectedItem();
		if (hItem == NULL)
			return;

		pNodeInfo = (StationNode *)m_treeList.GetItemData(hItem);
		//2表示变电站
		if (pNodeInfo == NULL||pNodeInfo->node_type != 2)
			return;

		if (m_pPtuManageDlg == NULL)
		{
			return;
		}

		//取出选中变电站的PTU信息
		char sql_buf[1024]={0x0};
		MYSQL_RES	* res;
		MYSQL_ROW	row;

		CString strPtuIPAddr = "";
		int nPtuPort = 0;
		CString strPtuUsername = "";
		CString strPtuPassword = "";
		CString strPtuPort = "";

		sprintf_s(sql_buf,"select rvu_ip,rvu_port,rvu_username,rvu_password from ass_rvu where station_id='%d'",pNodeInfo->station_id);
		if (!mysql_query(g_LinkageSettingMysql,sql_buf))
		{
			res = mysql_store_result(g_LinkageSettingMysql);
			row = mysql_fetch_row(res);
			if (row != NULL)
			{
				strPtuIPAddr.Format("%s",row[0]);
				strPtuPort.Format("%s",row[1]);
				nPtuPort = atoi(row[1]);
				strPtuUsername.Format("%s",row[2]);
				strPtuPassword.Format("%s",row[3]);
			}
			else
			{
				MessageBox("没找到该变电站所对应的PTU","辅助系统");
				//mysql_free_result(res);
				//return ;
			}
			mysql_free_result(res);
		}

		//处理PTU接入管理中的事件
		//m_pPtuManageDlg->SetEditControlText(strPtuIPAddr,strPtuPort,strPtuUsername,strPtuPassword,pNodeInfo->connect_ptu);

	}
	catch(...)
	{

	}
}

void CPtuDlg::OnNMDblclkTreeStationList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	try
	{
		HTREEITEM hItem = NULL;
		StationNode *pNodeInfo = NULL;

		// 查找点中的树结点项目
		hItem = m_treeList.GetSelectedItem();
		if (hItem == NULL)
			return;

		pNodeInfo = (StationNode *)m_treeList.GetItemData(hItem);
		//2表示变电站
		if (pNodeInfo == NULL||pNodeInfo->node_type != 2)
			return;

		if (m_pPtuManageDlg == NULL)
		{
			return;
		}

		GetDlgItem(IDC_STATIC_GROUP_BOX)->SetWindowText(pNodeInfo->station_name);

		//取出选中变电站的PTU信息
		char sql_buf[1024]={0x0};
		MYSQL_RES	* res;
		MYSQL_ROW	row;

		CString strPtuIPAddr = "";
		int nPtuPort = 0;
		CString strPtuUsername = "";
		CString strPtuPassword = "";
		CString strPtuPort = "";
		CString strPtuName = "";
		CString strRvuType = "";
		CString strRvuCode = "";

		sprintf_s(sql_buf,"select rvu_ip,rvu_port,rvu_username,rvu_password,rvu_name,rvu_protocol,rvu_code from ass_rvu where station_id='%d'",pNodeInfo->station_id);
		if (!mysql_query(g_LinkageSettingMysql,sql_buf))
		{
			res = mysql_store_result(g_LinkageSettingMysql);
			row = mysql_fetch_row(res);
			if (row != NULL)
			{
				strPtuIPAddr.Format("%s",row[0]);
				strPtuPort.Format("%s",row[1]);
				nPtuPort = atoi(row[1]);
				strPtuUsername.Format("%s",row[2]);
				strPtuPassword.Format("%s",row[3]);
				strPtuName.Format("%s",row[4]);
				strRvuType.Format("%s",row[5]);
				strRvuCode.Format("%s", row[6]);
			}
			else
			{
				MessageBox("没找到该变电站所对应的PTU","辅助系统");
				//mysql_free_result(res);
				//return ;
			}
			mysql_free_result(res);
		}

		//处理PTU接入管理中的事件
		m_pPtuManageDlg->SetEditControlText(strPtuIPAddr,strPtuPort,strPtuUsername,strPtuPassword,pNodeInfo->connect_ptu,strPtuName,strRvuType, strRvuCode);
		m_pPtuManageDlg->InialRvuDeviceList(pNodeInfo);
		//m_pPtuManageDlg->InialRouteRvuDeviceList(pNodeInfo);
		//m_pPtuManageDlg->InialSubSystemList(pNodeInfo);
		m_pPtuManageDlg->SetStationId(pNodeInfo->station_id);
		m_pPtuManageDlg->SetStationNode(pNodeInfo);

		//处理预置位管理窗口的事件
		m_pPresetSettingDlg->SetStationNode(pNodeInfo);
		m_pPresetSettingDlg->InitPresettingList(pNodeInfo);
		m_pPresetSettingDlg->InitLinkageList(pNodeInfo);
		m_pPresetSettingDlg->InitLinkageRelationList(pNodeInfo);

		//处理设备台账管理窗口的事件
		m_pDeviceLedgerDlg->SetStationNode(pNodeInfo);
		m_pDeviceLedgerDlg->InitDeviceLedgerListByStation(pNodeInfo);

		//处理联动配置窗口的事件
		m_pLinkageConfigDlg->SetStationNode(pNodeInfo);
		m_pLinkageConfigDlg->RefreshLinkageConfigDlg(pNodeInfo);

	}
	catch(...)
	{

	}
}

void CPtuDlg::FreeStationList(HTREEITEM hItem)
{
	StationNode* pStationNode = NULL;
	//释放子节点
	if (m_treeList.ItemHasChildren(hItem))
	{
		if (hItem != NULL)
		{
			pStationNode = (StationNode*)m_treeList.GetItemData(hItem);
			if (pStationNode != NULL)
			{
				delete pStationNode;
				pStationNode = NULL;
				m_treeList.SetItemData(hItem,NULL);
			}
		}

		HTREEITEM  hChild = m_treeList.GetChildItem(hItem);
		if (hChild != NULL)
		{
			pStationNode = (StationNode*)m_treeList.GetItemData(hChild);
			if (pStationNode != NULL)
			{
				delete pStationNode;
				pStationNode = NULL;
				m_treeList.SetItemData(hChild,NULL);
			}
		}

		while (hChild != NULL)
		{
			if (m_treeList.ItemHasChildren(hChild))
			{
				FreeStationList(hChild);
			}


			hChild = m_treeList.GetNextSiblingItem(hChild);

			if (hChild != NULL)
			{
				pStationNode = (StationNode*)m_treeList.GetItemData(hChild);
				if (pStationNode != NULL)
				{
					delete pStationNode;
					pStationNode = NULL;
					m_treeList.SetItemData(hChild,NULL);
				}
			}
		}
	}
}

BOOL CPtuDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp;
	BITMAP bmp;
	pOldBmp = srcDC.SelectObject(m_pBackBmp);
	m_pBackBmp->GetBitmap(&bmp);
	pDC->StretchBlt(0,0,m_WndRect.Width(),m_WndRect.Height(),
		&srcDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	srcDC.SelectObject(pOldBmp);
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CPtuDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//// TODO:  在此更改 DC 的任何属性
	//if (nCtlColor == CTLCOLOR_STATIC)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	//}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CPtuDlg::ShowOneDlg(int nId)
{
	m_pPresetSettingDlg->ShowWindow(SW_HIDE);
	m_pPtuManageDlg->ShowWindow(SW_HIDE);
	m_pDeviceLedgerDlg->ShowWindow(SW_HIDE);
	m_pLinkageConfigDlg->ShowWindow(SW_HIDE);
	m_pDeviceTypeConfigDlg->ShowWindow(SW_HIDE);

	GetDlgItem(nId)->ShowWindow(SW_SHOW);
}

void CPtuDlg::OnBnClickedButtonDeviceLedgerManage()
{
	// TODO: 在此添加控件通知处理程序代码
	//ShowOneDlg(IDD_DIALOG_DEVICE_LEDGER);
	//GetDlgItem(IDD_DIALOG_DEVICE_LEDGER)->ShowWindow(SW_SHOW);
	m_pPresetSettingDlg->ShowWindow(SW_HIDE);
	m_pPtuManageDlg->ShowWindow(SW_HIDE);
	m_pDeviceLedgerDlg->ShowWindow(TRUE);
	m_pLinkageConfigDlg->ShowWindow(SW_HIDE);
	m_pDeviceTypeConfigDlg->ShowWindow(SW_HIDE);
}

void CPtuDlg::OnBnClickedButtonLinkageConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pPresetSettingDlg->ShowWindow(SW_HIDE);
	m_pPtuManageDlg->ShowWindow(SW_HIDE);
	m_pDeviceLedgerDlg->ShowWindow(SW_HIDE);
	m_pLinkageConfigDlg->ShowWindow(TRUE);
	m_pDeviceTypeConfigDlg->ShowWindow(SW_HIDE);
	//ShowOneDlg(IDD_DIALOG_LINKAGE_CONFIG);
}

void CPtuDlg::OnBnClickedButtonDeviceTypeConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pPresetSettingDlg->ShowWindow(SW_HIDE);
	m_pPtuManageDlg->ShowWindow(SW_HIDE);
	m_pDeviceLedgerDlg->ShowWindow(SW_HIDE);
	m_pLinkageConfigDlg->ShowWindow(SW_HIDE);
	m_pDeviceTypeConfigDlg->ShowWindow(SW_SHOW);
	//ShowOneDlg(IDD_DIALOG_DEVICE_TYPE_CONFIG);
}

void CPtuDlg::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strText;
	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(strText);
	HTREEITEM item = FindItem(&m_treeList, m_treeList.GetRootItem(), strText);
	if (item != NULL)
	{
		m_treeList.SelectItem(item);
	}
}
