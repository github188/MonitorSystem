#ifndef __IWL_SIP_STACK_SIPAPI_H__
#define __IWL_SIP_STACK_SIPAPI_H__

#include <list>

#if !defined(BYTE)
	#define BYTE unsigned char
#endif

#if !defined(FALSE)
	#define FALSE	0
	#define TRUE	1
#endif

#if !defined(IN)
	#define IN    //调用者分配内存
#endif

#if !defined(OUT)
	#define OUT   //调用者无需分配内存
#endif

#if !defined(INOUT)
	#define INOUT //调用者分配内存
#endif

#if defined (_WIN32) || defined (_WIN64)
	#define RTAPI       __declspec(dllexport)
	#define RTCALLCONV  __stdcall
#else
	#define RTAPI
	#define RTCALLCONV
#endif

#define RT_DECLARE_HANDLE(_handleName) \
	typedef struct { int unused; } _handleName##__ ; \
	typedef const _handleName##__* _handleName; \
	typedef _handleName*  LP##_handleName

class RTAPI IRtCallCBSink;
class RTAPI IRtRegClientCBSink;
class RTAPI IRtTransactionCBSink;
class RTAPI IRtEventCBSink;

class RTAPI IRtCallAPI;
class RTAPI IRtRegClientAPI;
class RTAPI IRtTransactionAPI;
class RTAPI IRtSdpAPI;
class RTAPI IRtEventAPI;

typedef enum __e_RtSipResultCode
{
	E_RT_OK							= 1,	//成功
	E_RT_ERROR_UNKNOWN				= -1,	//未知错误
	E_RT_ERROR_OUTOFRESOURCES		= -2,	//没有资源可用
	E_RT_ERROR_BADPARAM				= -3,	//参数错误
	E_RT_ERROR_NULLPTR				= -4,	//指针为空
	E_RT_ERROR_NOTSUPPORTED			= -7,	//操作不允许
	E_RT_ERROR_UNINITIALIZED		= -8,	//未初始化
	E_RT_ERROR_NETWORK_PROBLEM		= -11,	//网络异常
	E_RT_ERROR_INVALID_HANDLE		= -12,	//无效的句柄
	E_RT_ERROR_NOT_FOUND			= -13,	//对象不能找到
	E_RT_ERROR_INSUFFICIENT_BUFFER	= -14,	//传入的内存尺寸太小
	E_RT_ERROR_OBJ_EXISTED			= -15,	//对象已存在
	E_RT_ERROR_UNAUTHED				= -16,	//未认证
} E_RtSipResultCode;

typedef enum __e_RtDirection
{
	E_RT_DIRECTION_UNDEFINED		= -1,
	E_RT_DIRECTION_IN				= 0,	//呼叫的被叫或事务的接收者
	E_RT_DIRECTION_OUT				= 1,	//呼叫的主叫或事务的发起者
} E_RtDirection;

typedef enum __e_RtRejectCode
{
	E_RT_REJECT_UNDEFINED			= -1,
	E_RT_REJECT_NORMAL				= 0,	//拒绝
	E_RT_REJECT_SERVER_ERROR		= 1,	//服务器错误
	E_RT_REJECT_GLOBAL_ERROR		= 2,	//全局错误
} E_RtRejectCode; 

typedef enum __e_RtTransportType
{
	E_RT_TRANSPORT_UNDEFINED		= -1,
	E_RT_TRANSPORT_UDP				= 0,
	E_RT_TRANSPORT_TCP				= 1,		
	E_RT_TRANSPORT_TLS				= 2,
	E_RT_TRANSPORT_SCTP				= 3,
	E_RT_TRANSPORT_RTP				= 4,
} E_RtTransportType;

typedef enum  __e_RtTransportAddressType
{
    E_RT_TRANSADDRESS_UNDEFINED		= -1,
	E_RT_TRANSADDRESS_IP4			= 0,
	E_RT_TRANSADDRESS_IP6			= 1,
} E_RtTransportAddressType;

typedef struct __t_RtSipUrlInfo
{
    char						szUser[64];
	char						szHost[52];
    unsigned					portNum;
    E_RtTransportType			eTransport;
	E_RtTransportAddressType	eAddrType;
} T_RtSipUrlInfo;

typedef enum __e_RtIID
{
	E_IID_ISIPCALL,				//呼叫接口
	E_IID_ISIPTRANS,			//事务接口
	E_IID_ISIPREG,				//注册接口
	E_IID_ISIPEVENT,			//事件接口
	E_IID_ISDPSTACK,			//SDP 接口
	E_IID_ISIPUNKNOW,			//无效
} E_RtIID; 

typedef enum __e_RtDynPtMedia
{
	E_RT_DPT_MEDIA_UNDEFINED	= -1,
	E_RT_DPT_MEDIA_H264,		//H264
	E_RT_DPT_MEDIA_MPEG4,		//MPEG4
	E_RT_DPT_MEDIA_AVS,			//AVS-P2
	E_RT_DPT_MEDIA_HIK264,		//海康
	E_RT_DPT_MEDIA_DAH264,		//大华
	E_RT_DPT_MEDIA_DAL264,		//大立
	E_RT_DPT_MEDIA_RFC2833,		//DTMF
	E_RT_DPT_MEDIA_SPEEX_8K,	//8k  SPEEX
	E_RT_DPT_MEDIA_SPEEX_16K,	//16k SPEEX
	E_RT_DPT_MEDIA_AMR_8K,		//8k  AMR
	E_RT_DPT_MEDIA_AMR_16K,		//16k AMR
	E_RT_DPT_MEDIA_H263_1998,	//H263v2
	E_RT_DPT_MEDIA_H263_2000,	//H263v3
} E_RtDynPtMedia; 

typedef enum __e_RtMediaType
{
	E_RT_MEDIA_TYPE_UNDEFINED	= -1,
	E_RT_MEDIA_TYPE_AUDIO,		//语音
	E_RT_MEDIA_TYPE_VIDEO,		//视频
	E_RT_MEDIA_TYPE_APP,		//应用共享
	E_RT_MEDIA_TYPE_CONTROL,	//控制类
} E_RtMediaType;

typedef enum __e_RtRtcpPacketType
{ 
	E_RT_RTCP_PACKET_SR,
	E_RT_RTCP_PACKET_RR,
	E_RT_RTCP_PACKET_SDES,
	E_RT_RTCP_PACKET_BYE,
	E_RT_RTCP_PACKET_APP,
	E_RT_RTCP_PACKET_UNKNOWN,
} E_RtRtcpPacketType;

typedef struct __t_RtQOS4
{
	unsigned int			nFractionLost;		//接收端的平均丢包率
	unsigned int			nLostCount;			//接收端的丢包个数
	unsigned int			nIntervalJitter;	//接收端的Jitter反馈(毫秒)
	unsigned int			nLocalJitter;		//本端根据算法得出的Jitter量值(毫秒)
	float					fLocalRoundtrip;	//本端根据算法得出的平均环路时间(毫秒)
	float					fLocalBandwidth;	//本端根据算法得出的带宽(kbps)
} T_RtQOS4;

typedef struct __t_RtStackCallBack
{
	IRtCallCBSink*			pCallSink;
	IRtRegClientCBSink*		pRegClientSink;
	IRtTransactionCBSink*	pTransactionSink;
	IRtEventCBSink*			pEventSink;
} T_RtStackCallBack;

typedef enum __e_RtStackLogFilters
{
	RT_SIP_LOG_INFO			= 0x02,	//调试
	RT_SIP_LOG_WARN			= 0x04,	//警告,一般不导致流程结束
	RT_SIP_LOG_ERROR		= 0x08,	//错误,一般导致流程失败或函数返回错误
	RT_SIP_LOG_EXCEP	    = 0x10,	//异常,一般在看到本级别错误应重启动程序
	RT_RTP_LOG				= 0x20,	//RTP,独立的文件,建议部署时不要开放
} E_RtStackLogFilters; 

typedef struct __t_RtStackConfigParam
{
	char	szLocalUdpAddress[56];	//栈的UDP地址，"0.0.0.0"表示使用多穴
	int		nLocalUdpPort;			//栈的UDP端口，等于0表示5060
	bool	bTcpEnabled;			//是否启用TCP，true使用，false不使用
	char	szLocalTcpAddress[56];	//TCP地址，"0.0.0.0"表示使用多穴
	int		nLocalTcpPort;			//TCP端口，等于0表示5060
	char	szObdProxyAddress[56];  //带外服务器地址(缺省不使用)
    int		nObdProxyPort;			//带外服务器端口
	char	szDnsServerList[256];	//DNS服务器列表(ip1:port1;ip2:port2;...空表示采用主机的设置)
	bool	bManualProv;			//手动发送临时响应，建议总是设置成false

	int		nMaxRegister;			//最大用户数
	int		nMaxCall;				//最大呼叫数
	int		nMaxTransactions;		//最大事务数
	int		nProcessingThreads;		//处理线程数(负数表示使用单线程, >0用户固定配置多线程数目)
	int		nLogLevel;				//日志等级(0x0:无日志，多等级采用位与操作如: RT_SIP_LOG_INFO|RT_SIP_LOG_ERROR)
	char	szLogBaseName[56];		//日志文件基数名(绝对路径或相对路径，栈会自动追加_n.log)

	bool	bUseJRtpCall;			//true表示使用"rtp模式"，false表示使用"non-rtp模式"
	int		nMinRtpPort;			//RTP端口最小值(仅bUseJRtpCall=true有效)
	int		nMaxRtpPort;			//RTP端口最大值(仅bUseJRtpCall=true有效)
	bool	bUseFastRtpProcess;		//使用快速RTP处理(仅bUseJRtpCall=true有效, 如果为true, RTCP则由应用层主动发送)
	bool	bRecvRtpUseLoop;		//使用Loop模式接收RTP数据(建议不要设为true, 在Loop模式下RTCP也需APP主动发送)

	int		nNatHoleMilliSecond;	//NAT老化时间,毫秒(仅在本端相对注册服务器位于NAT后有效)
	bool	bEnServerAuth;			//是否启动服务端认证能力(默认不启用)
	int		nFirstNotifyTimer;		//订阅者等待第一个NOTIFY到达的秒数
	int		nDivPacketMode;			//非标准流发送格式: 0=直接发送, 1=按nDivBorder值分割发送
	int		nDivBorder;				//RTP包的最大长度,取值范围应小于MTU
} T_RtStackConfigParam;

class RTAPI CRtSipStackManager
{
public:
	static CRtSipStackManager*	Instance();
	virtual ~CRtSipStackManager();

	int	  InitEngineer(IN T_RtStackConfigParam* pCfg); //初始化引擎

	int   SetModuleCallback(IN T_RtStackCallBack* pSinkList); //设置回调
	
	int	  StartEngineer(); //启动引擎

	int   StopEngineer();  //关闭引擎

	int	  QueryInterface(IN const E_RtIID& IID, OUT void** ppv); //查询接口类指针

	int   SetSpecMediaPayload(IN E_RtDynPtMedia eMedia, IN int pt); //调整动态PT值

protected:
	CRtSipStackManager();

private:
	IRtCallAPI*					m_pCall;
	IRtRegClientAPI*			m_pRegClient;
	IRtTransactionAPI*			m_pTransaction;
	IRtSdpAPI*					m_pSdp;
	IRtEventAPI*				m_pEvent;
	static CRtSipStackManager*	m_pInstance;
};

#endif
