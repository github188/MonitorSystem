#include "StdAfx.h"
#include "LoadDll.h"

DLL_PARAM g_DllParm;

HMODULE g_Dllhandel = 0;

CLoadDll::CLoadDll(void)
{

}

CLoadDll::~CLoadDll(void)
{

}

bool InitDll()
{
	char FileName[256] = "prj_MOBOTIX.dll";
	g_Dllhandel = ::LoadLibrary(FileName);
	if(!g_Dllhandel)
	{
		return false;
	}

	g_DllParm.InitClient = (MobotixSdk_InitClinet)GetProcAddress(g_Dllhandel,"MOBOTIX_InitClient");
	g_DllParm.UnIintClient = (MobotixSdk_UnInitClinet)GetProcAddress(g_Dllhandel,"MOBOTIX_UnInitClient");
	g_DllParm.OpenChannel = (MobotixSdk_OpenChannel)GetProcAddress(g_Dllhandel,"MOBOTIX_OpenChannel");
	g_DllParm.CloseChannel = (MobotixSdk_CloseChannel)GetProcAddress(g_Dllhandel,"MOBOTIX_CloseChannel");
	g_DllParm.StartVideo = (MobotixSdk_StartVideo)GetProcAddress(g_Dllhandel,"MOBOTIX_StartVideo");
	g_DllParm.StopVideo = (MobotixSdk_StopVideo)GetProcAddress(g_Dllhandel,"MOBOTIX_StopVideo");
	g_DllParm.SendCmd = (MobotixSdk_SendCmd)GetProcAddress(g_Dllhandel,"MOBOTIX_Preview_SendCmd");
	g_DllParm.PTZControl = (MobotixSdk_VPTZControl)GetProcAddress(g_Dllhandel,"MOBOTIX_VPTZControl");
	g_DllParm.SetGeneralDataCallBack2 = (MobotixSdk_SetGeneralDataCallBack2)GetProcAddress(g_Dllhandel,"MOBOTIX_SetGeneralDataCallBack2");
	g_DllParm.SetDrawCallBack = (MobotixSdk_SetDrawCallBack)GetProcAddress(g_Dllhandel,"MOBOTIX_SetDrawCallBack");
	g_DllParm.TakePicture = (MobotixSdk_TakePicture)GetProcAddress(g_Dllhandel,"MOBOTIX_TakePicture");
	g_DllParm.StartRecord = (MobotixSdk_StartRecord)GetProcAddress(g_Dllhandel,"MOBOTIX_StartRecord");
	g_DllParm.StopRecord = (MobotixSdk_StopRecord)GetProcAddress(g_Dllhandel,"MOBOTIX_StopRecord");
	return true;
}

void FreeDll()
{
	::FreeLibrary(g_Dllhandel);
}