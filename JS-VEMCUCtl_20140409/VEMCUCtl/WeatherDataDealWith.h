#ifndef  _WEATHER_DATA_DEAL_WITH_H_
#define _WEATHER_DATA_DEAL_WITH_H_

//////////////////////////////////////////////////////////////////////////
#define  GET_WEATHER_INFO_TYPE_MIN     0
#define  GET_WEATHER_INFO_TYPE_CURRENT_STATION_WEATHER   1 
#define  GET_WEATHER_INFO_TYPE_HISTORY_STATION_WEATHER   2
#define  GET_WEATHER_INFO_TYPE_CURRENT_WARNING_WEATHER   3
#define  GET_WEATHER_INFO_TYPE_HISTORY_WARNING_WEATHER   4
#define  GET_WEATHER_INFO_TYPE_FORECAST_WARNING_WEATHER   5
#define  GET_WEATHER_INFO_TYPE_TYPHOON_WARNING_WEATHER   6
#define  GET_WEATHER_INFO_TYPE_CURRENT_STATION_MICRO_WEATHER   7 
#define  GET_WEATHER_INFO_TYPE_HISTORY_STATION_MICRO_WEATHER   8
#define  GET_WEATHER_INFO_TYPE_MAX   9

int VemcuctlWeatherDataDealWith(int nType,void *pInfo,void *pUser);
int VemcuctlWeatherCurrentDataDealWith(void *pInfo,void *pUser);
int VemcuctlWeatherHistoryDataDealWith(void *pInfo,void *pUser);
int VemcuctlWeatherHistoryWarningDealWith(void *pInfo,void *pUser);
int VemcuctlWeatherTyphoonDealWith(void *pInfo,void *pUser);
int VemcuctlMicroWeatherCurrentDataDealWith(void *pInfo,void *pUser);
int VemcuctlMicroWeatherHistoryDataDealWith(void *pInfo,void *pUser);


#endif