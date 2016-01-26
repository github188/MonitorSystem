#include "stdafx.h"
#include "tinyxml.h"
#include "xmlApi.h"
#include "..\GBKToUTF8.h"

// ==================================发送请求===========================
// 告警通知
const char *pszNotifyAlarm=
"<?xml version=\"1.0\" encoding=\"GB2312\"?>"
"<SIP_XML EventType=\"Notify_Alarm\">"
	"<Item Code=\"100100010200000000\" Name=\"摄像头名称\" Type=\"1\" StartTime=\"2010-08-30 12:21:08\"/>"
	"<Item Code=\"100100010200000001\" Name=\"摄像头名称\" Type=\"2\" StartTime=\"2010-08-31 09:33:50\"/>"
"</SIP_XML>";

// 解析告警通知
int ParseNotifyAlarm(NOTIFY_ALARM notify_alarm[], int num, const char *strXML)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL, *pItem = NULL;
	const char		*pResult = NULL;
	int i = 0;
	
	try
	{
		resXML.Parse(strXML);

		pElement = resXML.FirstChildElement("SIP_XML");
		pItem = pElement->FirstChildElement("Item");
		while (pItem) 
		{
			if (i >= num)
				break;
			
			pResult = pItem->Attribute("Code");
			strcpy(notify_alarm[i].code, pResult);

			pResult = pItem->Attribute("Name");
			strcpy(notify_alarm[i].name, pResult);
			
			pResult	= pItem->Attribute("Type");
			notify_alarm[i].type = atoi(pResult);
			
			pResult = pItem->Attribute("StartTime");
			strcpy(notify_alarm[i].StartTime, pResult);

			pResult = pItem->Attribute("Flag");
			notify_alarm[i].flag = atoi(pResult);

			i ++;

			pItem = pItem->NextSiblingElement("Item");
		}		
	}
	catch (...)
	{
		return -1;
	}
	
	return 0;	
}


// 状态通知
const char *pszNotifyStatus=
"<?xml version=\"1.0\" encoding=\"GB2312\"?>"
"<SIP_XML EventType=\"Notify_Status\">"
	"<Item Code=\"状态源的地址编码\" Name=\"摄像头名称\" Status=\"活动状态\">"
	"<Item Code=\"状态源的地址编码\" Name=\"摄像头名称\" Status=\"活动状态\">"
"</SIP_XML>";

// 解析状态通知
int ParseNotifyStatus(NOTIFY_STATUS notify_status[], int num, const char *strXML)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL, *pItem = NULL;
	const char		*pResult = NULL;
	int i = 0;
	
	try
	{
		resXML.Parse(strXML);

		pElement = resXML.FirstChildElement("SIP_XML");
		pItem = pElement->FirstChildElement("Item");
		while (pItem)
		{
			if (i >= num)
				break;
			
			pResult = pItem->Attribute("Code");
			strcpy(notify_status[i].code, pResult);

			pResult = pItem->Attribute("Name");
			strcpy(notify_status[i].name, pResult);
			
			pResult = pItem->Attribute("Type");
			notify_status[i].status = atoi(pResult);
			
			i ++;
			
			pItem = pItem->NextSiblingElement("Item");
		}		
	}
	catch (...)
	{
		return -1;
	}
	
	return 0;	
}


// 获取请求XML类型和子项数
int GetXmlTypeNum(const char *strXML, int *type, int *num)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL, *pItem = NULL;
	const char		*pResult = NULL;

	resXML.Parse(strXML);

	*num = 0;
	*type = SIP_DEFAULT;

	pElement = resXML.FirstChildElement("SIP_XML");
	if (pElement != NULL)
	{
		pResult = pElement->Attribute("EventType");
		if (pResult != NULL) 
		{
			if (strcmp(pResult, "Notify_Alarm") == 0) 
			{
				*type = SIP_NOTIFY_ALARM;

				pItem = pElement->FirstChildElement("Item");
				while (pItem)
				{
					(*num) ++;
					pItem = pItem->NextSiblingElement("Item");					
				}
			}
			else if (strcmp(pResult, "Notify_Status") == 0)
			{
				*type = SIP_NOTIFY_STATUS;

				pItem = pElement->FirstChildElement("Item");
				while (pItem)
				{
					(*num) ++;
					pItem = pItem->NextSiblingElement("Item");
				}
			}
		}
	}

	return 0;
}


//得到Xml信息类型
int GetXmlInfoType(char *strXml,int nLen)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	const char		*pResult = NULL;
	int nResult = VIDEO_LINKAGE_INFO_UNDEFINE_TYPE;

	try
	{
		resXML.Parse(strXml);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return nResult;

		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return nResult;

		if (strcmp(pResult, "Video_Link") == 0) 
			nResult = VIDEO_LINKAGE_INFO_TYPE;
		else if (strcmp(pResult, "Alarm_Info") == 0)
			nResult = VIDEO_ALARM_LINKAGE_INFO_TYPE;
		else if (strcmp(pResult, "Device_State") == 0)
			nResult = VIDEO_STATE_LINKAGE_INFO_TYPE;
		else if (strcmp(pResult, "Control_Notify") == 0)
			nResult = VIDEO_CAMERA_CONTROL_NOTIFY_INFO_TYPE;
		else if (strcmp(pResult, "Weather_Warn") == 0)
			nResult = VIDEO_WEATHER_WARNING_INFO_TYPE;
		else if (strcmp(pResult, "Weather_Forecast") == 0)
			nResult = VIDEO_WEATHER_FORECAST_INFO_TYPE;

		return nResult;
	}
	catch(...)
	{

	}

	return nResult;
}

// 解析手动和巡检联动
int ParseLinkageInfo(VIDEO_LINKAGE_INFO *pVideoLinkageInfo,const char *strXML)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	TiXmlElement *pSubList = NULL;
	TiXmlElement *pItem = NULL;
	const char		*pResult = NULL;
	int nNum = 0;
	int i = 0;

	char szGB2312Info[1024] = {0};
	int    nGB2312InfoLen = 0;

	if (pVideoLinkageInfo == NULL||strXML == NULL)
		return -1;

	try
	{
		memset(pVideoLinkageInfo,0,sizeof(VIDEO_LINKAGE_INFO));

		resXML.Parse(strXML);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return -1;

		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return -1;

		if (strcmp(pResult, "Video_Link") != 0) 
			return -1;

		pSubList = pElement->FirstChildElement("SubList");
		if (pSubList == NULL)
			return -1;

		pResult = pSubList->Attribute("Station");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szStationName,sizeof(pVideoLinkageInfo->szStationName),szGB2312Info);
		}
		
		pResult = pSubList->Attribute("Device");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szDeviceName,sizeof(pVideoLinkageInfo->szDeviceName),szGB2312Info);
		}

		pResult = pSubList->Attribute("Type");
		if (pResult != NULL)
			pVideoLinkageInfo->nType = atoi(pResult);

		pResult = pSubList->Attribute("ScreenId");
		if (pResult != NULL)
			pVideoLinkageInfo->nScreenId = atoi(pResult);

		pResult = pSubList->Attribute("Time");
		if (pResult != NULL)
			strcpy_s(pVideoLinkageInfo->szTime,sizeof(pVideoLinkageInfo->szTime),pResult);

		pResult = pSubList->Attribute("LinkStation");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szLinkageStationName,sizeof(pVideoLinkageInfo->szLinkageStationName),szGB2312Info);
		}

		pResult = pSubList->Attribute("LinkNum");
		if (pResult != NULL)
			pVideoLinkageInfo->nLinkNum = nNum = atoi(pResult);

		if (nNum <= 0)
			return -1;

		if (nNum > 200)//最多解析200个
			pVideoLinkageInfo->nLinkNum = nNum = 200;

		pVideoLinkageInfo->pLinkageCameraInfo = new VIDEO_LINKAGE_CAMERA_INFO[nNum];

		if (pVideoLinkageInfo->pLinkageCameraInfo == NULL)
			return -1;

		memset(pVideoLinkageInfo->pLinkageCameraInfo,0,sizeof(VIDEO_LINKAGE_CAMERA_INFO)*nNum);

		pItem = pSubList->FirstChildElement("Item");
		while(pItem != NULL&&i < nNum)
		{
			pResult = pItem->Attribute("Code");
			if (pResult != NULL)
				strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szCode,LINKAGE_DEVICE_CODE_LEN,pResult);

			pResult = pItem->Attribute("Name");
			if (pResult != NULL)
			{
				if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
					strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szName,LINKAGE_DEVICE_NAME_LEN,szGB2312Info);
			}

			pResult = pItem->Attribute("DecoderTag");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nDecodeTag = atoi(pResult);

			pResult = pItem->Attribute("PresetName");
			if (pResult != NULL)
			{
				if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
					strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szPresetName,VIDEO_PRESET_NAME_LEN,szGB2312Info);
			}

			pResult = pItem->Attribute("PresetId");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nPresetId = atoi(pResult);

			pResult = pItem->Attribute("FromX1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromX1 = atoi(pResult);

			pResult = pItem->Attribute("FromY1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromY1 = atoi(pResult);

			pResult = pItem->Attribute("ToX1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToX1 = atoi(pResult);

			pResult = pItem->Attribute("ToY1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToY1 = atoi(pResult);

			pResult = pItem->Attribute("FromX2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromX2 = atoi(pResult);

			pResult = pItem->Attribute("FromY2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromY2 = atoi(pResult);

			pResult = pItem->Attribute("ToX2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToX2 = atoi(pResult);

			pResult = pItem->Attribute("ToY2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToY2 = atoi(pResult);

			i ++;
			pItem = pItem->NextSiblingElement("Item");
		}

		return 0;
	}
	catch (...)
	{

	}

	try
	{
		if (pVideoLinkageInfo->pLinkageCameraInfo != NULL)
		{
			delete [] pVideoLinkageInfo->pLinkageCameraInfo;
			pVideoLinkageInfo->pLinkageCameraInfo = NULL;
		}
	}
	catch(...)
	{

	}

	return -1;	
}

// 解析告警联动
int ParseAlarmLinkageInfo(VIDEO_ALARM_LINKAGE_INFO *pVideoLinkageInfo,const char *strXML)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	TiXmlElement *pSubList = NULL;
	TiXmlElement *pItem = NULL;
	const char		*pResult = NULL;
	int nNum = 0;
	int i = 0;

	char szGB2312Info[1024] = {0};
	int nGB2312InfoLen = 0;

	if (pVideoLinkageInfo == NULL||strXML == NULL)
		return -1;

	try
	{
		memset(pVideoLinkageInfo,0,sizeof(VIDEO_ALARM_LINKAGE_INFO));

		resXML.Parse(strXML);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return -1;

		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return -1;

		if (strcmp(pResult, "Alarm_Info") != 0) 
			return -1;

		pSubList = pElement->FirstChildElement("SubList");
		if (pSubList == NULL)
			return -1;

		pResult = pSubList->Attribute("Station");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szStationName,sizeof(pVideoLinkageInfo->szStationName),szGB2312Info);
		}

		pResult = pSubList->Attribute("Device");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szDeviceName,sizeof(pVideoLinkageInfo->szDeviceName),szGB2312Info);
		}

		pResult = pSubList->Attribute("AlmType");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szAlarmType,sizeof(pVideoLinkageInfo->szAlarmType),szGB2312Info);
		}

		pResult = pSubList->Attribute("Time");
		if (pResult != NULL)
			strcpy_s(pVideoLinkageInfo->szTime,sizeof(pVideoLinkageInfo->szTime),pResult);

		pResult = pSubList->Attribute("Content");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szContent,sizeof(pVideoLinkageInfo->szContent),szGB2312Info);
		}

		pResult = pSubList->Attribute("LinkStation");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szLinkageStationName,sizeof(pVideoLinkageInfo->szLinkageStationName),szGB2312Info);
		}

		pResult = pSubList->Attribute("LinkNum");
		if (pResult != NULL)
			pVideoLinkageInfo->nLinkNum = nNum = atoi(pResult);

		if (nNum <= 0)
			return -1;

		if (nNum > 200)//最多解析200个
			pVideoLinkageInfo->nLinkNum = nNum = 200;

		pVideoLinkageInfo->pLinkageCameraInfo = new VIDEO_LINKAGE_CAMERA_INFO[nNum];

		if (pVideoLinkageInfo->pLinkageCameraInfo == NULL)
			return -1;

		memset(pVideoLinkageInfo->pLinkageCameraInfo,0,sizeof(VIDEO_LINKAGE_CAMERA_INFO)*nNum);

		pItem = pSubList->FirstChildElement("Item");
		while(pItem != NULL && i < nNum)
		{
			pResult = pItem->Attribute("Code");
			if (pResult != NULL)
				strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szCode,LINKAGE_DEVICE_CODE_LEN,pResult);

			pResult = pItem->Attribute("Name");
			if (pResult != NULL)
			{
				if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
					strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szName,LINKAGE_DEVICE_NAME_LEN,szGB2312Info);
			}

			pResult = pItem->Attribute("DecoderTag");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nDecodeTag = atoi(pResult);

			pResult = pItem->Attribute("PresetName");
			if (pResult != NULL)
			{
				if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
					strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szPresetName,VIDEO_PRESET_NAME_LEN,szGB2312Info);
			}

			pResult = pItem->Attribute("PresetId");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nPresetId = atoi(pResult);

			pResult = pItem->Attribute("FromX1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromX1 = atoi(pResult);

			pResult = pItem->Attribute("FromY1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromY1 = atoi(pResult);

			pResult = pItem->Attribute("ToX1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToX1 = atoi(pResult);

			pResult = pItem->Attribute("ToY1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToY1 = atoi(pResult);

			pResult = pItem->Attribute("FromX2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromX2 = atoi(pResult);

			pResult = pItem->Attribute("FromY2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromY2 = atoi(pResult);

			pResult = pItem->Attribute("ToX2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToX2 = atoi(pResult);

			pResult = pItem->Attribute("ToY2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToY2 = atoi(pResult);


			i ++;
			pItem = pItem->NextSiblingElement("Item");
		}

		return 0;
	}
	catch (...)
	{

	}

	try
	{
		if (pVideoLinkageInfo->pLinkageCameraInfo != NULL)
		{
			delete [] pVideoLinkageInfo->pLinkageCameraInfo;
			pVideoLinkageInfo->pLinkageCameraInfo = NULL;
		}
	}
	catch(...)
	{

	}

	return -1;	
}

// 解析状态联动
int ParseStateLinkageInfo(VIDEO_STATE_LINKAGE_INFO *pVideoLinkageInfo,const char *strXML)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	TiXmlElement *pSubList = NULL;
	TiXmlElement *pItem = NULL;
	const char		*pResult = NULL;
	int nNum = 0;
	int i = 0;

	char szGB2312Info[1024] = {0};
	int    nGB2312InfoLen = 0;

	if (pVideoLinkageInfo == NULL||strXML == NULL)
		return -1;

	try
	{
		memset(pVideoLinkageInfo,0,sizeof(VIDEO_STATE_LINKAGE_INFO));

		resXML.Parse(strXML);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return -1;

		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return -1;

		if (strcmp(pResult, "Device_State") != 0) 
			return -1;

		pSubList = pElement->FirstChildElement("SubList");
		if (pSubList == NULL)
			return -1;

		pResult = pSubList->Attribute("Station");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szStationName,sizeof(pVideoLinkageInfo->szStationName),szGB2312Info);
		}

		pResult = pSubList->Attribute("Device");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szDeviceName,sizeof(pVideoLinkageInfo->szDeviceName),szGB2312Info);
		}

		pResult = pSubList->Attribute("DevType");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szDeviceType,sizeof(pVideoLinkageInfo->szDeviceType),szGB2312Info);
		}

		pResult = pSubList->Attribute("Time");
		if (pResult != NULL)
			strcpy_s(pVideoLinkageInfo->szTime,sizeof(pVideoLinkageInfo->szTime),pResult);

		pResult = pSubList->Attribute("Content");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szContent,sizeof(pVideoLinkageInfo->szContent),szGB2312Info);
		}

		pResult = pSubList->Attribute("Value1");
		if (pResult != NULL)
			strcpy_s(pVideoLinkageInfo->szValue1,sizeof(pVideoLinkageInfo->szValue1),pResult);

		pResult = pSubList->Attribute("Value2");
		if (pResult != NULL)
			strcpy_s(pVideoLinkageInfo->szValue2,sizeof(pVideoLinkageInfo->szValue2),pResult);

		pResult = pSubList->Attribute("LinkStation");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szLinkageStationName,sizeof(pVideoLinkageInfo->szLinkageStationName),szGB2312Info);
		}

		pResult = pSubList->Attribute("LinkNum");
		if (pResult != NULL)
			pVideoLinkageInfo->nLinkNum = nNum = atoi(pResult);

		if (nNum <= 0)
			return -1;

		if (nNum > 200)//最多解析200个
			pVideoLinkageInfo->nLinkNum = nNum = 200;

		pVideoLinkageInfo->pLinkageCameraInfo = new VIDEO_LINKAGE_CAMERA_INFO[nNum];

		if (pVideoLinkageInfo->pLinkageCameraInfo == NULL)
			return -1;

		memset(pVideoLinkageInfo->pLinkageCameraInfo,0,sizeof(VIDEO_LINKAGE_CAMERA_INFO)*nNum);

		pItem = pSubList->FirstChildElement("Item");
		while(pItem != NULL&&i < nNum)
		{
			pResult = pItem->Attribute("Code");
			if (pResult != NULL)
				strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szCode,LINKAGE_DEVICE_CODE_LEN,pResult);

			pResult = pItem->Attribute("Name");
			if (pResult != NULL)
			{
				if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
					strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szName,LINKAGE_DEVICE_NAME_LEN,szGB2312Info);
			}

			pResult = pItem->Attribute("DecoderTag");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nDecodeTag = atoi(pResult);

			pResult = pItem->Attribute("PresetName");
			if (pResult != NULL)
			{
				if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
					strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szPresetName,VIDEO_PRESET_NAME_LEN,szGB2312Info);
			}

			pResult = pItem->Attribute("PresetId");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nPresetId = atoi(pResult);

			pResult = pItem->Attribute("FromX1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromX1 = atoi(pResult);

			pResult = pItem->Attribute("FromY1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromY1 = atoi(pResult);

			pResult = pItem->Attribute("ToX1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToX1 = atoi(pResult);

			pResult = pItem->Attribute("ToY1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToY1 = atoi(pResult);

			pResult = pItem->Attribute("FromX2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromX2 = atoi(pResult);

			pResult = pItem->Attribute("FromY2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromY2 = atoi(pResult);

			pResult = pItem->Attribute("ToX2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToX2 = atoi(pResult);

			pResult = pItem->Attribute("ToY2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToY2 = atoi(pResult);

			i ++;
			pItem = pItem->NextSiblingElement("Item");
		}

		return 0;
	}
	catch (...)
	{

	}

	try
	{
		if (pVideoLinkageInfo->pLinkageCameraInfo != NULL)
		{
			delete [] pVideoLinkageInfo->pLinkageCameraInfo;
			pVideoLinkageInfo->pLinkageCameraInfo = NULL;
		}
	}
	catch(...)
	{

	}

	return -1;	
}

// 解析云镜控制通知信息
int ParseCameraControlInfo(VIDEO_CAMERA_CONTROL_NOTIFY_INFO *pCameraControlNotifyInfo,const char *strXML)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	TiXmlElement *pItem = NULL;
	const char		*pResult = NULL;

	char szGB2312Info[256] = {0};
	int    nGB2312InfoLen = 0;

	if (pCameraControlNotifyInfo == NULL||strXML == NULL)
		return -1;

	try
	{
		memset(pCameraControlNotifyInfo,0,sizeof(VIDEO_CAMERA_CONTROL_NOTIFY_INFO));

		resXML.Parse(strXML);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return -1;

		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return -1;

		if (strcmp(pResult, "Control_Notify") != 0) 
			return -1;

		pItem = pElement->FirstChildElement("Item");
		if (pItem == NULL)
			return -1;

		pResult = pItem->Attribute("Station");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pCameraControlNotifyInfo->szStationName,sizeof(pCameraControlNotifyInfo->szStationName),szGB2312Info);
		}

		pResult = pItem->Attribute("Name");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pCameraControlNotifyInfo->szDeviceName,sizeof(pCameraControlNotifyInfo->szDeviceName),szGB2312Info);
		}

		pResult = pItem->Attribute("Code");
		if (pResult != NULL)
			strcpy_s(pCameraControlNotifyInfo->szDeviceCode,sizeof(pCameraControlNotifyInfo->szDeviceCode),pResult);

		pResult = pItem->Attribute("User");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pCameraControlNotifyInfo->szUserName,sizeof(pCameraControlNotifyInfo->szUserName),szGB2312Info);
		}

		return 0;
	}
	catch (...)
	{

	}

	return -1;	
}


// 解析气象告警联动
int ParseWeatherWarningLinkageInfo(VIDEO_WEATHER_WARNING_INFO *pVideoLinkageInfo,const char *strXML)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	TiXmlElement *pSubList = NULL;
	TiXmlElement *pItem = NULL;
	const char		*pResult = NULL;
	int nNum = 0;
	int i = 0;

	char szGB2312Info[1024] = {0};
	int nGB2312InfoLen = 0;

	if (pVideoLinkageInfo == NULL||strXML == NULL)
		return -1;

	try
	{
		memset(pVideoLinkageInfo,0,sizeof(VIDEO_WEATHER_WARNING_INFO));

		resXML.Parse(strXML);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return -1;

		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return -1;

		if (strcmp(pResult, "Weather_Warn") != 0) 
			return -1;

		pSubList = pElement->FirstChildElement("SubList");
		if (pSubList == NULL)
			return -1;

		pResult = pSubList->Attribute("StationID");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szStationID,sizeof(pVideoLinkageInfo->szStationID),szGB2312Info);
		}

		pResult = pSubList->Attribute("WeatherType");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szWeatherType,sizeof(pVideoLinkageInfo->szWeatherType),szGB2312Info);
		}

		pResult = pSubList->Attribute("TypeName");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szTypeName,sizeof(pVideoLinkageInfo->szTypeName),szGB2312Info);
		}

		pResult = pSubList->Attribute("WarnValue");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szWarnValue,sizeof(pVideoLinkageInfo->szWarnValue),szGB2312Info);
		}

		pResult = pSubList->Attribute("Time");
		if (pResult != NULL)
		{
			strcpy_s(pVideoLinkageInfo->szTime,sizeof(pVideoLinkageInfo->szTime),pResult);
		}

		pResult = pSubList->Attribute("LinkStation");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szLinkageStationName,sizeof(pVideoLinkageInfo->szLinkageStationName),szGB2312Info);
		}

		pResult = pSubList->Attribute("LinkNum");
		if (pResult != NULL)
			pVideoLinkageInfo->nLinkNum = nNum = atoi(pResult);

		if (nNum <= 0)
			return -1;

		if (nNum > 200)//最多解析200个
			pVideoLinkageInfo->nLinkNum = nNum = 200;

		pVideoLinkageInfo->pLinkageCameraInfo = new VIDEO_LINKAGE_CAMERA_INFO[nNum];

		if (pVideoLinkageInfo->pLinkageCameraInfo == NULL)
			return -1;

		memset(pVideoLinkageInfo->pLinkageCameraInfo,0,sizeof(VIDEO_LINKAGE_CAMERA_INFO)*nNum);

		pItem = pSubList->FirstChildElement("Item");
		while(pItem != NULL && i < nNum)
		{
			pResult = pItem->Attribute("Scene");
			if (pResult != NULL)
			{
				if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
					strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szSceneName,VIDEO_SCENE_NAME_LEN,szGB2312Info);
			}

			pResult = pItem->Attribute("Code");
			if (pResult != NULL)
				strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szCode,LINKAGE_DEVICE_CODE_LEN,pResult);

			pResult = pItem->Attribute("Name");
			if (pResult != NULL)
			{
				if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
					strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szName,LINKAGE_DEVICE_NAME_LEN,szGB2312Info);
			}

			pResult = pItem->Attribute("DecoderTag");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nDecodeTag = atoi(pResult);

			pResult = pItem->Attribute("PresetName");
			if (pResult != NULL)
			{
				if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
					strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szPresetName,VIDEO_PRESET_NAME_LEN,szGB2312Info);
			}

			pResult = pItem->Attribute("PresetId");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nPresetId = atoi(pResult);

			pResult = pItem->Attribute("FromX1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromX1 = atoi(pResult);

			pResult = pItem->Attribute("FromY1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromY1 = atoi(pResult);

			pResult = pItem->Attribute("ToX1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToX1 = atoi(pResult);

			pResult = pItem->Attribute("ToY1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToY1 = atoi(pResult);

			pResult = pItem->Attribute("FromX2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromX2 = atoi(pResult);

			pResult = pItem->Attribute("FromY2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromY2 = atoi(pResult);

			pResult = pItem->Attribute("ToX2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToX2 = atoi(pResult);

			pResult = pItem->Attribute("ToY2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToY2 = atoi(pResult);


			i ++;
			pItem = pItem->NextSiblingElement("Item");
		}

		return 0;
	}
	catch (...)
	{

	}

	try
	{
		if (pVideoLinkageInfo->pLinkageCameraInfo != NULL)
		{
			delete [] pVideoLinkageInfo->pLinkageCameraInfo;
			pVideoLinkageInfo->pLinkageCameraInfo = NULL;
		}
	}
	catch(...)
	{

	}

	return -1;	

}

// 解析气象预警联动
int ParseWeatherForecastLinkageInfo(VIDEO_WEATHER_FORECAST_INFO *pVideoLinkageInfo,const char *strXML)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	TiXmlElement *pSubList = NULL;
	TiXmlElement *pItem = NULL;
	const char		*pResult = NULL;
	int nNum = 0;
	int i = 0;

	char szGB2312Info[2048] = {0};
	int nGB2312InfoLen = 0;

	if (pVideoLinkageInfo == NULL||strXML == NULL)
		return -1;

	try
	{
		memset(pVideoLinkageInfo,0,sizeof(VIDEO_WEATHER_FORECAST_INFO));

		resXML.Parse(strXML);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return -1;

		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return -1;

		if (strcmp(pResult, "Weather_Forecast") != 0) 
			return -1;

		pSubList = pElement->FirstChildElement("SubList");
		if (pSubList == NULL)
			return -1;

		pResult = pSubList->Attribute("ID");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szID,sizeof(pVideoLinkageInfo->szID),szGB2312Info);
		}

		pResult = pSubList->Attribute("WeatherType");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szWeatherType,sizeof(pVideoLinkageInfo->szWeatherType),szGB2312Info);
		}

		pResult = pSubList->Attribute("TypeName");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szTypeName,sizeof(pVideoLinkageInfo->szTypeName),szGB2312Info);
		}

		pResult = pSubList->Attribute("WeatherLevel");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szWeatherLevel,sizeof(pVideoLinkageInfo->szWeatherLevel),szGB2312Info);
		}

		pResult = pSubList->Attribute("AreaType");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szAreaType,sizeof(pVideoLinkageInfo->szAreaType),szGB2312Info);
		}

		pResult = pSubList->Attribute("CityID");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szCityID,sizeof(pVideoLinkageInfo->szCityID),szGB2312Info);
		}

		pResult = pSubList->Attribute("CountyID");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szCountyID,sizeof(pVideoLinkageInfo->szCountyID),szGB2312Info);
		}

		pResult = pSubList->Attribute("Content");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szContent,sizeof(pVideoLinkageInfo->szContent),szGB2312Info);
		}

		pResult = pSubList->Attribute("StartTime");
		if (pResult != NULL)
		{
			strcpy_s(pVideoLinkageInfo->szStartTime,sizeof(pVideoLinkageInfo->szStartTime),pResult);
		}

		pResult = pSubList->Attribute("StopTime");
		if (pResult != NULL)
		{
			strcpy_s(pVideoLinkageInfo->szStopTime,sizeof(pVideoLinkageInfo->szStopTime),pResult);
		}

		pResult = pSubList->Attribute("LinkStation");
		if (pResult != NULL)
		{
			if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
				strcpy_s(pVideoLinkageInfo->szLinkageStationName,sizeof(pVideoLinkageInfo->szLinkageStationName),szGB2312Info);
		}

		pResult = pSubList->Attribute("LinkNum");
		if (pResult != NULL)
			pVideoLinkageInfo->nLinkNum = nNum = atoi(pResult);

		if (nNum <= 0)
			return -1;

		if (nNum > 200)//最多解析200个
			pVideoLinkageInfo->nLinkNum = nNum = 200;

		pVideoLinkageInfo->pLinkageCameraInfo = new VIDEO_LINKAGE_CAMERA_INFO[nNum];

		if (pVideoLinkageInfo->pLinkageCameraInfo == NULL)
			return -1;

		memset(pVideoLinkageInfo->pLinkageCameraInfo,0,sizeof(VIDEO_LINKAGE_CAMERA_INFO)*nNum);

		pItem = pSubList->FirstChildElement("Item");
		while(pItem != NULL && i < nNum)
		{
			pResult = pItem->Attribute("Scene");
			if (pResult != NULL)
			{
				if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
					strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szSceneName,VIDEO_SCENE_NAME_LEN,szGB2312Info);
			}

			pResult = pItem->Attribute("Code");
			if (pResult != NULL)
				strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szCode,LINKAGE_DEVICE_CODE_LEN,pResult);

			pResult = pItem->Attribute("Name");
			if (pResult != NULL)
			{
				if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
					strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szName,LINKAGE_DEVICE_NAME_LEN,szGB2312Info);
			}

			pResult = pItem->Attribute("DecoderTag");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nDecodeTag = atoi(pResult);

			pResult = pItem->Attribute("PresetName");
			if (pResult != NULL)
			{
				if(ConvertUTF8ToGBK((char *)pResult,szGB2312Info,sizeof(szGB2312Info),nGB2312InfoLen))
					strcpy_s(pVideoLinkageInfo->pLinkageCameraInfo[i].szPresetName,VIDEO_PRESET_NAME_LEN,szGB2312Info);
			}

			pResult = pItem->Attribute("PresetId");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nPresetId = atoi(pResult);

			pResult = pItem->Attribute("FromX1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromX1 = atoi(pResult);

			pResult = pItem->Attribute("FromY1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromY1 = atoi(pResult);

			pResult = pItem->Attribute("ToX1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToX1 = atoi(pResult);

			pResult = pItem->Attribute("ToY1");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToY1 = atoi(pResult);

			pResult = pItem->Attribute("FromX2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromX2 = atoi(pResult);

			pResult = pItem->Attribute("FromY2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nFromY2 = atoi(pResult);

			pResult = pItem->Attribute("ToX2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToX2 = atoi(pResult);

			pResult = pItem->Attribute("ToY2");
			if (pResult != NULL)
				pVideoLinkageInfo->pLinkageCameraInfo[i].nToY2 = atoi(pResult);


			i ++;
			pItem = pItem->NextSiblingElement("Item");
		}

		return 0;
	}
	catch (...)
	{

	}

	try
	{
		if (pVideoLinkageInfo->pLinkageCameraInfo != NULL)
		{
			delete [] pVideoLinkageInfo->pLinkageCameraInfo;
			pVideoLinkageInfo->pLinkageCameraInfo = NULL;
		}
	}
	catch(...)
	{

	}

	return -1;	
}
