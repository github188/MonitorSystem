#ifndef _SIP_CALL_H_
#define _SIP_CALL_H_


typedef struct _VIDEO_SIP_CALL_SERVER_INFO_STRUCT_
{
	int   nType;//1:通过南瑞平台呼叫,2:直接呼叫前端设备,3:通过南瑞平台重连,4:通过直接呼叫前端设备重连
	char szName[256];
	char szCode[64];
	char szStation[256];
	int    nDecodeTag;
	int    nScreenId;
	HTREEITEM hItem;
	int    nStatus;
	int    nStreamless;
	char szReason[256];
	_T_NODE_INFO server_node_info;
}VIDEO_SIP_CALL_SERVER_INFO;

typedef struct _VIDEO_SIP_CALL_PRESET_INFO_STRUCT_
{
	int   nType;//1:通过南瑞平台的一般主动呼叫,2:通过南瑞平台带预置位呼叫,3:直接主动呼叫前端设备,4:带预置位直接呼叫前端设备
	char szName[256];
	char szCode[64];
	char szStation[256];
	int    nDecodeTag;
	int    nScreenId;
	HTREEITEM hItem;
	int    nStatus;
	int    nStreamless;
	char szReason[256];
	_T_NODE_PRESET_INFO preset_node_info;
}VIDEO_SIP_CALL_PRESET_INFO;


typedef struct _SIP_STATE_HANDLER_INFO_
{
	int nCID;
	char szCallerNum[64];
	char szCalleeNum[64];
	int   nDirection;
	char Sps[512];
	int  nSpsLen;
	int nReason;
	int nVideoPayload;
	int nAudioPayload;
	void *pAppData;
}SIP_STATE_HANDLER_INFO;

//主动视频呼叫
void ThreadMakeCallCameraServer(void *arg);

//主动视频呼叫并转到预置位
void ThreadMakeCallCameraPreset(void *arg);

//联动视频呼叫
void ThreadVideoLinkageMakeCall(void *arg);


//呼叫状态建立
void ThreadSipStateHandlerTalking(void *arg);

//呼叫结束
void ThreadSipStateHandlerTerminate(void *arg);

//主动结束呼叫
void ThreadStopCurrentVideo(void *arg);

//主动结束全部呼叫
void ThreadStopAllVideo(void *arg);

//主动结束直连设备呼叫
void ThreadStopDirectDeviceVideo(void *arg);

//主动结束平台呼叫
void ThreadStopPlatformVideoByCallId(void *arg);




#endif