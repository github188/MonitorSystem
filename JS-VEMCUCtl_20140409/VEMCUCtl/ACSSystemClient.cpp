#include "StdAfx.h"
#include "ACSSystemClient.h"
#include "ACSSystemClientCB.h"
#include "ACSClientProcessMessage.h"

/*
线程号-作用
0-给客户端回调函数使用
1-给心跳线程
*/
COMMONTHREADPOOL g_AcsThreadPool;//辅助系统的线程池指针

COMMONFIFO g_AcsCommonFifo;//接收消息的队列

CACSSystemClient::CACSSystemClient(void)
{
	m_bConnectFlag = FALSE;
	m_enumLoginStatus = LOGOUT;
}

CACSSystemClient::~CACSSystemClient(void)
{
}

/*
	初始化
		1.获取本地ip
		2.初始化客户端使用的线程池
*/
BOOL CACSSystemClient::Init()
{
	//获取本地IP
	GetLocalIp(m_szLocalIp);
	m_nLocalPort = 0;

	//初始化线程池
	int result = ComInitThreadPool(&g_AcsThreadPool,SINGLEFIFOTHREADPOOL|COPYARG,-1,THREADPOOLTHREADNUM,100);
	if (result)
		return FALSE;

	return TRUE;
}

//连接服务器
BOOL CACSSystemClient::ConnectServer(char *szRoutIp, int nRoutePort)
{
	if (szRoutIp != NULL)
		memcpy(m_szRouteIp, szRoutIp, 32);

	m_nRoutePort = nRoutePort;

	m_hClientTcpHandle =  ComOpenNet(m_szRouteIp, m_nLocalPort, m_szRouteIp, m_nRoutePort,NETCLIENT,TCP,ACS_RECVBUFSIZE,ACS_SOCKETRECVBUFSIZE,ACS_SOCKETSENDBUFSIZE,10,-1,10,ACS_TcpClientRecvCB,NULL,0,0);
	//m_hClientTcpHandle =  ComOpenNet(m_szLocalIp, m_nLocalPort, m_szRouteIp, m_nRoutePort,NETCLIENT,TCP,ACS_RECVBUFSIZE,ACS_SOCKETRECVBUFSIZE,ACS_SOCKETSENDBUFSIZE,10,-1,10,ACS_TcpClientRecvCB,NULL,0,0);

	if (!m_hClientTcpHandle)
	{
		m_bConnectFlag = FALSE;
		return FALSE;
	}

	m_bConnectFlag = TRUE;

	//连接成功后,开启一个线程处理服务端发过来的消息
	//ComDispatchTask(&g_AcsThreadPool,ProcessRecvMessage,NULL,0,0);

	return TRUE;
}

void CACSSystemClient::GetLocalIp( char *szLocalIp )
{
	char szHostName[MAX_PATH + 1];
	gethostname(szHostName, MAX_PATH);//获得主机名
	hostent *hn;
	hn = gethostbyname(szHostName);//根据本机主机名得到本南ip
	memcpy(szLocalIp, inet_ntoa(*(in_addr*)hn->h_addr_list[0]), 32);//把ip转换成字符串形式
}

char * CACSSystemClient::GetLocalIp()
{
	return m_szLocalIp;
}

void CACSSystemClient::SetLocalIp( char *szLocalIp )
{
	if (szLocalIp != NULL)
		sprintf_s(m_szLocalIp, szLocalIp, 32);
}

int CACSSystemClient::GetLocalPort()
{
	return m_nLocalPort;
}

void CACSSystemClient::SetLocalPort(int nLocalPort)
{
	m_nLocalPort = nLocalPort;
}

char * CACSSystemClient::GetRouteIp()
{
	return m_szRouteIp;
}

void CACSSystemClient::SetRouteIp( char *szRouteIp )
{
	if (szRouteIp != NULL)
		sprintf_s(m_szRouteIp, szRouteIp, 32);
}

int CACSSystemClient::GetRoutePort()
{
	return m_nRoutePort;
}

void CACSSystemClient::SetRoutePort(int nRoutePort)
{
	m_nRoutePort = nRoutePort;
}

//登陆服务器
BOOL CACSSystemClient::SendLoginServerRequest(char *username,char *password,int nType)
{
	if (username == NULL||password == NULL||nType != 0)
		return FALSE;

	char send_buf[1024] ={0};

	//拼接登陆命令
	sprintf_s(send_buf,"<?xml version=1.0 encoding=GB2312?> <HTTP_XML EventType=Request_Login>"
		"<Item UserName='%s' Password='%s' Type='%d' /> </HTTP_XML>",username, password, nType);

	//发送登陆命令
	int ret = ComSendNet(m_hClientTcpHandle,0,0,0,0,0,0,send_buf,sizeof(send_buf),0);
	if (ret)
		return FALSE;

	DEBUG_LOG("send 给服务端发送登陆服务器命令数据：%s", send_buf);
}

BOOL CACSSystemClient::SendSubscribeAlarmRequest(int nUserId, int nStationId)
{
	char send_buf[1024] ={0};

	//拼接告警订阅命令
	sprintf_s(send_buf,"<?xml version=1.0 encoding=GB2312?> <HTTP_XML EventType=Request_Subscribe_Alarm>"
		"<Item UserID='%d' StationID='%d' Type='%d' /></HTTP_XML>",nUserId, nStationId, 1);

	//发送登陆命令
	int ret = ComSendNet(m_hClientTcpHandle,0,0,0,0,0,0,send_buf,sizeof(send_buf),0);
	if (ret)
		return FALSE;

	DEBUG_LOG("send 给服务端发送告警订阅命令数据：%s", send_buf);
}

BOOL CACSSystemClient::SendCancelSubscribeAlarmRequest(int nUserId, int nStationId)
{
	char send_buf[1024] ={0};

	//拼接告警订阅命令
	sprintf_s(send_buf,"<?xml version=1.0 encoding=GB2312?> <HTTP_XML EventType=Request_Subscribe_Alarm>"
		"<Item UserID='%d' StationID='%d' Type='%d' /></HTTP_XML>",nUserId, nStationId, 0);

	//发送登陆命令
	int ret = ComSendNet(m_hClientTcpHandle,0,0,0,0,0,0,send_buf,sizeof(send_buf),0);
	if (ret)
		return FALSE;

	DEBUG_LOG("send 给服务端发送取消告警订阅命令数据：%s", send_buf);
}

//通知服务屏蔽某个设备
BOOL CACSSystemClient::SendHideDevRequest(int nUserId,int nDevId,int nAction)
{
	if (m_enumLoginStatus != LOGIN)
		return FALSE;

	if (nDevId <= 0 || nUserId <= 0)
		return FALSE;

	char send_buf[1024] ={0};

	//拼接设备屏蔽命令
	sprintf_s(send_buf,"<?xml version=1.0 encoding=GB2312?> <HTTP_XML EventType=Request_Hide_Dev>"
		"<Item UserID='%d' DevID='%d' Hide='%d' /> </HTTP_XML>",nUserId, nDevId, nAction);

	//发送设备屏蔽命令
	int ret = ComSendNet(m_hClientTcpHandle,0,0,0,0,0,0,send_buf,sizeof(send_buf),0);
	if (ret)
		return FALSE;

	DEBUG_LOG("send 给服务端发送屏蔽命令数据：%s", send_buf);

}