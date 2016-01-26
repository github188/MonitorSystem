// DlgSelectDevice.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"
#include "DlgSelectDevice.h"
#include "CameraVideoOperate.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
extern CVEMCUCtlDlg		*g_pMainDlg;


// CDlgSelectDevice 对话框

IMPLEMENT_DYNAMIC(CDlgSelectDevice, CDialog)

CDlgSelectDevice::CDlgSelectDevice(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectDevice::IDD, pParent)
{
	m_hSelectItem = NULL;
	m_pNodeInfo = NULL;
}

CDlgSelectDevice::~CDlgSelectDevice()
{

}

void CDlgSelectDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DEVICE, m_treeDevice);
	DDX_Control(pDX, IDC_BTN_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_BTN_SELECT, m_btnSelect);
}

BEGIN_MESSAGE_MAP(CDlgSelectDevice, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_DEVICE, &CDlgSelectDevice::OnNMDblclkTreeDevice)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDlgSelectDevice::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_SELECT, &CDlgSelectDevice::OnBnClickedBtnSelect)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_NCHITTEST()
	ON_COMMAND(ID_MENUITEM_SELECT_DEVICE_REFLASHLIST, &CDlgSelectDevice::OnMenuitemSelectDeviceReflashlist)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_DEVICE, &CDlgSelectDevice::OnNMRClickTreeDevice)
END_MESSAGE_MAP()


// CDlgSelectDevice 消息处理程序
BOOL CDlgSelectDevice::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	// 树状列表加载图片
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_TREE);
	m_TreeImage.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1);
	m_TreeImage.Add(&bmp, RGB(0, 255, 0));

	m_treeDevice.SetImageList(&m_TreeImage, TVSIL_NORMAL);
	m_treeDevice.SetBkColor(RGB(239, 246, 253));					//设置树状列表背景色
	m_treeDevice.SetTextColor(RGB(36, 162, 146));

	//////////////////////////////////////////////////////////////////////////
	OnMenuitemSelectDeviceReflashlist();

	loadSkin();

	return TRUE;
}

void CDlgSelectDevice::OnNMDblclkTreeDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	try
	{
		CTreeCtrl *pList = (CTreeCtrl *)GetDlgItem(IDC_TREE_DEVICE);	

		if (pList == NULL)
			return;

		HTREEITEM hItem = pList->GetSelectedItem();
		if (hItem == NULL)
			return;

		char stationname[64] = {0};

		HTREEITEM hInsertItem = NULL;
		HTREEITEM hParentItem = m_treeDevice.GetParentItem(hItem);
		if (hParentItem != NULL)
		{
			sprintf(stationname, "%s", m_treeDevice.GetItemText(hParentItem));
		}

		m_hSelectItem = hItem;
		m_treeDevice.SelectItem(hItem);

		// 判断节点是何种类型节点
		_T_NODE_INFO *pCameraNodeInfo = (_T_NODE_INFO *)(pList->GetItemData(hItem));

		if (pCameraNodeInfo == NULL)
			return;

		if (pCameraNodeInfo->node_type == 2)
		{
			if (m_treeDevice.ItemHasChildren(hItem))
				return;

			// 表示该节点为变电站，获取数据库中的信息
			char sql_buf[255] = {0};
			MYSQL_RES * res = NULL;
			MYSQL_ROW	row ;
			int camera_type = -1;
			int decode_tag = 0;
			int camera_status = 0;
			int streamless = 0;

			// 读取当前视频平台数量及信息
			sprintf_s(sql_buf, "SELECT camera_id, camera_code, camera_name, camera_type,camera_decodetag,camera_status,streamless "
				" FROM ob_d5000_camera_station_%d ORDER BY order_num", pCameraNodeInfo->node_id);

			if (!mysql_query(g_mySqlData, sql_buf))
			{
				res = mysql_store_result(g_mySqlData);

				while ( row = mysql_fetch_row( res ) )
				{
					camera_type = atoi(row[3]);
					if (camera_type <= 0||camera_type > 8)
						continue;

					decode_tag = atoi(row[4]);
					camera_status = atoi(row[5]);
					streamless = atoi(row[6]);

					if (camera_status == 0)
					{
						hInsertItem = m_treeDevice.InsertItem(row[2],4,4, m_hSelectItem);
						if (hInsertItem == NULL)
							continue;
					}
					else if (camera_status == 1)
					{
						if (streamless == 1)
						{
							hInsertItem = m_treeDevice.InsertItem(row[2],4,4, m_hSelectItem);
						}
						else
						{
							hInsertItem = m_treeDevice.InsertItem(row[2],5,5, m_hSelectItem);
						}
						
						if (hInsertItem == NULL)
							continue;
					}
					else if (camera_status == 2)
					{
						if (streamless == 1)
						{
							hInsertItem = m_treeDevice.InsertItem(row[2],11,11, m_hSelectItem);
						}
						else
						{
							hInsertItem = m_treeDevice.InsertItem(row[2],9,9, m_hSelectItem);
						}
						if (hInsertItem == NULL)
							continue;
					}
					else if (camera_status == 3)
					{
						hInsertItem = m_treeDevice.InsertItem(row[2],11,11, m_hSelectItem);
						if (hInsertItem == NULL)
							continue;
					}
					else
					{
						hInsertItem = m_treeDevice.InsertItem(row[2],5,5, m_hSelectItem);
						if (hInsertItem == NULL)
							continue;
					}

					SetCameraNodeInfo(hInsertItem, row[2], row[1], pCameraNodeInfo->node_id,
						camera_status,
						streamless,
						0,
						0,
						decode_tag,
						3,
						pCameraNodeInfo->node_station);
				}
				mysql_free_result( res ) ;
			}

			//设置摄像头DVR
			SetStationCameraDvrInfo(m_hSelectItem);

			////设置摄像头DVR
			//if (m_treeDevice.ItemHasChildren(m_hSelectItem) != FALSE)
			//{
			//	hInsertItem = m_treeDevice.GetChildItem(m_hSelectItem);
			//	while(hInsertItem != NULL)
			//	{
			//		SetCameraDvrInfo(hInsertItem);
			//		hInsertItem = m_treeDevice.GetNextSiblingItem(hInsertItem);
			//	}
			//}
		}
	}
	catch(...)
	{

	}

	*pResult = 0;
}

void CDlgSelectDevice::OnBnClickedBtnExit()
{
	m_pNodeInfo = NULL;
	ShowWindow(SW_HIDE);
}

void CDlgSelectDevice::OnBnClickedBtnSelect()
{
	CTreeCtrl *pList = (CTreeCtrl *)GetDlgItem(IDC_TREE_DEVICE);

	HTREEITEM hItem = pList->GetSelectedItem();
	if (hItem == NULL)
		return;

	m_hSelectItem = hItem;
	m_treeDevice.SelectItem(hItem);

	// 判断节点是何种类型节点
	m_pNodeInfo = (_T_NODE_INFO *)(pList->GetItemData(hItem));
	if (m_pNodeInfo == NULL)
		return;

	if (m_pNodeInfo->node_type != 3)
	{
		MessageBox("请选择摄像头","热点视频监视",MB_OK);
		return;
	}

	g_pDlgReplay->PostMessage(WM_SELECT_DEVICE_NOTIFY,1,WM_MAGIC_VEMCUCTL);

	ShowWindow(SW_HIDE);
}

BOOL CDlgSelectDevice::loadSkin()
{
	m_btnExit.LoadBitmap(IDB_BITMAP_MENU_QUIT, 0);
	m_btnSelect.LoadBitmap(IDB_BITMAP_SELECT, 0);

	m_Bmp.LoadBitmap(IDB_BITMAP_SELECT_DEVICE_BK);
	m_Bmp.GetObject(sizeof(BITMAP),(LPVOID)&m_bm);   
	return TRUE;
}

BOOL CDlgSelectDevice::InitTreeDevice()
{
	int i = 0, j = 0, k = 0;
	int ns = 0;

	DeleteTreeDevice();

	HTREEITEM	hMainItem = NULL;
	HTREEITEM	hChildItem = NULL, hChildItem1 = NULL, hChildItem2 = NULL;
	HTREEITEM	hStationItem = NULL;
	HTREEITEM	hCameraItem = NULL;

	BOOL b500kVFlag = FALSE;//当为500kV时，不显示下面的市级结点，直接显示变电站。

	// 添加根节点
	for (i = 0; i < g_TotalSysNum; i ++)
	{
		if (g_SystemInfo[i].localdomain == 1)
		{
			hMainItem = m_treeDevice.InsertItem(g_SystemInfo[i].sysname, 1, 1, TVI_ROOT, hMainItem);
			break;
		}
	}

	// 添加变电站中间节点和站点节点
	for (i = 0; i < g_nStationNode_Num; i ++)
	{
		if (g_tStationNode_Info[i].parent_id == g_tStationNode_Info[i].node_id)	//第一级中间节点
		{
			//if (strcmp(g_tStationNode_Info[i].node_name,"220kV") == 0)//不显示220kV
			//	continue;

			//if (strcmp(g_tStationNode_Info[i].node_name,"110kV") == 0)//不显示110kV
			//	continue;

			if (strcmp(g_tStationNode_Info[i].node_name,"500kV") == 0)//不显示下面的市级结点，直接显示变电站
				b500kVFlag = TRUE;
			else
				b500kVFlag = FALSE;

			hChildItem = m_treeDevice.InsertItem(g_tStationNode_Info[i].node_name, 7, 7, hMainItem);
			for (ns = 0; ns < g_nStation_Num; ns ++)
			{
				if (g_tStation_Info[ns].node_id == g_tStationNode_Info[i].node_id)
				{
					hStationItem = m_treeDevice.InsertItem(g_tStation_Info[ns].station_name_videoplant, 8, 8, hChildItem);
					SetCameraNodeInfo(hStationItem,
						g_tStation_Info[ns].station_name_videoplant,
						g_tStation_Info[ns].station_code_videoplant,
						g_tStation_Info[ns].station_id,
						1,
						0,
						0,
						0,
						0,
						2,
						g_tStation_Info[ns].station_name_videoplant);
				}
			}

			for (j = 0; j < g_nStationNode_Num; j ++)
			{
				if (g_tStationNode_Info[j].parent_id == g_tStationNode_Info[i].node_id && g_tStationNode_Info[j].parent_id != g_tStationNode_Info[j].node_id)	// 第二季中间节点
				{
					if (b500kVFlag == FALSE)
						hChildItem1 = m_treeDevice.InsertItem(g_tStationNode_Info[j].node_name, 7, 7, hChildItem);
					else
						hChildItem1 = hChildItem;

					for (ns = 0; ns < g_nStation_Num; ns ++)
					{
						if (g_tStation_Info[ns].node_id == g_tStationNode_Info[j].node_id)
						{
							hStationItem = m_treeDevice.InsertItem(g_tStation_Info[ns].station_name_videoplant, 8, 8, hChildItem1);
							SetCameraNodeInfo(hStationItem,
								g_tStation_Info[ns].station_name_videoplant,
								g_tStation_Info[ns].station_code_videoplant,
								g_tStation_Info[ns].station_id,
								1,
								0,
								0,
								0,
								0,
								2,
								g_tStation_Info[ns].station_name_videoplant);
						}
					}

					for (k = 0; k < g_nStationNode_Num; k++)
					{
						if (g_tStationNode_Info[k].parent_id == g_tStationNode_Info[j].node_id && g_tStationNode_Info[k].parent_id != g_tStationNode_Info[k].node_id)	// 第三季中间节点
						{
							hChildItem2 = m_treeDevice.InsertItem(g_tStationNode_Info[k].node_name, 7, 7, hChildItem1);
							for (ns = 0; ns < g_nStation_Num; ns ++)
							{
								if (g_tStation_Info[ns].node_id == g_tStationNode_Info[k].node_id)
								{
									hStationItem = m_treeDevice.InsertItem(g_tStation_Info[ns].station_name_videoplant, 8, 8, hChildItem2);
									SetCameraNodeInfo(hStationItem,
										g_tStation_Info[ns].station_name_videoplant,
										g_tStation_Info[ns].station_code_videoplant,
										g_tStation_Info[ns].station_id,
										1,
										0,
										0,
										0,
										0,
										2,
										g_tStation_Info[ns].station_name_videoplant);
								}
							}
						}
					}
				}
			}
		}
	}

	hMainItem = m_treeDevice.GetRootItem();
	if (hMainItem != NULL)
	{
		hChildItem = m_treeDevice.GetChildItem(hMainItem);
		if (hChildItem != NULL)
		{
			m_treeDevice.SortChildren(hChildItem);//排序
			hChildItem1 = m_treeDevice.GetChildItem(hChildItem);
			if (hChildItem1 != NULL)
			{
				m_treeDevice.EnsureVisible(hChildItem1);
			}
		}
	}

	hChildItem = m_treeDevice.GetNextSiblingItem(hChildItem);
	while(hChildItem != NULL)
	{
		m_treeDevice.SortChildren(hChildItem);//排序
		hChildItem1 = m_treeDevice.GetChildItem(hChildItem);
		while(hChildItem1 != NULL)
		{
			m_treeDevice.SortChildren(hChildItem1);
			hChildItem1 = m_treeDevice.GetNextSiblingItem(hChildItem1);
		}
		hChildItem = m_treeDevice.GetNextSiblingItem(hChildItem);
	}

	return TRUE;
}

BOOL CDlgSelectDevice::InitTreeDeviceByCity()
{
	int i = 0, j = 0, k = 0;
	int ns = 0;
	int nNodeId = 0;

	DeleteTreeDevice();

	HTREEITEM	hMainItem = NULL;
	HTREEITEM	hChildItem = NULL, hChildItem1 = NULL, hChildItem2 = NULL;
	HTREEITEM	hStationItem = NULL;
	HTREEITEM	hCameraItem = NULL;

	int nStationStatus = 0;//变电站状态
	char szStationShowName[256] = {0};
	memset(szStationShowName,0,sizeof(szStationShowName));

	// 添加根节点
	for (i = 0; i < g_TotalSysNum; i ++)
	{
		if (g_SystemInfo[i].localdomain == 1)
		{
			hMainItem = m_treeDevice.InsertItem(g_SystemInfo[i].sysname, 1, 1, TVI_ROOT, hMainItem);
			break;
		}
	}

	nNodeId = g_userpower.subgroupno;

	// 添加变电站中间节点和站点节点
	for (i = 0; i < g_nStationNode_Num; i++)
	{
		if (((nNodeId == 0&&g_tStationNode_Info[i].parent_id == g_tStationNode_Info[i].node_id)
			||(nNodeId != 0&&g_tStationNode_Info[i].node_id == nNodeId))
			&&g_tStationNode_Info[i].login_type == 1)	//第一级中间节点
		{
			hChildItem = m_treeDevice.InsertItem(g_tStationNode_Info[i].node_name, 7, 7, hMainItem);

			for (ns = 0; ns < g_nStation_Num; ns ++)
			{
				if (g_tStation_Info[ns].node_yw_id == g_tStationNode_Info[i].node_id)
				{
					if (strlen(g_tStation_Info[ns].notes) != 0)
						sprintf_s(szStationShowName,sizeof(szStationShowName),"%s(%s)",g_tStation_Info[ns].station_name_videoplant,g_tStation_Info[ns].notes);
					else
						strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

					hStationItem = m_treeDevice.InsertItem(szStationShowName, 8, 8, hChildItem);

					SetCameraNodeInfo(hStationItem,
						g_tStation_Info[ns].station_name_videoplant,
						g_tStation_Info[ns].station_code_videoplant,
						g_tStation_Info[ns].station_id,
						1,
						0,
						0,
						0,
						0,
						2,
						g_tStation_Info[ns].station_name_videoplant);
				}
			}

			for (j = 0; j < g_nStationNode_Num; j ++)//第二级中间节点
			{
				if (g_tStationNode_Info[j].parent_id == g_tStationNode_Info[i].node_id && g_tStationNode_Info[j].parent_id != g_tStationNode_Info[j].node_id)	// 第二季中间节点
				{
					hChildItem1 = m_treeDevice.InsertItem(g_tStationNode_Info[j].node_name, 7, 7, hChildItem);

					for (ns = 0; ns < g_nStation_Num; ns ++)
					{
						if (g_tStation_Info[ns].node_yw_id == g_tStationNode_Info[j].node_id)
						{
							if (strlen(g_tStation_Info[ns].notes) != 0)
								sprintf_s(szStationShowName,sizeof(szStationShowName),"%s(%s)",g_tStation_Info[ns].station_name_videoplant,g_tStation_Info[ns].notes);
							else
								strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

							hStationItem = m_treeDevice.InsertItem(szStationShowName, 8, 8, hChildItem1);

							SetCameraNodeInfo(hStationItem,
								g_tStation_Info[ns].station_name_videoplant,
								g_tStation_Info[ns].station_code_videoplant,
								g_tStation_Info[ns].station_id,
								1,
								0,
								0,
								0,
								0,
								2,
								g_tStation_Info[ns].station_name_videoplant);
						}
					}

					for (k = 0; k < g_nStationNode_Num; k++)//第三级中间节点
					{
						if (g_tStationNode_Info[k].parent_id == g_tStationNode_Info[j].node_id && g_tStationNode_Info[k].parent_id != g_tStationNode_Info[k].node_id)	// 第三季中间节点
						{
							hChildItem2 = m_treeDevice.InsertItem(g_tStationNode_Info[k].node_name, 7, 7, hChildItem1);

							for (ns = 0; ns < g_nStation_Num; ns ++)
							{
								if (g_tStation_Info[ns].node_yw_id == g_tStationNode_Info[k].node_id)
								{
									if (strlen(g_tStation_Info[ns].notes) != 0)
										sprintf_s(szStationShowName,sizeof(szStationShowName),"%s(%s)",g_tStation_Info[ns].station_name_videoplant,g_tStation_Info[ns].notes);
									else
										strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

									hStationItem = m_treeDevice.InsertItem(szStationShowName, 8, 8, hChildItem2);

									SetCameraNodeInfo(hStationItem,
										g_tStation_Info[ns].station_name_videoplant,
										g_tStation_Info[ns].station_code_videoplant,
										g_tStation_Info[ns].station_id,
										1,
										0,
										0,
										0,
										0,
										2,
										g_tStation_Info[ns].station_name_videoplant);
								}
							}
						}
					}
				}
			}
		}
	}

	hMainItem = m_treeDevice.GetRootItem();
	if (hMainItem != NULL)
	{
		hChildItem = m_treeDevice.GetChildItem(hMainItem);
		if (hChildItem != NULL)
		{
			m_treeDevice.SortChildren(hChildItem);//排序
			hChildItem1 = m_treeDevice.GetChildItem(hChildItem);
			if (hChildItem1 != NULL)
			{
				m_treeDevice.EnsureVisible(hChildItem1);
			}
		}
	}

	hChildItem = m_treeDevice.GetNextSiblingItem(hChildItem);
	while(hChildItem != NULL)
	{
		m_treeDevice.SortChildren(hChildItem);//排序
		hChildItem1 = m_treeDevice.GetChildItem(hChildItem);
		while(hChildItem1 != NULL)
		{
			m_treeDevice.SortChildren(hChildItem1);
			hChildItem1 = m_treeDevice.GetNextSiblingItem(hChildItem1);
		}
		hChildItem = m_treeDevice.GetNextSiblingItem(hChildItem);
	}
	return true;
}

BOOL CDlgSelectDevice::InitTreeDeviceByVoltage()
{
	try
	{
		int i = 0, j = 0, k = 0, l = 0;
		int ns = 0;
		int nNodeId = 0;
		int nNodeStationNum1 = 0,nNodeStationNum2 = 0,nNodeStationNum3 = 0;

		//删除摄像头树信息
		DeleteTreeDevice();

		HTREEITEM	hMainItem = NULL;
		HTREEITEM	hChildItem = NULL, hChildItem1 = NULL, hChildItem2 = NULL;
		HTREEITEM	hStationItem = NULL;
		HTREEITEM	hCameraItem = NULL;
		BOOL b500kVFlag = FALSE;//当为500kV时，不显示下面的市级结点，直接显示变电站。

		int nStationStatus = 0;//变电站状态
		char szStationShowName[256] = {0};
		memset(szStationShowName,0,sizeof(szStationShowName));

		// 添加根节点
		for (i = 0; i < g_TotalSysNum; i ++)
		{
			if (g_SystemInfo[i].localdomain == 1)
			{
				hMainItem = m_treeDevice.InsertItem(g_SystemInfo[i].sysname, 1, 1, TVI_ROOT, hMainItem);
				break;
			}
		}
		nNodeId = g_userpower.subgroupno;

		bool bFlag = false;

		if (g_userpower.usertype == 10 || g_userpower.usertype == 11)// || g_userpower.usertype == 12 || g_userpower.usertype == 13)
		{
			bFlag = true;
		}

		// 添加变电站中间节点和站点节点
		for (i = 0; i < g_nStationNode_Num; i ++)
		{
			if (g_tStationNode_Info[i].parent_id == g_tStationNode_Info[i].node_id
				&&g_tStationNode_Info[i].login_type == 2)
			{
				//if (strcmp(g_tStationNode_Info[i].node_name,"220kV") == 0)//不显示220kV
				//	continue;

				//if (strcmp(g_tStationNode_Info[i].node_name,"110kV") == 0)//不显示110kV
				//	continue;

				if (strcmp(g_tStationNode_Info[i].node_name,"500kV") == 0)//不显示下面的市级结点，直接显示变电站
					b500kVFlag = TRUE;
				else
					b500kVFlag = FALSE;

				hChildItem = m_treeDevice.InsertItem(g_tStationNode_Info[i].node_name, 7, 7, hMainItem);

				nNodeStationNum1 = 0;
				for (ns = 0; ns < g_nStation_Num; ns ++)
				{
					if (g_tStation_Info[ns].node_sd_yw_id == g_tStationNode_Info[i].node_id && g_userpower.usertype <= 11)
					{
						// 						if (strlen(g_tStation_Info[ns].notes) != 0)
						// 							sprintf_s(szStationShowName,sizeof(szStationShowName),"%s(%s)",g_tStation_Info[ns].station_name_videoplant,g_tStation_Info[ns].notes);
						// 						else
						strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

//						nStationStatus = CheckStationCurrentStatus2(g_tStation_Info[ns].station_id);
//						if (nStationStatus != 0)
							hStationItem = m_treeDevice.InsertItem(szStationShowName, 8, 8, hChildItem);
//						else
//							hStationItem = m_treeDevice.InsertItem(szStationShowName, 10, 10, hChildItem);

						SetCameraNodeInfo(hStationItem,
							g_tStation_Info[ns].station_name_videoplant,
							g_tStation_Info[ns].station_code_videoplant,
							g_tStation_Info[ns].station_id,
							1,
							0,
							0,
							0,
							0,
							2,
							g_tStation_Info[ns].station_name_videoplant);

						nNodeStationNum1 ++;
					}
				}

				for (j = 0; j < g_nStationNode_Num; j ++)//第二级中间节点--南京市、苏州市
				{
					if (g_tStationNode_Info[j].parent_id == g_tStationNode_Info[i].node_id && g_tStationNode_Info[j].parent_id != g_tStationNode_Info[j].node_id)	// 第二季中间节点
					{
						if (b500kVFlag == FALSE)
							hChildItem1 = m_treeDevice.InsertItem(g_tStationNode_Info[j].node_name, 7, 7, hChildItem);
						else
							hChildItem1 = hChildItem;

						nNodeStationNum2 = 0;
						for (ns = 0; ns < g_nStation_Num; ns ++)
						{
							if (g_tStation_Info[ns].node_sd_yw_id == g_tStationNode_Info[j].node_id)
							{
								// 								if (strlen(g_tStation_Info[ns].notes) != 0)
								// 									sprintf_s(szStationShowName,sizeof(szStationShowName),"%s(%s)",g_tStation_Info[ns].station_name_videoplant,g_tStation_Info[ns].notes);
								// 								else
								strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

//								nStationStatus = CheckStationCurrentStatus2(g_tStation_Info[ns].station_id);
//								if (nStationStatus != 0)
									hStationItem = m_treeDevice.InsertItem(szStationShowName, 8, 8, hChildItem1);
//								else
//									hStationItem = m_treeDevice.InsertItem(szStationShowName, 10, 10, hChildItem1);

								SetCameraNodeInfo(hStationItem,
									g_tStation_Info[ns].station_name_videoplant,
									g_tStation_Info[ns].station_code_videoplant,
									g_tStation_Info[ns].station_id,
									1,
									0,
									0,
									0,
									0,
									2,
									g_tStation_Info[ns].station_name_videoplant);

								nNodeStationNum1 ++;
								nNodeStationNum2 ++;
							}
						}

						for (k = 0; k < g_nStationNode_Num; k++)//第三级中间节点
						{
							if (g_tStationNode_Info[k].parent_id == g_tStationNode_Info[j].node_id && g_tStationNode_Info[k].parent_id != g_tStationNode_Info[k].node_id)	// 第三季中间节点
							{
								// 判断运维班是否属于该用户
								if (bFlag == FALSE)
								{
									for (l = 0; l < g_nUserStationViewYwbCount; l ++)
									{
										if (g_nUserStationViewYwbId[l] == g_tStationNode_Info[k].node_id)
										{
											break;
										}
									}
									if (l == g_nUserStationViewYwbCount)
									{
										continue;
									}
								}

								hChildItem2 = m_treeDevice.InsertItem(g_tStationNode_Info[k].node_name, 7, 7, hChildItem1);

								nNodeStationNum3 = 0;
								for (ns = 0; ns < g_nStation_Num; ns ++)
								{
									if (g_tStation_Info[ns].node_sd_yw_id == g_tStationNode_Info[k].node_id)
									{
										// 										if (strlen(g_tStation_Info[ns].notes) != 0)
										// 											sprintf_s(szStationShowName,sizeof(szStationShowName),"%s(%s)",g_tStation_Info[ns].station_name_videoplant,g_tStation_Info[ns].notes);
										// 										else
										strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

//										nStationStatus = CheckStationCurrentStatus2(g_tStation_Info[ns].station_id);
//										if (nStationStatus != 0)
											hStationItem = m_treeDevice.InsertItem(szStationShowName, 8, 8, hChildItem2);
//										else
//											hStationItem = m_treeDevice.InsertItem(szStationShowName, 10, 10, hChildItem2);

										SetCameraNodeInfo(hStationItem,
											g_tStation_Info[ns].station_name_videoplant,
											g_tStation_Info[ns].station_code_videoplant,
											g_tStation_Info[ns].station_id,
											1,
											0,
											0,
											0,
											0,
											2,
											g_tStation_Info[ns].station_name_videoplant);

										nNodeStationNum1 ++;
										nNodeStationNum2 ++;
										nNodeStationNum3 ++;
									}
								}
								if (bFlag == false && nNodeStationNum3 == 0)
								{
									m_treeDevice.DeleteItem(hChildItem2);
								}
							}
						}

						if (bFlag == false && nNodeStationNum2 == 0)
						{
							m_treeDevice.DeleteItem(hChildItem1);
						}
					}
				}

				if (bFlag == false && nNodeStationNum1 == 0)
				{
					m_treeDevice.DeleteItem(hChildItem);
				}
			}
		}

		hMainItem = m_treeDevice.GetRootItem();
		if (hMainItem != NULL)
		{
			hChildItem = m_treeDevice.GetChildItem(hMainItem);
			if (hChildItem != NULL)
			{
				m_treeDevice.SortChildren(hChildItem);//排序
				hChildItem1 = m_treeDevice.GetChildItem(hChildItem);
				if (hChildItem1 != NULL)
				{
					m_treeDevice.EnsureVisible(hChildItem1);
				}
			}
		}

		hChildItem = m_treeDevice.GetNextSiblingItem(hChildItem);
		while(hChildItem != NULL)
		{
			m_treeDevice.SortChildren(hChildItem);//排序
			hChildItem1 = m_treeDevice.GetChildItem(hChildItem);
			while(hChildItem1 != NULL)
			{
				m_treeDevice.SortChildren(hChildItem1);
				hChildItem1 = m_treeDevice.GetNextSiblingItem(hChildItem1);
			}
			hChildItem = m_treeDevice.GetNextSiblingItem(hChildItem);
		}
	}
	catch(...)
	{

	}

	return true;
/*	int i = 0, j = 0, k = 0;
	int ns = 0;
	int v = 0;

	DeleteTreeDevice();

	HTREEITEM	hMainItem = NULL;
	HTREEITEM	hChildItem = NULL, hChildItem1 = NULL, hChildItem2 = NULL;
	HTREEITEM	hStationItem = NULL;
	HTREEITEM	hCameraItem = NULL;
	BOOL b500kVFlag = FALSE;//当为500kV时，不显示下面的市级结点，直接显示变电站。

	HTREEITEM	hVoltageItem = NULL;
	int nNodeId = 0;
	int nNodeIndex = 0;
	int nNodeVoltage = 0;
	int nNodeVoltage2 = 0;

	int nStationStatus = 0;//变电站状态
	char szStationShowName[256] = {0};
	memset(szStationShowName,0,sizeof(szStationShowName));

	// 添加根节点
	for (i = 0; i < g_TotalSysNum; i ++)
	{
		if (g_SystemInfo[i].localdomain == 1)
		{
			hMainItem = m_treeDevice.InsertItem(g_SystemInfo[i].sysname, 1, 1, TVI_ROOT);
			break;
		}
	}

	nNodeId = g_userpower.subgroupno;

	if (nNodeId != 0)
	{
		for (i = 0;i < g_nStationNode_Num;i++)
		{
			if (g_tStationNode_Info[i].node_id == nNodeId)
			{
				nNodeIndex = i;
				break;
			}
		}

		if (nNodeIndex >= g_nStationNode_Num)
			return FALSE;

		nNodeVoltage = g_tStationNode_Info[nNodeIndex].voltage_class;
	}
	else
	{
		nNodeVoltage = 7;
	}

	for(v = 0;v < 3;v++)
	{
		if (v == 0)
		{
			if (nNodeVoltage&0x1)
			{
				hVoltageItem = m_treeDevice.InsertItem("500kV", 7, 7, hMainItem);
				b500kVFlag = TRUE;
			}
			else
			{
				continue;
			}
		}
		else if (v == 1)
		{
			if (nNodeVoltage&0x2)
			{
				hVoltageItem = m_treeDevice.InsertItem("220kV", 7, 7, hMainItem);
				b500kVFlag = FALSE;
			}
			else
			{
				continue;
			}
		}
		else if (v == 2)
		{
			if (nNodeVoltage&0x4)
			{
				hVoltageItem = m_treeDevice.InsertItem("110kV", 7, 7, hMainItem);
				b500kVFlag = FALSE;
			}
			else
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		// 添加变电站中间节点和站点节点
		for (i = 0; i < g_nStationNode_Num; i ++)
		{
			if (((nNodeId == 0&&g_tStationNode_Info[i].parent_id == g_tStationNode_Info[i].node_id)
				||(nNodeId != 0&&g_tStationNode_Info[i].node_id == nNodeId))
				&&g_tStationNode_Info[i].login_type == 1)	//第一级中间节点
			{
				nNodeVoltage2 = g_tStationNode_Info[i].voltage_class;
				if ((nNodeVoltage2&(1<<v)) == 0)
					continue;

				if (b500kVFlag == FALSE)
					hChildItem = m_treeDevice.InsertItem(g_tStationNode_Info[i].node_name, 7, 7, hVoltageItem);
				else 
					hChildItem = hVoltageItem;

				for (ns = 0; ns < g_nStation_Num; ns++)
				{
					if (g_tStation_Info[ns].node_yw_id == g_tStationNode_Info[i].node_id)
					{
						if ((g_tStation_Info[ns].voltage_class == 500&&v != 0)||(g_tStation_Info[ns].voltage_class == 220&&v != 1)||(g_tStation_Info[ns].voltage_class == 110&&v != 2))
							continue;

						if (strlen(g_tStation_Info[ns].notes) != 0)
							sprintf_s(szStationShowName,sizeof(szStationShowName),"%s(%s)",g_tStation_Info[ns].station_name_videoplant,g_tStation_Info[ns].notes);
						else
							strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

						hStationItem = m_treeDevice.InsertItem(szStationShowName, 8, 8, hChildItem);

						SetCameraNodeInfo(hStationItem,
							g_tStation_Info[ns].station_name_videoplant,
							g_tStation_Info[ns].station_code_videoplant,
							g_tStation_Info[ns].station_id,
							1,
							0,
							0,
							0,
							0,
							2,
							g_tStation_Info[ns].station_name_videoplant);
					}
				}

				for (j = 0; j < g_nStationNode_Num; j ++)//第二级中间节点
				{
					if (g_tStationNode_Info[j].parent_id == g_tStationNode_Info[i].node_id && g_tStationNode_Info[j].parent_id != g_tStationNode_Info[j].node_id)	// 第二季中间节点
					{
						nNodeVoltage2 = g_tStationNode_Info[j].voltage_class;
						if ((nNodeVoltage2&(1<<v)) == 0)
							continue;

						if (b500kVFlag == FALSE)
							hChildItem1 = m_treeDevice.InsertItem(g_tStationNode_Info[j].node_name, 7, 7, hChildItem);
						else
							hChildItem1 = hChildItem;

						for (ns = 0; ns < g_nStation_Num; ns ++)
						{
							if (g_tStation_Info[ns].node_yw_id == g_tStationNode_Info[j].node_id)
							{
								if ((g_tStation_Info[ns].voltage_class == 500&&v != 0)||(g_tStation_Info[ns].voltage_class == 220&&v != 1)||(g_tStation_Info[ns].voltage_class == 110&&v != 2))
									continue;

								if (strlen(g_tStation_Info[ns].notes) != 0)
									sprintf_s(szStationShowName,sizeof(szStationShowName),"%s(%s)",g_tStation_Info[ns].station_name_videoplant,g_tStation_Info[ns].notes);
								else
									strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

								hStationItem = m_treeDevice.InsertItem(szStationShowName, 8, 8, hChildItem1);

								SetCameraNodeInfo(hStationItem,
									g_tStation_Info[ns].station_name_videoplant,
									g_tStation_Info[ns].station_code_videoplant,
									g_tStation_Info[ns].station_id,
									1,
									0,
									0,
									0,
									0,
									2,
									g_tStation_Info[ns].station_name_videoplant);
							}
						}

						for (k = 0; k < g_nStationNode_Num; k++)//第三级中间节点
						{
							if (g_tStationNode_Info[k].parent_id == g_tStationNode_Info[j].node_id && g_tStationNode_Info[k].parent_id != g_tStationNode_Info[k].node_id)	// 第三季中间节点
							{
								nNodeVoltage2 = g_tStationNode_Info[k].voltage_class;
								if ((nNodeVoltage2&(1<<v)) == 0)
									continue;

								if (b500kVFlag == FALSE)
									hChildItem2 = m_treeDevice.InsertItem(g_tStationNode_Info[k].node_name, 7, 7, hChildItem1);
								else
									hChildItem2 = hChildItem1;

								for (ns = 0; ns < g_nStation_Num; ns ++)
								{
									if (g_tStation_Info[ns].node_yw_id == g_tStationNode_Info[k].node_id)
									{
										if ((g_tStation_Info[ns].voltage_class == 500&&v != 0)||(g_tStation_Info[ns].voltage_class == 220&&v != 1)||(g_tStation_Info[ns].voltage_class == 110&&v != 2))
											continue;

										if (strlen(g_tStation_Info[ns].notes) != 0)
											sprintf_s(szStationShowName,sizeof(szStationShowName),"%s(%s)",g_tStation_Info[ns].station_name_videoplant,g_tStation_Info[ns].notes);
										else
											strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

										hStationItem = m_treeDevice.InsertItem(szStationShowName, 8, 8, hChildItem2);

										SetCameraNodeInfo(hStationItem,
											g_tStation_Info[ns].station_name_videoplant,
											g_tStation_Info[ns].station_code_videoplant,
											g_tStation_Info[ns].station_id,
											1,
											0,
											0,
											0,
											0,
											2,
											g_tStation_Info[ns].station_name_videoplant);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	hMainItem = m_treeDevice.GetRootItem();
	if (hMainItem != NULL)
	{
		hChildItem = m_treeDevice.GetChildItem(hMainItem);
		if (hChildItem != NULL)
		{
			m_treeDevice.SortChildren(hChildItem);//排序
			hChildItem1 = m_treeDevice.GetChildItem(hChildItem);
			if (hChildItem1 != NULL)
			{
				m_treeDevice.EnsureVisible(hChildItem1);
			}
		}
	}

	hChildItem = m_treeDevice.GetNextSiblingItem(hChildItem);
	while(hChildItem != NULL)
	{
		m_treeDevice.SortChildren(hChildItem);//排序
		hChildItem1 = m_treeDevice.GetChildItem(hChildItem);
		while(hChildItem1 != NULL)
		{
			m_treeDevice.SortChildren(hChildItem1);
			hChildItem1 = m_treeDevice.GetNextSiblingItem(hChildItem1);
		}
		hChildItem = m_treeDevice.GetNextSiblingItem(hChildItem);
	}
	return TRUE;*/
}

void CDlgSelectDevice::DeleteTreeDevice()
{
	try
	{
		HTREEITEM	hMainItem = NULL;
		_T_NODE_INFO *pNodeInfo = NULL;
		DWORD_PTR pInfo = NULL;

		hMainItem = m_treeDevice.GetRootItem();
		while(hMainItem != NULL)
		{
			pInfo = m_treeDevice.GetItemData(hMainItem);
			if (pInfo != NULL)
			{
				pNodeInfo = (_T_NODE_INFO *)pInfo;
				delete pNodeInfo;
				pInfo = NULL;
				pNodeInfo = NULL;
			}

			DeleteChildTreeDeviceInfo(hMainItem);
			hMainItem = m_treeDevice.GetNextSiblingItem(hMainItem);
		}

		m_treeDevice.DeleteAllItems();
	}
	catch(...)
	{

	}
}

void CDlgSelectDevice::DeleteChildTreeDeviceInfo(HTREEITEM hItem)
{
	try
	{
		HTREEITEM	hChildItem = NULL;
		_T_NODE_INFO *pNodeInfo = NULL;
		DWORD_PTR pInfo = NULL;

		hChildItem = m_treeDevice.GetChildItem(hItem);
		while(hChildItem != NULL)
		{
			pInfo = m_treeDevice.GetItemData(hChildItem);
			if (pInfo != NULL)
			{
				pNodeInfo = (_T_NODE_INFO *)pInfo;
				delete pNodeInfo;
				pInfo = NULL;
				pNodeInfo = NULL;
				m_treeDevice.SetItemData(hChildItem,NULL);
			}

			DeleteChildTreeDeviceInfo(hChildItem);
			hChildItem = m_treeDevice.GetNextSiblingItem(hChildItem);
		}
	}
	catch(...)
	{

	}
}

void CDlgSelectDevice::SetCameraNodeInfo(HTREEITEM hItem, char* ItemName, char* Itemnum, int StationOrCameraID,int ItemStatus,int Streamless, float ItemLongitude, float ItemLatitude, int ItemDecodeTag, int Itemtype, char* stationname)
{
	_T_NODE_INFO *pNodeInfo = NULL;

	try
	{
		pNodeInfo = new _T_NODE_INFO;
		memset(pNodeInfo, 0, sizeof(_T_NODE_INFO));

		sprintf (pNodeInfo->node_name, "%s", ItemName);
		sprintf(pNodeInfo->node_num, "%s", Itemnum);

		pNodeInfo->node_id = StationOrCameraID;
		pNodeInfo->node_status = ItemStatus;
		pNodeInfo->node_streamless = Streamless;
		pNodeInfo->node_longitude = ItemLongitude;
		pNodeInfo->node_latitude = ItemLatitude;
		pNodeInfo->node_type = Itemtype;

		if (ItemDecodeTag == 1)
		{
			ItemDecodeTag = 150;
		}

		pNodeInfo->node_decodetag = ItemDecodeTag;
		sprintf(pNodeInfo->node_station, "%s", stationname);

		m_treeDevice.SetItemData(hItem, (DWORD_PTR)pNodeInfo);
	}
	catch(...)
	{

	}
}

BOOL CDlgSelectDevice::SetCameraDvrInfo(HTREEITEM hTreeItem)
{
	_T_NODE_INFO *pCameraNodeInfo = NULL;

	try
	{
		if (hTreeItem == NULL)
			return FALSE;

		pCameraNodeInfo = (_T_NODE_INFO *)m_treeDevice.GetItemData(hTreeItem);
		if (pCameraNodeInfo == NULL||pCameraNodeInfo->node_type != 3)
			return FALSE;

		_T_CAMERA_INFO * pCameraDvrInfo = &pCameraNodeInfo->camera_info;
		_T_DVR_INFO * pDvrInfo = &pCameraDvrInfo->dvr_info;

		char szDvrType[32] = {0};

		//表示该节点为摄像头信息,获取数据库中的摄像头DVR信息
		char sql_buf[1024] = {0};

		MYSQL_RES * res = NULL;
		MYSQL_ROW	row;

		// 读取摄像头DVR信息
		sprintf_s(sql_buf, "SELECT t1.id,t1.dvr_type,t1.dvr_id,t1.dvr_ip,t1.link_port,t2.channel,t1.adminuser,t1.adminpasswd,t1.station_group,t1.station_name,t1.station_seq  "
			" FROM video_dvr AS t1,video_camera AS t2 WHERE t2.camera_id='%s' AND t2.dvr_id=t1.dvr_id ORDER BY t1.id ASC", pCameraNodeInfo->node_num);

		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);
			while (row = mysql_fetch_row(res))
			{
				pDvrInfo->dvr_id = atoi(row[0]);
				strcpy_s(szDvrType,sizeof(szDvrType),row[1]);
				pDvrInfo->dvr_type = AppGetCameraDvrType(szDvrType);
				strcpy_s(pDvrInfo->dvr_num,sizeof(pDvrInfo->dvr_num),row[2]);
				strcpy_s(pDvrInfo->dvr_ip,sizeof(pDvrInfo->dvr_ip),row[3]);
				pDvrInfo->dvr_port = atoi(row[4]);
				pCameraDvrInfo->channel = atoi(row[5]);
				strcpy_s(pDvrInfo->dvr_user,sizeof(pDvrInfo->dvr_user),row[6]);
				strcpy_s(pDvrInfo->dvr_password,sizeof(pDvrInfo->dvr_password),row[7]);
				strcpy_s(pDvrInfo->station_num,sizeof(pDvrInfo->station_num),row[8]);
				strcpy_s(pDvrInfo->station_name,sizeof(pDvrInfo->station_name),row[9]);
				pDvrInfo->station_id = atoi(row[10]);
			}
			mysql_free_result(res) ;
		}

		return TRUE;
	}
	catch(...)
	{

	}
	return FALSE;
}

//设置站点摄像头DVR信息
BOOL CDlgSelectDevice::SetStationCameraDvrInfo(HTREEITEM hTreeItem)
{
	_T_NODE_INFO *pCameraNodeInfo = NULL;
	HTREEITEM hChildTreeItem = NULL;
	int i = 0;

	try
	{
		if (hTreeItem == NULL)
			return FALSE;

		pCameraNodeInfo = (_T_NODE_INFO *)m_treeDevice.GetItemData(hTreeItem);
		if (pCameraNodeInfo == NULL||pCameraNodeInfo->node_type != 2)
			return FALSE;

		_T_CAMERA_INFO * pCameraDvrInfo = NULL;
		_T_DVR_INFO * pDvrInfo = NULL;

		char szDvrType[32] = {0};

		g_nCameraDvrInfoCount = 0;
		memset(g_CameraDvrInfo,0,sizeof(g_CameraDvrInfo));

		//表示该节点为摄像头信息,获取数据库中的摄像头DVR信息
		char sql_buf[1024] = {0};

		MYSQL_RES * res = NULL;
		MYSQL_ROW	row;

		// 读取摄像头DVR信息
		sprintf_s(sql_buf, "SELECT t1.id,t1.dvr_type,t1.dvr_id,t1.dvr_ip,t1.link_port,t2.channel,t1.adminuser,t1.adminpasswd,t1.station_group,t1.station_name,t1.station_seq,t2.camera_id  "
			" FROM video_dvr AS t1,video_camera AS t2 WHERE t1.station_group='%s' AND t2.dvr_id=t1.dvr_id ORDER BY t1.id ASC", pCameraNodeInfo->node_num);

		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);
			while (row = mysql_fetch_row(res))
			{
				if (g_nCameraDvrInfoCount >= 512)
				{
					break;
				}

				pCameraDvrInfo = &g_CameraDvrInfo[g_nCameraDvrInfoCount];
				pDvrInfo = &pCameraDvrInfo->dvr_info;

				pDvrInfo->dvr_id = atoi(row[0]);
				strcpy_s(szDvrType,sizeof(szDvrType),row[1]);
				pDvrInfo->dvr_type = AppGetCameraDvrType(szDvrType);
				strcpy_s(pDvrInfo->dvr_num,sizeof(pDvrInfo->dvr_num),row[2]);
				strcpy_s(pDvrInfo->dvr_ip,sizeof(pDvrInfo->dvr_ip),row[3]);
				pDvrInfo->dvr_port = atoi(row[4]);
				pCameraDvrInfo->channel = atoi(row[5]);
				strcpy_s(pDvrInfo->dvr_user,sizeof(pDvrInfo->dvr_user),row[6]);
				strcpy_s(pDvrInfo->dvr_password,sizeof(pDvrInfo->dvr_password),row[7]);
				strcpy_s(pDvrInfo->station_num,sizeof(pDvrInfo->station_num),row[8]);
				strcpy_s(pDvrInfo->station_name,sizeof(pDvrInfo->station_name),row[9]);
				pDvrInfo->station_id = atoi(row[10]);
				strcpy_s(pCameraDvrInfo->camera_num,sizeof(pCameraDvrInfo->camera_num),row[11]);

				g_nCameraDvrInfoCount++;
			}
			mysql_free_result(res);
		}

		//设置摄像头DVR信息
		if (m_treeDevice.ItemHasChildren(hTreeItem) != FALSE)
		{
			hChildTreeItem = m_treeDevice.GetChildItem(hTreeItem);
			while(hChildTreeItem != NULL)
			{
				pCameraNodeInfo = (_T_NODE_INFO *)m_treeDevice.GetItemData(hChildTreeItem);
				if (pCameraNodeInfo != NULL&&pCameraNodeInfo->node_type == 3)
				{
					for (i = 0;i < g_nCameraDvrInfoCount;i++)
					{
						if (strcmp(pCameraNodeInfo->node_num,g_CameraDvrInfo[i].camera_num) == 0)
						{
							memcpy(&pCameraNodeInfo->camera_info,&g_CameraDvrInfo[i],sizeof(g_CameraDvrInfo[i]));
						}
					}
				}
				hChildTreeItem = m_treeDevice.GetNextSiblingItem(hChildTreeItem);
			}
		}

		return TRUE;
	}
	catch(...)
	{

	}
	return FALSE;
}

BOOL CDlgSelectDevice::OnEraseBkgnd(CDC* pDC)
{
	CRect   rect;
	GetClientRect(&rect);
	CDC   dcMem;

	dcMem.CreateCompatibleDC(pDC);   
	CBitmap*   pOldBitmap = dcMem.SelectObject(&m_Bmp);   

	pDC->StretchBlt(rect.left,
		rect.top,
		rect.right,
		rect.bottom,
		&dcMem,
		0,
		0,
		m_bm.bmWidth,
		m_bm.bmHeight,
		SRCCOPY);

	dcMem.SelectObject(pOldBitmap);

	return TRUE;
}

void CDlgSelectDevice::OnDestroy()
{
	CDialog::OnDestroy();

	//////////////////////////////////////////////////////////////////////////

}

LRESULT CDlgSelectDevice::OnNcHitTest(CPoint point)
{
	return HTCAPTION;
}

void CDlgSelectDevice::OnMenuitemSelectDeviceReflashlist()
{
	if (g_nUserResourceShowType == 1)
	{
		InitTreeDevice();
	}
	else if (g_nUserResourceShowType == 2)
	{
		InitTreeDeviceByCity();
	}
	else if (g_nUserResourceShowType == 3)
	{
		InitTreeDeviceByVoltage();
	}
}

void CDlgSelectDevice::OnNMRClickTreeDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu  popMenu;

	popMenu.LoadMenu(IDR_MENU_SELECT_DEVICE);
	CMenu *pMenu = popMenu.GetSubMenu(0); 

	CPoint posMouse;
	GetCursorPos(&posMouse);

	popMenu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_RIGHTALIGN,  posMouse.x, posMouse.y, this); 
	//////////////////////////////////////////////////////////////////////////
	*pResult = 0;
}
