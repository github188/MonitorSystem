#ifndef  _SIP_STATE_HANDLER_H_
#define _SIP_STATE_HANDLER_H_

#include "include\\IWLSipAPI.h"
#include "include\\IWLSipRegAPI.h"
#include "include\\IWLSipTrAPI.h"
#include "include\\IWLSipSdpAPI.h"
#include "include\\IWLSipCallAPI.h"
#include "include\\IWLPortAPI.h"
#include "SipUtil.h"

/***************************************呼叫状态处理相关函数***********************************/

//呼叫UNDEFINED状态处理函数
BOOL SipCallStatusUndefinedHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData);

//呼叫IDLE状态处理函数
BOOL SipCallStatusIdleHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData);

//呼叫INVITING状态处理函数
BOOL SipCallStatusInvitingHandler(unsigned int	 nCID,E_RtCallStateReason eReason,void* pAppData);

//呼叫OFFERING状态处理函数
BOOL SipCallStatusOfferingHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData);

//呼叫PROCEEDING状态处理函数
BOOL SipCallStatusProceedingHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData);

//呼叫RECV_CALL状态处理函数
BOOL SipCallStatusRecvCallHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData);

//呼叫WAIT_TALKING状态处理函数
BOOL SipCallStatusWaitTalkHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData);

//呼叫TALKING状态处理函数
BOOL SipCallStatusTalkingHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData);

//呼叫TERMINATED状态处理函数
BOOL SipCallStatusTerminatedHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData);


/***************************************事务状态处理相关函数***********************************/

//事务UNDEFINED状态处理函数
BOOL SipTransStatusUndefinedHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData);

//事务IDLE状态处理函数
BOOL SipTransStatusIdleHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData);

//事务OK状态处理函数
BOOL SipTransStatusOKHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData);

//事务FAIL状态处理函数
BOOL SipTransStatusFailHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData);

//事务PROCEEDING状态处理函数
BOOL SipTransStatusProceedingHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData);

//事务RECV_REQ状态处理函数
BOOL SipTransStatusRecvReqHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData);

//事务WAIT_USER_PROCESS状态处理函数
BOOL SipTransStatusWaitUserProcessHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData);

//事务USER_ACK_ARRIVED状态处理函数
BOOL SipTransStatusUserAckArrivedHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData);

//事务USER_ACK_DISCARD状态处理函数
BOOL SipTransStatusUserAckDiscardHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData);


#endif
