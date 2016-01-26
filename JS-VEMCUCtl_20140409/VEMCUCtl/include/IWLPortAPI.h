#ifndef __IWL_SIP_STACK_PORTAPI_H__
#define __IWL_SIP_STACK_PORTAPI_H__

typedef struct __t_RtCommConfigParam 
{
	int		maxUserFd;		//最大设备句柄数
	int		maxUserTimers;	//最大定时器数
} T_RtCommConfigParam;

class RTAPI CRtCommEngineer 
{
public:
	static CRtCommEngineer*	Instance();
	virtual ~CRtCommEngineer();
	
	int	  Init(IN T_RtCommConfigParam* pCfg);
	int	  Start();
	int   Stop();

protected:
	CRtCommEngineer();
	
private:
	static CRtCommEngineer*	m_pInstance;
};

#if defined (_WIN32)
	#define RT_COMM_INT64			__int64
	#define RT_COMM_UINT64			unsigned __int64
	#define RT_INT64_SUFFIX(_n)		_n ## i64
	#define RT_UINT64_SUFFIX(_n)	_n ## Ui64
#else
	#define RT_COMM_INT64			signed long long
	#define RT_COMM_UINT64			unsigned long long
	#define RT_INT64_SUFFIX(_n)		_n ## LL
	#define RT_UINT64_SUFFIX(_n)	_n ## ULL
#endif

class RTAPI CRtCommSData 
{
public:
	static char* I64UtoA(IN RT_COMM_UINT64 num64, INOUT char* pBuf);  //64位字符化(返回值为NULL表示失败，下同)
	static char* I64toA(IN RT_COMM_INT64 num64, INOUT char* pBuf);    //64位字符化
	static char* I64toHex(IN RT_COMM_UINT64 num64, INOUT char* pBuf); //64位Hex字符化
};

typedef struct __t_RtClock
{
	unsigned int sec;	//秒
	unsigned int usec;	//微秒(1s = 1000ms = 1000*1000us = 1000*1000*1000ns)
} T_RtClock;

typedef struct __t_RtNTP
{
	unsigned int msw;	//高32位
	unsigned int lsw;	//低32位
} T_RtNTP;

typedef struct __t_RtTm
{
	unsigned	 tm_nsec;	//纳秒(秒之后的剩余值换算成纳秒)
	int			 tm_sec;	//秒	   [0-60]
	int			 tm_min;	//分       [0-59]
	int			 tm_hour;	//时	   [0-23]
	int			 tm_mday;	//天	   [1-31]
	int			 tm_mon;	//月	   [0-11]
	int			 tm_year;	//年	   (Year-1900)
	int			 tm_wday;	//周       [0-6]
	int			 tm_yday;	//第几天   [0-365]
	int			 tm_isdst;	//上午下午 [-1/0/1]
} T_RtTm;

class RTAPI CRtCommTime
{
public:
	/* ----------------   开机时钟函数  ----------------- */
	static unsigned int  GetSecond4Power(); //获得自开机以来的秒数
	static unsigned int  GetMilli4Power();	//获得自开机以来的毫秒数
	static RT_COMM_INT64 GetNano4Power();	//获得自开机以来的纳秒数

	/* -----------------   时钟函数  ----------------- */
	static int  GetClock( //获得当前时钟
		INOUT T_RtClock*	t);
	
	static int	SetClock( //设置时钟
		IN	  T_RtClock*	t);	

	static int  ConstructClockFrom64( //通过纳秒构造时钟
		IN    RT_COMM_INT64	nNanosecs,
		INOUT T_RtClock*	t);

	static RT_COMM_INT64 ConvertClockTo64( //转换到64位纳秒时钟返回
		IN	  T_RtClock*	t);	

	static int ConvertClockToNTP( //转换到NTP
		IN    T_RtClock&	clock, 
		INOUT T_RtNTP&		ntp);

	static int ConvertNTPToClock( //转换到Clock
		IN	  T_RtNTP&		ntp, 
		INOUT T_RtClock&	clock);

	static int  CompareClock( //比较时钟(t1>t2返回1)
		IN	  T_RtClock*	t1, 
		IN	  T_RtClock*	t2);

	static int  ClockAdd( //调整时钟
		INOUT T_RtClock*	t, 
		IN    int			nSec,
		IN    int			nUs); 
		
	/* -----------------   时间函数  ----------------- */
	static int GetLocalTm( //获得本地时间
		IN    T_RtClock*	t, 
		INOUT T_RtTm*		pTM);

	static int GetUtcTm( //获得UTC时间
		IN    T_RtClock*	t, 
		INOUT T_RtTm*		pTM);

	static int TmAsctime( //将时间转换成[week month day hour:min:sec year]
		IN    T_RtTm*		pTM,
		INOUT char*			pBuffer,
		IN    int			nBufferLen);
};

typedef struct __t_RtRTProfile
{
	char			szEncodeName[56];	//该编码的名称,如"H264"
	int				nMediaType;			//该编码的媒体类型(0=Audio, 1=Video)
	unsigned int	nClockRate;			//该编码的采样频率,如"90000"
	char			szRateDesc[56];		//该编码的速率说明(对视频无意义,一串描述值)
	double			fBandwidth;			//该编码的带宽说明(kb/s,对视频无意义)
	E_RtDynPtMedia	eDyncMedia;			//仅动态PT该值有效，其他类型则返回E_RT_DPT_MEDIA_UNDEFINED
} T_RtRTProfile;

class RTAPI CRtCommParse 
{
public:
	static int GetH264FmtpConfig( //解析H264的SPS和PPS成fmtp参数 [[无需引擎即能调用]]
		IN	  unsigned char*	pszAnnexB,			//必须是AnnexB串,即0x000001 or 0x00000001 开始的SPS+PPS NALu
		IN	  unsigned int		nAnnexBLen,			//AnnexB长度
		INOUT char*				pFmtpConfig,		//输出FMTP串,格式为profile-level-id=<6字节>;packetization-mode=1;sprop-parameter-sets=<64位编码值>
		IN	  int				nFmtpConfigLen);	//建议大于100个字节

	static int ParseRtpProfile( //解析RTP Profile的定义 [[无需引擎即能调用]]
		IN    const int			nPayload,
		INOUT T_RtRTProfile&	tProfile);
	
	static int ParseSipUri( //解析SipUri成结构 [[仅SIP引擎启动后有效]]
		IN    const char*		pszString, 
		INOUT T_RtSipUrlInfo&	tSipUrl);

	static bool GetMasterAddr(char* pMasterAddr); //获得本机的主地址
};


class RTAPI CRtCommLog 
{
public:
	static void PError(char* buffer, ...);	
	static void PWarning(char* buffer, ...);
	static void PInfo(char* buffer, ...);
	static void PDebug(char* buffer, ...);
	static void PDbgView(char* buffer, ...); //将信息输出到DebugView中(非Windows则标准输出)
};


#if defined (_WIN32) || defined (_WIN64)
	typedef struct __t_RtThread
	{
		void*		 h;
		unsigned	 id;
	} T_RtThread;
#else
	typedef pthread_t T_RtThread;
#endif

typedef void* (*pfnThreadEv)(void* pContext); //线程回调函数

class RTAPI CRtCommThread 
{
public:	
	static T_RtThread*	Create(IN pfnThreadEv pFunc, IN void* pAppContext); //创建线程
	static int			Join(IN T_RtThread* pThread); //等待线程退出(一直阻塞)
	static int			Exit(); //退出线程,用于从线程内部进行条件终止(和返回在效果上一样),不能在非本线程中调用!
};


/**************************************************************
 *	 
 *   LZAri 压 缩 函 数 类 [无需引擎即能调用]
 *
 * 1) 注意类实例仅允许对Compress或Uncompress操作一次, 
 *    之后必须销毁, 不能重复使用.
 *
 * 2) 可以用本类来对过大的SIP消息体(超过MTU)进行压缩传输,
 *    平均1500个字节压缩后370个字节, 压缩和解压时间 1ms.
 *
 *************************************************************/
RT_DECLARE_HANDLE(RtCommAriHandle);

class RTAPI CRtCommAri
{
public:
	CRtCommAri();
	~CRtCommAri();

	int  Compress(
		IN	   const char*		pInBuffer,
		IN	   int				nInLength,  //不要将Buffer的结束如'\0'计算在内
		OUT    const BYTE*&		pOutBuffer, //调用时不要分配内存,删除对象时会释放
		INOUT  int*				pnOutLength);

	int  Uncompress( 
		IN     const char*		pInBuffer,
		IN	   int				nInLength,
		OUT    const BYTE*&		pOutBuffer, //调用时不要分配内存,删除对象时会释放
		INOUT  int*				pnOutLength);

private:
	RtCommAriHandle m_pAri;
	int				m_nComplite;
};


RT_DECLARE_HANDLE(RtCommMutexHandle);

class RTAPI CRtCommMutex 
{
public:
	CRtCommMutex();
	~CRtCommMutex();
	
	void Lock();
	void Unlock();
	
private:
	RtCommMutexHandle m_pMutex;
	bool			  m_bUseCommEg;	
};


RT_DECLARE_HANDLE(RtCommTimerHandle);

typedef void (*pfnTimerEv)(void* pContext);

class RTAPI CRtCommTimer 
{
public:
	CRtCommTimer();
	~CRtCommTimer();

	// 当nMiliTimeOut时间到达, pfnTimerEv函数将被执行, pfnTimerEv函数如果返回则本Timer结束, 资源由引擎回收, 
	// 如果需要, 可在pfnTimerEv函数中再次调用Set函数, 但是必须保证Set的再调用操作是有条件结束.
	int Set( 
		IN unsigned int nMiliTimeOut, 
		IN pfnTimerEv	pCallbackFunc, 
		IN void*		pContext);

	int Release(int nReplay = 20); //销毁Timer，注意不要在pfnTimerEv函数内调本函数，意味直接return即可

	int GetTTL(INOUT unsigned int* pnTTL); //获得Timer的剩余生存时间

private:
	RtCommTimerHandle m_pTimer;
	bool			  m_bUseCommEg;	
};


/**************************************************************
 *	 
 *   队 列 处 理 [无需引擎即能调用]
 *
 *************************************************************/
RT_DECLARE_HANDLE(RtCommQueueHandle);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ×
 × 关于队列消息结构的定义:
 ×
 ×	1) T_RT_FIFOMSGITEM.Msg的定义可扩展, 如果以后有新需求;
 ×
 ×	2) T_RT_FIFOMSGHEAD.wDataLen的值表示T_RT_FIFOMSGITEM.Msg的长度;
 ×
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef struct __t_RT_FIFOMSGHEAD
{
	unsigned int		ucCommand;	//存放命令,如SIP状态值
	unsigned int		wParam;		//存放参数,如SIP CALL-ID,建议传值
	unsigned int		wDataLen;	//存放MSG的数据长度
} T_RT_FIFOMSGHEAD, *P_RT_FIFOMSGHEAD;

typedef struct __t_RT_FIFOMSGITEM
{
	T_RT_FIFOMSGHEAD	Head;
	union
	{
		long			lParam;		//该参数建议传指针
		unsigned char	ucN[4500];
	} Msg;
} T_RT_FIFOMSGITEM, *P_RT_FIFOMSGITEM;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ×
 × 关于多线程调用队列的注意:
 ×
 ×	1) 如果只有一个线程负责Read, 一个线程负责Write, 那么无需加锁;
 ×
 ×	2) 如果Read有多个线程在操作, 则在操作前需用户对每个Read线程加锁, Write行为一样;
 ×
 × 3) AppendMsg()具有一个隐性要求, 就是Read动作必须比Write慢, 如果保证不了, 则使用TryAppendMsg+UpdateAppendMsg;
 ×
 × 4) AppendMsg()中参数dwBytes, 格式建议(不容易出错): sizeof(T_RT_FIFOMSGHEAD) + xxxxx.Head.wDataLen;
 ×
 × 5) 一些逻辑是由用户来定义的, 比如因为队列满导致写动作失败, 是丢弃该包, 还是清空队列等;
 ×
 × 6) 分配队列长度时, 计算好自己需要的长度, 建议不要大于0x1E00000(30M Bytes).
 × 
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
class RTAPI CRtCommQueue 
{
public:
	CRtCommQueue( //分配FIFO, Name字符小于16, 长度指数据池大小
		IN char*				pszQueueName,
		IN unsigned long		dwQueueMaxSize);

	~CRtCommQueue(); //释放FIFO

	void Reset(); //清空队列中数据

	unsigned long GetFreeBytes(); //返回队列的空余长度
	
	unsigned long GetWrittenBytes(); //返回队列已写入的长度


	/* -------------------- 纯数据队列操作函数 -------------------- */

	int Append( //向队列写入指定长度的数据, 不识别数据格式, 会自动翻转指针
		IN unsigned long		dwBytes,
		IN unsigned char*		pucItem);

	int AppendAtEnd( //向队列尽力写数据, 遇队列底部结束
		IN    unsigned long		dwBytes,
		IN	  unsigned char*	pucItem,
		INOUT unsigned long*	lpNumberOfBytesWritten); //返回写入的实际数据长度

	int Pop( //从队列中取指定长度的数据,如果队列中数据小于参数dwBufferBytes指定的长度则返回实际数据长度
		INOUT unsigned char*	pucBuffer,
		IN    unsigned long		dwBufferBytes,
		INOUT unsigned long*	lpNumberOfBytesReadden); //返回读入的实际数据长度


	/* -------------------- 消息数据队列操作函数 -------------------- */

	int AppendMsg( //向队列写入指定长度的数据
		IN unsigned long		dwBytes,  //应为: sizeof(T_RT_FIFOMSGHEAD)+pItem->Head.wDataLen
		IN unsigned char*		pucItem); //应为: T_RT_FIFOMSGITEM* 的数据块头指针值

	P_RT_FIFOMSGITEM PopMsg(); //取出队列中消息

	
	/* 下面2个函数配套使用, 可以提高效率 */
	P_RT_FIFOMSGITEM TryAppendMsg(IN unsigned long dwBytes);    //查询队列能否存放指定长度(sizeof(T_RT_FIFOMSGHEAD)+pItem->Head.wDataLen)的消息,如可以,返回当前单元的数据指针否则返回NULL
	void			 UpdateAppendMsg(IN unsigned long dwBytes); //更新指定的长度的数据, 应为: sizeof(T_RT_FIFOMSGHEAD)+pItem->Head.wDataLen


	/* 下面2个函数配套使用, 可以提高效率 */
	P_RT_FIFOMSGITEM TryPopMsg();	  //查询队列中是否有新消息,如有则返回指针,但不更新队列信息,否则返回NULL
	void			 RemoveCurMsg();  //将通过TryPopMsg()取出的消息从队列中抹去

private:
	RtCommQueueHandle m_pQueue;
};

/**************************************************************
 *	 
 *   RTP 操 作 函 数
 *
 *************************************************************/
RT_DECLARE_HANDLE(RtRtpSessionHandle);

typedef void(* pfnRtRtpRecv)(
	void*				pData,
	int					nDatalen,
	unsigned char		cPT, 
	void*				pContext, 
	bool				bMark,
	unsigned int		nTimestamp,
	unsigned int		nSeqNumber);

typedef void(* pfnRtRtcpRecv)(
	E_RtMediaType			eMediaType,
	E_RtRtcpPacketType		ePacketType,
	unsigned int			dwSecondTime,
	const char*				pszSenderAddress,
	const T_RtQOS4*			pQos,
	void*					pContext);

class RTAPI CRtRtpSession
{
public:
	static int Create( //创建
		IN  const char*				pszLocalIP,		//本地地址,如果要多穴,指定值为0
		IN  const unsigned short	nLocalPort,		//本地端口
		IN  const double			nClock,			//采样频率
		IN  const unsigned int		nPayload,		//负载内型
		IN  const unsigned int		nTimestampInc,	//时间戳增量
		IN  const bool				bFastRtp,		//是否使用快速RTP(RTCP需自己发送,不对包进行抖动排序)
		IN  const E_RtMediaType		eMediaType,		//媒体类型
		OUT RtRtpSessionHandle*		phSession);		//Session句柄返回
	
	static void Destroy( //销毁
		IN  RtRtpSessionHandle		hSession);
	
	static void SetReadRtpCB( //RTP数据回调设置
		IN  RtRtpSessionHandle		hSession,
		IN  const pfnRtRtpRecv		myBackcall,
		IN  void*					pContext);
	
	static void SetReadRtcpCB( //RTCP数据回调设置
		IN	RtRtpSessionHandle		hSession,
		IN  const pfnRtRtcpRecv		myBackcall,
		IN  void*					pContext);
};

#endif


