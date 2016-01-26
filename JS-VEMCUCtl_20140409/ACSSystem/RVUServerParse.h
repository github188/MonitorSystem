#pragma once

void ParseRVUResponseSetSM(unsigned char* buff,int size);
void ParseRVUResponseSetNodes(unsigned char* buff,int size);
void ParseRVUResponseSetPointAck(unsigned char* buff,int size);
void ParseRVUSendRealtimeAlarm(int handle,unsigned char* buff,int size);
//void ParseRVUServerKeepAlive(unsigned char* buff,int size);
void CheckRVUIsOnLine(int handle);
int recv_parse(NETHANDLE handle,char* buf,int size,void* user);