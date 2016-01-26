#pragma once

int connectDB();
int disconnectDB();
int GetRvuIDByStationID(int nStationId);
int GetStationIdByRvuId(int nRvuId);
void GetCameraCodeByDevId(int nStationId, int nDevId, T_LINKAGE_CAMERA_INFO *tCameraCode, int *nCount);
void GetSMRealDataInfoByPushDataResponse(T_SM_REALDATA *tSmRealData, PUSH_DATA_RESPONSE *pTSmRealData);