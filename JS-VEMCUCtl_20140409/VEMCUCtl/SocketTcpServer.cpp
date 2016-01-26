#include "StdAfx.h"
#include "SocketTcpServer.h"

CSocketTcpServer::CSocketTcpServer(void)
{
	memset(m_szLocalIp,0,sizeof(m_szLocalIp));
	m_nLocalPort = 0;
	m_nState = 0;
	m_hThread = NULL;
	m_uThreadID = 0;
	m_bThreadExitFlag = FALSE;
	m_ListenSocket = INVALID_SOCKET;
	m_TcpServerCallback = NULL;
	m_pUserInfo = NULL;
}

CSocketTcpServer::~CSocketTcpServer(void)
{
	UninitTcpServer();

	memset(m_szLocalIp,0,sizeof(m_szLocalIp));
	m_nLocalPort = 0;
	m_nState = 0;
	m_hThread = NULL;
	m_uThreadID = 0;
	m_bThreadExitFlag = FALSE;
}

BOOL CSocketTcpServer::SetServerConfig(char *szLocalIp,int nLocalPort)
{
	if (szLocalIp == NULL||strlen(szLocalIp) == 0)
		return FALSE;

	if (nLocalPort <= 0)
		return FALSE;

	strcpy_s(m_szLocalIp,sizeof(m_szLocalIp),szLocalIp);
	m_nLocalPort = nLocalPort;

	return TRUE;
}

BOOL CSocketTcpServer::SetServerCallback(void *TcpServerCB,void *user)
{
	if (TcpServerCB == NULL)
		return FALSE;

	m_TcpServerCallback = (TcpServerCallback)TcpServerCB;
	m_pUserInfo = user;

	return TRUE;
}

BOOL CSocketTcpServer::InitTcpServer()
{
	if (m_nState != SOCKET_TCP_SERVER_STATE_UNDEFINED)
		return FALSE;

	m_nState = SOCKET_TCP_SERVER_STATE_INIT;

	return TRUE;
}

BOOL CSocketTcpServer::StartTcpServer()
{
	if (m_nState != SOCKET_TCP_SERVER_STATE_INIT)
		return FALSE;

	if (m_nLocalPort <= 0||strlen(m_szLocalIp) == 0)
		return FALSE;

	m_nState = SOCKET_TCP_SERVER_STATE_STARTING;
	
	m_bThreadExitFlag = FALSE;

	m_hThread = (HANDLE)_beginthreadex(NULL,0,SocketTcpServerThread,(void *)this,0,&m_uThreadID);
	if (m_hThread == NULL)
		return FALSE;

	m_nState = SOCKET_TCP_SERVER_STATE_SERVER_RUN;

	return TRUE;
}

BOOL CSocketTcpServer::StopTcpServer()
{
	DWORD dwResult = 0;

	if (m_nState != SOCKET_TCP_SERVER_STATE_SERVER_RUN)
		return FALSE;

	m_nState = SOCKET_TCP_SERVER_STATE_STOPPING;

	if (m_hThread != NULL)
	{
		m_bThreadExitFlag = TRUE;
		dwResult = WaitForSingleObject(m_hThread,10*1000);
		if (dwResult != WAIT_OBJECT_0)
		{
			TerminateThread(m_hThread,1);
		}

		CloseHandle(m_hThread);
		m_hThread = NULL;
		m_uThreadID = 0;
	}

	m_nState = SOCKET_TCP_SERVER_STATE_INIT;

	return TRUE;
}

BOOL CSocketTcpServer::UninitTcpServer()
{
	StopTcpServer();

	if (m_nState != SOCKET_TCP_SERVER_STATE_INIT)
		return FALSE;

	m_nState = SOCKET_TCP_SERVER_STATE_UNDEFINED;

	return TRUE;
}

int CSocketTcpServer::GetTcpServerState()
{
	return m_nState;
}


unsigned  __stdcall SocketTcpServerThread(void *arg)
{
	if (arg == NULL)
		return -1;

	int nResult = -1;

	char Buffer[8*1024] = {0};
	int    nDataLen = 0;

	struct sockaddr_in tcp_server_addr;
	struct sockaddr_in tcp_client_addr;
	int    tcp_client_addr_len = 0;

	int i = 0;
	int nMaxIndex = -1;
	int maxfd, listenfd, connfd, sockfd;
	int nReadyCount = 0;
	int ClientSocket[FD_SETSIZE];

	int nError = -1;

	fd_set rset;
	fd_set allset;

	timeval time_out;
	time_out.tv_sec = 2;
	time_out.tv_usec = 0;

	try
	{
		CSocketTcpServer *pSocketTcpServer = (CSocketTcpServer *)arg;

		pSocketTcpServer->m_ListenSocket = ::socket(AF_INET,SOCK_STREAM,0);
		if (pSocketTcpServer->m_ListenSocket == INVALID_SOCKET)
			return -1;

		memset(&tcp_server_addr,0,sizeof(tcp_server_addr));
		tcp_server_addr.sin_family = AF_INET;
		tcp_server_addr.sin_addr.s_addr = inet_addr(pSocketTcpServer->m_szLocalIp);
		tcp_server_addr.sin_port = htons(pSocketTcpServer->m_nLocalPort);

		nResult = ::bind(pSocketTcpServer->m_ListenSocket,(struct sockaddr*)&tcp_server_addr,sizeof(tcp_server_addr));
		if (nResult == SOCKET_ERROR)
		{
			closesocket(pSocketTcpServer->m_ListenSocket);
			pSocketTcpServer->m_ListenSocket = INVALID_SOCKET;
			return -1;
		}

		nResult = ::listen(pSocketTcpServer->m_ListenSocket,5);
		if (nResult == SOCKET_ERROR)
		{
			closesocket(pSocketTcpServer->m_ListenSocket);
			pSocketTcpServer->m_ListenSocket = INVALID_SOCKET;
			return -1;
		}

		listenfd = pSocketTcpServer->m_ListenSocket;
		maxfd = listenfd;
		nMaxIndex = -1;

		for (i = 0;i < FD_SETSIZE;i++)
			ClientSocket[i] = -1;

		FD_ZERO(&allset);
		FD_SET(listenfd,&allset);

		while(1)
		{
			if (pSocketTcpServer->m_bThreadExitFlag == TRUE)
			{
				for (i = FD_SETSIZE-1;i >= 0;i--)
				{
					if (ClientSocket[i] < 0)
						continue;

					g_VMLog.WriteVmLog("SocketTcpServer---m_bThreadExitFlag--closesocket--ClientSocket[%d] = %d",i,ClientSocket[i]);

					FD_CLR(ClientSocket[i],&allset);
					closesocket(ClientSocket[i]);
					ClientSocket[i] = -1;
				}

				if (pSocketTcpServer->m_ListenSocket != INVALID_SOCKET)
				{
					FD_CLR(pSocketTcpServer->m_ListenSocket,&allset);
					closesocket(pSocketTcpServer->m_ListenSocket);
					pSocketTcpServer->m_ListenSocket = INVALID_SOCKET;
				}
				break;
			}

			rset = allset;
			nReadyCount = ::select(maxfd+1,&rset,NULL,NULL,&time_out);
			if (nReadyCount == 0)
			{
				continue;
			}
			else if (nReadyCount < 0 )
			{
				nError = WSAGetLastError();
				g_VMLog.WriteVmLog("SocketTcpServer---select的nError = %d",nError);
				break;
			}

			g_VMLog.WriteVmLog("SocketTcpServer---select的nReadyCount = %d",nReadyCount);

			if (FD_ISSET(listenfd,&rset))
			{
				tcp_client_addr_len = sizeof(tcp_client_addr);
				connfd = ::accept(listenfd,(struct sockaddr *)&tcp_client_addr,&tcp_client_addr_len);
				if (connfd == INVALID_SOCKET)
					continue;

				g_VMLog.WriteVmLog("SocketTcpServer---accept的connfd = %d",connfd);

				if (pSocketTcpServer->m_TcpServerCallback != NULL)
				{
					pSocketTcpServer->m_TcpServerCallback(connfd,Buffer,0,pSocketTcpServer->m_pUserInfo);
				}

				g_VMLog.WriteVmLog("SocketTcpServer---正查找空闲的ClientSocket");

				for(i = 0;i < FD_SETSIZE-1;i++)
				{
					if (ClientSocket[i] < 0)
					{
						ClientSocket[i] = connfd;
						g_VMLog.WriteVmLog("SocketTcpServer---查找存储connfd---ClientSocket[%d] = %d",i,ClientSocket[i]);
						break;
					}
				}

				g_VMLog.WriteVmLog("SocketTcpServer---查找空闲的ClientSocket，i = %d",i);

				if (i >= FD_SETSIZE-1)
				{
					for (i = FD_SETSIZE-2;i >= 0;i--)
					{
						if (ClientSocket[i] < 0)
							continue;

						FD_CLR(ClientSocket[i],&allset);
						FD_CLR(ClientSocket[i],&rset);
						closesocket(ClientSocket[i]);

						g_VMLog.WriteVmLog("SocketTcpServer---closesocket--ClientSocket[%d] = %d",i,ClientSocket[i]);

						if (pSocketTcpServer->m_TcpServerCallback != NULL)
						{
							pSocketTcpServer->m_TcpServerCallback(ClientSocket[i],Buffer,-1,pSocketTcpServer->m_pUserInfo);
						}

						ClientSocket[i] = -1;
					}

					maxfd = listenfd;
					i = 0;
					ClientSocket[i] = connfd;
					nMaxIndex = i;
				}

				FD_SET(connfd,&allset);

				if (connfd > maxfd)
					maxfd = connfd;

				if (i > nMaxIndex)
					nMaxIndex = i;

				if(--nReadyCount == 0)
					continue;
			}

			g_VMLog.WriteVmLog("SocketTcpServer----recv----nMaxIndex = %d,nReadyCount=%d",nMaxIndex,nReadyCount);

			for (i = 0;i <= nMaxIndex;i++)
			{
				if(ClientSocket[i] < 0)
					continue;

				sockfd = ClientSocket[i];

				if (FD_ISSET(sockfd,&rset))
				{
					memset(Buffer,0,sizeof(Buffer));
					nDataLen = ::recv(sockfd,Buffer,sizeof(Buffer),0);
					if (nDataLen == 0)//正常关闭
					{
						FD_CLR(sockfd,&allset);
						closesocket(sockfd);

						g_VMLog.WriteVmLog("SocketTcpServer-正常关闭--closesocket的sockfd = %d",sockfd);

						if (pSocketTcpServer->m_TcpServerCallback != NULL)
						{
							pSocketTcpServer->m_TcpServerCallback(sockfd,Buffer,-1,pSocketTcpServer->m_pUserInfo);
						}

						ClientSocket[i] = -1;
					}
					else if (nDataLen > 0)
					{
						g_VMLog.WriteVmLog("SocketTcpServer---sockfd:%d得到数据--nDataLen = %d",sockfd,nDataLen);
						g_VMLog.WriteVmLog(Buffer);

						if (pSocketTcpServer->m_TcpServerCallback != NULL)
						{
							pSocketTcpServer->m_TcpServerCallback(sockfd,Buffer,nDataLen,pSocketTcpServer->m_pUserInfo);
						}
					}
					else if (nDataLen < 0)//异常关闭
					{
						nError = WSAGetLastError();
						g_VMLog.WriteVmLog("SocketTcpServer-异常关闭--recv的nError = %d",nError);

						FD_CLR(sockfd,&allset);
						closesocket(sockfd);

						g_VMLog.WriteVmLog("SocketTcpServer-异常关闭--closesocket的sockfd = %d",sockfd);

						if (pSocketTcpServer->m_TcpServerCallback != NULL)
						{
							pSocketTcpServer->m_TcpServerCallback(sockfd,Buffer,-1,pSocketTcpServer->m_pUserInfo);
						}

						ClientSocket[i] = -1;
					}

					if (--nReadyCount == 0)
						break;
				}
			}
		}

		return 0;
	}
	catch(...)
	{

	}

	return -1;
}