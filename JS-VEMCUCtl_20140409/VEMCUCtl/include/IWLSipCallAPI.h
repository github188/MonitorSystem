#ifndef __IWL_SIP_STACK_SIPCALLAPI_H_
#define __IWL_SIP_STACK_SIPCALLAPI_H_

RT_DECLARE_HANDLE(RtSubsHandle);

typedef enum  __e_RtCallState
{
	E_RT_CALL_STATE_UNDEFINED				= -1,	//无效
	E_RT_CALL_STATE_IDLE					= 0,	//空闲
	E_RT_CALL_STATE_TALKING					= 1,	//通话
	E_RT_CALL_STATE_TERMINATED				= 2,	//终止
	E_RT_CALL_STATE_INVITING				= 3,	//正在呼出	  (主叫状态)
	E_RT_CALL_STATE_PROCEEDING				= 4,	//远端正在处理(主叫状态)
	E_RT_CALL_STATE_RECV_CALL				= 5,	//收到呼入	  (被叫状态)
	E_RT_CALL_STATE_OFFERING				= 6,	//震铃		  (被叫状态)
	E_RT_CALL_STATE_WAIT_TALKING			= 7,	//等待进入通话(被叫状态)
} E_RtCallState;

typedef enum __e_RtCallStateReason
{	
	E_RT_CALL_REASON_UNDEFINED				= -1,	
	E_RT_CALL_REASON_NORMAL					= 0,	//正常的状态迁移
	E_RT_CALL_REASON_LOCAL_CANCEL			= 1,	//本地取消呼叫(主叫)
	E_RT_CALL_REASON_REMOTE_CANCEL			= 2,	//远端取消呼叫(被叫)
	E_RT_CALL_REASON_LOCAL_HANGUP			= 3,	//本地挂机
	E_RT_CALL_REASON_REMOTE_HANGUP			= 4,	//远端挂机
	E_RT_CALL_REASON_FAIL_REDIRECTED		= 5,	//3xx消息(主叫,即要求向另一地址呼叫)
	E_RT_CALL_REASON_FAIL_REJECT			= 6,	//4xx消息(主叫,即被拒绝)
	E_RT_CALL_REASON_FAIL_SERVER			= 7,	//5xx消息(主叫,即服务端故障)
	E_RT_CALL_REASON_FAIL_GLOBAL			= 8,	//6xx消息(主叫,即全局网络故障)
	E_RT_CALL_REASON_NETWORK_ERROR			= 9,	//网络故障
	E_RT_CALL_REASON_UNAUTHENTICATED		= 10,	//授权失败(主叫)
	E_RT_CALL_REASON_RECV_180				= 11,	//收到180指示回铃(主叫)
	E_RT_CALL_REASON_RECV_181				= 12,	//收到181指示呼叫在转发(主叫)
	E_RT_CALL_REASON_RECV_182				= 13,	//收到182指示呼叫被放入排队(主叫)
	E_RT_CALL_REASON_RECV_183				= 14,	//收到183指示彩铃(主叫)
	E_RT_CALL_REASON_WAIT_PICKUP_TIMEOUT	= 15,	//等待被叫摘机超时(主叫)
	E_RT_CALL_REASON_LOCAL_TIMEOUT			= 16,	//本端处理超时
	E_RT_CALL_REASON_LOCAL_FAILURE			= 17,	//本端因为某些逻辑失败(内存不够,数据错误等)
} E_RtCallStateReason;

typedef struct __t_RtSipMixerBody 
{
	 char			szMaster[56];	//主类型,如application
	 char			szSubs[56];		//从类型,如SDP
	 char			szBody[2048];	//消息体
	 unsigned int	nBodyLen;		//消息体长度
} T_RtSipMixerBody;

class RTAPI IRtCallCBSink
{
public:
	virtual	void StateChangeEvent( //状态迁移
		OUT unsigned int			nCID,
		OUT E_RtCallState			eState,
		OUT E_RtCallStateReason		eReason,
		OUT void*					pAppData
		) = 0; 

	virtual	void RecvOutboundDtmfEvent( //收到带外DTMF
		OUT unsigned int			nCID,
		OUT char					nDtmfChar,	//DTMF字符
		OUT void*					pAppData
		) = 0;

	virtual void RecvReferEvent( //三方会议事件
		OUT unsigned int			nOwnerCID,
		OUT RtSubsHandle			hSubs,		//三方会议句柄
		OUT char*					pReferTo,	//邀请的第三方信息
		OUT void*					pAppData
		) = 0;

	virtual void JRtpRecvMediaData(	//收到RTP(仅当使用Rtp模式有效)
		OUT int						nCID,
		OUT void*					pData,
		OUT int						nDataLen,
		OUT E_RtMediaType			eMediaType,
		OUT unsigned int			nMediaPayload,	//*dynamic payload maybe is unmatch in local maybe)
		OUT bool					bMark, 
		OUT unsigned int			nTimestamp,
		OUT unsigned int			nSeqNo
		) = 0;

	virtual void JRtpRecvRtcpData( //收到RTCP(仅当使用Rtp模式有效)
		OUT int						nCID,
		OUT E_RtMediaType			eMediaType,
		OUT E_RtRtcpPacketType		eType,
		OUT unsigned int			dwSecond,
		OUT const char*				pSenderAddress,
		OUT const T_RtQOS4*			pQos
		) = 0;

	virtual ~IRtCallCBSink() = 0;
};

class RTAPI IRtCallAPI 
{
public:	
	/*
	 * 不区分模式的共用函数
	 */
	virtual int ProduceCID( //主叫为呼出申请CID
		INOUT int*				pCID
		) = 0;
	
	virtual int Reject( //被叫拒绝呼入
		IN    int				nCID, 
		IN    E_RtRejectCode	eReasonCode,
		IN    const char*		pszConfPlainText = NULL,
		IN    const int			nConfPlainTextLen = 0,
		IN    char*				pszBodyType = NULL	//格式"[主类型]/[子类型]",如果空且消息体不空则设置为text/plain
		) = 0;

	virtual int Hangup( //挂机,在任何状态下都可调用,栈会根据情况决定发送何种消息
		IN    int				nCID,
		IN    bool				bTerminated = false
		) = 0;

	virtual int SendDtmfByOutbound( //发送带外DTMF
		IN    int				nCID,
		IN    char				szChar
		) = 0;

	virtual int SendResponseCode( //被叫发送最终响应
		IN    int				nCID, 
		IN    int				nCode
		) = 0;
	
	virtual int SendProvisionalCode( //被叫发送临时响应(如设置了手动发送临时响应才需调用本函数)
		IN    int				nCID, 
		IN    int				nCode,
		IN    bool				bReliable, //是否发送可靠临时响应
		IN    const RtSdpHandle	hSdp
		) = 0;
	   
	virtual int GetAppData( //获得传入的数据
		IN	  int				nCID,
		OUT   void**			ppAppData
		) = 0;
		
	virtual int SetAppData( //设置传入的数据
		IN	  int				nCID,
		IN    void*				pAppData
		) = 0;
	
	virtual int GetCalleeHost( //获取被叫地址
		IN	  int				nCID,
		INOUT char*				pAddrBuffer,
		IN	  int				nAddrBufferLen,
		INOUT int*				pActualLen
		) = 0;

	virtual int GetCalleeUser( //获取被叫用户名
		IN	  int				nCID,
		INOUT char*				pUsernameBuffer,
		IN	  int				nUsernameBufferLen,
		INOUT int*				pActualLen
		) = 0;
	   
	virtual int GetCallerHost( //获取主叫地址
		IN	  int				nCID,
		INOUT char*				pAddrBuffer,
		IN	  int				nAddrBufferLen,
		INOUT int*				pActualLen
		) = 0;
	   
	virtual int GetCallerUser( //获取主叫用户名
		IN	  int				nCID,
		INOUT char*				pUsernameBuffer,
		IN	  int				nUsernameBufferLen,
		INOUT int*				pActualLen
		) = 0;
	   
	virtual int GetCurrentState( //获取当前状态
		IN	  int				nCID,
		INOUT E_RtCallState*	peState
		) = 0;
	
	virtual int GetDirection( //获取本端的呼叫方向
		IN	  int				nCID,
		INOUT E_RtDirection*	peDirect
		) = 0;

	virtual int GetRemoteBodyMediaType( //获得远端消息体类型
		IN    int				nCID,
		INOUT char*				pBuffer,
		IN    int				nBufferLen,
		INOUT int*				pActualLen
		) = 0;

	virtual int GetRemoteBody( //获得远端消息体内容
		IN    int				nCID,
		INOUT char*				pBuffer,
		IN    int				nBufferLen,
		INOUT int*				pActualLen
		) = 0;
	
	virtual int GetBoundaryBodyList( //获得远端组合消息体
		IN	  int							nCID,
		INOUT std::list<T_RtSipMixerBody>&	lst
		) = 0;

	virtual int GetCallID( //获取CALL-ID
		IN	  int				nCID,
		INOUT char*				pszBuffer,
		IN	  int				bufLen,
		INOUT int*				pActualLen
		) = 0;

	virtual int GetFromTag( //获取FROM Tag
		IN	  int				nCID,
		INOUT char*				pszBuffer,
		IN	  int				bufLen,
		INOUT int*				pActualLen
		) = 0;

	virtual int GetValueForHeaderName( //获得特定消息头字段值
		IN    int				nCID,
		IN	  char*				pHeaderName, //不能用于CALL-ID,From,To头字段
		INOUT char*				pValueBuffer,
		IN	  int				nValueBufferLen,
		INOUT int*				pActualLen
		) = 0;

	virtual int GetResponseCode( //主叫获取具体响应码
		IN    int				nCID,
		INOUT int*				pnCode,
		INOUT char*				pszReason,   //原因描述
		IN	  int				nReasonBufferLen
		) = 0;

	/*
	 * 三方会议函数
	 */
	virtual int ReferAccept( //接受Refer请求并发起呼叫
		IN    RtSubsHandle		hRtSubs,		//RecvReferEvent回调提供该句柄值
		IN    const char*		pszCallee,
		IN    const RtSdpHandle	hSdp,
		IN	  const char*		pszConfPlainText,
		IN    const int			nConfPlainTextLen,
		IN    char*				pszBodyType,	//消息体类型,格式是"[主类型]/[子类型]",如果空且消息体不空则设置为text/plain
		IN    char*				pszUserAgent,	//User-Agent Header Value
		IN    char*				pszSubject,		//Subject Header Value
		IN    char*				pszRequire,		//Require Header Value
		IN    char*				pszServer,		//Server Header Value
		IN    char*				pszLocalName,
		IN    void*				pAppData,
		INOUT int*				pnCID			//新呼叫的CID值
		) = 0;
	
	virtual int ReferAccept( //接受Refer请求, 之后需调用ReferConnectCall()发起呼叫
		IN    RtSubsHandle		hRtSubs,		//RecvReferEvent回调提供该句柄值
		INOUT int*				pnCID			//新呼叫的CID值
		) = 0;	
	
	virtual int ReferConnectCall( //通过Refer发起一个外呼
		IN    int				nCID,
		IN    const char*		pszCallee,
		IN    const RtSdpHandle	hSdp,
		IN	  const char*		pszConfPlainText,
		IN    const int			nConfPlainTextLen,
		IN    char*				pszBodyType,	//消息体类型,格式是"[主类型]/[子类型]",如果空且消息体不空则设置为text/plain
		IN    char*				pszUserAgent,	//User-Agent Header Value
		IN    char*				pszSubject,		//Subject Header Value
		IN    char*				pszRequire,		//Require Header Value
		IN    char*				pszServer,		//Server Header Value
		IN    char*				pszLocalName,
		IN    void*				pAppData
		) = 0;
	
	virtual int ReferSendNotify( //发送一个Refer关联的NOTIFY
		IN    RtSubsHandle		hRtSubs,
		IN    int				nStateCode
		) = 0;
	
	virtual int ReferReject( //拒绝Refer请求
		IN    RtSubsHandle		hRtSubs,
		IN    unsigned int		nRejectCode
		) = 0;
	
	virtual int ReferTerminated( //终止Refer进程
		IN    RtSubsHandle		hRtSubs
		) = 0;
	
	virtual int ReferSendRefer( //发起一个Refer请求
		IN	  int				nCID,
		IN	  char*				pszReferTo,
		OUT   RtSubsHandle*		phRtSubs
		) = 0;


	/*
	 * 仅 NoRtp 模式使用函数
	 */
	virtual int MakeForPlain( //主叫呼出
		IN  const int			nCID,
		IN  const char*			pszCallee,			//被叫号码,注册则填写对端账号,未注册则填写对方IP地址
		IN  const char*			pRawBody,			//消息体,pRawBody[0]不应等于0x0,其后的位置允许0x0出现
		IN	unsigned int		nBodyLen,			//消息体长度
		IN	char*				pszBodyType,		//格式是"[主类型]/[子类型]",如果空则设置为application/SDP
		IN  char*				pszUserAgent,		//NULL则无该SIP头
		IN  char*				pszLocalUsername,	//本端名字,NULL:在注册时为本端注册账号,未注册时则为Unknown
		IN  void*				pAppData,			//应用层APP数据
		IN	char*				pszFromHost = NULL,	//强制更改From头的Host (缺省NULL时: 已注册则为CMS地址,未注册则为本端地址)
		IN	char*				pszToHost = NULL	//强制更改To头的Host   (缺省NULL时: 已注册则为CMS地址,未注册则为pszCallee中Host地址)
		) = 0;

	virtual int MakeForSdp( //主叫呼出
		IN  const int			nCID,
		IN  const char*			pszCallee,
		IN	const RtSdpHandle	hSdp,
		IN  char*				pszUserAgent,
		IN  char*				pszLocalUsername,
		IN  void*				pAppData,
		IN	char*				pszFromHost = NULL,
		IN	char*				pszToHost = NULL
		) = 0;

	virtual int PickupForPlain( //被叫摘机
		IN  int					nCID,
		IN  const char*			pRawBody,
		IN  unsigned int		nBodyLen,
		IN  char*				pszBodyType
		) = 0;

	virtual int PickupForSdp( //被叫摘机
		IN  int					nCID,
		IN  const RtSdpHandle 	hSdp
		) = 0;


	/*
	 * 仅 RTP 模式使用函数
	 */	
	virtual int Make( //呼出
		IN  const int			nCID,
		IN  const char*			pszCallee,					//被叫号码,注册则填写对端账号,未注册则填写对方IP地址
		IN  const char*			pszMediaCapaSet,			//媒体集,以逗号分隔,如[0,8,100]
		IN  char*				pszUserAgent,				//NULL则无该SIP头
		IN  char*				pszLocalUsername,			//本端名字,NULL:在注册时为本端注册账号,未注册时则补充为Unknown
		IN  void*				pAppData,					//应用层透明传输数据指针
		IN  char*				pszVideoCustomAttr = NULL,	//用户自定义的视频参数
		IN  char*				pszFmtpInfo = NULL,			//FMTP信息值,用于标准H264和MPEG4时格式为:profile-level-id=<6字节>;packetization-mode=1;sprop-parameter-sets=<64位编码值>
		IN  char*				pszAppHeaderName1 = NULL,	//用户自定义扩展SIP头(第一组)
		IN  char*				pszAppHeaderValue1 = NULL,	//用户自定义扩展SIP头的值(第一组)
		IN  char*				pszAppHeaderName2 = NULL,	//用户自定义扩展SIP头(第二组)
		IN  char*				pszAppHeaderValue2 = NULL,	//用户自定义扩展SIP头的值(第二组)
		IN  const int			nSendRecvMode = 0,			//RTP连接模式,0-SendRecv, 1-SendOnly, 2-RecvOnly 
		IN	char*				pszFromHost = NULL,			//强制更改From头的Host (缺省NULL时: 已注册则为CMS地址,未注册则为本端地址)
		IN	char*				pszToHost = NULL			//强制更改To头的Host   (缺省NULL时: 已注册则为CMS地址,未注册则为pszCallee中Host地址)
		) = 0;	
	
	virtual int Pickup( //被叫摘机
		IN  int					nCID,
		IN  const char*			pszMediaCapaSet = NULL,		//媒体集,以逗号分隔,如[0,8,100](都取本地值, 为NULL则匹配主叫能力集)
		IN  char*				pszVideoCustomAttr = NULL,	//用户自定义的视频参数
		IN  char*				pszFmtpInfo = NULL			//FMTP信息值, 用于标准H264和MPEG4时格式为:profile-level-id=<6字节>;packetization-mode=1;sprop-parameter-sets=<64位编码值>
		) = 0;	

	virtual int PacketSend( //发送RTP包(非标准视频流应采用本函数,标准H264应采用PacketSendH264AnnexB函数)
		IN	int					nCID,
		IN	E_RtMediaType		eMediaType,
		IN	char*				pszBuffer, 
		IN	int					nBufferLen,
		IN  bool				bSetMark = false //对标准视频流,bSetMark无效,一帧数据结束总是置Mark位
		) = 0;	

	virtual int PacketSendH264AnnexB( //发送标准H264视频流,如果包大于MTU并且bUseFUA=true,则按RFC3984进行FU-A分片
		IN	  int				nCID,
		IN	  unsigned char*	pszBuffer,		//必须是AnnexB的位流数据 
		IN	  unsigned int		nBufferLen,		//AnnexB长度
		IN    bool				bUseFUA = true	//如设置false,则无论数据多长都单个RTP包发送,否则根据nDivBorder值进行FUA分片
		) = 0;	

	virtual int GetRequestMediaType( //获得请求的媒体类型,主被叫可在通话状态下调本函数获知最终建立的媒体链路,被叫也可在OFFERING调用以获知主叫请求类型
		IN	  int				nCID,
		INOUT bool*				pbHasAudio,
		INOUT bool*				pbHasVideo
		) = 0;
	
	virtual int GetCurrentPayload( //获取协商后的媒体编码类型
		IN	  int				nCID,
		IN	  E_RtMediaType		eMediaType,
		INOUT int*				pnPt //* auto-adapt for dynamic pt, return rtp's packet value 
		) = 0;  

	virtual int GetRemoteMediaInfo( //获得远端媒体信息,返回诸如[0,8,100]
		IN	  int				nCID,
		INOUT char*				pBuffer,
		IN    int				nBufferLen,
		INOUT int*				pnConnMode //0-SendRecv, 1-SendOnly, 2-RecvOnly 
		) = 0;

	virtual int GetVideoChannelNumber( //获得用户特定的视频信息
		IN	  int				nCID,
		INOUT char*				pBuffer,
		IN    int				nBufferLen
		) = 0;
	
	virtual int GetFmtpConfig( //获得FMTP参数(仅用于标准H264),返回格式:profile-level-id=<6字节>;packetization-mode=1;sprop-parameter-sets=<64位编码值>格式
		IN	  int				nCID,
		INOUT char*				pBuffer,
		IN    int				nBufferLen
		) = 0;


	virtual	int Redirect( //主叫根据转寄响应发起呼叫
		IN    int				nCID
		) = 0;

	virtual int Hold( //发起呼叫保持
		IN    int				nCID
		) = 0;
	
	virtual	int	Transfer( //发起呼叫转移
		IN    int				nCID,
		IN    char*				szReferToUrl
		) = 0;	

	
	virtual int UdpSendByRtpSocket( //通过本端RTP端口发送UDP包到远端RTP端口(因为栈会主动Hole媒体链路,一般无需调用本函数)
		IN	  int				nCID,
		IN	  E_RtMediaType		eMediaType
		) = 0;

	virtual int UdpSendByRtcpSocket( //通过本端RTCP端口发送UDP包到远端RTCP端口(因为栈会主动Hole媒体链路,一般无需调用本函数)
		IN	  int				nCID,
		IN	  E_RtMediaType		eMediaType
		) = 0;

	virtual int PacketRecv( //轮询模式接收RTP包,本函数用于特殊项目,AfterPacketRecv()与其配套调用
		IN	  int				nCID, 
		IN	  E_RtMediaType		eMediaType,
		INOUT int*				pnPt,
		OUT	  unsigned char**	ppBuffer,		//数据Buffer,应用层无需分配内存
		INOUT int*				pnBufferLen,
		INOUT bool*				pbMark,
		INOUT unsigned int*		pnTimestamp,
		INOUT unsigned int*		pnSeqNo,
		OUT   void**			phRtpSession,	//为AfterPacketRecv()提供参数
		OUT   void**			phRtpPackage	//为AfterPacketRecv()提供参数
		) = 0;

	virtual void AfterPacketRecv( //调用PacketRecv函数后，必须调用本函数删除数据资源
		IN    void*				hRtpSession,
		IN    void*				hRtpPackage
		) = 0;

	virtual void SetAudioSampleTimePerRTP( //设置音频一次采样时间, 毫秒
		IN	  int				nCID,
		IN    int				nMillis
		) = 0;

	virtual void SetVideoSampleTimePerRTP( //设置视频一帧采样时间, 毫秒
		IN	  int				nCID,
		IN    int				nMillis
		) = 0;

	virtual ~IRtCallAPI() = 0;
};

#endif 
