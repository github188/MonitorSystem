// DlgTreeElecMap.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgTreeElecMap.h"
#include "xappany.h"
#include "VEMCUCtlDlg.h"
#include "DlgCameraAndControl.h"
#include "SDDElecMap/DLLInterface.h"

const char* g_host = _T("26.47.100.15");
const char* g_user = _T("root");
const char* g_pwd  = _T("mdcs2008");
const char* g_dbName = _T("test");
const int   g_nport = (int)3306;

const CString PROVINCE_NAME =_T("江苏省");
// CDlgTreeElecMap 对话框

extern CVEMCUCtlDlg		*g_pMainDlg;

IMPLEMENT_DYNAMIC(CDlgTreeElecMap, CDialog)

CDlgTreeElecMap::CDlgTreeElecMap(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTreeElecMap::IDD, pParent)
	,m_bIsAddProvinceScene(FALSE)
{

}

CDlgTreeElecMap::~CDlgTreeElecMap()
{
}

void CDlgTreeElecMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_ELECMAP, m_ctrlTree);
	DDX_Text(pDX, IDC_EDIT_SEARCHSTATE, m_strSearch);
	DDV_MaxChars(pDX, m_strSearch, 64);
	DDX_Control(pDX, IDC_BUTTON_SEARCHSTATE, m_btnSearch);
}


BEGIN_MESSAGE_MAP(CDlgTreeElecMap, CDialog)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE_ELECMAP, OnTreeSelChanging)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ELECMAP, OnTreeSelChange)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_ELECMAP, OnTreeNMDBLCLK)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void CDlgTreeElecMap::OnTreeSelChanging( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	*pResult = 0;
	//TODO: open a station scene.

}

void CDlgTreeElecMap::OnTreeSelChange( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	HTREEITEM hm = m_ctrlTree.GetSelectedItem();
	CString str;

	if (hm==NULL) return;
	DWORD dw=m_ctrlTree.GetItemData(hm);

	//TODO: open a station scene.

	*pResult = 0;
}

void CDlgTreeElecMap::OnTreeNMDBLCLK( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	HTREEITEM hm = m_ctrlTree.GetSelectedItem();
	CString strItemName = m_ctrlTree.GetItemText(hm);

	if (hm==NULL) return;
	DWORD dw=m_ctrlTree.GetItemData(hm);

	//TODO: open a station scene.  
	if ((dw&TREEITEM_CLASS_STATE)!=0){
		addSceneToStation(hm,strItemName);

		//把变电站名称传给CameraAndControl窗口
		int nStationId = GetStationIdByStationName(strItemName);
		g_pMainDlg->m_pDlgCameraAndControl->PostMessage(OM_ELECTREE_TO_CAMERAANDCONTROL_STATIONID, nStationId, NULL);
	}

	if ((dw&TREEITEM_CLASS_SCENE)!=0){
		OpenASceneElecMap(strItemName);

		RefreshDeviceStatusBySecne(strItemName);
	}

	if (dw&TREEITEM_CLASS_VOLT){
		if (!addSceneToVoltNode(hm)){
			TRACE0("add scene to 500kV node failed.\n");
		}
	}

	if ((dw&TREEITEM_CLASS_PROVINCE)){
		if (!addSceneToProvince(hm)){
			TRACE0("add province map failed.");
		}
	}

	*pResult = 0;
}

void CDlgTreeElecMap::RefreshDeviceStatusBySecne(CString strItemName)
{
	///////////////////将该场景下的设备实时状态传递给电子地图//////////////////////////
	//获取当前场景下设备的实时状态
	std::vector<PUSH_DATA_RESPONSE> vecAlarm = GetDeviceRealDataInfoBySceneName(strItemName);

	//将vector转换成数组
	PUSH_DATA_RESPONSE tPushDataResponse[ACS_DEVICE_REALDATA_COUNT];
	memset(&tPushDataResponse,0,sizeof(PUSH_DATA_RESPONSE)*ACS_DEVICE_REALDATA_COUNT);
	for(size_t i=0; i<vecAlarm.size(); i++){
		tPushDataResponse[i].nRvuId = vecAlarm.at(i).nRvuId;
		tPushDataResponse[i].nSmId = vecAlarm.at(i).nSmId;
		tPushDataResponse[i].fValue = vecAlarm.at(i).fValue;
		tPushDataResponse[i].nStatus = vecAlarm.at(i).nStatus;
		tPushDataResponse[i].nType = vecAlarm.at(i).nType;
	}

	//调用电子地图接口，将数据传过去
	ElecMap_RefreshYXDevState((char*)tPushDataResponse, vecAlarm.size());
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
static BOOL connectDatabase()
{
	if ((g_mySqlData = mysql_init((MYSQL*)0)) && mysql_real_connect(g_mySqlData, g_host, g_user, g_pwd, g_dbName, g_nport, NULL, 0))
	{
		if (mysql_select_db(g_mySqlData, g_dbName) < 0)
		{
			TRACE("已成功连接mysql, 但选择数据库出错!\n");
			mysql_close(g_mySqlData);
			g_mySqlData = NULL;
			return FALSE;
		}

		int nValue = 1;
		mysql_options(g_mySqlData,MYSQL_OPT_RECONNECT,(char *)&nValue);

		mysql_query(g_mySqlData,"SET NAMES GB2312");
		TRACE("已成功连接mysql!\n");

		return TRUE;
	}
	return FALSE;
}

static std::vector<CString> readStationSceneNames(const CString strStation)
{
	if (!g_mySqlData){	
		if (!connectDatabase()){
			TRACE0("open database failed.\n");
		}
	}

	std::vector<CString> vecSceneName;

	CString str;
	str.Format("SELECT station_scenename FROM ass_d5000_station_scene WHERE station_name='%s'",strStation);

	int re = mysql_query(g_mySqlData,str);
	if (re!=0){
		TRACE0("execute sql failed.\n");
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;

	res = mysql_store_result(g_mySqlData);
	while (row = mysql_fetch_row(res)){
		CString strRead;
		strRead.Format("%s",row[0]);
		vecSceneName.push_back(strRead);
	}

	mysql_free_result(res) ;

	return vecSceneName;
}

BOOL readSceneNameOfStation(std::vector<CString>& vecSceneName, std::vector<int>& vecSceneVoltClass, const CString strStation )
{
	if (!g_mySqlData){	
		if (!connectDatabase()){
			TRACE0("open database failed.\n");
		}
	}

	CString str;
	str.Format("SELECT station_scenename,station_voltclass FROM ass_d5000_station_scene WHERE station_name='%s'",strStation);

	int re = mysql_query(g_mySqlData,str);
	if (re!=0){
		TRACE0("execute sql failed.\n");
	}

	MYSQL_RES	*res;
	MYSQL_ROW	row;

	res = mysql_store_result(g_mySqlData);
	while (row = mysql_fetch_row(res)){
		CString strRead;
		strRead.Format("%s",row[0]);
		vecSceneName.push_back(strRead);

		int voltclass = atoi(row[1]);
		vecSceneVoltClass.push_back(voltclass);
	}

	mysql_free_result(res) ;

	return TRUE;
}

BOOL CDlgTreeElecMap::addSceneToStation(HTREEITEM& hItem, const CString strStation)
{
	//todo:read database and return station scene name
	std::vector<CString> vecSceneName = readStationSceneNames(strStation);

	if (m_ctrlTree.ItemHasChildren(hItem)){
		TRACE0("the station node already has be loaded.\n");
		return TRUE;
	}

	for(size_t t=0; t<vecSceneName.size(); t++){
		HTREEITEM hChildItem = m_ctrlTree.InsertItem(vecSceneName.at(t),2,2,hItem);
		m_ctrlTree.SetItemData(hChildItem,TREEITEM_CLASS_SCENE);
	}

	return TRUE;
}

BOOL CDlgTreeElecMap::addSceneToVoltNode(HTREEITEM& hItem)
{		
	std::vector<CString> vecSceneName;
	std::vector<int> vecVoltClass;
	int voltClass(0);
	CString str = m_ctrlTree.GetItemText(hItem);
	if (StrCmp(str,_T("500kV"))==0){
		voltClass=(int)500;
		if (!readSceneNameOfStation(vecSceneName,vecVoltClass,PROVINCE_NAME)){
			TRACE0("read scene name of city failed.\n");
			return FALSE;
		}
	}

	//visit the city node to find the scene node is or not loaded.
	if (m_ctrlTree.ItemHasChildren(hItem)){
		HTREEITEM hChildItem = m_ctrlTree.GetChildItem(hItem);
		while(hChildItem){
			CString strName = m_ctrlTree.GetItemText(hChildItem);
			for(size_t t=0; t<vecSceneName.size(); t++){
				if ((StrCmp(vecSceneName.at(t),strName) ==0) && (voltClass == vecVoltClass.at(t))){
					TRACE0("the station node already has children.");
					return TRUE;
				}
			}
			hChildItem = m_ctrlTree.GetNextSiblingItem(hChildItem);
		}
	}

	for(size_t t=0; t<vecSceneName.size(); t++){
		HTREEITEM hChildItem = m_ctrlTree.InsertItem(vecSceneName.at(t),2,2,hItem,TVI_FIRST);
		m_ctrlTree.SetItemData(hChildItem,TREEITEM_CLASS_SCENE);
	}

	return TRUE;
}

BOOL CDlgTreeElecMap::addSceneToProvince(HTREEITEM& hItem)
{	
	std::vector<CString> vecSceneName;
	CString strSceneName(_T("省变电站电子地图"));

	if (m_bIsAddProvinceScene ==TRUE){
		return TRUE;
	}

	HTREEITEM hChildItem = m_ctrlTree.InsertItem(strSceneName,2,2,hItem,TVI_FIRST);
	m_ctrlTree.SetItemData(hChildItem,TREEITEM_CLASS_SCENE);

	m_bIsAddProvinceScene =TRUE;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BOOL CDlgTreeElecMap::OnInitDialog()
{
	if (!CDialog::OnInitDialog()){
		return FALSE;
	}
	
	//initial: initial control///read database///initial tree
	LoadSkin();
	ReadDBSystemInfo();
	InitCameraTreelist();

	return TRUE;
}

void CDlgTreeElecMap::LoadSkin()
{
	m_imageList.Create(IDB_BITMAP_TREE,16,0,RGB(255,0,255));
	m_ctrlTree.SetImageList(&m_imageList, TVSIL_NORMAL);

	m_btnSearch.LoadBitmap(IDB_BITMAP_BTN_SEARCH_NODE_BK,0);
	m_btnSearch.SetIndex(0);
	m_btnSearch.Invalidate(TRUE);
}

void CDlgTreeElecMap::ReadDBSystemInfo()
{
	if (g_nStation_Num >0){
		return;
	}

	try
	{
		bool result = false;

		char sql_buf[1024] = {0};

		MYSQL_RES	* res ;
		MYSQL_ROW	row ;

		// 读取当前视频平台数量及信息，不读上级平台
		g_TotalSysNum = 0;
		sprintf_s(sql_buf, "SELECT system_id, system_name, cms_ip, cms_port, httpserver_ip, httpserver_port,"
			" system_status, up_system_id, local_domain FROM sm_d5000_plant_system where local_domain<>2 ORDER BY system_id");

		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);

			while ( row = mysql_fetch_row( res ) )
			{
				sprintf_s(g_SystemInfo[g_TotalSysNum].sysid, "%s", row[0]);
				sprintf_s(g_SystemInfo[g_TotalSysNum].sysname, "%s", row[1]);
				sprintf_s(g_SystemInfo[g_TotalSysNum].cms_ip, "%s", row[2]);
				g_SystemInfo[g_TotalSysNum].cms_port = atoi(row[3]);
				sprintf_s(g_SystemInfo[g_TotalSysNum].http_ip, "%s", row[4]);
				g_SystemInfo[g_TotalSysNum].http_port = atoi(row[5]);
				g_SystemInfo[g_TotalSysNum].sys_status = atoi(row[6]);
				sprintf_s(g_SystemInfo[g_TotalSysNum].upsysid, "%s", row[7]);
				g_SystemInfo[g_TotalSysNum].localdomain = atoi(row[8]);

				g_TotalSysNum ++;
			}
			mysql_free_result( res ) ;
		}

		// 读取变电站的节点信息
		g_nStationNode_Num = 0;
		sprintf_s(sql_buf, "SELECT node_id,parent_id,node_name,order_num,ywb_id,login_type,voltage_class FROM ob_d5000_node_relation ORDER by node_id ASC");
		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);

			while (row = mysql_fetch_row(res))
			{
				g_tStationNode_Info[g_nStationNode_Num].node_id = atoi(row[0]);
				g_tStationNode_Info[g_nStationNode_Num].parent_id = atoi(row[1]);
				sprintf_s(g_tStationNode_Info[g_nStationNode_Num].node_name, "%s", row[2]);
				g_tStationNode_Info[g_nStationNode_Num].order_num = atoi(row[3]);
				g_tStationNode_Info[g_nStationNode_Num].ywb_id = atoi(row[4]);
				g_tStationNode_Info[g_nStationNode_Num].login_type = atoi(row[5]);
				g_tStationNode_Info[g_nStationNode_Num].voltage_class = atoi(row[6]);

				g_nStationNode_Num ++;
				if (g_nStationNode_Num >= MAX_STATION_NODE_NUM)
				{
					MessageBox("系统最大支持的结点数为2000个，超过部分无法进行展示","热点视频监视");
					break;
				}
			}
			mysql_free_result(res) ;
		}

		// 读取变电站信息
		g_nStation_Num = 0;
		sprintf_s(sql_buf, "SELECT station_id,station_name_videoplant,station_code_videoplant,station_name_d5000,station_code_d5000,"
			" station_name_oms,station_code_oms,station_name_meteo_a,station_code_meteo_a,station_code_weather,station_code_typhoon,station_code_microclimate, station_video_status,voltage_class, node_id, node_yw_id,node_sd_yw_id,notes FROM ob_d5000_station WHERE voltage_class=500 ORDER BY order_num ASC");
		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);

			while (row = mysql_fetch_row(res))
			{
				if (g_nStation_Num >= MAX_STATION_NUM)
					break;

				g_tStation_Info[g_nStation_Num].station_id = atoi(row[0]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_name_videoplant, "%s", row[1]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_videoplant, "%s", row[2]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_name_d5000, "%s", row[3]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_d5000, "%s", row[4]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_name_oms, "%s", row[5]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_oms, "%s", row[6]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_name_meteo_a, "%s", row[7]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_meteo_a, "%s", row[8]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_weather,"%s",row[9]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_typhoon,"%s",row[10]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_microclimate,"%s",row[11]);
				g_tStation_Info[g_nStation_Num].station_video_status = atoi(row[12]);
				g_tStation_Info[g_nStation_Num].voltage_class = atoi(row[13]);
				g_tStation_Info[g_nStation_Num].node_id = atoi(row[14]);
				g_tStation_Info[g_nStation_Num].node_yw_id = atoi(row[15]);
				g_tStation_Info[g_nStation_Num].node_sd_yw_id = atoi(row[16]);
				sprintf_s(g_tStation_Info[g_nStation_Num].notes,"%s",row[17]);

				g_nStation_Num ++;
			}
			mysql_free_result(res) ;
		}

		sprintf_s(sql_buf, "SELECT station_id,station_name_videoplant,station_code_videoplant,station_name_d5000,station_code_d5000,"
			" station_name_oms,station_code_oms,station_name_meteo_a,station_code_meteo_a,station_code_weather,station_code_typhoon,station_code_microclimate, station_video_status,voltage_class, node_id, node_yw_id,node_sd_yw_id,notes FROM ob_d5000_station WHERE voltage_class=220 ORDER BY order_num ASC");
		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);

			while (row = mysql_fetch_row(res))
			{
				if (g_nStation_Num >= MAX_STATION_NUM)
					break;

				g_tStation_Info[g_nStation_Num].station_id = atoi(row[0]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_name_videoplant, "%s", row[1]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_videoplant, "%s", row[2]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_name_d5000, "%s", row[3]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_d5000, "%s", row[4]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_name_oms, "%s", row[5]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_oms, "%s", row[6]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_name_meteo_a, "%s", row[7]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_meteo_a, "%s", row[8]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_weather,"%s",row[9]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_typhoon,"%s",row[10]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_microclimate,"%s",row[11]);
				g_tStation_Info[g_nStation_Num].station_video_status = atoi(row[12]);
				g_tStation_Info[g_nStation_Num].voltage_class = atoi(row[13]);
				g_tStation_Info[g_nStation_Num].node_id = atoi(row[14]);
				g_tStation_Info[g_nStation_Num].node_yw_id = atoi(row[15]);
				g_tStation_Info[g_nStation_Num].node_sd_yw_id = atoi(row[16]);
				sprintf_s(g_tStation_Info[g_nStation_Num].notes,"%s",row[17]);

				g_nStation_Num ++;
			}
			mysql_free_result(res) ;
		}

		sprintf_s(sql_buf, "SELECT station_id,station_name_videoplant,station_code_videoplant,station_name_d5000,station_code_d5000,"
			" station_name_oms,station_code_oms,station_name_meteo_a,station_code_meteo_a,station_code_weather,station_code_typhoon,station_code_microclimate, station_video_status,voltage_class, node_id, node_yw_id,node_sd_yw_id,notes FROM ob_d5000_station WHERE voltage_class!=500 AND voltage_class!=220 ORDER BY order_num ASC");
		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);

			while (row = mysql_fetch_row(res))
			{
				if (g_nStation_Num >= MAX_STATION_NUM)
					break;

				g_tStation_Info[g_nStation_Num].station_id = atoi(row[0]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_name_videoplant, "%s", row[1]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_videoplant, "%s", row[2]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_name_d5000, "%s", row[3]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_d5000, "%s", row[4]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_name_oms, "%s", row[5]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_oms, "%s", row[6]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_name_meteo_a, "%s", row[7]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_meteo_a, "%s", row[8]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_weather,"%s",row[9]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_typhoon,"%s",row[10]);
				sprintf_s(g_tStation_Info[g_nStation_Num].station_code_microclimate,"%s",row[11]);
				g_tStation_Info[g_nStation_Num].station_video_status = atoi(row[12]);
				g_tStation_Info[g_nStation_Num].voltage_class = atoi(row[13]);
				g_tStation_Info[g_nStation_Num].node_id = atoi(row[14]);
				g_tStation_Info[g_nStation_Num].node_yw_id = atoi(row[15]);
				g_tStation_Info[g_nStation_Num].node_sd_yw_id = atoi(row[16]);
				sprintf_s(g_tStation_Info[g_nStation_Num].notes,"%s",row[17]);

				g_nStation_Num ++;

			}
			mysql_free_result(res) ;
		}

		if (g_nStation_Num >= MAX_STATION_NUM)
		{
			MessageBox("系统最大支持的站点数为4000个，超过部分无法进行展示","热点视频监视");
		}
	}
	catch(...)
	{

	}
}

void CDlgTreeElecMap::InitCameraTreelist()
{
	try
	{
		int i = 0, j = 0, k = 0;
		int ns = 0;

		HTREEITEM	hMainItem = NULL;
		HTREEITEM	hChildItem = NULL, hChildItem1 = NULL, hChildItem2 = NULL;
		HTREEITEM	hStationItem = NULL;
		HTREEITEM	hCameraItem = NULL;
		BOOL b500kVFlag = FALSE;//当为500kV时，不显示下面的市级结点，直接显示变电站。

		int nStationStatus = 0;//变电站状态
		char szStationShowName[256] = {0};
		memset(szStationShowName,0,sizeof(szStationShowName));

		// 添加根节点
		hMainItem = m_ctrlTree.InsertItem(_T("江苏省变电站电子地图"), 1, 1, TVI_ROOT, hMainItem);
		m_ctrlTree.SetItemData(hMainItem,TREEITEM_CLASS_PROVINCE);

		//添加变电站中间节点和站点节点
		for (i = 0; i < g_nStationNode_Num; i ++)
		{
			if (g_tStationNode_Info[i].parent_id == g_tStationNode_Info[i].node_id&&g_tStationNode_Info[i].login_type == 0)	//第一级中间节点--500kV,220kV,110kV等
			{
				if (strcmp(g_tStationNode_Info[i].node_name,"500kV") == 0)//不显示下面的市级结点，直接显示变电站
					b500kVFlag = TRUE;
				else
					b500kVFlag = FALSE;

				hChildItem = m_ctrlTree.InsertItem(g_tStationNode_Info[i].node_name, 7, 7, hMainItem);
				m_ctrlTree.SetItemData(hChildItem,TREEITEM_CLASS_VOLT);

				for (ns = 0; ns < g_nStation_Num; ns ++)
				{
					if (g_tStation_Info[ns].node_id == g_tStationNode_Info[i].node_id)
					{
						strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

						nStationStatus = CheckStationCurrentStatus2(g_tStation_Info[ns].station_id);
						if (nStationStatus != 0)
							hStationItem = m_ctrlTree.InsertItem(szStationShowName, 8, 8, hChildItem);
						else
							hStationItem = m_ctrlTree.InsertItem(szStationShowName, 10, 10, hChildItem);
					}
				}

				for (j = 0; j < g_nStationNode_Num; j ++)//第二级中间节点--南京市、苏州市
				{
					if (g_tStationNode_Info[j].parent_id == g_tStationNode_Info[i].node_id && g_tStationNode_Info[j].parent_id != g_tStationNode_Info[j].node_id)	// 第二季中间节点
					{
						if (b500kVFlag == FALSE){
							hChildItem1 = m_ctrlTree.InsertItem(g_tStationNode_Info[j].node_name, 7, 7, hChildItem);
							m_ctrlTree.SetItemData(hChildItem1,TREEITEM_CLASS_CITY);
						}
						else
							hChildItem1 = hChildItem;

						for (ns = 0; ns < g_nStation_Num; ns ++)
						{
							if (g_tStation_Info[ns].node_id == g_tStationNode_Info[j].node_id)
							{
								strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

								nStationStatus = CheckStationCurrentStatus2(g_tStation_Info[ns].station_id);
								if (nStationStatus != 0){
									hStationItem = m_ctrlTree.InsertItem(szStationShowName, 8, 8, hChildItem1);
									m_ctrlTree.SetItemData(hStationItem,TREEITEM_CLASS_STATE);
								}
								else{
									hStationItem = m_ctrlTree.InsertItem(szStationShowName, 10, 10, hChildItem1);
									m_ctrlTree.SetItemData(hStationItem,TREEITEM_CLASS_STATE);
								}
							}
						}

						for (k = 0; k < g_nStationNode_Num; k++)//第三级中间节点
						{
							if (g_tStationNode_Info[k].parent_id == g_tStationNode_Info[j].node_id && g_tStationNode_Info[k].parent_id != g_tStationNode_Info[k].node_id)	// 第三季中间节点
							{
								hChildItem2 = m_ctrlTree.InsertItem(g_tStationNode_Info[k].node_name, 7, 7, hChildItem1);

								for (ns = 0; ns < g_nStation_Num; ns ++)
								{
									if (g_tStation_Info[ns].node_id == g_tStationNode_Info[k].node_id)
									{
										strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

										nStationStatus = CheckStationCurrentStatus2(g_tStation_Info[ns].station_id);
										if (nStationStatus != 0)
											hStationItem = m_ctrlTree.InsertItem(szStationShowName, 8, 8, hChildItem2);
										else
											hStationItem = m_ctrlTree.InsertItem(szStationShowName, 10, 10, hChildItem2);
									}
								}
							}
						}
					}
				}
			}
		}

		hMainItem = m_ctrlTree.GetRootItem();
		if (hMainItem != NULL)
		{
			hChildItem = m_ctrlTree.GetChildItem(hMainItem);
			if (hChildItem != NULL)
			{
				m_ctrlTree.SortChildren(hChildItem);//排序
				hChildItem1 = m_ctrlTree.GetChildItem(hChildItem);
				if (hChildItem1 != NULL)
				{
					//m_ctrlTree.EnsureVisible(hChildItem1);
				}
			}
		}

		hChildItem = m_ctrlTree.GetNextSiblingItem(hChildItem);
		while(hChildItem != NULL)
		{
			m_ctrlTree.SortChildren(hChildItem);//排序
			hChildItem1 = m_ctrlTree.GetChildItem(hChildItem);
			while(hChildItem1 != NULL)
			{
				m_ctrlTree.SortChildren(hChildItem1);
				hChildItem1 = m_ctrlTree.GetNextSiblingItem(hChildItem1);
			}
			hChildItem = m_ctrlTree.GetNextSiblingItem(hChildItem);
		}
	}
	catch(...)
	{

	}
}

int CDlgTreeElecMap::CheckStationCurrentStatus2(int nStationId)
{
	int nStationStatus = 1;

	try
	{
		for (int i = 0;i < g_nStation_Num;i++)
		{
			if (g_tStation_Info[i].station_id == nStationId)
			{
				if (g_tStation_Info[i].station_video_status == 0)
					nStationStatus = 0;
			}
		}
		return nStationStatus;
	}
	catch(...)
	{

	}
	return nStationStatus;
}

// CDlgTreeElecMap 消息处理程序

void CDlgTreeElecMap::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect winrc,list_rect;
	GetClientRect(&winrc);

	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_TREE_ELECMAP);
	if (pWnd != NULL)
	{
		list_rect.top = winrc.top;
		list_rect.bottom = winrc.bottom;
		list_rect.left = winrc.left;
		list_rect.right = winrc.right;
		pWnd->MoveWindow(list_rect);
	}
}
