#include "stdafx.h"
#include "ACSSystemClientCB.h"
#include "ACSSystemClient.h"
#include "ACSClientProcessMessage.h"

//tcp»Øµ÷º¯Êý
int ACS_TcpClientRecvCB(NETHANDLE handle,char* buf,int size,void* user)
{
	if ((buf == NULL) || size == 0 || strlen(buf) > size)
	{
		return 1;
	}

	ComDispatchTask(&g_AcsThreadPool,ProcessRecvMessage,buf,size,0);

	return size;
}