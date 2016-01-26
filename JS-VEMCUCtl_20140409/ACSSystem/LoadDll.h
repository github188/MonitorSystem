#pragma once

class CLoadDll
{
public:
	CLoadDll(void);
	~CLoadDll(void);
};

typedef BOOL (_stdcall *MobotixSdk_InitClinet)(long lParentHandle);
typedef BOOL (_stdcall *MobotixSdk_UnInitClinet)();
typedef long (_stdcall *MobotixSdk_OpenChannel)(const char* pszURL,HardDecodeMode hdmHardDecodeMode);
typedef BOOL (_stdcall *MobotixSdk_CloseChannel)(long lChannelHandle);
typedef BOOL (_stdcall *MobotixSdk_StartVideo)(long lChannelHandle,long lDisplayHandle);
typedef BOOL (_stdcall *MobotixSdk_StopVideo)(long lChannelHandle);
typedef BOOL (_stdcall *MobotixSdk_SendCmd)(long lChannelHandle,long lCmdID,long lParam1,long lParam2,long lParam3);
typedef void (_stdcall *MobotixSdk_SetGeneralDataCallBack2)(long lChannelHandle,GeneralDataCallBack pGeneralDataCallBack,DWORD dwUser);
typedef BOOL (_stdcall *MobotixSdk_VPTZControl)(VPTZCommandTargetType ttType,long lChannelHandle,long lCommand,long lParam1,long lParam2);
typedef BOOL (_stdcall *MobotixSdk_SetDrawCallBack)(long lChannelHandle,DrawCallBack pDrawCallBack,DWORD dwUser);
typedef BOOL (_stdcall *MobotixSdk_TakePicture)(long lChannelHandle,char *pszFileName);
typedef BOOL (_stdcall *MobotixSdk_StartRecord)(long lChannelHandle,const char* pszFileName);
typedef BOOL (_stdcall *MobotixSdk_StopRecord)(long lChannelHandle);

typedef struct _dll_param
{
	BOOL (_stdcall *InitClient)(long lParentHandle);
	BOOL (_stdcall *UnIintClient)();
	long (_stdcall *OpenChannel)(const char* pszURL,HardDecodeMode hdmHardDecodeMode);
	BOOL (_stdcall *CloseChannel)(long lChannelHandle);
	BOOL (_stdcall *StartVideo)(long lChannelHandle,long lDisplayHandle);
	BOOL (_stdcall *StopVideo)(long lChannelHandle);
	BOOL (_stdcall *SendCmd)(long lChannelHandle,long lCmdID,long lParam1,long lParam2,long lParam3);
	void (_stdcall *SetGeneralDataCallBack2)(long lChannelHandle,GeneralDataCallBack pGeneralDataCallBack,DWORD dwUser);
	BOOL (_stdcall *PTZControl)(VPTZCommandTargetType ttType,long lChannelHandle,long lCommand,long lParam1,long lParam2);
	BOOL (_stdcall *SetDrawCallBack)(long lChannelHandle,DrawCallBack pDrawCallBack,DWORD dwUser);
	BOOL (_stdcall *TakePicture)(long lChannelHandle,char *pszFileName);
	BOOL (_stdcall *StartRecord)(long lChannelHandle,const char* pszFileName);
	BOOL (_stdcall *StopRecord)(long lChannelHandle);
}DLL_PARAM;

bool InitDll();
void FreeDll();

extern DLL_PARAM g_DllParm;