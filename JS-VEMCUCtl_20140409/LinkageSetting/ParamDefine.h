#ifndef _PARAM_DEFINE_H_
#define _PARAM_DEFINE_H_

#define MAX_RECV_BUFF_SIZE   64*1024
#define MAX_SEND_BUFF_SIZE   64*1024

#define SERVERSOCKET_MAXNUM					100
#define CLIENTSOCKET_MAXNUM					1000

#define RECVBUFSIZE							1024*1024
#define SOCKETRECVBUFSIZE					1024*1024*16
#define SOCKETSENDBUFSIZE					1024*1024*16

#define SOCKETSENDBUFMAXSIZE				1453

#define STOPALL				0	//停止当前动作
#define	TILTUP				1	//向上
#define TILTDOWN			2   //向下
#define PANLEFT				3	//左转
#define PANRIGHT			4	//右转
#define CLOSEIRIS			5	//光圈关
#define OPENIRIS			6	//光圈开
#define FOCUSNEAR			7	//近聚焦
#define FOCUSFAR			8   //远聚焦
#define ZOOMIN				9	//缩小
#define ZOOMOUT				10	//放大
#define UPLEFT				17	//左上方向运动
#define DOWNLEFT			18	//左下方向运动
#define UPRIGHT				19	//右上方向运动
#define DOWNRIGHT			20	//右下方向运动

#define YT_CLOSEIRISSTOP		0x0101	//光圈关停止
#define YT_CLOSEIRIS			0x0102	//光圈关
#define YT_OPENIRIS			0x0103	//光圈开
#define YT_OPENIRISSTOP		0x0104	//光圈开停止
#define YT_FOCUSNEARSTOP		0x0201	//近聚焦停止
#define YT_FOCUSNEAR			0x0202	//近聚焦
#define YT_FOCUSFARSTOP		0x0203	//远聚焦停止
#define YT_FOCUSFAR			0x0204  //远聚焦
#define YT_ZOOMINSTOP			0x0301  //缩小停止
#define YT_ZOOMIN				0x0302	//缩小
#define YT_ZOOMOUTSTOP			0x0303	//放大停止
#define YT_ZOOMOUT				0x0304	//放大
#define YT_TILTUPSTOP			0x0401	//向上停止
#define	YT_TILTUP				0x0402	//向上
#define YT_TILTDOWNSTOP		0x0403	//向下停止
#define YT_TILTDOWN			0x0404  //向下
#define YT_PANRIGHTSTOP		0x0501	//右转停止
#define YT_PANRIGHT			0x0502	//右转
#define YT_PANLEFTSTOP			0x0503	//左转停止
#define YT_PANLEFT				0x0504	//左转
#define YT_SETPRESET			0x0601	//预置位保存
#define YT_GOTOPRESET			0x0602	//预置位调用
#define YT_CLEPRESET			0x0603	//预置位删除
#define YT_UPLEFTSTOP			0x0701	//左上方向运动停止
#define YT_UPLEFT				0x0702	//左上方向运动
#define YT_DOWNLEFTSTOP		0x0703	//左下方向运动停止
#define YT_DOWNLEFT			0x0704	//左下方向运动
#define YT_UPRIGHTSTOP			0x0801	//右上方向运动停止
#define YT_UPRIGHT				0x0802	//右上方向运动
#define YT_DOWNRIGHTSTOP		0x0803	//右下方向运动停止
#define YT_DOWNRIGHT			0x0804	//右下方向运动

#define NAMELENGTH			40	//名字命名长度	40	40字节长
#define PASSWORDLEN			20	//口令长度	20	20字节长
#define DES_LENGTH			160 //描述信息长度	160	160字节长
#define UNIT_LENGTH			8   //数据单位的长度	8	8字节长
#define VER_LENGTH			40  //版本描述的长度	40	40字节长
#define PHONE_LENGTH		20  //电话号码长度	20	20字节长
#define DATA_LENGTH			60  //数据长度	60	60字节长
#define SDESC_LENGTH		80  //短描述长度	80	80字节长
#define CARDLENGTH			20  //卡号长度	20	20字节长

#define MAIN_TITLE_HEIGHT           140
#define MAIN_BUTTON_WIDTH           131
#define MAIN_BUTTON_HIGHT           27

#define REQUEST_RVU_LOGIN                 101
#define RESPONSE_RVU_LOGIN                102
#define REQUEST_RVU_GET_SM                201
#define RESPONSE_RVU_SET_SM               202
#define REQUEST_RVU_GET_NODES             203
#define RESPONSE_RVU_SET_NODES            204
#define REQUEST_RVU_SET_DYN_ACCESS_MODE   301
#define RESPONSE_RVU_DYN_ACCESS_MODE_ACK  302

#define SEND_REALTIME_ALARM               401
#define HEART_BEAT                      601
#define HEART_BEAT_ACK                  602

#define REQUEST_RVU_SET_POINT            501
#define RESPONSE_RVU_SET_POINT_ACK       502

#define MAX_FRAME_LEN   (1452+8)
#define USER_DATA_OFFSET  30
#define DATA_FRAME_HEADER  8
#define ETHERNET_FRAME_HEADER 14
#define SET_SM_ACK_DATA_OFFSET  30

#define MAX_SM_NUM_PER_STATION  128
#define MAX_NODE_NUM_PER_STATION 256

#define MAX_STATION_NUM_PER_USER  1024
#define MAX_DVR_NUM_PER_USER      (MAX_STATION_NUM_PER_USER*16)
#define MAX_CAMERA_NUM_PER_USER   (MAX_DVR_NUM_PER_USER*16)

#define MAX_RVU_NUM_PER_STATION  16
#define MAX_RVU_DEVICE_NUM_PER_STATION 1024

#define DATABASE_OPERATE_INSERT   1
#define DATABASE_OPERATE_UPDATE   2
#define DATABASE_OPERATE_DELETE   3

#define CAMERA_SUB_SYSTEM	1
#define DOOR_SUB_SYSTEM	2
#define ENVIRONMENT_SUB_SYSTEM	3
#define SECURITY_SUB_SYSTEM	4
#define LIGHT_SUB_SYSTEM	5
#define FIRE_SUB_SYSTEM	6
#define OTHER_SUB_SYSTEM 7

////////////////////////////////////各窗口之间消息传递////////////////////////////////
#define WM_ADD_DEVICE_LEDGER_COMPLETED (WM_USER+10000)

typedef struct _recv_packet_
{
	char recvbuf[RECVBUFSIZE];
	int size;
	int handle;
}RECV_PACKET;

typedef struct _t_ttime_
{
	short years;
	char month;
	char day;
	char hour;
	char minute;
	char second;
	char milsecond;
}TTime;

typedef struct _t_tid_
{
	short sm_id;
	short node_id;
}TID;

typedef struct _t_tsm_
{
	long size;
	long type;
	TID id;
	long member_id;
	long port_id;
	short port_mode;
	short baudrate;
	long address;
	float longitude;
	float latitude;
	char sm_name[NAMELENGTH];
	char desc[SDESC_LENGTH];
	long device_type;
	long sub_device_type;
	char productor[NAMELENGTH];
	char vision[VER_LENGTH];
	long camera_channel;
	long rvu_id;
	long reserve1;
	char reserve2[NAMELENGTH];
}TSM;

typedef struct _t_talarm_
{
	long type;//数据点类型
	TID id;//数据点ID
	TTime starttime;//告警发生时间
	TTime endtime;//告警结束时间
	TTime confirmtime;//告警确认时间
	TTime canceltime;//告警取消时间
	long alarm_level;//告警级别。0-无告警判断，1-严重告警，2-主要告警，3-一般告警
	float alarm_value;//告警数值。
	long  alarm_status;//上传告警的类型。0-开始，1-结束，2-确认，3-取消
	char description[DATA_LENGTH];//告警值描述
}TALARM;

typedef struct _t_tnode_
{
	long size;//本数据结构的长度
	long type;//数据的类型
	TID id;//数据标识ID
	TID virtual_id;//保留
	char node_name[NAMELENGTH];
	char desc[SDESC_LENGTH];
	float max_val;//有效上限
	float min_val;//有效下限
	long alarm_enable;//告警使能标记
	float HiLimit1;//一级告警上限
	float LoLimit1;//一级告警下限
	float HiLimit2;//二级告警上限
	float LoLimit2;//二级告警下限
	float HiLimit3;//三级告警上限
	float LoLimit3;//三级告警下限
	float percision;//精度
	long lSaved;//是否保存
	char unit[UNIT_LENGTH];//单位
	long phy_id;//对应的物理节点
	long reserver1;//保留1
	long reserver2;//保留2
	long reserver3;//保留3
	long reserver4;//保留4
}TNODE;

//typedef struct _recv_packet_ 
//{
//	char recvbuf[MAX_RECV_BUFF_SIZE];
//	int size;
//}RECV_PACKET;

typedef struct DBConfigParam 
{
	char DBServerIP[16];
	int  DBServerPort;
	char DBName[32];
	char DBUserName[32];
	char DBUserPassword[32];
}DBConfigParam;

typedef struct RVUConfig
{
	char rvu_ip[24];
	int  rvu_port;
	char rvu_username[32];
	char rvu_password[32];
}RVUConfig;

typedef  struct StationNode
{
	int station_id;
	char station_code[32];
	char station_name[64];
	int node_type;
	int connect_ptu;//0-表示未连接，1表示连接
}StationNode;

typedef struct nodeinfo
{
	int  node_id;
	int  parent_id;
	int  station_id;
	int  dvr_id;
	int  channel_id;
	char node_name[64];
	char node_code[32];
	int node_type;
	int node_level;
}NodeInfo;

typedef struct ACCESS_METHOD_IP 
{
	char ip_addr[32];
	int  ip_protocol;
	int ip_port;
}ACCESS_METHOD_IP;

typedef struct ACCESS_METHOD_SERIAL 
{
	char local_addr[32];
	int local_port;
	char device_addr[32];
	int device_port;
	int serial_bandrate;
	int serial_data;
	int serial_stopdata;
	int serial_parity;
}ACCESS_METHOD_SERIAL;

typedef struct ACCESS_DEVICE_INFO
{
	int id;
	char dev_code_sys[32];
	int node_code_id;
	char dev_name[64];
	int connect_type;
	ACCESS_METHOD_IP access_ip;
	ACCESS_METHOD_SERIAL access_serial;
	char login_name[64];
	char login_password[32];
	int dev_type;
	int factory_type;
	int yx_num;
	int yc_num;
	int yk_num;
	int yt_num;
	int camera_num;
}ACCESS_DEVICE_INFO;

typedef struct _vem_user_config_info_
{
	char szUserName[256];
	char szUserPassword[64];
	char szLocalUdpIp[32];
	int    nLocalUdpPort;
	int    nMinRtpPort;
	int    nMaxRtpPort;
	char szSipServerIp[32];
	int    nSipServerPort;
	char szHttpServerIp[32];
	int    nHttpServerPort;
	char szDBServerIp[32];
	int    nDBServerPort;
	char szDBName[256];
	char szLinkServerIp[32];
	int    nLlinkServerPort;
	int    nPreSetSaveEnable;
	int nAcsTcpHandle;
	char szAcsServerIp[32];
	int nAcsServerPort;
	int nUserId;
}VEM_USER_CONFIG_INFO;

typedef struct CAMERA_INFO
{
	char ip_addr[32];
	int ip_port;
	char login_name[64];
	char login_password[32];
	char szDvrType[32];
	int dvr_type;
	int dvr_id;
	int channel;
}CAMERA_INFO;
//子系统枚举
enum SUB_SYSTEM_TYPE
{
	ALL_SUB_SYSTEM         = 1000,//全部子系统
	DOOR_ACCESS            = 1001,//门禁子系统
	VIDEO_MONITOR          = 1002,//视频监控子系统
	ENVIRONMENT_MONITOR    = 1003,//环境监控子系统
	FIRE_PROTECTING        = 1004,//消防子系统
	SECURITY_PROTECTING    = 1005,//安防子系统
	LIGHT_CONTROL          = 1006,//灯光控制子系统
	POWER_MONITOR          = 1007,//电源监控子系统
	SCADA_SYSTEM           = 1008,//SCADA系统
	FIVE_PROTECTING        = 1009,//五防系统
	ROUTING_CHECK          = 1010//巡检系统
}SUB_SYSTEM_TYPE;

//子系统设备枚举类型
enum SUB_SYSTEM_DEVICE_TYPE
{
	DOOR_ACCESS_DEVICE            = 1,//门禁设备
	VIDEO_MONITOR_DEVICE          = 2,//视频监控子系统设备
	ENVIRONMENT_MONITOR_DEVICE    = 3,//环境监控子系统
	FIRE_PROTECTING_DEVICE        = 4,//消防子系统
	SECURITY_PROTECTING_DEVICE    = 5,//安防子系统
	LIGHT_CONTROL_DEVICE          = 6,//灯光控制子系统
	POWER_MONITOR_DEVICE          = 7,//电源监控子系统
	SCADA_SYSTEM_DEVICE           = 8,//SCADA系统
	FIVE_PROTECTING_DEVICE        = 9,//五防系统
	ROUTING_CHECK_DEVICE          = 10//巡检系统
}SUB_SYSTEM_DEVICE_TYPE;

enum ACCESS_DEVICE_FACTORY
{
	HK_DVR_LESS_8000  =   2000,//海康DVR/DVS 8000以下
	HK_DVR_MORE_8000,//海康DVR/DVS 8000以上
	DH_BOARD,//大华板卡
	DH_DVR,//大华DVR
	HY_BOARD,//恒亿板卡
	HY_DVR,//恒亿DVR/DVS
	HB_BOARD,//汉邦板卡
	HB_DVR,//汉邦DVR
	LSZX_BOARD,//蓝色之星板卡
	LSZX_DVR,//蓝色之星DVR
	DLink_BOARD,//DLink板卡
	DLink_DVR,//DLink DVR
	DL_BOARD,//大力板卡
	DL_DVR,//大力DVR
	ZY_DVR,//广东中钰DVR
	AXIS_BOARD,//安讯视板卡
	AXIS_DVR,//安讯视DVR
	MOBOTIX_DVR,//MOBO DVR
	NAZI_BOARD,//南自信息板卡
	NAZI_DVR,//南自信息DVR
	PANASONIC_IP_CAMERA,//
	Emerson_DVR,//艾默生DVR
	CL_DVR,//潮流DVR

	NARI_DOOR_SYSTEM = 2100,//南瑞门禁系统，用于直接对接系统
	NARI_DOOR_CONTROL_1,//南瑞XX1型号门禁控制器
	NARI_DOOR_CONTROL_2,//南瑞XX2型号门禁控制器
	NAZI_DOOR_SYSTEM,//南自门禁系统，用于直接对接系统
	NAZI_DOOR_CONTROL_1,//南自XX1型号门禁控制器
	NAZI_DOOR_CONTROL_2,//南自XX2型号门禁控制器
	NBE_DOOR_SYSTEM,//纽贝尔门禁系统，用于直接对接系统
	NBE_DOOR_CONTROL_1,//纽贝尔XX1型号门禁控制器
	NBE_DOOR_CONTROL_2,//纽贝尔XX2型号门禁控制器
	QF_DOOR_SYSTEM,//奇峰门禁系统，用于直接对接系统
	QF_DOOR_CONTROL_1,//奇峰XX1型号门禁控制器
	QF_DOOR_CONTROL_2,//奇峰XX2型号门禁控制器

	HK_MASTER = 2200,//海康动环主机
	NARI_MASTER,//南瑞环境主机
	XX_ELECTRIC_FENCE,//XX厂家电子围栏
	XX_FIRE_CONTROL_MASTER,//消防报警主机
	XX_AIR_CONDITION,//空调主机
	XX_SECURITY_CONTROL_MASTER,//安防报警主机

	XX_LIGHT_CONTROL_MASTER = 2700,//灯光控制主机

	ACCESS_DEVICE_FLAG = 2899
}ACCESS_DEVICE_FACTORY;

//////////////////////////////////////////////////////////////////////////

#define DEVICE_OFF_LINE_TEXT "设备掉线"
#define VIDEO_MOTION_DETECT_ALARM_TEXT "移动侦测告警"
#define VIDEO_SHELTER_ALARM_TEXT "视频遮挡告警"
#define VIDEO_BORDER_DETECT_ALARM_TEXT "视频边界检测告警"
#define VIDEO_OPEN_TEXT "用户打开视频"

#define DVR_HIGHER_ALARM_TEXT "DVR设备高温"
#define DVR_LOWER_ALARM_TEXT "DVR设备低温"
#define DVR_FUN_ALARM_TEXT "DVR风扇故障"
#define DVR_HARDDISK_ALARM_TEXT "DVR磁盘故障"

#define AD_UP_ALARM_TEXT "模拟量上限告警"
#define AD_UP_UP_ALARM_TEXT "模拟量上上限告警"
#define AD_LOW_ALARM_TEXT "模拟量下限告警"
#define AD_LOW_LOW_ALARM_TEXT "模拟量下下限告警"

#define STATUES_ALARM_TEXT "状态量告警"

#define DOOR_CONTROL_LOW_POWER_TEXT "电量低"
#define DOOR_CONTROL_ILLEGAL_TEXT "非法刷卡"
#define DOOR_CONTROL_OFFLINE_ALARM_TEXT "脱机报警"
#define DOOR_CONTROL_STRESSED_TEXT "被胁迫"
#define DOOR_CONTROL_NO_LOCK_TEXT "未锁好"
#define DOOR_CONTROL_FORCE_BREAKIN_TEXT "强行闯入"
#define YX_DATA_IS_1_TEXT  "遥信数据正常变为1"
#define YX_DATA_IS_0_TEXT "遥信数据正常变为0"
#define YC_DATA_CHANGE_NORMAL_TEXT "遥测数据正常变化"
#define YK_EVENT_OPERATE_NORMAL_TEXT "遥控事件正常发生"

#define DOOR_OPEN_IN_CARD_TEXT "刷卡开门"
#define DOOR_OPEN_IN_PASSWORD_TEXT "密码开门"
#define DOOR_OPEN_IN_REMOTE_CONTROL_TEXT "远程开门"
#define DOOR_OPEN_IN_FORCE_TEXT "强行开门"
//////////////////////////////////////////////////////////////////////////
enum DEVICE_ALARM_INFO
{
	DEVICE_OFF_LINE =2900,//设备掉线
	VIDEO_MOTION_DETECT_ALARM,//移动侦测告警
	VIDEO_SHELTER_ALARM,//视频遮挡告警
	VIDEO_BORDER_DETECT_ALARM,//视频边界检测告警
	VIDEO_OPEN,//用户打开视频

	DVR_HIGHER_ALARM = 2910,//DVR设备高温
	DVR_LOWER_ALARM,//DVR设备低温
	DVR_FUN_ALARM,//DVR风扇故障
	DVR_HARDDISK_ALARM,//DVR磁盘故障

	AD_UP_ALARM = 2920,//模拟量上限告警
	AD_UP_UP_ALARM,//模拟量上上限告警
	AD_LOW_ALARM,//模拟量下限告警
	AD_LOW_LOW_ALARM,//模拟量下下限告警

	STATUES_ALARM = 2930,//状态量告警

	DOOR_CONTROL_LOW_POWER = 2940,//电量低
	DOOR_CONTROL_ILLEGAL,//非法刷卡
	DOOR_CONTROL_OFFLINE_ALARM,//脱机报警
	DOOR_CONTROL_STRESSED,//被胁迫
	DOOR_CONTROL_NO_LOCK,//未锁好
	DOOR_CONTROL_FORCE_BREAKIN//强行闯入
}DEVICE_ALARM_INFO;//设备告警类型

enum DEVICE_EVENT_INFO
{
	YX_DATA_IS_1 = 3000,//遥信数据正常变为1
	YX_DATA_IS_0,//遥信数据正常变为0
	YC_DATA_CHANGE_NORMAL,//遥测数据正常变化
	YK_EVENT_OPERATE_NORMAL,//遥控事件正常发生

	DOOR_OPEN_IN_CARD = 3031,//刷卡开门
	DOOR_OPEN_IN_PASSWORD,//密码开门
	DOOR_OPEN_IN_REMOTE_CONTROL,//远程开门
	DOOR_OPEN_IN_FORCE//强行开门
}DEVICE_EVENT_INFO;//设备事件类型

enum CU_LINKAGE_EVENT_TYPE
{
	CU_OPEN_VIDEO = 3101,//打开联动动作设备视频
	CU_VOICE_TALK,//语音对讲
	CU_LOCAL_RECORD,//本地录像
	CU_CAPITURE_PIC,//抓图
	CU_GOTO_PRESET,//转至预置位
	CU_CRUISE,//巡航
	CU_TRAIL,//轨迹
	CU_AUTO_TRACK,//自动跟踪
	CU_YK_ADJUST,//遥控量控制
	CU_AD_ADJUST,//模拟量调节
	CU_SOUND_ALARM,//声音报警
	CU_LIGHT_ALARM,//灯光报警
	CU_LINKAGE_DATA_DISPLAY,//联动动作设备数据展示
	CU_FLASH_2D_EMAP,//联动事件动作在二维电子地图中闪烁
	CU_FLASH_3D_EMAP//联动事件动作在三维电子地图中闪烁
}CU_LINKAGE_EVENT_TYPE;//客户端联动动作事件类型

enum PTS_LINKAGE_EVENT_TYPE
{
	PTS_LOCAL_RECORD=3201,//本地录像
	PTS_CAPITURE_PIC,//抓图
	PTS_GOTO_PRESET,//转至预置位
	PTS_CRUISE,//巡航
	PTS_TRAIL,//轨迹
	PTS_AUTO_TRACK,//自动跟踪
	PTS_YK_ADJUST_ON,//遥控量控制开
	PTS_YK_ADJUST_OFF,//模拟量控制关
	PTS_AD_ADJUST,//模拟量调节增一档
}PTS_LINKAGE_EVENT_TYPE;//后台联动动作事件类型

typedef struct _t_ass_login_user_info_
{
	char szUserName[64];
	BYTE usertype;
	BYTE enablectrl_p;
	BYTE realtalk_p;
	BYTE eventlog_p;
	BYTE replay_p;
	BYTE alarm_p;
	BYTE tunecycle_p;
	BYTE usermanage_p;
	BYTE devicemanage_p;
	BYTE videorecord_p;
	BYTE elecmap_p;
	BYTE result;
}_T_ASS_LOGIN_USER_INFO_;

#define ACCESS_DEVICE_NODE         100
#define YX_DEVICE_NODE             200
#define YC_DEVICE_NODE             201
#define YK_DEVICE_NODE             202
#define YT_DEVICE_NODE             203


#define DVR_DEVICE_NODE            210
#define CAMERA_DEVICE_NODE         211
#define CAMERA_PRESET_NODE         212


#define DOOR_CARD_NODE_PARENT      300
#define DOOR_CARD_NODE             301
#define DOOR_CONTROL_DEVICE_NODE   302


#define COMBO_GAIN_VALUE  1001

#define DEPARTMENT_NODE             10
#define USER_NODE                   20
#define POWER_NODE                  30

#define ENABLE_CTRL                 31
#define REALTALK_P                  32
#define EVENTLOG_P                  33
#define REPALY_P                    34
#define ALARM_P                     35
#define TUNECYCLE_P                 36
#define USERMANAGER_P               37
#define DEVICEMANAGER_P             38
#define TIMERECORD_P                39
#define ELECMAP_P                   40

#define USE_SEPRATE_DVR_SDK  1


#define MAX_VIDEO_NUM      9


#define MULTIVIEW_STATION_CAMERA_LIST_WIDTH  200
#define MULTIVIEW_LINKAGE_INTERVAL    15
#define MULTIVIEW_TITLE_HEIGHT        30
#define MULTIVIEW_ALARM_INFO_HEIGHT         200

//////////////////////////////////////////////////////////////////////////
#define MAX_STATION_OB_NODE_NUM    128
#define MAX_STATION_OB_ACCESS_DEV  256
#define MAX_STATION_OB_CAMERA      512
#define MAX_STATION_ST_RELATION    1024
#define MAX_STATION_OB_YK          1024
#define MAX_STATION_OB_YT          1024
#define MAX_STATION_OB_YC          1024
#define MAX_STATION_OB_YX          1024
#define MAX_STATION_OB_CARD        512
#define MAX_STATION_CAMERA_PRESET  2048
#define MAX_STATION_CT_USER        1024
//////////////////////////////////////////////////////////////////////////
//windows消息自定义
#define WM_CLOSE_VIDEO   (WM_USER + 100)
#define WM_YT_CONTROL    (WM_USER + 101)
#define WM_PRESET_ID_CHANGE  (WM_USER + 102)

#define OM_MESSAGE_MULTI_CHANNEL_CHANGE  (WM_USER + 200)
#define OM_MESSAGE_MULTI_YTCONTROL (WM_USER + 201)
#define OM_MESSAGE_MULTI_POPUP_MENU (WM_USER + 202)
#define OM_MESSAGE_SUBSYSTEM_SELECT_CHANGE (WM_USER + 203)
#define OM_MESSAGE_CAMERA_CONTROL (WM_USER + 300)
//////////////////////////////////////////////////////////////////////////
//定时器定义
#define TIMER_TUNE_CYCLE_AUTO  1
#define TIME_SPAN_TUNE_CYCLE_AUTO   10
//////////////////////////////////////////////////////////////////////////
#define VIEW_CHANNEL_WINDOW_CONTROLID  1000
//////////////////////////////////////////////////////////////////////////
#define MULTIVIEW_CHILD_WINDOW     1
#define ASS_DEVICE_MONITOR_WINDOW  2
#define ELECMAP_CHILD_WINDOW       3
#define HISTORYLOG_CHILD_WINDOW    4
#define REALPLAY_CHILD_WINDOW      5
#define PTU_CHILD_WINDOW           6
#define PLATFORM_SYN_CHILD_WINDOW  7
#define PLATFORM_ASS_LINKAGE_WINDOW 8
//////////////////////////////////////////////////////////////////////////
//XML解析消息定义
#define ASS_RESPONSE_LOGIN           1
#define ASS_RESPONSE_LOGOUT          2
#define ASS_KEEPALIVE                3
#define ASS_RESPONSE_YK_CONTROL      4
#define ASS_RESPONSE_REALTIMESTATUS   5
//////////////////////////////////////////////////////////////////////////
//节点定义
#define ROOT_NODE     0
#define STATION_NODE  1
#define SUBSYSTEM_NODE 2
#define DVR_NODE  3
#define CAMERA_NODE  4
#define RVU_NODE   5
#define RVU_DEVICE_NODE 6
#define PRESET_NODE  7
#define DEVICE_LEDGER_NODE 8

//每个节点下面挂相应的结构体
enum TREE_NODE_TYPE
{
	ENUM_ROOT_NODE = 0,		//根节点
	ENUM_STATION_NODE = 1,	//变电站节点
	ENUM_SUBSYSTEM_NODE = 2,	//子系统节点
	ENUM_DVR_NODE = 3,	//dvr节点
	ENUM_CAMERA_NODE = 4,	//摄像头节点
	ENUM_RVU_NODE = 5,	//rvu节点
	ENUM_RVU_DEVICE_NODE = 6,	//rvu设备节点
	ENUM_PRESET_NODE = 7,	//预置位节点
	ENUM_DEVICE_LEDGER_NODE = 8,	//设备台账节点
	ENUM_DEVICE_TYPE_NODE = 9	//设备类型节点
};

typedef struct _t_ass_tree_node_
{
	TREE_NODE_TYPE enumTreeNodeType;
	char *body;
}T_ASS_TREE_NODE;

typedef struct _t_ass_device_type_
{
	int nId;
	int nParentId;
	char szDeviceTypeName[32];
	int nDeviceTypeNum;
}T_ASS_DEVICE_TYPE;

typedef struct _t_ass_subsystem_
{
	int nId;
	int nParentId;
	char szSubSystemName[32];
	int nSubSystemNum;
}T_ASS_SUBSYSTEM;

//数据表定义
typedef struct _t_acs_station_ob_node_
{
	int id;
	int parent_id;
	char node_name[64];
	char node_sysid[32];
	int node_type;
}_T_ACS_STATION_OB_NODE_;

typedef struct _t_ass_station_ob_node_
{
	int id;
	int parent_id;
	char node_name[64];
}_T_ASS_STATION_OB_NODE_;

typedef struct _t_acs_station_ob_access_dev_
{
	int id;
	int parent_id;
	char dev_code_sys[32];
	char dev_name[64];
	BYTE connect_type;
	char ipaddr[24];
	BYTE ip_protocol;
	int ipport;
	char local_addr[32];//本地串口地址
	int local_port;//本地串口端口
	char serial_device_addr[32];//接入设备串口地址
	int serial_device_port;//接入设备端口
	int bandrate;//波特率
	int data;//数据位
	int stopdata;//停止位
	int parity;//校验位
	char username[64];//登陆用户名
	char userpassword[32];//登陆用户密码
	BYTE dev_type;//接入设备类型
	int factory_type;//厂家类型
	int yx_num;//遥信设备数量
	int yc_num;//遥测设备数量
	int yk_num;//遥控设备数量
	int yt_num;//遥调设备数量
	int camera_num;//camera数量
	BYTE dev_online_state;//设备状态
}_T_ACS_STATION_OB_ACCESS_DEV_;

typedef struct _t_acs_station_ob_camera
{
	int id;
	int accessdev_code_id;
	char camera_name[64];
	char camera_code[32];
	BYTE camera_channel;
	BYTE camera_online_state;
}_T_ACS_STATION_OB_CAMERA_;

typedef struct _t_acs_station_linage_relation
{
	int id;//序列号
	int event_access_dev_id;//联动事件各个子系统，对应ob_acess_dev中的id
	int event_dev_id;//对应yk、yc、yt、yx、camera中的id
	int event_type;//联动事件类型，0事件触发 1告警触发
	int event_reason;//联动事件原因
	char event_reason_text[64];//联动事件原因内容
	int action_access_dev_id;//联动动作各个子系统中的设备,对应ob_access_dev中的id
	int action_dev_id;//对应yk、yc、yt、yx、camera中的id
	int action_level;//联动动作等级
	int action_type;//联动动作事件类型
	char action_type_text[64];//联动动作事件类型文本
	int action_pretime;//录像、抓图预录时间
	int action_time;//录像、抓图动作时间
	int action_timeinterval;//抓图时间间隔
	int action_digital;//数字量，预置位编号等
	int action_yk;//开关量控制，一般为0、1
	char action_analog[32];//设备参数
}_T_ACS_STATION_LINKAGE_RELATION_;

typedef struct _t_acs_station_ob_yk
{
	int id;//序列号
	char yk_code_sys[32];//遥控设备在平台中的编码
	int access_code_id;//关联到ob_access_dev中的id
	int yk_type;
	char yk_name[64];//遥控设备名称
	char yk_addr[32];//遥控设备号、端口号
	int normal_state;//正常状态值
	int current_state;//当前状态值
	BYTE dev_online_state;//在线状态
}_T_ACS_STATION_OB_YK_;

typedef struct _t_acs_station_ob_yt
{
	int id;//序列号
	char yt_code_sys[32];//遥调设备在平台中的编码
	int access_code_id;//关联到ob_access_dev中的id
	int yt_type;
	char yt_name[64];//遥调设备名称
	char yt_addr[32];//遥调设备号、端口号
	float setting_value;//设定值
	float current_value;//当前值
	BYTE dev_online_state;//在线状态
}_T_ACS_STATION_OB_YT_;

//设备
class CRvuDevice
{
public:
	int id;
	int sub_sys_id;
	int rvu_id;
	int sm_id;
	char sm_name[64];
	int node_id;
	int device_type;
	int sub_device_type;
	int type;
};
//yc设备
class CYcRvuDevice : CRvuDevice
{

};
//yx设备
class CYxRvuDevice : CRvuDevice
{

};
//yk设备
class CYkRvuDevice : CRvuDevice
{

};
//yt设备
class CYtRvuDevice : CRvuDevice
{

};

typedef struct _t_acs_station_ob_yc
{
	int id;//序列号
	char yc_code_sys[32];//遥测设备在平台中的编码
	int access_code_id;//关联到ob_access_dev中的id
	int yc_type;
	char yc_name[64];//遥测设备名称
	char yc_addr[32];//遥测设备号、端口号
	float up_up_value;//上上限值
	float up_value;//上限值
	float current_value;//当前值
	float low_value;//下限值
	float low_low_value;//下下限值
	BYTE dev_online_state;//在线状态
}_T_ACS_STATION_OB_YC_;

typedef struct _t_acs_station_ob_yx
{
	int id;//序列号
	char yx_code_sys[32];//遥信设备在平台中的编码
	int access_code_id;//关联到ob_access_dev中的id
	int yx_type;
	char yx_name[64];//遥信设备名称
	char yx_addr[32];//遥信设备号、端口号
	int normal_value;//正常状态值
	int current_value;//当前值
	BYTE dev_online_state;//在线状态
}_T_ACS_STATION_OB_YX_;

typedef struct _t_acs_station_camera_preset
{
	int id;//序列号
	int camera_id;//摄像机ID号
	int preset_id;//预置位号
	char preset_name[32];//预置位名称
}_T_ACS_STATION_CAMERA_PRESET_;

typedef struct _t_acs_station_ob_card
{
	int id;//序列号
	char code_door[32];//门卡在门禁系统中的编码
	char code_sys[32];//门卡在平台中的统一编码
	char user_name[32];//门卡用户的名称
	char personal_code[32];//用户工号
	char department[32];//用户所属部门
	int sex;//性别
}_T_ACS_STATION_OB_CARD_;

typedef struct _t_device_list_node_info_ 
{
	int node_id;
	int parent_id;
	char device_code[32];
	char device_name[64];
	BYTE IsOnLine;
	BYTE IsAlarm;
	BYTE IsRelateCamera;
	int relate_camera_id;
	BYTE relate_preset_id;
}_T_DEVICE_LIST_NODE_INFO_;


typedef struct _t_acs_station_ct_user_
{
	int user_id;
	char user_name[64];
	char user_password[32];
	BYTE user_type;
	BYTE enablectrl_p;
	BYTE realtalk_p;
	BYTE eventlog_p;
	BYTE replay_p;
	BYTE alarm_p;
	BYTE tunecycle_p;
	BYTE usermanage_p;
	BYTE devicemanage_p;
	BYTE videorecord_p;
	BYTE elecmap_p;
}_T_ACS_STATION_CT_USER_;

typedef struct _t_acs_rvu_camera_info_
{
	int camera_id;
	int dvr_id;
	char camera_code[32];
	char camera_name[64];
	int channel_id;
	int station_id;
}_T_ACS_RVU_CAMERA_INFO_;

typedef struct _t_acs_rvu_dvr_info_
{
	int dvr_id;
	int station_id;
	char dvr_code[24];
	char dvr_name[64];
	char dvr_type[16];
	char dvr_ip[24];
	int dvr_port;
	char dvr_username[32];
	char dvr_password[32];
}_T_ACS_RVU_DVR_INFO_;

typedef struct _t_acs_rvu_station_info_
{
	int user_id;
	int station_id;
	char station_code[32];
	char station_name[64];
}_T_ACS_RVU_STATION_INFO_;

typedef struct _t_ass_rvu_device_info_
{
	int id;
	char rvu_ip[16];
	int  rvu_port;
	char rvu_loginname[32];
	char rvu_loginpassword[32];
	int sm_id;
	int node_id;
	char node_name[NAMELENGTH];
	char node_code[32];
	int device_type;
	int node_type;
	int rvu_id;
	int station_id;
	int sm_type;
	int sub_sys_id;
}_T_ASS_RVU_DEVICE_INFO_;

typedef struct _t_ass_rvu_info_
{
	int rvu_id;
	char rvu_name[64];
	char rvu_code[32];
	char rvu_ip[16];
	int  rvu_port;
	char rvu_username[32];
	char rvu_userpassword[32];
	int station_id;
	int commnet_handle;
	int lost_heatbeat;
	int node_type;
	char rvu_type[32];
	//int reconnect;
}_T_ASS_RVU_INFO_;

typedef struct _t_ass_device_ledger_
{
	int id;
	int station_id;
	char szStationName[32];
	char device_name[32];
	char device_manufacturer[32];
	char device_ip[32];
	int device_port;
	char login_username[32];
	char login_password[32];
	char protocol_type[32];
	char device_type[32];
	char harddisk_capacity[32];
	int analog_video_num;
	int net_video_num;
}_T_ASS_DEVICE_LEDGER_;

typedef struct _t_tree_node_info
{
	DWORD dwDataPtr;
	int node_type;
	int parent_id;

}_T_TREE_NODE_INFO;

typedef struct _t_ass_rvu_id_handle_
{
	int rvu_id;
	int comnet_handle;
}_T_ASS_RVU_ID_HANDLE;

//用户名表
typedef struct _t_user_
{
	int user_id;
	char szUserName[256];
}T_USER;

#define ID_DELETE_LINKAGE_RECORD 20200
//设备结构体
typedef struct _t_dev_info_
{
	int nDevId;
	char szDevName[64];
	int nStationId;
	char szStationName[32];
	int nRvuId;
	char szRuvName[32];
	char szRvuIP[32];
	int nRvuPort;
}_T_DEV_INFO_;

//rvu类型结构体
typedef struct
{
	int nId;
	char szRvuType[32];
	char szDescription[128];
}T_ASS_RVU_TYPE;

typedef struct _t_linkage_config_info_
{
	int nId;
	int nDevId;//设备id
	char szDevName[32];//设备名称
	int nRelateDevId;//联动设备id
	char szRelateCameraCode[32];//联动摄像头
	char szRelateDevName[32];//联动设备名称
	int nRelateDevType;//联动类型，1：门禁，2：摄像头
	int nRelateDevAction;//联动动作，1：打开，2：关闭，3，转预置位
	int nPresetNum;//预置位编号
	char szPresetName[32];//预置位名称
	char szRelateCameraName[32];//联动摄像头名称
}_T_LINK_CONFIG_INFO_;

typedef struct t_camera_info
{
	int nCameraId;
	char szCameraCode[32];
	char szCameraName[32];
}T_CAMERA_INFO;

typedef struct t_door_info
{
	int nDevId;
	char szDevName[32];
}T_DOOR_INFO;

#endif