#define MY_FILENAME_MAXLEN		512
#define	MY_TIME_MAXLEN			32

typedef struct _dvr_info{
	unsigned short dvrid;
	unsigned short dvrtype;
	LONG loginid;
	char ip[16];
	unsigned short port;
	char user[20];
	char password[20];
}DVRINFO,*PDVRINFO;

typedef struct _camera_info{
	int id;
	unsigned short dvrid;
	unsigned short channel;
	LONG playid;
	unsigned short dvrtype;
}CAMERAINFO,*PCAMERAINFO;

typedef struct _local_file_info{
	unsigned short dvrid;
	unsigned short channel;
	unsigned short dvrtype;
	LONG loginid;
	LONG getfileid;
	char ip[16];
	unsigned short port;
	char user[20];
	char password[20];
}LOCALFILEINFO,*PLOCALFILEINFO;

typedef struct _play_local_file_info{
	LONG id;
	LONG port;
	unsigned short dvrtype;
}PLAYLOCALFILEINFO,*PPLAYLOCALFILEINFO;

typedef struct _playsdk_time{
	unsigned long year;
	unsigned long month;
	unsigned long day;
	unsigned long hour;
	unsigned long minute;
	unsigned long second;
}PLAYSDKTIME,*PPLAYSDKTIME;

typedef struct _playrecord_item {
	char	FileName[MY_FILENAME_MAXLEN];
	char	BeginTime[MY_TIME_MAXLEN];
	char	EndTime[MY_TIME_MAXLEN];
	int		size;
}PLAYRECORDITEM,*PPLAYRECORDITEM;

typedef struct _openvideo_param{
	char ip[16];
	unsigned short port;
	char user[20]; 
	char password[20];
	unsigned short dvrtype;
	unsigned short dvrid;
	unsigned short channel;
	int id;
	HWND playwnd;
}OPENVIDEOPARAM,*POPENVIDEOPARAM;

typedef struct _capturepicture_param{
	int id;
	char picfilename[260];
}CAPTUREPICTUREPARAM,*PCAPTUREPICTUREPARAM;

typedef struct _videorecord_param{
	int id;
	char recordfilename[260];
}VIDEORECORDPARAM,*PVIDEORECORDPARAM;

typedef int (*SdkLib_GetProtocolID)();
typedef bool (*SdkLib_InitDvr)();
typedef void (*SdkLib_ExitDvr)();
typedef LONG (*SdkLib_LoginDvr)(char* ip , unsigned short port , char* user ,char* password);
typedef void (*SdkLib_LogoutDvr)(LONG loginid);
typedef LONG (*SdkLib_OpenDvrVideo)(LONG longid, unsigned short channel , HWND playhwnd);
typedef void (*SdkLib_CloseDvrVideo)(LONG playid);
typedef bool (*SdkLib_DvrPtzControl)(LONG longid, unsigned short channel, unsigned short control,unsigned short speed);
typedef bool (*SdkLib_DvrPtzPresetControl)(LONG longid, unsigned short channel, unsigned short control);
typedef bool (*SdkLib_GetRecordFileInfo)(LONG longid, unsigned short channel,PLAYSDKTIME start,PLAYSDKTIME stop,void** info,int* num);
typedef bool (*SdkLib_FreeRecordFileInfo)(void* info);
typedef LONG (*SdkLib_StartPlayBack)(LONG longid, PLAYRECORDITEM filename , HWND playhwnd);
typedef bool (*SdkLib_StopPlayBack)(LONG playid);
typedef bool (*SdkLib_PausePlayBack)(LONG playid);
typedef bool (*SdkLib_ReStartPlayBack)(LONG playid);
typedef bool (*SdkLib_FastPlayBack)(LONG playid);
typedef bool (*SdkLib_SlowPlayBack)(LONG playid);
typedef bool (*SdkLib_SetPosPlayBack)(LONG playid,int pos);
typedef LONG (*SdkLib_GetTime)(LONG playid);
typedef LONG (*SdkLib_GetTotalTime)(LONG playid);
typedef bool (*SdkLib_GetDevState)(LONG loginid);
typedef LONG (*SdkLib_StartDownLoadLocalFile)(LONG longid, PLAYRECORDITEM filename,char* file);
typedef void (*SdkLib_StopDownLoadLocalFile)(LONG id);
typedef LONG (*SdkLib_GetDownLoadLocalFilePos)(LONG id);
typedef LONG (*SdkLib_StartPlayLocalFile)(char* file,HWND playhwnd);
typedef void (*SdkLib_StopPlayLocalFile)(LONG id);
typedef LONG (*SdkLib_GetPlayLocalFilePos)(LONG id);
typedef bool (*SdkLib_PausePlayLocalFile)(LONG id);
typedef bool (*SdkLib_ReStartPlayLocalFile)(LONG id);
typedef bool (*SdkLib_FastPlayLocalFile)(LONG id);
typedef bool (*SdkLib_SlowPlayLocalFile)(LONG id);
typedef bool (*SdkLib_SetPosPlayLocalFile)(LONG id,int pos);
typedef bool (*SdkLib_CaptureLocalPicture)(int id,char *szPicFileName);
typedef bool (*SdkLib_StartRecordLocalFile)(int id,char *szRecordFileName);
typedef bool (*SdkLib_StopRecordLocalFile)(int id);


typedef struct _dev_playsdklib_node{
	HMODULE hDevPlaySdkLibModule;
	int		iDevPlaySdkID;
	char	m_sDevPlaySdkLibFile[MAX_PATH];
	int (*GetProtocolID)();
	bool (*InitDvr)();
	void (*ExitDvr)();
	LONG (*LoginDvr)(char* ip , unsigned short port , char* user ,char* password);
	void (*LogoutDvr)(LONG loginid);
	LONG (*OpenDvrVideo)(LONG longid, unsigned short channel , HWND playhwnd);
	void (*CloseDvrVideo)(LONG playid);
	bool (*DvrPtzControl)(LONG longid, unsigned short channel, unsigned short control, unsigned short speed);
//	bool (*DvrPtzPresetControl)(LONG longid, unsigned short channel, unsigned short control);
	bool (*GetRecordFileInfo)(LONG longid, unsigned short channel,PLAYSDKTIME start,PLAYSDKTIME stop,void** info,int* num);
	bool (*FreeRecordFileInfo)(void* info);
	LONG (*StartPlayBack)(LONG longid, PLAYRECORDITEM filename , HWND playhwnd);
	bool (*StopPlayBack)(LONG playid);
	bool (*PausePlayBack)(LONG playid);
	bool (*ReStartPlayBack)(LONG playid);
	bool (*FastPlayBack)(LONG playid);
	bool (*SlowPlayBack)(LONG playid);
	bool (*SetPosPlayBack)(LONG playid,int pos);
	LONG (*GetTime)(LONG playid);
	LONG (*GetTotalTime)(LONG playid);
	bool (*GetDevState)(LONG loginid);
	LONG (*StartDownLoadLocalFile)(LONG longid, PLAYRECORDITEM filename,char* file);
	void (*StopDownLoadLocalFile)(LONG id);
	LONG (*GetDownLoadLocalFilePos)(LONG id);
	LONG (*StartPlayLocalFile)(char* file,HWND playhwnd);
	void (*StopPlayLocalFile)(LONG id);
	LONG (*GetPlayLocalFilePos)(LONG id);
	bool (*PausePlayLocalFile)(LONG id);
	bool (*ReStartPlayLocalFile)(LONG id);
	bool (*FastPlayLocalFile)(LONG id);
	bool (*SlowPlayLocalFile)(LONG id);
	bool (*SetPosPlayLocalFile)(LONG id,int pos);
	bool (*CaptureLocalPicture)(int id,char *szPicFileName);
	bool (*StartRecordLocalFile)(int id,char *szPicFileName);
	bool (*StopRecordLocalFile)(int id);
}DEVPLAYSDKLIB,*PDEVPLAYSDKLIB;