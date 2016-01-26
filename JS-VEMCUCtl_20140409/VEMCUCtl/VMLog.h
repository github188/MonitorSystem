#ifndef _VM_LOG_H_
#define _VM_LOG_H_

#include "InfoRecvQueue.h"

class CVMLog
{
public:
	CVMLog();
	virtual ~CVMLog();

public:
	bool InitVmLog();
	bool StartVmLog();
	bool StopVmLog();
	bool CreateVmLog();
	bool WriteVmLog(char *szFormat,...);
	bool UnInitVmlog();
	bool GetVmAppPath();

public:
	bool WriteVmLogFromInfoRecvQueue();


public:
	bool m_bVmLogInitFlag;
	int    m_nVmLogFileRecordCount;
	int    m_nVmLogFileIndex;
	char m_szVmLogFileName[MAX_PATH];
	CRITICAL_SECTION m_csVmLogLock;
	HANDLE m_hVmLogFile;
	HANDLE m_hVmLogThread;
	UINT m_uVmLogThreadID;
	char  m_szAppPath[MAX_PATH];
};

//////////////////////////////////////////////////////////////////////////
unsigned __stdcall _VmLogFileWriteThread(void *arg);

//////////////////////////////////////////////////////////////////////////
extern CVMLog g_VMLog;


#endif