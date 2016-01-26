#include "stdafx.h"
#include "VMLog.h"

//////////////////////////////////////////////////////////////////////////
#define  VM_FILE_RECORD_MAX_COUNT  20000
#define  VM_FILE_MAX_INDEX  256


//////////////////////////////////////////////////////////////////////////
CVMLog g_VMLog;
Recv_Buf_Queue g_FileRecvBufQueue;
volatile bool g_bVmLogFileWriteThreadExitFlag = false;
Recv_Buf_Node_Type g_FileWritEnNode;
Recv_Buf_Node_Type g_ThreadFileWriteDeNode;

//////////////////////////////////////////////////////////////////////////

CVMLog::CVMLog()
{
	m_bVmLogInitFlag = false;
	m_nVmLogFileRecordCount = 0;
	m_nVmLogFileIndex = 0;
	strcpy_s(m_szVmLogFileName,sizeof(m_szVmLogFileName),"VMLog_0.txt");
	m_hVmLogFile = INVALID_HANDLE_VALUE;
	m_hVmLogThread = NULL;
	m_uVmLogThreadID = 0;
	InitVmLog();
	StartVmLog();
}

CVMLog::~CVMLog()
{
	StopVmLog();
	UnInitVmlog();
	m_bVmLogInitFlag = false;
	m_nVmLogFileRecordCount = 0;
	m_nVmLogFileIndex = 0;
	strcpy_s(m_szVmLogFileName,sizeof(m_szVmLogFileName),"VMLog_0.txt");
	m_hVmLogFile = INVALID_HANDLE_VALUE;
}

bool CVMLog::InitVmLog()
{
	if (m_bVmLogInitFlag)
		return true;

	GetVmAppPath();

	InitializeCriticalSection(&m_csVmLogLock);

	m_nVmLogFileIndex = 0;
	m_nVmLogFileRecordCount = 0;

	InitRecvQueue(g_FileRecvBufQueue);

	memset(&g_FileWritEnNode,0,sizeof(g_FileWritEnNode));
	memset(&g_ThreadFileWriteDeNode,0,sizeof(g_ThreadFileWriteDeNode));

	m_bVmLogInitFlag = true;

	return true;
}

bool CVMLog::StartVmLog()
{
	m_hVmLogThread = (HANDLE)_beginthreadex(NULL, 0,_VmLogFileWriteThread,(void*)this, 0, &m_uVmLogThreadID);
	if (m_hVmLogThread == 0)
		return false;
	return true;
}

bool CVMLog::StopVmLog()
{
	g_bVmLogFileWriteThreadExitFlag = true;

	DWORD dwResult = WaitForSingleObject(m_hVmLogThread,6000);
	if (dwResult != WAIT_OBJECT_0)
	{
		TerminateThread(m_hVmLogThread,1);
	}

	m_hVmLogThread = NULL;
	m_uVmLogThreadID = 0;

	return true;
}

bool CVMLog::CreateVmLog()
{
	try
	{
		char szFileFullName[512] = {0};
		memset(szFileFullName,0,sizeof(szFileFullName));

		if (m_nVmLogFileRecordCount > VM_FILE_RECORD_MAX_COUNT)
		{
			if (m_hVmLogFile != INVALID_HANDLE_VALUE)
			{
				CloseHandle(m_hVmLogFile);
				m_hVmLogFile = INVALID_HANDLE_VALUE;
			}
			m_nVmLogFileRecordCount = 0;
		}

		if (m_hVmLogFile == INVALID_HANDLE_VALUE)
		{
			if (m_nVmLogFileIndex > VM_FILE_MAX_INDEX)
				m_nVmLogFileIndex = 0;

			sprintf_s(m_szVmLogFileName,sizeof(m_szVmLogFileName),"VMLog_%d.txt",m_nVmLogFileIndex);
			
			if (strlen(m_szAppPath) > 0)
			{
				sprintf_s(szFileFullName,sizeof(szFileFullName)-1,"%s\\%s",m_szAppPath,m_szVmLogFileName);
			}
			else
			{
				sprintf_s(szFileFullName,sizeof(szFileFullName)-1,"%s",m_szVmLogFileName);
			}

			m_hVmLogFile = (HANDLE)CreateFile(szFileFullName, GENERIC_WRITE, FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
			if (m_hVmLogFile != INVALID_HANDLE_VALUE)
			{
				m_nVmLogFileIndex++;
				m_nVmLogFileRecordCount = 0;
			}
		}
	}
	catch(...)
	{

	}

	return false;
}

bool CVMLog::WriteVmLog(char *szFormat,...)
{
	va_list argList;
	char InputBuffer[4*1024] = {0};
	DWORD dwCount = 0;
	bool bReturn = false;

	if (!m_bVmLogInitFlag)
		return false;

	EnterCriticalSection(&m_csVmLogLock);

	try
	{
		SYSTEMTIME sysTm;
		::GetLocalTime(&sysTm);

		sprintf(InputBuffer,"%04d-%02d-%02d %02d:%02d:%02d  ",sysTm.wYear,sysTm.wMonth,sysTm.wDay,sysTm.wHour,sysTm.wMinute,sysTm.wSecond);

		va_start(argList,szFormat);
		_vstprintf_s(InputBuffer+strlen(InputBuffer),sizeof(InputBuffer)-strlen(InputBuffer)-3,szFormat,argList);
		va_end(argList);

		sprintf(InputBuffer+strlen(InputBuffer),"\r\n");

		g_FileWritEnNode.DataLen = strlen(InputBuffer);
		memcpy(g_FileWritEnNode.Buf,InputBuffer,g_FileWritEnNode.DataLen);

		RecvEnQueue(g_FileRecvBufQueue,g_FileWritEnNode);
	}
	catch(...)
	{

	}

	LeaveCriticalSection(&m_csVmLogLock);
	return bReturn;
}

bool CVMLog::UnInitVmlog()
{
	if (!m_bVmLogInitFlag)
		return true;

	EnterCriticalSection(&m_csVmLogLock);

	if (m_hVmLogFile != INVALID_HANDLE_VALUE)
	{
		FlushFileBuffers(m_hVmLogFile);
		CloseHandle(m_hVmLogFile);
		m_hVmLogFile = INVALID_HANDLE_VALUE;
	}

	ClearRecvQueue(g_FileRecvBufQueue);

	LeaveCriticalSection(&m_csVmLogLock);
	DeleteCriticalSection(&m_csVmLogLock);

	m_bVmLogInitFlag = false;

	return true;
}

bool CVMLog::GetVmAppPath()
{
	char Buffer[MAX_PATH] = {0};
	int nResult = 0;
	char *p = NULL;

	try
	{
		memset(Buffer,0,sizeof(Buffer));

		nResult = GetModuleFileName(NULL,Buffer,sizeof(Buffer)-1);
		if (nResult > 0)
		{
			p = strrchr(Buffer,'\\');
			if (p > Buffer)
			{
				memcpy(m_szAppPath,Buffer,p-Buffer);
				return true;
			}
		}
		return false;
	}
	catch(...)
	{

	}
	return false;
}

bool CVMLog::WriteVmLogFromInfoRecvQueue()
{
	bool bReturn = false;
	DWORD dwCount = 0;

	EnterCriticalSection(&m_csVmLogLock);

	do 
	{
		if (RecvQueueEmpty(g_FileRecvBufQueue))
		{
			break;
		}

		if(RecvDeQueue(g_FileRecvBufQueue,g_ThreadFileWriteDeNode) > 0)
		{
			if (m_hVmLogFile == INVALID_HANDLE_VALUE||m_nVmLogFileRecordCount > VM_FILE_RECORD_MAX_COUNT)
			{
				CreateVmLog();
			}

			if (m_hVmLogFile != INVALID_HANDLE_VALUE)
			{
				WriteFile(m_hVmLogFile,g_ThreadFileWriteDeNode.Buf,g_ThreadFileWriteDeNode.DataLen,&dwCount,NULL);
				m_nVmLogFileRecordCount++;
				bReturn = true;
			}
		}
	} while (TRUE);

	LeaveCriticalSection(&m_csVmLogLock);

	return bReturn;
}

unsigned __stdcall _VmLogFileWriteThread(void *arg)
{
	if (arg == NULL)
		return -1;

	CVMLog *pVmLog = (CVMLog *)arg;

	try
	{
		while (1)
		{
			if (g_bVmLogFileWriteThreadExitFlag == true)
			{
				return 0;
			}

			pVmLog->WriteVmLogFromInfoRecvQueue();

			Sleep(200);
		}
	}
	catch(...)
	{

	}

	return 0;
}
