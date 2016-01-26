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

#define ACS_SUBSCRIBE_ALARM_REQUEST 1010 //告警订阅
#define ACS_SUBSCRIBE_ALARM_RESPONSE 1011 //告警订阅

#define ACS_PUSH_ALARM_RESPONSE 1012 //告警推送

#define ACS_LINKAGE_REALTIME_INFO_RESPONSE 1013 //联动实时消息

#define ACS_DEVICE_REALDATA_COUNT 1024	//一次最多接收1024个设备实时数据
#define ACS_PUSH_ALARM_COUNT 1024 //一次最多接收1024个告警推送数据
#define ACS_LINKAGE_REALTIME_INFO_COUNT 1024 //一次最多接收1024个联动实时数据


#define ACS_RECVBUFSIZE							1024*1024
#define ACS_SOCKETRECVBUFSIZE					1024*1024*16
#define ACS_SOCKETSENDBUFSIZE					1024*1024*16

#define MAX_DEV_RELATION_CAMERA 16  //每个设备最大关联的视频个数

#define PRESETNUMMIN (5)
#define PRESETNUMMAX (10)

///////////////////////////////////////右键菜单/////////////////////////////////////////
#define ID_LINKAGE_INFO1 (10011)
#define ID_LINKAGE_INFO2 (ID_LINKAGE_INFO1+1)
#define ID_LINKAGE_INFO3 (ID_LINKAGE_INFO1+2)
#define ID_LINKAGE_INFO4 (ID_LINKAGE_INFO1+3)
#define ID_LINKAGE_INFO5 (ID_LINKAGE_INFO1+4)
#define ID_LINKAGE_INFO6 (ID_LINKAGE_INFO1+5)
#define ID_LINKAGE_INFO7 (ID_LINKAGE_INFO1+6)
#define ID_LINKAGE_INFO8 (ID_LINKAGE_INFO1+7)
#define ID_LINKAGE_INFO9 (ID_LINKAGE_INFO1+8)
#define ID_LINKAGE_INFO10 (ID_LINKAGE_INFO1+9)
#define ID_LINKAGE_INFO11 (ID_LINKAGE_INFO1+10)
#define ID_LINKAGE_INFO12 (ID_LINKAGE_INFO1+11)
#define ID_LINKAGE_INFO13 (ID_LINKAGE_INFO1+12)
#define ID_LINKAGE_INFO14 (ID_LINKAGE_INFO1+13)
#define ID_LINKAGE_INFO15 (ID_LINKAGE_INFO1+14)
#define ID_LINKAGE_INFO16 (ID_LINKAGE_INFO1+15)

#define ID_HIDE_ALARM 10040	//屏蔽告警
#define ID_COMPLETE_PROCESS_ALARM 10050	//完成处理
#define ID_RECOVER_ALARM 10060	//恢复告警
#define ID_CANCEL_HIDE_ALARM 10070	//取消屏蔽
#define ID_DELETE_LINKAGE_RECORD 10071 //删除该条记录
#define ID_REMOVE_ALL_ITEMS 10072 //清空列表
#define ID_SHOW_ELECMAP 10073	//显示电子地图
#define ID_PROCESS_ALARM 10074	//处理告警
#define ID_OPEN_VIDEO 10075	//打开视频
#define ID_DLG_CAMERA_AND_CONTROL_CLOSE_VIDEO 10076 //关闭视频

#define ID_DLG_ALARM_PREVIEW_VIEW_DETAIL 10077//查看详细
#define ID_DLG_ALARM_PREVIEW_PROCESS_ALL 10078//全部处理
#define ID_DLG_STATION_ALARM_DELETE_SELECTED_ITEMS 10079 //删除选中行
///////////////////////////////////////////////////////////////////////////////////////////
#define HIDE_DEV 1	//设备屏蔽
#define CANCEL_HIDE_DEV 0	//取消设备屏蔽

#define PROCESS_ALARM 0 //已处理的告警
#define UN_PROCESS_ALARM 1//未处理的告警
#define COMPLETE_PROCESS_ALARM 2 //完成处理的告警

//////////////////////////////////////////////////////////////////////////////////////////
#define LINKAGE_CAMERA 2  //2表示联动设备类型为摄像头
#define LINKAGE_DOOR 1 //1表示联动设备为门禁

#define LINKAGE_ACTION_OPEN 1 //打开
#define LINKAGE_ACTION_CLOSE 2 //关闭
#define LINKAGE_ACTION_PRESET 3 //转预置位

#define LINKAGE_SHOW_TYPE_MULTIVIDEO 0 //多画面展示
#define LINKAGE_SHOW_TYPE_ELECMAP 1	//电子地图展示

/////////////////////////////////////////////////////////////////////////
#define BTNST_COLOR_BK_IN_COLOR RGB(158,195,225)
#define BTNST_COLOR_BK_OUT_COLOR RGB(195,220,242)
#define BTNST_COLOR_BK_FOCUS_COLOR RGB(158,195,225)

//dvr信息
typedef struct _ass_dvr_info_
{
	int nDvrId;
	char szDvrCode[32];
	char szDvrIp[32];
	int nDvrPort;
	char szDvrUserName[32];
	char szDvrPassword[32];
	char szDvrType[32];
	int nDvrType;
}ASS_DVR_INFO;

//摄像头信息
typedef struct _ass_camera_info_
{
	char szCameraCode[32];
	char szCameraName[32];
	int nChannel;
	ASS_DVR_INFO tDvrInfo;
}ASS_CAMERA_INFO;

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

//通用的返回响应，只有成功，失败和失败错误码
typedef struct _common_response
{
	int nResult;
	int nCode;
}COMMON_RESPONSE;

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

//告警统计结构体
typedef struct _alarm_statistics_
{
	int nStationId;//变电站id
	char szStationName[64];//变电站名称 
	int nAlarmCount;//告警次数
}ALARM_STATISTICS;

//告警推送实时消息结构
typedef struct _push_alarm_response
{
	int nDevId;
	int nStationId;
	int nAlarmType;
	char szAlarmStartTime[64];
	int nPriorLevel;
}PUSH_ALARM_RESPONSE;

typedef struct _linkage_realtime_info_response_
{
	int nDevID;
	char szCamreaCode[32];
	int nPresetNum;
	char szStartTime[32];
}LINKAGE_REALTIME_INFO_RESPONSE;

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
	char szCameraCode[64];
	int nPresetId;
	char szPresetName[64];
}T_LINKAGE_CAMERA_INFO;

typedef struct _t_ass_linkage_realtime_info_
{
	int nDevId;
	int nRelateDevId;
	char szCamreaCode[32];
	int nRelateType;
	int nRelateAction;
	int nPresetNum;
	char szStartTime[32];
}T_ASS_LINKAGE_REALTIME_INFO;

//联动配置结构体
typedef struct _t_ass_linkage_config_
{
	int nUserId;//用户id，每个用户都有一套配置
	int nShowType;//0:多画面方式展示，1：电子地图方式展示
	int nShowVideo;//0:不展示视频，1:展示视频
	int nChangeTime;//事件切换时间
	int nSecurity;//0：不联动安防告警,1:联动安防告警
	int nFire;//0：不联动消防告警,1:联动消防告警
	int nDoorAlarm;//0：不联动门禁报警,1:联动门禁报警
	int nDoorAction;//0：不联动门禁开关门,1:联动门禁开关门
}T_ASS_LINKAGE_CONFIG;

typedef struct _t_ass_subscribe_info_
{
	int nId;
	int nUserId;
	int nStatiionId;
}T_ASS_SUBSCRIBE_INFO;

typedef struct _acs_dev_info
{
	int nDevId;
	char szDevName[64];
	int nRelationVideo;
	int nRvuId;
	int nSmId;
}ACS_DEV_INFO;

typedef struct _acs_station_info
{
	int nStationId;
	char szStationName[64];
	int nVolClass;
}ACS_STATION_INFO;

typedef struct _acs_alarm_info_
{
	int nId;
	int nDevId;
	int nStationId;
	char szVolGrade[32];//电压等级
	char szStationName[32];//变电站名称
	char szDevName[32];//告警设备名称
	int nAlarmType;//告警类型
	char szAlarmTypeName[32];//告警类型名称 
	char szAlarmStartTime[32];//告警发生时间
	char szAlarmProcessTime[32];//告警处理时间
	char szAlarmProcessUser[32];//告警处理人员
	char szAlarmHideTime[32];//告警屏蔽时间
	int nRelationVideo;//是否关联视频 1表示关联视频，0表示未关联视频
	int nPriorLevel;//优先级
	int nStatus;//处理状态 0表示已处理，1表示未处理,2表示处理完成
}ACS_ALARM_INFO;

typedef struct _ass_linkage_list_item_
{
	int nDevId;
	char szDevName[32];
	int nRelateDevId;
	char szRelateDevName[32];
	char szCamreaCode[32];
	char szCameraName[32];
	int nRelateType;
	int nRelateAction;
	int nPresetNum;
	char szStartTime[32];
}ASS_LINKAGE_LIST_ITEM;































#endif