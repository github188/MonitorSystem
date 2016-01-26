#include "stdafx.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"
#include "RequestWeatherInfo.h"

//////////////////////////////////////////////////////////////////////////
void RequestCurrentWeatherInfo(void *arg)
{
	if (g_pMainDlg == NULL)
		return;

	try
	{
		int nIndex = *(int *)arg;

		if (nIndex < 0||nIndex >= MAXVIEWCH)
			return;

		g_pMainDlg->VemSendCurrentWeatherInfoRequest(nIndex,g_CurrentWeatherDataWinInfo[nIndex].szUserKey,
			g_CurrentWeatherDataWinInfo[nIndex].nStationID,g_CurrentWeatherDataWinInfo[nIndex].nTimeRange);
	}
	catch(...)
	{

	}
}

void RequestHistoryWeatherInfo(void *arg)
{
	if (g_pMainDlg == NULL)
		return;

	try
	{
		int nIndex = *(int *)arg;

		if (nIndex < 0||nIndex > 25)
			return;

		g_pMainDlg->VemSendHistoryWeatherInfoRequest(nIndex,g_HistoryWeatherDataWinInfo[nIndex].szUserKey,
			g_HistoryWeatherDataWinInfo[nIndex].nStationID,g_HistoryWeatherDataWinInfo[nIndex].szStartTime,
			g_HistoryWeatherDataWinInfo[nIndex].szStopTime,g_HistoryWeatherDataWinInfo[nIndex].szSortColumn,
			g_HistoryWeatherDataWinInfo[nIndex].nPageRecords,g_HistoryWeatherDataWinInfo[nIndex].nPageIndex);
	}
	catch(...)
	{

	}
}

void RequestHistoryWarningInfo(void *arg)
{
	if (g_pMainDlg == NULL)
		return;

}

void RequestTyphoonInfo(void *arg)
{
	if (g_pMainDlg == NULL)
		return;

}

void RequestCurentMicroWeatherInfo(void *arg)
{
	if (g_pMainDlg == NULL)
		return;

	try
	{
		int nIndex = *(int *)arg;

		if (nIndex < 0||nIndex > 25)
			return;

		g_pMainDlg->VemSendCurrentMicroWeatherInfoRequest(nIndex,g_CurrentMicroWeatherDataWinInfo[nIndex].szUserKey,
			g_CurrentMicroWeatherDataWinInfo[nIndex].nComCode,g_CurrentMicroWeatherDataWinInfo[nIndex].szComType,g_CurrentMicroWeatherDataWinInfo[nIndex].nTimeRange);
	}
	catch(...)
	{

	}
}

void RequestHistoryMicroWeatherInfo(void *arg)
{
	if (g_pMainDlg == NULL)
		return;
}