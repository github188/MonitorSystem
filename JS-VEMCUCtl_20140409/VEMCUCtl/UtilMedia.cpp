/********************************************************************
 *
 *	Copyright (c) 2010 by SniffXu.
 *
 *	Name		: UtilMedia.cpp
 *
 *	Purpose		:提供视频数据分包重组函数
 *
 *
 ********************************************************************/
#include "stdafx.h"
#include "UtilMedia.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRtUtilH264ReFUA::CRtUtilH264ReFUA(unsigned int nMaxFrameLen)
{
#if defined(__WIN32__) || defined(_WIN32)
	m_pInternalAddr = (unsigned char*)GlobalAlloc(GMEM_FIXED, nMaxFrameLen);
#else
	m_pInternalAddr = (unsigned char*)malloc(nMaxFrameLen);
#endif
	memset(m_pInternalAddr, 0x00, nMaxFrameLen);
	m_bWaitNextPacket = false;
	m_nFrameLen = 0;
	m_nCurDataOffset = 0;
	m_nTimestamp = 0;
	m_nSeqNo = -1;
	m_nMaxBufferSize = nMaxFrameLen;
}

CRtUtilH264ReFUA::~CRtUtilH264ReFUA()
{
	if(m_pInternalAddr != NULL)
	{
#if defined(__WIN32__) || defined(_WIN32)
		GlobalFree(m_pInternalAddr);
#else
		free(m_pInternalAddr);
#endif
		m_pInternalAddr = NULL;
	}
}

void CRtUtilH264ReFUA::PushData(
	unsigned char*	pData, 
	int				nDataLen, 
	bool			    bMark,
	unsigned		nTS, 
	unsigned		nSEQ)
{
	if(pData==NULL || m_pInternalAddr==NULL)
	{
		TRACE("<ERROR> The data is null");
		return;
	}

	__try
	{
		if(m_nCurDataOffset+nDataLen > m_nMaxBufferSize)
		{
			TRACE( "<ERROR> The buffer will overlow");
			memset(m_pInternalAddr, 0x00, m_nMaxBufferSize);
			m_bWaitNextPacket = false;
			m_nFrameLen = 0;
			m_nCurDataOffset = 0;
			m_nTimestamp = 0;
			m_nSeqNo = -1;
			return;
		}

		if(m_nSeqNo == -1||m_nTimestamp != nTS) //第1次收包或下一包开始
		{
			m_nCurDataOffset = 0;
			m_nFrameLen = 0;
		}
		else if(m_nTimestamp == nTS) //中间包
		{
			if (nSEQ > m_nSeqNo+1||(nSEQ <= m_nSeqNo&& nSEQ+600 > m_nSeqNo)||m_bWaitNextPacket != true)//发生丢包,清空相同时间戳的前面的包,跳过相同时间戳的后面的包
			{
				m_nFrameLen = 0;
				m_nSeqNo = nSEQ;
				m_nTimestamp = nTS;
				m_bWaitNextPacket = false;
				m_nCurDataOffset = 0;
				return;
			}
		}

		memmove(&m_pInternalAddr[m_nCurDataOffset], pData, nDataLen);

		m_nFrameLen += nDataLen;
		m_nSeqNo = nSEQ;
		m_nTimestamp = nTS;

		if(bMark == true)
		{
			m_bWaitNextPacket = false;
			m_nCurDataOffset = 0;
		}
		else
		{
			m_bWaitNextPacket = true;
			m_nCurDataOffset = m_nFrameLen;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("\n************************************************************\n");
		TRACE("PushData组包异常!\n");
		TRACE("************************************************************\n");
	}

	return;
}

unsigned CRtUtilH264ReFUA::PopFrame(unsigned char** ppFrame)
{
	if(m_nFrameLen == 0 || m_bWaitNextPacket == true||ppFrame == NULL)
	{
		return 0;
	}

	*ppFrame = m_pInternalAddr;

	return m_nFrameLen;
}


void CRtUtilH264ReFUA::PhonePushData(
								unsigned char*	pData, 
								int				nDataLen, 
								bool			    bMark,
								unsigned		nTS, 
								unsigned		nSEQ)
{
	unsigned char	startBit = 0;
	unsigned char	endBit = 0;
	unsigned char	CurPacketNALUnitType = 0;

	if(pData == NULL || m_pInternalAddr == NULL)
	{
		TRACE( "<ERROR> The Data is NULL\n");
		return;
	}
	__try
	{
		if(m_nCurDataOffset+nDataLen+4 > m_nMaxBufferSize)
		{
			TRACE( "<ERROR> The Buffer will Overlow\n");
			memset(m_pInternalAddr, 0x00, m_nMaxBufferSize);
			m_bWaitNextPacket = false;
			m_nFrameLen = 0;
			m_nCurDataOffset = 0;
			m_nTimestamp = 0;
			m_nSeqNo = -1;
			return;
		}

		CurPacketNALUnitType = (pData[0]&0x1F);
		if(CurPacketNALUnitType!=28 && CurPacketNALUnitType!=29)
		{
			if(CurPacketNALUnitType!=1 && CurPacketNALUnitType!=5)
			{
				if(m_nSeqNo==-1)
				{
					m_nCurDataOffset = 0;
					m_nFrameLen = (nDataLen+4);
				}
				else if(m_nTimestamp==nTS)
				{
					m_nCurDataOffset = m_nFrameLen;
					m_nFrameLen += (nDataLen+4);
				}
				else 
				{
					m_nCurDataOffset = 0;
					m_nFrameLen = (nDataLen+4);
				}

				m_pInternalAddr[m_nCurDataOffset] = 0x0;
				m_pInternalAddr[m_nCurDataOffset+1] = 0x0;
				m_pInternalAddr[m_nCurDataOffset+2] = 0x0;
				m_pInternalAddr[m_nCurDataOffset+3] = 0x1;
				memmove(&m_pInternalAddr[m_nCurDataOffset+4], pData, nDataLen);

				m_nSeqNo = nSEQ;
				m_nTimestamp = nTS;
				m_bWaitNextPacket = true;
				m_nCurDataOffset = m_nFrameLen;
			}
			else
			{
				if((m_nTimestamp==nTS) || (m_nSeqNo==-1)) 
				{
					m_pInternalAddr[m_nCurDataOffset] = 0x0;
					m_pInternalAddr[m_nCurDataOffset+1] = 0x0;
					m_pInternalAddr[m_nCurDataOffset+2] = 0x0;
					m_pInternalAddr[m_nCurDataOffset+3] = 0x1;
					memmove(&m_pInternalAddr[m_nCurDataOffset+4], pData, nDataLen);

					m_nSeqNo = nSEQ;
					m_nTimestamp = nTS;
					m_nFrameLen += (nDataLen+4);

					if(bMark == true) 
					{
						m_bWaitNextPacket = false;
						m_nCurDataOffset = 0;
					}
					else
					{
						m_bWaitNextPacket = true;
						m_nCurDataOffset = m_nFrameLen;
					}
				}
				else 
				{
					m_nCurDataOffset = 0;

					m_pInternalAddr[m_nCurDataOffset] = 0x0;
					m_pInternalAddr[m_nCurDataOffset+1] = 0x0;
					m_pInternalAddr[m_nCurDataOffset+2] = 0x0;
					m_pInternalAddr[m_nCurDataOffset+3] = 0x1;
					memmove(&m_pInternalAddr[m_nCurDataOffset+4], pData, nDataLen);

					m_nSeqNo = nSEQ;
					m_nTimestamp = nTS;
					m_nFrameLen = (nDataLen+4);

					if(bMark == true) 
					{
						m_bWaitNextPacket = false;
						m_nCurDataOffset = 0;
					}
					else
					{
						m_bWaitNextPacket = true;
						m_nCurDataOffset = m_nFrameLen;
					}
				}
			}

			return;
		}

		startBit = pData[1]&0x80;
		endBit = pData[1]&0x40;

		if(startBit)
		{
			if((m_nTimestamp!=nTS) && (m_nSeqNo!=-1))
			{
				m_nCurDataOffset = 0;
				m_nFrameLen = 0;
			}

			m_pInternalAddr[m_nCurDataOffset] = 0x0;
			m_pInternalAddr[m_nCurDataOffset+1] = 0x0;
			m_pInternalAddr[m_nCurDataOffset+2] = 0x0;
			m_pInternalAddr[m_nCurDataOffset+3] = 0x1;
			m_pInternalAddr[m_nCurDataOffset+4] = (pData[0]&0xE0)+(pData[1]&0x1F);
			memmove(&m_pInternalAddr[m_nCurDataOffset+5], pData+2, nDataLen-2);

			m_nFrameLen += (nDataLen-2+5);
			m_nSeqNo = nSEQ;
			m_nTimestamp = nTS;
			m_bWaitNextPacket = true;
			m_nCurDataOffset = m_nFrameLen;
		}
		else 
		{
			if(m_nTimestamp!=nTS || m_bWaitNextPacket!=true)
			{
				TRACE("<ERROR> The RTP Loss Packet(%d)\n", nSEQ-m_nSeqNo);
				m_nFrameLen = 0;
				m_nSeqNo = nSEQ;
				m_nTimestamp = nTS;
				m_bWaitNextPacket = false;
				m_nCurDataOffset = 0;
			}
			else
			{
				memmove(&m_pInternalAddr[m_nCurDataOffset], pData+2, nDataLen-2);
				m_nFrameLen += (nDataLen-2);			
				m_nSeqNo = nSEQ;
				m_nTimestamp = nTS;		
				if(endBit)
				{
					m_bWaitNextPacket = false;
					m_nCurDataOffset = 0;
				}
				else
				{
					m_bWaitNextPacket = true;
					m_nCurDataOffset = m_nFrameLen;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("\n************************************************************\n");
		TRACE("PhonePushData组包异常!\n");
		TRACE("************************************************************\n");
	}
	return;
}

unsigned CRtUtilH264ReFUA::PhonePopFrame(unsigned char** ppFrame)
{
	if(m_nFrameLen==0 || m_bWaitNextPacket == true||ppFrame == NULL)
	{
		return 0;
	}

	*ppFrame = m_pInternalAddr;

	return m_nFrameLen;
}

void CRtUtilH264ReFUA::H264PushData(
	unsigned char*	pData, 
	int				    nDataLen, 
	bool			        bMark,
	unsigned		    nTS, 
	unsigned		    nSEQ)
{
	unsigned char	startBit = 0;
	unsigned char	endBit = 0;
	unsigned char	CurPacketNALUnitType = 0;

	if(pData==NULL || m_pInternalAddr==NULL)
	{
		TRACE( "<ERROR> The Data is NULL\n");
		return;
	}

	__try
	{
		if(m_nCurDataOffset+nDataLen+4 > m_nMaxBufferSize)
		{
			TRACE( "<ERROR> The Buffer will Overlow\n");
			memset(m_pInternalAddr, 0x00, m_nMaxBufferSize);
			m_bWaitNextPacket = false;
			m_nFrameLen = 0;
			m_nCurDataOffset = 0;
			m_nTimestamp = 0;
			m_nSeqNo = -1;
			return;
		}

		CurPacketNALUnitType = (pData[0]&0x1F);

		if(CurPacketNALUnitType != 28 && CurPacketNALUnitType != 29)
		{
			if(CurPacketNALUnitType != 1 && CurPacketNALUnitType != 5)
			{
				if((m_nSeqNo == -1)||(m_nTimestamp != nTS))//第一次或下个时间戳开始
				{
					m_nCurDataOffset = 0;
					m_nFrameLen = 0;
				}
				else if(m_nTimestamp == nTS)//相同时间戳,相同帧
				{
					if (nSEQ > m_nSeqNo+1||(nSEQ <= m_nSeqNo&& nSEQ+600 > m_nSeqNo)||m_bWaitNextPacket != true)//发生丢包,清空相同时间戳的前面的包,跳过相同时间戳的后面的包
					{
						m_nFrameLen = 0;
						m_nSeqNo = nSEQ;
						m_nTimestamp = nTS;
						m_bWaitNextPacket = false;
						m_nCurDataOffset = 0;
						return;
					}
				}

				m_pInternalAddr[m_nCurDataOffset] = 0x0;
				m_pInternalAddr[m_nCurDataOffset+1] = 0x0;
				m_pInternalAddr[m_nCurDataOffset+2] = 0x0;
				m_pInternalAddr[m_nCurDataOffset+3] = 0x1;
				memmove(&m_pInternalAddr[m_nCurDataOffset+4], pData, nDataLen);

				m_nFrameLen += (nDataLen+4);
				m_nSeqNo = nSEQ;
				m_nTimestamp = nTS;
				m_bWaitNextPacket = true;
				m_nCurDataOffset = m_nFrameLen;
			}
			else//(CurPacketNALUnitType == 1||CurPacketNALUnitType == 5)
			{
				if ((m_nSeqNo == -1)||(m_nTimestamp != nTS))//第一次或下个时间戳开始
				{
					m_nCurDataOffset = 0;
					m_nFrameLen = 0;
				}
				else if(m_nTimestamp == nTS)//相同时间戳
				{
					if (nSEQ > m_nSeqNo+1||(nSEQ <= m_nSeqNo&& nSEQ+600 > m_nSeqNo)||m_bWaitNextPacket != true)//发生丢包,清空相同时间戳的前面的包,跳过相同时间戳的后面的包
					{
						m_nFrameLen = 0;
						m_nSeqNo = nSEQ;
						m_nTimestamp = nTS;
						m_bWaitNextPacket = false;
						m_nCurDataOffset = 0;
						return;
					}
				}

				m_pInternalAddr[m_nCurDataOffset] = 0x0;
				m_pInternalAddr[m_nCurDataOffset+1] = 0x0;
				m_pInternalAddr[m_nCurDataOffset+2] = 0x0;
				m_pInternalAddr[m_nCurDataOffset+3] = 0x1;
				memmove(&m_pInternalAddr[m_nCurDataOffset+4], pData, nDataLen);

				m_nFrameLen += (nDataLen+4);
				m_nSeqNo = nSEQ;
				m_nTimestamp = nTS;

				if(bMark == true) 
				{
					m_bWaitNextPacket = false;
					m_nCurDataOffset = 0;
				}
				else
				{
					m_bWaitNextPacket = true;
					m_nCurDataOffset = m_nFrameLen;
				}
			}

			return;
		}

		startBit = pData[1]&0x80;
		endBit = pData[1]&0x40;

		if(startBit)//FU开始包
		{
			if ((m_nSeqNo == -1)||(m_nTimestamp != nTS))//第一次或下个时间戳开始
			{
				m_nCurDataOffset = 0;
				m_nFrameLen = 0;
			}
			else if (m_nTimestamp == nTS)//中间包,前面有SPS、PPS帧
			{
				if (nSEQ > m_nSeqNo+1||(nSEQ <= m_nSeqNo&& nSEQ+600 > m_nSeqNo)||m_bWaitNextPacket != true)//发生丢包,清空相同时间戳的前面的包,跳过相同时间戳的后面的包
				{
					m_nFrameLen = 0;//清空前面的包
					m_nSeqNo = nSEQ;
					m_nTimestamp = nTS;
					m_bWaitNextPacket = false;//跳过后面的包
					m_nCurDataOffset = 0;//清空前面的包
					return;
				}
			}

			m_pInternalAddr[m_nCurDataOffset] = 0x0;
			m_pInternalAddr[m_nCurDataOffset+1] = 0x0;
			m_pInternalAddr[m_nCurDataOffset+2] = 0x0;
			m_pInternalAddr[m_nCurDataOffset+3] = 0x1;
			m_pInternalAddr[m_nCurDataOffset+4] = (pData[0]&0xE0)+(pData[1]&0x1F);
			memmove(&m_pInternalAddr[m_nCurDataOffset+5], pData+2, nDataLen-2);

			m_nFrameLen += (nDataLen-2+5);
			m_nSeqNo = nSEQ;
			m_nTimestamp = nTS;
			m_bWaitNextPacket = true;
			m_nCurDataOffset = m_nFrameLen;
		}
		else//FU后续包,包括中间包和结束包 
		{
			if(m_nTimestamp != nTS||m_bWaitNextPacket != true||nSEQ > m_nSeqNo+1||(nSEQ <= m_nSeqNo&& nSEQ+600 > m_nSeqNo))
			{
				m_nFrameLen = 0;
				m_nSeqNo = nSEQ;
				m_nTimestamp = nTS;
				m_bWaitNextPacket = false;
				m_nCurDataOffset = 0;
				return;
			}

			memmove(&m_pInternalAddr[m_nCurDataOffset], pData+2, nDataLen-2);
			m_nFrameLen += (nDataLen-2);			
			m_nSeqNo = nSEQ;
			m_nTimestamp = nTS;		

			if(endBit)//FU结束包
			{
				m_bWaitNextPacket = false;
				m_nCurDataOffset = 0;
			}
			else
			{
				m_bWaitNextPacket = true;
				m_nCurDataOffset = m_nFrameLen;
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("\n************************************************************\n");
		TRACE("H264PushData组包异常!\n");
		TRACE("************************************************************\n");
	}

	return;
}

void CRtUtilH264ReFUA::H264PushData4GrandstreamCL3504Dvs( //潮流CL3504 DVS采用海思芯片会发送非标准FUA格式的RTP包
	unsigned char*	pData, 
	int				nDataLen, 
	bool			    bMark,
	unsigned		nTS, 
	unsigned		nSEQ)
{
	unsigned char	startBit = 0;
	unsigned char	endBit = 0;
	unsigned char	CurPacketNALUnitType = 0;

	if(pData==NULL || m_pInternalAddr==NULL)
	{
		TRACE( "<ERROR> The data is null\n");
		return;
	}

	__try
	{
		if(m_nCurDataOffset+nDataLen+4 > m_nMaxBufferSize)
		{
			TRACE( "<ERROR> The buffer will overlow\n");
			memset(m_pInternalAddr, 0x00, m_nMaxBufferSize);
			m_bWaitNextPacket = false;
			m_nFrameLen = 0;
			m_nCurDataOffset = 0;
			m_nTimestamp = 0;
			m_nSeqNo = -1;
			return;
		}

		CurPacketNALUnitType = (pData[0]&0x1F);
		if(CurPacketNALUnitType != 28 && CurPacketNALUnitType != 29)
		{
			TRACE( "<ERROR> This data seems not generated by CL3504\n");
			return;
		}

		CurPacketNALUnitType = (pData[1]&0x1F);
		startBit = pData[1]&0x80;
		endBit = pData[1]&0x40;

		if(startBit) 
		{
			if((m_nTimestamp!=nTS) && (m_nSeqNo!=-1))
			{
				m_nCurDataOffset = 0;
				m_nFrameLen = 0;
			}

			m_pInternalAddr[m_nCurDataOffset] = 0x0;
			m_pInternalAddr[m_nCurDataOffset+1] = 0x0;
			m_pInternalAddr[m_nCurDataOffset+2] = 0x0;
			m_pInternalAddr[m_nCurDataOffset+3] = 0x1;
			m_pInternalAddr[m_nCurDataOffset+4] = (pData[0]&0xE0)+(pData[1]&0x1F);
			memmove(&m_pInternalAddr[m_nCurDataOffset+5], pData+2, nDataLen-2);
			m_nFrameLen += (nDataLen-2+5);
			m_nSeqNo = nSEQ;
			m_nTimestamp = nTS;

			if(CurPacketNALUnitType != 1&&CurPacketNALUnitType != 5)
			{
				m_bWaitNextPacket = true;
				m_nCurDataOffset = m_nFrameLen;
			}
			else
			{
				if(bMark == true)
				{
					m_bWaitNextPacket = false;
					m_nCurDataOffset = 0;
				}
				else
				{
					m_bWaitNextPacket = true;
					m_nCurDataOffset = m_nFrameLen;
				}
			}
		}
		else 
		{
			if(m_nTimestamp!=nTS || m_bWaitNextPacket!=true)
			{
				TRACE("<ERROR> The rtp loss packet(%d)\n", nSEQ-m_nSeqNo);
				m_nFrameLen = 0;
				m_nSeqNo = nSEQ;
				m_nTimestamp = nTS;
				m_bWaitNextPacket = false;
				m_nCurDataOffset = 0;
			}
			else
			{
				memmove(&m_pInternalAddr[m_nCurDataOffset], pData+2, nDataLen-2);
				m_nFrameLen += (nDataLen-2);			
				m_nSeqNo = nSEQ;
				m_nTimestamp = nTS;		
				if(endBit)
				{
					m_bWaitNextPacket = false;
					m_nCurDataOffset = 0;
				}
				else
				{
					m_bWaitNextPacket = true;
					m_nCurDataOffset = m_nFrameLen;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE("\n************************************************************\n");
		TRACE("H264PushData4GrandstreamCL3504Dvs组包异常!\n");
		TRACE("************************************************************\n");
	}
	
	return;
}

unsigned CRtUtilH264ReFUA::H264PopFrame(unsigned char** ppFrame)
{
	if(m_nFrameLen == 0 || m_bWaitNextPacket == true||ppFrame == NULL)
	{
		return 0;
	}

	*ppFrame = m_pInternalAddr;

	return m_nFrameLen;
}
