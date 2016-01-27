#pragma once

int RecvPTUServerCallBack(NETHANDLE handle,char* buf,int size,void* user);

void ParseXML(void* arg);

int GetParseXMLType(char* buf,int size);

int ParseLoginInfo(_T_ASS_LOGIN_USER_INFO_* pAssLoginInfo,const char *strXML);

int ParseTimeSet(const char* strXML,char* sendtime);

int ParseKeepAlive(const char* strXML,char* sendtime);

int ParseYKControl(_T_ASS_CONTROL_ * passcontrol,const char* strXML);

int ParseStationInfo(_T_ASS_STATION_INFO_* pAssstation,const char *strXML);

int ParseAlarm(_T_ASS_ALARM_SET_* pAssalarm,const char *strXML);

int ParseHide(_T_ASS_HIDE_SET_* pAssHide,const char *strXML);

