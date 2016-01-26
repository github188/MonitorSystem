#include "stdafx.h"
#include "AssCommonFunction.h"
#include "BtnST.h"

//等待某个信号多长时间
int WaitTimeOut(CEvent *pEvent, int nTimeOut)
{
	//下面要等待告警订阅返回消息返回成功
	DWORD dwReturn = WaitForSingleObject(pEvent, nTimeOut);
	switch (dwReturn)
	{
	case WAIT_OBJECT_0:
		//插入成功，则更新列表
		return 0;
		break;
		//等待超时
	case WAIT_TIMEOUT:
		return -1;
		break;
	}
}

CString GetCurrentProcessAlarmTime()
{
	CTime time = CTime::GetCurrentTime();
	//取处理时间
	CString strTime;
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d",
		time.GetYear(), time.GetMonth(), time.GetDay(),
		time.GetHour(), time.GetMinute(), time.GetSecond());
	return strTime;
}

int GetCurrentYear()
{
	CTime time = CTime::GetCurrentTime();
	int nYear = time.GetYear();
	return nYear;
}

void GetLinkageTypeTextByLinkageType(char *pLinkageTypeText, int nLinkageType)
{
	if (nLinkageType == LINKAGE_CAMERA)
	{
		sprintf_s(pLinkageTypeText,32,"摄像头");
	}
	if (nLinkageType == LINKAGE_DOOR)
	{
		sprintf_s(pLinkageTypeText,32,"视频告警");
	}
}

void GetLinkageActionTextByLinkageAction(char *pLinkageActionText, int nLinkageAction)
{
	if (nLinkageAction == LINKAGE_ACTION_OPEN)
	{
		sprintf_s(pLinkageActionText,32,"打开");
	}
	if (nLinkageAction == LINKAGE_ACTION_CLOSE)
	{
		sprintf_s(pLinkageActionText,32,"关闭");
	}
	if (nLinkageAction == LINKAGE_ACTION_PRESET)
	{
		sprintf_s(pLinkageActionText,32,"转预置位");
	}
}

//根据告警类型获取告警文本
void GetAlarmTypeText(char *szAlarmTypeText,int nAlarmType)
{
	if (nAlarmType == 1)
	{
		sprintf_s(szAlarmTypeText,32,"温湿度告警");
	}
	if (nAlarmType == 11)
	{
		sprintf_s(szAlarmTypeText,32,"安防告警");
	}
	if (nAlarmType == 12)
	{
		sprintf_s(szAlarmTypeText,32,"视频告警");
	}
}

CString GetPriorLevelText(int nPriorLevel)
{
	CString strText;
	if (nPriorLevel == 1)
	{
		strText = "一般告警";
	} 
	else
	{
		strText = "严重告警";
	}
	return strText;
}

CString GetProcessStatusText(int nProcessStatus)
{
	CString strText;
	if (nProcessStatus == 0)
	{
		strText = "已处理";
	} 
	else
	{
		strText = "未处理";
	}
	return strText;
}

//检查两个字符串的时间间隔
double CheckLastLinkageMessageTime(char *szCurrentTime, char *szLastTime)
{
	COleDateTime oleCurrentTime;
	oleCurrentTime.ParseDateTime(szCurrentTime);
	COleDateTime oleLastTime;
	oleLastTime.ParseDateTime(szLastTime);

	COleDateTimeSpan resultTimeSpan = oleCurrentTime - oleLastTime;

	double nInterval = resultTimeSpan.GetTotalSeconds();

	return nInterval;
}

int GetDvrTypeByDvrText(char *szDvrText)
{
	int nDvrType;
	if (stricmp(szDvrText,"HIK")==0)
	{
		nDvrType = DEVTYPE_HK;
	}
	else if (stricmp(szDvrText,"DH")==0)
	{
		nDvrType = DEVTYPE_DH;
	}
	else if (stricmp(szDvrText,"DL")==0)
	{
		nDvrType = DEVTYPE_DL;
	}
	else if (stricmp(szDvrText,"MOBO")==0)
	{
		nDvrType = DEVTYPE_MOBOTIX;
	}
	else
	{
		return -1;
	}

	return nDvrType;
}

void InitButtonColor(CWnd *pWnd)
{
	CButtonST* button = (CButtonST*)pWnd;
	button->SetColor(CButtonST::BTNST_COLOR_BK_OUT, BTNST_COLOR_BK_OUT_COLOR);
	button->SetColor(CButtonST::BTNST_COLOR_BK_IN, BTNST_COLOR_BK_IN_COLOR);
	button->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, BTNST_COLOR_BK_FOCUS_COLOR);
	//button->SetFlat(FALSE);
}

//初始化某个窗口按钮控件的颜色
void InitDlgButtonColor(HWND hWnd)
{
	HWND hWndChild = ::GetWindow(hWnd, GW_CHILD);
	while(hWndChild)
	{
		char buf[512] = {0};
		::GetClassName(hWndChild, buf, 512);
		CString str = buf;
		str.TrimRight();
		if (str == "Button")
		{
			CWnd *pWnd = CWnd::FromHandle(hWndChild);
			if (pWnd->IsKindOf(RUNTIME_CLASS(CButtonST)))
			{
				CButtonST* button = (CButtonST*)pWnd;
				button->SetColor(CButtonST::BTNST_COLOR_BK_OUT, BTNST_COLOR_BK_OUT_COLOR);
				button->SetColor(CButtonST::BTNST_COLOR_BK_IN, BTNST_COLOR_BK_IN_COLOR);
				button->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, BTNST_COLOR_BK_FOCUS_COLOR);
				//button->SetFlat(FALSE);
			}
		}
		hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT);
	}
}

CString IntToCString(int n)
{
	CString strText;
	strText.Format("%d",n);
	return strText;
}