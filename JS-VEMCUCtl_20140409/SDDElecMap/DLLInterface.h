#pragma once
#include <vector>
using namespace std;

#define  _EXPORT_ELECMAPDLL	 __declspec(dllexport)

enum ERROR_INFO//error information list
{
	ERROR_NONE =0,
	ERROR_EXCEPTION
};

enum TYPE_DEVICE
{
	enumYX =0,
	enumYSH,
	enumYC
};

#ifdef __cplusplus

extern "C" 
{
	//@ func: show elec map view dialog.
	_EXPORT_ELECMAPDLL BOOL ElecMapWindow_Show(HWND hWnd, BOOL bShow);

	//@ func:  opened elec map by scene name.
	//
	//@ param: strSceneName(scene name).
	//
	//@ return: int(error information).
	_EXPORT_ELECMAPDLL int OpenASceneElecMap(const CString& strSceneName);

	//@ func:    open a scene  elecmap of where rvuid=xxx smid=xxx.
	//
	//@ param:    int rvuID     ruv id 
	//		     int smID      sm  id
	//
	//@ return:  int           error information
	_EXPORT_ELECMAPDLL int OpenASceneElecMapBySM(const int rvuID, const int smID);

	//note: design for MDI,  not used currently.
	_EXPORT_ELECMAPDLL int OpenAStateElecMap(const std::vector<CString> vecStrNames);
	
	//@func:     set elecmap dialog position
	//
	//@param:    CRect
	_EXPORT_ELECMAPDLL void SetElecMapDlgPosition(CRect rect);

	//@ func: refresh yx device status.
	_EXPORT_ELECMAPDLL void ElecMap_RefreshYXDevState(char *pRealData, int nCount);

	_EXPORT_ELECMAPDLL BOOL ElecMap_TranslateMessage(MSG* pMsg);
}

#endif