#include "StdAfx.h"
#include "GWPlay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
CGWPlay::CGWPlay()
{
	InitializeCriticalSection(&m_cs);
}

CGWPlay::~CGWPlay()
{
	DeleteCriticalSection(&m_cs);
}


BOOL CGWPlay::LoadGWDecode(int nCode)
{
	CString strDllPath = "";
	CString strDecDllName = "";
	HINSTANCE hInstance = NULL;
	bool bResult = false;

	strDllPath.Format(".\\AVDecode_%d",nCode);
    ::SetDllDirectory(strDllPath);

	strDecDllName.Format("AVDecode_%d.dll",nCode);

    hInstance = ::LoadLibrary(strDecDllName);
    if(hInstance != NULL)
    {
        TRACE("AVDecode_%d.dll加载成功\r\n",nCode);
    }
    else
    {
        TRACE("AVDecode_%d.dll加载失败\r\n",nCode);
		return FALSE;
    }

	//得到解码函数 
	bResult = GWGetModuleFunc(hInstance,nCode);
	if (!bResult)
		return FALSE;

	bResult = InitGWDecFuncInfo(nCode);
	if (!bResult)
		return FALSE;

	return TRUE;
}

BOOL CGWPlay::Initialize(int nCode)
{
	BOOL  bReturn = FALSE;
	bool  bResult = false;

	__try
	{
		bResult = InitGWDecFuncInfo(nCode);

		bReturn = ((bResult == true)?TRUE:FALSE);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("Initialize异常!");
	}

	return FALSE;
}

void CGWPlay::Uninitialize(int nCode)
{
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return;

		if(pDecFuncInfo->DecodeFunc.Uninitialize != NULL)
			pDecFuncInfo->DecodeFunc.Uninitialize();
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("Uninitialize异常!");
	}
}

BOOL CGWPlay::GetPluginInfo (int nCode,TPluginInfo *pInfo)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.GetPluginInfo == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.GetPluginInfo(pInfo);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("GetPluginInfo异常!");
	}
	return FALSE;
}

int CGWPlay::GetFileHeaderLen (int nCode,LPCTSTR lpszLocalFileName)
{
	int nReturn = 0;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return nReturn;

		if(pDecFuncInfo->DecodeFunc.GetFileHeaderLen == NULL)
			return nReturn;

		nReturn = pDecFuncInfo->DecodeFunc.GetFileHeaderLen(lpszLocalFileName);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{	
		TRACE("GetFileHeaderLen异常!");
	}
	return 0;
}

HANDLE CGWPlay::CreateHandle(int nCode)
{
	HANDLE hDecodeHandle = NULL;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	EnterCriticalSection(&m_cs);//防止CreateHandle在多线程同时访问时出现问题

	__try
	{
		do 
		{
			pDecFuncInfo = GetGWDecFuncInfo(nCode);
			if (pDecFuncInfo == NULL)
				break;

			if(pDecFuncInfo->DecodeFunc.CreateHandle != NULL)
				hDecodeHandle = pDecFuncInfo->DecodeFunc.CreateHandle();//CreateHandle在多线程同时访问时会出现问题

		} while (FALSE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{	
		TRACE("CreateHandle异常!");
	}

	LeaveCriticalSection(&m_cs);

	return hDecodeHandle;
}

BOOL CGWPlay::DestoryHandle(int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.DestroyHandle == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.DestroyHandle(hHandle);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("DestoryHandle异常!");
	}
	return FALSE;
}

BOOL CGWPlay::StreamOpen (int nCode,HANDLE hHandle, char *pData,int nLen, int nStreamType, long nReserved1)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
		{
			g_VMLog.WriteVmLog("GetGWDecFuncInfo失败,nCode=%d",nCode);
			return FALSE;
		}

		if(pDecFuncInfo->DecodeFunc.StreamOpen == NULL)
		{
			g_VMLog.WriteVmLog("DecodeFunc.StreamOpen为空");
			return FALSE;
		}

		g_VMLog.WriteVmLog("DecodeFunc.StreamOpen---------1,hHandle=%d,pData=0x%x,nLen=%d,nStreamType=%d,nReserved1=%d",hHandle,pData,nLen,nStreamType,nReserved1);

		bReturn = pDecFuncInfo->DecodeFunc.StreamOpen(hHandle,pData,nLen,nStreamType,nReserved1);

		g_VMLog.WriteVmLog("DecodeFunc.StreamOpen---------2,bReturn=%d,hHandle=%d,pData=0x%x,nLen=%d,nStreamType=%d,nReserved1=%d",bReturn,hHandle,pData,nLen,nStreamType,nReserved1);
		
		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("StreamOpen异常!");
	}
	return FALSE;
}

BOOL CGWPlay::Play (int nCode,HANDLE hHandle, HWND hWnd)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.Play == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.Play(hHandle,hWnd);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("Play异常!");
	}
	return FALSE;
}

BOOL CGWPlay::SetDecCallback(int nCode,HANDLE hHandle, PDecCBFun *pfDecCBFun, DWORD dwUser)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.SetDecCallBack == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.SetDecCallBack(hHandle,pfDecCBFun,dwUser);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("SetDecCallback异常!");
	}
	return FALSE;
}

BOOL CGWPlay::SetDrawCallback (int nCode,HANDLE hHandle, PDrawCBFun *pfDrawCBFun, DWORD dwUser)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.SetDrawCallBack == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.SetDrawCallBack(hHandle,pfDrawCBFun,dwUser);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("SetDrawCallback异常!");
	}
	return FALSE;
}

BOOL CGWPlay::SetSourceBufCallBack (int nCode,HANDLE hHandle, DWORD dwThreShold,PSourceBufCBFun* pfSourceBufCBFun, DWORD dwUser, void *pReserved)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.SetSourceBufCallBack == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.SetSourceBufCallBack(hHandle,dwThreShold,pfSourceBufCBFun,dwUser,pReserved);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("SetSourceBufCallBack异常!");
	}
	return FALSE;
}

HANDLE CGWPlay::SetFileIndexCallBack(int nCode,LPCTSTR lpszFileName, PFileIndexCBFun* pfFileIndexCBFun, DWORD dwUser)
{
	HANDLE hTemp = NULL;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return NULL;

		if(pDecFuncInfo->DecodeFunc.SetFileIndexCallBack != NULL)
			hTemp =  pDecFuncInfo->DecodeFunc.SetFileIndexCallBack(lpszFileName,pfFileIndexCBFun,dwUser);

		return hTemp;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("SetFileIndexCallBack异常!");
	}
	return NULL;
}

HANDLE CGWPlay::StopFileIndex(int nCode,HANDLE hIndexHandle)
{
	HANDLE hTemp = NULL;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return NULL;

		if(pDecFuncInfo->DecodeFunc.StopFileIndex != NULL)
			hTemp = pDecFuncInfo->DecodeFunc.StopFileIndex(hIndexHandle);

		return hTemp;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("StopFileIndex异常!");
	}

	return NULL;
}

BOOL CGWPlay::StreamSetPlayBuffer(int nCode,HANDLE hHandle,int nBufferLen)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.StreamSetPlayBuffer == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.StreamSetPlayBuffer(hHandle,nBufferLen);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("StreamSetPlayBuffer异常!");
	}
	return FALSE;
}

BOOL CGWPlay::StreamInputData (int nCode,HANDLE hHandle, TStreamDataParams* pStreamInData)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.StreamInputData == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.StreamInputData(hHandle,pStreamInData);
		
		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("StreamInputData exception!");
	}
	return FALSE;
}

BOOL CGWPlay::StreamResetBuffer (int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.StreamResetBuffer == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.StreamResetBuffer(hHandle);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("StreamResetBuffer异常!");
	}
	return FALSE;
}

BOOL CGWPlay::StreamRefreshPlay (int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.StreamRefreshPlay == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.StreamRefreshPlay(hHandle);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("StreamRefreshPlay异常!");
	}
	return FALSE;
}

BOOL CGWPlay::Pause (int nCode,HANDLE hHandle, BOOL bPause)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.Pause == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.Pause(hHandle,bPause);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("Pause异常!");
	}
	return FALSE;
}

BOOL CGWPlay::Fast(int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.Fast == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.Fast(hHandle);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("Fast异常!");
	}
	return FALSE;
}

BOOL CGWPlay::Slow(int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.Slow == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.Slow(hHandle);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("Slow异常!");
	}
	return FALSE;
}

BOOL CGWPlay::SoundOpen(int nCode,HANDLE hHandle , int nSoundPlayType)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.SoundOpen == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.SoundOpen(hHandle,nSoundPlayType);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("SoundOpen异常!");
	}
	return FALSE;
}

BOOL CGWPlay::SoundClose (int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.SoundClose == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.SoundClose(hHandle);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("SoundClose异常!");
	}
	return FALSE;
}

BOOL CGWPlay::SetVolume (int nCode,HANDLE hHandle, DWORD dwVolume)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.SetVolume == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.SetVolume(hHandle,dwVolume);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("SetVolume异常!");
	}
	return FALSE;
}

BOOL CGWPlay::StreamClose (int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.StreamClose == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.StreamClose(hHandle);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("StreamClose异常!");
	}
	return FALSE;
}


BOOL CGWPlay::FileOpen(int nCode,HANDLE hHandle, LPCTSTR lpszFileName)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.FileOpen == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.FileOpen(hHandle,lpszFileName);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("FileOpen异常!");
	}
	return FALSE;
}

BOOL CGWPlay::FileClose(int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.FileClose == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.FileClose(hHandle);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("fileclose异常!");
	}
	return FALSE;
}

BOOL CGWPlay::Stop(int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.Stop == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.Stop(hHandle);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("stop异常!");
	}
	return FALSE;
}

BOOL CGWPlay::PlayCapturePic (int nCode,HANDLE hHandle, int nPicType, int nPicQuality, LPCTSTR lpszPicFileName)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.PlayCapturePic == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.PlayCapturePic(hHandle,nPicType,nPicQuality,lpszPicFileName);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("PlayCapturePic异常!");
	}
	return FALSE;
}

BOOL CGWPlay::PlayRefresh (int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.PlayRefresh == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.PlayRefresh(hHandle);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("PlayRefresh异常!");
	}
	return FALSE;
}

BOOL CGWPlay::FastBack(int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.FastBack == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.FastBack(hHandle);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("FastBack异常!");
	}
	return FALSE;
}

BOOL CGWPlay::SlowBack(int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.SlowBack == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.SlowBack(hHandle);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("SlowBack异常!");
	}
	return FALSE;
}

BOOL CGWPlay::Step(int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.Step == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.Step(hHandle);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("Step异常!");
	}
	return FALSE;
}

BOOL CGWPlay::StepBack(int nCode,HANDLE hHandle)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.StepBack == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.StepBack(hHandle);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("StepBack异常!");
	}
	return FALSE;
}


BOOL CGWPlay::SetPos(int nCode,HANDLE hHandle, float fRealPos)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.SetPos == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.SetPos(hHandle,fRealPos);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("SetPos异常!");
	}
	return bReturn;
}

BOOL CGWPlay::GetPos(int nCode,HANDLE hHandle, float *pfRealPos)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.GetPos == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.GetPos(hHandle,pfRealPos);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("GetPos异常!");
	}
	return FALSE;
}

BOOL CGWPlay::GetTotalTime(int nCode,HANDLE hHandle, DWORD *pdwFileTime)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.GetTotalTime == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.GetTotalTime(hHandle,pdwFileTime);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("GetTotalTime异常!");
	}
	return FALSE;
}

BOOL CGWPlay::GetPlayedTime (int nCode,HANDLE hHandle, DWORD *pdwPlayedTime)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.GetPlayedTime == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.GetPlayedTime(hHandle,pdwPlayedTime);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("GetPlayedTime异常!");	
	}
	return FALSE;
}

BOOL CGWPlay::GetTotalFrames (int nCode,HANDLE hHandle, DWORD *pdwTotalFrames)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.GetTotalFrames == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.GetTotalFrames(hHandle,pdwTotalFrames);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("GetTotalFrames异常!");
	}
	return FALSE;
}

BOOL CGWPlay::GetFramePosition (int nCode,HANDLE hHandle, DWORD dwFrameNo, DWORD *pdwPosition)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.GetFramePosition == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.GetFramePosition(hHandle,dwFrameNo,pdwPosition);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("GetFramePosition异常!");
	}
	return FALSE;
}

BOOL CGWPlay::GetPlayedFrames (int nCode,HANDLE hHandle, DWORD *pdwPlayedFrames)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.GetPlayedFrames == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.GetPlayedFrames(hHandle,pdwPlayedFrames);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("GetPlayedFrames异常!");
	}
	return FALSE;
}

BOOL CGWPlay::SetVideoParams(int nCode,HANDLE hHandle,TVideoParams* pVideoParams, BOOL bDefault)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.SetVideoParams == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.SetVideoParams(hHandle,pVideoParams,bDefault);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("SetVideoParams异常!");
	}
	return FALSE;
}

BOOL CGWPlay::GetVideoParams(int nCode,HANDLE hHandle, TVideoParams* pVideoParams)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.GetVideoParams == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.GetVideoParams(hHandle,pVideoParams);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("GetVideoParams异常!");
	}
	return FALSE;
}

BOOL CGWPlay::SetVideoWindows (int nCode,HANDLE hHandle, HWND hWnd, RECT* pstDisplayRect,BOOL bEnable)
{
	BOOL bReturn = FALSE;
	GW_DECODE_FUNC_INFO *pDecFuncInfo = NULL;

	__try
	{
		pDecFuncInfo = GetGWDecFuncInfo(nCode);
		if (pDecFuncInfo == NULL)
			return FALSE;

		if(pDecFuncInfo->DecodeFunc.SetVideoWindows == NULL)
			return FALSE;

		bReturn = pDecFuncInfo->DecodeFunc.SetVideoWindows(hHandle,hWnd,pstDisplayRect,bEnable);

		return bReturn;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("SetVideoWindows异常!");
	}
	return FALSE;
}

