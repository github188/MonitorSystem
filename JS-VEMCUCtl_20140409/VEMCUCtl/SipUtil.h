#ifndef __SIPUTIL_H__
#define __SIPUTIL_H__

#include ".\include\IWLSipAPI.h"
#include ".\include\IWLSipRegAPI.h"
#include ".\include\IWLSipTrAPI.h"
#include ".\include\IWLSipSdpAPI.h"
#include ".\include\IWLSipCallAPI.h"
#include ".\include\IWLPortAPI.h"
#include ".\include\IWLSipEventAPI.h"

#define SIP_CHANNEL_MAX_NUM 65535

static long read_num = 0;
static long write_num = 0;
static long recvrtp_num = 0;
static long sendrtp_num = 0;

//////////////////////////////////////////////////////////////////////////
// RT栈处理函数

//注册信息(每个)
typedef struct __t_App_RegClient_Info  
{
	int			reg_chid;						//注册的通道号
	int			reg_id;							//注册的ID号
	int			reg_state;					//注册的状态
	int			reg_oldstate;				//注册旧的状态
	int			reg_expires;				    //注册的有效期
	int			reg_ttl;							//注册的剩余生存期
	char		    reg_server[50];			// 注册的服务器地址
	int			reg_serverport;			// 注册的服务器端口号
	char		    reg_user[50];				// 注册的用户名称
	char		    reg_password[50];		// 注册的用户口令
}T_App_RegClient_Info;

//定义了一组SIP通道
typedef struct __t_App_Sip_Channel
{
	int			nUserChannelID;				//相关的坐席通道的ID
	int			nSipCallID;							//SIP侧CID
	int			nAppSipCallState;				//SIP侧呼叫应用状态
	char		    szSipCaller[50];					//SIP侧呼叫的主叫
	char		    szSipCallee[50];					//SIP侧呼叫的被叫
	int			nDevType;							//通道上类型
}T_App_Sip_Channel; 

//事务信息(每个)
typedef struct __t_App_Transaction_Info
{
	int		tran_chid;								//事务通道号
	int		tran_id;								    //事务ID号
	int		tran_state;								//事务状态
	int		tran_oldstate;							//事务旧的状态
	int		tran_direction;						//事务的方向
	char	    tran_caller[50];						//事务主叫者
	char	    tran_callee[50];						//事务被叫者
	char	    tran_last_msg[50];					//消息内容
} T_App_Transaction_Info;

typedef struct __t_App_Subscribe_Info
{
	int		iSID;									        // 订阅ID号
	char	    stationcode[10][20];			    // 订阅信息中各个站点的编码
	int		istationnum;							// 订阅的站点个数
	int		istate;									    // 订阅的状态
}T_App_Subscribe_Info;

typedef struct __t_App_Msg_List //消息结构
{
	__t_App_Msg_List*	msg_next;
	char				msg_content[50];
} T_App_Msg_List;

typedef enum
{
	PCUM,
	G711,
	G723,
	G729,
	H264
}RTP_CODEC;

typedef struct _RTP_ACTION
{
	void(*ReadFromDev)(char* playload,int len,void* args);
	void(*WriteToDev)(char* playload,int len,void* args);
}RTP_ACTION;

typedef struct _THREAD_ARGS
{
	int call_id;
	RTP_CODEC codec;
	RTP_ACTION action;
}THREAD_ARGS;

//SDP协商结构
typedef struct __t_RtSdpAudioNeg
{
	int		nPayloadNumber;
	int		nPort;
	char	    szAddress[56];
	char  	szCalleeName[56];
} T_RtSdpAudioNeg;

//用于RTP的端口获得
typedef struct __t_RtpPortRange
{
	__t_RtpPortRange();
	~__t_RtpPortRange();

	unsigned short 	GetPort(unsigned increment); //获得空的端口
	int				GetNumPorts() const;  //获得区间的总值
	int				GetMaxPort(); //获得最大的值
	void			LoadConfig(unsigned short minValue = 1024, unsigned short maxValue = 65535); //配置端口区间

private:
	__t_RtpPortRange(const __t_RtpPortRange&);
	__t_RtpPortRange& operator=(const __t_RtpPortRange&);	
	unsigned short	port, minport, maxport;

} T_RtpPortRange;

/*************************************协议栈相关函数***********************************/
// 初始化SIP通道
void AppInitSipChannel(void);

// 启动SIP协议栈
int	AppStartSipStack();

// 停止SIP协议栈
int	AppStopSipStack();

/***************************************注册相关函数***********************************/
// 发起新注册
int	AppMakeReg(char* pszServer,char* pszUsername,char* pszPassword,int nExpires,char* pszUserAgent);
// 取消注册
int	AppCancelReg();
// 终止注册
int	AppTerminateReg();

/***************************************呼叫相关函数***********************************/

// 获得一个呼叫状态是空闲的UAC通道
int AppGetIdleUacID();

// 是否是有效的ID
BOOL AppUacUasIDIsValid(int callid);

// 清除通道
BOOL AppClearUacUasID(int callid);

/***************************************事务相关函数***********************************/
// 发起新事务
int	AppMakeTrans(char* pszDestAddr, E_RtMethodType eMethodType, char* pszBody, int iWinID);

// 获取空闲的事务通道
int	AppGetIdleTransCh();

/**************************************************************
*	 
*   注册的Callback接收处理者
*
*************************************************************/
class CSGSipRegisterClient : public IRtRegClientCBSink
{
public:
	void StateChangeEvent( 
		IN unsigned int					nRID,
		IN E_RtRegClientState			eState,
		IN E_RtRegClientStateReason		eReason,
		IN void*						pAppData);
};


/**************************************************************
*	 
*   呼叫的Callback接收处理者
*
*************************************************************/

class CSGSipCall : public IRtCallCBSink
{
public:
	virtual	void StateChangeEvent(
		IN unsigned int				nCID,
		IN E_RtCallState			eState,
		IN E_RtCallStateReason	 eReason,
		IN void*					pAppData); 

	virtual	void RecvOutboundDtmfEvent( //带外DTMF收到
		IN unsigned int		nCID,
		IN char				nDtmfChar,
		IN void*			pAppData);

	virtual void JRtpRecvMediaData(
		OUT int						nCID,
		OUT void*					pData,
		OUT int						nDataLen,
		OUT E_RtMediaType	nMediaType,
		OUT unsigned int		nMediaPayload,
		OUT bool					bMark,
		OUT unsigned int		nTimestamp,
		OUT unsigned int		nSeqNo);

	virtual void JRtpRecvRtcpData(
		OUT int						nCID,
		OUT E_RtMediaType			eMediaType,
		OUT E_RtRtcpPacketType	eType,
		OUT unsigned int			dwSecond,
		OUT const char*				pSenderAddress,
		OUT const T_RtQOS4*	pQos);

	virtual void RecvReferEvent( //Refer事件收到
		IN unsigned int				nOwnerCID,
		IN RtSubsHandle				hSubs,
		IN char*					pReferTo,  //该内存在对象返回即销毁
		IN void*					pAppData)
	{

	}
};


/**************************************************************
*	 
*   事件的Callback接收处理者
*
*************************************************************/
class CSGSipEvent : public IRtEventCBSink
{
public:
	virtual	void StateChangeEvent(
		OUT unsigned int			nSID,
		OUT E_RtEventState			eState,
		OUT E_RtEventStateReason	eReason,
		OUT void*					pAppData);
};


/**************************************************************
*	 
*   事务的Callback接收处理者
*
*************************************************************/
class CSGSipTransaction : public IRtTransactionCBSink
{
public:
	virtual	void StateChangeEvent(
		IN unsigned int					nTID,
		IN E_RtTransactionState			eState,
		IN E_RtTransactionStateReason	eReason,
		IN void*						pAppData);
};


/**************************************全局变量声明***************************************/
extern T_RtStackConfigParam	app_StackCfg;						// 协议栈配置信息
extern T_RtStackCallBack		    app_Callback;						// 回调函数句柄

extern IRtRegClientAPI*			g_pRegClientAPI;				// 注册的处理接口函数
extern IRtCallAPI*				        g_pCallAPI;							// 呼叫的处理接口函数
extern IRtSdpAPI*				        g_pSdpAPI;							// SDP呼叫的处理接口函数
extern IRtTransactionAPI*		    g_pTransactionAPI;			// 事务的处理接口函数
extern IRtEventAPI*				    g_pEventAPI;						// 事务的API

extern T_RtSdpAudioNeg	 g_tLocalAudio;			//本端音频信息(端口不使用)

extern T_App_RegClient_Info		app_RegCh;		// 注册通道列表
extern T_App_Sip_Channel		app_UacCh[SIP_CHANNEL_MAX_NUM];		// 主叫通道列表
extern T_App_Sip_Channel		app_UasCh[SIP_CHANNEL_MAX_NUM];		// 被叫通道列表
extern T_App_Transaction_Info	app_TranCh[SIP_CHANNEL_MAX_NUM];	    // 事务通道列表
extern T_App_Subscribe_Info		app_SubCh[SIP_CHANNEL_MAX_NUM];		// 订阅通道列表

extern int						app_CurRegCh;						// 当前的注册通道
extern int						app_CurUacCh;						// 当前的主叫通道
extern int						app_CurUasCh;						// 当前的被叫通道
extern int						app_CurTranCh;						// 当前的事务通道


#endif /*__SIPUTIL_H__*/

