#include "StdAfx.h"
#include "DVRPtz.h"
#include "DataBaseExcute.h"

int getdevstate(unsigned short dvrid,bool state)
{
	TRACE("getdevstate dvrid:%d state:%d\n",dvrid,state);
	return 0;
}

int getpos(unsigned short dvrid,unsigned short channel,int pos)
{
	TRACE("getpos dvrid:%d channel:%d pos:%d\n",dvrid,channel,pos);
	return 0;
}

int getfilepos(unsigned short dvrid,unsigned short channel,int pos)
{
	TRACE("getfilepos dvrid:%d channel:%d pos:%d\n",dvrid,channel,pos);
	return 0;
}

int playfilepos(LONG id,int pos)
{
	TRACE("playfilepos id:%d pos:%d\n",id,pos);
	return 0;
}

int initDVRDev()
{
	if(!Init_DevSdk())
	{
		printf("\n dvrSDK初始化失败");
		return 0;
	}
	SetCallbackGetDevState(getdevstate);
	SetCallbackGetPlayBackPos(getpos);
	SetCallbackGetLocalFilePos(getfilepos);
	SetCallbackPlayLocalFilePos(playfilepos);

	return 1;
}

int DVR_Contrl(C_LINK c_link)
{
	C_CAMERAINFO *camerainfo;
	camerainfo=new C_CAMERAINFO;
	bool ret;
	int result=0;
	if (GetCameraInfo(c_link.c_code,camerainfo)>0)
	{
		if (strcmp(camerainfo->dvrtype,"Hik")==0)//海康
		{
			ret=DvrPtzControl_Devsdk(camerainfo->ip,camerainfo->port,camerainfo->username,camerainfo->password,DEVTYPE_HK,1,camerainfo->channel,GOTOPRESET,c_link.c_pset);
			if (ret==true)
			{
				result=1;
			}
		}
		else if (strcmp(camerainfo->dvrtype,"DH")==0)//大华
		{
			ret=DvrPtzControl_Devsdk(camerainfo->ip,camerainfo->port,camerainfo->username,camerainfo->password,DEVTYPE_DH,1,camerainfo->channel,GOTOPRESET,c_link.c_pset);
			if (ret==false)
			{
				result=1;
			}
		}
		else if (strcmp(camerainfo->dvrtype,"DL")==0)//大立
		{
			ret=DvrPtzControl_Devsdk(camerainfo->ip,camerainfo->port,camerainfo->username,camerainfo->password,DEVTYPE_DL,1,camerainfo->channel,GOTOPRESET,c_link.c_pset);
			if (ret==false)
			{
				result=1;
			}
		}
		else if (strcmp(camerainfo->dvrtype,"AXIS")==0)//AXIS
		{
			ret=DvrPtzControl_Devsdk(camerainfo->ip,camerainfo->port,camerainfo->username,camerainfo->password,DEVTYPE_AXIS,1,camerainfo->channel,GOTOPRESET,c_link.c_pset);
			if (ret==false)
			{
				result=1;
			}
		}
		else if (strcmp(camerainfo->dvrtype,"MOBO1")==0)//MOBO1
		{
			ret=DvrPtzControl_Devsdk(camerainfo->ip,camerainfo->port,camerainfo->username,camerainfo->password,DEVTYPE_MOBOTIX,1,camerainfo->channel,GOTOPRESET,c_link.c_pset);
			if (ret==false)
			{
				result=1;
			}
		}
		else
			result=0;

	}
	else
		result=0;

	delete camerainfo;

	return result;



}