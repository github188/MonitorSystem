#include "StdAfx.h"
#include "SipMediaHandler.h"
#include "GWPlayInfoDef.h"
#include "GWPlaySDK.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/**************************************************************
 *	 
 *   音频数据接收处理函数
 *
 *************************************************************/
bool AudioMediaDataRecvHandler(int  nCID,
							   void* pData,
							   int	 nDataLen,
							   unsigned int nMediaPayload,
							   bool bMark,
							   unsigned int nTimestamp,
							   unsigned int nSeqNo)
{
	if (nDataLen <= 0)
		return false;

	return true;
}


/**************************************************************
 *	 
 *   视频数据接收处理函数
 *
 *************************************************************/
bool VideoMediaDataRecvHandler(int  nCID,
							   void* pData,
							   int	 nDataLen,
							   unsigned int nMediaPayload,
							   bool bMark,
							   unsigned int nTimestamp,
							   unsigned int nSeqNo)
{
	if (nDataLen <= 0)
		return false;

	try
	{
		GW_PLAY_CHANNEL_INFO *pPlayChannelInfo = NULL;
		TStreamDataParams DataParam;

		int nIndex = g_pMainDlg->SearchIndexCallByCID(nCID);

		if (nIndex < 0||nIndex >= MAXVIEWCH)
			return false;

		 g_DlgVideoView[nIndex].SaveRecordFile(pData,nDataLen);

		 g_DlgVideoView[nIndex].RecvVideoFlow(nDataLen);//流量

		pPlayChannelInfo = g_DlgVideoView[nIndex].m_pPlayChannelInfo;
		if (pPlayChannelInfo == NULL)
			return false;

		DataParam.pBuffer = (BYTE*)pData;
		DataParam.nSize = nDataLen;

		//return true;

		//////////////////////////////////////////////////////////////////////////
		pPlayChannelInfo->Lock();
		try
		{
			if ((int)pPlayChannelInfo->m_hDecodeHandle > 0 && pPlayChannelInfo->m_nDecodeType > 0 && pPlayChannelInfo->m_bVideoStartOrStop == true && pPlayChannelInfo->m_nCid == nCID)
			{
				g_GWPlay.StreamInputData(pPlayChannelInfo->m_nDecodeType,pPlayChannelInfo->m_hDecodeHandle,&DataParam);
			}
		}
		catch(...)
		{
		}
		pPlayChannelInfo->Unlock();
		//////////////////////////////////////////////////////////////////////////

		return true ;
	}
	catch(...)
	{

	}

	return false;
}