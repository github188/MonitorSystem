#include "stdafx.h"
#include "GWPlayInfoDef.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


GW_PLAY_CHANNEL_INFO::GW_PLAY_CHANNEL_INFO()
{
	InitializeCriticalSection(&m_cs);
	memset(m_szDeviceCode,0,sizeof(m_szDeviceCode));
	memset(m_szDeviceName,0,sizeof(m_szDeviceName));
	m_nChannelNum = -1;
	m_nDecodeType = -1;
	memset(m_Sps,0,sizeof(m_Sps));
	m_nSpsLen = 0;
	m_nCid = -1;
	m_hPlayWnd = NULL;
	m_hDecodeHandle = NULL;
	m_nFlag = GW_PLAY_FLAG_INIT;
	memset(m_szFileName,0,sizeof(m_szFileName));
	m_bVideoStartOrStop = false;
}

GW_PLAY_CHANNEL_INFO::~GW_PLAY_CHANNEL_INFO()
{
	EnterCriticalSection(&m_cs);

	memset(m_szDeviceCode,0,sizeof(m_szDeviceCode));
	memset(m_szDeviceName,0,sizeof(m_szDeviceName));
	m_nChannelNum = -1;
	m_nDecodeType = -1;
	memset(m_Sps,0,sizeof(m_Sps));
	m_nSpsLen = 0;
	m_nCid = -1;
	m_hPlayWnd = NULL;
	m_hDecodeHandle = NULL;
	m_nFlag = GW_PLAY_FlAG_UNDEFINE;
	memset(m_szFileName,0,sizeof(m_szFileName));

	LeaveCriticalSection(&m_cs);
	DeleteCriticalSection(&m_cs);
}

bool GW_PLAY_CHANNEL_INFO::Lock()
{
	EnterCriticalSection(&m_cs);
	return true;
}

bool GW_PLAY_CHANNEL_INFO::Unlock()
{
	LeaveCriticalSection(&m_cs);
	return true;
}

bool GW_PLAY_CHANNEL_INFO::Empty()
{
	memset(m_szDeviceCode,0,sizeof(m_szDeviceCode));
	memset(m_szDeviceName,0,sizeof(m_szDeviceName));
	m_nChannelNum = -1;
	m_nDecodeType = -1;
	memset(m_Sps,0,sizeof(m_Sps));
	m_nSpsLen = 0;
	m_nCid = -1;
	m_hPlayWnd = NULL;
	m_hDecodeHandle = NULL;
	m_nFlag = GW_PLAY_FlAG_UNDEFINE;

	return true;
}

bool GW_PLAY_CHANNEL_INFO::StartPlay()
{
	BOOL bResult = FALSE;

	if (m_nDecodeType <= 0)
	{
		return false;
	}

	if (m_nFlag == GW_PLAY_FLAG_INIT)
	{
		m_hDecodeHandle = g_GWPlay.CreateHandle(m_nDecodeType);
		if ((int)m_hDecodeHandle < 0)
		{
			g_VMLog.WriteVmLog("g_GWPlay.CreateHandleÊ§°Ü,m_nDecodeType=%d",m_nDecodeType);
			return false;
		}

		bResult = g_GWPlay.StreamOpen(m_nDecodeType,m_hDecodeHandle,m_Sps,m_nSpsLen,IFC_PLAY_REAL,0);
		if (bResult == FALSE)
		{
			g_GWPlay.DestoryHandle(m_nDecodeType,m_hDecodeHandle);
			m_hDecodeHandle = NULL;
			g_VMLog.WriteVmLog("g_GWPlay.StreamOpenÊ§°Ü,m_nDecodeType=%d",m_nDecodeType);
			return false;
		}

		bResult = g_GWPlay.Play(m_nDecodeType,m_hDecodeHandle,m_hPlayWnd);
		if (bResult == FALSE)
		{
			g_GWPlay.StreamClose(m_nDecodeType,m_hDecodeHandle);
			g_GWPlay.DestoryHandle(m_nDecodeType,m_hDecodeHandle);
			m_hDecodeHandle = NULL;
			g_VMLog.WriteVmLog("g_GWPlay.PlayÊ§°Ü,m_nDecodeType=%d",m_nDecodeType);
			return false;
		}

		m_nFlag = GW_PLAY_FLAG_STREAM_PLAY;
		m_bVideoStartOrStop = true;
		return true;
	}

	return false;
}

bool GW_PLAY_CHANNEL_INFO::StopPlay()
{
	m_bVideoStartOrStop = false;
	if (m_nDecodeType <= 0)
	{
		m_nDecodeType = 150;
	}
	if ((int)m_hDecodeHandle <= 0)
	{
		m_hDecodeHandle = NULL;
		m_nFlag = GW_PLAY_FLAG_INIT;
		return false;
	}
	if (m_nFlag == GW_PLAY_FLAG_STREAM_PLAY||m_nFlag == GW_PLAY_FLAG_STREAM_PAUSE)
	{
		g_GWPlay.Stop(m_nDecodeType,m_hDecodeHandle);
		g_GWPlay.StreamClose(m_nDecodeType,m_hDecodeHandle);
		g_GWPlay.DestoryHandle(m_nDecodeType,m_hDecodeHandle);
		m_hDecodeHandle = NULL;
		m_nFlag = GW_PLAY_FLAG_INIT;
		return true;
	}
	else if (m_nFlag == GW_PLAY_FLAG_FILE_PLAY||m_nFlag == GW_PLAY_FLAG_FILE_PAUSE)
	{
		g_GWPlay.Stop(m_nDecodeType,m_hDecodeHandle);
		g_GWPlay.FileClose(m_nDecodeType,m_hDecodeHandle);
		g_GWPlay.DestoryHandle(m_nDecodeType,m_hDecodeHandle);
		m_hDecodeHandle = NULL;
		m_nFlag = GW_PLAY_FLAG_INIT;
		return true;
	}

	return false;
}

bool GW_PLAY_CHANNEL_INFO::FastPlay()
{
	BOOL bResult = FALSE;

	if (m_nFlag == GW_PLAY_FLAG_STREAM_PLAY||m_nFlag == GW_PLAY_FLAG_FILE_PLAY)
	{
		bResult = g_GWPlay.Fast(m_nDecodeType,m_hDecodeHandle);
	}

	return ((bResult != FALSE)?true:false);
}

bool GW_PLAY_CHANNEL_INFO::SlowPlay()
{
	BOOL bResult = FALSE;

	if (m_nFlag == GW_PLAY_FLAG_STREAM_PLAY||m_nFlag == GW_PLAY_FLAG_FILE_PLAY)
	{
		bResult = g_GWPlay.Slow(m_nDecodeType,m_hDecodeHandle);
	}

	return ((bResult != FALSE)?true:false);
}

bool GW_PLAY_CHANNEL_INFO::PausePlay()
{
	BOOL bResult = FALSE;

	if (m_nFlag == GW_PLAY_FLAG_STREAM_PLAY)
	{
		bResult = g_GWPlay.Pause(m_nDecodeType,m_hDecodeHandle,TRUE);
		if (bResult != FALSE)
			m_nFlag = GW_PLAY_FLAG_STREAM_PAUSE;
	}
	else if (m_nFlag == GW_PLAY_FLAG_FILE_PLAY)
	{
		bResult = g_GWPlay.Pause(m_nDecodeType,m_hDecodeHandle,TRUE);
		if (bResult != FALSE)
			m_nFlag = GW_PLAY_FLAG_FILE_PAUSE;
	}

	return ((bResult != FALSE)?true:false);
}

bool GW_PLAY_CHANNEL_INFO::ResumePlay()
{
	BOOL bResult = FALSE;

	if (m_nFlag == GW_PLAY_FLAG_STREAM_PAUSE)
	{
		bResult = g_GWPlay.Pause(m_nDecodeType,m_hDecodeHandle,FALSE);
		if (bResult != FALSE)
			m_nFlag = GW_PLAY_FLAG_STREAM_PLAY;
	}
	else if (m_nFlag == GW_PLAY_FLAG_FILE_PAUSE)
	{
		bResult = g_GWPlay.Pause(m_nDecodeType,m_hDecodeHandle,FALSE);
		if (bResult != FALSE)
			m_nFlag = GW_PLAY_FLAG_FILE_PLAY;
	}

	return ((bResult != FALSE)?true:false);
}

bool GW_PLAY_CHANNEL_INFO::SetFileName(char *szFileName)
{
	if (szFileName == NULL||strlen(szFileName) == 0)
		return false;

	sprintf_s(m_szFileName,sizeof(m_szFileName),"%s",szFileName);
	
	return true;
}

bool GW_PLAY_CHANNEL_INFO::FileStartPlay()
{
	if (strlen(m_szFileName) == 0)
		return false;

	BOOL bResult = FALSE;

	if (m_nFlag == GW_PLAY_FLAG_INIT)
	{
		m_hDecodeHandle = g_GWPlay.CreateHandle(m_nDecodeType);
		if ((int)m_hDecodeHandle < 0)
			return false;

		bResult = g_GWPlay.FileOpen(m_nDecodeType,m_hDecodeHandle,m_szFileName);
		if (bResult == FALSE)
		{
			g_GWPlay.DestoryHandle(m_nDecodeType,m_hDecodeHandle);
			m_hDecodeHandle = NULL;
			return false;
		}

		bResult = g_GWPlay.Play(m_nDecodeType,m_hDecodeHandle,m_hPlayWnd);
		if (bResult == FALSE)
		{
			g_GWPlay.FileClose(m_nDecodeType,m_hDecodeHandle);
			g_GWPlay.DestoryHandle(m_nDecodeType,m_hDecodeHandle);
			m_hDecodeHandle = NULL;
			return false;
		}

		m_nFlag = GW_PLAY_FLAG_FILE_PLAY;

		return true;
	}

	return false;
}

bool GW_PLAY_CHANNEL_INFO::FileStopPlay()
{
	if (m_nFlag == GW_PLAY_FLAG_FILE_PLAY||m_nFlag == GW_PLAY_FLAG_FILE_PAUSE)
	{
		g_GWPlay.Stop(m_nDecodeType,m_hDecodeHandle);
		g_GWPlay.FileClose(m_nDecodeType,m_hDecodeHandle);
		g_GWPlay.DestoryHandle(m_nDecodeType,m_hDecodeHandle);
		m_nFlag = GW_PLAY_FLAG_INIT;
		return true;
	}
	else if (m_nFlag == GW_PLAY_FLAG_STREAM_PLAY||m_nFlag == GW_PLAY_FLAG_STREAM_PAUSE)
	{
		g_GWPlay.Stop(m_nDecodeType,m_hDecodeHandle);
		g_GWPlay.StreamClose(m_nDecodeType,m_hDecodeHandle);
		g_GWPlay.DestoryHandle(m_nDecodeType,m_hDecodeHandle);
		m_hDecodeHandle = NULL;
		m_nFlag = GW_PLAY_FLAG_INIT;
		return true;
	}
	return false;
}

bool GW_PLAY_CHANNEL_INFO::SetPos(float fRealPos)
{
	BOOL bResult = FALSE;

	if (m_nFlag == GW_PLAY_FLAG_FILE_PLAY||m_nFlag == GW_PLAY_FLAG_FILE_PAUSE)
	{
		bResult = g_GWPlay.SetPos(m_nDecodeType,m_hDecodeHandle,fRealPos);
		return ((bResult != FALSE)?true:false);
	}
	return false;
}

bool GW_PLAY_CHANNEL_INFO::GetPos(float *pfRealPos)
{
	BOOL bResult = FALSE;

	if (m_nFlag == GW_PLAY_FLAG_FILE_PLAY||m_nFlag == GW_PLAY_FLAG_FILE_PAUSE)
	{
		bResult = g_GWPlay.GetPos(m_nDecodeType,m_hDecodeHandle,pfRealPos);
		return ((bResult != FALSE)?true:false);
	}
	return false;
}

bool GW_PLAY_CHANNEL_INFO::GetVideoParams(int &nBrightness,int &nContrast,int &nSaturation,int &nHue)
{
	TVideoParams VideoParams;
	memset(&VideoParams,0,sizeof(TVideoParams));

	BOOL bResult = FALSE;

	if (m_nFlag == GW_PLAY_FLAG_STREAM_PLAY||m_nFlag == GW_PLAY_FLAG_STREAM_PAUSE
		||m_nFlag == GW_PLAY_FLAG_FILE_PLAY||m_nFlag == GW_PLAY_FLAG_FILE_PAUSE)
	{
		bResult = g_GWPlay.GetVideoParams(m_nDecodeType,m_hDecodeHandle,&VideoParams);
		if (bResult == FALSE)
			return false;

		nBrightness = VideoParams.nBrightness;
		nContrast = VideoParams.nContrast;
		nSaturation = VideoParams.nSaturation;
		nHue = VideoParams.nHue;

		return true;
	}

	return false;
}

bool GW_PLAY_CHANNEL_INFO::SetVideoParams(int nBrightness,int nContrast,int nSaturation,int nHue,bool bDefault)
{
	TVideoParams VideoParams;
	memset(&VideoParams,0,sizeof(TVideoParams));

	BOOL bResult = FALSE;

	if (m_nFlag == GW_PLAY_FLAG_STREAM_PLAY||m_nFlag == GW_PLAY_FLAG_STREAM_PAUSE
		||m_nFlag == GW_PLAY_FLAG_FILE_PLAY||m_nFlag == GW_PLAY_FLAG_FILE_PAUSE)
	{
		VideoParams.nBrightness = nBrightness;
		VideoParams.nContrast = nContrast;
		VideoParams.nSaturation = nSaturation;
		VideoParams.nHue = nHue;

		bResult = g_GWPlay.SetVideoParams(m_nDecodeType,m_hDecodeHandle,&VideoParams,bDefault);

		return ((bResult != FALSE)?true:false);
	}

	return false;
}

bool GW_PLAY_CHANNEL_INFO::SetDecodeType(int nDecodeType)
{
	if (nDecodeType >= 0)
	{
		m_nDecodeType = nDecodeType;
		return true;
	}
	return false;
}

bool GW_PLAY_CHANNEL_INFO::SetSpsInfo(char *pSps,int nSpsLen)
{
	if (pSps == NULL||nSpsLen <= 0)
		return false;

	if (nSpsLen > sizeof(m_Sps))
		return false;

	memcpy(m_Sps,pSps,nSpsLen);
	
	return true;
}

bool GW_PLAY_CHANNEL_INFO::SetData(char *pDeviceCode,char *pDeviceName,int nChannelNum,int nDecodeType,
			 char *pSps,int nSpsLen,HWND hPlayWnd,HANDLE hDecodeHandle,int nFlag)
{
	if (pDeviceCode != NULL)
	{
		strcpy(m_szDeviceCode,pDeviceCode);
	}

	if (pDeviceName != NULL)
	{
		strcpy(m_szDeviceName,pDeviceCode);
	}

	if (nChannelNum >= 0)
	{
		m_nChannelNum = nChannelNum;
	}

	if (nDecodeType >= 0)
	{
		m_nDecodeType = nDecodeType;
	}

	if (nSpsLen == 0)
	{
		m_nSpsLen = 0;
	}
	else if (nSpsLen > 0&&pSps != NULL)
	{
		m_nSpsLen = nSpsLen;
		memcpy(m_Sps,pSps,m_nSpsLen);
	}

	if (hPlayWnd != NULL)
	{
		m_hPlayWnd = hPlayWnd;
	}

	if (hDecodeHandle != NULL)
	{
		m_hDecodeHandle = hDecodeHandle;
	}

	if (nFlag >= 0)
	{
		m_nFlag = nFlag;
	}
	
	return true;
}
