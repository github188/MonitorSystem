#include "StdAfx.h"
#include "Log.h"
#include "LogManager.h"
#include <iostream>
#include <time.h>
#include <iomanip>

//-----------------------------------------------------------------------
Log::Log(const std::string& name, bool debuggerOuput, bool suppressFile) : 
mLogLevel(LL_NORMAL), mDebugOut(debuggerOuput),
	mSuppressFile(suppressFile), mTimeStamp(true), mLogName(name)
{
	if (!mSuppressFile)
	{
		mfpLog.open(name.c_str());
	}
}
//-----------------------------------------------------------------------
Log::~Log()
{
	if (!mSuppressFile)
	{
		mfpLog.flush();
		mfpLog.close();
	}
}
//-----------------------------------------------------------------------
void Log::logMessage(const std::string& message, LogMessageLevel lml, bool maskDebug)
{
	if ((mLogLevel + lml) >= LOG_THRESHOLD)
	{
		if (mDebugOut && !maskDebug)
		{
			if (mTimeStamp)
			{
				struct tm *pTime;
				time_t ctTime; 
				time(&ctTime);
				pTime = localtime(&ctTime);

				std::cerr << std::setw(4) << pTime->tm_year+1900 << "-" << std::setw(2)
					<< std::setfill('0') << pTime->tm_mon+1 << "-" << std::setw(2)
					<<std::setfill('0') <<pTime->tm_mday<< "  "
					<< std::setw(2) << std::setfill('0') << pTime->tm_hour
					<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_min
					<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_sec
					<< "    ";
			}

			std::cerr << message << std::endl;
		}

		// Write time into log
		if (!mSuppressFile)
		{
			if (mTimeStamp)
			{
				struct tm *pTime;
				time_t ctTime; 
				time(&ctTime);
				pTime = localtime(&ctTime);

				mfpLog << std::setw(4) << pTime->tm_year+1900 << "-" << std::setw(2)
					<< std::setfill('0') << pTime->tm_mon+1 << "-" << std::setw(2)
					<<std::setfill('0') <<pTime->tm_mday<< "  "
					<< std::setw(2) << std::setfill('0') << pTime->tm_hour
					<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_min
					<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_sec
					<< "    ";
			}
			mfpLog << message << std::endl;
			mfpLog.flush();
		}
	}
}

//-----------------------------------------------------------------------
void Log::setTimeStampEnabled(bool timeStamp)
{
	mTimeStamp = timeStamp;
}

//-----------------------------------------------------------------------
void Log::setDebugOutputEnabled(bool debugOutput)
{
	mDebugOut = debugOutput;
}

//-----------------------------------------------------------------------
void Log::setLogDetail(LoggingLevel ll)
{
	mLogLevel = ll;
}
