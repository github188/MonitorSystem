#include "stdafx.h"
#include "WeatherDataDealWith.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"
#include "WeatherDataWinShowInfo.h"


//////////////////////////////////////////////////////////////////////////
int VemcuctlWeatherDataDealWith(int nType,void *pInfo,void *pUser)
{
	if (pInfo == NULL)
		return -1;

	int nResult = -1;

	try
	{
		switch (nType)
		{
		case GET_WEATHER_INFO_TYPE_CURRENT_STATION_WEATHER:
			{
				SetEvent(g_hWeatherCurrentInfoEvent);
				g_nWeatherCurrentInfoType = GET_WEATHER_INFO_TYPE_MIN;
				nResult = VemcuctlWeatherCurrentDataDealWith(pInfo,pUser);
			}
			break;

		case GET_WEATHER_INFO_TYPE_HISTORY_STATION_WEATHER:
			{
				SetEvent(g_hWeatherHistoryInfoEvent);
				g_nWeatherHistoryInfoType = GET_WEATHER_INFO_TYPE_MIN;
				nResult = VemcuctlWeatherHistoryDataDealWith(pInfo,pUser);
			}
			break;

		case GET_WEATHER_INFO_TYPE_HISTORY_WARNING_WEATHER:
			{
				SetEvent(g_hWeatherHistoryWarningInfoEvent);
				g_nWeatherHistoryWarningInfoType = GET_WEATHER_INFO_TYPE_MIN;
				nResult = VemcuctlWeatherHistoryWarningDealWith(pInfo,pUser);
			}
			break;

		case GET_WEATHER_INFO_TYPE_TYPHOON_WARNING_WEATHER:
			{
				SetEvent(g_hWeatherTyphoonWarningInfoEvent);
				g_nWeatherTyphoonWarningInfoType = GET_WEATHER_INFO_TYPE_MIN;
				nResult = VemcuctlWeatherTyphoonDealWith(pInfo,pUser);
			}
			break;

		case GET_WEATHER_INFO_TYPE_CURRENT_STATION_MICRO_WEATHER:
			{
				SetEvent(g_hMicroWeatherCurrentInfoEvent);
				g_nMicroWeatherCurrentInfoType = GET_WEATHER_INFO_TYPE_MIN;
				nResult = VemcuctlMicroWeatherCurrentDataDealWith(pInfo,pUser);
			}
			break;

		case GET_WEATHER_INFO_TYPE_HISTORY_STATION_MICRO_WEATHER:
			{
				SetEvent(g_hMicroWeatherHistoryInfoEvent);
				g_nMicroWeatherHistoryInfoType = GET_WEATHER_INFO_TYPE_MIN;
				nResult = VemcuctlMicroWeatherHistoryDataDealWith(pInfo,pUser);
			}
			break;

		default:
			break;
		}
	}
	catch(...)
	{

	}

	return nResult;
}

int VemcuctlWeatherCurrentDataDealWith(void *pInfo,void *pUser)
{
	if (pInfo == NULL)
		return -1;

	PT_GET_WEATHER_RESPONSE_HEAD head = NULL;
	PT_GET_WEATHER_STATTION_CUR_DATA_LIST pNode = NULL;

	head = (PT_GET_WEATHER_RESPONSE_HEAD)pInfo;
	pNode = (PT_GET_WEATHER_STATTION_CUR_DATA_LIST)head->list;

	if (pNode != NULL)
	{
		if (g_nWeatherCurrentInfoIndex < 0||g_nWeatherCurrentInfoIndex >= MAXVIEWCH)
			return 0;

		g_CurrentWeatherDataWinInfo[g_nWeatherCurrentInfoIndex].temperature = pNode->temperature;
		g_CurrentWeatherDataWinInfo[g_nWeatherCurrentInfoIndex].humidity = pNode->humidity;
		g_CurrentWeatherDataWinInfo[g_nWeatherCurrentInfoIndex].ave_wd_2min = pNode->ave_wd_2min;
		g_CurrentWeatherDataWinInfo[g_nWeatherCurrentInfoIndex].ave_ws_2min =  pNode->ave_ws_2min;
		g_CurrentWeatherDataWinInfo[g_nWeatherCurrentInfoIndex].rain_10min = pNode->rain_10min;
		g_CurrentWeatherDataWinInfo[g_nWeatherCurrentInfoIndex].air_pressure = pNode->air_pressure;
	}

	return 0;
}

int VemcuctlWeatherHistoryDataDealWith(void *pInfo,void *pUser)
{
	if (pInfo == NULL||pUser == NULL)
		return -1;

	PT_GET_WEATHER_RESPONSE_HEAD head = NULL;
	PT_GET_WEATHER_STATTION_HISTORY_DATA_LIST pNode = NULL;

	head = (PT_GET_WEATHER_RESPONSE_HEAD)pInfo;
	pNode = (PT_GET_WEATHER_STATTION_HISTORY_DATA_LIST)head->list;

	int nDataRecordCount = 0;

	if (pNode != NULL)
	{
		if (g_nWeatherHistoryInfoIndex < 0||g_nWeatherHistoryInfoIndex > 25)
			return 0;

		if (nDataRecordCount >= 20)
			return 0;

		g_HistoryWeatherDataWinInfo[g_nWeatherHistoryInfoIndex].DataRecordInfo[nDataRecordCount].temperature = pNode->temperature;
		g_HistoryWeatherDataWinInfo[g_nWeatherHistoryInfoIndex].DataRecordInfo[nDataRecordCount].humidity = pNode->humidity;
		g_HistoryWeatherDataWinInfo[g_nWeatherHistoryInfoIndex].DataRecordInfo[nDataRecordCount].ave_wd_2min = pNode->ave_wd_2min;
		g_HistoryWeatherDataWinInfo[g_nWeatherHistoryInfoIndex].DataRecordInfo[nDataRecordCount].ave_ws_2min =  pNode->ave_ws_2min;
		g_HistoryWeatherDataWinInfo[g_nWeatherHistoryInfoIndex].DataRecordInfo[nDataRecordCount].rain_10min = pNode->rain_10min;
		g_HistoryWeatherDataWinInfo[g_nWeatherHistoryInfoIndex].DataRecordInfo[nDataRecordCount].air_pressure = pNode->air_pressure;
		
		nDataRecordCount++;
		g_HistoryWeatherDataWinInfo[g_nWeatherHistoryInfoIndex].nRecordCount = nDataRecordCount;
		pNode = pNode->next;
	}

	return 0;
}

int VemcuctlWeatherHistoryWarningDealWith(void *pInfo,void *pUser)
{
	if (pInfo == NULL||pUser == NULL)
		return -1;

	PT_GET_WEATHER_RESPONSE_HEAD head = NULL;
	PT_GET_WEATHER_WARNING_HISTORY_DATA_LIST pNode = NULL;

	head = (PT_GET_WEATHER_RESPONSE_HEAD)pInfo;
	pNode = (PT_GET_WEATHER_WARNING_HISTORY_DATA_LIST)head->list;

	if (pNode != NULL)
	{


	}

	return 0;

}

int VemcuctlWeatherTyphoonDealWith(void *pInfo,void *pUser)
{
	if (pInfo == NULL||pUser == NULL)
		return -1;

	PT_GET_WEATHER_RESPONSE_HEAD head = NULL;
	PT_GET_WEATHER_TYPHOON_DATA_LIST pNode = NULL;

	head = (PT_GET_WEATHER_RESPONSE_HEAD)pInfo;
	pNode = (PT_GET_WEATHER_TYPHOON_DATA_LIST)head->list;

	if (pNode != NULL)
	{


	}

	return 0;
}

int VemcuctlMicroWeatherCurrentDataDealWith(void *pInfo,void *pUser)
{
	if (pInfo == NULL||pUser == NULL)
		return -1;

	PT_GET_WEATHER_RESPONSE_HEAD head = NULL;
	PT_GET_MICRO_WEATHER_STATTION_CUR_DATA_LIST pNode = NULL;

	head = (PT_GET_WEATHER_RESPONSE_HEAD)pInfo;
	pNode = (PT_GET_MICRO_WEATHER_STATTION_CUR_DATA_LIST)head->list;

	if (pNode != NULL)
	{
		if (g_nMicroWeatherCurrentInfoIndex < 0||g_nMicroWeatherCurrentInfoIndex > 25)
			return 0;

		g_CurrentMicroWeatherDataWinInfo[g_nMicroWeatherCurrentInfoIndex].wind_speed = pNode->wind_speed;
		g_CurrentMicroWeatherDataWinInfo[g_nMicroWeatherCurrentInfoIndex].temperature = pNode->temperature;
		g_CurrentMicroWeatherDataWinInfo[g_nMicroWeatherCurrentInfoIndex].humidity = pNode->humidity;
		g_CurrentMicroWeatherDataWinInfo[g_nMicroWeatherCurrentInfoIndex].wind_direction = pNode->wind_direction;
		g_CurrentMicroWeatherDataWinInfo[g_nMicroWeatherCurrentInfoIndex].air_pressure =  pNode->air_pressure;
		g_CurrentMicroWeatherDataWinInfo[g_nMicroWeatherCurrentInfoIndex].precipitation = pNode->precipitation;
		g_CurrentMicroWeatherDataWinInfo[g_nMicroWeatherCurrentInfoIndex].radiation = pNode->radiation;
		strcpy_s(g_CurrentMicroWeatherDataWinInfo[g_nMicroWeatherCurrentInfoIndex].precipitation_type,sizeof(g_CurrentMicroWeatherDataWinInfo[g_nMicroWeatherCurrentInfoIndex].precipitation_type),pNode->precipitation_type);
	}

	return 0;
}


int VemcuctlMicroWeatherHistoryDataDealWith(void *pInfo,void *pUser)
{
	if (pInfo == NULL||pUser == NULL)
		return -1;

	PT_GET_WEATHER_RESPONSE_HEAD head = NULL;
	PT_GET_MICRO_WEATHER_STATTION_HISTORY_DATA_LIST pNode = NULL;

	head = (PT_GET_WEATHER_RESPONSE_HEAD)pInfo;
	pNode = (PT_GET_MICRO_WEATHER_STATTION_HISTORY_DATA_LIST)head->list;

	if (pNode != NULL)
	{


	}

	return 0;
}