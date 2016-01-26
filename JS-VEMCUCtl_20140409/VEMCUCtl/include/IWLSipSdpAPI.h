#ifndef __IWL_SIP_STACK_SIPSDPAPI_H__
#define __IWL_SIP_STACK_SIPSDPAPI_H__

RT_DECLARE_HANDLE(RtSdpHandle);			//SDP句柄
RT_DECLARE_HANDLE(RtSdpMediaHandle);	//媒体段句柄

typedef enum __e_RtSdpConnMode
{
	E_RT_SDPCONNMODE_UNDEFINED = 0,		//无效
	E_RT_SDPCONNMODE_SENDONLY,			//仅发送
	E_RT_SDPCONNMODE_RECVONLY,			//仅接收
	E_RT_SDPCONNMODE_SENDRECV,			//同时发送和接收
} E_RtSdpConnMode;

class RTAPI IRtSdpAPI
{
public:
	virtual	int Construct( //构造SDP
		OUT RtSdpHandle*		phSdp
		) = 0;

	virtual	int Construct( //从String中构造SDP
		IN    char*				pSdpBuffer, 
		IN    int*				pSdpBufferLen,
		OUT   RtSdpHandle*		phSdp
		) = 0;

	virtual int Destruct( //销毁SDP
		IN    const RtSdpHandle	hSdp
		) = 0;

	virtual int ConstructCopy( //拷贝构造SDP
		OUT   RtSdpHandle*		phDest, 
		IN    const RtSdpHandle	hSrc
		) = 0;

	virtual int EncodeToBuf( //将SDP编码成String
		IN  const RtSdpHandle	hSdp, 
		IN  char*				pBuf, 
		IN  int					nLen
		) = 0;

	virtual int SetBoundaryLine( //设置组合行默认值(o, s, i, u, c, t, 不包括m行)
		IN const RtSdpHandle	hSdp,
		IN const char*			pAddress
		) = 0;

	virtual int SetOrigin( //设置o行
		IN const RtSdpHandle	hSdp,
		IN const char*			pUsername,
		IN const char*			pSessionID,
		IN const char*			pSessionVer,
		IN const char*			pAddress //仅支持Internet的IPv4格式
		) = 0;

	virtual int GetOrigin( //获得o行
		IN    const RtSdpHandle	hSdp,
		INOUT char*				pUsername,		//建议bufferLen > 56
		INOUT char*				pSessionID,		//建议bufferLen > 56
		INOUT char*				pSessionVer,	//建议bufferLen > 32
		INOUT char*				pAddress		//建议bufferLen > 56
		) = 0;

	virtual int SetSLine( //设置s行
		IN  const RtSdpHandle	hSdp,
		IN  const char*			pSessionName
		) = 0;

	virtual const char* GetSLine( //获得s行
		IN const RtSdpHandle	hSdp
		) = 0; 


	virtual int SetILine( //设置i行
		IN  const RtSdpHandle	hSdp,
		IN  const char*			pInfoName
		) = 0;

	virtual const char* GetILine( //获得i行
		IN const RtSdpHandle	hSdp
		) = 0; 


	virtual int SetULine( //设置uri行
		IN  const RtSdpHandle	hSdp,
		IN  const char*			pUriAddr
		) = 0;

	virtual const char*	GetULine( //获得uri行
		IN const RtSdpHandle	hSdp
		) = 0; 


	virtual int SetCLine( //设置c行
		IN  const RtSdpHandle	hSdp,
		IN  const char*			pAddress
		) = 0;

	virtual const char*	GetCLine( //获得c行IPv4地址
		IN const RtSdpHandle	hSdp
		) = 0; 


	virtual int SetTLine( //设置t行
		IN  const RtSdpHandle	hSdp,
		IN  unsigned int		nStart,
		IN  unsigned int		nStop
		) = 0;

	virtual int GetTLine( //获得t行
		IN    const RtSdpHandle	hSdp,
		INOUT unsigned int*		pnStart,
		INOUT unsigned int*		pnStop
		) = 0;

	virtual void ClearAllMediaDescr( //清除所有的m行
		IN const RtSdpHandle	hSdp
		) = 0; 

	virtual unsigned int GetNumOfMediaDescr( //获得m行个数
		IN const RtSdpHandle	hSdp
		) = 0; 


	/* -------------------- 媒体段操作函数 -------------------- */
			
	virtual RtSdpMediaHandle GetMediaDescr( //获得指定的m行句柄
		IN const RtSdpHandle	hSdp, 
		IN int					nIndex
		) = 0; 

	virtual RtSdpMediaHandle AddMediaDescr( //增加m行
		IN  const RtSdpHandle	hSdp,
		IN  E_RtMediaType		eMediaType,
		IN  unsigned int		nRtpPort,
		IN  E_RtTransportType	eProtocol
		) = 0;

	
	virtual E_RtMediaType MediaGetType( //获得m行类型
		IN  const RtSdpMediaHandle	hMedia
		) = 0; 

	virtual E_RtTransportType MediaGetProtocol( //获得m行协议
		IN  const RtSdpMediaHandle	hMedia
		) = 0; 

	virtual unsigned int MediaGetPort( //获得m行端口
		IN  const RtSdpMediaHandle	hMedia
		) = 0; 

	virtual unsigned int MediaGetNumOfPt( //获得m行中pt数
		IN  const RtSdpMediaHandle	hMedia
		) = 0; 

	virtual int MediaGetPt( //获得m行指定的pt值
		IN  const RtSdpMediaHandle	hMedia, 
		IN  int						nIndex
		) = 0; 

	virtual int MediaSetConnMode( //设置一个连接模式
		IN  const RtSdpMediaHandle	hMedia,
		IN  E_RtSdpConnMode			eConnMode
		) = 0;
	
	virtual E_RtSdpConnMode MediaGetConnMode( //获得连接模式
		IN  const RtSdpMediaHandle	hMedia
		) = 0; 

	
	virtual int MediaAddPt( //在m行中增加一个pt值(与MediaAddRtpmap()函数配套使用)
		IN  const RtSdpMediaHandle	hMedia, 
		IN  int						nPayload
		) = 0; 

	virtual int MediaAddFormat( //在m行中增加一个非PT的值(与MediaAddAttr()函数配套使用)
		IN  const RtSdpMediaHandle	hMedia, 
		IN  const char*				pFormat
		) = 0; 

	virtual unsigned int MediaGetNumOfRtpmap( //获得rtpmap类型的a行个数
		IN  const RtSdpMediaHandle	hMedia
		) = 0; 

	virtual unsigned int MediaGetNumOfAttr( //获得非标准类型的a行个数(不包括rtpmap和conn mode行)
		IN  const RtSdpMediaHandle	hMedia
		) = 0; 


	/* -------------------- a行操作函数 -------------------- */

	virtual int MediaGetAttr( //获得指定的a行(不能用于获得rtpmap和conn mode行)
		IN const RtSdpMediaHandle	hMedia,
		IN    int					nIndex,
		INOUT char*					pszName,		//建议bufferLen > 32
		INOUT char*					pszValue		//建议bufferLen > 120
		) = 0;	

	virtual int MediaAddAttr( //增加新的a行
		IN  const RtSdpMediaHandle	hMedia,
		IN  char*					pszName,
		IN 	char*					pszValue
		) = 0; 
	
	virtual int MediaGetRtpmap( //获得指定的rtpmap类型a行
		IN    const RtSdpMediaHandle hMedia,
		IN    int					 nIndex,
		INOUT int*					 pnPayload,
		INOUT char*					 pszEncodeName,	//建议bufferLen > 16
		INOUT int*					 pnRate
		) = 0;
	
	virtual int MediaAddRtpmap( //增加新的rtpmap类型a行
		IN  const RtSdpMediaHandle	hMedia,
		IN	int						nPayload,
		IN  char*					pszEncodeName,
		IN 	int						nRate
		) = 0; 

	virtual ~IRtSdpAPI() = 0;
};

#endif
