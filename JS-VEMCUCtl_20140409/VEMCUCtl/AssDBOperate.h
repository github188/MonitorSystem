#ifndef _ASSDBOPERATE_H_
#define _ASSDBOPERATE_H_

#include <vector>

bool InsertLinkageConfigInfoToDB(T_ASS_LINKAGE_CONFIG* pAssLinkageConfig);
BOOL GetSubscribeInfoById(T_ASS_SUBSCRIBE_INFO *pAssSubscribeInfo, int nId);
BOOL GetCameraNameByCameraCode(ASS_CAMERA_INFO *pAssCameraInfo, char *szCameraCode);
//判断一个表中是否存在某个记录
bool CheckExists(char *szTable, char *szCondition);
void GetDevInfoByDevId(ACS_DEV_INFO *pDevInfo, int nDevId);
void GetStationInfoByStationId(ACS_STATION_INFO *pStationInfo, int nStationId);
void GetLinkageConfig(T_ASS_LINKAGE_CONFIG *pAssLinkageConfig);
void GetCameraInfoByCameraCode(ASS_CAMERA_INFO *pCameraInfo, char *szCameraCode);
int GetDevIdByDevName(char *szDevName);
BOOL GetCameraCodeByCameraName(char *szCameraCode, char *szCameraName);
void GetStationCodeByStationName(char *szStationCode, char *szStationName);
std::vector<PUSH_DATA_RESPONSE> GetDeviceRealDataInfoBySceneName(CString strSceneName);
std::vector<_T_ASS_DEVICE_LEDGER_> GetDeviceLedgersFromDB(CString strCondition);
std::vector<ALARM_STATISTICS> GetAlarmStatisticsInfo();
std::vector<_T_ASS_SUBSYSTEM_ALARM_COUNT> GetSubSystemAlarmCount(CString strCondition);
std::vector<ACS_ALARM_INFO> GetAlarmInfoByStationId(CString strCondition);
int GetStationIdByStationName(CString strStationName);
bool UpdateAlarmInfoToCompleted(int nAlarmID, int nAlarmStatus);
#endif
