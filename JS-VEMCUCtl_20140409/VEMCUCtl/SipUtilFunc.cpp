#include "stdafx.h"
#include <process.h>
#include <windows.h>
#include "SipUtilFunc.h"
#include "SipStateHandler.h"
#include "SipMediaHandler.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/**************************************************************
 *	 
 *   呼叫状态改变回调函数
 *
 *************************************************************/
 void StateChangeEvent_SipCall(
			IN unsigned int					nCID,
			IN E_RtCallState				    eState,
			IN E_RtCallStateReason		eReason,
			IN void*						        pAppData)
{
	try
	{
		switch (eState)
		{
		case E_RT_CALL_STATE_UNDEFINED:
			SipCallStatusUndefinedHandler(nCID,eReason,pAppData);
			break;

		case E_RT_CALL_STATE_IDLE:
			SipCallStatusIdleHandler(nCID,eReason,pAppData);
			break;

		case E_RT_CALL_STATE_TALKING:
			SipCallStatusTalkingHandler(nCID,eReason,pAppData);
			break;

		case E_RT_CALL_STATE_TERMINATED:
			SipCallStatusTerminatedHandler(nCID,eReason,pAppData);
			break;

		case E_RT_CALL_STATE_INVITING:
			SipCallStatusInvitingHandler(nCID,eReason,pAppData);
			break;

		case E_RT_CALL_STATE_PROCEEDING:
			SipCallStatusProceedingHandler(nCID,eReason,pAppData);
			break;

		case E_RT_CALL_STATE_RECV_CALL:
			SipCallStatusRecvCallHandler(nCID,eReason,pAppData);
			break;

		case E_RT_CALL_STATE_OFFERING:
			SipCallStatusOfferingHandler(nCID,eReason,pAppData);
			break;

		case E_RT_CALL_STATE_WAIT_TALKING:
			SipCallStatusWaitTalkHandler(nCID,eReason,pAppData);
			break;

		default:
			SipCallStatusUndefinedHandler(nCID,eReason,pAppData);
			break;
		}
	}
	catch(...)
	{

	}
}


/**************************************************************
*	 
*   呼叫媒体数据接收回调函数
*
*************************************************************/
void JRtpRecvMediaData_SipCall(	
			OUT int						nCID,
			OUT void*					pData,
			OUT int						nDataLen,
			OUT E_RtMediaType		nMediaType,
			OUT unsigned int			nMediaPayload,
			OUT bool					    bMark,
			OUT unsigned int			nTimestamp,
			OUT unsigned int			nSeqNo)
{
	try
	{
		//if (nMediaType == E_RT_MEDIA_TYPE_VIDEO)
		{
			VideoMediaDataRecvHandler(nCID,pData,nDataLen,nMediaPayload,bMark,nTimestamp,nSeqNo);
		}
		//else if( nMediaType == E_RT_MEDIA_TYPE_AUDIO)//音频数据
		{
			//AudioMediaDataRecvHandler(nCID,pData,nDataLen,nMediaPayload,bMark,nTimestamp,nSeqNo);
		}
	}
	catch(...)
	{

	}
}


/**************************************************************
 *	 
 *   事务状态改变
 *
 *************************************************************/
void StateChangeEvent_SipTrans(IN unsigned int nTID, 
										  IN E_RtTransactionState eState, 
										  IN E_RtTransactionStateReason eReason, 
										  IN void* pAppData)
{
	try
	{
		switch (eState)
		{
		case E_RT_TRANSC_STATE_UNDEFINED:
			SipTransStatusUndefinedHandler(nTID,eReason,pAppData);
			break;

		case E_RT_TRANSC_STATE_IDLE:
			SipTransStatusIdleHandler(nTID,eReason,pAppData);
			break;

		case E_RT_TRANSC_C_STATE_OK:
			SipTransStatusOKHandler(nTID,eReason,pAppData);
			break;

		case E_RT_TRANSC_C_STATE_FAIL:
			SipTransStatusFailHandler(nTID,eReason,pAppData);
			break;

		case E_RT_TRANSC_C_STATE_PROCEEDING:
			SipTransStatusProceedingHandler(nTID,eReason,pAppData);
			break;

		case E_RT_TRANSC_S_STATE_RECV_REQ:
			SipTransStatusRecvReqHandler(nTID,eReason,pAppData);
			break;

		case E_RT_TRANSC_S_STATE_WAIT_USER_PEOCESS:
			SipTransStatusWaitUserProcessHandler(nTID,eReason,pAppData);
			break;

		case E_RT_TRANSC_S_STATE_USER_ACK_ARRIVED:
			SipTransStatusUserAckArrivedHandler(nTID,eReason,pAppData);
			break;

		case E_RT_TRANSC_S_STATE_USER_ACK_DISCARD:
			SipTransStatusUserAckDiscardHandler(nTID,eReason,pAppData);
			break;

		default:
			SipTransStatusUndefinedHandler(nTID,eReason,pAppData);
			break;
		}
	}
	catch(...)
	{

	}
}