//CommonLib--网络连接数
#define DEFAULT_SERVER_NUM 512
#define DEFAULT_CLIENT_NUM 1024
#define DEFAULT_SERVER_CLIENT_NUM 8
#define MAX_ONTHREAD_NUM 64 //解析线程同时运行最大负载数

//CommonLib--网络缓冲区大小
#define DEFAULT_TCP_SERVER_RECV_BUF_SIZE (8*1024)
#define DEFAULT_TCP_SERVER_SOCKET_RECV_BUF_SIZE (64*1024)
#define DEFAULT_TCP_SERVER_SOCKET_SEND_BUF_SIZE (64*1024)

#define DEFAULT_TCP_CLIENT_RECV_BUF_SIZE (8*1024)
#define DEFAULT_TCP_CLIENT_SOCKET_RECV_BUF_SIZE (64*1024)
#define DEFAULT_TCP_CLIENT_SOCKET_SEND_BUF_SIZE (64*1024)

#define SOCKETSENDBUFMAXSIZE	1453
//数据大小
#define MAX_RVU_NUM_PER_STATION 512
#define RECVBUFSIZE 512*1024
#define MAX_SQL_LENTH 1024*2
#define STR_SQL_LENTH 256

#define NAMELENGTH			40	//名字命名长度	40	40字节长
#define PASSWORDLEN			20	//口令长度	20	20字节长
#define DES_LENGTH			160 //描述信息长度	160	160字节长
#define UNIT_LENGTH			8   //数据单位的长度	8	8字节长
#define VER_LENGTH			40  //版本描述的长度	40	40字节长
#define PHONE_LENGTH		20  //电话号码长度	20	20字节长
#define DATA_LENGTH			60  //数据长度	60	60字节长
#define SDESC_LENGTH		80  //短描述长度	80	80字节长
#define CARDLENGTH			20  //卡号长度	20	20字节长

#define MAX_FRAME_LEN   (1452+8)
#define USER_DATA_OFFSET  30
#define DATA_FRAME_HEADER  8
#define ETHERNET_FRAME_HEADER 14
#define SET_SM_ACK_DATA_OFFSET  30

#define MAX_SM_NUM_PER_STATION  128

#define MAX_STATION_NUM_PER_USER  1024
#define MAX_DVR_NUM_PER_USER      (MAX_STATION_NUM_PER_USER*16)
#define MAX_CAMERA_NUM_PER_USER   (MAX_DVR_NUM_PER_USER*16)

#define MAX_RVU_DEVICE_NUM_PER_STATION 1024

//协议类型定义
#define REQUEST_RVU_LOGIN                 101
#define RESPONSE_RVU_LOGIN                102
#define REQUEST_RVU_GET_SM                201
#define RESPONSE_RVU_SET_SM               202
#define REQUEST_RVU_GET_NODES             203
#define RESPONSE_RVU_SET_NODES            204
#define REQUEST_RVU_SET_DYN_ACCESS_MODE   301
#define RESPONSE_RVU_DYN_ACCESS_MODE_ACK  302

#define SEND_REALTIME_ALARM             401
#define HEART_BEAT                      601
#define HEART_BEAT_ACK                  602

#define REQUEST_RVU_SET_POINT            501
#define RESPONSE_RVU_SET_POINT_ACK       502


////////////////////////////////////////////////////

#define	DEVTYPE_HK					110		//海康
#define	DEVTYPE_DH					111		//大华
#define	DEVTYPE_DL					112		//大立
#define DEVTYPE_MOBOTIX				113		//mobotix
#define DEVTYPE_AXIS				114		//AXIS

#define OPENVIDEO_SUCCESS			0		// 打开视频成功
#define OPENVIDEO_LOADDLLERROR		1		// 加载播放SDK失败
#define OPENVIDEO_FINDDVRTYPEERROR	2		// 没有找到该类型解码库
#define OPENVIDEO_LOGINDVRERROR		3		// 登陆DVR失败
#define OPENVIDEO_PLAYERROR			4		// 打开视频失败


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
//XML解析消息定义
#define ASS_REQUEST_LOGIN           1
#define ASS_REQUEST_STATION          2
#define ASS_KEEPALIVE                3
#define ASS_REQUEST_YK_CONTROL      4
#define ASS_REQUEST_ALARM           5
#define ASS_REQUEST_HIDE            6
#define ASS_REQUEST_TIME            7

/////////////////////////////////////////////////////
typedef struct _recv_packet
{
	char recvbuf[RECVBUFSIZE];
	int size;
	int handle;

}RECV_PACKET;

typedef struct _c_recv_packet
{
	char recvbuf[RECVBUFSIZE];
	int size;
	int handle;
	int Client_Num;
	
}C_RECV_PACKET;


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

/*监控模块属性*/
typedef struct t_sm_
{
	long Size;//本数据结构的长度
	long Type;//数据的类型
	T_ID ID;//数据标识ID
	long MemberGropID;//用户组编号
	long PortID;//端口号
	short PortMode;//端口模式
	short BaudRate;//波特率
	long Address;//地址
	float Longitude;//经度
	float Latitude;//维度
	char Name[NAMELENGTH];//名字
	char Desc[SDESC_LENGTH];//描述
	long DeviceType;//设备类型
	long DeviceSubType;//设备子类型
	char Productor[NAMELENGTH];//生产厂家描述
	char Version[VER_LENGTH];//版本描述
	long CameraChannel;//摄像头通道号
	long RVUID;//RVU的编号
	long Reserver1;
	char Reserver2[NAMELENGTH];
	
}T_SM_;

typedef struct t_sm_load
{
	int count;
	int rvuid;
	T_SM_ *t_sm;

}T_SM_LOAD;
/*监控节点属性*/
typedef struct t_node_
{
	long Size;//长度
	long Type;//数据的类型 1YC 2YX 3YK 4YT
	T_ID ID;//数据标识
	T_ID VirtuaID;//保留
	char Name[NAMELENGTH];//名字 
	char Desc[SDESC_LENGTH];//描述
	float MaxVal;//有效上限
	float MinVal;//有效下限
	long AlarmEnable;//告警使能标记
	float HiLimit1;//一级告警上限
	float LoLimit1;//一级告警下限
	float HiLimit2;//二级告警上限
	float LoLimit2;//二级告警下限
	float HiLimit3;//三级告警上限
	float LoLimit3;//三级告警下限
	float Percision;//精度
	long Saved;//是否保存
	char Unit[UNIT_LENGTH];//单位
	long PHYID;//对应的物理节点
	long Reserver1;
	long Reserver2;
	long Reserver3;
	long Reserver4;
}T_NODE_;
typedef struct t_node_load
{
	int count;
	int rvuid;
	T_NODE_ *t_node;
	
}T_NODE_LOAD;

typedef struct _t_data_
{
	long type;//数据点类型
	T_ID id;//数据标识ID
	float t_value;//值
	long enumstate;//状态
	long size;//如果监控点是字符串量，则表示后接字符串长度；如果时间量，则为Ttime的结构大小，后跟时间量；如果为0，后面不跟任何数据
	char* desc;//描述
	int changeflag;//更新改变标志
	int alarmflag;//告警改变标志
	int updateflag;//上传标志
	
}TDATA;

typedef struct t_Data_
{
	long type;//数据点类型
	T_ID id;//数据标识ID
	float t_value;//值
	long enumstate;//状态
	long size;//如果监控点是字符串量，则表示后接字符串长度；如果时间量，则为Ttime的结构大小，后跟时间量；如果为0，后面不跟任何数据
	char* desc;//描述
	
}T_DATA_;
typedef struct t_data_load
{
	int count;
	int rvuid;
	T_DATA_ *t_data;
	
}T_data_LOAD;

typedef struct _t_ass_rvu_info_
{
	int rvu_id;
	int station_id;
	char rvu_code[32];
	char rvu_name[32];
	char rvu_ip[16];
	int  rvu_port;
	char rvu_username[41];
	char rvu_password[21];	
	int commnet_handle;
	int lost_heatbeat;
	int login_flag;
	char rvu_type[32];
	//int reconnect;
}_T_ASS_RVU_INFO_;

typedef struct _t_ass_rvu_data_
{
	int rvu_id;
	int station_id;
	int count;
	TDATA t_datas[MAX_SM_NUM_PER_STATION*10];
	int uploadflag;//1表示未上传 0表示已上传

}T_RVU_DATA;

typedef struct _t_sql_info_
{
	int ID;
	int num;

}_T_SQL_INFO;

typedef struct _t_send_rvu_info_
{
	char rvu_code[32];
	char rvu_ip[16];
	int  rvu_port;
	int rvu_handle;
	char send_buff[RECVBUFSIZE];
	int send_size;
}SEND_RVU_INFO;

typedef struct _t_rvu_node_info_
{
	int rvu_handle;
	int rvu_id;
	char rvu_ip[16];
	int rvu_port;
	TID tids[MAX_SM_NUM_PER_STATION];
	int count;
}RVU_NODE_INFO;


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
	int changeflag;//更新变更标示
}TALARM;

typedef struct t_talarm_
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
}T_ALARM_;

typedef struct _t_ass_rvu_alarm_
{
	int rvu_id;
	int station_id;
	int count;
	TALARM t_alarms[MAX_SM_NUM_PER_STATION];
	int uploadflag;//1表示未上传 0表示已上传
	
}T_RVU_ALARM;

typedef struct t_alarm_load
{
	int count;
	int rvuid;
	T_ALARM_ *t_alarm;
	
}T_ALARM_LOAD;
// 

typedef struct DBConfigParam 
{
	char LocalIP[16];
	int  LocalPort;
	char DBServerIP[16];
	int  DBServerPort;
	char DBName[32];
	char DBUserName[32];
	char DBUserPassword[32];
}DBConfigParam;

typedef struct _t_ass_login_user_info_
{
	char rvu_username[41];
	char rvu_password[21];
}_T_ASS_LOGIN_USER_INFO_;

typedef struct _t_ass_station_info_
{
	int user_id;
	int station_id;
}_T_ASS_STATION_INFO_;

typedef struct _t_ass_alarm_set_
{
	int user_id;
	int station_id[MAX_RVU_DEVICE_NUM_PER_STATION];
	int num;
	int type;//0删除 1增加
}_T_ASS_ALARM_SET_;

typedef struct _t_ass_hide_set_
{
	int user_id;
	int devID[MAX_RVU_DEVICE_NUM_PER_STATION];
	int num;
	int hide;//0删除 1增加
}_T_ASS_HIDE_SET_;

typedef struct _t_ass_control_
{
	int rvuid;
	int smid;
	int nodeid;	
	int value;
	int type;
}_T_ASS_CONTROL_;

typedef struct _t_ct_user
{
	int userid;
	int handle;
	int stationID;
	int AlarmStation[MAX_RVU_DEVICE_NUM_PER_STATION];
	int alarmstationNum;
	int devID[MAX_RVU_DEVICE_NUM_PER_STATION];
	int devidNum;
	int login_flag;//登陆标识 1：登陆 2：未登陆

}_C_LOGIN_USER;

typedef struct _t_dev_id
{
	int devID;
	int rvuid;
	int SmID;
	int NodeID;
	int enumtype;
}_T_DEV_ID;

typedef struct _t_104_yx1
{
	int addressID;
	int status;
}_T_104_YX1;

typedef struct _t_104_yx2
{
	int addressID;
	int status;
	char s_time[10];
}_T_104_YX2;



typedef struct _t_104_yx1e
{
	int addressID;
	int status;
	char cp_time[21];
}_T_104_YX1E;

typedef struct _t_104_buff
{
	BYTE head[1];//报头
	BYTE bufflen[2];//报文长度
	BYTE sendflag[1];//发送标志
	BYTE funcode[2];//功能码
	BYTE datalen[2];//数据长度
	BYTE data[32];//数据区
	BYTE end[1];//结束标志
}_T_104_BUFF;

typedef struct _t_104_SM
{
	BYTE smid[2];//设备编号
	BYTE nodeid[2];//节点编号
	BYTE dev_type[4];//设备类型
	char node_type[32];//节点类型 YC YX YK YT
	char devname[64];//设备名称
	char nodename[64];//节点名称
}T_104_SM;

typedef struct _t_104_real
{
	BYTE smid[2];//设备编号
	BYTE nodeid[2];//节点编号
	char t_value[32];//节点的值

}T_104_REAL;

typedef struct t_104_sm_load
{
	int count;
	int rvuid;
	T_104_SM t_sm[MAX_SM_NUM_PER_STATION*3];
	
}T_104_SM_LOAD;

typedef struct _c_link
{
	char c_code[20];
	int c_pset;
}C_LINK;

typedef struct _c_linkinfo
{
	int linknum;
	C_LINK c_link[6];
}C_LINKINFO;

typedef struct _c_camerainfo
{
	char ip[32];
	int port;
	char username[16];
	char password[16];
	char dvrtype[16];
	int channel;
}C_CAMERAINFO;

typedef struct _t_linkpush
{
	int handle;
	int devid;
	C_LINKINFO c_linkinfo;
}T_LINKPUSH;


