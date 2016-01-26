#ifndef __IWL_SIP_STACK_SIPEVENTAPI_H_
#define __IWL_SIP_STACK_SIPEVENTAPI_H_

typedef enum  __e_RtEventState
{
	E_RT_EVENT_STATE_UNDEFINED				= -1,	//无效
	E_RT_EVENT_STATE_IDLE					= 0,	//空闲
	E_RT_EVENT_STATE_SUER_PROCEEDING		= 1,	//订阅已发送(订阅者)
	E_RT_EVENT_STATE_SUER_EXPIRE_ALERT		= 2,	//订阅即将到期(订阅者)
	E_RT_EVENT_STATE_SUER_NOTIFY_RCVD		= 3,	//收到事件(订阅者)
	E_RT_EVENT_STATE_PUER_RCVD				= 5,	//订阅请求收到(发布者)
	E_RT_EVENT_STATE_PUER_WAIT_PROCESS		= 6,	//等待处理订阅请求(发布者)
	E_RT_EVENT_STATE_PUER_REFRESH_WAIT_PROCESS = 7,	//等待处理刷新订阅请求(发布者)
	E_RT_EVENT_STATE_ACTIVATED				= 9,	//订阅激活(订阅者+发布者)
	E_RT_EVENT_STATE_TERMINATED				= 10,	//订阅结束(订阅者+发布者)
} E_RtEventState;

typedef enum __e_RtEventStateReason
{	
	E_RT_EVENT_REASON_UNDEFINED				= -1,	
	E_RT_EVENT_REASON_NORMAL				= 0,
	E_RT_EVENT_REASON_REMOTE				= 1,	//远端结束
	E_RT_EVENT_REASON_LOCAL					= 2,	//本端结束
	E_RT_EVENT_REASON_UNKNOW				= 3,	//不明确原因
} E_RtEventStateReason;

typedef enum __e_RtEventSubsState
{	
	E_RT_EVENT_SUBSSTATE_UNDEFINED			= -1,
	E_RT_EVENT_SUBSSTATE_ACTIVE				= 1,	//订阅活动
	E_RT_EVENT_SUBSSTATE_PENDING			= 2,	//订阅等待处理
	E_RT_EVENT_SUBSSTATE_TERMINATED			= 3,	//订阅终止
} E_RtEventSubsState;


class RTAPI IRtEventCBSink
{
public:
	virtual	void StateChangeEvent(
		OUT unsigned int			nSID,
		OUT E_RtEventState			eState,
		OUT E_RtEventStateReason	eReason,
		OUT void*					pAppData
		) = 0;

	virtual ~IRtEventCBSink() = 0;
};

class RTAPI IRtEventAPI
{
public:
	// ***** 订 阅 操 作 函 数 ****** //
	
	virtual int  ProduceSID(
		INOUT int*				pnSID
		) = 0;

	virtual int MakeSubs( //订阅者发起一个新订阅
		IN  int					nSID,
		IN  const char*			pszDestUri,			//发布者URI,对端的账号或IP地址
		IN  const char*			pszBody,			//请求的消息体
		IN  unsigned int		nBodyLen,			//消息体的长度
		IN  char*				pszLocalName,		//强制更改From头的User(NULL时:已注册为本端账号,未注册则为Unknown)
		IN  unsigned int		nExpireSecond,		//注册有效期,单位秒
		IN  const char*			pszEvent,			//订阅的事件类型,建议格式:eventPackageX.eventTemplateY
		IN  void*				pAppData,			//应用层APP数据
		IN  char*				pszBodyType,		//消息体类型,格式是"[主类型]/[子类型]",如果空且消息体不空则设置为text/plain
		IN	char*				pszFromHost = NULL,	//强制更改From头的Host (缺省NULL时: 已注册则为CMS地址,未注册则为本端地址)
		IN	char*				pszToHost = NULL	//强制更改To头的Host   (缺省NULL时: 已注册则为CMS地址,未注册则为pszDestUri中Host地址)
		) = 0; 

	virtual int RefreshSubs( //订阅者刷新订阅(在E_RT_EVENT_STATE_SUER_EXPIRE_ALERT下调用)
		IN  int					nSID,
		IN  unsigned int		nExpireSecond,
		IN  const char*			pszBody,
		IN  unsigned int		nBodyLen,
		IN  char*				pszBodyType			//消息体类型,格式是"[主类型]/[子类型]",如果空且消息体不空则设置为text/plain
		) = 0; 

	virtual int Unsubscribe( //订阅者取消订阅
		IN  int					nSID
		) = 0;

	virtual int AcceptSubs( //发布者接受订阅
		IN  int					nSID, 
		IN  int					nRefreshTime = -1, //订阅有效期,等于-1表示以SUBSCRIBE请求中时间为主	
		IN  const char*			pszBody = NULL,    //响应的消息体
		IN  unsigned int		nBodyLen = 0,	   //消息体长度
		IN  char*				pszBodyType = NULL //消息体类型,格式是"[主类型]/[子类型]",如果空且消息体不空则设置为text/plain
		) = 0; 

	virtual int RejectSubs( //发布者拒绝订阅
		IN  int					nSID, 
		IN  unsigned int		nCode,
		IN  const char*			pszBody = NULL,
		IN  const int			nBodyLen = 0,
		IN  char*				pszBodyType = NULL //消息体类型,格式是"[主类型]/[子类型]",如果空且消息体不空则设置为text/plain
		) = 0; 

	virtual int TerminatedSubs( //订阅结束(会根据当前状态决定如何处理)
		IN  int					nSID
		) = 0;

	/*
	 * 仅允许在 E_RT_EVENT_STATE_PUER_WAIT_PROCESS 和 E_RT_EVENT_STATE_PUER_REFRESH_WAIT_PROCESS 状态下调用
	 */
	virtual int GetRemoteBody4Subs( //获得Subs过程的BODY内容
		IN    int				nSID,
		INOUT char*				pBuffer,
		IN    int				nBufferLen,
		INOUT int*				pActualLen
		) = 0;


	// ***** 通 知 操 作 函 数 ****** //

	virtual int MakeNotify( //发布者通知事件产生
		IN  int					nSID,
		IN  const char*			pszBody, 
		IN  unsigned int		nBodyLen,
		IN  E_RtEventSubsState	eSubstate,
		IN  char*				pszBodyType //消息体类型,格式是"[主类型]/[子类型]",如果空且消息体不空则设置为text/plain
		) = 0; 

	/*
	 * 仅允许在E_RT_EVENT_STATE_SUER_NOTIFY_RCVD状态下调用
	 */
	virtual int GetRemoteBody4Notify( //获得Notify过程的BODY内容
		IN    int				nSID,
		INOUT char*				pBuffer,
		IN    int				nBufferLen,
		INOUT int*				pActualLen
		) = 0;


	// ***** 通 用 操 作 函 数 ****** //

	virtual int GetAppData( //获得应用层数据
		IN	  int				nSID,
		OUT   void**			ppAppData
		) = 0;
		
	virtual int SetAppData( //设置应用层的数据
		IN	  int				nSID,
		IN	  void*				pAppData
		) = 0;
	
	virtual int GetSuerInfo( //获得订阅者信息
		IN    int				nSID,
		INOUT T_RtSipUrlInfo*	pSuer
		) = 0;
	
	virtual int GetPuerInfo( //获得发布者信息
		IN    int				nSID,
		INOUT T_RtSipUrlInfo*	pPuer
		) = 0;

	virtual int GetCurrentState( //获取当前状态
		IN	  int				nSID,
		INOUT E_RtEventState*	pState
		) = 0;

	virtual int GetExpiresVal( //获得订阅有效期
		IN	  int				nSID,
		INOUT int*				pnExpires
		) = 0;

	virtual ~IRtEventAPI() = 0;
};

#endif
