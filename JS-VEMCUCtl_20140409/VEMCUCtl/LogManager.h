#ifndef _LOG_MANAGER_H
#define _LOG_MANAGER_H
#include "Log.h"
#include "Singleton.h"
#include <map>
#include <time.h>
#include <Windows.h>


class LogManager: public Singleton<LogManager>
{
public:
	LogManager();
	~LogManager();

	Log* createLog(const std::string& name, bool defaultLog = false, bool debuggerOutput = true, 
		bool suppressFileOutput = false);

	Log* getLog(const std::string& name);
	Log* getDefaultLog();

	void destroyLog(const std::string& name);
	void destroyLog(Log* log);

	Log* setDefaultLog(Log* newLog);

	void logMessage(const std::string& message, LogMessageLevel lml = LML_NORMAL, 
		bool maskDebug = false);

	void logMessage(LogMessageLevel lml, const std::string& message,  
		bool maskDebug = false) 
	{ 
		logMessage(message, lml, maskDebug); 
	}

	void setLogDetail(LoggingLevel ll);  

	static LogManager& getSingleton();      
	static LogManager* getSingletonPtr();
	unsigned int getLogNum() const;
	void SetLogFileSize(int iLogFileSize)
	{
		m_iLogFileSize = iLogFileSize;
	}
	void SetLogFileSaveTime(int iLogFileSaveTime)
	{
		m_iLogFileSaveTime = iLogFileSaveTime;
	}
	int GetLogFileSize()
	{
		return m_iLogFileSize;
	}
	void CreateNewLog();
protected:	
	typedef std::map<std::string, Log*>	LogList;
	LogList mLogs;
	Log* mDefaultLog;
	int m_iLogFileSize;//日志保存大小
	int m_iLogFileSaveTime;//日志保存天数
	int m_iLogFileSeq;
	std::string m_strFileName;//日志文件
private:
	CRITICAL_SECTION mMutex;

};

//产生新日志文件，删除旧日志文件


#define WARNING_LOG(format,...)	\
{ \
	char sLog[10000] = {0}; \
	sprintf(sLog,"[WARNING][File:%s Func:%2s, Line:%d] \n\t"format,\
	__FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
	LogManager::getSingleton().logMessage(sLog,LML_NORMAL,true); \
}


#define ERROR_LOG(format,...)	\
{ \
	char sLog[10000] = {0}; \
	sprintf(sLog,"[ERR][File:%s Func:%2s, Line:%d] \n\t"format,\
	__FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
	LogManager::getSingleton().logMessage(sLog,LML_NORMAL,true); \
}

#define DEBUG_LOG(format,...)	\
{ \
	char sLog[10000] = {0}; \
	sprintf(sLog,"[DEBUG][File:%s Func:%s, Line:%d] \n\t"format,\
	__FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
	LogManager::getSingleton().logMessage(sLog,LML_NORMAL,true); \
}

#define ENTER_FUN_LOG()	\
{\
	char sLog[500] = {0}; \
	sprintf(sLog,"[DEBUG][Enter File:%s Func:%s,Line:%d]",	\
	__FILE__, __FUNCTION__, __LINE__);	\
	LogManager::getSingleton().logMessage(sLog,LML_NORMAL,true); \
}

#define EXIT_FUN_LOG()	\
{\
	char sLog[500] = {0}; \
	sprintf(sLog,"[DEBUG][Exit File:%s Func:%s,Line:%d]",	\
	__FILE__, __FUNCTION__, __LINE__);	\
	LogManager::getSingleton().logMessage(sLog,LML_NORMAL,true); \
}

extern void Convert_Data_To_Hex(char* out,int iOutSize, char* in,int len);
#define DEBUG_MESSAGE_LOG(msgid,in,len)	\
{\
	char sLog[10000] = {0};\
	sprintf(sLog,"Process Requst message msgid = %d msglen = %d msgbody = \r\n",msgid,len);\
	CONVERT_DATA_TO_BINRARY(sLog+strlen(sLog),in,len);	\
	LogManager::getSingleton().logMessage(sLog,LML_NORMAL,true); \
}
#endif
