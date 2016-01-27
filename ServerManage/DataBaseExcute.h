#pragma once

void GetRVUInfo();
void GetDEVInfo();
void CreateTables();
void AutoCreat(void *arg);
int GetDEVType(int rvuid,int smid,C_LINKINFO *c_linkinfo);
void GetUserInfo(void *arg);
int connectDB();
void SqlInsert(void *arg);
int disconnectDB();
//int SaveRvuSM(T_SM *t_sm,int count,int rvuid);
void SaveRvuSM(void *arg);
void Save104RvuSM(void *arg);
void SaveRvuNODE(void *arg);
void SaveRvuData(void *arg);
void Save104RvuData(void *arg);
void SaveRelation(void *arg);
//void SaveRvuAlarm(void *arg);
int cmpLogin(_T_ASS_LOGIN_USER_INFO_ *login_user);
int GetCameraInfo(char *camera_code,C_CAMERAINFO *camerainfo);


