#ifndef _GW_PLAY_SDK_H_
#define _GW_PLAY_SDK_H_


//////////////////////////////////////////////////////////////////////////
///描述符长度
#define        DES_LEN                128   
///默认字符串长度
#define        DEFAULT_LEN        128   

///播放类型：实时流
#define        IFC_PLAY_REAL                    0
///播放类型：历史流
#define        IFC_PLAY_HISTORY              1

//////////////////////////////////////////////////////////////////////////
typedef enum _DEOCDE_DATA_TYPE
{
	T_AUDIO16     = 1,
	T_RGB32         = 2,
	T_YV12            = 3,
}DEOCDE_DATA_TYPE;


// SDK信息
typedef struct _TPluginInfo
{
	long nSize;//数据结构长度          
	char szDesc[DES_LEN];//模块描述            
	char szSoftVersion[DEFAULT_LEN]; //插件版本
	char szProvider[DEFAULT_LEN];//厂商信息    
}TPluginInfo;

// 帧信息
typedef  struct _TFrameInfo
{
	long nWidth;//画面宽，单位像素。如果是音频数据则为0                     
	long nHeight;//画面高。如果是音频数据则为0                    
	long nStamp;//时标信息，单位毫秒                     
	DEOCDE_DATA_TYPE nDecodeType;//数据类型，1（T_AUDIO16），2（T_RGB32）， 3（T_YV12）                      
	long nFrameDuration;//每帧持续时间                 
}TFrameInfo;

//解码回调参数信息
typedef struct _TDecCallbackParams
{
	BYTE*       pBuf;//数据内容
	long        nSize;//数据长度
	TFrameInfo* pFrameInfo;//帧信息
	long    nReserved1;//保留值
	long    nReserved2;//保留值
}TDecCallbackParams;

///绘图回调参数信息
typedef struct _TDrawCallbackParams
{
	HDC     hDc;//画图DC
	long    nReserved1;//保留值
	long    nReserved2;//保留值
}TDrawCallbackParams;

///文件索引信息
typedef struct _TFileIndexParams
{
	long    nFrameNo;//帧编号
	long    nFilePos;//文件位置
	long    nFrameTime;//帧时标（毫秒）    
}TFileIndexParams;

///视频参数信息
typedef struct _TVideoParams
{
	int nBrightness;//亮度（取值：0-255）
	int nContrast;//对比度（取值：0-255）
	int nSaturation;//饱和度（取值：0-255）
	int nHue;//色调（取值：0-255）
}TVideoParams;

//流数据参数
typedef struct _TStreamDataParams
{
	BYTE*   pBuffer;//数据内容
	long    nSize;//数据长度
	long    nReserved1;//保留值
	long    nReserved2;//保留值
}TStreamDataParams;

//////////////////////////////////////////////////////////////////////////
//音视频解码回调函数
typedef void (CALLBACK* PDecCBFun)( HANDLE hHandle, 
								  TDecCallbackParams* pDecCBInfo, 
								  DWORD* dwUser);

//画图回调函数
typedef void (CALLBACK* PDrawCBFun)(HANDLE hHandle, 
								   TDrawCallbackParams* pDrawCBInfo, 
								   DWORD* dwUser);

//缓冲处理回调函数
typedef void (CALLBACK * PSourceBufCBFun)(HANDLE hHandle, 
										 DWORD dwBufSize, 
										 DWORD dwUser,
										 void*pResvered);

typedef void (CALLBACK * PFileIndexCBFun)(TFileIndexParams* pFileIndexCBInfo, 
										 DWORD dwUser);

//-----------------------------------------------------------------------------
//     接口定义
//-----------------------------------------------------------------------------

//IFC_GetLastError 函数指针定义
typedef int (__stdcall* PIFC_GetLastError) (HANDLE hHandle);

//IFC_Initialize 函数指针定义
typedef BOOL (__stdcall* PIFC_Initialize) ();

//IFC_Uninitialize 函数指针定义
typedef void (__stdcall* PIFC_Uninitialize) ();

//IFC_GetPluginInfo 函数指针定义
typedef BOOL( __stdcall* PIFC_GetPluginInfo) (TPluginInfo *pInfo);

//IFC_GetFileHeaderLen 函数指针定义
typedef int ( __stdcall* PIFC_GetFileHeaderLen) (LPCTSTR lpszLocalFileName);

//-----------------------------------------------------------------------------
//     句柄操作定义
//-----------------------------------------------------------------------------

//IFC_CreateHandle 函数指针定义
typedef HANDLE ( __stdcall* PIFC_CreateHandle)();

//IFC_DestoryHandle 原型定义
typedef BOOL ( __stdcall* PIFC_DestroyHandle)(HANDLE hHandle);

//-----------------------------------------------------------------------------
//     数据流解码方式定义
//-----------------------------------------------------------------------------

//IFC_StreamOpen 函数指针定义
typedef BOOL (__stdcall* PIFC_StreamOpen) (HANDLE hHandle, char *pData, int nLen, int nStreamType, long nReserved1);

//IFC_Play 原型定义
typedef BOOL (__stdcall* PIFC_Play) (HANDLE hHandle, HWND hWnd);

//IFC_SetDecCallback 函数指针定义
typedef BOOL (__stdcall* PIFC_SetDecCallBack) (HANDLE hHandle, PDecCBFun *pfDecCBFun, DWORD dwUser);

//IFC_SetDrawCallback 函数指针定义
typedef BOOL (__stdcall* PIFC_SetDrawCallBack) (HANDLE hHandle, PDrawCBFun *pfDrawCBFun, DWORD dwUser);

//IFC_SetSourceBufCallBack 函数指针定义
typedef BOOL (__stdcall* PIFC_SetSourceBufCallBack) (HANDLE hHandle, 
                                                   DWORD dwThreShold,
                                                   PSourceBufCBFun* pfSourceBufCBFun, 
                                                   DWORD dwUser, 
                                                   void *pReserved);

//IFC_SetFileIndexCallBack 函数指针定义
typedef HANDLE (__stdcall* PIFC_SetFileIndexCallBack)(LPCTSTR lpszFileName, 
                                                     PFileIndexCBFun* pfFileIndexCBFun, 
                                                     DWORD dwUser);

//IFC_StopFileIndex 函数指针定义
typedef HANDLE (__stdcall* PIFC_StopFileIndex)(HANDLE hIndexHandle);

//IFC_StreamSetPlayBuffer 函数指针定义
typedef BOOL (__stdcall* PIFC_StreamSetPlayBuffer) ( HANDLE hHandle,  int nBufferLen);

//IFC_StreamInputData 函数指针定义
typedef BOOL (__stdcall* PIFC_StreamInputData) (HANDLE hHandle, TStreamDataParams* pStreamInData);

//IFC_StreamResetBuffer 函数指针定义
typedef BOOL (__stdcall* PIFC_StreamResetBuffer) (HANDLE hHandle);

//IFC_StreamRefreshPlay 函数指针定义
typedef BOOL (__stdcall* PIFC_StreamRefreshPlay) ( HANDLE hHandle);

//IFC_Pause 函数指针定义
typedef BOOL (__stdcall* PIFC_Pause) (HANDLE hHandle, BOOL bPause);

//IFC_Fast 函数指针定义
typedef BOOL (__stdcall* PIFC_Fast) (HANDLE hHandle);

//IFC_Slow 函数指针定义
typedef BOOL (__stdcall* PIFC_Slow) (HANDLE hHandle);

//IFC_SoundOpen 函数指针定义
typedef BOOL (__stdcall* PIFC_SoundOpen) (HANDLE hHandle , int nSoundPlayType);

//IFC_SoundClose 函数指针定义
typedef BOOL (__stdcall* PIFC_SoundClose) (HANDLE hHandle);

//IFC_SetVolume 函数指针定义
typedef BOOL (__stdcall* PIFC_SetVolume) (HANDLE hHandle, DWORD dwVolume);

//IFC_StreamClose 函数指针定义
typedef BOOL (__stdcall* PIFC_StreamClose) ( HANDLE hHandle);

//-----------------------------------------------------------------------------
//     文件解码方式定义
//-----------------------------------------------------------------------------

//IFC_FileOpen 函数指针定义
typedef BOOL (__stdcall* PIFC_FileOpen) (HANDLE hHandle, LPCTSTR lpszFileName);

//IFC_FileClose 函数指针定义
typedef BOOL (__stdcall* PIFC_FileClose) (HANDLE hHandle);

//IFC_Stop 函数指针定义
typedef BOOL (__stdcall* PIFC_Stop) (HANDLE hHandle);

//IFC_PlayCapturePic 函数指针定义
typedef BOOL (__stdcall* PIFC_PlayCapturePic) (HANDLE hHandle, int nPicType, int nPicQuality, LPCTSTR lpszPicFileName);

//IFC_PlayRefresh 函数指针定义
typedef BOOL (__stdcall* PIFC_PlayRefresh) (HANDLE hHandle);

//IFC_Pause 函数指针定义
typedef BOOL (__stdcall* PIFC_Pause) (HANDLE hHandle,BOOL bPause);

//IFC_FastBack 函数指针定义
typedef BOOL (__stdcall* PIFC_FastBack) (HANDLE hHandle);

//IFC_SlowBack 函数指针定义
typedef BOOL (__stdcall* PIFC_SlowBack) (HANDLE hHandle);

//IFC_Step 函数指针定义
typedef BOOL (__stdcall* PIFC_Step) (HANDLE hHandle);

//IFC_StepBack 函数指针定义
typedef BOOL (__stdcall* PIFC_StepBack) (HANDLE hHandle);

//IFC_SetPos 函数指针定义
typedef BOOL (__stdcall* PIFC_SetPos) (HANDLE hHandle, float fRealPos);

//IFC_GetPos 函数指针定义
typedef BOOL (__stdcall* PIFC_GetPos) (HANDLE hHandle, float *pfRealPos);

//IFC_GetTotalTime 函数指针定义
typedef BOOL (__stdcall* PIFC_GetTotalTime) (HANDLE hHandle, DWORD *pdwFileTime);

//IFC_GetPlayedTime 函数指针定义
typedef BOOL (__stdcall* PIFC_GetPlayedTime) (HANDLE hHandle, DWORD *pdwPlayedTime);

//IFC_GetTotalFrames 函数指针定义
typedef BOOL (__stdcall* PIFC_GetTotalFrames) (HANDLE hHandle, DWORD *pdwTotalFrames);

//IFC_GetFramePosition 函数指针定义
typedef BOOL (__stdcall* PIFC_GetFramePosition) (HANDLE hHandle, DWORD dwFrameNo, DWORD *pdwPosition);

//IFC_GetPlayedFrames 函数指针定义
typedef BOOL (__stdcall* PIFC_GetPlayedFrames) (HANDLE hHandle, DWORD *pdwPlayedFrames);

//IFC_SetVideoParams 函数指针定义
typedef BOOL (__stdcall* PIFC_SetVideoParams)(HANDLE hHandle, TVideoParams* pVideoParams, BOOL bDefault);

//IFC_GetVideoParams 函数指针定义
typedef BOOL (__stdcall* PIFC_GetVideoParams)(HANDLE hHandle, TVideoParams* pVideoParams);

//IFC_SetVideoWindows 函数指针定义
typedef BOOL (__stdcall* PIFC_SetVideoWindows)(HANDLE hHandle, HWND hWnd, RECT* pstDisplayRect, BOOL bEnable);

//IFC 函数指针结构体
typedef struct _IFC_FUNCTION_PTR_
{
          PIFC_GetLastError                  GetLastError;
          PIFC_Initialize                         Initialize;
          PIFC_Uninitialize                    Uninitialize;
          PIFC_GetPluginInfo                GetPluginInfo;
          PIFC_GetFileHeaderLen         GetFileHeaderLen;
          PIFC_CreateHandle                CreateHandle;
          PIFC_DestroyHandle              DestroyHandle;
          PIFC_StreamOpen                  StreamOpen;
          PIFC_Play                                Play;
          PIFC_SetDecCallBack             SetDecCallBack;
          PIFC_SetDrawCallBack           SetDrawCallBack;
          PIFC_SetSourceBufCallBack   SetSourceBufCallBack;
          PIFC_SetFileIndexCallBack     SetFileIndexCallBack;
          PIFC_StopFileIndex                 StopFileIndex;
          PIFC_StreamSetPlayBuffer     StreamSetPlayBuffer;
          PIFC_StreamInputData           StreamInputData;
          PIFC_StreamResetBuffer        StreamResetBuffer;
          PIFC_StreamRefreshPlay        StreamRefreshPlay;
          PIFC_Pause                              Pause;
          PIFC_Fast                                 Fast;
          PIFC_Slow                                Slow;
          PIFC_SoundOpen                    SoundOpen;
          PIFC_SoundClose                    SoundClose;
          PIFC_SetVolume                     SetVolume;
          PIFC_StreamClose                  StreamClose;
          PIFC_FileOpen                        FileOpen;
          PIFC_FileClose                        FileClose;
          PIFC_Stop                               Stop;
          PIFC_PlayCapturePic             PlayCapturePic;
          PIFC_PlayRefresh                  PlayRefresh;
          PIFC_FastBack                       FastBack;
          PIFC_SlowBack                      SlowBack;
          PIFC_Step                              Step;
          PIFC_StepBack                      StepBack;
          PIFC_SetPos                          SetPos;
          PIFC_GetPos                         GetPos;
          PIFC_GetTotalTime              GetTotalTime;
          PIFC_GetPlayedTime           GetPlayedTime;
          PIFC_GetTotalFrames          GetTotalFrames;
          PIFC_GetFramePosition       GetFramePosition;
          PIFC_GetPlayedFrames       GetPlayedFrames;
          PIFC_SetVideoParams          SetVideoParams;
          PIFC_GetVideoParams         GetVideoParams;
          PIFC_SetVideoWindows       SetVideoWindows;
}IFC_FUNCTION_PTR;

//解码信息
typedef struct _GW_DECODE_FUNC_INFO_
{
	int nInitFlag;
	int nCode;
	HMODULE hModule; 
	IFC_FUNCTION_PTR DecodeFunc;
}GW_DECODE_FUNC_INFO;

//////////////////////////////////////////////////////////////////////////
//设置解码信息
bool SetGWDecFuncInfo(GW_DECODE_FUNC_INFO *pDecFuncInfo,int nCode,HMODULE hModule,IFC_FUNCTION_PTR* pIfcFuncPtr);

//得到解码函数
bool GWGetModuleFunc(HMODULE hModule,int nCode);

//初始化解码
bool InitGWDecFuncInfo(int nCode);

//得到解码信息
GW_DECODE_FUNC_INFO *GetGWDecFuncInfo(int nCode);

//判断是否存在解码信息
BOOL IsExsitGWDecInfo(int nCode);

#endif
