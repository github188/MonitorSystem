#ifndef _SYSTEMLINKAGESETTINGAPI_H_
#define _SYSTEMLINKAGESETTINGAPI_H_

#define _x_system_linkage_setting_dll_ __declspec(dllexport)
//---------------------------------------------------------
// C语言标准
#ifdef __cplusplus
extern "C"{
#endif
	_x_system_linkage_setting_dll_ BOOL SystemLinkageSetting_ShowWindow(HWND hParentWnd,BOOL bShow);
	_x_system_linkage_setting_dll_ BOOL SystemLinkageSetting_SetAPI(void* pCallAPI,void* pTransactionAPI,void* pConfigInfo);//设置SIP接口的API
	_x_system_linkage_setting_dll_ BOOL SystemLinkageSetting_SipCall_StateChangeEvent(unsigned int nCID,int nState,int nReason,void* pAppData);//SIP呼叫状态回调函数
	_x_system_linkage_setting_dll_ BOOL SystemLinkageSetting_SipCall_JRtpRecvMediaData(int nCID,void* pData,int nDataLen,int nMediaType,unsigned int nMediaPayload,bool bMark,unsigned int nTimestamp,unsigned int nSeqNo);//SIP媒体流回调函数
	_x_system_linkage_setting_dll_ BOOL SystemLinkageSetting_SipTrans_StateChangeEvent(unsigned int nTID,int nState,int nReason,void* pAppData);//SIP事务状态回调函数
	_x_system_linkage_setting_dll_ BOOL SystemLinkageSetting_PreTranslateMessage(MSG* pMsg);
	_x_system_linkage_setting_dll_ BOOL SystemLinkageSetting_ReLoginUser(char* szUserName);
#ifdef __cplusplus
}
#endif

//--------------------------------
#undef _x_system_linkage_setting_dll_
#endif