#pragma once


//接收--http--xml--消息端口
#define  TCP_LOCAL_SERVER_PORT 82
#define  TCP_REMOTE_SERVER_PORT 82

//CommonLib--网络连接数
#define DEFAULT_SERVER_NUM 4
#define DEFAULT_CLIENT_NUM 64
#define DEFAULT_SERVER_CLIENT_NUM 8

//CommonLib--网络缓冲区大小
#define DEFAULT_TCP_SERVER_RECV_BUF_SIZE (8*1024)
#define DEFAULT_TCP_SERVER_SOCKET_RECV_BUF_SIZE (64*1024)
#define DEFAULT_TCP_SERVER_SOCKET_SEND_BUF_SIZE (64*1024)

#define DEFAULT_TCP_CLIENT_RECV_BUF_SIZE (8*1024)
#define DEFAULT_TCP_CLIENT_SOCKET_RECV_BUF_SIZE (64*1024)
#define DEFAULT_TCP_CLIENT_SOCKET_SEND_BUF_SIZE (64*1024)

//////////////////////////////////////////////////////////////////////////
typedef int (*TcpInfoCB)(char *buf,int nLen,void *user);

class CTcpServerClient
{
public:
	CTcpServerClient();
	virtual ~CTcpServerClient();

public:
	BOOL InitNetTcp();
	BOOL StartNetTcpServer(int nFlag=0);
	BOOL StopNetTcpServer(int nFlag=0);
	BOOL StartNetTcpClient(int nFlag=0);
	BOOL StopNetTcpClient(int bFlag=0);
	BOOL UnInitNetTcp();
	BOOL SetLocalIp(char *pLocalIp);
	BOOL SetRemoteIp(char *pRemoteIp);
	BOOL SetLocalServerPort(int nLocalServerPort);
	BOOL SetRemotePort(int nRemotePort);
	BOOL SetInfoCB(void *pFunc,void *user);
	int      TcpClientSendData(char *pData,int nLength,int nFlag);

public:
	int m_nTcpServerPort;
	char m_szLocalIp[32];
	char m_szRemoteIp[32];
	int  m_nRemotePort;
	NETHANDLE m_hTcpServerHandle;
	NETHANDLE m_hTcpClientHandle;
	TcpInfoCB m_pTcpInfoCB;
	void *m_user;

public:
	SOCKET m_ClientSocket;
	char *m_pBuffer;
	int  m_nBufferLen;
	int  m_nDataLen;
	time_t m_DataTime;
};
