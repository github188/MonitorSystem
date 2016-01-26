#pragma once
#include <vector>
extern MYSQL			*g_LinkageSettingMysql;						            //mysql ˝æ›ø‚∑√Œ 
bool CheckExists(char *szTable, char *szCondition);
std::vector<T_ASS_DEVICE_TYPE> GetDeviceTypeFromDB();
std::vector<T_ASS_SUBSYSTEM> GetSubSystemFromDB();
std::vector<T_ASS_RVU_TYPE> GetRvuTypeFromDB();
BOOL UpdateSubSystemOfDeviceType(CString strDeviceTypeName, int nId);