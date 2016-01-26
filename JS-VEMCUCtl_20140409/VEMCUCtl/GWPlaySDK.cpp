#include "stdafx.h"
#include "GWPlaySDK.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
GW_DECODE_FUNC_INFO g_GW_DecodeFuncInfo[256];

bool SetGWDecFuncInfo(GW_DECODE_FUNC_INFO *pDecFuncInfo,int nCode,HMODULE hModule,IFC_FUNCTION_PTR* pIfcFuncPtr)
{
	if (pDecFuncInfo == NULL||pIfcFuncPtr == NULL)
		return false;

	__try
	{
		pDecFuncInfo->nInitFlag = true;
		pDecFuncInfo->nCode = nCode;
		pDecFuncInfo->hModule = hModule;

		memcpy(&pDecFuncInfo->DecodeFunc,pIfcFuncPtr,sizeof(IFC_FUNCTION_PTR));
		return true;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{

	}

	return false;
}


bool GWGetModuleFunc(HMODULE hModule,int nCode)
{
	if (nCode < 0||nCode >= 256)
		return false;

	IFC_FUNCTION_PTR ifc_func_ptr;

	__try
	{
		memset(&ifc_func_ptr,0,sizeof(IFC_FUNCTION_PTR));

		ifc_func_ptr.GetLastError                = (PIFC_GetLastError)GetProcAddress(hModule,"IFC_GetLastError");        
		ifc_func_ptr.Initialize                       = (PIFC_Initialize)GetProcAddress(hModule,"IFC_Initialize");
		ifc_func_ptr.Uninitialize                  = (PIFC_Uninitialize)GetProcAddress(hModule,"IFC_Uninitialize");
		ifc_func_ptr.GetPluginInfo              = (PIFC_GetPluginInfo)GetProcAddress(hModule,"IFC_GetPluginInfo");
		ifc_func_ptr.GetFileHeaderLen       = (PIFC_GetFileHeaderLen)GetProcAddress(hModule,"IFC_GetFileHeaderLen");
		ifc_func_ptr.CreateHandle              = (PIFC_CreateHandle)GetProcAddress(hModule,"IFC_CreateHandle");
		ifc_func_ptr.DestroyHandle            = (PIFC_DestroyHandle)GetProcAddress(hModule,"IFC_DestroyHandle");
		ifc_func_ptr.StreamOpen                = (PIFC_StreamOpen)GetProcAddress(hModule,"IFC_StreamOpen");
		ifc_func_ptr.Play                              = (PIFC_Play)GetProcAddress(hModule,"IFC_Play");
		ifc_func_ptr.SetDecCallBack           = (PIFC_SetDecCallBack)GetProcAddress(hModule,"IFC_SetDecCallBack");
		ifc_func_ptr.SetDrawCallBack         = (PIFC_SetDrawCallBack)GetProcAddress(hModule,"IFC_SetDrawCallBack");
		ifc_func_ptr.SetSourceBufCallBack = (PIFC_SetSourceBufCallBack)GetProcAddress(hModule,"IFC_SetSourceBufCallBack");
		ifc_func_ptr.SetFileIndexCallBack   = (PIFC_SetFileIndexCallBack)GetProcAddress(hModule,"IFC_SetFileIndexCallBack");
		ifc_func_ptr.StopFileIndex               = (PIFC_StopFileIndex)GetProcAddress(hModule,"IFC_StopFileIndex");
		ifc_func_ptr.StreamSetPlayBuffer   = (PIFC_StreamSetPlayBuffer)GetProcAddress(hModule,"IFC_StreamSetPlayBuffer");
		ifc_func_ptr.StreamInputData         = (PIFC_StreamInputData)GetProcAddress(hModule,"IFC_StreamInputData");
		ifc_func_ptr.StreamResetBuffer      = (PIFC_StreamResetBuffer)GetProcAddress(hModule,"IFC_StreamResetBuffer");
		ifc_func_ptr.StreamRefreshPlay      = (PIFC_StreamRefreshPlay)GetProcAddress(hModule,"IFC_StreamRefreshPlay");
		ifc_func_ptr.Pause                           = (PIFC_Pause)GetProcAddress(hModule,"IFC_Pause");
		ifc_func_ptr.Fast                              = (PIFC_Fast)GetProcAddress(hModule,"IFC_Fast");
		ifc_func_ptr.Slow                             = (PIFC_Slow)GetProcAddress(hModule,"IFC_Slow");
		ifc_func_ptr.SoundOpen                 = (PIFC_SoundOpen)GetProcAddress(hModule,"IFC_SoundOpen");
		ifc_func_ptr.SoundClose                 = (PIFC_SoundClose)GetProcAddress(hModule,"IFC_SoundClose");
		ifc_func_ptr.SetVolume                  = (PIFC_SetVolume)GetProcAddress(hModule,"IFC_SetVolume");
		ifc_func_ptr.StreamClose               = (PIFC_StreamClose)GetProcAddress(hModule,"IFC_StreamClose");
		ifc_func_ptr.FileOpen                     = (PIFC_FileOpen)GetProcAddress(hModule,"IFC_FileOpen");
		ifc_func_ptr.FileClose                     = (PIFC_FileClose)GetProcAddress(hModule,"IFC_FileClose");
		ifc_func_ptr.Stop                            = (PIFC_Stop)GetProcAddress(hModule,"IFC_Stop");
		ifc_func_ptr.PlayCapturePic          = (PIFC_PlayCapturePic)GetProcAddress(hModule,"IFC_PlayCapturePic");
		ifc_func_ptr.PlayRefresh                = (PIFC_PlayRefresh)GetProcAddress(hModule,"IFC_PlayRefresh");
		ifc_func_ptr.FastBack                     = (PIFC_FastBack)GetProcAddress(hModule,"IFC_FastBack");
		ifc_func_ptr.SlowBack                    = (PIFC_SlowBack)GetProcAddress(hModule,"IFC_SlowBack");
		ifc_func_ptr.Step                            = (PIFC_Step)GetProcAddress(hModule,"IFC_Step");
		ifc_func_ptr.StepBack                    = (PIFC_StepBack)GetProcAddress(hModule,"IFC_StepBack");
		ifc_func_ptr.SetPos                        = (PIFC_SetPos)GetProcAddress(hModule,"IFC_SetPos");
		ifc_func_ptr.GetPos                       = (PIFC_GetPos)GetProcAddress(hModule,"IFC_GetPos");
		ifc_func_ptr.GetTotalTime            = (PIFC_GetTotalTime)GetProcAddress(hModule,"IFC_GetTotalTime");
		ifc_func_ptr.GetPlayedTime         = (PIFC_GetPlayedTime)GetProcAddress(hModule,"IFC_GetPlayedTime");
		ifc_func_ptr.GetTotalFrames        = (PIFC_GetTotalFrames)GetProcAddress(hModule,"IFC_GetTotalFrames");
		ifc_func_ptr.GetFramePosition     = (PIFC_GetFramePosition)GetProcAddress(hModule,"IFC_GetFramePosition");
		ifc_func_ptr.GetPlayedFrames      = (PIFC_GetPlayedFrames)GetProcAddress(hModule,"IFC_GetPlayedFrames");
		ifc_func_ptr.SetVideoParams        = (PIFC_SetVideoParams)GetProcAddress(hModule,"IFC_SetVideoParams");
		ifc_func_ptr.GetVideoParams       = (PIFC_GetVideoParams)GetProcAddress(hModule,"IFC_GetVideoParams");
		ifc_func_ptr.SetVideoWindows     = (PIFC_SetVideoWindows)GetProcAddress(hModule,"IFC_SetVideoWindows");
		
		//////////////////////////////////////////////////////////////////////////
		if (ifc_func_ptr.DestroyHandle == NULL)
			ifc_func_ptr.DestroyHandle = (PIFC_DestroyHandle)GetProcAddress(hModule,"IFC_DestoryHandle");
		
		if (ifc_func_ptr.SetDecCallBack  == NULL)
			ifc_func_ptr.SetDecCallBack = (PIFC_SetDecCallBack)GetProcAddress(hModule,"IFC_SetDecCallback");

		if (ifc_func_ptr.SetDrawCallBack == NULL)
			ifc_func_ptr.SetDrawCallBack = (PIFC_SetDrawCallBack)GetProcAddress(hModule,"IFC_SetDrawCallback");
		//////////////////////////////////////////////////////////////////////////

		return SetGWDecFuncInfo(&g_GW_DecodeFuncInfo[nCode],nCode,hModule,&ifc_func_ptr);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{

	}
	return false;
}

//初始化解码
bool InitGWDecFuncInfo(int nCode)
{
	if (nCode < 0||nCode >= 256)
		return false;

	BOOL bResult = FALSE;
	int nResult = 0;

	__try
	{
		if (g_GW_DecodeFuncInfo[nCode].nInitFlag == 0
			||g_GW_DecodeFuncInfo[nCode].nCode != nCode)
			return false;

		if (g_GW_DecodeFuncInfo[nCode].DecodeFunc.Initialize == NULL)
			return false;

		bResult = g_GW_DecodeFuncInfo[nCode].DecodeFunc.Initialize();
		if (bResult == FALSE)
		{
			if (g_GW_DecodeFuncInfo[nCode].DecodeFunc.GetLastError != NULL)
			{
				nResult = g_GW_DecodeFuncInfo[nCode].DecodeFunc.GetLastError(0);
				TRACE("nCode = %d, Error = %d\n",nCode,nResult);
			}	
			return false;
		}

		return true;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{

	}

	return false;
}

//得到解码信息
GW_DECODE_FUNC_INFO *GetGWDecFuncInfo(int nCode)
{
	if (nCode < 0||nCode >= 256)
		return NULL;

	__try
	{
		if (g_GW_DecodeFuncInfo[nCode].nInitFlag == 0
			||g_GW_DecodeFuncInfo[nCode].nCode != nCode
			||g_GW_DecodeFuncInfo[nCode].hModule == NULL)
			return NULL;

		return &g_GW_DecodeFuncInfo[nCode];
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{

	}

	return NULL;
}

BOOL IsExsitGWDecInfo(int nCode)
{
	if (nCode < 0||nCode >= 256)
		return FALSE;

	__try
	{
		if (g_GW_DecodeFuncInfo[nCode].nInitFlag != 0
			&&g_GW_DecodeFuncInfo[nCode].nCode != nCode
			&&g_GW_DecodeFuncInfo[nCode].hModule != NULL)
			return TRUE;

		return FALSE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{

	}
	return FALSE;
}

