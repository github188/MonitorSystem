#ifndef _ACSSYSTEMAPI_H_
#define _ACSSYSTEMAPI_H_

#define _x_ACSSystem_dll_ __declspec(dllexport)
typedef int (*CALLBACKACSSYSTEMOPENDVIDEOID)(int);
extern CALLBACKACSSYSTEMOPENDVIDEOID callbackacssystemopenvideoid;
//---------------------------------------------------------
// CÓïÑÔ±ê×¼
#ifdef __cplusplus
extern "C"{
#endif
	_x_ACSSystem_dll_ BOOL ACSSystem_ShowWindow(HWND hParentWnd,BOOL bShow);
	_x_ACSSystem_dll_ BOOL ACSSystem_SetAPI(void* pConfigInfo);
	_x_ACSSystem_dll_ BOOL ACSSystem_PreTranslateMessage(MSG* pMsg);
	_x_ACSSystem_dll_ void ACSSystem_ProcessSMRealData(char *pSmRealData, int nCount);
	_x_ACSSystem_dll_ BOOL ACSSystem_SetCallbackGetOpenVideoId(CALLBACKACSSYSTEMOPENDVIDEOID GetACSSystemOpenDVideoIdCB);
	_x_ACSSystem_dll_ BOOL ACSSystem_SetDlgPosition(CRect rect);
#ifdef __cplusplus
}
#endif

//--------------------------------
#undef _x_ACSSystem_dll_
#endif