#ifdef DEVPLAYVIDEOSDK_EXPORTS
#define DEVPLAYVIDEOSDKLIB __declspec(dllexport)
#else
#define DEVPLAYVIDEOSDKLIB __declspec(dllimport)
#endif

#define	DEVTYPE_HK					110		//海康
#define	DEVTYPE_DH					111		//大华
#define	DEVTYPE_DL					112		//大立
#define DEVTYPE_MOBOTIX				113		//mobotix
#define DEVTYPE_AXIS				114		//AXIS

#define OPENVIDEO_SUCCESS			0		// 打开视频成功
#define OPENVIDEO_LOADDLLERROR		1		// 加载播放SDK失败
#define OPENVIDEO_FINDDVRTYPEERROR	2		// 没有找到该类型解码库
#define OPENVIDEO_LOGINDVRERROR		3		// 登陆DVR失败
#define OPENVIDEO_PLAYERROR			4		// 打开视频失败

#define CLOSEIRISSTOP		0x0101	//光圈关停止
#define CLOSEIRIS			0x0102	//光圈关
#define OPENIRIS			0x0103	//光圈开
#define OPENIRISSTOP		0x0104	//光圈开停止
#define FOCUSNEARSTOP		0x0201	//近聚焦停止
#define FOCUSNEAR			0x0202	//近聚焦
#define FOCUSFARSTOP		0x0203	//远聚焦停止
#define FOCUSFAR			0x0204  //远聚焦
#define ZOOMINSTOP			0x0301  //缩小停止
#define ZOOMIN				0x0302	//缩小
#define ZOOMOUTSTOP			0x0303	//放大停止
#define ZOOMOUT				0x0304	//放大
#define TILTUPSTOP			0x0401	//向上停止
#define	TILTUP				0x0402	//向上
#define TILTDOWNSTOP		0x0403	//向下停止
#define TILTDOWN			0x0404  //向下
#define PANRIGHTSTOP		0x0501	//右转停止
#define PANRIGHT			0x0502	//右转
#define PANLEFTSTOP			0x0503	//左转停止
#define PANLEFT				0x0504	//左转
#define SETPRESET			0x0601	//预置位保存
#define GOTOPRESET			0x0602	//预置位调用
#define CLEPRESET			0x0603	//预置位删除
#define UPLEFTSTOP			0x0701	//左上方向运动停止
#define UPLEFT				0x0702	//左上方向运动
#define DOWNLEFTSTOP		0x0703	//左下方向运动停止
#define DOWNLEFT			0x0704	//左下方向运动
#define UPRIGHTSTOP			0x0801	//右上方向运动停止
#define UPRIGHT				0x0802	//右上方向运动
#define DOWNRIGHTSTOP		0x0803	//右下方向运动停止
#define DOWNRIGHT			0x0804	//右下方向运动

#define FILENAME_MAXLEN		512
#define	TIME_MAXLEN			32

typedef struct _sdk_time{
	unsigned long year;
	unsigned long month;
	unsigned long day;
	unsigned long hour;
	unsigned long minute;
	unsigned long second;
}SDKTIME,*PSDKTIME;

typedef struct _record_item {
	char	FileName[FILENAME_MAXLEN];
	char	BeginTime[TIME_MAXLEN];
	char	EndTime[TIME_MAXLEN];
	int		size;
}RECORDITEM,*PRECORDITEM;

typedef int (*CALLBACKGETDEVSTATE) (unsigned short dvrid,bool state);
typedef int (*CALLBACKGETPOS) (unsigned short dvrid,unsigned short channel,int pos);
typedef int (*CALLBACKOPENVIDEO) (int  id, int nresult);
typedef int (*CALLBACKCLOSEVIDEO)(int id);
typedef int (*CALLBACKGETFILEPOS) (unsigned short dvrid,unsigned short channel,int pos);
typedef int (*CALLBACKPLAYFILEPOS) (LONG id,int pos);


#ifdef __cplusplus
extern "C" {
#endif
	DEVPLAYVIDEOSDKLIB  bool	Init_DevSdk();
	DEVPLAYVIDEOSDKLIB  bool	Exit_DevSdk(); 
	DEVPLAYVIDEOSDKLIB  int 	GetID_DevSdk(); 
	DEVPLAYVIDEOSDKLIB  int		OpenVideo_DevSdk(char* ip,unsigned short port, char* user, char* password,unsigned short dvrtype,unsigned short dvrid,unsigned short channel,int id ,HWND playwnd);
	DEVPLAYVIDEOSDKLIB  void	 CloseVideo_DevSdk(int id);
	DEVPLAYVIDEOSDKLIB  bool	 DvrPtzControl_Devsdk(char* ip,unsigned short port, char* user, char* password,unsigned short dvrtype,unsigned short dvrid,unsigned short channel,unsigned short control,unsigned short speed);
	DEVPLAYVIDEOSDKLIB  bool	 GetRecordFileInfo_DevSdk(char* ip,unsigned short port, char* user, char* password,unsigned short dvrtype,unsigned short dvrid,unsigned short channel,SDKTIME starttime,SDKTIME stoptime,void** info,int* num);
	DEVPLAYVIDEOSDKLIB  bool FreeRecordFileInfo_DevSdk(unsigned short dvrtype,void* info);
	DEVPLAYVIDEOSDKLIB  bool	 StartPlayBack_DevSdk(char* ip,unsigned short port, char* user, char* password,unsigned short dvrtype,unsigned short dvrid,unsigned short channel,RECORDITEM info,HWND playwnd);
	DEVPLAYVIDEOSDKLIB  void	 StopPlayBack_DevSdk(unsigned short dvrid,unsigned short channel);
	DEVPLAYVIDEOSDKLIB  bool	 PausePlayBack_DevSdk(unsigned short dvrid,unsigned short channel);
	DEVPLAYVIDEOSDKLIB  bool	 ReStartPlayBack_DevSdk(unsigned short dvrid,unsigned short channel);
	DEVPLAYVIDEOSDKLIB  bool 	FastPlayBack_DevSdk(unsigned short dvrid,unsigned short channel);
	DEVPLAYVIDEOSDKLIB  bool 	SlowPlayBack_DevSdk(unsigned short dvrid,unsigned short channel);
	DEVPLAYVIDEOSDKLIB  bool	 SetPosPlayBack_DevSdk(unsigned short dvrid,unsigned short channel,int pos);
	DEVPLAYVIDEOSDKLIB  bool    StartDownLoadFile_DevSdk(char* ip,unsigned short port, char* user, char* password,unsigned short dvrtype,unsigned short dvrid,unsigned short channel,RECORDITEM info,char* localfile);
	DEVPLAYVIDEOSDKLIB  bool    StopDownLoadFile_DevSdk(unsigned short dvrid,unsigned short channel);
	DEVPLAYVIDEOSDKLIB  LONG    StartPlayLocalFile_DevSdk(unsigned short dvrtype,char* file,HWND playhwnd);
	DEVPLAYVIDEOSDKLIB  bool    StopPlayLocalFile_DevSdk(LONG id);
	DEVPLAYVIDEOSDKLIB  bool	PausePlayLocalFile_DevSdk(LONG id);
	DEVPLAYVIDEOSDKLIB  bool	ReStartPlayLocalFile_DevSdk(LONG id);
	DEVPLAYVIDEOSDKLIB  bool	FastPlayLocalFile_DevSdk(LONG id);
	DEVPLAYVIDEOSDKLIB  bool	SlowPlayLocalFile_DevSdk(LONG id);
	DEVPLAYVIDEOSDKLIB  bool	SetPosPlayLocalFile_DevSdk(LONG id,int pos);
	DEVPLAYVIDEOSDKLIB  bool	SetCallbackGetDevState(CALLBACKGETDEVSTATE GetDevState);
	DEVPLAYVIDEOSDKLIB  bool	SetCallbackGetPlayBackPos(CALLBACKGETPOS GetPlayBackPos);
	DEVPLAYVIDEOSDKLIB  bool	SetCallbackOpenVideo(CALLBACKOPENVIDEO OpenVideo);
	DEVPLAYVIDEOSDKLIB  bool    SetCallbackCloseVideo(CALLBACKCLOSEVIDEO CloseVideo);
	DEVPLAYVIDEOSDKLIB  bool    SetCallbackGetLocalFilePos(CALLBACKGETFILEPOS GetFilePos);
	DEVPLAYVIDEOSDKLIB  bool    SetCallbackPlayLocalFilePos(CALLBACKPLAYFILEPOS GetFilePos);
	DEVPLAYVIDEOSDKLIB  bool	 CaptureLocalPicture(int id,char *szPicFileName);
	DEVPLAYVIDEOSDKLIB  bool	 StartRecordLocalFile(int id,char *szRecordFileName);
	DEVPLAYVIDEOSDKLIB  bool	 StopRecordLocalFile(int id);

#ifdef __cplusplus
}
#endif
