#ifndef LIBJSIC_H
#define LIBJSIC_H

#define  NAMELENGTH   40
#define  PASSWORDLEN  20
#define  DES_LENGTH   160
#define  UNIT_LENGTH  8
#define  VER_LENGTH   40
#define  PHONE_LENGTH 20
#define  DATA_LENTH   60
#define  SDESC_LENGTH 80
#define  CARDLENGTH   20 

#define  DEV_SUM      50

typedef unsigned char BYTE;


/*时间结构*/
typedef struct t_time
{
	short Years;//年
	char Month;//月
	char Day;//日
	char Hour;//时
	char Minute;//分
	char Second;//秒
	char Milsecond;//毫秒
}T_TIME;
/*监控编号的结构*/
typedef struct t_id
{
	short SmID;//监控模块编号  dev表ID 对应yx/yk/yc表 node_code_id
	short NodeID;//监控节点编号 yk/yx/yc表 ID
}T_ID;

/*监控节点属性*/
typedef struct t_node
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
}T_NODE;

/*监控模块属性*/
typedef struct t_sm
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

}T_SM;


//告警结构
typedef struct t_alarm
{
	long Type;//数据点类型
	T_ID t_ids;//数据点ID
	T_TIME StartTime;//告警发生时间
	T_TIME EndTime;//告警结束时间
	T_TIME ConfirmTime;//告警确认时间
	T_TIME CancelTime;//告警取消时间
	long AlarmLevel;//告警级别
	float AlarmValue;//数值
	long EnumAlarmTime;//当前告警是属于发生还是结束，确认或取消
	char Description[DATA_LENTH];//告警值描述

}T_ALARM;
typedef struct t_recode
{
	long size;//数据结构的长度
	T_ID ID;//摄像头的编号
	T_TIME StartTime;//开始时间
	T_TIME EndTime;//结束时间
	long LiveTime;//生存时间
	long RecordFlag;//录像原因
	long SevialNo;//录像文件流水号，唯一
	long VideoFormat;//录像存储格式
	long Reserver1;//保留1
	long Reserver2;//保留2
	long Reserver3;//保留3
	char Reserver4[NAMELENGTH];//保留4
}T_RECODE;

typedef struct t_Pro
{
	BYTE txPro[8];//通信层协议
	BYTE hlPro[18];//互联协议
	BYTE yhPro[1452];//用户数据

}T_PRO;

typedef struct t_Data
{
	long type;//数据点类型
	T_ID id;//数据标识ID
	float t_value;//值
	long enumstate;//状态
	long size;//如果监控点是字符串量，则表示后接字符串长度；如果时间量，则为Ttime的结构大小，后跟时间量；如果为0，后面不跟任何数据
	char* desc;//描述

}T_DATA;
//联动结构
typedef struct t_tactivelink
{
	long size;
	long aid;
	T_ID id;
	T_ID linkid;
	long linkmode;
	long ctrl;
	float value;
	float activevalue;
	float unactivevalue;
	long timelength;
	char name[NAMELENGTH];
	long reserver1;
	long reserver2;
	long reserver3;
	long reserver4;

}TACTIVELINK;
//预案配置结构
typedef struct t_presch
{
	long size;
	long PID;
	T_ID ID;
	long startoper;
	long endoper;
	long reserver1;
	long reserver2;
	long reserver3;

}TPRESCHEME;
//预置位配置结构
typedef struct t_preset
{
	long size;
	t_id ID;
	char No;
	char name[NAMELENGTH];
	short reserver1;
	short reserver2;
	long reserver3;

}TPRESET;

typedef struct t_HlPro
{
	BYTE hlPro[1452];
}T_HLPRO;
//登陆
typedef struct js_Login
{
	char username[NAMELENGTH];
	char password[PASSWORDLEN];
	long memberID;
	long enumlog;
	
}JS_LOGIN;
//登陆
typedef struct js_ReLogin
{

	long MemberLevel;
	char Desc[DES_LENGTH];
	
}JS_RELOGIN;
typedef struct b_ReLogin
{
	BYTE MemberLevel[4];
	BYTE Desc[DES_LENGTH];
		
}B_RELOGIN;
//系统结构
typedef struct js_Sys
{
	long cnt;
	struct t_id tids[DEV_SUM];
}JS_SYS;

typedef struct js_ReSysSM
{
	long cnt;
	struct t_sm tsms[DEV_SUM];
}JS_RESYS_SM;
typedef struct js_ReSysNODE
{
	long cnt;
	struct t_node tnodes[DEV_SUM];
}JS_RESYS_NODE;
//实时数据
typedef struct js_Real
{
	long mode;//0:一问一答 1：改变时自动发送 2：定时发送 3：停止发送
	long pollingtime;//定时方式时的发送间隔秒数
	long cnt;
	struct t_id tids[DEV_SUM];
}JS_REAL;

typedef struct js_ReReal
{
	long result;//设定成功与否的标志 0表示失败
	long cnt;//返回数据值的数量
	struct t_Data tdatas[50];//相应的值与状态
}JS_REREAL;

typedef struct b_ReReal
{
	BYTE result[4];//设定成功与否的标志 0表示失败
	BYTE cnt[4];//返回数据值的数量
	BYTE values[1200];//相应的值与状态
}B_ReREAL;

//告警
typedef struct js_Warning
{
	long cnt;
	struct t_alarm t_alarm[DEV_SUM];
//	T_ALARM t_alarm;
	
}JS_WARNING;
//告警
typedef struct b_Warning
{
	BYTE cnt[4];
	BYTE values[1024];

}B_WARNING;
//时间同步
typedef struct js_Time
{
	T_TIME t_time;
}JS_TIME;
//时间同步
typedef struct js_ReTime
{
	long result;
	char desc[DES_LENGTH];
}JS_RETIME;
//时间同步
typedef struct b_ReTime
{
	BYTE result[2];
	BYTE desc[DES_LENGTH];
}B_RETIME;

//控制命令
typedef struct js_Control
{
	T_ID t_id;//节点编号
	T_DATA t_data;//控制值
}JS_CONTROL;

//控制命令回复
typedef struct js_ReCon
{
	long result;//成功/失败
	char desc[DES_LENGTH];//描述
}JS_RECON;

//控制命令回复
typedef struct b_ReCon
{
	BYTE result[4];
	BYTE desc[DES_LENGTH];
}B_RECON;


extern "C" void quyu(int n,int c,int *s,int *y);
extern "C" int RECV_TYPE(BYTE *t_buff,BYTE *buff,int lenth);
extern "C" void TxProtocol(T_PRO *t_pro,int pk_type,BYTE *yh_buff,int lenth);
extern "C" void HlProtocol(BYTE *hl_buff,int pk_type,int s_num,int lenth);
extern "C" void RVU_RECV_JsLogin(BYTE *l_byte,JS_RELOGIN *js_relogin);
extern "C" void RVU_SEND_JsLogin(JS_LOGIN js_login,BYTE *l_byte,int *lenth);
extern "C" void RVU_SEND_CONN(BYTE *l_byte);
extern "C" void RVU_RECV_JsSys_SM(BYTE *l_byte,JS_RESYS_SM *js_resys);
extern "C" void RVU_RECV_JsSys_NODE(BYTE *l_byte,JS_RESYS_NODE *js_resys);
extern "C" void RVU_SEND_JsSys(JS_SYS js_sys,BYTE *l_byte,int *lenth);
extern "C" void RVU_RECV_JsReal(BYTE *l_byte,JS_REREAL *js_rereal);
extern "C" void RVU_SEND_JsReal(JS_REAL js_real,BYTE *l_byte,int *lenth);
extern "C" void RVU_RECV_JsTime(BYTE *l_byte,JS_RETIME *js_retime);
extern "C" void RVU_SEND_JsTime(JS_TIME js_time,BYTE *l_byte,int *lenth);
extern "C" void RVU_RECV_JsWarn(BYTE *l_byte,JS_WARNING *js_warning);
extern "C" void RVU_SEND_JsControl(JS_CONTROL js_control,BYTE *l_byte,int *lenth);
extern "C" void RVU_RECV_JsControl(BYTE *l_byte,JS_RECON *js_recon);




#endif