#ifndef  _SEARCH_WEATHER_INFO_API_H_
#define _SEARCH_WEATHER_INFO_API_H_

#include "WeatherDefine.h"


#ifdef _x_search_weather_info_dll_
#define _x_searchweatherdll_  __declspec(dllexport)
#else
#define _x_searchweatherdll_  __declspec(dllimport)
#endif

//---------------------------------------------------------------------------
// CÓïÑÔ±ê×¼
#ifdef __cplusplus
extern "C" {
#endif
	_x_searchweatherdll_ BOOL SetConfigInfo(int nIndex,char * szLocalIP,int nLocalPort,char * szRemoteIP,int nRemotePort);
	_x_searchweatherdll_ BOOL SetWeatherInfoCallback(int nIndex,void *infoCB,void *user);
	_x_searchweatherdll_ BOOL InitWeatherInfo(int nIndex);
	_x_searchweatherdll_ BOOL StartWeatherInfo(int nIndex);
	_x_searchweatherdll_ BOOL StopWeatherInfo(int nIndex);
	_x_searchweatherdll_ BOOL UnInitWeatherInfo(int nIndex);
	_x_searchweatherdll_ int      GetWeatherInfoState(int nIndex);
	_x_searchweatherdll_ BOOL SendCurrentWeatherInfoRequest(int nIndex,char *szUserKey,int nStationID,int nTimeRange);
	_x_searchweatherdll_ BOOL SendHistoryWeatherInfoRequest(int nIndex,char *szUserKey,int nStationID,char *szStartTime,char *szStopTime,char *szSortColumn,int nPageRecords,int nPageIndex);
	_x_searchweatherdll_ BOOL SendHistoryWarningInfoRequest(int nIndex,char *szUserKey,int nStationID,char *szStartTime,char *szStopTime,char *szSortColumn,int nPageRecords,int nPageIndex);
	_x_searchweatherdll_ BOOL SendTyphoonInfoRequest(int nIndex,char *szUserKey,int nTFID,char *szStartTime,char *szStopTime,char *szSortColumn,int nPageRecords,int nPageIndex);
	_x_searchweatherdll_ BOOL SendCurrentMicroWeatherInfoRequest(int nIndex,char *szUserKey,int nComCode,char *szComType,int nTimeRange);
	_x_searchweatherdll_ BOOL SendHistoryMicroWeatherInfoRequest(int nIndex,char *szUserKey,int nComCode,char *szComType,char *szStartTime,char *szStopTime,char *szSortColumn,int nPageRecords,int nPageIndex);

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
#undef _x_searchweatherdll_







#endif