#ifndef __MYSQL__HEADER__
#define __MYSQL__HEADER__

#include <string>
using namespace std;

const char* g_host = _T("26.47.100.15");
const char* g_user = _T("root");
const char* g_pwd  = _T("mdcs2008");
const char* g_dbName = _T("test");
const int   g_port = (int)3306;


const string STR_READ_SCENEID_BY_SCENENAME = _T("SELECT scene_id FROM ass_d5000_station_scene WHERE station_scenename='%s'");

const string STR_READ_CAMERAID_BYSMID = _T("SELECT camera_code FROM ass_camerapreset WHERE dev_id=%d");

const string STR_READ_CAMERANAME_BYID = _T("SELECT name FROM video_camera WHERE camera_id='%s'");

const string STR_READ_RVUANDSMID_BY_SCENEANDENAME = _T("SELECT rvu_id,sm_id FROM ass_rvu_sm_entity WHERE scene_id='%s' AND sm_ename='%s'");

const string STR_READ_DEVID_BY_RVUANDSMID = _T("SELECT id FROM ass_rvu_sm WHERE rvu_id=%d AND sm_id=%d");

const string STR_READ_CAMERAIDS_BY_DEVID = _T("SELECT camera_code FROM ass_camerapreset WHERE dev_id=%d");

const string STR_READ_CAMERANAME_BY_CAMERAID = _T("SELECT name FROM video_camera WHERE camera_id='%s'");

const string STR_READ_CAMERAID_BY_SCENEIDANDENAME = _T("SELECT camera_id FROM ass_camera_entity WHERE scene_id='%s' AND camera_ename='%s'");

const string STR_READ_SCENEID_BY_RVUANDSMID = _T("SELECT scene_id FROM ass_rvu_sm_entity WHERE rvu_id=%d AND sm_id=%d");

const string STR_READ_SCENENAME_BY_SCENEID = _T("SELECT station_scenename FROM ass_d5000_station_scene WHERE scene_id='%s'");

#endif