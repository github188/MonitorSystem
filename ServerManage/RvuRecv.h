

void RvuConnect(void* arg);
void SendRVUBuf(void* arg);
int ParseRVUSendLogin(int handle,unsigned char* buff,int size);
int ParseRVUSendSM(int handle,unsigned char* buff,int size,int rvuid);
int ParseRVUSendNODE(int handle,unsigned char* buff,int size,int rvuid);
int ParseRVUSendREAL(int handle,unsigned char* buff,int size,int rvuid);
//void ParseRVUSendRealtimeAlarm(int handle,unsigned char* buff,int size,int rvuid);
void ParseRVURecvBuf(void* arg);
int recv_parse(NETHANDLE handle,char* buf,int size,void* user);