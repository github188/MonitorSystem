#pragma  once

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;
	
//变电站实时消息结构
typedef struct _push_data_response
{
	int nRvuId;
	int nSmId;
	int nNodeId;
	float fValue;
	int nType;
	int nStatus;
}PUSH_DATA_RESPONSE;

typedef std::vector<PUSH_DATA_RESPONSE> VEC_STUREALDATA;

class CFileRead
{
public:
	CFileRead();
	~CFileRead();

public:
	static CString getAppPath();
	static std::vector<CString> getPowerGridName(const CString &strFilePath);

	static BOOL getDevIDByRvuIDAndsmID(int rvuID, int smID, int& devID);

	static BOOL	getRvuIDAndSmIDBySceneAndEname(CString strSceneID, CString strEname, int& rvuID, int& smID);

	static BOOL getCameraIDBySmID(int smID, std::vector<CString>& vecCameraID);

	static BOOL getCameraNameByID(const CString strCamID, CString& strCamName);

	static BOOL	getCameraIDSByDevID( int devID, std::vector<CString>& vecCameraID );

	static BOOL getCameraNameByCameraID(CString strCameraID, CString& strCameraName);

	static BOOL getSceneIDBySceneName(CString strSceneName, CString& strSceneID);

	static BOOL getCameraIDBySceneIDAndEname(CString strSceneID, CString strEname, CString& strCameraID);

	static BOOL getSceneIDByRvuIDAndSmID( int rvuID, int smID, CString& strSceneID );

	static BOOL	getSceneNameBySceneID( CString strSceneID, CString& strSceneName );
};