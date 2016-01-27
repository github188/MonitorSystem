int recv_104_parse(NETHANDLE handle,char* buf,int size,void* user);
void Parse104RVURecvBuf(void* arg);
int Parse104RVUSendSM(int handle,unsigned char* buff,int rvuid);
int Parse104RVUSendREAL(int handle,unsigned char* buff,int rvuid,int enumstate);