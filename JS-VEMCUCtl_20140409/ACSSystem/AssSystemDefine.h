/*
辅助系统相关的全局定义
*/
#ifndef _ASSSYSTEM_DEFINE_H_
#define _ASSSYSTEM_DEFINE_H_

////////////////////////////辅助系统相关//////////////////////////////////////////////
#define ACS_LOGIN_SERVER_REQUEST                 1001   //登录请求
#define ACS_LOGIN_SERVER_RESPONSE                1002   //登录响应

#define ACS_BEATHEART_REQUEST 1003    //心跳发送
#define ACS_BEATHEART_RESPONSE  1004	//心跳接收

#define ACS_PUSH_DATA_RESPONSE 1005	//数据推送

#define ACS_SUBSCRIBE_STATION_REQUEST 1006	//变电站订阅
#define ACS_SUBSCRIBE_STATION_RESPONSE 1007

#define ACS_Hide_DEV_REQUEST 1008    //设备屏蔽
#define ACS_Hide_DEV_RESPONSE 1009

#define ACS_DEVICE_REALDATA_COUNT 1024	//一次最多接收1024个设备实时数据

#define MAX_DEV_RELATION_CAMERA 16  //每个设备最大关联的视频个数


#define ACS_RECVBUFSIZE							1024*1024
#define ACS_SOCKETRECVBUFSIZE					1024*1024*16
#define ACS_SOCKETSENDBUFSIZE					1024*1024*16

///////////////////////////////////////右键菜单/////////////////////////////////////////
#define ID_LINKAGE_INFO1 10001
#define ID_LINKAGE_INFO2 10002
#define ID_LINKAGE_INFO3 10003
#define ID_LINKAGE_INFO4 10004
#define ID_LINKAGE_INFO5 10005
#define ID_LINKAGE_INFO6 10006
#define ID_LINKAGE_INFO7 10007
#define ID_LINKAGE_INFO8 10008
#define ID_LINKAGE_INFO9 10009
#define ID_LINKAGE_INFO10 10010
#define ID_LINKAGE_INFO11 10011
#define ID_LINKAGE_INFO12 10012
#define ID_LINKAGE_INFO13 10013
#define ID_LINKAGE_INFO14 10014
#define ID_LINKAGE_INFO15 10015
#define ID_LINKAGE_INFO16 10016
////////////////////////////////////////////////////////////////////////////////////////

//用户操作日志记录结构体
typedef struct _ass_user_operate_log
{
	int id;
	int user_id;
	char szUserName[32];
	char ip_address[32];
	char operate_time[64];
	char operate_action[128];
}ASS_USER_OPERATE_LOG;

//登录返回结构体
typedef struct _login_response
{
	int nResult;
}LOGIN_REPONSE;

//变电站消息订阅返回结构体
typedef struct _subscribe_station_response
{
	int nResult;
}SUBSCRIBE_STATION_REPONSE;

//变电站实时消息结构
typedef struct _push_data_response
{
	int nRvuId;
	int nSmId;
	int nNodeId;
	float fValue;
	int nType;
	int nStatus;
}PUSH_DATA_RESPONSE;

//实时数据结构体
typedef struct _t_sm_realdata_
{
	int station_id;
	int dev_id;//设备的dev_id
	int rvu_id;//rvu_id,sm_id,node_id,这三个一起可以确定一个设备
	int sm_id;
	int node_id;
	float real_value;//实时数据值
	int type;//类型,YX,YC,YK等
	int status;//0正常,1严重告警,2主要告警,3一般告警,4无效数据
	int relation_video;//0未关联视频，1关联视频
	char dev_name[32];//设备名称
}T_SM_REALDATA;

typedef struct _t_linkage_camera_info
{
	int nDevId;
	char szCameraCode[32];
	int nPresetId;
	char szPresetName[64];
}T_LINKAGE_CAMERA_INFO;

#endif