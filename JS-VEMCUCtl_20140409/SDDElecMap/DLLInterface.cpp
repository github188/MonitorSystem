#include "StdAfx.h"
#include "resource.h"
#include "DLLInterface.h"
#include "SDDElecMapDlg.h"

BOOL ElecMapWindow_Show(HWND hWnd, BOOL bShow)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* pParentWnd = NULL;

	try
	{
		if (g_pMainDlg == NULL)
		{
			g_pMainDlg = new CSDDElecMapDlg;

			if (g_pMainDlg != NULL)
			{
				if (hWnd != NULL)
				{
					pParentWnd = CWnd::FromHandle(hWnd);
				}

				g_pMainDlg->Create(IDD_SDDELECMAP_DIALOG,pParentWnd);
			}
		}

		if (g_pMainDlg != NULL)
		{
			AfxGetApp()->m_pMainWnd = g_pMainDlg;
			g_pMainDlg->ShowWindow(bShow?SW_SHOW:SW_HIDE);
		}
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}

int OpenASceneElecMap(const CString& strSceneName)
{
	if (!g_pMainDlg){
		return -1;
	}

	if (g_pMainDlg->openASceneElecMap(strSceneName)){
		return 0;
	}

	return -1;
}

int OpenASceneElecMapBySM( const int rvuID, const int smID )
{
	if (!g_pMainDlg->openASceneElecMap(rvuID,smID)){
		return -1;
	}

	return (int)ERROR_NONE;
}

int OpenAStateElecMap(const std::vector<CString>& vecStrNames)
{
	for (size_t t=0; t<vecStrNames.size(); t++){
		OpenASceneElecMap(vecStrNames.at(t));
	}
	return -1;
}

void SetElecMapDlgPosition( CRect rect )
{
	g_pMainDlg->setDlgPosition(rect);
}

void ElecMap_RefreshYXDevState( char *pRealData, int nCount )
{
	if (!g_pMainDlg->updateEntityInStation(pRealData,nCount)){
		TRACE0("update entity status failed.\n");
		return;
	}
}

BOOL ElecMap_TranslateMessage( MSG* pMsg )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (g_pMainDlg == NULL){
		return TRUE;
	}

	try{
		if (g_pMainDlg != NULL){
			g_pMainDlg->PreTranslateMessage(pMsg);
		}
	}
	catch(...){
		return FALSE;
	}

	return TRUE;
}


