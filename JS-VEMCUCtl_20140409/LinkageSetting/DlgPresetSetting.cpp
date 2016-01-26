// DlgPresetSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "LinkageSetting.h"
#include "DlgPresetSetting.h"
#include "LinkageSettingAPI.h"
#include "PtuDlg.h"
#include "DBExcute.h"
#include "ViewChannel.h"


#define PRESETNUMMIN (5)
#define PRESETNUMMAX (10)

#define SETPRESET 1
#define UNSETPRESET 0
// CDlgPresetSetting 对话框

IMPLEMENT_DYNAMIC(CDlgPresetSetting, CDialog)

CDlgPresetSetting::CDlgPresetSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPresetSetting::IDD, pParent)
{
	m_nCurrentID = 0;
	m_pImageWindow = NULL;
	m_pStationNode = NULL;
	m_nDvrType = 0;
	m_nPresetId = 0;
	m_nCameraPresetId = 0;
	memset(m_szCameraCode,0,64);
	memset(m_szPresetName,0,64);
	m_pBackBmp = NULL;
	m_pViewChannel = NULL;
}

CDlgPresetSetting::~CDlgPresetSetting()
{
	if (m_pImageWindow != NULL)
	{
		delete m_pImageWindow;
		m_pImageWindow = NULL;
	}

	FreeNodeRes(m_treeLinkageList.GetRootItem(),0);
	FreeNodeRes(m_treePresetList.GetRootItem(),1);
}

void CDlgPresetSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_LINKAGE_LIST, m_treeLinkageList);
	DDX_Control(pDX, IDC_TREE_PRESETTING_LIST, m_treePresetList);
	DDX_Control(pDX, IDC_COMBO_PRESET_NUM, m_combo_presetnum);
	DDX_Control(pDX, IDC_LIST_LINKAGE_RELATION, m_lstLinkageRelation);
}


BEGIN_MESSAGE_MAP(CDlgPresetSetting, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_TREE_PRESETTING_LIST, &CDlgPresetSetting::OnNMClickTreePresettingList)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_PRESETTING_LIST, &CDlgPresetSetting::OnNMDblclkTreePresettingList)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_LEFT, &CDlgPresetSetting::OnBnClickedButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_LOCK, &CDlgPresetSetting::OnBnClickedButtonLock)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, &CDlgPresetSetting::OnBnClickedButtonRight)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CDlgPresetSetting::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_IN, &CDlgPresetSetting::OnBnClickedButtonZoomIn)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_OUT, &CDlgPresetSetting::OnBnClickedButtonZoomOut)
	ON_BN_CLICKED(IDC_BUTTON_TO_PRESET, &CDlgPresetSetting::OnBnClickedButtonToPreset)
	ON_BN_CLICKED(IDC_BUTTON_SET_PRESET, &CDlgPresetSetting::OnBnClickedButtonSetPreset)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_PRESET, &CDlgPresetSetting::OnBnClickedButtonDeletePreset)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_LINKAGE_LIST, &CDlgPresetSetting::OnNMDblclkTreeLinkageList)
	ON_MESSAGE(OM_MESSAGE_CAMERA_CONTROL,&CDlgPresetSetting::OnStartYTControl)
	ON_MESSAGE(WM_CLOSE_VIDEO,&CDlgPresetSetting::OnCloseVideoHandle)
	ON_NOTIFY(NM_CLICK, IDC_TREE_LINKAGE_LIST, &CDlgPresetSetting::OnNMClickTreeLinkageList)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_CURRENT_CAMERA, &CDlgPresetSetting::OnStnClickedStaticCurrentCamera)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET_NUM, &CDlgPresetSetting::OnCbnSelchangeComboPresetNum)
	ON_COMMAND(ID_CLOSE_VIDEO, &CDlgPresetSetting::OnCloseVideo)
END_MESSAGE_MAP()


// CDlgPresetSetting 消息处理程序

LRESULT CDlgPresetSetting::OnStartYTControl(WPARAM wParam,LPARAM lParam)
{
	int ytcommand = 0;

	TRACE("-----iMove=%d,iCommandID=%d------\n\n",lParam,wParam);

	if (m_pViewChannel->m_video_opened == false)
	{
		return 0;
	}

	m_pViewChannel->CameraControl(wParam, lParam);

	return 1;
}

BOOL CDlgPresetSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化各个控件数据，外观等
	InitControl();

	m_pBackBmp = new CBitmap;
	m_pBackBmp->LoadBitmap(IDB_BITMAP_BACK);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgPresetSetting::InitControl()
{
	m_pViewChannel = new CViewChannel();
	CRect Rect(400, 400, 400, 400);
	m_pViewChannel->Create(NULL, NULL, WS_TABSTOP|WS_CHILD|WS_VISIBLE, Rect, this, 1000);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_TREE);
	m_ilTreeIcons.Create(16,16,ILC_MASK|ILC_COLOR24,0,1);
	m_ilTreeIcons.Add(&bmp,RGB(255,0,255));

	m_treeLinkageList.SetImageList(&m_ilTreeIcons,TVSIL_NORMAL);
	m_treePresetList.SetImageList(&m_ilTreeIcons,TVSIL_NORMAL);

	CRect rect(200,200,200,200);
	m_pImageWindow = new CImageView;
	m_pImageWindow->CreateWnd(rect,this);

	//初始化预置位编号
	CString strText = "";
	for (int i=PRESETNUMMIN; i<PRESETNUMMAX; i++)
	{
		strText.Format("%d",i+1);
		m_combo_presetnum.AddString(strText);
	}
	m_combo_presetnum.SetCurSel(0);

	//联动关系列表
	DWORD dwStyle;
	dwStyle = m_lstLinkageRelation.GetStyle();
	dwStyle = LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT ;

	m_lstLinkageRelation.SetExtendedStyle(dwStyle);
	m_lstLinkageRelation.SetBkColor(RGB(239,246,253));
	m_lstLinkageRelation.SetTextBkColor(RGB(0xfe,0xFF,0xc6));

	m_lstLinkageRelation.DeleteAllItems();
	while(m_lstLinkageRelation.DeleteColumn(0));

	CRect winrc;
	m_lstLinkageRelation.GetClientRect(winrc);

	int nWidth = 400;

	m_lstLinkageRelation.InsertColumn(0,"ID");
	m_lstLinkageRelation.SetColumnWidth(0,nWidth*0.2);
	m_lstLinkageRelation.InsertColumn(1,"摄像机名称");
	m_lstLinkageRelation.SetColumnWidth(1,nWidth*0.2);
	m_lstLinkageRelation.InsertColumn(2,"设备名称");
	m_lstLinkageRelation.SetColumnWidth(2,nWidth*0.2);
	m_lstLinkageRelation.InsertColumn(3,"预置位编号");
	m_lstLinkageRelation.SetColumnWidth(3,nWidth*0.2);
	m_lstLinkageRelation.InsertColumn(4,"预置位名称");
	m_lstLinkageRelation.SetColumnWidth(4,nWidth*0.2);
}

int CDlgPresetSetting::CreatePresetTable(int nStationId)
{
	char sql_buf[1024]={0x0};

	MYSQL_RES	* res ;
	MYSQL_ROW	row ;
	int rnum = 0;

	sprintf_s(sql_buf, "SHOW TABLES LIKE 'ass_camerapreset'");
	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		rnum = mysql_num_rows(res);
		mysql_free_result( res ) ;
	}

	if (rnum == 0)
	{
		sprintf_s(sql_buf, "CREATE TABLE `ass_camerapreset` ( "
			"`id` int(16) NOT NULL auto_increment, "
			"`camera_code` char(20) default NULL, "
			"`dev_name` char(64) default NULL, "
			"`camera_preset_id` int(16) default NULL, "
			"`preset_name` char(64) default NULL, "
			"PRIMARY KEY  (`id`) "
			") ENGINE=InnoDB DEFAULT CHARSET=utf8;");

		TRACE("%s", sql_buf);

		if (!mysql_query(g_LinkageSettingMysql, sql_buf))
		{
			res = mysql_store_result(g_LinkageSettingMysql);
			mysql_free_result(res) ;
		}
		else
		{
			AfxMessageBox("创建新的预置位列表失败");
			return -1;
		}
	}
	return 0;
}

void CDlgPresetSetting::InitLinkageRelationList(StationNode* pNodeInfo)
{
	m_lstLinkageRelation.DeleteAllItems();
	//按照变电站id创建预置位表
	int ret = CreatePresetTable(pNodeInfo->station_id);
	if (ret)
	{
		return;
	}

	//从数据库中获取数据
	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row; 
	int nIndex = 0;
	sprintf_s(sql_buf,1024,"select a.id,b.name as camera_name,c.name,a.camera_preset_id,a.preset_name,a.dev_id from "
		" ass_camerapreset a left join video_camera b on a.camera_code=b.camera_id left join ass_rvu_sm " 
		" c on a.dev_id=c.id where a.dev_id in (select d.id from ass_rvu_sm as d where d.rvu_id in "
		" (select e.rvu_id from ass_rvu as e where e.station_id=%d)) order by a.id", pNodeInfo->station_id);
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			nIndex = m_lstLinkageRelation.InsertItem(0,row[0]);
			m_lstLinkageRelation.SetItemText(nIndex,1,row[1]);
			m_lstLinkageRelation.SetItemText(nIndex,2,row[2]);
			m_lstLinkageRelation.SetItemText(nIndex,3,row[3]);
			m_lstLinkageRelation.SetItemText(nIndex,4,row[4]);
		}
		mysql_free_result(res);
	}

	//添加到列表中
}

void CDlgPresetSetting::SetNodeInfo(HTREEITEM hItem,int node_id,int parent_id,int station_id,int dvr_id,int channel_id,char* node_name,char* node_code,int node_type,int node_level)
{
	NodeInfo* pNodeInfo = NULL;
	pNodeInfo = new NodeInfo;
	if (pNodeInfo != NULL)
	{
		pNodeInfo->node_id = node_id;
		pNodeInfo->parent_id = parent_id;
		sprintf_s(pNodeInfo->node_name,"%s",node_name);
		sprintf_s(pNodeInfo->node_code,"%s",node_code);
		pNodeInfo->node_type = node_type;
		pNodeInfo->node_level = node_level;
		pNodeInfo->dvr_id = dvr_id;
		pNodeInfo->station_id = station_id;
		pNodeInfo->channel_id = channel_id;

		if (node_level == 0)
		{
			m_treeLinkageList.SetItemData(hItem,(DWORD)pNodeInfo);
		}
		else if (node_level == 1)
		{
			m_treePresetList.SetItemData(hItem,(DWORD)pNodeInfo);
		}
	}
}

void CDlgPresetSetting::SetStationNode(StationNode* pStationNode)
{
	m_pStationNode = pStationNode;
}

void CDlgPresetSetting::InitLinkageList(StationNode* pNodeInfo)
{
	//清空列表
	FreeAllTreeItem(&m_treeLinkageList,NULL);
	//添加变电站根结点
	HTREEITEM hMainItem = NULL;
	HTREEITEM hItemSubSystem = NULL,hItemDevice = NULL;
	hMainItem = m_treeLinkageList.InsertItem(pNodeInfo->station_name, 8, 8, TVI_ROOT, NULL);
	SetTreeNodeInfo(&m_treeLinkageList,hMainItem,pNodeInfo->station_id,STATION_NODE,pNodeInfo->station_name,0,NULL);

	//获取变电站下的所有RVU
	int nRvuId[100]={0};//一个变电站下最大不超过100个RVU
	int nRvuNum = 0;
	for (int m=0;m<g_nAssRVUNum;m++)
	{
		if (g_tAssRVU[m].station_id == pNodeInfo->station_id)
		{
			nRvuId[nRvuNum] = g_tAssRVU[m].rvu_id;
			nRvuNum++;
		}
	}

	
	for (int j = 0;j<g_nAssRVUDeviceNum;j++)
	{
		//添加设备（先找到变电站下面的rvu,然后再找rvu下面的设备）
		for(int k=0;k<nRvuNum;k++)
		{
			if (g_tAssRVUDevice[j].rvu_id == nRvuId[k])
			{
				hItemDevice = m_treeLinkageList.InsertItem(g_tAssRVUDevice[j].node_name,10,10,hMainItem);
				SetTreeNodeInfo(&m_treeLinkageList,hItemDevice,g_tAssRVUDevice[j].id,RVU_DEVICE_NODE,g_tAssRVUDevice[j].node_name,pNodeInfo->station_id,NULL);
			}	
		}
	}


	ExpandAllItem(&m_treeLinkageList,NULL);
}

void CDlgPresetSetting::InitPresettingList(StationNode* pNodeInfo)
{
	//清空列表
	FreeAllTreeItem(&m_treePresetList,NULL);
	//添加变电站根结点
	HTREEITEM hMainItem = NULL;
	HTREEITEM hCameraItem = NULL;
	hMainItem = m_treePresetList.InsertItem(pNodeInfo->station_name, 8, 8, TVI_ROOT, NULL);
	SetTreeNodeInfo(&m_treePresetList,hMainItem,pNodeInfo->station_id,STATION_NODE,pNodeInfo->station_name,0,NULL);
	//寻找该变电站下的摄像头
	char sql_buf[1024]={0x0};
	MYSQL_RES* res;
	MYSQL_ROW row; 
	sprintf_s(sql_buf,1024,"SELECT camera_id,camera_code,camera_name FROM ob_d5000_camera_station_%d",pNodeInfo->station_id);
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			hCameraItem = m_treePresetList.InsertItem(row[2],5,5,hMainItem);
			SetTreeNodeInfo(&m_treePresetList,hCameraItem,atoi(row[0]),CAMERA_NODE,row[2],pNodeInfo->station_id,row[1]);
		}
		mysql_free_result(res);
	}

	ExpandAllItem(&m_treePresetList,NULL);
}

void CDlgPresetSetting::SetTreeNodeInfo(CTreeCtrl* pTreeList,HTREEITEM hItem,int nNodeId,int nNodeType,char* szNodeName,int nParentId,char* szNodeCode)
{
	NodeInfo* pNodeInfo = NULL;
	pNodeInfo = new NodeInfo;
	memset(pNodeInfo,0,sizeof(NodeInfo));

	pNodeInfo->node_id = nNodeId;
	pNodeInfo->node_type = nNodeType;
	sprintf_s(pNodeInfo->node_name,"%s",szNodeName);
	pNodeInfo->parent_id = nParentId;
	sprintf_s(pNodeInfo->node_code,"%s",szNodeCode);

	pTreeList->SetItemData(hItem,(DWORD)pNodeInfo);
}

void CDlgPresetSetting::ExpandAllItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem)
{
	if (hTreeItem == NULL)
	{
		hTreeItem = pTreeList->GetRootItem();
	}

	if (!pTreeList->ItemHasChildren(hTreeItem))
	{
		return;
	}

	HTREEITEM hNextItem = pTreeList->GetChildItem(hTreeItem);
	while(hNextItem!=NULL)
	{
		ExpandAllItem(pTreeList,hNextItem);
		hNextItem = pTreeList->GetNextItem(hNextItem,TVGN_NEXT);
	}

	pTreeList->Expand(hTreeItem,TVE_EXPAND);
}

void CDlgPresetSetting::FreeAllTreeItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem)
{
	NodeInfo* pNodeInfo = NULL;
	if (hTreeItem == NULL)
	{
		hTreeItem = pTreeList->GetRootItem();
	}

	HTREEITEM hNextItem = pTreeList->GetChildItem(hTreeItem);
	while(hNextItem!=NULL)
	{
		FreeAllTreeItem(pTreeList,hNextItem);
		hNextItem = pTreeList->GetNextItem(hNextItem,TVGN_NEXT);
	}

	if (hTreeItem!=NULL)
	{
		pNodeInfo = (NodeInfo*)pTreeList->GetItemData(hTreeItem);
		if (pNodeInfo != NULL)
		{
			delete pNodeInfo;
			pNodeInfo = NULL;
			pTreeList->SetItemData(hTreeItem,NULL);
		}
		pTreeList->DeleteItem(hTreeItem);
	}

}

void CDlgPresetSetting::FreeNodeRes(HTREEITEM hItem,int nMethod)
{
	if (nMethod == 0)
	{
		NodeInfo* pNodeInfo = NULL;
		//释放子节点
		if (m_treeLinkageList.ItemHasChildren(hItem))
		{
			if (hItem != NULL)
			{
				pNodeInfo = (NodeInfo*)m_treeLinkageList.GetItemData(hItem);
				if (pNodeInfo != NULL)
				{
					delete pNodeInfo;
					pNodeInfo = NULL;
					m_treeLinkageList.SetItemData(hItem,NULL);
				}
			}

			HTREEITEM  hChild = m_treeLinkageList.GetChildItem(hItem);
			if (hChild != NULL)
			{
				pNodeInfo = (NodeInfo*)m_treeLinkageList.GetItemData(hChild);
				if (pNodeInfo != NULL)
				{
					delete pNodeInfo;
					pNodeInfo = NULL;
					m_treeLinkageList.SetItemData(hChild,NULL);
				}
			}

			while (hChild != NULL)
			{
				if (m_treeLinkageList.ItemHasChildren(hChild))
				{
					FreeNodeRes(hChild,nMethod);
				}

				hChild = m_treeLinkageList.GetNextSiblingItem(hChild);

				if (hChild != NULL)
				{
					pNodeInfo = (NodeInfo*)m_treeLinkageList.GetItemData(hChild);
					if (pNodeInfo != NULL)
					{
						delete pNodeInfo;
						pNodeInfo = NULL;
						m_treeLinkageList.SetItemData(hChild,NULL);
					}
				}
			}
		}
	}
	else if (nMethod == 1)
	{
		NodeInfo* pNodeInfo = NULL;
		//释放子节点
		if (m_treePresetList.ItemHasChildren(hItem))
		{
			if (hItem != NULL)
			{
				pNodeInfo = (NodeInfo*)m_treePresetList.GetItemData(hItem);
				if (pNodeInfo != NULL)
				{
					delete pNodeInfo;
					pNodeInfo = NULL;
					m_treePresetList.SetItemData(hItem,NULL);
				}
			}

			HTREEITEM  hChild = m_treePresetList.GetChildItem(hItem);
			if (hChild != NULL)
			{
				pNodeInfo = (NodeInfo*)m_treePresetList.GetItemData(hChild);
				if (pNodeInfo != NULL)
				{
					delete pNodeInfo;
					pNodeInfo = NULL;
					m_treePresetList.SetItemData(hChild,NULL);
				}
			}

			while (hChild != NULL)
			{
				if (m_treePresetList.ItemHasChildren(hChild))
				{
					FreeNodeRes(hChild,nMethod);
				}

				hChild = m_treePresetList.GetNextSiblingItem(hChild);

				if (hChild != NULL)
				{
					pNodeInfo = (NodeInfo*)m_treePresetList.GetItemData(hChild);
					if (pNodeInfo != NULL)
					{
						delete pNodeInfo;
						pNodeInfo = NULL;
						m_treePresetList.SetItemData(hChild,NULL);
					}
				}
			}
		}
	}
}



void CDlgPresetSetting::OnNMClickTreePresettingList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

int CDlgPresetSetting::GetPresetInfoByCameraCode(HTREEITEM hItem,int nStationId,char* szCameraCode,int nDvrId)
{
	if (m_treePresetList.ItemHasChildren(hItem))
		return -1;

	char sql_buf[1024] = {0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;
	int nRet = 0;
	HTREEITEM hInsertItem;
	CString strText = "";

	sprintf_s(sql_buf,1024,"select camera_preset_id,preset_name,id,dev_id from ass_camerapreset where camera_code='%s'",szCameraCode);
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while ( row = mysql_fetch_row( res ) )
		{
			strText.Format("%s-%s",row[0],row[1]);
			hInsertItem = m_treePresetList.InsertItem(strText, 12, 12, hItem);
			SetTreeNodeInfo(&m_treePresetList,hInsertItem,atoi(row[0]),PRESET_NODE,row[1],nDvrId,szCameraCode);
		}
		mysql_free_result(res);
	}
	else
	{
		return -1;
	}
	return nRet;
}

int CDlgPresetSetting::OpenCamera(char* szCameraCode,unsigned short usDvrId)
{
	char dvr_ip[32] = {0x0};
	char login_name[64] = {0x0};
	char login_password[32] = {0x0};
	int dvr_port = 0;
	int channel = 0;
	int dvr_type  = 0;
	char szDvrType[16] = {0x0};
	char szCameraName[64] = {0};
	//根据camera_code获取dvr信息
	char sql_buf[1024] = {0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,1024,"select b.dvr_ip,b.tcp_port,b.dvr_type,b.adminpasswd,b.adminuser,a.channel,a.name from video_camera as a "
		" left join video_dvr as b on a.dvr_id=b.dvr_id where a.camera_id='%s'",szCameraCode);
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		row = mysql_fetch_row(res);
		if (row != NULL)
		{
			sprintf_s(dvr_ip,"%s",row[0]);
			dvr_port = atoi(row[1]);
			sprintf_s(szDvrType,"%s",row[2]);
			sprintf_s(login_name,"%s",row[4]);
			sprintf_s(login_password,"%s",row[3]);
			channel = atoi(row[5]);
			sprintf_s(szCameraName,"%s",row[6]);
		}
		else
		{
			MessageBox("查找DVR信息失败！","辅助系统");
			mysql_free_result(res);
			return -1;
		}
		mysql_free_result(res);
	}
	else
	{
		MessageBox("查找DVR信息失败！","辅助系统");
		return -1;
	}

	if (stricmp(szDvrType,"HIK")==0)
	{
		dvr_type = DEVTYPE_HK;
	}
	else if (stricmp(szDvrType,"DH")==0)
	{
		dvr_type = DEVTYPE_DH;
	}
	else if (stricmp(szDvrType,"DL")==0)
	{
		dvr_type = DEVTYPE_DL;
	}
	else if (stricmp(szDvrType,"MOBO")==0)
	{
		dvr_type = DEVTYPE_MOBOTIX;
	}
	else
	{
		MessageBox("未知的DVR类型","辅助系统");
		return -1;
	}

	CViewChannel::T_CAMERA_INFO tCameraInfo;
	tCameraInfo.nDvrType = dvr_type;
	sprintf_s(tCameraInfo.szDvrIP,"%s",dvr_ip);
	tCameraInfo.nDvrPort = dvr_port;
	tCameraInfo.nChannel = channel;
	sprintf_s(tCameraInfo.szCameraName,"%s",szCameraName);
	sprintf_s(tCameraInfo.szLoginName,"%s",login_name);
	sprintf_s(tCameraInfo.szLoginPassword,"%s",login_password);
	m_pViewChannel->SetCameraInfo(tCameraInfo);
	int nRes = m_pViewChannel->OpenCamera();
	if (nRes != 0)
	{
		AfxMessageBox("打开视频失败！");
		return -1;
	}

	m_camera_info.channel = channel;
	sprintf_s(m_camera_info.ip_addr,"%s",dvr_ip);
	sprintf_s(m_camera_info.login_name,"%s",login_name);
	sprintf_s(m_camera_info.login_password,"%s",login_password);
	m_camera_info.dvr_type = dvr_type;
	m_camera_info.ip_port = dvr_port;
	m_camera_info.dvr_id = usDvrId;
	m_nDvrType = dvr_type;

	m_bVideoOpen = true;

	return 0;
}

//双击预置位列表树
void CDlgPresetSetting::OnNMDblclkTreePresettingList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	HTREEITEM hItem = m_treePresetList.GetSelectedItem();
	if (hItem == NULL)
	{
		return;
	}

	NodeInfo* pNodeInfo = (NodeInfo*)m_treePresetList.GetItemData(hItem);
	if (pNodeInfo == NULL)
	{
		return;
	}

	GetDlgItem(IDC_EDIT_CURRENT_CAMERA)->SetWindowText("");
	if (pNodeInfo->node_type == CAMERA_NODE)
	{
		CloseCamera(m_nCurrentID);

		//设置各个成员变量
		sprintf_s(m_szCameraCode,64,pNodeInfo->node_code);
		
		//打开摄像头
		if (OpenCamera(m_szCameraCode,pNodeInfo->parent_id)==-1)
		{
			MessageBox("转到预置位失败！","辅助系统");
			return;
		}

		GetDlgItem(IDC_EDIT_CURRENT_CAMERA)->SetWindowText(pNodeInfo->node_name);
		m_combo_presetnum.SetCurSel(0);
		
		//设置预置位名称
		SetPresetEditText();

		//加载该摄像头下已经设置的预置位
		if (GetPresetInfoByCameraCode(hItem,m_pStationNode->station_id,m_szCameraCode,pNodeInfo->parent_id))
			return;
	}

	if (pNodeInfo->node_type == PRESET_NODE)
	{
		//关闭摄像头
		CloseCamera(m_nCurrentID);

		sprintf_s(m_szCameraCode,64,pNodeInfo->node_code);

		//打开摄像头
		if (OpenCamera(m_szCameraCode,pNodeInfo->parent_id)==-1)
		{
			MessageBox("转到预置位失败！","辅助系统");
			return;
		}

		m_nCameraPresetId = pNodeInfo->node_id;

		ToPreset(m_nCameraPresetId);	
	}
}

void CDlgPresetSetting::SetPresetEditText()
{
	CString strCameraName;
	CString strDevName;
	CString strPresetNum;

	GetDlgItem(IDC_EDIT_CURRENT_CAMERA)->GetWindowText(strCameraName);
	GetDlgItem(IDC_EDIT_CURRENT_DEVICE)->GetWindowText(strDevName);
	m_combo_presetnum.GetLBText(m_combo_presetnum.GetCurSel(), strPresetNum);

	CString strPresetName;
	strPresetName.Format("%s[%s]-%s", strCameraName, strPresetNum, strDevName);
	GetDlgItem(IDC_EDIT_PRESET_NAME)->SetWindowText(strPresetName);
}

void CDlgPresetSetting::ToPreset(int nPresetId)
{
	m_pViewChannel->ToPreset(nPresetId);
}

LRESULT CDlgPresetSetting::OnCloseVideoHandle(WPARAM w, LPARAM l)
{
	CloseCamera(m_nCurrentID);
	return 0;
}

void CDlgPresetSetting::CloseCamera(int nId)
{
	m_pViewChannel->CloseCamera();
}

void CDlgPresetSetting::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd* pWnd = NULL;
	CRect winrc,button_rect,list_rect,static_rect,tree_rect;

	GetClientRect(&winrc);

	int nPresetTreeWidth = 0.15 * cx;
	int nPresetTreeHeight = cy - 30;
	int nLinkageTreeWidth = 0.15 * cx;
	int nLinkageTreeHeight = cy - 30;

	int nStaticPresetTreeWidth = nPresetTreeWidth / 2;
	int nStaticPresetTreeHeight = 30;

	int nVideoWidth = 0.35 * cx;
	int nVideoHeight = 0.5 * cy;

	int nButtonWidth = 0.35 * cx / 3;
	int nButtonHeight = 30;//0.6 * cy / 8;
	int nButtonExWidth = 0.35 * cx / 2;

	int nLinkageListWidth = 0.35 * cx;
	int nLinkageListHeight = cy - 30;

	int nStaticLinkageTreeWidth = nLinkageTreeWidth / 2;
	int nStaticLinkageTreeHeight = 30;

	int nStaticLinkageRelationListWidth = nLinkageListWidth / 2;
	int nStaticLinkageRelationListHeight = 30;

	int nStaticLeftStart = (nVideoWidth - nButtonWidth * 2)/2 + nPresetTreeWidth;//当前选中摄像头的起始位置


	//预置位树文本
	pWnd = GetDlgItem(IDC_STATIC_PRESET_LIST);
	if (pWnd != NULL)
	{
		static_rect.top = winrc.top;
		static_rect.bottom = static_rect.top + nStaticPresetTreeHeight;
		static_rect.left = winrc.left;
		static_rect.right = static_rect.left + nStaticPresetTreeWidth;
		pWnd->MoveWindow(static_rect);
	}

	//预置位树
	pWnd = GetDlgItem(IDC_TREE_PRESETTING_LIST);
	if (pWnd != NULL)
	{
		tree_rect.top = static_rect.bottom;
		tree_rect.bottom = tree_rect.top + nPresetTreeHeight;
		tree_rect.left = winrc.left;
		tree_rect.right = tree_rect.left + nPresetTreeWidth;
		pWnd->MoveWindow(tree_rect);
	}

	//摄像头窗口
	if (m_pViewChannel != NULL)
	{
		button_rect.top = winrc.top + nStaticPresetTreeHeight;
		button_rect.bottom = button_rect.top + nVideoHeight;
		button_rect.left = tree_rect.right;
		button_rect.right = button_rect.left + nVideoWidth;
		m_pViewChannel->MoveWindow(button_rect);
	}

	//上
	pWnd = GetDlgItem(IDC_BUTTON_UP);
	if (pWnd != NULL)
	{
		button_rect.top = winrc.top + nVideoHeight + nStaticPresetTreeHeight;//窗体下面20
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = winrc.left + nPresetTreeWidth + nButtonWidth;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}
	
	//左
	pWnd = GetDlgItem(IDC_BUTTON_LEFT);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.bottom;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.left - nButtonWidth;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//锁
	pWnd = GetDlgItem(IDC_BUTTON_LOCK);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//右
	pWnd = GetDlgItem(IDC_BUTTON_RIGHT);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//拉近
	pWnd = GetDlgItem(IDC_BUTTON_ZOOM_IN);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.bottom;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = winrc.left + nPresetTreeWidth;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//下
	pWnd = GetDlgItem(IDC_BUTTON_DOWN);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//拉远
	pWnd = GetDlgItem(IDC_BUTTON_ZOOM_OUT);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//当前选中的摄像头static
	pWnd = GetDlgItem(IDC_STATIC_CURRENT_CAMERA);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.bottom;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = nStaticLeftStart;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//当前选中的摄像头edit
	pWnd = GetDlgItem(IDC_EDIT_CURRENT_CAMERA);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//当前选中的设备static
	pWnd = GetDlgItem(IDC_STATIC_CURRENT_DEVICE);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.bottom;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = nStaticLeftStart;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//当前选中的设备edit
	pWnd = GetDlgItem(IDC_EDIT_CURRENT_DEVICE);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//预置位编号文本
	pWnd = GetDlgItem(IDC_STATIC_PRESET_NUM);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.bottom;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = nStaticLeftStart;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}
	//预置位编号
	pWnd = GetDlgItem(IDC_COMBO_PRESET_NUM);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight + 100;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}
	//预置位名称文本
	pWnd = GetDlgItem(IDC_STATIC_PRESET_NAME);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top + nButtonHeight;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = nStaticLeftStart;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}
	//预置位名称
	pWnd = GetDlgItem(IDC_EDIT_PRESET_NAME);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}
	//转至预置位
	pWnd = GetDlgItem(IDC_BUTTON_TO_PRESET);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.bottom;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = winrc.left + nPresetTreeWidth;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}
	//设置预置位
	pWnd = GetDlgItem(IDC_BUTTON_SET_PRESET);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}
	//删除预置位
	pWnd = GetDlgItem(IDC_BUTTON_DELETE_PRESET);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//联动树文本
	pWnd = GetDlgItem(IDC_STATIC_LINKAGE_LIST);
	if (pWnd != NULL)
	{
		button_rect.top = winrc.top;
		button_rect.bottom = button_rect.top + nStaticLinkageTreeHeight;
		button_rect.left = winrc.left + nPresetTreeWidth + nVideoWidth;
		button_rect.right = button_rect.left + nStaticLinkageTreeWidth;
		pWnd->MoveWindow(button_rect);
	}

	//联动树
	pWnd = GetDlgItem(IDC_TREE_LINKAGE_LIST);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.bottom;
		button_rect.bottom = button_rect.top + nLinkageTreeHeight;
		button_rect.left = button_rect.left;
		button_rect.right = button_rect.left + nLinkageTreeWidth;
		pWnd->MoveWindow(button_rect);
	}

	//联动关系列表文本
	pWnd = GetDlgItem(IDC_STATIC_LINKAGE_RELATION);
	if (pWnd != NULL)
	{
		list_rect.top = winrc.top;
		list_rect.bottom = list_rect.top + nStaticLinkageRelationListHeight;
		list_rect.left = winrc.left + nPresetTreeWidth + nVideoWidth + nLinkageTreeWidth;
		list_rect.right = list_rect.left + nStaticLinkageRelationListWidth;
		pWnd->MoveWindow(list_rect);
	}

	//联动关系列表
	pWnd = GetDlgItem(IDC_LIST_LINKAGE_RELATION);
	if (pWnd != NULL)
	{
		list_rect.top = list_rect.bottom;
		list_rect.bottom = list_rect.top + nLinkageListHeight;
		list_rect.left = list_rect.left;
		list_rect.right = list_rect.left + nLinkageListWidth;
		pWnd->MoveWindow(list_rect);
	}
	

}



void CDlgPresetSetting::OnBnClickedButtonLeft()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgPresetSetting::OnBnClickedButtonLock()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgPresetSetting::OnBnClickedButtonRight()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgPresetSetting::OnBnClickedButtonDown()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgPresetSetting::OnBnClickedButtonZoomIn()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgPresetSetting::OnBnClickedButtonZoomOut()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgPresetSetting::OnBnClickedButtonToPreset()
{
	if (m_bVideoOpen == FALSE)
	{
		MessageBox("请先打开摄像头！","辅助系统");
		return ;
	}

	CString strPresetId = "";
	GetDlgItem(IDC_COMBO_PRESET_NUM)->GetWindowText(strPresetId);
	m_nCameraPresetId = atoi(strPresetId);

	if (m_nCameraPresetId == 0)
	{
		MessageBox("没有选择预置位！","辅助系统");
		return;
	}

	//调用转到预置位接口函数
	int nRet = DvrPtzControl_Devsdk(m_camera_info.ip_addr,m_camera_info.ip_port,m_camera_info.login_name,
		m_camera_info.login_password,m_camera_info.dvr_type,m_camera_info.dvr_id,
		m_camera_info.channel,GOTOPRESET,m_nCameraPresetId);
	if (nRet == FALSE)
	{
		MessageBox("转到预置位失败！","辅助系统");
		return ;
	}

	//写操作记录
	CString strText = "";
	strText.Format("转到预置位成功：摄像头code：%s，预置位编号：%d", m_szCameraCode,m_nCameraPresetId);
	WriteAssUserOperateLogPre(strText.GetBuffer());
}

void CDlgPresetSetting::OnBnClickedButtonSetPreset()
{
	if (m_bVideoOpen == FALSE)
	{
		MessageBox("请先打开摄像头！","辅助系统");
		return ;
	}

	CString str_preset_id,strPresetName,strText;
	m_combo_presetnum.GetWindowText(str_preset_id);
	int nPresetID = atoi(str_preset_id);
	GetDlgItem(IDC_EDIT_PRESET_NAME)->GetWindowText(strPresetName);
	if (strPresetName.IsEmpty())
	{
		MessageBox("请输入预置位名称！","辅助系统");
		return ;
	}

	HTREEITEM hRvuDeviceItem = m_treeLinkageList.GetSelectedItem();
	if (hRvuDeviceItem == NULL)
	{
		MessageBox("请选择联动设备！","辅助系统");
		return;
	}

	NodeInfo* pRvuDeviceNodeInfo = (NodeInfo*)m_treeLinkageList.GetItemData(hRvuDeviceItem);
	if (pRvuDeviceNodeInfo == NULL)
	{
		MessageBox("请选择联动设备！","辅助系统");
		return ;
	}
	if (pRvuDeviceNodeInfo->node_type != RVU_DEVICE_NODE)
	{
		MessageBox("请选择联动设备！","辅助系统");
		return ;
	}

	HTREEITEM hItem = m_treePresetList.GetSelectedItem();
	if (hItem == NULL)
	{
		MessageBox("请选择摄像头！","辅助系统");
		return ;
	}

	NodeInfo* pNodeInfo = (NodeInfo*)m_treePresetList.GetItemData(hItem);
	if (pNodeInfo == NULL)
	{
		MessageBox("请选择摄像头！","辅助系统");
		return ;
	}

	if (pNodeInfo->node_type != CAMERA_NODE)
	{
		MessageBox("请选择摄像头！","辅助系统");
		return ;
	}

	//设置预置位前判断当前预置位号有没有被占用
	if (!QueryPresetListDB(m_pStationNode->station_id, pNodeInfo->node_code, pRvuDeviceNodeInfo->node_id, nPresetID))
	{
		return ;
	}

	//设置预置位
	if(!m_pViewChannel->SetPreset(nPresetID))
	{
		return;
	}

	//入库
	int nStationId = m_pStationNode->station_id;
	char szCameraCode[64] = {0};
	sprintf_s(szCameraCode,"%s",pNodeInfo->node_code);
	char szDevName[64] = {0};
	sprintf_s(szDevName,"%s",pRvuDeviceNodeInfo->node_name);
	if (UpdatePresetInfoToDB(nStationId, szCameraCode, nPresetID, strPresetName.GetBuffer(), pRvuDeviceNodeInfo->node_id, 1))
	{
		MessageBox("预置位设置成功！","辅助系统");

		//写操作记录
		CString strText = "";
		strText.Format("预置位设置成功！摄像头名称：%s,预置位编号：%d", pNodeInfo->node_name, nPresetID);
		WriteAssUserOperateLogPre(strText.GetBuffer());
	}

	//设置成员变量
	m_nCameraPresetId = nPresetID;
	sprintf_s(m_szCameraCode,64,pNodeInfo->node_code);

	//初始化一下联动关系表
	InitLinkageRelationList(m_pStationNode);
}

/*
	1.设置预置位时，操作数据表的动作，首先更新ass_rvu_sm表的relation_video_status字段为1，表示该设备已关联视频
	2.然后将预置位信息插入到ass_camerapreset表中，%d代表变电站的id，是ob_d5000_station中的id字段。
*/
BOOL CDlgPresetSetting::UpdatePresetInfoToDB(int nStationId, char* szCameraCode, int nPresetId, char* szPresetName, int nDevId, int nLinkageVideoStatus)
{
	char sql_buf[1024]={0x0};
	MYSQL_RES	* res ;
	MYSQL_ROW	row ;
	int rnum = 0;

	//如果是设置预置位
	if (nLinkageVideoStatus == SETPRESET)
	{
		//更新ass_rvu_sm表的relation_video_status字段为1
		sprintf_s(sql_buf,"UPDATE ass_rvu_sm set relation_video_status=%d where id=%d", nLinkageVideoStatus,nDevId);
		if (mysql_query(g_LinkageSettingMysql,sql_buf))
		{
			MessageBox("设置预置位时，入库失败!","辅助系统");
			return FALSE;
		}

		//入ass_camerapreset_station库
		sprintf_s(sql_buf,"INSERT INTO ass_camerapreset(camera_code,dev_id,camera_preset_id,preset_name) VALUES('%s',%d,%d,'%s')",
			szCameraCode, nDevId, nPresetId, szPresetName);

		if (mysql_query(g_LinkageSettingMysql,sql_buf))
		{
			MessageBox("设置预置位时，入库失败!","辅助系统");
			return FALSE;
		}
	}

	//如果是删除预置位
	if (nLinkageVideoStatus == UNSETPRESET)
	{
		//更新ass_rvu_sm表的relation_video_status字段为0
		sprintf_s(sql_buf,"UPDATE ass_rvu_sm set relation_video_status=%d where id=%d", nLinkageVideoStatus,nDevId);
		if (mysql_query(g_LinkageSettingMysql,sql_buf))
		{
			MessageBox("设置预置位时，入库失败!","辅助系统");
			return FALSE;
		}

		//删除ass_camerapreset_station表中的记录
		sprintf_s(sql_buf,1024,"delete from ass_camerapreset where camera_preset_id=%d and camera_code='%s'", nPresetId, szCameraCode);
		if (mysql_query(g_LinkageSettingMysql,sql_buf))
		{
			MessageBox("删除预置位失败，请检测数据库连接!","辅助系统");
			return FALSE;
		}
	}

	return TRUE;
}

/*
	功能：设置预置位时，判断
		1.判断是否创建了ass_camerapreset_station_%d表
		2.一个摄像头不能有两个预置位和某一个设备对应
		3.一个摄像头的一个预置位不能和两个设备对应
*/
BOOL CDlgPresetSetting::QueryPresetListDB(int station_id, char* camera_code, int nDevId, int nPresetID)
{
	char sql_buf[1024]={0x0};

	MYSQL_RES	* res ;
	MYSQL_ROW	row ;
	int rnum = 0;

	sprintf_s(sql_buf, "SHOW TABLES LIKE 'ass_camerapreset'");
	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		rnum = mysql_num_rows(res);
		mysql_free_result( res ) ;
	}

	if (rnum == 0)
	{
		sprintf_s(sql_buf, "CREATE TABLE `ass_camerapreset` ( "
			"`id` int(16) NOT NULL auto_increment, "
			"`camera_code` char(20) default NULL, "
			"`dev_name` char(64) default NULL, "
			"`camera_preset_id` int(16) default NULL, "
			"`preset_name` char(64) default NULL, "
			"PRIMARY KEY  (`id`) "
			") ENGINE=InnoDB DEFAULT CHARSET=utf8;");

		TRACE("%s", sql_buf);

		if (!mysql_query(g_LinkageSettingMysql, sql_buf))
		{
			res = mysql_store_result(g_LinkageSettingMysql);

			mysql_free_result(res) ;
		}
		else
		{
			AfxMessageBox("创建新的预置位列表失败");
			return FALSE;
		}
	}

	if (rnum != 0)
	{
		//一个摄像头不能有两个预置位和某一个设备对应
		sprintf_s(sql_buf, "SELECT camera_code, dev_id FROM ass_camerapreset");

		if (!mysql_query(g_LinkageSettingMysql, sql_buf))
		{
			res = mysql_store_result(g_LinkageSettingMysql);

			while (row = mysql_fetch_row(res))
			{
				if ((strcmp(row[0],camera_code)==0)&&(atoi(row[1]) == nDevId))
				{
					mysql_free_result( res ) ;
					AfxMessageBox("当前设备已经和选中的摄像头一个预置位关联");
					return FALSE;
				}
			}
			mysql_free_result( res ) ;
		}
		else
		{
			AfxMessageBox("读取预置位表失败");
			return FALSE;
		}

		//一个摄像头的一个预置位不能和两个设备对应
		sprintf_s(sql_buf,"select * from ass_camerapreset where camera_code='%s' and camera_preset_id=%d", camera_code, nPresetID);
		if (!mysql_query(g_LinkageSettingMysql, sql_buf))
		{
			res = mysql_store_result(g_LinkageSettingMysql);
			if (mysql_fetch_row(res) != NULL)
			{
				mysql_free_result( res ) ;
				AfxMessageBox("当前摄像头的这个预置位已经关联了其他设备");
				return FALSE;
			}
			mysql_free_result( res ) ;
		}
		else
		{
			AfxMessageBox("读取预置位表失败");
			return FALSE;
		}
	}
	
	return TRUE;
}

void CDlgPresetSetting::OnBnClickedButtonDeletePreset()
{
	if (m_bVideoOpen == FALSE)
	{
		MessageBox("请先打开摄像头！","辅助系统");
		return ;
	}

	int nStationId = m_pStationNode->station_id;
	CString strPresetId = "";
	GetDlgItem(IDC_COMBO_PRESET_NUM)->GetWindowText(strPresetId);
	m_nCameraPresetId = atoi(strPresetId);
	if (m_nCameraPresetId == 0)
	{
		MessageBox("请先选择预置位！","辅助系统");
		return;
	}

	//通过预置位编号和摄像头code获取dev_id
	int nDevId = GetDevIdByPresetIdAndCameraCode(nStationId, m_nCameraPresetId, m_szCameraCode);
	if (nDevId == -1)
	{
		return;
	}

	CString strText = "";
	strText.Format("确定删除预置位%d吗？",m_nCameraPresetId);
	if (MessageBox(strText,"辅助系统",MB_ICONEXCLAMATION|MB_OKCANCEL)==IDCANCEL)
	{
		return;
	}

	if (!m_pViewChannel->DeletePreset(m_nCameraPresetId))
	{
		return;
	}

	//将数据库中的预置位信息删掉
	if (UpdatePresetInfoToDB(nStationId, m_szCameraCode, m_nCameraPresetId, "", nDevId, 0))
	{
		MessageBox("删除预置位成功","辅助系统");

		//写操作记录
		CString strText = "";
		strText.Format("删除预置位成功！摄像头code：%s,预置位编号：%d", m_szCameraCode, m_nCameraPresetId);
		WriteAssUserOperateLogPre(strText.GetBuffer());
	}

	//成员变量清空
	m_nPresetId = 0;
	m_nCameraPresetId = 0;
	sprintf_s(m_szPresetName,64,"");
	//文本框置空
	GetDlgItem(IDC_EDIT_PRESET_NAME)->SetWindowText("");

	//初始化一下联动关系表
	InitLinkageRelationList(m_pStationNode);
}

int CDlgPresetSetting::GetDevIdByPresetIdAndCameraCode(int nStationId, int nPresetId, char *szCameraCode)
{
	int nDevId = 0;
	char sql_buf[1024] = {0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,1024,"SELECT dev_id FROM ass_camerapreset WHERE camera_preset_id=%d and camera_code='%s'", nPresetId, szCameraCode);
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		row = mysql_fetch_row(res);
		if (row != NULL)
		{
			nDevId = atoi(row[1]);
		}
		else
		{
			MessageBox("查找DVR信息失败！","辅助系统");
			mysql_free_result(res);
			return -1;
		}
		mysql_free_result(res);
	}
	else
	{
		MessageBox("查找DVR信息失败！","辅助系统");
		return -1;
	}

	return nDevId;
}

void CDlgPresetSetting::OnNMDblclkTreeLinkageList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	HTREEITEM hItem = m_treeLinkageList.GetSelectedItem();
	if (hItem == NULL)
	{
		return ;
	}

	NodeInfo* pNodeInfo = (NodeInfo*)m_treeLinkageList.GetItemData(hItem);
	if (pNodeInfo == NULL)
	{
		return ;
	}

	GetDlgItem(IDC_EDIT_CURRENT_DEVICE)->SetWindowText("");
	if (pNodeInfo->node_type == RVU_DEVICE_NODE)
	{
		GetDlgItem(IDC_EDIT_CURRENT_DEVICE)->SetWindowText(pNodeInfo->node_name);
		SetPresetEditText();

		////加载这个设备绑定的摄像头
		if (GetCameraInfoByDevId(hItem, pNodeInfo->node_id))
			return;
	}

	if (pNodeInfo->node_type == PRESET_NODE)
	{
		sprintf_s(m_szCameraCode,64,pNodeInfo->node_code);

		//打开摄像头
		if (OpenCamera(m_szCameraCode,pNodeInfo->parent_id)==-1)
		{
			MessageBox("打开摄像头失败！","辅助系统");
			return;
		}

		m_nCameraPresetId = pNodeInfo->node_id;

		ToPreset(m_nCameraPresetId);	
	}
}



int CDlgPresetSetting::GetCameraInfoByDevId(HTREEITEM hItem, int nDevId)
{
	if (m_treePresetList.ItemHasChildren(hItem))
		return -1;

	char sql_buf[1024] = {0x0};
	MYSQL_RES* res;
	MYSQL_ROW row;
	int nRet = 0;
	HTREEITEM hInsertItem;
	CString strText = "";

	sprintf_s(sql_buf,1024,"select camera_preset_id,preset_name,id,dev_id,camera_code from ass_camerapreset where dev_id=%d",nDevId);
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while ( row = mysql_fetch_row( res ) )
		{
			strText.Format("%s", row[1]);
			hInsertItem = m_treeLinkageList.InsertItem(strText, 12, 12, hItem);
			SetTreeNodeInfo(&m_treeLinkageList, hInsertItem, atoi(row[0]), PRESET_NODE, row[1], nDevId, row[4]);
		}
		mysql_free_result(res);
	}
	else
	{
		return -1;
	}
	return nRet;
}

BOOL CDlgPresetSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		//上开始
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_UP)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,1,1);
		}

		//下
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_DOWN)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,2,1);
		}

		//左
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_LEFT)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,3,1);
		}

		//右
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_RIGHT)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,4,1);
		}

		//拉近
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOM_IN)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,5,1);
		}

		//拉远
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOM_OUT)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,6,1);
		}
	}

	if (pMsg->message == WM_LBUTTONUP)
	{
		//上停
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_UP)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,1,0);
		}

		//下
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_DOWN)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,2,0);
		}

		//左
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_LEFT)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,3,0);
		}

		//右
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_RIGHT)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,4,0);
		}

		//拉近
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOM_IN)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,5,0);
		}

		//拉远
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOM_OUT)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,6,0);
		}
	}
	return FALSE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgPresetSetting::OnNMClickTreeLinkageList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

BOOL CDlgPresetSetting::OnEraseBkgnd(CDC* pDC)
{
	//设置对话框背景色
	CBrush backBrush(RGB(218,255,251));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);

	//画需要的区域
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	//// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CDC srcDC;
	//srcDC.CreateCompatibleDC(pDC);
	//CBitmap* pOldBmp;
	//BITMAP bmp;
	//CRect winrc;

	//GetClientRect(&winrc);

	//pOldBmp = srcDC.SelectObject(m_pBackBmp);
	//m_pBackBmp->GetBitmap(&bmp);
	//pDC->StretchBlt(0,0,winrc.Width(),winrc.Height(),
	//	&srcDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	//srcDC.SelectObject(pOldBmp);
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgPresetSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CDlgPresetSetting::OnStnClickedStaticCurrentCamera()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgPresetSetting::OnCbnSelchangeComboPresetNum()
{
	// TODO: 在此添加控件通知处理程序代码
	SetPresetEditText();
}

void CDlgPresetSetting::OnCloseVideo()
{
	// TODO: 在此添加命令处理程序代码
}
