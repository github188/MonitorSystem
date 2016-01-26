#include "stdafx.h"
#include "gloable/FileRead.h"
#include "gloable/mysql_interface.h"
#include "gloable/sql.h"

CFileRead::CFileRead()
{

}

CFileRead::~CFileRead()
{

}

CString CFileRead::getAppPath()
{
	//TODO: get app current path.
	CString strPath("global\\data\\");
	return strPath;
}

std::vector<CString> CFileRead::getPowerGridName(const CString &strFilePath)
{
	CString strPath("global\\data\\JiangSu.ini");
	CStdioFile file;
	if (!file.Open(strPath, CStdioFile::modeRead))
	{
		TRACE0("read database error.");
	}

	CString  lineText;
	int size=0;
	while(file.ReadString(lineText))
	{
		lineText.Trim();
		if(lineText.IsEmpty())
			continue;
		size++;
	}

	std::vector<CString> vecPowerGridName;
	vecPowerGridName.resize(size);

	file.SeekToBegin();
	for (int i=0; i<size; i++)
	{
		file.ReadString(lineText);
		lineText.Trim();

		if(lineText.IsEmpty())
			continue;

		vecPowerGridName[i] = lineText;
	}
	
	file.Close();

	return vecPowerGridName;
}

BOOL CFileRead::getCameraIDBySmID( int smID, std::vector<CString>& vecCameraID )
{
	if (!CMySqlInterface::isOpen()){	
		BOOL bRes = CMySqlInterface::openDatabase(g_host, g_user, g_pwd, g_dbName, g_port);
		if (!bRes){
			TRACE("open database failed.");
			return FALSE;
		}
	}

	char sql_buf[1024];
	memset(sql_buf,0,sizeof(sql_buf));
	sprintf_s(sql_buf,STR_READ_CAMERAID_BYSMID.c_str(),smID);

	BOOL ok = CMySqlInterface::executeSQL(sql_buf);
	if (!ok){
		TRACE0("execute sql failed.\n");
		return FALSE;
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;

	res = mysql_store_result(CMySqlInterface::getMysqlInstance());
	while (row = mysql_fetch_row(res)){
		CString strRead;
		strRead.Format("%s",row[0]);
		vecCameraID.push_back(strRead);
	}

	mysql_free_result(res) ;

	return TRUE;
}

BOOL CFileRead::getCameraNameByID( const CString strCamID, CString& strCamName )
{
	if (!CMySqlInterface::isOpen()){	
		BOOL bRes = CMySqlInterface::openDatabase(g_host, g_user, g_pwd, g_dbName, g_port);
		if (!bRes){
			TRACE("open database failed.");
			return FALSE;
		}
	}

	char sql_buf[1024];
	memset(sql_buf,0,sizeof(sql_buf));
	sprintf_s(sql_buf,STR_READ_CAMERANAME_BYID.c_str(),strCamID);

	BOOL ok = CMySqlInterface::executeSQL(sql_buf);
	if (!ok){
		TRACE0("execute sql failed.\n");
		return FALSE;
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;

	res = mysql_store_result(CMySqlInterface::getMysqlInstance());
	if (row = mysql_fetch_row(res)){
		strCamName.Format("%s",row[0]);
	}

	mysql_free_result(res) ;

	return TRUE;
}

BOOL CFileRead::getDevIDByRvuIDAndsmID( int rvuID, int smID, int& devID )
{
	if (!CMySqlInterface::isOpen()){	
		BOOL bRes = CMySqlInterface::openDatabase(g_host, g_user, g_pwd, g_dbName, g_port);
		if (!bRes){
			TRACE("open database failed.");
			return FALSE;
		}
	}

	char sql_buf[1024];
	memset(sql_buf,0,sizeof(sql_buf));
	sprintf_s(sql_buf,STR_READ_DEVID_BY_RVUANDSMID.c_str(),rvuID,smID);

	BOOL ok = CMySqlInterface::executeSQL(sql_buf);
	if (!ok){
		TRACE0("execute sql failed.\n");
		return FALSE;
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;

	res = mysql_store_result(CMySqlInterface::getMysqlInstance());
	if (row = mysql_fetch_row(res)){
		CString str;
		str.Format("%s",row[0]);
		devID = atoi(str);
	}

	mysql_free_result(res) ;

	return TRUE;
}

BOOL CFileRead::getRvuIDAndSmIDBySceneAndEname( CString strSceneID, CString strEname, int& rvuID, int& smID )
{
	if (!CMySqlInterface::isOpen()){	
		BOOL bRes = CMySqlInterface::openDatabase(g_host, g_user, g_pwd, g_dbName, g_port);
		if (!bRes){
			TRACE("open database failed.");
			return FALSE;
		}
	}

	char sql_buf[1024];
	memset(sql_buf,0,sizeof(sql_buf));
	sprintf_s(sql_buf,STR_READ_RVUANDSMID_BY_SCENEANDENAME.c_str(),strSceneID,strEname);

	BOOL ok = CMySqlInterface::executeSQL(sql_buf);
	if (!ok){
		TRACE0("execute sql failed.\n");
		return FALSE;
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;

	res = mysql_store_result(CMySqlInterface::getMysqlInstance());
	if (row = mysql_fetch_row(res)){
		CString str;
		str.Format("%s",row[0]);
		rvuID = atoi(str);

		str.Format("%s",row[1]);
		smID = atoi(str);
	}

	mysql_free_result(res) ;

	return TRUE;
}

BOOL CFileRead::getCameraIDSByDevID( int devID, std::vector<CString>& vecCameraID )
{
	if (!CMySqlInterface::isOpen()){	
		BOOL bRes = CMySqlInterface::openDatabase(g_host, g_user, g_pwd, g_dbName, g_port);
		if (!bRes){
			TRACE("open database failed.");
			return FALSE;
		}
	}

	char sql_buf[1024];
	memset(sql_buf,0,sizeof(sql_buf));
	sprintf_s(sql_buf,STR_READ_CAMERAIDS_BY_DEVID.c_str(),devID);

	BOOL ok = CMySqlInterface::executeSQL(sql_buf);
	if (!ok){
		TRACE0("execute sql failed.\n");
		return FALSE;
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;

	res = mysql_store_result(CMySqlInterface::getMysqlInstance());
	while (row = mysql_fetch_row(res)){
		CString str;
		str.Format("%s",row[0]);
		
		vecCameraID.push_back(str);
	}

	mysql_free_result(res) ;

	return TRUE;
}

BOOL CFileRead::getCameraNameByCameraID( CString strCameraID, CString& strCameraName )
{
	if (!CMySqlInterface::isOpen()){	
		BOOL bRes = CMySqlInterface::openDatabase(g_host, g_user, g_pwd, g_dbName, g_port);
		if (!bRes){
			TRACE("open database failed.");
			return FALSE;
		}
	}

	char sql_buf[1024];
	memset(sql_buf,0,sizeof(sql_buf));
	sprintf_s(sql_buf,STR_READ_CAMERANAME_BY_CAMERAID.c_str(), strCameraID);

	BOOL ok = CMySqlInterface::executeSQL(sql_buf);
	if (!ok){
		TRACE0("execute sql failed.\n");
		return FALSE;
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;

	res = mysql_store_result(CMySqlInterface::getMysqlInstance());
	if (row = mysql_fetch_row(res)){
		strCameraName.Format("%s",row[0]);
	}

	mysql_free_result(res) ;

	return TRUE;
}

BOOL CFileRead::getSceneIDBySceneName( CString strSceneName, CString& strSceneID )
{
	if (!CMySqlInterface::isOpen()){	
		BOOL bRes = CMySqlInterface::openDatabase(g_host, g_user, g_pwd, g_dbName, g_port);
		if (!bRes){
			TRACE("open database failed.");
			return FALSE;
		}
	}

	char sql_buf[1024];
	memset(sql_buf,0,sizeof(sql_buf));
	sprintf_s(sql_buf,STR_READ_SCENEID_BY_SCENENAME.c_str(), strSceneName);

	BOOL ok = CMySqlInterface::executeSQL(sql_buf);
	if (!ok){
		TRACE0("execute sql failed.\n");
		return FALSE;
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;

	res = mysql_store_result(CMySqlInterface::getMysqlInstance());
	if (row = mysql_fetch_row(res)){
		strSceneID.Format("%s",row[0]);
	}

	mysql_free_result(res) ;

	return TRUE;
}

BOOL CFileRead::getCameraIDBySceneIDAndEname( CString strSceneID, CString strEname, CString& strCameraID )
{
	if (!CMySqlInterface::isOpen()){	
		BOOL bRes = CMySqlInterface::openDatabase(g_host, g_user, g_pwd, g_dbName, g_port);
		if (!bRes){
			TRACE("open database failed.");
			return FALSE;
		}
	}

	char sql_buf[1024];
	memset(sql_buf,0,sizeof(sql_buf));
	sprintf_s(sql_buf,STR_READ_CAMERAID_BY_SCENEIDANDENAME.c_str(), strSceneID, strEname);

	BOOL ok = CMySqlInterface::executeSQL(sql_buf);
	if (!ok){
		TRACE0("execute sql failed.\n");
		return FALSE;
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;

	res = mysql_store_result(CMySqlInterface::getMysqlInstance());
	if (row = mysql_fetch_row(res)){
		strCameraID.Format("%s",row[0]);
	}

	mysql_free_result(res) ;

	return TRUE;
}

BOOL CFileRead::getSceneIDByRvuIDAndSmID( int rvuID, int smID, CString& strSceneID )
{
	if (!CMySqlInterface::isOpen()){	
		BOOL bRes = CMySqlInterface::openDatabase(g_host, g_user, g_pwd, g_dbName, g_port);
		if (!bRes){
			TRACE("open database failed.\n");
			return FALSE;
		}
	}

	char sql_buf[1024];
	memset(sql_buf,0,sizeof(sql_buf));
	sprintf_s(sql_buf,STR_READ_SCENEID_BY_RVUANDSMID.c_str(), rvuID, smID);

	BOOL ok = CMySqlInterface::executeSQL(sql_buf);
	if (!ok){
		TRACE0("execute sql failed.\n");
		return FALSE;
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;

	res = mysql_store_result(CMySqlInterface::getMysqlInstance());
	if (row = mysql_fetch_row(res)){
		strSceneID.Format("%s",row[0]);
	}

	mysql_free_result(res) ;

	return TRUE;
}

BOOL CFileRead::getSceneNameBySceneID( CString strSceneID, CString& strSceneName )
{
	if (!CMySqlInterface::isOpen()){	
		BOOL bRes = CMySqlInterface::openDatabase(g_host, g_user, g_pwd, g_dbName, g_port);
		if (!bRes){
			TRACE("open database failed.");
			return FALSE;
		}
	}

	char sql_buf[1024];
	memset(sql_buf,0,sizeof(sql_buf));
	sprintf_s(sql_buf,STR_READ_SCENENAME_BY_SCENEID.c_str(), strSceneID);

	BOOL ok = CMySqlInterface::executeSQL(sql_buf);
	if (!ok){
		TRACE0("execute sql failed.\n");
		return FALSE;
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;

	res = mysql_store_result(CMySqlInterface::getMysqlInstance());
	if (row = mysql_fetch_row(res)){
		strSceneName.Format("%s",row[0]);
	}

	mysql_free_result(res) ;

	return TRUE;
}
