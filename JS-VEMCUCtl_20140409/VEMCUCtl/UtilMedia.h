/********************************************************************
 *
 *	Copyright (c) 2010 by SniffXu.
 *
 *	Name		: UtilMedia.h
 *
 *	Purpose		:提供视频数据分包重组函数
 *
 *
 ********************************************************************/
#ifndef _RT_SIP_STACK_FILE_H_UTIL_VIDEO_MEDIA_
#define _RT_SIP_STACK_FILE_H_UTIL_VIDEO_MEDIA_

#include <stdio.h>
#include <string.h>

#if defined(WIN32)
	#include <windows.h>
#else
	#include  <unistd.h>
	#include  <fcntl.h>
	#include  <sys/types.h>
	#include  <sys/stat.h>
	#include  <stdlib.h>
	#include  <errno.h>
#endif

/**************************************************************
 *	 
 *  重组接收到的视频数据
 *
 *************************************************************/
class CRtUtilH264ReFUA
{
public:
	CRtUtilH264ReFUA(unsigned int nMaxFrameLen);
	~CRtUtilH264ReFUA();

	void	  PushData(unsigned char* pData, int nDataLen, bool bMark, unsigned nTS, unsigned nSEQ);
	unsigned  PopFrame(unsigned char** ppFrame);

	void	  PhonePushData(unsigned char* pData, int nDataLen, bool bMark, unsigned nTS, unsigned nSEQ);
	unsigned  PhonePopFrame(unsigned char** ppFrame);

	void	  H264PushData(unsigned char* pData, int nDataLen, bool bMark, unsigned nTS, unsigned nSEQ);
	void	  H264PushData4GrandstreamCL3504Dvs(unsigned char* pData, int nDataLen, bool bMark, unsigned nTS, unsigned nSEQ); //潮流CL3504 DVS采用海思芯片会发送非标准FUA格式的RTP包
	unsigned  H264PopFrame(unsigned char** ppFrame);

private:
	bool			    m_bWaitNextPacket;//是否需要等待mark包
	unsigned char*	 m_pInternalAddr;//内部缓冲区
	unsigned		m_nFrameLen;//数据长度
	unsigned		m_nCurDataOffset;//当前数据偏移量
	unsigned		m_nTimestamp;//时间戳
	unsigned		m_nSeqNo;//序列号
	unsigned		m_nMaxBufferSize;//最大缓冲区大小
};

#endif //_RT_SIP_STACK_FILE_H_UTIL_VIDEO_MEDIA_

