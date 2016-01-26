#ifndef __IWL_SIP_STACK_SIPREGAPI_H__
#define __IWL_SIP_STACK_SIPREGAPI_H__

typedef enum __e_RtRegClientState
{
	E_RT_REG_STATE_UNDEFINED				= -1, //无效
	E_RT_REG_STATE_IDLE						= 0,  //空闲
	E_RT_REG_STATE_PROCEEDING				= 1,  //正在注册
	E_RT_REG_STATE_OK						= 2,  //注册成功
	E_RT_REG_STATE_FAILED					= 3,  //注册失败
} E_RtRegClientState;

typedef enum __e_RtRegClientStateReason
{	
	E_RT_REG_REASON_UNDEFINED				= -1,
	E_RT_REG_REASON_NORMAL					= 0,  //正常
	E_RT_REG_REASON_FAIL_UNAUTH				= 1,  //未通过服务器的认证	
	E_RT_REG_REASON_FAIL_REDIRECTION		= 2,  //3xx消息(即要求向另一地址注册)
	E_RT_REG_REASON_FAIL_REJECT				= 3,  //4xx消息(即被拒绝)
	E_RT_REG_REASON_FAIL_SERVER				= 4,  //5xx消息(即服务端故障)
	E_RT_REG_REASON_FAIL_GLOBAL				= 5,  //6xx消息(即全局网络故障)
	E_RT_REG_REASON_FAIL_TIMEOUT			= 6,  //超时,在32s内没有收到响应
	E_RT_REG_REASON_FAIL_NETWORK			= 7	  //该失败原因有多种可能: 本端网络故障/不存在的服务器/远端网络故障
} E_RtRegClientStateReason;


class RTAPI IRtRegClientCBSink 
{
public:
	virtual	void StateChangeEvent(
		OUT unsigned int				nRID,
		OUT E_RtRegClientState			eState,
		OUT E_RtRegClientStateReason	eReason,
		OUT void*						pAppData
		) = 0;

	virtual ~IRtRegClientCBSink() = 0;
};

class RTAPI IRtRegClientAPI
{
public:
	virtual int Make( //新注册
		IN    const char*			pszServer,
		IN    const char*			pszUsername,
		IN    const char*			pszPassword,
		IN    const int				nExpires, 
		IN    const char*			pszUserAgent,
		IN    void*					pAppData,
		INOUT int*					pnRID,
		IN    const char*			pszFromHost = NULL,	//强制更改From头的Host(缺省NULL时:为服务器地址)
		IN    const char*			pszToHost = NULL	//强制更改To头的Host(缺省NULL时:为服务器地址)
		) = 0;	 
	
	virtual int MakeEx( //不受控的新注册(在需要向多个不同地址的服务器进行注册时使用)
		IN    const char*			pszServer,
		IN    const char*			pszUsername,
		IN    const char*			pszPassword,
		IN    const int				nExpires,
		IN    const char*			pszUserAgent,
		IN    void*					pAppData,
		INOUT int*					pnRID,
		IN    const char*			pszFromHost = NULL,
		IN    const char*			pszToHost = NULL
		) = 0; 
	
	virtual int Unregister( //取消注册
		IN    int					nRID
		) = 0; 
	
	virtual int Terminate( //强制终止注册
		IN    int					nRID
		) = 0;  
	
	virtual int GetExpiresTTL( //获得该注册剩余时间(ms)
		IN    int					nRID, 
		INOUT unsigned*				pnTTL
		) = 0; 
	
	virtual int GetExpiresTime( //获得注册有效期时间
		IN    int					nRID, 
		INOUT int*					pnExpires
		) = 0;
	
	virtual int SetExpiresTime( //设置注册有效期时间
		IN    int					nRID, 
		IN    int					nExpires
		) = 0;
	
	virtual int GetAppData(  //获得应用层的数据
		IN    int					nRID, 
		OUT   void**				ppAppData
		) = 0; 
	
	virtual int SetAppData(  //设置应用层的数据
		IN    int					nRID, 
		IN    void*					pAppData
		) = 0; 
	
	virtual int GetServerInfo( //获得服务器信息
		IN    int					nRID,
		INOUT char*					pszServerAddr,
		IN    unsigned				nBufferLen,
		INOUT unsigned*				pnBufferActualLen,
		INOUT unsigned*				pnServerPort
		) = 0;
	
	virtual int GetUserInfo( //获得本端注册用户信息
		IN    int					nRID,
		INOUT char*					pszUsername,
		IN    unsigned				nBufferLen,
		INOUT unsigned*				pnBufferActualLen
		) = 0;
	
	virtual int GetCurrentState( //获得当前状态
		IN    int					nRID, 
		INOUT E_RtRegClientState*	peState
		) = 0; 

	virtual ~IRtRegClientAPI() = 0;
};

#endif
