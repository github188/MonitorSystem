// DlgDeviceLedger.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"
#include "DlgDeviceLedger.h"

CDlgDeviceLedger *g_pDlgDeviceLedger = NULL;


// CDlgDeviceLedger 对话框

IMPLEMENT_DYNAMIC(CDlgDeviceLedger, CDialog)

CDlgDeviceLedger::CDlgDeviceLedger(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDeviceLedger::IDD, pParent)
{

}

CDlgDeviceLedger::~CDlgDeviceLedger()
{
}

void CDlgDeviceLedger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DEVICE_LEDGER, m_lstDeviceLedger);
	DDX_Control(pDX, IDC_DEVICE_LEDGER_STATION_TREE, m_treeDeviceLedgerStation);
	DDX_Control(pDX, IDC_BUTTON_DEVICE_LEDGER_QUERY, m_btnDeviceLedger);
}


BEGIN_MESSAGE_MAP(CDlgDeviceLedger, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEVICE_LEDGER_QUERY, &CDlgDeviceLedger::OnBnClickedButtonDeviceLedgerQuery)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_STATION, &CDlgDeviceLedger::OnBnClickedButtonSelectStation)
	ON_BN_CLICKED(IDC_BUTTON_DEVICE_LEDGER_EXPORT, &CDlgDeviceLedger::OnBnClickedButtonDeviceLedgerExport)
	ON_BN_CLICKED(IDC_BUTTON_DEVICE_LEDGER_STATISTICAL, &CDlgDeviceLedger::OnBnClickedButtonDeviceLedgerStatistical)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgDeviceLedger 消息处理程序

//拼出选择的变电站的条件
CString CDlgDeviceLedger::GetQueryCondition(std::vector<HTREEITEM> vecSelectedItems)
{
	_T_NODE_INFO* pNodeInfo = NULL;
	CString strCondition = "where 1=2";

	for (size_t t = 0; t < vecSelectedItems.size(); t++)
	{
		HTREEITEM item = vecSelectedItems.at(t);
		pNodeInfo = (_T_NODE_INFO*)m_treeDeviceLedgerStation.GetItemData(item);

		//如果是变电站
		if (pNodeInfo->node_type == 2)
		{
			int nStationId = pNodeInfo->node_id;
			CString strStationIdText;
			strStationIdText.Format(" or a.station_id=%d",nStationId);
			strCondition += strStationIdText;
		}
	}

	return strCondition;
}

void CDlgDeviceLedger::AddDeviceLedgersToList(std::vector<_T_ASS_DEVICE_LEDGER_> vecDeviceLedgers)
{
	int nIndex = 0;
	for (size_t t = 0; t < vecDeviceLedgers.size(); t++)
	{
		CString strIndex = "";
		strIndex.Format("%d",nIndex);
		m_lstDeviceLedger.InsertItem(nIndex,strIndex);

		m_lstDeviceLedger.SetItemText(nIndex,1,vecDeviceLedgers.at(t).szStationName);
		m_lstDeviceLedger.SetItemText(nIndex,2,vecDeviceLedgers.at(t).device_name);
		m_lstDeviceLedger.SetItemText(nIndex,3,vecDeviceLedgers.at(t).device_manufacturer);
		m_lstDeviceLedger.SetItemText(nIndex,4,vecDeviceLedgers.at(t).device_ip);
		m_lstDeviceLedger.SetItemText(nIndex,5,IntToCString(vecDeviceLedgers.at(t).device_port));
		m_lstDeviceLedger.SetItemText(nIndex,6,vecDeviceLedgers.at(t).login_username);
		m_lstDeviceLedger.SetItemText(nIndex,7,vecDeviceLedgers.at(t).login_password);
		m_lstDeviceLedger.SetItemText(nIndex,8,vecDeviceLedgers.at(t).protocol_type);
		m_lstDeviceLedger.SetItemText(nIndex,9,vecDeviceLedgers.at(t).device_type);
		m_lstDeviceLedger.SetItemText(nIndex,10,vecDeviceLedgers.at(t).harddisk_capacity);
		m_lstDeviceLedger.SetItemText(nIndex,11,IntToCString(vecDeviceLedgers.at(t).analog_video_num));
		m_lstDeviceLedger.SetItemText(nIndex,12,IntToCString(vecDeviceLedgers.at(t).net_video_num));

		nIndex++;
	}

	if (nIndex == 0)
	{
		m_lstDeviceLedger.InsertItem(nIndex,"无数据");

		m_lstDeviceLedger.SetItemText(nIndex,1,"无数据");
		m_lstDeviceLedger.SetItemText(nIndex,2,"无数据");
		m_lstDeviceLedger.SetItemText(nIndex,3,"无数据");
		m_lstDeviceLedger.SetItemText(nIndex,4,"无数据");
		m_lstDeviceLedger.SetItemText(nIndex,5,"无数据");
		m_lstDeviceLedger.SetItemText(nIndex,6,"无数据");
		m_lstDeviceLedger.SetItemText(nIndex,7,"无数据");
		m_lstDeviceLedger.SetItemText(nIndex,8,"无数据");
		m_lstDeviceLedger.SetItemText(nIndex,9,"无数据");
		m_lstDeviceLedger.SetItemText(nIndex,10,"无数据");
		m_lstDeviceLedger.SetItemText(nIndex,11,"无数据");
		m_lstDeviceLedger.SetItemText(nIndex,12,"无数据");
	}
}

void CDlgDeviceLedger::OnBnClickedButtonDeviceLedgerQuery()
{
	//清空之前的数据
	m_lstDeviceLedger.DeleteAllItems();

	//获取所有选中的变电站
	std::vector<HTREEITEM> vecSelectedStationItems;
	m_treeDeviceLedgerStation.GetCheckItems(vecSelectedStationItems, m_treeDeviceLedgerStation.GetRootItem());

	//拼接出查询条件
	CString strCondition = GetQueryCondition(vecSelectedStationItems);

	//查询数据库
	std::vector<_T_ASS_DEVICE_LEDGER_> vecDeviceLedgers = GetDeviceLedgersFromDB(strCondition);

	//添加到列表
	AddDeviceLedgersToList(vecDeviceLedgers);
}

BOOL CDlgDeviceLedger::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControl();

	InitStationTree(&m_treeDeviceLedgerStation);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDeviceLedger::InitStationTree(CTreeCtrl *pTreeCtrl)
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
		for (i = 0; i < g_TotalSysNum; i ++)
		{
			if (g_SystemInfo[i].localdomain == 1)
			{
				hMainItem = pTreeCtrl->InsertItem("全部", 1, 1, TVI_ROOT, hMainItem);
				SetCameraNodeInfo(pTreeCtrl,
					hMainItem,
					"",
					"",
					0,
					1,
					0,
					0,
					0,
					0,
					0,
					"");
				break;
			}
		}

		//添加变电站中间节点和站点节点
		for (i = 0; i < g_nStationNode_Num; i ++)
		{
			if (g_tStationNode_Info[i].parent_id == g_tStationNode_Info[i].node_id&&g_tStationNode_Info[i].login_type == 0)	//第一级中间节点--500kV,220kV,110kV等
			{
				//if (strcmp(g_tStationNode_Info[i].node_name,"220kV") == 0)//不显示220kV
				//	continue;

				//if (strcmp(g_tStationNode_Info[i].node_name,"110kV") == 0)//不显示110kV
				//	continue;

				if (strcmp(g_tStationNode_Info[i].node_name,"500kV") == 0)//不显示下面的市级结点，直接显示变电站
					b500kVFlag = TRUE;
				else
					b500kVFlag = FALSE;

				hChildItem = pTreeCtrl->InsertItem(g_tStationNode_Info[i].node_name, 7, 7, hMainItem);
				SetCameraNodeInfo(pTreeCtrl,
					hChildItem,
					"",
					"",
					0,
					1,
					0,
					0,
					0,
					0,
					1,
					"");

				for (ns = 0; ns < g_nStation_Num; ns ++)
				{
					if (g_tStation_Info[ns].node_id == g_tStationNode_Info[i].node_id)
					{
						if (strlen(g_tStation_Info[ns].notes) != 0)
							sprintf_s(szStationShowName,sizeof(szStationShowName),"%s(%s)",g_tStation_Info[ns].station_name_videoplant,g_tStation_Info[ns].notes);
						else
							strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

						nStationStatus = CheckStationCurrentStatus2(g_tStation_Info[ns].station_id);
						if (nStationStatus != 0)
							hStationItem = pTreeCtrl->InsertItem(szStationShowName, 8, 8, hChildItem);
						else
							hStationItem = pTreeCtrl->InsertItem(szStationShowName, 10, 10, hChildItem);

						SetCameraNodeInfo(pTreeCtrl,
							hStationItem,
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

				for (j = 0; j < g_nStationNode_Num; j ++)//第二级中间节点--南京市、苏州市
				{
					if (g_tStationNode_Info[j].parent_id == g_tStationNode_Info[i].node_id && g_tStationNode_Info[j].parent_id != g_tStationNode_Info[j].node_id)	// 第二季中间节点
					{
						if (b500kVFlag == FALSE)
							hChildItem1 = pTreeCtrl->InsertItem(g_tStationNode_Info[j].node_name, 7, 7, hChildItem);
						else
							hChildItem1 = hChildItem;

						SetCameraNodeInfo(pTreeCtrl,
							hChildItem1,
							"",
							"",
							0,
							1,
							0,
							0,
							0,
							0,
							1,
							"");

						for (ns = 0; ns < g_nStation_Num; ns ++)
						{
							if (g_tStation_Info[ns].node_id == g_tStationNode_Info[j].node_id)
							{
								if (strlen(g_tStation_Info[ns].notes) != 0)
									sprintf_s(szStationShowName,sizeof(szStationShowName),"%s(%s)",g_tStation_Info[ns].station_name_videoplant,g_tStation_Info[ns].notes);
								else
									strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

								nStationStatus = CheckStationCurrentStatus2(g_tStation_Info[ns].station_id);
								if (nStationStatus != 0)
									hStationItem = pTreeCtrl->InsertItem(szStationShowName, 8, 8, hChildItem1);
								else
									hStationItem = pTreeCtrl->InsertItem(szStationShowName, 10, 10, hChildItem1);

								SetCameraNodeInfo(pTreeCtrl,
									hStationItem,
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
								hChildItem2 = pTreeCtrl->InsertItem(g_tStationNode_Info[k].node_name, 7, 7, hChildItem1);
								SetCameraNodeInfo(pTreeCtrl,
									hChildItem2,
									"",
									"",
									0,
									1,
									0,
									0,
									0,
									0,
									1,
									"");

								for (ns = 0; ns < g_nStation_Num; ns ++)
								{
									if (g_tStation_Info[ns].node_id == g_tStationNode_Info[k].node_id)
									{
										if (strlen(g_tStation_Info[ns].notes) != 0)
											sprintf_s(szStationShowName,sizeof(szStationShowName),"%s(%s)",g_tStation_Info[ns].station_name_videoplant,g_tStation_Info[ns].notes);
										else
											strcpy_s(szStationShowName,sizeof(szStationShowName),g_tStation_Info[ns].station_name_videoplant);

										nStationStatus = CheckStationCurrentStatus2(g_tStation_Info[ns].station_id);
										if (nStationStatus != 0)
											hStationItem = pTreeCtrl->InsertItem(szStationShowName, 8, 8, hChildItem2);
										else
											hStationItem = pTreeCtrl->InsertItem(szStationShowName, 10, 10, hChildItem2);

										SetCameraNodeInfo(pTreeCtrl,
											hStationItem,
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

		hMainItem = pTreeCtrl->GetRootItem();
		if (hMainItem != NULL)
		{
			hChildItem = pTreeCtrl->GetChildItem(hMainItem);
			if (hChildItem != NULL)
			{
				pTreeCtrl->SortChildren(hChildItem);//排序
				hChildItem1 = pTreeCtrl->GetChildItem(hChildItem);
				if (hChildItem1 != NULL)
				{
					pTreeCtrl->EnsureVisible(hChildItem1);
				}
			}
		}

		hChildItem = pTreeCtrl->GetNextSiblingItem(hChildItem);
		while(hChildItem != NULL)
		{
			pTreeCtrl->SortChildren(hChildItem);//排序
			hChildItem1 = pTreeCtrl->GetChildItem(hChildItem);
			while(hChildItem1 != NULL)
			{
				pTreeCtrl->SortChildren(hChildItem1);
				hChildItem1 = pTreeCtrl->GetNextSiblingItem(hChildItem1);
			}
			hChildItem = pTreeCtrl->GetNextSiblingItem(hChildItem);
		}
	}
	catch(...)
	{

	}
}

void CDlgDeviceLedger::SetCameraNodeInfo(CTreeCtrl *pTreeCtrl, HTREEITEM hItem, char* ItemName, char* Itemnum, int StationOrCameraID,int ItemStatus,int Streamless,float ItemLongitude, float ItemLatitude, int ItemDecodeTag, int Itemtype, char* stationname)
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

		if (Itemtype == 2)//站点
		{
			//记录第一个站点名称
			if (strlen(g_VideoPageViewInfo.szFirstStationName) == 0)
			{
				strcpy_s(g_VideoPageViewInfo.szFirstStationName,sizeof(g_VideoPageViewInfo.szFirstStationName)-1,ItemName);
			}
		}

		pNodeInfo->node_decodetag = ItemDecodeTag;
		sprintf(pNodeInfo->node_station, "%s", stationname);

		pTreeCtrl->SetItemData(hItem, (DWORD_PTR)pNodeInfo);
	}
	catch(...)
	{

	}
}

int CDlgDeviceLedger::CheckStationCurrentStatus2(int nStationId)
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

//删除摄像头树的子节点信息
void CDlgDeviceLedger::DeleteCameraChildTreeListInfo(CTreeCtrl *pTreeCtrl, HTREEITEM hItem)
{
	HTREEITEM	hChildItem = NULL;
	_T_NODE_INFO *pNodeInfo = NULL;
	DWORD_PTR pInfo = NULL;

	try
	{
		hChildItem = pTreeCtrl->GetChildItem(hItem);
		while(hChildItem != NULL)
		{
			pInfo = pTreeCtrl->GetItemData(hChildItem);
			if (pInfo != NULL)
			{
				pNodeInfo = (_T_NODE_INFO *)pInfo;
				delete pNodeInfo;
				pInfo = NULL;
				pNodeInfo = NULL;
				pTreeCtrl->SetItemData(hChildItem,NULL);
			}

			DeleteCameraChildTreeListInfo(pTreeCtrl, hChildItem);
			hChildItem = pTreeCtrl->GetNextSiblingItem(hChildItem);
		}
	}
	catch(...)
	{

	}
}

//删除摄像头列表树
void CDlgDeviceLedger::DeleteCameraTreeList(CTreeCtrl *pTreeCtrl)
{
	HTREEITEM	hMainItem = NULL;
	_T_NODE_INFO *pNodeInfo = NULL;
	DWORD_PTR pInfo = NULL;

	try
	{
		//树形列表
		hMainItem = pTreeCtrl->GetRootItem();
		while(hMainItem != NULL)
		{
			pInfo = pTreeCtrl->GetItemData(hMainItem);
			if (pInfo != NULL)
			{
				pNodeInfo = (_T_NODE_INFO *)pInfo;
				delete pNodeInfo;
				pInfo = NULL;
				pNodeInfo = NULL;
				pTreeCtrl->SetItemData(hMainItem,NULL);
			}

			DeleteCameraChildTreeListInfo(pTreeCtrl, hMainItem);
			hMainItem = pTreeCtrl->GetNextSiblingItem(hMainItem);
		}
	}
	catch(...)
	{

	}

	pTreeCtrl->DeleteAllItems();
}

void CDlgDeviceLedger::InitControl()
{
	m_imageList.Create(IDB_BITMAP_TREE,16,0,RGB(255,0,255));
	m_treeDeviceLedgerStation.SetImageList(&m_imageList, TVSIL_NORMAL);

	DWORD dwStyle;
	dwStyle = m_lstDeviceLedger.GetStyle();
	dwStyle = LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT ;

	m_lstDeviceLedger.SetExtendedStyle(dwStyle);
	m_lstDeviceLedger.SetBkColor(RGB(239,246,253));
	m_lstDeviceLedger.SetTextBkColor(RGB(0xfe,0xFF,0xc6));

	m_lstDeviceLedger.DeleteAllItems();
	while(m_lstDeviceLedger.DeleteColumn(0));

	CRect rect;
	GetWindowRect(&rect);

	int nWidth = rect.right - rect.left;

	m_lstDeviceLedger.InsertColumn(0,"序号");
	m_lstDeviceLedger.SetColumnWidth(0,nWidth*0.05);
	m_lstDeviceLedger.InsertColumn(1,"设备名称");
	m_lstDeviceLedger.SetColumnWidth(1,nWidth*0.1);
	m_lstDeviceLedger.InsertColumn(2,"设备厂商");
	m_lstDeviceLedger.SetColumnWidth(2,nWidth*0.1);
	m_lstDeviceLedger.InsertColumn(3,"设备IP");
	m_lstDeviceLedger.SetColumnWidth(3,nWidth*0.1);
	m_lstDeviceLedger.InsertColumn(4,"设备端口");
	m_lstDeviceLedger.SetColumnWidth(4,nWidth*0.1);
	m_lstDeviceLedger.InsertColumn(5,"登陆用户名");
	m_lstDeviceLedger.SetColumnWidth(5,nWidth*0.1);
	m_lstDeviceLedger.InsertColumn(6,"登陆密码");
	m_lstDeviceLedger.SetColumnWidth(6,nWidth*0.1);
	m_lstDeviceLedger.InsertColumn(7,"协议类型");
	m_lstDeviceLedger.SetColumnWidth(7,nWidth*0.1);
	m_lstDeviceLedger.InsertColumn(8,"设备类型");
	m_lstDeviceLedger.SetColumnWidth(8,nWidth*0.1);
	m_lstDeviceLedger.InsertColumn(9,"硬盘容量");
	m_lstDeviceLedger.SetColumnWidth(9,nWidth*0.1);
	m_lstDeviceLedger.InsertColumn(10,"模拟视频数量");
	m_lstDeviceLedger.SetColumnWidth(10,nWidth*0.1);
	m_lstDeviceLedger.InsertColumn(11,"网络视频数量");
	m_lstDeviceLedger.SetColumnWidth(11,nWidth*0.1);

	InitButtonColor(GetDlgItem(IDC_BUTTON_DEVICE_LEDGER_QUERY));
}
void CDlgDeviceLedger::OnBnClickedButtonSelectStation()
{
	
}

void CDlgDeviceLedger::OnBnClickedButtonDeviceLedgerExport()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgDeviceLedger::OnBnClickedButtonDeviceLedgerStatistical()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgDeviceLedger::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect winrc,static_rect,list_rect;
	GetClientRect(&winrc);

	int nWidth = cx;
	int nHeight = cy;

	int nStaticStationTextHeight = 30;
	int nStaticStationTextWidth = cx * 0.15;

	int nStationTreeWidth = cx * 0.15;
	int nStationTreeHeight = cy - nStaticStationTextHeight;

	int nButtonHeight = 30;
	int nButtonWidth = 100;

	int nDeviceListWidth = cx * 0.85;
	int nDeviceListHeight = cy - nButtonHeight;

	int nJianXi = 7;//两边的间隙

	CWnd* pWnd = NULL;
	//变电站文本
	//pWnd = GetDlgItem(IDC_STATIC_DEVICE_LEDGER_STATION);
	//if (pWnd != NULL)
	//{
	//	static_rect.top = winrc.top;
	//	static_rect.bottom = static_rect.top + nStaticStationTextHeight;
	//	static_rect.left = winrc.left;
	//	static_rect.right = static_rect.left + nStaticStationTextWidth;
	//	pWnd->MoveWindow(static_rect);
	//}

	//变电站树
	pWnd = GetDlgItem(IDC_DEVICE_LEDGER_STATION_TREE);
	if (pWnd != NULL)
	{
		static_rect.top = winrc.top;
		static_rect.bottom = static_rect.top + nStationTreeHeight;
		static_rect.left = winrc.left;
		static_rect.right = static_rect.left + nStationTreeWidth;
		pWnd->MoveWindow(static_rect);
	}

	//查询按钮
	pWnd = GetDlgItem(IDC_BUTTON_DEVICE_LEDGER_QUERY);
	if (pWnd != NULL)
	{
		static_rect.top = winrc.top;
		static_rect.bottom = static_rect.top + nButtonHeight;
		static_rect.left = static_rect.right + nJianXi;
		static_rect.right = static_rect.left + nButtonWidth;
		pWnd->MoveWindow(static_rect);
	}

	//列表
	pWnd = GetDlgItem(IDC_LIST_DEVICE_LEDGER);
	if (pWnd != NULL)
	{
		static_rect.top = static_rect.bottom;
		static_rect.bottom = static_rect.top + nDeviceListHeight;
		static_rect.left = static_rect.left;
		static_rect.right = static_rect.left + nDeviceListWidth;
		pWnd->MoveWindow(static_rect);
	}


}

BOOL CDlgDeviceLedger::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}
