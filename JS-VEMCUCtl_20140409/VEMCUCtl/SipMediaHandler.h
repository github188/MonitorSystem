#ifndef   _SIP_MEDIA_HANDLER_H_
#define  _SIP_MEDIA_HANDLER_H_

#include "UtilMedia.h"
#include "SipUtil.h"

/***************************************媒体数据相关函数***********************************/

//音频数据接收处理函数
bool AudioMediaDataRecvHandler(int  nCID,
							                            void* pData,
														int	 nDataLen,
														unsigned int nMediaPayload,
														bool bMark,
														unsigned int nTimestamp,
														unsigned int nSeqNo);

//视频数据接收处理函数
bool VideoMediaDataRecvHandler(int  nCID,
							                            void* pData,
														int	 nDataLen,
														unsigned int nMediaPayload,
														bool bMark,
														unsigned int nTimestamp,
														unsigned int nSeqNo);

//组包
extern CRtUtilH264ReFUA*  g_pRecvH264ReFUA;

#endif