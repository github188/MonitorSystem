#ifndef __IWL_SIP_STACK_SIPTRAPI_H__
#define __IWL_SIP_STACK_SIPTRAPI_H__

typedef enum __e_RtMethodType
{
	E_RT_METHOD_TYPE_UNDEFINED				= -1,
	E_RT_METHOD_TYPE_MESSAGE				= 0,
	E_RT_METHOD_TYPE_INFO					= 1,
	E_RT_METHOD_TYPE_OPTIONS				= 2,
	E_RT_METHOD_TYPE_NOTIFY					= 3,
	E_RT_METHOD_TYPE_UPDATE					= 4,
	E_RT_METHOD_TYPE_REGISTER				= 5,
} E_RtMethodType;

typedef enum  __e_RtTransactionState
{ 
	E_RT_TRANSC_STATE_UNDEFINED				= -1,	//无效
	E_RT_TRANSC_STATE_IDLE					= 0,	//空闲
	E_RT_TRANSC_C_STATE_OK					= 1,	//被叫接受发送的请求(UAC)
	E_RT_TRANSC_C_STATE_FAIL				= 2,	//被叫拒绝发送的请求(UAC)
	E_RT_TRANSC_C_STATE_PROCEEDING			= 3,	//被叫正在处理请求  (UAC)
	E_RT_TRANSC_S_STATE_RECV_REQ			= 4,	//收到主叫发送请求  (UAS)
	E_RT_TRANSC_S_STATE_WAIT_USER_PEOCESS	= 5,	//等待用户处理请求  (UAS)
	E_RT_TRANSC_S_STATE_USER_ACK_ARRIVED	= 6,	//响应消息发送成功  (UAS)
	E_RT_TRANSC_S_STATE_USER_ACK_DISCARD	= 7,	//响应消息发送失败  (UAS)
	E_RT_TRANSC_S_STATE_SET_PASSWORD		= 8,	//等待用户为注册请求设置密码(UAS)
} E_RtTransactionState; 

typedef enum __e_RtTransactionStateReason
{
	E_RT_TRANSC_REASON_UNDEFINED			= -1,	//无效
	E_RT_TRANSC_REASON_NORMAL				= 0,	//正常
	E_RT_TRANSC_REASON_FAIL_REDIRECTION		= 1,	//3xx消息 (UAC)
	E_RT_TRANSC_REASON_FAIL_REJECT			= 2,	//4xx消息 (UAC)
	E_RT_TRANSC_REASON_FAIL_SERVER			= 3,	//5xx消息 (UAC)
	E_RT_TRANSC_REASON_FAIL_GLOBAL			= 4,	//6xx消息 (UAC)
	E_RT_TRANSC_REASON_FAIL_NETWORK			= 5,	//网络错误(UAC)
	E_RT_TRANSC_REASON_FAIL_CANCEL			= 6,	//事务取消(UAC)
	E_RT_TRANSC_REASON_FAIL_TIMEOUT			= 7,	//超时    (UAC)
} E_RtTransactionStateReason; 

class RTAPI IRtTransactionCBSink
{
public:
	virtual	void StateChangeEvent(
		OUT unsigned int				nTID,
		OUT E_RtTransactionState		eState,
		OUT E_RtTransactionStateReason	eReason,
		OUT void*						pAppData
		) = 0;

	virtual ~IRtTransactionCBSink() = 0;
};

class RTAPI IRtTransactionAPI
{
public:
	virtual int ProduceTID( //主叫产生呼出请求句柄
		INOUT int*				pnTID, 
		IN    void*				pAppData
		) = 0;

	virtual int Cancel(  //主叫取消呼出请求
		IN  int					nTID
		) = 0;

	virtual int Reject( //被叫拒绝呼入请求
		IN  int					nTID,
		IN	E_RtRejectCode		eCode
		) = 0;

	virtual int SendResponseCode( //被叫发送响应代码
		IN  int					nTID, 
		IN  int					nCode
		) = 0;

	virtual int Terminate( //主叫或被叫终止事务(根据当前状态决定操作方式,推荐)
		IN  int					nTID
		) = 0;

	virtual int SetAppData( //设置APP数据
		IN    int				nTID, 
		INOUT void*				pAppData
		) = 0;
	
	virtual int Make( //主叫发起请求
		IN  int					nTID,
		IN  const char*			pszDestUri,						//被叫的账号或地址
		IN  E_RtMethodType		eMethodType,					//请求的方法类型
		IN  const char*			pszBody,						//消息体内容
		IN  unsigned int		nBodyLen,						//消息体长度
		IN  const char*			pszUserAgent = NULL,			//User-Agent
		IN  char*				pszAppHeaderName1 = NULL,		//第一组自定义头
		IN  char*				pszAppHeaderValue1 = NULL,		//第一组自定义值
		IN  char*				pszAppHeaderName2 = NULL,		//第二组自定义头
		IN  char*				pszAppHeaderValue2 = NULL,		//第二组自定义值
		IN  char*				pszLocalUsername = NULL,		//强制更改From头的User (缺省NULL时: 已注册则为本端账号,未注册则为Unknown)
		IN	char*				pszBodyType = NULL,				//消息体类型,格式为"主类型/子类型",如果空则缺省为text/plain
		IN	char*				pszFromHost = NULL,				//强制更改From头的Host (缺省NULL时: 已注册则为CMS地址,未注册则为本端地址)
		IN	char*				pszToHost = NULL				//强制更改To头的Host   (缺省NULL时: 已注册则为CMS地址,未注册则为pszDestUri参数中Host值)
		) = 0;		
	
	virtual int Accept( //被叫接受请求
		IN  int					nTID, 
		IN  const char*			pszBody = NULL,					//消息体内容
		IN  unsigned int		nBodyLen = 0,					//消息体长度
		IN	char*				pszBodyType = NULL,				//消息体类型,格式是"主类型/子类型",如果空则缺省为text/plain
		IN  const char*			pRegName = NULL,				//如果收到的消息是Register,本参数必须填写,用于构造Contact,可通过GetRegisterInfo()获得信息
		IN  const char*			pRegAddress = NULL,				//如果收到的消息是Register,本参数必须填写,用于构造Contact,可通过GetRegisterInfo()获得信息
		IN  int					nRegRefreshTime = 50			//如果收到的消息是Register,本参数必须填写,用于构造Contact,可通过GetRegisterInfo()获得信息
		) = 0;
	
	virtual int GetReceiverInfo( //获得被叫信息
		IN    int				nTID,
		INOUT T_RtSipUrlInfo*	pReceiver
		) = 0;

	virtual int GetSenderInfo( //获得主叫信息
		IN    int				nTID,
		INOUT T_RtSipUrlInfo*	pSender
		) = 0;

	virtual int GetRemoteBody( //获得对方的消息体
		IN    int				nTID, 
		INOUT char*				pBodyStr,
		IN    int				nBodyLen,
		INOUT int*				pActualLen,
		INOUT char*				pContentType = NULL
		) = 0;
	
	virtual int GetMethod( //获得方法
		IN    int				nTID, 
		INOUT E_RtMethodType*	peMethod
		) = 0;
	
	virtual int GetDirection( //获得呼叫方向
		IN	  int				nTID, 
		INOUT E_RtDirection*	peDirection
		) = 0;
	
	virtual int GetCurrentState( //获得当前状态
		IN    int					nTID, 
		INOUT E_RtTransactionState*	peState
		) = 0;

	virtual int GetAppData( //获得APP数据
		IN    int				nTID, 
		OUT   void**			ppAppData
		) = 0;

	virtual int GetValueForHeaderName( //获得指定头的值(不能用于CALL-ID,FROM,TO,CONTENT-TYPE)
		IN    int				nTID,
		IN	  char*				pHeaderName,
		INOUT char*				pValueBuffer,
		IN	  int				nValueBufferLen,
		INOUT int*				pActualLen
		) = 0;

	virtual int SetPassword( //被叫为注册请求设置密码
		IN    int				nTID,
		IN	  char*				pszPassword
		) = 0;

	virtual int GetRegisterInfo( //被叫获得注册请求的信息
		IN	  int				nTID,
		INOUT T_RtSipUrlInfo*	pSender,
		INOUT unsigned*			pnExpires
		) = 0;

	virtual ~IRtTransactionAPI() = 0;
};

#endif
