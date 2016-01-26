#ifndef _ASSCAMERAOPERATE_H_
#define _ASSCAMERAOPERATE_H_

//打开摄像头
bool AssOpenCamera(char* ip,unsigned short port, char* user, char* password,unsigned short dvrtype,unsigned short dvrid,unsigned short channel,int id ,HWND playwnd);

//摄像头转预置位
bool AssToPreset(char* ip,unsigned short port, char* user, char* password,unsigned short dvrtype,unsigned short dvrid,unsigned short channel,unsigned short control,unsigned short speed);

//关闭摄像头
bool AssCloseCamera(int nID);

#endif