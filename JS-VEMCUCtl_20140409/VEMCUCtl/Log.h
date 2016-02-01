#ifndef _LOG_H
#define _LOG_H

#include <string>
#include <fstream>
#define LOG_THRESHOLD 4


enum LoggingLevel
{
	LL_LOW = 1,
	LL_NORMAL = 2,
	LL_BOREME = 3
};

enum LogMessageLevel
{
	LML_TRIVIAL = 1,
	LML_NORMAL = 2,
	LML_CRITICAL = 3
};


class Log
{
public:
	Log(const std::string& name, bool debugOutput = true, bool suppressFileOutput = false);
	~Log();

	const std::string& getName() const { return mLogName; }
	bool isDebugOutputEnabled() const { return mDebugOut; }
	bool isFileOutputSuppressed() const { return mSuppressFile; }
	bool isTimeStampEnabled() const { return mTimeStamp; }


	void logMessage(const std::string& message, LogMessageLevel lml = LML_NORMAL, bool maskDebug = false);

	void setDebugOutputEnabled(bool debugOutput);
	void setLogDetail(LoggingLevel ll);
	void setTimeStampEnabled(bool timeStamp);

	LoggingLevel getLogDetail() const { return mLogLevel; }

protected:
	std::ofstream	mfpLog;
	LoggingLevel	mLogLevel;
	bool			mDebugOut;
	bool			mSuppressFile;
	bool			mTimeStamp;
	std::string		mLogName;
};

#endif
