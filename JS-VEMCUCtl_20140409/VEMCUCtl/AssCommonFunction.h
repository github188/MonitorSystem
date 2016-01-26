#ifndef _ASSCOMMONFUNCTION_H_
#define _ASSCOMMONFUNCTION_H_

int WaitTimeOut(CEvent *pEvent, int nTimeOut);
void GetLinkageTypeTextByLinkageType(char *pLinkageTypeText, int nLinkageType);
void GetLinkageActionTextByLinkageAction(char *pLinkageActionText, int nLinkageAction);
void GetAlarmTypeText(char *szAlarmTypeText,int nAlarmType);
double CheckLastLinkageMessageTime(char *szCurrentTime, char *szLastTime);
int GetDvrTypeByDvrText(char *szDvrText);
void InitDlgButtonColor(HWND hWnd);
CString IntToCString(int n);
CString GetProcessStatusText(int nProcessStatus);
CString GetPriorLevelText(int nPriorLevel);
void InitButtonColor(CWnd *pWnd);
int GetCurrentYear();
CString GetCurrentProcessAlarmTime();
#endif
