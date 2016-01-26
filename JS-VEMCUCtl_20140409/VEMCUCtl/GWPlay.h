#ifndef _GW_PLAY_H_
#define _GW_PLAY_H_

#include "GWPlaySDK.h"

class CGWPlay
{
public:
	CGWPlay(void);
	virtual ~CGWPlay(void);

public:
	BOOL  LoadGWDecode(int nCode);
	BOOL  Initialize(int nCode);
	void    Uninitialize(int nCode);
	BOOL  GetPluginInfo (int nCode,TPluginInfo *pInfo);
	int       GetFileHeaderLen (int nCode,LPCTSTR lpszLocalFileName);
	HANDLE  CreateHandle(int nCode);
	BOOL  DestoryHandle(int nCode,HANDLE hHandle);
	BOOL  StreamOpen (int nCode,HANDLE hHandle, char *pData, int nLen, int nStreamType, long nReserved1);
	BOOL  Play (int nCode,HANDLE hHandle, HWND hWnd);
	BOOL  SetDecCallback(int nCode,HANDLE hHandle, PDecCBFun *pfDecCBFun, DWORD dwUser);
	BOOL  SetDrawCallback (int nCode,HANDLE hHandle, PDrawCBFun *pfDrawCBFun, DWORD dwUser);
	BOOL  SetSourceBufCallBack (int nCode,HANDLE hHandle, DWORD dwThreShold,PSourceBufCBFun* pfSourceBufCBFun, DWORD dwUser, void *pReserved);
	HANDLE SetFileIndexCallBack(int nCode,LPCTSTR lpszFileName,PFileIndexCBFun* pfFileIndexCBFun, DWORD dwUser);
	HANDLE StopFileIndex(int nCode,HANDLE hIndexHandle);
	BOOL  StreamSetPlayBuffer(int nCode,HANDLE hHandle,  int nBufferLen);
	BOOL  StreamInputData (int nCode,HANDLE hHandle, TStreamDataParams* pStreamInData);
	BOOL  StreamResetBuffer (int nCode,HANDLE hHandle);
	BOOL  StreamRefreshPlay ( int nCode,HANDLE hHandle);
	BOOL  Pause (int nCode,HANDLE hHandle, BOOL bPause);
	BOOL  Fast(int nCode,HANDLE hHandle);
	BOOL  Slow(int nCode,HANDLE hHandle);
	BOOL  SoundOpen(int nCode,HANDLE hHandle , int nSoundPlayType);
	BOOL  SoundClose (int nCode,HANDLE hHandle);
	BOOL  SetVolume (int nCode,HANDLE hHandle, DWORD dwVolume);
	BOOL  StreamClose (int nCode,HANDLE hHandle);
	BOOL  FileOpen(int nCode,HANDLE hHandle, LPCTSTR lpszFileName);
	BOOL  FileClose(int nCode,HANDLE hHandle);
	BOOL  Stop(int nCode,HANDLE hHandle);
	BOOL  PlayCapturePic (int nCode,HANDLE hHandle, int nPicType, int nPicQuality, LPCTSTR lpszPicFileName);
	BOOL  PlayRefresh (int nCode,HANDLE hHandle);
	BOOL  FastBack(int nCode,HANDLE hHandle);
	BOOL  SlowBack(int nCode,HANDLE hHandle);
	BOOL  Step(int nCode,HANDLE hHandle);
	BOOL  StepBack(int nCode,HANDLE hHandle);
	BOOL  SetPos(int nCode,HANDLE hHandle, float fRealPos);
	BOOL  GetPos(int nCode,HANDLE hHandle, float *pfRealPos);
	BOOL  GetTotalTime(int nCode,HANDLE hHandle, DWORD *pdwFileTime);
	BOOL  GetPlayedTime (int nCode,HANDLE hHandle, DWORD *pdwPlayedTime);
	BOOL  GetTotalFrames (int nCode,HANDLE hHandle, DWORD *pdwTotalFrames);
	BOOL  GetFramePosition (int nCode,HANDLE hHandle, DWORD dwFrameNo, DWORD *pdwPosition);
	BOOL  GetPlayedFrames (int nCode,HANDLE hHandle, DWORD *pdwPlayedFrames);
	BOOL  SetVideoParams(int nCode,HANDLE hHandle,TVideoParams* pVideoParams,BOOL bDefault);
	BOOL  GetVideoParams(int nCode,HANDLE hHandle, TVideoParams* pVideoParams);
	BOOL  SetVideoWindows (int nCode,HANDLE hHandle, HWND hWnd,RECT* pstDisplayRect,BOOL bEnable);

public:
	CRITICAL_SECTION m_cs;
};

#endif
