#include "StdAfx.h"
#include <process.h>
#include "SipStateHandler.h"
#include "SipMediaHandler.h"
#include "SpsParse.h"
#include "GWPlayInfoDef.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"
#include "SipCall.h"
#include "VMLog.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/***************************************呼叫状态处理相关函数***********************************/


/********************************************************************************
*  
*  Caller(主叫),Callee(被叫)
*  E_RT_CALL_STATE_UNDEFINED = -1(无效)
*	呼叫UNDEFINED状态处理函数
*
*********************************************************************************/
BOOL SipCallStatusUndefinedHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*  Caller(主叫),Callee(被叫)
*  E_RT_CALL_STATE_IDLE = 0(空闲)
*	呼叫IDLE状态处理函数
*
*********************************************************************************/
BOOL SipCallStatusIdleHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*  Caller(主叫)
*  E_RT_CALL_STATE_INVITING = 3(呼出)
*	呼叫INVITING状态处理函数
*
*********************************************************************************/
BOOL SipCallStatusInvitingHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*   Callee(被叫)
*   E_RT_CALL_STATE_OFFERING = 6(震铃)
*	呼叫OFFERING状态处理函数
*
*********************************************************************************/
BOOL SipCallStatusOfferingHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*   Caller(主叫)
*   E_RT_CALL_STATE_PROCEEDING	= 4(远端处理中 Todo: 增加彩铃状态)
*	呼叫PROCEEDING状态处理函数
*
*********************************************************************************/
BOOL SipCallStatusProceedingHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*   Callee(被叫)
*   E_RT_CALL_STATE_RECV_CALL= 5(收到呼入,@注意无法再该状态下拒绝呼入)
*	呼叫RECV_CALL状态处理函数
*
*********************************************************************************/
BOOL SipCallStatusRecvCallHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*  callee(被叫)
*  E_RT_CALL_STATE_WAIT_TALKING= 7(等待进入通话)
*	呼叫WAIT_TALKING状态处理函数
*
*********************************************************************************/
BOOL SipCallStatusWaitTalkHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*   Caller(主叫),Callee(被叫)
*   E_RT_CALL_STATE_TALKING = 1(通话)
*	呼叫TALKING状态处理函数
*
*********************************************************************************/
BOOL SipCallStatusTalkingHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData)
{
	int nRetCode = 0;
	BOOL bResult = FALSE;
	int nUACID = -1;		//自己是主叫时的ID号
	int nUASID = -1;		//自己是被叫时的ID号
	int nAudioPayload = -1;
	int nVideoPayload = -1;
	char szFmtpConfig[1024] = {0};
	E_RtDirection	eDirection; //呼叫方向
	int nActualLen = 0;
	char *pSPS = NULL;
	char newSps[512] = {0};
	int    newSpsLen = 0;
	HANDLE hDecodeHandle = NULL;
	int nIndex = -1;

	char	szCalleeUser[50] = {0};
	char	szCalleeHost[50] = {0};
	char	szCallerUser[50] = {0};
	char	szCallerHost[50] = {0};

	nRetCode = g_pCallAPI->GetDirection(nCID, &eDirection);
	if(nRetCode != E_RT_OK)
	{
		g_pCallAPI->Hangup(nCID);
		return FALSE;
	}

	if(eDirection == E_RT_DIRECTION_OUT)
	{
		g_pCallAPI->GetCalleeUser(nCID,szCalleeUser,50,&nActualLen);
		g_pCallAPI->GetCalleeHost(nCID,szCalleeHost,50,&nActualLen);
	}
	else if (eDirection == E_RT_DIRECTION_IN)
	{
		g_pCallAPI->GetCallerUser(nCID,szCallerUser,50,&nActualLen);
		g_pCallAPI->GetCallerHost(nCID,szCallerHost,50,&nActualLen);
	}

	// 获取payload值
	g_pCallAPI->GetCurrentPayload(nCID,E_RT_MEDIA_TYPE_AUDIO,&nAudioPayload);
	g_pCallAPI->GetCurrentPayload(nCID,E_RT_MEDIA_TYPE_VIDEO,&nVideoPayload);

	// 获取fmtp值，用于初始化播放解码器
	g_pCallAPI->GetFmtpConfig(nCID,szFmtpConfig,1024);

	g_VMLog.WriteVmLog("建立呼叫--0:nCID = %d,eReason = %d",nCID,eReason);
	
	//bResult = IsStandardH264Info(szFmtpConfig);
	if (nVideoPayload != 100)//100为标准h264
	{
		pSPS = strstr(szFmtpConfig,"sprop-parameter-sets=");
		if (pSPS == NULL) return FALSE;
		
		pSPS += strlen("sprop-parameter-sets=");

		g_VMLog.WriteVmLog("pSPS=%s",pSPS);

		newSpsLen = SwitchSPS(pSPS,(BYTE *)newSps);

		//////////////////////////////////////////////////////////////////////////
		SIP_STATE_HANDLER_INFO SipStateHandlerInfo;
		memset(&SipStateHandlerInfo,0,sizeof(SipStateHandlerInfo));

		SipStateHandlerInfo.nCID = nCID;
		SipStateHandlerInfo.nDirection = (int)eDirection;
		strcpy_s(SipStateHandlerInfo.szCallerNum,sizeof(SipStateHandlerInfo.szCallerNum),szCallerUser);
		strcpy_s(SipStateHandlerInfo.szCalleeNum,sizeof(SipStateHandlerInfo.szCalleeNum),szCalleeUser);
		memcpy(SipStateHandlerInfo.Sps,newSps,newSpsLen);
		SipStateHandlerInfo.nSpsLen = newSpsLen;
		SipStateHandlerInfo.nVideoPayload = nVideoPayload;
		SipStateHandlerInfo.nReason = (int)eReason;
		SipStateHandlerInfo.pAppData = pAppData;

		g_VMLog.WriteVmLog("建立呼叫--1:nCID = %d,eReason = %d,nVideoPayload= %d,CalleeUser=%s",nCID,eReason,nVideoPayload,szCalleeUser);

		g_VMLog.WriteVmLog("SipStateHandlerInfo.nSpsLen=%d",SipStateHandlerInfo.nSpsLen);

		ThreadSipStateHandlerTalking(&SipStateHandlerInfo);

		g_VMLog.WriteVmLog("建立呼叫--2:nCID = %d,eReason = %d,nVideoPayload= %d,CalleeUser=%s",nCID,eReason,nVideoPayload,szCalleeUser);
	
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		SIP_STATE_HANDLER_INFO SipStateHandlerInfo;
		memset(&SipStateHandlerInfo,0,sizeof(SipStateHandlerInfo));

		SipStateHandlerInfo.nCID = nCID;
		SipStateHandlerInfo.nDirection = (int)eDirection;
		strcpy_s(SipStateHandlerInfo.szCallerNum,sizeof(SipStateHandlerInfo.szCallerNum),szCallerUser);
		strcpy_s(SipStateHandlerInfo.szCalleeNum,sizeof(SipStateHandlerInfo.szCalleeNum),szCalleeUser);
		SipStateHandlerInfo.nSpsLen = 0;
		SipStateHandlerInfo.nVideoPayload = nVideoPayload;
		SipStateHandlerInfo.nReason = (int)eReason;
		SipStateHandlerInfo.pAppData = pAppData;

		g_VMLog.WriteVmLog("建立呼叫--1:nCID = %d,eReason = %d,nVideoPayload= %d,CalleeUser=%s",nCID,eReason,nVideoPayload,szCalleeUser);

		g_VMLog.WriteVmLog("SipStateHandlerInfo.nSpsLen=%d",SipStateHandlerInfo.nSpsLen);

		ThreadSipStateHandlerTalking(&SipStateHandlerInfo);

		g_VMLog.WriteVmLog("建立呼叫--2:nCID = %d,eReason = %d,nVideoPayload= %d,CalleeUser=%s",nCID,eReason,nVideoPayload,szCalleeUser);
	}

	return TRUE;
}


/********************************************************************************
*
*   Caller(主叫),Callee(被叫)
*   E_RT_CALL_STATE_TERMINATED = 2(终止)
*	呼叫TERMINATED状态处理函数
*
*********************************************************************************/
BOOL SipCallStatusTerminatedHandler(unsigned int nCID,E_RtCallStateReason eReason,void* pAppData)
{
	if (g_pMainDlg == NULL)
		return FALSE;

	//////////////////////////////////////////////////////////////////////////
	SIP_STATE_HANDLER_INFO SipStateHandlerInfo;
	memset(&SipStateHandlerInfo,0,sizeof(SipStateHandlerInfo));

	SipStateHandlerInfo.nCID = nCID;
	SipStateHandlerInfo.nReason = (int)eReason;
	SipStateHandlerInfo.pAppData = pAppData;

	g_VMLog.WriteVmLog("结束呼叫--1:nCID = %d,eReason = %d",nCID,eReason);

	ThreadSipStateHandlerTerminate(&SipStateHandlerInfo);

	g_VMLog.WriteVmLog("结束呼叫--2:nCID = %d,eReason = %d",nCID,eReason);


	return TRUE;
}


/***************************************事务状态处理相关函数***********************************/


/********************************************************************************
*
*  Sender,Receiver
*  E_RT_TRANSC_STATE_UNDEFINED= -1(无效)
*	事务UNDEFINED状态处理函数
*
*********************************************************************************/
BOOL SipTransStatusUndefinedHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*   Sender,Receiver
*   E_RT_TRANSC_STATE_IDLE = 0(空闲)
*	事务IDLE状态处理函数
*
*********************************************************************************/
BOOL SipTransStatusIdleHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*  Sender
*  E_RT_TRANSC_C_STATE_OK= 1(成功)
*	事务OK状态处理函数
*
*********************************************************************************/
BOOL SipTransStatusOKHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*  Sender
*  E_RT_TRANSC_C_STATE_FAIL = 2(失败)
*	事务FAIL状态处理函数
*
*********************************************************************************/
BOOL SipTransStatusFailHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData)
{

	return TRUE;
}

/********************************************************************************
*
*  Sender
*  E_RT_TRANSC_C_STATE_PROCEEDING = 3(事务进行中)
*	事务PROCEEDING状态处理函数
*
*********************************************************************************/
BOOL SipTransStatusProceedingHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*  Receiver
*  E_RT_TRANSC_S_STATE_RECV_REQ = 4(收到请求)
*	事务RECV_REQ状态处理函数
*
*********************************************************************************/
BOOL SipTransStatusRecvReqHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*  Receiver
*  E_RT_TRANSC_S_STATE_WAIT_USER_PEOCESS = 5(等待用户处理)
*	事务WAIT_USER_PROCESS状态处理函数
*
*********************************************************************************/
BOOL SipTransStatusWaitUserProcessHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*  Receiver
*  E_RT_TRANSC_S_STATE_USER_ACK_ARRIVED = 6(响应已送达)
*	事务USER_ACK_ARRIVED状态处理函数
*
*********************************************************************************/
BOOL SipTransStatusUserAckArrivedHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData)
{
	return TRUE;
}


/********************************************************************************
*
*  Receiver
*  E_RT_TRANSC_S_STATE_USER_ACK_DISCARD = 7(响应消息丢失)
*	事务USER_ACK_DISCARD状态处理函数
*
*********************************************************************************/
BOOL SipTransStatusUserAckDiscardHandler(unsigned int nTID,E_RtTransactionStateReason eReason,void* pAppData)
{
	return TRUE;
}



