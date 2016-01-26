#include "stdafx.h"
#include "xappany.h"
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// 全局变量和函数定义

//---------------------------------------------------------------------------

char			 g_DBServerIP[IPADDRESS_LEN] = {0}; //数据库服务器的地址
char          g_HttpServerIP[IPADDRESS_LEN] = {0};//Http服务器地址
char          g_LinkServerIP[IPADDRESS_LEN] = {0};//联动服务器地址
int             g_nDBServerPort = 0;                        //数据库服务器端口
int             g_nHttpServerPort = 0;                      //Http服务器端口
int             g_nLinkServerPort = 0;                       //联动服务器端口
MYSQL		*g_mySqlData = NULL;					   //mysql数据库访问
bool          app_IsUserLogin = false;
char          g_DBName[256] = {0};
char g_AcsServerIp[IPADDRESS_LEN] = {0}; //辅助系统服务器的地址
int	 g_nAcsServerPort = 0;	//辅助系统服务的端口
int  g_nShowManagerClient = 0;//是否显示管理客户端

//用户语音对讲
int		g_ua_realtalk_callid;
int		g_ua_realtalk_direction;
int		g_ua_realtalk_state;

//登陆用户权限
PUPR	g_userpower;
int     g_nUserType = 0;//1:表示admin用户,2:表示省检用户,3表示省调用户
int     g_nUserResourceShowType = 0;//1:表示按省调方式显示,2:表示按省检管理结构显示,3:表示按省调电压等级显示

//人工巡视信息
_T_VIDEOPAGEVIEWINFO g_VideoPageViewInfo;

//系统信息结构体
_T_SYSTEM_INFO	g_SystemInfo[TOTALSYSNUM];
int		g_TotalSysNum = 0;

// 当前全部报警设置设备信息列表
_T_ALARMINFOLIST g_AlarmInfoList[TOTALALARMDEVICENUM];
int		g_TotalAlarmInfoListNum = 0;

// 轮巡方案
_T_TUNECYCLE_METHODINFO	g_TuneCycle_MehtodInfo;

// 变电站上一级的节点信息
_T_STATION_NODE_INFO	g_tStationNode_Info[MAX_STATION_NODE_NUM];
int						g_nStationNode_Num = 0;

// 所有变电站的信息
_T_STATION_INFO		g_tStation_Info[MAX_STATION_NUM];
int					g_nStation_Num = 0;

// 恒易固定头
BYTE    pDSPHeader[19] = {0X00,0X00,01,0XC6,0X4D ,0X50 ,0X47, 0X34, 0x01, 0x00 ,0X19 ,0X20 ,0XA1 ,0x07 ,0x00 ,0X60 ,0x01, 0X20, 0x01};	

void	*pDecHandle;						//编解码句柄
void	*pEncodeHandle;

VEM_USER_CONFIG_INFO g_user_config_info;//用户配置信息
_T_PRESET_SETTING	g_preset_setting[1000];
int					g_total_preset_setting;
