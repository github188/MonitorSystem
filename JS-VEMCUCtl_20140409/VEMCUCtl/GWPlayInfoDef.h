#ifndef _GW_PLAY_INFO_DEF_H_
#define _GW_PLAY_INFO_DEF_H_

//////////////////////////////////////////////////////////////////////////
#define GW_PLAY_CODE_LEN 64
#define GW_PLAY_NAME_LEN 256

//////////////////////////////////////////////////////////////////////////
#define  GW_PLAY_FlAG_UNDEFINE -1
#define  GW_PLAY_FLAG_INIT 0
#define  GW_PLAY_FLAG_STREAM_PLAY 1
#define  GW_PLAY_FLAG_STREAM_PAUSE 2
#define  GW_PLAY_FLAG_FILE_PLAY 3
#define  GW_PLAY_FLAG_FILE_PAUSE 4


//////////////////////////////////////////////////////////////////////////


//解码通道信息
class GW_PLAY_CHANNEL_INFO
{
public:
	GW_PLAY_CHANNEL_INFO();
	~GW_PLAY_CHANNEL_INFO();

public:
	bool Lock();
	bool Unlock();

	bool Empty();
	bool StartPlay();
	bool StopPlay();
	bool FastPlay();
	bool SlowPlay();
	bool PausePlay();
	bool ResumePlay();

	bool SetDecodeType(int nDecodeType);
	bool SetSpsInfo(char *pSps,int nSpsLen);
	bool SetFileName(char *szFileName);
	bool FileStartPlay();
	bool FileStopPlay();
	bool SetPos(float fRealPos);
	bool GetPos(float *pfRealPos);

	bool GetVideoParams(int &nBrightness,int &nContrast,int &nSaturation,int &nHue);
	bool SetVideoParams(int nBrightness,int nContrast,int nSaturation,int nHue,bool bDefault);


	bool SetData(char *pDeviceCode,char *szDeviceName,int nChannelNum,int nDecodeType,
		                  char *pSps,int nSpsLen,HWND hPlayWnd,HANDLE hDecodeHandle,int nFlag);

public:
	CRITICAL_SECTION m_cs;
	int    m_nCid;
	char m_szDeviceCode[GW_PLAY_CODE_LEN];
	char m_szDeviceName[GW_PLAY_NAME_LEN];
	int    m_nChannelNum;
	int    m_nDecodeType;
	char m_Sps[512];
	int    m_nSpsLen;
	HWND m_hPlayWnd;
	HANDLE m_hDecodeHandle;
	int    m_nFlag;
	char m_szFileName[256];
	bool m_bVideoStartOrStop;//  视频的开关，用于表示当前视频播放句柄在释放前先停止流的播放
};


#endif