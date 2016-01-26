#include "stdafx.h"
#include "ACSSystem.h"
#include "ACSSystemAPI.h"
#include "ACSSystemDlg.h"

CALLBACKACSSYSTEMOPENDVIDEOID callbackacssystemopenvideoid = NULL;

//设置ACSSystem的API
BOOL ACSSystem_SetAPI(void* pConfigInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		g_pConfigInfo = (VEM_USER_CONFIG_INFO*)pConfigInfo;
	}

	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}

//设置dll窗口位置
BOOL ACSSystem_SetDlgPosition(CRect rect)
{
	if (g_pMainDlg != NULL)
	{
		g_pMainDlg->MoveWindow(rect);
	}
	return TRUE;
}


//显示主界面
BOOL ACSSystem_ShowWindow(HWND hParentWnd,BOOL bShow)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* pParentWnd = NULL;

	try
	{
		if (g_pMainDlg == NULL)
		{
			g_pMainDlg = new CACSSystemDlg;

			if (g_pMainDlg != NULL)
			{
				if (hParentWnd != NULL)
				{
					pParentWnd = CWnd::FromHandle(hParentWnd);
				}

				g_pMainDlg->Create(IDD_ACSSYSTEM_DIALOG,pParentWnd);
			}
		}

		if (g_pMainDlg != NULL)
		{
			g_pMainDlg->ShowWindow(bShow?SW_SHOW:SW_HIDE);
		}
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}

//需要从主程序通过dll接口往里传递pMSG
BOOL ACSSystem_PreTranslateMessage(MSG* pMSG)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (g_pMainDlg == NULL)
	{
		return TRUE;
	}

	try
	{
		if (g_pMainDlg != NULL)
		{
			g_pMainDlg->PreTranslateMessage(pMSG);
		}
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

//给ACSSystem设置获取openvideo的id的回调函数
BOOL ACSSystem_SetCallbackGetOpenVideoId(CALLBACKACSSYSTEMOPENDVIDEOID GetACSSystemOpenDVideoIdCB)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	callbackacssystemopenvideoid = GetACSSystemOpenDVideoIdCB;
	return TRUE;
}

//处理设备实时数据
void ACSSystem_ProcessSMRealData(char* pSmRealData, int nCount)
{
	g_pMainDlg->RefreshYXListRealData(pSmRealData, nCount);
}
