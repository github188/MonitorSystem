#ifndef _xappany_H_
#define _xappany_H_

#ifndef   __STACK_H__   
#define   __STACK_H__

#include ".\include\mysql\mysql.h"				//连接数据库头文件

//------------------------------------------------------------------------//
//预定义
//------------------------------------------------------------------------//

#define HIKVISION				110
#define DAHUA					111
#define HENGYI					112
#define HANBANG					113
#define LANSEZHIXING			114
#define DLINK					115
#define DALI					116
#define AXIS					117
#define SONY					118
//#define NANZI					119
#define PANASONIC				120
#define AMS						121
#define GRANDSTREAM				122
#define MOBOTIX					124
#define NANZI					125
#define XIANJINYUAN				161		//西安金源

#define	IPADDRESS_LEN			   16		//IP地址的长度

#define	CAMERANAMELEN			256		//摄像头名称长度
#define	CAMERACALLNUMLEN		32			//摄像头呼叫号码长度
#define	DVRNUMLEN				        64		//DVR号码长度
#define DVRIPLEN				            32		//DVR IP长度
#define	DVRUSERNAMELEN			256		//DVR用户名长度
#define	DVRPASSWORDLEN			64			//DVR密码长度

#define	TOTALCAMERANUM			10000	//最大支持的摄像头数目
#define TOTALDVRNUM				2000		//最大支持的DVR数目

#define    MAXNODENUM				1024*32

#define    CHANNEL					        1			//语音对讲g711编解码参数
#define    BITS_PER_SAMPLE			    16			//语音对讲g711编解码参数
#define    SAMPLES_PER_SECOND    8000		//语音对讲g711编解码参数
#define	    BITRATE					        16000	//语音对讲g711编解码参数

#define R_DATE_LEN 				        20			//录像指令中的时间长度
#define R_USERNAME_LEN			32			//录像指令中的DVR用户名长度
#define R_PASSWORD_LEN 			50			//录像指令中的DVR密码长度

#define DISPACKETBUF			1024*1024	 //接收到的数据包最大包的大小

#define MAXVIEWCH			 9					    //多画面中的画面通道总数
#define MAPVIEWCHA			25						//电子地图的通道号A
#define MAPVIEWCHB			30						//电子地图的通道号B
#define MAPVIEWCHC			31						//电子地图的通道号C
#define MAPVIEWCHD			32						//电子地图的通道号D
#define TEMPFAVORITECHA		26				    //临时收藏夹通道A
#define TEMPFAVORITECHB		27				    //临时收藏夹通道B
#define TEMPFAVORITECHC		28				    //临时收藏夹通道C
#define TEMPFAVORITECHD		29	                //临时收藏夹通道D
#define REALTALKCHREMOTE	33				    //语音对讲远端视频端口
#define REALTALKCHLOCAL		34				    //语音对讲本地视频端口
#define REPLAYCH			            35					//回放视频端口

//////////////////////////////////////////////////////////////////////////
#define	OM_CHANNLECHANGE	OM_MESSAGE				//视频通道切换通道消息定义
#define   OM_MESSAGE			WM_USER + 200
#define	OM_DBLCHANNEL		 OM_MESSAGE + 1			//视频通道双击消息定义
#define	OM_RIGHTCLICKCHANNEL		 OM_MESSAGE + 2			//视频通道右键消息定义
#define	OM_CONTROLBUTTON	 OM_MESSAGE + 3		//各窗体之间的按钮消息
#define	OM_YTCONTROL		 OM_MESSAGE + 9			//云台控制按钮消息

/////////////////////////////////////各窗口之间的通信////////////////////////////////
#define OM_ELECTREE_TO_CAMERAANDCONTROL_STATIONID (OM_MESSAGE + 10)//电子地图树向视频控制窗口传变电站ID

//////////////////////////////////////////////////////////////////////////
#define    WM_MAGIC_VEMCUCTL   9669
#define    WM_SELECT_DEVICE_NOTIFY  (WM_USER+666) //选择设备

//////////////////////////////////////////////////////////////////////////
#define    WM_VIDEO_LINKAGE_MANUAL_MESSAGE  (WM_USER+700) //手动联动消息
#define    WM_VIDEO_LINKAGE_ALARM_MESSAGE     (WM_USER+701)  //告警联动消息
#define    WM_VIDEO_LINKAGE_STATE_MESSAGE       (WM_USER+702)  //状态变位消息
#define    WM_VIDEO_CAMERA_CONTROL_NOTIFY_MESSAGE  (WM_USER+703)  //摄像头云镜控制通知消息
#define    WM_VIDEO_WEATHER_WARNING_LINKAGE_MESSAGE  (WM_USER+704) //气象告警联动消息
#define    WM_VIDEO_WEATHER_FORECAST_LINKAGE_MESSAGE  (WM_USER+705) //气象预警联动消息

//////////////////////////////////////////////////////////////////////////
#define WM_SYSTEM_TRAY_ICON_NOTIFY  (WM_USER+766) //系统托盘消息

//////////////////////////////////////////////////////////////////////////
#define  WM_DEVICE_VIDEO_CLOSE_MESSAGE (WM_USER+810)                              //视频关闭消息
#define  WM_DEVICE_VIDEO_ERROR_REASON_NOTIFY_MESSAGE (WM_USER+811) //视频错误原因通知消息
#define  WM_DEVICE_VIDEO_TALK_NOTIFY_MESSAGE (WM_USER+812)                   //视频打开通知消息
#define  WM_DEVICE_VIDEO_QUICK_RESTART_MESSAGE (WM_USER+816)               //视频快速重连消息
#define  WM_DEVICE_VIDEO_STATUS_ONLINE_NOTIFY_MESSAGE  (WM_USER+817)  //视频状态在线通知消息

//////////////////////////////////////////////////////////////////////////
#define  WM_UPDATE_USER_VIEW_CAMERA_STATUS_MESSAGE  (WM_USER+1019) //更新摄像头状态消息
#define  WM_ELECMAP_OPEN_CAMERA_MESSAGE (WM_USER+1020)	//电子地图打开摄像头消息

//////////////////////////////////////////////////////////////////////////
#define  WM_DEVICE_VIDEO_TALK_OPERATE_RESULT_FAIL  11
#define  WM_DEVICE_VIDEO_TALK_OPERATE_RESULT_SUCCESS  12
#define  WM_DEVICE_VIDEO_OPERATE_RESULT_NOTAG_OFFLINE_VIDEO 13
#define  WM_DEVICE_VIDEO_OPERATE_RESULT_NOTAG_ONLINE_VIDEO 14
#define  WM_DEVICE_VIDEO_OPERATE_RESULT_TAG_ONLINE_VIDEO 15
#define  WM_DEVICE_VIDEO_OPERATE_RESULT_TAG_OFFLINE_VIDEO 16


//自定义消息_初始化USB摄像头
#define WM_INIT_USB_VIDEO (WM_USER+2555)
//自定义消息_关闭USB摄像头
#define WM_EXIT_USB_VIDEO (WM_USER+2556)
//自定义消息_收到报警信息
#define WM_RECEIVED_ALARMINFO (WM_USER+2656)
//自定义消息_发起订阅成功
#define WM_SUB_OK (WM_USER+2657)
//自定义消息_发起订阅失败
#define WM_SUB_FAIL (WM_USER+2658)

#define   PAGEVIEWHEIGHT 40                     //手动轮巡导航高度
#define   LINKAGEWIDTH     268                   //联动宽度
#define	  CONTROLWIDTH	278				    //云台控制栏的宽度
#define	  TITLEHIGHT			    0					//标题栏的高度
#define   MENUHIGHT			147					//菜单栏的高度
#define   ACCEHIGHT			    0					//下部栏的高度
#define   ALARMHIGHT			200						//报警栏的高度
#define   SHOWALARMHIGHT			10						//显示报警栏那个按钮的高度

#define   CYCLENUM			32						    //最大的轮巡方案数目
#define   VIEWCHANNELMETHOD	6				//轮巡中的多画面方案最大为6

#define	TIMER_MAIN_EVENTID	1						//主对话框的主时钟ID
#define	TIMER_CYCLE_EVENTID	2				    //主对话框的轮巡时钟ID
#define TIMER_WEATHER_EVENTID  3              //气象刷新时钟ID  
#define TIMER_YT_LOCK_CHECK_EVENTID 4    //检测云台锁定时钟ID
#define TIMER_VIDEO_FLOW_EVENTID      5     //流量检测
#define TIMER_SUBSCRIBE_CAMERA_STATUS_EVENTID      6    //订阅摄像头状态
#define TIMER_CHECK_AND_UPDATE_SYSTEM_SOFTWARE_EVENTID     7    //检测并自动更新系统
#define TIMER_VIDEO_PAGEVIEW_AUTOINFO 8          //人工自动巡视时钟ID

#define TIMER_VIDEO_ERROR_REASON_VIDEO_CHANNEL_0_TIMER  1200		//主对话框的视频错误原因视频通道0
#define TIMER_WEATHER_CHANNEL_0_TIMER  1300       //视频通道0实时气象时钟ID  
#define TIMER_MICRO_WEATHER_CHANNEL_0_TIMER  1400       //视频通道0微气象时钟ID  
#define TIMER_HISTORY_WEATHER_CHANNEL_0_TIMER 1500      //视频通道0历史气象时钟ID 
#define TIMER_VIDEO_INFO_SHOW_CHANNEL_0_TIMER 1600      //视频通道0视频信息显示时钟ID 
#define TIMER_ACSSYSTEM_LOAD_DLL 1700	//加载dll库的定时器

#define		TOTALSYSNUM		10						             //支持的最大系统数
#define		TOTALALARMDEVICENUM		1				//支持的最大报警设备数

// D5000系统最大的变电站节点数
#define	  MAX_STATION_NODE_NUM		(2*1024)
#define   MAX_STATION_NUM				    (4*1024)

enum
{	
	    REPLAY_NORMALSPEED	= 0,
		REPLAY_2HIGHSPEED,
		REPLAY_4HIGHSPEED,
		REPLAY_8HIGHSPEED,
		REPLAY_2LOWSPEED,
		REPLAY_4LOWSPEED,
		REPLAY_8LOWSPEED
};

enum
{
		REPLAY_PLAY = 0,
		REPLAY_STOP = 1,
		REPLAY_PAUSE = 2
};

enum
{
		SLDVR_CALLEE = 0,		//潮流dvr
		SLPHONE_CALLEE = 1,	//潮流话机
		EYEBEAM_CALLEE = 2,	//eyebeam
		RTSIP_CALLEE = 3,		    //rtsip客户端
};

enum AMC_POPUPS
{
	/// AMC will not show any Popups.
	AMC_POPUPS_NONE		      =	0L,

	/// AMC will show a Login Dialog if the username or password is incorrect.
	AMC_POPUPS_LOGIN_DIALOG	=	1L,

	/// All messages are shown as Popups.
	AMC_POPUPS_MESSAGES			=	2L,

	/// "No Video" overlay is shown when the stream is stopped.
	AMC_POPUPS_NO_VIDEO  = 4L,

	/// Untrusted certificates will be rejected without prompting the user.
	AMC_POPUPS_SUPPRESS_UNTRUSTED_CERTIFICATE_ALERT  = 8L,
};

enum AMC_STATUS
{
	/// AMC is uninitialized
	AMC_STATUS_NONE                           = 0L,

	/// AMC is initialized and ready.
	AMC_STATUS_INITIALIZED                    = 1L,

	/// A media stream/file is playing.
	AMC_STATUS_PLAYING                        = 2L,

	/// Playing of a media stream/file is paused.
	AMC_STATUS_PAUSED                         = 4L,

	/// A media stream/file is being recorded.
	AMC_STATUS_RECORDING                      = 8L,

	/// AMC is opening a media stream/file.
	AMC_STATUS_OPENING                       = 16L,

	/// AMC is performing reconnection.
	AMC_STATUS_RECONNECTING                  = 32L,

	/// Issuing a Pan, Tilt, Zoom command to the device.
	AMC_STATUS_ISSUING_PTZ_COMMAND          = 512L,

	/// An extended text/message is displayed in the status bar.
	AMC_STATUS_EXTENDED_TEXT               = 1024L,

	/// UIMode is set to ptz-absolute.
	AMC_STATUS_PTZ_UIMODE_ABS              = 2048L,

	/// UIMode is set to ptz-relative or ptz-relative-no-cross.
	AMC_STATUS_PTZ_UIMODE_REL              = 4096L,

	/// The stream for receiveing audio is being opened.
	AMC_STATUS_OPENING_RECEIVE_AUDIO      = 65536L,

	/// AMC is opening the stream for transmitting audio.
	AMC_STATUS_OPENING_TRANSMIT_AUDIO    = 131072L,

	/// Receiving audio.
	AMC_STATUS_RECEIVE_AUDIO             = 262144L,

	/// Transmitting audio.
	AMC_STATUS_TRANSMIT_AUDIO            = 524288L,

	/// An audio file is being transmitted.
	AMC_STATUS_TRANSMIT_AUDIO_FILE      = 1048576L,

	/// Recording audio.
	AMC_STATUS_RECORDING_AUDIO          = 2097152L
};

enum AMC_VIDEO_RENDERER
{
	/// Video Mixing Renderer 7
	AMC_VIDEO_RENDERER_VMR7          = 0x00000,

	/// Video Mixing Renderer 9
	AMC_VIDEO_RENDERER_VMR9          = 0x01000,

	/// Enhanced Video Renderer
	AMC_VIDEO_RENDERER_EVR           = 0x10000
};

/// Option flags for StartRecordMedia
enum AMC_RECORD_FLAG
{
	AMC_RECORD_FLAG_NONE              = 0,

	// Record received audio samples
	AMC_RECORD_FLAG_RECEIVED_AUDIO    = 1,

	// Record transmitted audio samples
	AMC_RECORD_FLAG_TRANSMITTED_AUDIO = 2,

	// If set, audio should be saved in its encoded form. Otherwise in wav(pcm)-format.
	AMC_RECORD_FLAG_ENCODED_AUDIO     = 4,

	// Record video only
	AMC_RECORD_FLAG_VIDEO 			      = 8,

	// Record both video and audio
	AMC_RECORD_FLAG_AUDIO_VIDEO       = 16
};
//------------------------------------------------------------------------//
//结构体定义
//------------------------------------------------------------------------//
//用户设备访问权限
typedef	struct	_t_UserDevicePower
{
	int	view;
	int	ytcontrol;
	int	preset;
	int	viewsetting;
	int	record;
	int	capture;
}PUDPR,*PPUDPR;

//用户控制权限
typedef	struct	_t_UserControlPower
{
	int	videoview;
	int	ytcontrol;
	int	replay;
	int	historylog;
	int	devicemanage;
	int    tunecycle;
	int    usermanage;
	int    eventlevel;
}PUCPR,*PPUCPR;

//用户总的权限
typedef struct _t_UserTotalPower
{
	int	userID;
	char	username[32];
	char	password[32];
	int    logintype;
	int    usertype;
	int    groupno;
	int    subgroupno;
	char   groupname[64];
	int    userlevel;
	PUCPR	userCP;
}PUPR,*PPUPR;

//DVR信息结构体
typedef struct _T_DVR_INFO
{
	int       dvr_id;                     //DVR序号
	int       dvr_type;                 //DVR类型
	char	   dvr_name[64];         //DVR名称
	char	   dvr_num[32];           //DVR号码
	char	   dvr_ip[32];                //DVR的IP地址
	int       dvr_port;                  //DVR的PORT端口
	char    dvr_user[64];            //DVR的用户名
	char	   dvr_password[64];   //DVR的密码
	int       station_id;                //站点序号
	char    station_num[32];     //站点号码
	char    station_name[64];   //站点名称
}_t_dvr_info;

typedef struct _T_DIAGNOSIS_INFO
{
	int nTotalResult;//是否有故障
	int nLoginErr;//登录失败   
	int nOpenVideoErr;//打开视频失败
	int nRecordErr;//录像失败
	int nLose;//视频丢失
	int nImpluse;//噪点
	int nStripe;//条纹干扰
	int nBright;//亮度异常
	int nVague;//视频模糊
	int nColor;//视频偏色
	int nPTZ;//云台失控
	int nShake;//抖动
}_t_diagnosis_info;

//摄像头信息结构体
typedef struct _T_CAMERA_INFO
{
	int camera_flag;
	int channel;
	char camera_name[64];
	char camera_num[32];
	//_T_DIAGNOSIS_INFO diagnosis_info;
	_T_DVR_INFO dvr_info;
}_t_camera_info;

// 节点信息，任意节点
typedef struct node_info
{
	int    node_type;		//0表示系统，1表示节点，2表示站点，3表示camera，4表示IO设备
	char node_num[20];
	char node_name[64];
	int    node_id;
	int    node_status;				        // 节点的状态，0表示不可用，1表示可用，2表示手动标注为不可用
	int    node_streamless;              //节点流量,0表示有流量,1表示没有流量
	float node_longitude;
	float node_latitude;
	int	node_decodetag;				//节点的解码标签
	char node_station[64];			    //节点所属站点的站点名称
	char node_stationnum[20];		//节点所属站点的站点编码
	_T_CAMERA_INFO camera_info;   //CAMERA信息
}_T_NODE_INFO, *_PT_NODE_INFO;

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

typedef struct
{
	char szStationName[64];
	int nDoorCount;
	int nAnFangCount;
	int nFireCount;
}_T_ASS_SUBSYSTEM_ALARM_COUNT;

// 预置位节点信息
typedef struct node_preset_info
{
	int		node_type;			    // 0表示系统，1表示节点，2表示站点，3表示camera，4表示IO设备，5表示预置位，6表示区域组，7表示设备组
	char	    node_num[20];		// 表示站点编码或者摄像头编码
	char	    node_name[64];		// 表示站点名称或者摄像头名称
	int		node_id;			        // 表示站点或摄像头在数据表中的id号
	char	    preset_name[64];	// 表示预置位的名称
	int		preset_id;			    // 表示预置位的号码
	int		node_status;		    // 节点的状态，0表示不可用，1表示可用，2表示手动标注为不可用
	int        node_streamless;  // 节点流量,0表示有流量,1表示没有流量
	float	    node_longitude;	    // 表示摄像头的坐标
	float	    node_latitude;		    // 表示摄像头的坐标
	int		node_decodetag;   // 摄像头的解码标签
	char	    node_station[64];    // 节点所属站点的站点名称
	char  	node_stationnum[20];// 节点所属站点的站点编码
	int       line1_from_x;
	int       line1_from_y;
	int       line1_to_x;
	int       line1_to_y;
	int       line2_from_x;
	int       line2_from_y;
	int       line2_to_x;
	int       line2_to_y;
	_T_CAMERA_INFO  camera_info;  //CAMERA信息
}_T_NODE_PRESET_INFO, *_PT_NODE_PRESET_INFO;

// 轮巡节点信息
typedef struct _tunecycle_node_info_
{
	int    node_type;		                 //0表示系统，1表示方案，2表示camera
	char node_num[20];                   //节点号
	char node_name[64];                 //节点名称
	int    node_status;				         // 节点的状态，0表示不可用，1表示可用
	int    node_streamless;               //节点流量,0表示有流量，1表示没有流量
	float node_longitude;
	float node_latitude;
	int	 node_decodetag;				//节点的解码标签
	char  node_station[64];			    //节点所属站点的站点名称
	char  node_stationnum[20];		//节点所属站点的站点编码
	int     nTime;                              //轮巡时间
	int     nCycleNum;                     //轮巡数
	_T_CAMERA_INFO  camera_info;  //CAMERA信息
}_T_TUNECYCLE_NODE_INFO, *_PT_TUNECYCLE_NODE_INFO;

// 变电站节点信息
typedef struct _t_station_node_info
{
	int		node_id;		//节点ID号
	int		parent_id;		//父节点ID号
	char	    node_name[64];	//节点名称
	int		order_num;		//排序号码
	int        ywb_id;           //省检
	int        login_type;      //登录类型
	int        voltage_class; //电压等级
	char	    notes[64];		//备注
}_T_STATION_NODE_INFO, *_PT_STATION_NODE_INFO;

// 变电站信息
typedef struct _t_station_info
{
	int	station_id;		//站点ID号
	char	station_name_videoplant[64];	//站点在视频平台中的名称
	char	station_code_videoplant[20];     //站点在视频平台中的号码
	char	station_name_d5000[64];           //站点在D5000中的名称
	char	station_code_d5000[20];             //站点在D5000中的号码
	char	station_name_oms[64];              //站点在OMS中的名称
	char	station_code_oms[20];               //站点在OMS中的号码
	char	station_name_meteo_a[64];       
	char	station_code_meteo_a[20];
	char station_code_weather[64];        //站点在气象中的号码
	char station_code_typhoon[64];        //站点在台风中的号码
	char station_code_microclimate[64];//站点在微气象中的号码
	int    station_video_status;
	int    voltage_class;                              //电压等级
	int	node_id;                                       //省调
	int		   node_sd_yw_id;		//省调
	int    node_yw_id;                                //省检
	char notes[256];                                 //备注
}_T_STATION_INFO, *_PT_STATION_INFO;

// 系统信息结构体定义
typedef struct T_SYSTEM_INFO
{
	char	sysid[20];		//系统号
	char	sysname[64];	//系统名称
	char	cms_ip[20];		//cms的ip地址
	int	cms_port;		//cms的端口
	char	http_ip[20];	//http的ip地址
	int	http_port;		//http的端口
	int	sys_status;		//系统的状态
	char	upsysid[20];	//上级系统号
	int	localdomain;	//是否是本级系统，1：表示本级系统，2：表示上级系统，0表示下级系统
}_T_SYSTEM_INFO, *_PT_SYSTEM_INFO;

typedef struct _t_Replay_Query_Info
{
	bool	   system_uporlow;			//查询本级系统还是下级系统，0表示本级，1表示下级
	int		query_type;				//查询类型，0表示基于视频通道和时间范围查询，1表示基于告警事件查询
	char	    choose_address[32];		//选定的设备地址编码
	int		alarm_type;				//告警类型选择：choose_address为视频通道时，0x0表示视频丢失告警，0x1表示移动侦测告警，0x2表示视频遮挡告警
									//				choose_address为编码设备时，0x80表示设备高温告警，0x81表示低温告警，0x82表示风扇故障告警，0x83表示磁盘故障告警
									//				0xff表示所有告警
	char	start_time[22];			//开始时间2005-11-10T12:05:10
	char	stop_time[22];			//结束时间2005-11-10T12:05:10
}_T_REPLAY_QUERY_INFO, *_PT_REPLAY_QUERY_INFO;

typedef struct _t_VideoPageViewInfo
{
	int nTimeSpan;
	int nStatus;
	char szStationName[256];
	char szFirstStationName[256];
}_T_VIDEOPAGEVIEWINFO,*_PT_VIDEOPAGEVIEWINFO;

typedef struct _t_TuneCycle_MethodInfo
{
	int		MethodID;
	bool  	bState;
	char	    MethodName[64];
	int		TimeInt;
	int		ViewNum;
	int		TotalCameraNum;
	char	    CameraNum[100][20];
	char	    CameraName[100][128];
	int		CameraType[100];
	bool	    PlayFlag[100];
	int		CurrentID;
}_T_TUNECYCLE_METHODINFO, *_PT_TUNECYCLE_METHODINFO;

// 报警设置信息列表
typedef struct _t_AlarmInfoList
{
	char	node_name[64];
	char	node_num[20];
	char	node_stationnum[20];
	char	node_stationname[64];
	int		node_type;
	int		dvr_disk;
	int		dvr_temp;
	int		dvr_fan;
	int		camera_videolost;
	int		camera_videomove;
	int		camera_videocover;
	int		io_state;
	bool	dvr_disk_disable;
	bool	dvr_temp_disable;
	bool	dvr_fan_disable;
	bool	camera_videolost_disable;
	bool	camera_videomove_disable;
	bool	camera_videocover_disable;
	bool	io_state_disable;
}_T_ALARMINFOLIST, *_PT_ALARMINFOLIST;

typedef struct _t_AlarmReceivedList
{
	char	node_code[20];
	char	alarm_type[20];
	char	alarm_time[22];
}_T_ALARMRECEIVEDLIST, *_PT_ALARMRECEIVEDLIST;

// 告警联动设置
typedef struct _t_AlarmLink_Info
{
	char alarmdev_name[64];
	int	alarmdev_type;
	char alarmdev_code[20];
	char linkdev_name[64];
	char linkdev_code[20];
	int linkdev_action;			// 联动动作类型
	int linkdev_actionnotes;	// 联动为预置位时，为预置位号；联动为录像时，为录像时间
	int link_level;
	char notes[64];
	char stationname[64];
}_T_ALARMLINK_INFO, *_PT_ALARMLINK_INFO;

//------------------------------------------------------------------------//
//全局变量定义
//------------------------------------------------------------------------//
extern char				g_DBServerIP[IPADDRESS_LEN];			//数据库服务器的地址
extern char               g_HttpServerIP[IPADDRESS_LEN];      //Http服务器地址
extern char               g_LinkServerIP[IPADDRESS_LEN];      //联动服务器地址
extern int                  g_nDBServerPort;                             //数据库服务器端口
extern int                  g_nHttpServerPort;                          //Http服务器端口              
extern int                  g_nLinkServerPort;                           //联动服务器端口
extern MYSQL			*g_mySqlData;						            //mysql数据库访问
extern char               g_DBName[256];
extern char g_AcsServerIp[IPADDRESS_LEN]; //辅助系统服务器的地址
extern int	 g_nAcsServerPort;	//辅助系统服务的端口
extern int g_nShowManagerClient;//是否显示管理客户端

// 用户登录结构
extern bool				app_IsUserLogin;					// 用户登录状态
//extern MDCS_USER		app_User;						// 登录用户


//用户语音对讲
extern int				g_ua_realtalk_callid;
extern int				g_ua_realtalk_direction;
extern int				g_ua_realtalk_state;

//登陆用户权限
extern PUPR			g_userpower;
extern int              g_nUserType;//1:表示admin用户,2:表示省检用户,3表示省调用户
extern int              g_nUserResourceShowType;//1:表示按admin方式显示,2:表示按省检管理结构显示,3:表示按省调电压等级显示


//人工巡视信息
extern _T_VIDEOPAGEVIEWINFO g_VideoPageViewInfo;

// 系统信息结构体
extern	_T_SYSTEM_INFO	g_SystemInfo[TOTALSYSNUM];
extern	int		g_TotalSysNum;

// 报警设置设备信息列表
extern	_T_ALARMINFOLIST g_AlarmInfoList[TOTALALARMDEVICENUM];
extern	int		g_TotalAlarmInfoListNum;

// 恒易固定头
extern BYTE				pDSPHeader[19];

// 轮巡方案
extern _T_TUNECYCLE_METHODINFO	g_TuneCycle_MehtodInfo;

// 变电站上一级的节点信息
extern _T_STATION_NODE_INFO	g_tStationNode_Info[MAX_STATION_NODE_NUM];
extern int					g_nStationNode_Num;

// 所有变电站的信息
extern _T_STATION_INFO		g_tStation_Info[MAX_STATION_NUM];
extern int					g_nStation_Num;


extern void	*pDecHandle;						//编解码句柄
extern void	*pEncodeHandle;

typedef struct _t_preset_setting
{
	int id;
	int up_id;
	char camera_name[256];
	char camera_code[20];
	char preset_name[256];
	int preset_code;
	int node_type;			// 1表示节点，0表示摄像头
	int decoder_tag;
}_T_PRESET_SETTING, *_PT_PRESET_SETTING;

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

extern VEM_USER_CONFIG_INFO g_user_config_info;
extern _T_PRESET_SETTING	g_preset_setting[1000];
extern int					g_total_preset_setting;


#endif  // __STACK_H__ 
#endif	// _xappany_H_