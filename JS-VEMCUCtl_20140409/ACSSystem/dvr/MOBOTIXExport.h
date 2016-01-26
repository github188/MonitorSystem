//---------------------------------------------------------------------------
#ifndef MOBOTIXExportH
#define MOBOTIXExportH
#include "windows.h"

#ifdef MOBOTIX_LIB
#include "RecordConst.h"
#else

#endif
//---------------------------------------------------------------------------

#ifdef MOBOTIX_EXPORTS
#define EN_API extern "C" __declspec(dllexport)
#else
#define EN_API extern "C" __declspec(dllimport)
#endif

#define CC __stdcall

#ifndef CALLBACK_TYPE
#define CALLBACK_TYPE
enum CallBackType
{
    cbtUnknown = 0,     //未知类型
    cbtSerial = 1,      //串口数据上传回调
    cbtVoice = 2,       //语音数据上传回调
    cbtVideo = 3,       //视频数据上传回调
    cbtMessage = 4      //消息通知上传回调
};
#endif

#ifndef HANDLE_KIND
#define HANDLE_KIND
enum HandleKind
{
    hkUser = 1,         //用户注册句柄
    hkMedia = 2,        //通道连接句柄
    hkSerial = 3,       //串口控制句柄
    hkVioce = 4,        //语音对讲句柄
    hkAuxOut = 5        //辅助输出句柄
};
#endif

#ifndef NET_DVR_ALARM_INFO
#define NET_DVR_ALARM_INFO
//上传报警信息
struct NetDVRAlarmInfo
{
    DWORD dwDeviceType;                 /*设备类型 MOBOTIX EDVR 类型值为5*/
	DWORD dwAlarmType;                  /*0-信号量报警,1-硬盘满,2-信号丢失，3－移动侦测，4－硬盘未格式化,5-读写硬盘出错,6-遮挡报警,7-制式不匹配, 8-非法访问*/
	DWORD dwAlarmInputNumber;           /*报警输入端口,从0开始*/
	DWORD dwAlarmOutputNumber[4];       /*报警输入端口对应的输出端口，哪一位为1表示对应哪一个输出*/
	DWORD dwAlarmRelateChannel[16];     /*报警输入端口对应的录像通道，哪一位为1表示对应哪一路录像,dwAlarmRelateChannel[0]对应第1个通道*/
	DWORD dwChannel[16];                /*dwAlarmType为2或3时，表示哪个通道，dwChannel[0]位对应第0个通道*/
	DWORD dwDiskNumber[16];             /*dwAlarmType为4时,表示哪个硬盘*/
};                                                    
#endif

//定义解码类型          added in 20070716
#ifndef DECODE_TYPE
#define DECODE_TYPE
enum DecodeType
{
    dtSoftDecode = 1,   //软解码
    dtHardDecode = 2    //硬解码
};
#endif

//定义视频制式          added in 20070720
#ifndef NZ_VIDEO_STANDARD
#define NZ_VIDEO_STANDARD
typedef enum {
   vsNone  = 0x80000000,
   vsNTSC  = 0x00000001,
   vsPAL   = 0x00000002,
   vsSECAM = 0x00000004
} VideoStandard;
#endif

//定义硬解码模式                added in 20070724
#ifndef HARD_DECODE_MODE
#define HARD_DECODE_MODE
enum HardDecodeMode
{
   hdmBoth                  = 0,        //硬解码后同时输出到PC屏幕和电视墙上
   hdmDecodeOnly            = 1,        //硬解码后仅输出到PC屏幕上
   hdmOutputOnly            = 2,        //硬解码后仅输出到电视墙上
   hdmBothButSoftDecode     = 3         //硬解码后输出到电视墙，软解码后输出到PC屏幕上  added in 20080605
};
#endif

//通道状态                added in 20080523
#ifndef CHANNEL_STATUS
#define CHANNEL_STATUS
struct ChannelStatus
{
    unsigned long   ulNum;
    BOOL            bOpen;                          //是否已打开
    BOOL            bVideoPlay;                     //是否正在播放视频
    BOOL            bRecord;                        //是否正在录像
    BOOL            bAudioPlay;                     //是否正在播放音频
    BOOL            bInAutoConnection;              //是否在自动恢复之中
};
#endif

//mobotix虚拟PTZ命令发送对象added by ligen for mobotix in 20091020
enum VPTZCommandTargetType
{
    ttRemote = 0,  //发向设备
    ttSoftware     //发向软件
};
//end

#ifdef MOBOTIX_LIB
//录像文件信息
typedef SEC_MSG_QUERY_DVR_RECORD_FILE_INFO RecordFileInfo;
typedef SEC_MSG_QUERY_DVR_RECORD_FILE_INFO_EX RecordFileInfoEx;
#else

#ifndef RECORD_INFO_DEFINED
#define RECORD_INFO_DEFINED

//录像文件类型
#define NZIT_DVR_RECORD_TYPE_UNKNOWN     0x00    //未知的录像类型
#define NZIT_DVR_RECORD_TYPE_NORMAL      0x01    //普通录像
#define NZIT_DVR_RECORD_TYPE_MOTION      0x02    //动态侦测录像
#define NZIT_DVR_RECORD_TYPE_ALARM       0x03    //报警录像
#define NZIT_DVR_RECORD_TYPE_ALL         0x10    //所有类型的录像

//录像时间
#ifndef DEC_RecordTime
#define DEC_RecordTime
typedef struct tag_RecordTime
{
    DWORD m_dwYear;       /* 年*/
    DWORD m_dwMonth;      /* 月*/
    DWORD m_dwDay;        /* 日*/
    DWORD m_dwHour;       /* 时*/
    DWORD m_dwMinute;     /* 分*/
    DWORD m_dwSecond;     /* 秒*/
}RecordTime;
#endif

//录像文件信息
typedef struct tag_RecordFileInfo
{
    unsigned short  m_usChannelNo;      //通道号，从1开始，第1个通道为1
    unsigned char   m_ucRecordFileType; //录像文件类型
    unsigned char   m_ucReserve;        //保留
    unsigned char   m_szPath[64];       //录像文件的绝对路径
    RecordTime      m_rtStart;          //录像文件开始时间、日期
    RecordTime      m_rtEnd;            //录像文件结束时间、日期
}RecordFileInfo;

//录像文件信息扩充
typedef struct tag_RecordFileInfoEx
{
    unsigned short  m_usID;             //录像文件ID
    unsigned short  m_usReserve;        //保留
    unsigned short  m_usChannelNo;      //通道号，从1开始，第1个通道为1
    unsigned char   m_ucRecordFileType; //录像文件类型
    unsigned char   m_ucReserve2;       //保留2
    unsigned char   m_szName[64];       //录像文件路径和名称
    RecordTime      m_rtStart;          //录像文件开始时间、日期
    RecordTime      m_rtEnd;            //录像文件结束时间、日期
    unsigned long   m_ulSize;           //录像文件大小
}RecordFileInfoEx;

#endif
#endif

//视频服务器连接信息
struct ConnInfo;

//mobotix 摄像机的控制命令, 复制自mobotix_sdk.h added by ligen for mobotix in 20091020
#ifndef MOBOTIX_EXPORTS
#define MOBOTIX_SDK_BASE                           10000
//虚拟PTZ命令字
#define MOBOTIX_SDK_VPTZ_BASE                      (MOBOTIX_SDK_BASE + 1000)
#define MOBOTIX_SDK_VPTZ_UP                        (MOBOTIX_SDK_VPTZ_BASE + 1)    //虚拟PTZ上移
#define MOBOTIX_SDK_VPTZ_DOWN                      (MOBOTIX_SDK_VPTZ_BASE + 2)    //虚拟PTZ下移
#define MOBOTIX_SDK_VPTZ_LEFT                      (MOBOTIX_SDK_VPTZ_BASE + 3)    //虚拟PTZ左移
#define MOBOTIX_SDK_VPTZ_RIGHT                     (MOBOTIX_SDK_VPTZ_BASE + 4)    //虚拟PTZ右移
#define MOBOTIX_SDK_VPTZ_CENTER_PAN                (MOBOTIX_SDK_VPTZ_BASE + 5)    //虚拟PTZ回到中间
#define MOBOTIX_SDK_VPTZ_ZOOMIN                    (MOBOTIX_SDK_VPTZ_BASE + 6)    //虚拟PTZ拉近
#define MOBOTIX_SDK_VPTZ_ZOOMOUT                   (MOBOTIX_SDK_VPTZ_BASE + 7)    //虚拟PTZ拉远
#define MOBOTIX_SDK_VPTZ_ZOOM_X1                   (MOBOTIX_SDK_VPTZ_BASE + 8)    //虚拟PTZ回到原始距离
#define MOBOTIX_SDK_VPTZ_ROTATE                    (MOBOTIX_SDK_VPTZ_BASE + 9)    //虚拟PTZ顺时针旋转
#define MOBOTIX_SDK_VPTZ_ROTATE2                   (MOBOTIX_SDK_VPTZ_BASE + 10)   //虚拟PTZ逆时针旋转
#define MOBOTIX_SDK_VPTZ_DISPLAYMODE_SIMPLE        (MOBOTIX_SDK_VPTZ_BASE + 11)   //虚拟PTZ原始画面
#define MOBOTIX_SDK_VPTZ_DISPLAYMODE_SURROUND      (MOBOTIX_SDK_VPTZ_BASE + 12)   //虚拟PTZ展平画面
#define MOBOTIX_SDK_VPTZ_DISPLAYMODE_SWEEP         (MOBOTIX_SDK_VPTZ_BASE + 13)   //虚拟PTZ环视画面
#define MOBOTIX_SDK_VPTZ_DISPLAYMODE_PANORAMA      (MOBOTIX_SDK_VPTZ_BASE + 14)   //虚拟PTZ全景画面
#define MOBOTIX_SDK_VPTZ_DISPLAYMODE_DOUBLEPANO    (MOBOTIX_SDK_VPTZ_BASE + 15)   //虚拟PTZ双全景画面
#define MOBOTIX_SDK_VPTZ_DISPLAYMODE_PANORAMAFOCUS (MOBOTIX_SDK_VPTZ_BASE + 16)   //虚拟PTZ全景焦点画面
#define MOBOTIX_SDK_VPTZ_DISPLAYMODE_FOUR          (MOBOTIX_SDK_VPTZ_BASE + 17)   //虚拟PTZ四画面
#define MOBOTIX_SDK_VPTZ_SELECT_WINDOW             (MOBOTIX_SDK_VPTZ_BASE + 18)   //虚拟PTZ选择控制窗口, 多画面时有效
#endif // MOBOTIX_EXPORTS
//end

#ifndef GENERAL_CALLBACK
#define GENERAL_CALLBACK
typedef BOOL (CALLBACK* GeneralDataCallBack)(LONG lUserID,CallBackType cbtCallBackType,
        char *pBuf,DWORD dwBufLen,DWORD dwUser);
#endif

//added on 20100104
#ifndef DRAW_CALLBACK
#define DRAW_CALLBACK
typedef void (CALLBACK* DrawCallBack)(LONG lChannelHandle,HDC hDC,int iWidth,
    int iHeight,DWORD dwUser);
#endif

//设置非全局回调函数
EN_API void CC MOBOTIX_SetGeneralDataCallBack2(long lChannelHandle,
											   GeneralDataCallBack pGeneralDataCallBack,
											   DWORD dwUser);
//设置解码类型  added in 20070716
EN_API BOOL CC MOBOTIX_SetDecodeType(DecodeType dtType);

EN_API void CC MOBOTIX_SetGeneralDataCallBack(GeneralDataCallBack pGeneralDataCallBack);

EN_API BOOL CC MOBOTIX_InitClient(long lParentHandle);
EN_API BOOL CC MOBOTIX_UnInitClient();

EN_API long CC MOBOTIX_Login(ConnInfo* pCurConnInfo);
EN_API BOOL CC MOBOTIX_Logout(long lLoginHandle);

EN_API long CC MOBOTIX_OpenChannel(const char* pszURL,HardDecodeMode hdmHardDecodeMode);
//指定硬解码通道号，如不指定，自动分配一个硬解码通道号 added in 20080701
EN_API long CC MOBOTIX_OpenChannel2(const char* pszURL,
    HardDecodeMode hdmHardDecodeMode,unsigned long ulHardDecodeChannel);
EN_API BOOL CC MOBOTIX_CloseChannel(long lChannelHandle);

//added on 20100104
EN_API BOOL CC MOBOTIX_SetDrawCallBack(long lChannelHandle,
                                       DrawCallBack pDrawCallBack,
                                       DWORD dwUser);

EN_API BOOL CC MOBOTIX_StartVideo(long lChannelHandle,long lDisplayHandle);
EN_API BOOL CC MOBOTIX_StopVideo(long lChannelHandle);
//刷新某个通道视频     added in 20090421
EN_API BOOL CC MOBOTIX_RefreshVideo(long lChannelHandle);

EN_API BOOL CC MOBOTIX_StartRecord(long lChannelHandle,const char* pszFileName);
EN_API BOOL CC MOBOTIX_StopRecord(long lChannelHandle);

EN_API BOOL CC MOBOTIX_StartAudio(long lChannelHandle);
EN_API BOOL CC MOBOTIX_StopAudio(long lChannelHandle);

EN_API BOOL CC MOBOTIX_TakePicture(long lChannelHandle,char *pszFileName);

EN_API BOOL CC MOBOTIX_Preview_Control(long lChannelHandle,char *pPTZCodeBuf,
        DWORD dwBufSize,long lCOMPort);
EN_API BOOL CC MOBOTIX_NoPreview_Control(long lChannelHandle,unsigned long ulChannel,
        char *pPTZCodeBuf,DWORD dwBufSize,long lCOMPort);

//lParam1控制摄像机方向时表示速度;控制摄像机预置点时表示预置点号，其他参数暂未使用
//在启用视频预览功能情况下，通过串口发送摄像机控制命令  added in 20080508
EN_API BOOL CC MOBOTIX_Preview_SendCmd(long lChannelHandle,
    long lCmdID,long lParam1,long lParam2,long lParam3);
//在未启用视频预览功能情况下，通过串口发送摄像机控制命令  added in 20080508
EN_API BOOL CC MOBOTIX_NoPreview_SendCmd(long lChannelHandle,unsigned long ulChannel,
    long lCmdID,long lParam1,long lParam2,long lParam3);

EN_API BOOL CC MOBOTIX_SetAlarmOut(long lChannelHandle, long lOutPort, long lOutStatic);

EN_API BOOL CC MOBOTIX_StartRemoteRecord(long lChannelHandle,LONG lRecordType);
EN_API BOOL CC MOBOTIX_StopRemoteRecord(long lChannelHandle);
EN_API long CC MOBOTIX_GetRemoteRecordState(long lChannelHandle);


//前端录像主机登录
EN_API long CC MOBOTIX_RecordLogin(ConnInfo* pCurConnInfo);
//前端录像主机注销
EN_API BOOL CC MOBOTIX_RecordLogout(long lLoginHandle);
//打开录像通道
EN_API long CC MOBOTIX_RecordChannelOpen(const char* pszURL);
//打开录像通道
EN_API long CC MOBOTIX_RecordChannelOpen2(const char* pszURL,unsigned long ulHardDecodeChannel);
//关闭录像通道
EN_API BOOL CC MOBOTIX_RecordChannelClose(long lChannelHandle);

//查询录像记录
EN_API long  CC MOBOTIX_FindFile(long lChannelHandle,DWORD dwFileType,
    RecordTime* pStartTime, RecordTime* pStopTime,bool bQueryAllChannel);
//返回下一个录像记录
EN_API RecordFileInfo*  CC MOBOTIX_FindNextFile(long lFindHandle);
//关闭本次查询
EN_API void  CC MOBOTIX_FindClose(long lFindHandle);
//按文件名称开始播放录像
EN_API BOOL  CC MOBOTIX_StartPlay(long lFindHandle,
                                       RecordFileInfo* pRecordFileInfo,
                                       HWND hWnd);
//按时间段开始播放录像
EN_API BOOL  CC MOBOTIX_StartPlay2(long lFindHandle,
                                       RecordTime* pStartTime,
                                       RecordTime* pStopTime,
                                       HWND hWnd);

//按文件名开始播放录像   added in 20080528
EN_API BOOL  CC MOBOTIX_StartPlay3(long lFindHandle,
    const char* pszFileName,HWND hWnd);
//停止播放录像
EN_API BOOL  CC MOBOTIX_StopPlay(long lFindHandle);
//暂停播放录像
EN_API BOOL CC MOBOTIX_PausePlay(long lFindHandle);
//恢复播放录像
EN_API BOOL CC MOBOTIX_ResumePlay(long lFindHandle);
//快速播放录像
EN_API BOOL CC MOBOTIX_FastPlay(long lFindHandle);
//向后快速播放录像
EN_API BOOL CC MOBOTIX_SlowPlay(long lFindHandle);
//单帧播放录像
EN_API BOOL CC MOBOTIX_OneByOnePlay(long lFindHandle);
//返回录像文件时间
EN_API long CC MOBOTIX_GetFileTime(long lFindHandle);
//返回录像文件已播放时间
EN_API long CC MOBOTIX_GetPlayedTime(long lFindHandle);
//返回录像文件帧数
EN_API long CC MOBOTIX_GetFileFrames(long lFindHandle);
//返回录像文件已播放帧数
EN_API long CC MOBOTIX_GetPlayedFrames(long lFindHandle);
//返回录像文件播放进度
EN_API float CC MOBOTIX_GetPlayPos(long lFindHandle);
//设置录像文件播放进度
EN_API BOOL CC MOBOTIX_SetPlayPos(long lFindHandle,float fPlayPos);
//恢复Step后录像文件播放
EN_API BOOL CC MOBOTIX_StepResumePlay(long lFindHandle);
//调节音量
EN_API BOOL CC MOBOTIX_SetVolume(long lFindHandle,long lVolume);
//播放声音
EN_API BOOL CC MOBOTIX_PlaySound(long lFindHandle);
//停止播放声音
EN_API BOOL CC MOBOTIX_StopSound(long lFindHandle);
//开始下载录像
EN_API BOOL CC MOBOTIX_StartDownloadFile(long lFindHandle,
                                              RecordFileInfo* pRecordFileInfo,
                                              char* pszSavedFileName);
//录像文件下载进度
EN_API int CC MOBOTIX_GetDownloadPos(long lFindHandle);
//停止下载录像文件
EN_API BOOL CC MOBOTIX_StopDownloadFile(long lFindHandle);

//启动不进行录像查询，直接按文件名称回放录像功能            //add 20080528
EN_API long CC MOBOTIX_RecordPlay(long lChannelHandle);

//停止不进行录像查询，直接按文件名称回放录像功能            //add 20080528
EN_API void CC MOBOTIX_RecordStop(long lFindHandle);

//根据设备通道句柄返回设备连接句柄
EN_API long CC MOBOTIX_GetUserID(long lChannelHandle);

//是否设备自动校时功能
EN_API void CC MOBOTIX_SetAjustTime(BOOL bEnabled);

//设置转发模式下回放前端设备录像文件是否采用转发模式，
//缺省采用直连模式访问、回放录像  added in 20080220
EN_API void CC MOBOTIX_SetRecordPlayWithProxy(BOOL bEnabled);

//返回录像状态，1表示正在录像，0表示未录像，-1返回状态失败
EN_API long CC MOBOTIX_GetRecordState(long lChannelHandle);

//设置音量，范围0-0xffff
EN_API BOOL CC MOBOTIX_SetAudioVolume(long lChannelHandle,
                                           long lVolume);

//设置该通道视频参数
//亮度值，范围在0-255；
//对比度值：范围在0-255；
//饱和度值：范围在0-255；
//色度值：范围在0-255；
EN_API BOOL  CC MOBOTIX_SetVideoParam(long lChannelHandle,
                                           int iBrightness,
                                           int iContrast,
                                           int iSaturation,
                                           int iHue);

//获取该通道视频参数
EN_API BOOL  CC MOBOTIX_GetVideoParam(long lChannelHandle,
                                           int* piBrightness,
                                           int* piContrast,
                                           int* piSaturation,
                                           int* piHue);

//切换画面分割器
EN_API BOOL  CC MOBOTIX_Switch_Camera(long lChannelHandle,
                                           long lCmdID);

//切换画面分割器，支持1－16视频输入通道通过（1－16）任意视频输出通道切换输出视频，
//支持画中画、4、9、12、16画面通过（1－16）任意视频输出通道输出视频
//added in 20080222
EN_API BOOL  CC MOBOTIX_Switch_Camera2(long lChannelHandle,long lCmdID,
        long lVideoOutputChannel);

//返回解码通道数量  added in 20070716
EN_API unsigned long CC MOBOTIX_GetDecodeChannelCount();

//返回显示通道数量  added in 20070716
EN_API unsigned long CC MOBOTIX_GetDisplayChannelCount();

//划分显示区域
EN_API BOOL __stdcall MOBOTIX_SetupDisplayRegion(unsigned long ulDisplayChannel,
                                                unsigned long ulDisplayRegionCount);

//清空显示区域
EN_API BOOL __stdcall MOBOTIX_ClearDisplayRegion(unsigned long ulDisplayChannel);

//设置一路解码通道视频输出到一块电视上
//视频通道句柄      关联一个解码通道
//ulPort            解码端口，0或1，可任意选其一
//bOpen             打开或关闭
//ulDisplayChannel  输出通道，从1开始
//ulDisplayRegion   显示区域，从1开始，最大为16
EN_API BOOL CC MOBOTIX_SetVideoOutput(long lChannelHandle,
                                           unsigned long ulPort,
                                           BOOL bOpen,
                                           unsigned long ulDisplayChannel,
                                           unsigned long ulDisplayRegion,
                                           unsigned long ulDisplayRegionCount);

//设置一路解码通道音频输出到一块电视上
//视频通道句柄      关联一个解码通道
//bOpen             打开或关闭
//ulDisplayChannel  输出通道，从1开始
EN_API BOOL CC MOBOTIX_SetAudioOutput(long lChannelHandle,
                                           BOOL bOpen,
                                           unsigned long ulDisplayChannel);

//设置输出模拟视频亮度（0－255）
EN_API BOOL CC MOBOTIX_SetDisplayVideoBrightness(unsigned long ulDisplayChannel,
        long lBrightness);

//设置输出模拟视频制式
EN_API BOOL CC MOBOTIX_SetDisplayStandard(unsigned long ulDisplayChannel,
                                               VideoStandard vsVideoStandard);

//刷新PC屏幕上所有解码通道的视频画面
EN_API BOOL CC MOBOTIX_RefreshSurface();

//刷新PC屏幕上对应指定解码通道的区域视频画面
EN_API BOOL CC MOBOTIX_RefreshSurfaceRegion(long lChannelHandle,
                                                 BOOL bFullScreen);

//返回通道状态   addded in 20080523
EN_API BOOL CC MOBOTIX_GetChannelStatus(long lChannelHandle,ChannelStatus* pChannelStatus);

//added by ligen for mobotix in 20091020
//发送mobotix摄像机虚拟PTZ命令
EN_API BOOL CC MOBOTIX_VPTZControl(VPTZCommandTargetType ttType,
                                   long lChannelHandle,
                                   long lCommand,
                                   long lParam1,
                                   long lParam2);
//end


#endif
