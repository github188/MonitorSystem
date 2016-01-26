#include "stdafx.h"
#include "TCPServerClient.h"
#include "SocketTcpServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
CSocketTcpServer g_SocketTcpServer;

//////////////////////////////////////////////////////////////////////////
char HttpResponseInfo[1024] = "HTTP/1.1 200 OK\r\n" \
                                                      "Host: LocalHost\r\n" \
                                                      "Connect: Keep-Alive\r\n" \
                                                      "Content-Length: 0\r\n\r\n";

//////////////////////////////////////////////////////////////////////////
int TcpServer_info(SOCKET clientsocket,char *Data,int nDataLen,void *user)
{
	if (user == NULL)
		return -1;

	CTcpServerClient *pTcpServerClient = (CTcpServerClient*)user;
	PHTTPMSG  pHttpMsg = NULL;
	int realsize = 0;

	try
	{
		if (nDataLen == 0)
		{
			TRACE("clientsocket = %d,建立连接!\n",clientsocket);
			if (pTcpServerClient->m_ClientSocket != clientsocket)
			{
				pTcpServerClient->m_ClientSocket = clientsocket;
				pTcpServerClient->m_DataTime = time(NULL);
				pTcpServerClient->m_nDataLen = 0;
			}
		}
		else if (nDataLen < 0)
		{
			TRACE("clientsocket = %d,断开连接!\n",clientsocket);
			if (pTcpServerClient->m_ClientSocket == clientsocket)
			{
				pTcpServerClient->m_ClientSocket = INVALID_SOCKET;
				pTcpServerClient->m_DataTime = time(NULL);
				pTcpServerClient->m_nDataLen = 0;
			}
		}
		else if (nDataLen > 0)
		{
			TRACE("clientsocket = %d,接到数据\n",clientsocket);
			TRACE("--------------------------------------------------\n");
			TRACE("%s\n",Data);
			TRACE("--------------------------------------------------\n");

			time_t cur_time = time(NULL);

			if (pTcpServerClient->m_ClientSocket == clientsocket 
				&&pTcpServerClient->m_DataTime+30 > cur_time)
			{
				pTcpServerClient->m_DataTime = cur_time;
				if (pTcpServerClient->m_nDataLen+nDataLen < pTcpServerClient->m_nBufferLen)
				{
					memcpy(pTcpServerClient->m_pBuffer+pTcpServerClient->m_nDataLen,Data,nDataLen);
					pTcpServerClient->m_nDataLen += nDataLen;

					pHttpMsg = new HTTPMSG;
					if (pHttpMsg == NULL)
						return 0;

					memset(pHttpMsg,0,sizeof(HTTPMSG));

					realsize = ComHttpMsgParse(pTcpServerClient->m_pBuffer,pTcpServerClient->m_nDataLen,pHttpMsg);
					if (realsize > 0)
					{
						::send(clientsocket,HttpResponseInfo,strlen(HttpResponseInfo),0);

						if (pTcpServerClient->m_pTcpInfoCB != NULL)
						{
							pTcpServerClient->m_pTcpInfoCB(pHttpMsg->body,pHttpMsg->bodysize,pTcpServerClient->m_user);
						}

						if (pHttpMsg != NULL)
						{
							delete pHttpMsg;
							pHttpMsg = NULL;
						}

						return 0;
					}
				}
			}
		}
	}
	catch(...)
	{

	}

	if (pHttpMsg != NULL)
	{
		delete pHttpMsg;
		pHttpMsg = NULL;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
int tcp_server_cb(NETHANDLE handle,char* buf,int len,void* user)
{
	int realsize = 0;

	if (user == NULL)
		return len;

	CTcpServerClient * pTcpServerClient = NULL;
	pTcpServerClient = (CTcpServerClient *)user;
	PHTTPMSG  pHttpMsg = NULL;

	try
	{
		if(len==0)
		{
			TRACE("new comer for index file\n");
		}
		else
		{
			pHttpMsg = new HTTPMSG;
			if (pHttpMsg == NULL)
				return len;

			memset(pHttpMsg,0,sizeof(HTTPMSG));

			realsize = ComHttpMsgParse(buf,len,pHttpMsg);
			if (realsize > 0)
			{
				ComSendNet(handle,NULL,0,0,0,0,0,HttpResponseInfo,strlen(HttpResponseInfo),1);

				if (pTcpServerClient->m_pTcpInfoCB != NULL)
				{
					pTcpServerClient->m_pTcpInfoCB(pHttpMsg->body,pHttpMsg->bodysize,pTcpServerClient->m_user);
				}

				if (pHttpMsg != NULL)
				{
					delete pHttpMsg;
					pHttpMsg = NULL;
				}

				return realsize;
			}
		}
	}
	catch(...)
	{

	}

	if (pHttpMsg != NULL)
	{
		delete pHttpMsg;
		pHttpMsg = NULL;
	}

	return len;
}


//////////////////////////////////////////////////////////////////////////
int tcp_client_cb(NETHANDLE handle,char* buf,int len,void* user)
{
	int realsize = 0;

	if (user == NULL)
		return len;

	CTcpServerClient * pTcpServerClient = NULL;
	pTcpServerClient = (CTcpServerClient *)user;

	try
	{

		if(len==0)
		{
			TRACE("new comer for index file\n");
		}
		else
		{
			ComCloseNet(handle,1);
			pTcpServerClient->m_hTcpClientHandle = -1;
		}
	}
	catch(...)
	{

	}

	return len;
}

CTcpServerClient::CTcpServerClient()
{
	m_nTcpServerPort = TCP_LOCAL_SERVER_PORT;
	m_hTcpServerHandle = -1;
	m_hTcpClientHandle = -1;
	memset(m_szLocalIp,0,sizeof(m_szLocalIp));
	memset(m_szRemoteIp,0,sizeof(m_szRemoteIp));
	m_nRemotePort = TCP_REMOTE_SERVER_PORT;

	//////////////////////////////////////////////////////////////////////////
	m_ClientSocket = INVALID_SOCKET;
	m_pBuffer = NULL;
	m_nBufferLen = 0;
	m_nDataLen = 0;
	m_DataTime = time(NULL);

	try
	{
		m_nBufferLen = 16*1024;
		m_pBuffer = new char[m_nBufferLen];
		if (m_pBuffer != NULL)
			memset(m_pBuffer,0,m_nBufferLen);
	}
	catch(...)
	{

	}
}

CTcpServerClient::~CTcpServerClient()
{
	if (m_pBuffer != NULL)
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}

	m_nBufferLen = 0;
	m_ClientSocket = INVALID_SOCKET;
}

BOOL CTcpServerClient::InitNetTcp()
{
	if(0 > ComInitNet(-1,NET_IOCP,DEFAULT_SERVER_NUM,DEFAULT_CLIENT_NUM))
		return FALSE;

	g_SocketTcpServer.SetServerConfig(m_szLocalIp,m_nTcpServerPort);
	g_SocketTcpServer.SetServerCallback(TcpServer_info,(void *)this);
	g_SocketTcpServer.InitTcpServer();

	return TRUE;
}

BOOL CTcpServerClient::StartNetTcpServer(int nFlag)
{
	//m_hTcpServerHandle = ComOpenNet(
	//	m_szLocalIp,
	//	m_nTcpServerPort,
	//	NULL,
	//	0,
	//	NETSERVER,
	//	TCP,
	//	DEFAULT_TCP_SERVER_RECV_BUF_SIZE,
	//	DEFAULT_TCP_SERVER_SOCKET_RECV_BUF_SIZE,
	//	DEFAULT_TCP_SERVER_SOCKET_SEND_BUF_SIZE,
	//	0,
	//	-1,
	//	DEFAULT_SERVER_CLIENT_NUM,
	//	tcp_server_cb,
	//	(void *)this,
	//	10,
	//	nFlag);

	//return ((m_hTcpServerHandle > 0)?TRUE:FALSE);

	return g_SocketTcpServer.StartTcpServer();
}

BOOL CTcpServerClient::StopNetTcpServer(int nFlag)
{
	//if(m_hTcpServerHandle >= 0)
	//{
	//	ComCloseNet(m_hTcpServerHandle,nFlag);
	//	m_hTcpServerHandle = -1;
	//}

	//return TRUE;

	return g_SocketTcpServer.StopTcpServer();
}

BOOL CTcpServerClient::StartNetTcpClient(int nFlag)
{
	m_hTcpClientHandle = ComOpenNet(
		m_szLocalIp,
		0,
		m_szRemoteIp,
		m_nRemotePort,
		NETCLIENT,
		TCP,
		DEFAULT_TCP_CLIENT_RECV_BUF_SIZE,
		DEFAULT_TCP_CLIENT_SOCKET_RECV_BUF_SIZE,
		DEFAULT_TCP_CLIENT_SOCKET_SEND_BUF_SIZE,
		0,
		-1,
		0,
		tcp_client_cb,
		(void *)this,
		10,
		nFlag);

	return TRUE;
}

BOOL CTcpServerClient::StopNetTcpClient(int nFlag)
{
	if (m_hTcpClientHandle >= 0)
	{
		ComCloseNet(m_hTcpClientHandle,nFlag);
		m_hTcpClientHandle = -1;
	}

	return TRUE;
}

BOOL CTcpServerClient::UnInitNetTcp()
{
	ComFreeNet();
	return TRUE;
}

BOOL CTcpServerClient::SetLocalIp(char *pLocalIp)
{
	if (pLocalIp == NULL||strlen(pLocalIp) < 7)
		return FALSE;

	strcpy_s(m_szLocalIp,sizeof(m_szLocalIp),pLocalIp);

	return TRUE;
}

BOOL CTcpServerClient::SetRemoteIp(char *pRemoteIp)
{
	if (pRemoteIp == NULL||strlen(pRemoteIp) < 7)
		return FALSE;

	strcpy_s(m_szRemoteIp,sizeof(m_szRemoteIp),pRemoteIp);

	return TRUE;
}

BOOL CTcpServerClient::SetLocalServerPort(int nLocalServerPort)
{
	if (nLocalServerPort <= 0||nLocalServerPort > 65535)
		return FALSE;

	m_nTcpServerPort = nLocalServerPort;

	return TRUE;
}

BOOL CTcpServerClient::SetRemotePort(int nRemotePort)
{
	if (nRemotePort <= 0||nRemotePort > 65535)
		return FALSE;

	m_nRemotePort = nRemotePort;

	return TRUE;
}

BOOL CTcpServerClient::SetInfoCB(void *pFunc,void *user)
{
	if (pFunc == NULL)
		return FALSE;

	m_pTcpInfoCB = (TcpInfoCB)pFunc;
	m_user = user;

	return TRUE;
}

int CTcpServerClient::TcpClientSendData(char *pData,int nLength,int nFlag)
{
	if (m_hTcpClientHandle >= 0)
	{
		ComSendNet(m_hTcpClientHandle,NULL,0,0,0,0,0,pData,nLength,nFlag);
	}
	return 0;
}