// DlgLinkageConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "LinkageSetting.h"
#include "DlgLinkageConfig.h"
#include "DBExcute.h"
#include "DlgAddLinkageDev.h"
#include "PtuDlg.h"


// CDlgLinkageConfig 对话框

IMPLEMENT_DYNAMIC(CDlgLinkageConfig, CDialog)

CDlgLinkageConfig::CDlgLinkageConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLinkageConfig::IDD, pParent)
{
	memset(&m_tDevInfo, 0, sizeof(_T_DEV_INFO_));
	memset(&m_tLinkageConfigInfo, 0, sizeof(_T_LINK_CONFIG_INFO_));
	m_nIndex = 0;
}

CDlgLinkageConfig::~CDlgLinkageConfig()
{
}

void CDlgLinkageConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DEV, m_treeDev);
	DDX_Control(pDX, IDC_LIST_LINKAGE, m_lstLinkage);
}


BEGIN_MESSAGE_MAP(CDlgLinkageConfig, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_TREE_DEV, &CDlgLinkageConfig::OnNMClickTreeDev)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_DEV, &CDlgLinkageConfig::OnNMDblclkTreeDev)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LINKAGE, &CDlgLinkageConfig::OnNMRClickListLinkage)
	ON_COMMAND(ID_DELETE_LINKAGE_RECORD,&CDlgLinkageConfig::OnClickDeleteLinkageInRecord)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LINKAGE_DEV, &CDlgLinkageConfig::OnBnClickedButtonAddLinkageDev)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgLinkageConfig 消息处理程序

void CDlgLinkageConfig::OnNMClickTreeDev(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CDlgLinkageConfig::FreeLinkageList()
{
	int nNum = m_lstLinkage.GetItemCount();
	for (int i=0; i<nNum; i++)
	{
		_T_LINK_CONFIG_INFO_* pDeviceInfo = (_T_LINK_CONFIG_INFO_*)m_lstLinkage.GetItemData(i);
		delete pDeviceInfo;
		pDeviceInfo = NULL;
		m_lstLinkage.SetItemData(i,(DWORD_PTR)pDeviceInfo);
	}
	m_lstLinkage.DeleteAllItems();
}

void CDlgLinkageConfig::SetLikageConfigListInfo(_T_DEV_INFO_ *pDevInfo)
{
	//清空联动配置列表
	FreeLinkageList();

	char sql_buf[1024] = {0};
	MYSQL_RES* res;
	MYSQL_ROW row;
	int nIndex = 0;

	if (pDevInfo == NULL)
		return;

	sprintf_s(sql_buf,"SELECT a.f_id,a.f_dev_id,a.f_relate_dev_id,a.f_relate_camera_code,a.f_relate_dev_type, "
		" a.f_relate_dev_action,a.f_preset_num,b.name AS dev_name,c.name AS relate_dev_name,d.preset_name,e.name AS camera_name "
		" FROM t_ass_linkage_relation AS a "
		" left Join ass_rvu_sm AS b ON a.f_dev_id = b.id left Join ass_rvu_sm AS c ON a.f_relate_dev_id = c.id "
		" left Join ass_camerapreset AS d ON a.f_relate_camera_code = d.camera_code AND a.f_preset_num = d.camera_preset_id "
		" Left Join video_camera AS e ON a.f_relate_camera_code = e.camera_id " 
		" where a.f_dev_id=%d", pDevInfo->nDevId);

	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			_T_LINK_CONFIG_INFO_ *pLinkageConfigInfo = new _T_LINK_CONFIG_INFO_;
			pLinkageConfigInfo->nId = atoi(row[0]);
			pLinkageConfigInfo->nDevId = atoi(row[1]);
			pLinkageConfigInfo->nRelateDevId = atoi(row[2]);
			sprintf_s(pLinkageConfigInfo->szRelateCameraCode,"%s",row[3]);
			pLinkageConfigInfo->nRelateDevType = atoi(row[4]);
			pLinkageConfigInfo->nRelateDevAction = atoi(row[5]);
			pLinkageConfigInfo->nPresetNum = atoi(row[6]);
			sprintf_s(pLinkageConfigInfo->szDevName,"%s",row[7]);
			sprintf_s(pLinkageConfigInfo->szRelateDevName,"%s",row[8]);
			sprintf_s(pLinkageConfigInfo->szPresetName,"%s",row[9]);
			sprintf_s(pLinkageConfigInfo->szRelateCameraName,"%s",row[10]);

			m_lstLinkage.InsertItem(nIndex,pLinkageConfigInfo->szDevName);

			char szLinkageTypeText[32]={0};
			GetLinkageTypeText(szLinkageTypeText,pLinkageConfigInfo->nRelateDevType);
			m_lstLinkage.SetItemText(nIndex,1,szLinkageTypeText);

			m_lstLinkage.SetItemText(nIndex,2,pLinkageConfigInfo->szRelateDevName);
			m_lstLinkage.SetItemText(nIndex,3,pLinkageConfigInfo->szRelateCameraName);

			char szLinkageActionText[32]={0};
			GetLinkageActionText(szLinkageActionText,pLinkageConfigInfo->nRelateDevAction);
			m_lstLinkage.SetItemText(nIndex,4,szLinkageActionText);

			m_lstLinkage.SetItemText(nIndex,5,row[6]);
			m_lstLinkage.SetItemText(nIndex,6,pLinkageConfigInfo->szPresetName);

			m_lstLinkage.SetItemData(nIndex,(DWORD_PTR)pLinkageConfigInfo);

			nIndex++;
		}
		mysql_free_result(res);
	}

}

void CDlgLinkageConfig::GetLinkageTypeText(char *szLinkageTypeText,int nRelateDevType)
{
	if (nRelateDevType == 1)
	{
		sprintf_s(szLinkageTypeText,32,"门禁");
	}

	if (nRelateDevType == 2)
	{
		sprintf_s(szLinkageTypeText,32,"摄像头");
	}
}

void CDlgLinkageConfig::GetLinkageActionText(char *szLinkageActionText,int nRelateDevAction)
{
	if (nRelateDevAction == 1)
	{
		sprintf_s(szLinkageActionText,32,"打开");
	}

	if (nRelateDevAction == 2)
	{
		sprintf_s(szLinkageActionText,32,"关闭");
	}

	if (nRelateDevAction == 3)
	{
		sprintf_s(szLinkageActionText,32,"转预置位");
	}
}


BOOL CDlgLinkageConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgLinkageConfig::InitControl()
{
	DWORD dwStyle;
	dwStyle = GetStyle();
	dwStyle = LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT ;

	m_lstLinkage.SetExtendedStyle(dwStyle);
	m_lstLinkage.SetBkColor(RGB(239,246,253));
	m_lstLinkage.SetTextBkColor(RGB(0xfe,0xFF,0xc6));

	CRect winrc;
	int nWidth = GetSystemMetrics(SM_CXSCREEN)*0.4;

	m_lstLinkage.DeleteAllItems();
	while(m_lstLinkage.DeleteColumn(0));

	m_lstLinkage.InsertColumn(0,"设备名称");
	m_lstLinkage.SetColumnWidth(0,nWidth*0.1);
	m_lstLinkage.InsertColumn(1,"联动设备类型");
	m_lstLinkage.SetColumnWidth(1,nWidth*0.1);
	m_lstLinkage.InsertColumn(2,"联动设备名称");
	m_lstLinkage.SetColumnWidth(2,nWidth*0.2);
	m_lstLinkage.InsertColumn(3,"联动摄像头名称");
	m_lstLinkage.SetColumnWidth(3,nWidth*0.2);
	m_lstLinkage.InsertColumn(4,"联动动作");
	m_lstLinkage.SetColumnWidth(4,nWidth*0.1);
	m_lstLinkage.InsertColumn(5,"预置位编号");
	m_lstLinkage.SetColumnWidth(5,nWidth*0.1);
	m_lstLinkage.InsertColumn(6,"预置位名称");
	m_lstLinkage.SetColumnWidth(6,nWidth*0.2);
}

void CDlgLinkageConfig::OnNMDblclkTreeDev(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hItem = m_treeDev.GetSelectedItem();
	if (hItem == NULL)
		return ;

	HTREEITEM hChildItem = m_treeDev.GetChildItem(hItem);
	if (hChildItem != NULL)
		return;

	_T_DEV_INFO_ *pDevInfo = (_T_DEV_INFO_*)m_treeDev.GetItemData(hItem);
	if (pDevInfo->nDevId == m_tDevInfo.nDevId)
		return;

	//设置设备成员变量为当前选中的设备
	memcpy(&m_tDevInfo, pDevInfo, sizeof(_T_DEV_INFO_));

	//从数据库中取出当前设备的联动设备显示到联动配置列表中
	SetLikageConfigListInfo(&m_tDevInfo);

	*pResult = 0;
}

void CDlgLinkageConfig::OnNMRClickListLinkage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	POSITION pos = m_lstLinkage.GetFirstSelectedItemPosition();

	if (pos == NULL)
		return;

	int nIndex = m_lstLinkage.GetNextSelectedItem(pos);

	if (nIndex < 0)
		return;

	_T_LINK_CONFIG_INFO_* pLinkageConfigInfo = (_T_LINK_CONFIG_INFO_*)m_lstLinkage.GetItemData(nIndex);
	if (pLinkageConfigInfo != NULL)
		memcpy(&m_tLinkageConfigInfo, pLinkageConfigInfo, sizeof(_T_LINK_CONFIG_INFO_));
	m_nIndex = nIndex;

	//弹出菜单
	PopRightMenu();

	*pResult = 0;
}

void CDlgLinkageConfig::PopRightMenu()
{
	CMenu menuMain;
	menuMain.CreatePopupMenu();

	menuMain.AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,ID_DELETE_LINKAGE_RECORD,"删除记录");

	CPoint point;
	GetCursorPos(&point);

	menuMain.TrackPopupMenu(TPM_RIGHTBUTTON,point.x,point.y,this);
}

void CDlgLinkageConfig::OnClickDeleteLinkageInRecord()
{
	//从数据库删除该记录
	DeleteLinkageRecordFromDB(m_tLinkageConfigInfo.nId);

	//从列表中清除该条记录
	DeleteLinkageRecordFromList(m_nIndex);
}

bool CDlgLinkageConfig::DeleteLinkageRecordFromDB(int nId)
{
	char sql_buf[1024] = {0};
	//拼sql语句
	sprintf_s(sql_buf, "Delete from t_ass_linkage_relation where f_id=%d",nId);

	if (mysql_query(g_LinkageSettingMysql, sql_buf))
	{
		return false;
	}
}

void CDlgLinkageConfig::DeleteLinkageRecordFromList(int nIndex)
{
	_T_LINK_CONFIG_INFO_* pLinkageConfigInfo = (_T_LINK_CONFIG_INFO_*)m_lstLinkage.GetItemData(nIndex);
	if (pLinkageConfigInfo != NULL)
	{
		delete pLinkageConfigInfo;
		pLinkageConfigInfo = NULL;
		m_lstLinkage.SetItemData(nIndex,(DWORD_PTR)pLinkageConfigInfo);
	}
	m_lstLinkage.DeleteItem(nIndex);
}
void CDlgLinkageConfig::OnBnClickedButtonAddLinkageDev()
{
	if (m_tDevInfo.nDevId == 0)
	{
		MessageBox("请先选择设备！");
		return;
	}

	CDlgAddLinkageDev dlgAddLinkageDev(this,&m_tDevInfo);
	if (dlgAddLinkageDev.DoModal() == IDOK)
	{
		SetLikageConfigListInfo(&m_tDevInfo);
		return;
	}
	
}

void CDlgLinkageConfig::SetStationNode(StationNode* pStationNode)
{
	m_pStationNode = pStationNode;
}

void CDlgLinkageConfig::RefreshLinkageConfigDlg(StationNode* pNodeInfo)
{
	InitDevTree(pNodeInfo);

	FreeLinkageList();

	memset(&m_tDevInfo,0,sizeof(_T_DEV_INFO_));

	m_nIndex = 0;

	memset(&m_tLinkageConfigInfo,0,sizeof(_T_LINK_CONFIG_INFO_));
}

void CDlgLinkageConfig::InitDevTree(StationNode* pNodeInfo)
{
	//清空列表
	FreeAllTreeItem(&m_treeDev,NULL);
	//添加变电站根结点
	HTREEITEM hMainItem = NULL;
	HTREEITEM hItemSubSystem = NULL,hItemDevice = NULL;
	hMainItem = m_treeDev.InsertItem(pNodeInfo->station_name, 8, 8, TVI_ROOT, NULL);
	SetTreeNodeInfo(&m_treeDev,hMainItem,pNodeInfo->station_id,STATION_NODE,pNodeInfo->station_name,0,NULL);

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
				hItemDevice = m_treeDev.InsertItem(g_tAssRVUDevice[j].node_name,10,10,hMainItem);
				SetTreeNodeInfo(&m_treeDev,hItemDevice,g_tAssRVUDevice[j].id,RVU_DEVICE_NODE,g_tAssRVUDevice[j].node_name,pNodeInfo->station_id,NULL);
			}	
		}
	}


	ExpandAllItem(&m_treeDev,NULL);
}

void CDlgLinkageConfig::ExpandAllItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem)
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

void CDlgLinkageConfig::SetTreeNodeInfo(CTreeCtrl* pTreeList,HTREEITEM hItem,int nNodeId,int nNodeType,char* szNodeName,int nParentId,char* szNodeCode)
{
	_T_DEV_INFO_* pDevInfo = NULL;
	pDevInfo = new _T_DEV_INFO_;
	memset(pDevInfo,0,sizeof(_T_DEV_INFO_));

	pDevInfo->nDevId = nNodeId;
	sprintf_s(pDevInfo->szDevName,"%s",szNodeName);
	pDevInfo->nStationId = nParentId;

	pTreeList->SetItemData(hItem,(DWORD)pDevInfo);
}

void CDlgLinkageConfig::FreeAllTreeItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem)
{
	_T_DEV_INFO_* pNodeInfo = NULL;
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
		pNodeInfo = (_T_DEV_INFO_*)pTreeList->GetItemData(hTreeItem);
		if (pNodeInfo != NULL)
		{
			delete pNodeInfo;
			pNodeInfo = NULL;
			pTreeList->SetItemData(hTreeItem,NULL);
		}
		pTreeList->DeleteItem(hTreeItem);
	}
}

void CDlgLinkageConfig::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CWnd* pWnd = NULL;
	CRect winrc,button_rect,list_rect,tree_rect;

	GetClientRect(&winrc);

	int nWidth = winrc.right - winrc.left - 20;
	int nHeight = winrc.bottom - winrc.top - 30;
	
	int nTreeWidth = nWidth * 0.2;
	int nListWidth = nWidth * 0.8;

	int nButtonHeight = nHeight * 0.05;
	int nTreeHeight = nHeight * 0.95;
	int nListHeight = nHeight * 0.95;

	pWnd = GetDlgItem(IDC_STATIC_DEV_TREE);
	if (pWnd != NULL)
	{
		button_rect.top = winrc.top + 10;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = winrc.left + 10;
		button_rect.right = button_rect.left + nTreeWidth;
		pWnd->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_TREE_DEV);
	if (pWnd != NULL)
	{
		tree_rect.top = button_rect.bottom;
		tree_rect.bottom = tree_rect.top + nTreeHeight - 10;
		tree_rect.left = winrc.left + 10;
		tree_rect.right = tree_rect.left + nTreeWidth;
		pWnd->MoveWindow(tree_rect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_ADD_LINKAGE_DEV);
	if (pWnd != NULL)
	{
		button_rect.top = winrc.top + 10;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = tree_rect.right + 10;
		button_rect.right = button_rect.left + nTreeWidth;
		pWnd->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_LIST_LINKAGE);
	if (pWnd != NULL)
	{
		list_rect.top = button_rect.bottom;
		list_rect.bottom = list_rect.top + nListHeight;
		list_rect.left = tree_rect.right + 10;
		list_rect.right = list_rect.left + nListWidth;
		pWnd->MoveWindow(list_rect);
	}
}
