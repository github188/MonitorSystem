#include "stdafx.h"
#include "XmlInfoCallbackHandle.h"
#include "VMHistoryLog.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"


//////////////////////////////////////////////////////////////////////////
//Xml消息回调函数
int XmlInfoCallBack(char *strXml,int nLen,char *user)
{
	if (strlen(strXml) > nLen)
		return -1;

	if (g_pMainDlg != NULL)
	{
		g_pMainDlg->ThreadPoolDispatchTask(XmlInfoHandle,strXml,nLen);
	}

	return 0;
}

void XmlInfoHandle(void *strXmlInfo)
{

//省检版本
#if VM_SJ_CLIENT_VERSION
	{
		return;
	}
#endif

	int nType = VIDEO_LINKAGE_INFO_UNDEFINE_TYPE;
	int nResult = -1;

	char *strXml = (char *)strXmlInfo;

	if (strXml == NULL||strlen(strXml) == 0||strlen(strXml) > 64*1024)
		return;

	try
	{
		nType = GetXmlInfoType(strXml,strlen(strXml));

		switch(nType)
		{
		case VIDEO_LINKAGE_INFO_UNDEFINE_TYPE:
			break;

		case VIDEO_LINKAGE_INFO_TYPE:
			{
				VIDEO_LINKAGE_INFO  VideoManualLinkageInfo;
				memset(&VideoManualLinkageInfo,0,sizeof(VIDEO_LINKAGE_INFO));
				nResult = ParseLinkageInfo(&VideoManualLinkageInfo,strXml);

				if(nResult >= 0&&g_pMainDlg != NULL)
				{
					g_VMHistoryLog.WriteManualLinkageLog(&VideoManualLinkageInfo);
					g_pMainDlg->SendMessage(WM_VIDEO_LINKAGE_MANUAL_MESSAGE,(WPARAM)&VideoManualLinkageInfo,(LPARAM)WM_MAGIC_VEMCUCTL);
				}

				if (VideoManualLinkageInfo.pLinkageCameraInfo != NULL)
				{
					delete [] VideoManualLinkageInfo.pLinkageCameraInfo;
					VideoManualLinkageInfo.pLinkageCameraInfo = NULL;
				}
			}
			break;

		case VIDEO_ALARM_LINKAGE_INFO_TYPE:
			{
				VIDEO_ALARM_LINKAGE_INFO VideoAlarmLinkageInfo;
				memset(&VideoAlarmLinkageInfo,0,sizeof(VIDEO_ALARM_LINKAGE_INFO));
				nResult = ParseAlarmLinkageInfo(&VideoAlarmLinkageInfo,strXml);

				if(nResult >= 0&&g_pMainDlg != NULL)
				{
					g_VMHistoryLog.WriteAlarmLinkageLog(&VideoAlarmLinkageInfo);
					g_pMainDlg->SendMessage(WM_VIDEO_LINKAGE_ALARM_MESSAGE,(WPARAM)&VideoAlarmLinkageInfo,(LPARAM)WM_MAGIC_VEMCUCTL);
				}

				if (VideoAlarmLinkageInfo.pLinkageCameraInfo != NULL)
				{
					delete [] VideoAlarmLinkageInfo.pLinkageCameraInfo;
					VideoAlarmLinkageInfo.pLinkageCameraInfo = NULL;
				}
			}
			break;

		case VIDEO_STATE_LINKAGE_INFO_TYPE:
			{
				VIDEO_STATE_LINKAGE_INFO VideoStateLinkageInfo;
				memset(&VideoStateLinkageInfo,0,sizeof(VIDEO_STATE_LINKAGE_INFO));
				nResult = ParseStateLinkageInfo(&VideoStateLinkageInfo,strXml);

				if(nResult >= 0&&g_pMainDlg != NULL)
				{
					g_VMHistoryLog.WriteStateLinkageLog(&VideoStateLinkageInfo);
					g_pMainDlg->SendMessage(WM_VIDEO_LINKAGE_STATE_MESSAGE,(WPARAM)&VideoStateLinkageInfo,(LPARAM)WM_MAGIC_VEMCUCTL);
				}

				if (VideoStateLinkageInfo.pLinkageCameraInfo != NULL)
				{
					delete [] VideoStateLinkageInfo.pLinkageCameraInfo;
					VideoStateLinkageInfo.pLinkageCameraInfo = NULL;
				}
			}
			break;

		case VIDEO_CAMERA_CONTROL_NOTIFY_INFO_TYPE:
			{
				VIDEO_CAMERA_CONTROL_NOTIFY_INFO VideoCameraControlNotifyInfo;
				memset(&VideoCameraControlNotifyInfo,0,sizeof(VIDEO_CAMERA_CONTROL_NOTIFY_INFO));
				nResult = ParseCameraControlInfo(&VideoCameraControlNotifyInfo,strXml);

				if(nResult >= 0&&g_pMainDlg != NULL)
					g_pMainDlg->SendMessage(WM_VIDEO_CAMERA_CONTROL_NOTIFY_MESSAGE,(WPARAM)&VideoCameraControlNotifyInfo,(LPARAM)WM_MAGIC_VEMCUCTL);
			}
			break;

		case VIDEO_WEATHER_WARNING_INFO_TYPE:
			{
				VIDEO_WEATHER_WARNING_INFO VideoWeatherWarningInfo;
				memset(&VideoWeatherWarningInfo,0,sizeof(VIDEO_WEATHER_WARNING_INFO));
				nResult = ParseWeatherWarningLinkageInfo(&VideoWeatherWarningInfo,strXml);

				if(nResult >= 0&&g_pMainDlg != NULL)
				{
					g_VMHistoryLog.WriteWeatherWarningLinkageLog(&VideoWeatherWarningInfo);
					g_pMainDlg->SendMessage(WM_VIDEO_WEATHER_WARNING_LINKAGE_MESSAGE,(WPARAM)&VideoWeatherWarningInfo,(LPARAM)WM_MAGIC_VEMCUCTL);
				}

				if (VideoWeatherWarningInfo.pLinkageCameraInfo != NULL)
				{
					delete [] VideoWeatherWarningInfo.pLinkageCameraInfo;
					VideoWeatherWarningInfo.pLinkageCameraInfo = NULL;
				}
			}
			break;

		case VIDEO_WEATHER_FORECAST_INFO_TYPE:
			{
				VIDEO_WEATHER_FORECAST_INFO VideoWeatherForecastInfo;
				memset(&VideoWeatherForecastInfo,0,sizeof(VIDEO_WEATHER_FORECAST_INFO));
				nResult = ParseWeatherForecastLinkageInfo(&VideoWeatherForecastInfo,strXml);
			
				if(nResult >= 0&&g_pMainDlg != NULL)
				{
					g_VMHistoryLog.WriteWeatherForecastLinkageLog(&VideoWeatherForecastInfo);
					g_pMainDlg->SendMessage(WM_VIDEO_WEATHER_FORECAST_LINKAGE_MESSAGE,(WPARAM)&VideoWeatherForecastInfo,(LPARAM)WM_MAGIC_VEMCUCTL);
				}

				if (VideoWeatherForecastInfo.pLinkageCameraInfo != NULL)
				{
					delete [] VideoWeatherForecastInfo.pLinkageCameraInfo;
					VideoWeatherForecastInfo.pLinkageCameraInfo = NULL;
				}
			}
			break;

		default:
			break;
		}
	}
	catch(...)
	{

	}
}
