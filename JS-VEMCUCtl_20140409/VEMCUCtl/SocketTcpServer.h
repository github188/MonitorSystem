#pragma once

#include "stdafx.h"

#define  SOCKET_TCP_SERVER_STATE_UNDEFINED 0
#define  SOCKET_TCP_SERVER_STATE_INIT              1
#define  SOCKET_TCP_SERVER_STATE_STARTING    2
#define  SOCKET_TCP_SERVER_STATE_SERVER_RUN  3
#define  SOCKET_TCP_SERVER_STATE_STOPPING   4

typedef int (*TcpServerCallback)(SOCKET clientsocket,char *Data,int nDataLen,void *user);

class CSocketTcpServer
{
public:
	CSocketTcpServer(void);
	~CSocketTcpServer(void);

public:
	BOOL SetServerConfig(char *szLocalIp,int nLocalPort);
	BOOL SetServerCallback(void *TcpServerCB,void *user);
	BOOL InitTcpServer();
	BOOL StartTcpServer();
	BOOL StopTcpServer();
	BOOL UninitTcpServer();
	int      GetTcpServerState();

public:
	char m_szLocalIp[64];
	int    m_nLocalPort;
	int    m_nState;
	HANDLE m_hThread;
	UINT m_uThreadID;
	BOOL m_bThreadExitFlag;
	SOCKET m_ListenSocket;

	TcpServerCallback m_TcpServerCallback;
	void * m_pUserInfo;
};

//////////////////////////////////////////////////////////////////////////
unsigned  __stdcall SocketTcpServerThread(void *arg);
