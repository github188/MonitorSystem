#ifndef  _SIP_NET_CLIENT_SIP_UTIL_FUNC_H_
#define _SIP_NET_CLIENT_SIP_UTIL_FUNC_H_

#include "include\\IWLSipAPI.h"
#include "include\\IWLSipRegAPI.h"
#include "include\\IWLSipTrAPI.h"
#include "include\\IWLSipSdpAPI.h"
#include "include\\IWLSipCallAPI.h"
#include "include\\IWLPortAPI.h"


/**************************************************************
 *	 
 *   呼叫的Callback接收处理者
 *
 *************************************************************/
void StateChangeEvent_SipCall(
			IN unsigned int				nCID,
			IN E_RtCallState			eState,
			IN E_RtCallStateReason		eReason,
			IN void*					pAppData); 

void JRtpRecvMediaData_SipCall(
			OUT int						nCID,
			OUT void*					pData,
			OUT int						nDataLen,
			OUT E_RtMediaType		nMediaType,
			OUT unsigned int			nMediaPayload,
			OUT bool					    bMark,
			OUT unsigned int			nTimestamp,
			OUT unsigned int			nSeqNo);


/**************************************************************
 *	 
 *   事务的Callback接收处理者
 *
 *************************************************************/
void StateChangeEvent_SipTrans(
		IN unsigned int					    nTID,
		IN E_RtTransactionState			eState,
		IN E_RtTransactionStateReason	eReason,
		IN void*						pAppData);


#endif /*_SIP_NET_CLIENT_SIP_UTIL_FUNC_H_*/

