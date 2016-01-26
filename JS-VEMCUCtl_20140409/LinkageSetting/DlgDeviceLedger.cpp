// DlgDeviceLedger.cpp : 实现文件
//

#include "stdafx.h"
#include "LinkageSetting.h"
#include "DlgDeviceLedger.h"
#include "DBExcute.h"
#include "DlgAddDeviceLedger.h"
#include "PtuDlg.h"
#include "ListControlOperate.h"


// CDlgDeviceLedger 对话框

IMPLEMENT_DYNAMIC(CDlgDeviceLedger, CDialog)

CDlgDeviceLedger::CDlgDeviceLedger(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDeviceLedger::IDD, pParent)
{
	m_pStationNode = NULL;
	m_pAddDeviceLedger = NULL;
}

CDlgDeviceLedger::~CDlgDeviceLedger()
{
	if (m_pAddDeviceLedger != NULL)
	{
		delete m_pAddDeviceLedger;
		m_pAddDeviceLedger = NULL;
	}
}

void CDlgDeviceLedger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DEVICE_LIST, m_treeDeviceList);
	DDX_Control(pDX, IDC_LIST_DEVICE_LEDGER, m_lstDeviceLedger);
}

void CDlgDeviceLedger::SetStationNode(StationNode* pStationNode)
{
	m_pStationNode = pStationNode;
}


BEGIN_MESSAGE_MAP(CDlgDeviceLedger, CDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_ADD_DEVICE, &CDlgDeviceLedger::OnBnClickedButtonAddDevice)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_DEVICE_LIST, &CDlgDeviceLedger::OnNMDblclkTreeDeviceList)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_DEVICE, &CDlgDeviceLedger::OnBnClickedButtonEditDevice)
	ON_BN_CLICKED(IDC_BUTTON_DEL_DEVICE, &CDlgDeviceLedger::OnBnClickedButtonDelDevice)
	ON_MESSAGE(WM_ADD_DEVICE_LEDGER_COMPLETED, &CDlgDeviceLedger::OnAddDeviceLedgerCompletedHandler)
END_MESSAGE_MAP()


// CDlgDeviceLedger 消息处理程序

BOOL CDlgDeviceLedger::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pBackBmp = new CBitmap;
	m_pBackBmp->LoadBitmap(IDB_BITMAP_BACK);

	InitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDeviceLedger::InitCtrl()
{
	InitDeviceLedgerListColumn(&m_lstDeviceLedger);
}

void CDlgDeviceLedger::InitDeviceLedgerListByStation(StationNode* pNodeInfo)
{
	FreeDeviceLedgerList(&m_lstDeviceLedger);

	//获取变电站下的所有设备
	char sql_buf[1024]={0x0};

	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"SELECT id,station_id,device_name,device_manufacturer,device_ip,device_port,login_username, "
		" login_password,protocol_type,device_type,harddisk_capacity,analog_video_num,net_video_num "
		" FROM ass_device_ledger where station_id=%d",pNodeInfo->station_id);
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		int nIndex = 0;
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			_T_ASS_DEVICE_LEDGER_* pDeviceLedger = new _T_ASS_DEVICE_LEDGER_;
			pDeviceLedger->id = atoi(row[0]);
			pDeviceLedger->station_id = atoi(row[1]);
			sprintf_s(pDeviceLedger->device_name,"%s",row[2]);
			sprintf_s(pDeviceLedger->device_manufacturer,"%s",row[3]);
			sprintf_s(pDeviceLedger->device_ip,"%s",row[4]);
			pDeviceLedger->device_port = atoi(row[5]);
			sprintf_s(pDeviceLedger->login_username,"%s",row[6]);
			sprintf_s(pDeviceLedger->login_password,"%s",row[7]);
			sprintf_s(pDeviceLedger->protocol_type,"%s",row[8]);
			sprintf_s(pDeviceLedger->device_type,"%s",row[9]);
			sprintf_s(pDeviceLedger->harddisk_capacity,"%s",row[10]);
			pDeviceLedger->analog_video_num = atoi(row[11]);
			pDeviceLedger->net_video_num = atoi(row[12]);
			sprintf_s(pDeviceLedger->szStationName, "%s", pNodeInfo->station_name);
			
			AddItemToDeviceLedgerList(&m_lstDeviceLedger, nIndex, pDeviceLedger);
			nIndex++;
		}
		mysql_free_result(res);
	}
}

void CDlgDeviceLedger::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd* pWnd = NULL;
	CRect winrc,tree_rect,button_rect,list_rect;

	int nButtonHeight = 30;
	int nButtonWidth = 100;

	int nButtonListIntval = 10;

	int nListHeight = cy - nButtonHeight - nButtonListIntval;
	int nListWidth = cx;

	GetClientRect(&winrc);

	pWnd = GetDlgItem(IDC_BUTTON_ADD_DEVICE);
	if (pWnd != NULL)
	{
		button_rect.top = winrc.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = winrc.left;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_EDIT_DEVICE);
	if (pWnd != NULL)
	{
		button_rect.top = winrc.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_DEL_DEVICE);
	if (pWnd != NULL)
	{
		button_rect.top = winrc.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_LIST_DEVICE_LEDGER);
	if (pWnd != NULL)
	{
		list_rect.top = button_rect.bottom;//list_rect.top = button_rect.bottom + nButtonListIntval;
		list_rect.bottom = winrc.bottom;
		list_rect.left = winrc.left;
		list_rect.right = list_rect.left + nListWidth;
		pWnd->MoveWindow(list_rect);
	}	
}

HBRUSH CDlgDeviceLedger::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC_DEVICE_MANUFACTURE_IDENTIFY)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}

	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC_PROTOCOL_TYPE_IDENTIFY)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}

	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC_DEVICE_IP_IDENTIFY)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}

	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC_DEVICE_PORT_IDENTIFY)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}

	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC_LOGIN_PASSWORD_IDENTIFY)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}

	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC_LOGIN_USERNAME_IDENTIFY)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CDlgDeviceLedger::OnEraseBkgnd(CDC* pDC)
{
	//设置对话框背景色
	CBrush backBrush(RGB(218,255,251));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);

	//画需要的区域
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);

	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CDC srcDC;
	//srcDC.CreateCompatibleDC(pDC);
	//CBitmap* pOldBmp;
	////pOldBmp = srcDC.SelectObject(m_pTitleBmp);
	//BITMAP bmp;
	////m_pTitleBmp->GetBitmap(&bmp);
	////pDC->SetStretchBltMode(COLORONCOLOR);
	////pDC->StretchBlt(0,MAIN_TITLE_HEIGHT + 1,m_WndRect.Width(),MAIN_BUTTON_HIGHT,&srcDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	////srcDC.SelectObject(pOldBmp);
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

void CDlgDeviceLedger::InitDeviceList(StationNode* pNodeInfo)
{
	//清空列表
	FreeDeviceListTree();
	//添加变电站根结点
	HTREEITEM hMainItem = NULL,hItemDevice = NULL;
	hMainItem = m_treeDeviceList.InsertItem(pNodeInfo->station_name, 8, 8, TVI_ROOT, NULL);
	SetTreeNodeInfo(&m_treeDeviceList,hMainItem,pNodeInfo->station_id,STATION_NODE,pNodeInfo->station_name,0,NULL);

	//获取变电站下的所有设备
	char sql_buf[1024]={0x0};

	MYSQL_RES* res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"SELECT id,station_id,device_name,device_manufacturer,device_ip,device_port,login_username, "
		" login_password,protocol_type,device_type,harddisk_capacity,analog_video_num,net_video_num "
		" FROM ass_device_ledger where station_id=%d",pNodeInfo->station_id);
	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			_T_ASS_DEVICE_LEDGER_* pDeviceLedger = new _T_ASS_DEVICE_LEDGER_;
			pDeviceLedger->id = atoi(row[0]);
			pDeviceLedger->station_id = atoi(row[1]);
			sprintf_s(pDeviceLedger->device_name,"%s",row[2]);
			sprintf_s(pDeviceLedger->device_manufacturer,"%s",row[3]);
			sprintf_s(pDeviceLedger->device_ip,"%s",row[4]);
			pDeviceLedger->device_port = atoi(row[5]);
			sprintf_s(pDeviceLedger->login_username,"%s",row[6]);
			sprintf_s(pDeviceLedger->login_password,"%s",row[7]);
			sprintf_s(pDeviceLedger->protocol_type,"%s",row[8]);
			sprintf_s(pDeviceLedger->device_type,"%s",row[9]);
			sprintf_s(pDeviceLedger->harddisk_capacity,"%s",row[10]);
			pDeviceLedger->analog_video_num = atoi(row[11]);
			pDeviceLedger->net_video_num = atoi(row[12]);
			sprintf_s(pDeviceLedger->szStationName, "%s", pNodeInfo->station_name);
			hItemDevice = m_treeDeviceList.InsertItem(pDeviceLedger->device_name,10,10,hMainItem);
			m_treeDeviceList.SetItemData(hItemDevice,(DWORD)pDeviceLedger);
		}
		mysql_free_result(res);
	}

	ExpandAllItem(&m_treeDeviceList,NULL);

	EmptyEditControl();
}

void CDlgDeviceLedger::EmptyEditControl()
{
	GetDlgItem(IDC_EDIT_DEVICE_NAME)->SetWindowText("");
	GetDlgItem(IDC_EDIT_DEVICE_MANUFACTURE)->SetWindowText("");
	GetDlgItem(IDC_EDIT_DEVICE_IP)->SetWindowText("");
	GetDlgItem(IDC_EDIT_PROTOCOL_TYPE)->SetWindowText("");
	GetDlgItem(IDC_EDIT_DEVICE_PORT)->SetWindowText("");
	GetDlgItem(IDC_EDIT_LOGIN_USERNAME)->SetWindowText("");
	GetDlgItem(IDC_EDIT_LOGIN_PASSWORD)->SetWindowText("");
	GetDlgItem(IDC_EDIT_DEVICE_TYPE)->SetWindowText("");
	GetDlgItem(IDC_EDIT_HARDDISK_CAPACITY)->SetWindowText("");
	GetDlgItem(IDC_EDIT_ANOLOG_VIDEO_NUM)->SetWindowText("");
	GetDlgItem(IDC_EDIT_NET_VIDEO_NUM)->SetWindowText("");
}

void CDlgDeviceLedger::SetTreeNodeInfo(CTreeCtrl* pTreeList,HTREEITEM hItem,int nNodeId,int nNodeType,char* szNodeName,int nParentId,char* szNodeCode)
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

void CDlgDeviceLedger::FreeDeviceListTree()
{
	HTREEITEM	hMainItem = NULL;
	NodeInfo *pNodeInfo = NULL;
	DWORD_PTR pInfo = NULL;

	try
	{
		//树形列表
		hMainItem = m_treeDeviceList.GetRootItem();
		while(hMainItem != NULL)
		{
			pNodeInfo = (NodeInfo *)m_treeDeviceList.GetItemData(hMainItem);
			if (pNodeInfo != NULL)
			{
				delete pNodeInfo;
				pNodeInfo = NULL;
				m_treeDeviceList.SetItemData(hMainItem,NULL);
			}

			DeleteDeviceListTreeChildInfo(hMainItem);
			hMainItem = m_treeDeviceList.GetNextSiblingItem(hMainItem);
		}
	}
	catch(...)
	{

	}

	m_treeDeviceList.DeleteAllItems();
}

void CDlgDeviceLedger::DeleteDeviceListTreeChildInfo(HTREEITEM hItem)
{
	HTREEITEM	hChildItem = NULL;
	_T_ASS_DEVICE_LEDGER_ *pNodeInfo = NULL;

	try
	{
		hChildItem = m_treeDeviceList.GetChildItem(hItem);
		while(hChildItem != NULL)
		{
			pNodeInfo = (_T_ASS_DEVICE_LEDGER_*)m_treeDeviceList.GetItemData(hChildItem);
			if (pNodeInfo != NULL)
			{
				delete pNodeInfo;
				pNodeInfo = NULL;
				m_treeDeviceList.SetItemData(hChildItem,NULL);
			}
			hChildItem = m_treeDeviceList.GetNextSiblingItem(hChildItem);
		}
	}
	catch(...)
	{

	}
}

void CDlgDeviceLedger::ExpandAllItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem)
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

void CDlgDeviceLedger::FreeAllTreeItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem)
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



void CDlgDeviceLedger::OnBnClickedButtonAddDevice()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pStationNode == NULL)
	{
		return;
	}
	
	if (m_pAddDeviceLedger == NULL)
	{
		 m_pAddDeviceLedger = new CDlgAddDeviceLedger(this);
		 m_pAddDeviceLedger->Create(IDD_DIALOG_ADD_DEVICE_LEDGER, this); 
	}
	m_pAddDeviceLedger->InitControlData();
	m_pAddDeviceLedger->m_nStationId = m_pStationNode->station_id;
	m_pAddDeviceLedger->ShowWindow(SW_SHOW);
}

void CDlgDeviceLedger::OnNMDblclkTreeDeviceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	HTREEITEM hItem = m_treeDeviceList.GetSelectedItem();
	if (hItem == NULL)
	{
		return ;
	}

	HTREEITEM hChildItem = m_treeDeviceList.GetChildItem(hItem);
	if (hChildItem != NULL)
	{
		return;
	}

	_T_ASS_DEVICE_LEDGER_* pDeviceLedger = NULL;
	pDeviceLedger  = (_T_ASS_DEVICE_LEDGER_*)m_treeDeviceList.GetItemData(hItem);

	GetDlgItem(IDC_EDIT_DEVICE_NAME)->SetWindowText(pDeviceLedger->device_name);
	GetDlgItem(IDC_EDIT_DEVICE_MANUFACTURE)->SetWindowText(pDeviceLedger->device_manufacturer);
	GetDlgItem(IDC_EDIT_DEVICE_IP)->SetWindowText(pDeviceLedger->device_ip);
	GetDlgItem(IDC_EDIT_PROTOCOL_TYPE)->SetWindowText(pDeviceLedger->protocol_type);

	CString strDevicePort = "";
	strDevicePort.Format("%d",pDeviceLedger->device_port);
	GetDlgItem(IDC_EDIT_DEVICE_PORT)->SetWindowText(strDevicePort);

	GetDlgItem(IDC_EDIT_LOGIN_USERNAME)->SetWindowText(pDeviceLedger->login_username);
	GetDlgItem(IDC_EDIT_LOGIN_PASSWORD)->SetWindowText(pDeviceLedger->login_password);
	GetDlgItem(IDC_EDIT_DEVICE_TYPE)->SetWindowText(pDeviceLedger->device_type);
	GetDlgItem(IDC_EDIT_HARDDISK_CAPACITY)->SetWindowText(pDeviceLedger->harddisk_capacity);

	CString strAnaLogVideoNum = "";
	strAnaLogVideoNum.Format("%d",pDeviceLedger->analog_video_num);
	GetDlgItem(IDC_EDIT_ANOLOG_VIDEO_NUM)->SetWindowText(strAnaLogVideoNum);

	CString strNetVideoNum = "";
	strNetVideoNum.Format("%d",pDeviceLedger->net_video_num);
	GetDlgItem(IDC_EDIT_NET_VIDEO_NUM)->SetWindowText(strNetVideoNum);
}

void CDlgDeviceLedger::OnBnClickedButtonEditDevice()
{
	POSITION pos = m_lstDeviceLedger.GetFirstSelectedItemPosition();

	if (pos == NULL)
		return;

	int index = m_lstDeviceLedger.GetNextSelectedItem(pos);

	if (index < 0)
		return;

	_T_ASS_DEVICE_LEDGER_* pDeviceLedger = NULL;
	pDeviceLedger  = (_T_ASS_DEVICE_LEDGER_*)m_lstDeviceLedger.GetItemData(index);

	if (m_pAddDeviceLedger == NULL)
	{
		m_pAddDeviceLedger = new CDlgAddDeviceLedger(this);
		m_pAddDeviceLedger->Create(IDD_DIALOG_ADD_DEVICE_LEDGER, this); 
	}
	m_pAddDeviceLedger->m_nStationId = m_pStationNode->station_id;
	m_pAddDeviceLedger->SetDeviceLedger(pDeviceLedger);
	m_pAddDeviceLedger->InitControl();
	m_pAddDeviceLedger->ShowWindow(SW_SHOW);
	//if (m_pAddDeviceLedger == NULL)
	//{
	//	m_pAddDeviceLedger = new CDlgAddDeviceLedger(this);
	//}
	//m_pAddDeviceLedger->m_nStationId = m_pStationNode->station_id;
	//m_pAddDeviceLedger->SetDeviceLedger(pDeviceLedger);
	//if (m_pAddDeviceLedger->DoModal() == IDOK)
	//{
	//	InitDeviceLedgerListByStation(m_pStationNode);
	//}
}

void CDlgDeviceLedger::OnBnClickedButtonDelDevice()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_lstDeviceLedger.GetFirstSelectedItemPosition();

	if (pos == NULL)
		return;

	int index = m_lstDeviceLedger.GetNextSelectedItem(pos);

	if (index < 0)
		return;

	_T_ASS_DEVICE_LEDGER_* pDeviceLedger = NULL;
	pDeviceLedger  = (_T_ASS_DEVICE_LEDGER_*)m_lstDeviceLedger.GetItemData(index);

	char sql_buf[1024]={0x0};
	MYSQL_RES	* res;
	int ret;
	sprintf_s(sql_buf,"delete from ass_device_ledger where id='%d'",
		pDeviceLedger->id);
	ret = mysql_query(g_LinkageSettingMysql, sql_buf);
	if (!ret)
	{
		MessageBox("设备删除成功","辅助系统");

		//写操作记录
		CString strText = "";
		strText.Format("设备删除成功：%s", pDeviceLedger->device_name);
		WriteAssUserOperateLogPre(strText.GetBuffer());
	}
	else
	{
		MessageBox("设备删除失败,请检查数据库连接","辅助系统");

		//写操作记录
		CString strText = "";
		strText.Format("设备删除失败：%s", pDeviceLedger->device_name);
		WriteAssUserOperateLogPre(strText.GetBuffer());

		return;
	}
	InitDeviceLedgerListByStation(m_pStationNode);
}

LRESULT CDlgDeviceLedger::OnAddDeviceLedgerCompletedHandler( WPARAM wParam, LPARAM lParam )
{
	InitDeviceLedgerListByStation(m_pStationNode);
	return 0;
}