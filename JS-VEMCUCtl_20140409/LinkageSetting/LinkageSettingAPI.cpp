#include "stdafx.h"
#include "LinkageSetting.h"
#include "LinkageSettingAPI.h"
#include "PtuDlg.h"

CALLBACKGETLINKAGESETTINGOPENDVIDEOID callbackgetlinkagesettingopenvideoid = NULL;

//显示主界面
BOOL LinkageSetting_ShowWindow(HWND hParentWnd,BOOL bShow)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* pParentWnd = NULL;

	try
	{
		if (g_pMainDlg == NULL)
		{
			g_pMainDlg = new CPtuDlg;

			if (g_pMainDlg != NULL)
			{
				if (hParentWnd != NULL)
				{
					pParentWnd = CWnd::FromHandle(hParentWnd);
				}

				g_pMainDlg->Create(IDD_DIALOG_PTU,pParentWnd);
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

//设置LinkageSetting的API
BOOL LinkageSetting_SetAPI(void* pConfigInfo)
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

//给LinkageSetting设置DevPlayVideoSdkFuns
BOOL LinkageSetting_SetDevPlayVideoSdkFuns(void* pDevPlayVideoSdkFuns)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	try
	{
		g_pstruDevPlayVideoSdkFunsEx = (DEV_PLAY_VIDEO_SDK_FUNS*)pDevPlayVideoSdkFuns;
	}

	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}

//给LinkageSetting设置获取openvideo的id的回调函数
BOOL LinkageSetting_SetCallbackGetOpenVideoId(CALLBACKGETLINKAGESETTINGOPENDVIDEOID GetLinkageSettingOpenDVideoIdCB)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	callbackgetlinkagesettingopenvideoid = GetLinkageSettingOpenDVideoIdCB;
	return TRUE;
}

//需要从主程序通过dll接口往里传递pMSG
BOOL LinkageSetting_PreTranslateMessage(MSG* pMSG)
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