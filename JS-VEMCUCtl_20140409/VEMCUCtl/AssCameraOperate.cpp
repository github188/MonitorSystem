#include "stdafx.h"
#include "AssCameraOperate.h"

bool AssOpenCamera(char* ip,unsigned short port, char* user, char* password,unsigned short dvrtype,unsigned short dvrid,unsigned short channel,int id ,HWND playwnd)
{
	int nRet = OpenVideo_DevSdk(ip, port, user, password, dvrtype, dvrid, channel, id, playwnd);
	if (nRet != 0)
	{
		return false;
	}
	return true;
}

bool AssToPreset(char* ip,unsigned short port, char* user, char* password,unsigned short dvrtype,unsigned short dvrid,unsigned short channel,unsigned short control,unsigned short speed)
{
	//调用转到预置位接口函数
	int nRet = DvrPtzControl_Devsdk(ip, port, user, password, dvrtype, dvrid, channel, control, speed);
	if (nRet == FALSE)
	{	
		return false;
	}
	return true;
}

bool AssCloseCamera(int nID)
{
	CloseVideo_DevSdk(nID);
	return true;
}