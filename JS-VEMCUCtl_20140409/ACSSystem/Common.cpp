#include "stdafx.h"
#include "Common.h"

//获取实时数据状态文本
void GetStatusTextByStatus(char *szStatusText, int nStatus)
{
	if (nStatus == 0 || nStatus == 4)
	{
		sprintf_s(szStatusText,32,"无告警");
	}
	else
	{
		sprintf_s(szStatusText,32,"有告警");
	}
}

void GetRelateVideoText(char *szRelateVideoText, int nRelateVideo)
{
	if (nRelateVideo == 0)
	{
		sprintf_s(szRelateVideoText, 32, "无关联视频");
	}
	if (nRelateVideo == 1)
	{
		sprintf_s(szRelateVideoText, 32, "已关联视频");
	}
}

