/*
	辅助系统客户端类
*/
#pragma once

/*
线程号-作用
0-给客户端回调函数使用
1-心跳线程
*/
#define THREADPOOLTHREADNUM 10  //线程池中的线程数量

//客户端连接状态
enum LOGIN_STATUS
{
	LOGIN=0,//登陆
	LOGOUT=1,//退出
	LOGGING=2//正在登陆
};

class CACSSystemClient
{
public:
	CACSSystemClient(void);
	~CACSSystemClient(void);
public:
	int m_hClientTcpHandle;//客户端句柄
	int m_nConnectServer;//是否登陆上了服务器，0表示已登陆，1表示正在登陆，-1表示退出
	BOOL m_bConnectFlag;//是否连接到服务器
	LOGIN_STATUS m_enumLoginStatus;//登陆状态

private:
	char m_szLocalIp[32];
	int m_nLocalPort;
	char m_szRouteIp[32];
	int m_nRoutePort;

public:
	//获取和设置本地IP地址
	char *GetLocalIp();
	void SetLocalIp(char *szLocalIp);
	//获取和设置本地端口
	int GetLocalPort();
	void SetLocalPort(int nLocalPort);
	//获取和设置远程IP地址
	char *GetRouteIp();
	void SetRouteIp(char *szRouteIp);	
	//获取和设置远程端口
	int GetRoutePort();
	void SetRoutePort(int nRoutePort);
private:
	//获取本地IP地址
	void GetLocalIp(char *szLocalIp);
public:
	//客户端初始化
	BOOL Init();
	//连接tcpserver
	BOOL ConnectServer(char *szRoutIp, int nRoutePort);
	//登陆服务器请求
	BOOL SendLoginServerRequest(char *username,char *password,int nType);
	//发送设备屏蔽命令
	BOOL SendHideDevRequest(int nUserId,int nDevId,int nAction);
	//发送告警订阅命令
	BOOL SendSubscribeAlarmRequest(int nUserId, int nStationId);
	BOOL SendCancelSubscribeAlarmRequest(int nUserId, int nStationId);
};

extern COMMONTHREADPOOL g_AcsThreadPool;

