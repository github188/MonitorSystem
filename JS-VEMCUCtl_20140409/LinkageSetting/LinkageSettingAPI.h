#ifndef _LINKAGESETTINGAPI_H_
#define _LINKAGESETTINGAPI_H_

#define _x_LinkageSetting_dll_ __declspec(dllexport)
typedef int (*CALLBACKGETLINKAGESETTINGOPENDVIDEOID)(int);
extern CALLBACKGETLINKAGESETTINGOPENDVIDEOID callbackgetlinkagesettingopenvideoid;
//---------------------------------------------------------
// CÓïÑÔ±ê×¼
#ifdef __cplusplus
extern "C"{
#endif
	_x_LinkageSetting_dll_ BOOL LinkageSetting_ShowWindow(HWND hParentWnd,BOOL bShow);
	_x_LinkageSetting_dll_ BOOL LinkageSetting_SetAPI(void* pConfigInfo);
	_x_LinkageSetting_dll_ BOOL LinkageSetting_SetDevPlayVideoSdkFuns(void* pDevPlayVideoSdkFuns);
	_x_LinkageSetting_dll_ BOOL LinkageSetting_SetCallbackGetOpenVideoId(CALLBACKGETLINKAGESETTINGOPENDVIDEOID GetLinkageSettingOpenDVideoIdCB);
	_x_LinkageSetting_dll_ BOOL LinkageSetting_PreTranslateMessage(MSG* pMsg);

#ifdef __cplusplus
}
#endif

//--------------------------------
#undef _x_LinkageSetting_dll_
#endif