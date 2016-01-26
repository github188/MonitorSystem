// DlgAlarmPreview.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgAlarmPreview.h"
#include "AssListControlOperate.h"
#include "AssTreeOperate.h"
#include "DlgStationAlarm.h"
#include "AssDialogOperate.h"


// CDlgAlarmPreview 对话框

IMPLEMENT_DYNAMIC(CDlgAlarmPreview, CDialog)

CDlgAlarmPreview::CDlgAlarmPreview(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmPreview::IDD, pParent)
{
	m_nStationId = 0;
}

CDlgAlarmPreview::~CDlgAlarmPreview()
{
	GlobalDeleteCameraTreeList(&m_treeStation);
}

void CDlgAlarmPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_QUERY, m_btnQuery);
	DDX_Control(pDX, IDC_LIST_ALARM, m_lstAlarm);
	DDX_Control(pDX, IDC_TREE_STATION, m_treeStation);
}


BEGIN_MESSAGE_MAP(CDlgAlarmPreview, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CDlgAlarmPreview::OnBnClickedButtonQuery)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ALARM, &CDlgAlarmPreview::OnNMDblclkListAlarm)
	ON_COMMAND(ID_DLG_ALARM_PREVIEW_VIEW_DETAIL, &CDlgAlarmPreview::OnClickViewDetail)
	ON_COMMAND(ID_DLG_ALARM_PREVIEW_PROCESS_ALL, &CDlgAlarmPreview::OnClickProcessAll)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ALARM, &CDlgAlarmPreview::OnNMRClickListAlarm)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgAlarmPreview 消息处理程序

BOOL CDlgAlarmPreview::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化控件
	InitControl();

	//变电站树数据添加
	InitStationtree();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgAlarmPreview::InitControl()
{
	//设置tree图标
	m_imageList.Create(IDB_BITMAP_TREE,16,0,RGB(255,0,255));
	m_treeStation.SetImageList(&m_imageList, TVSIL_NORMAL);

	//设置按钮颜色
	InitDlgButtonColor(m_hWnd);

	//列表中有以下列,变电站名称,安防告警次数,消防告警次数,门禁告警次数
	InitAlarmListColumnAndWidth();
}

void CDlgAlarmPreview::InitAlarmListColumnAndWidth()
{
	DWORD dwStyle;
	dwStyle = m_lstAlarm.GetStyle();
	dwStyle = LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT ;

	m_lstAlarm.SetExtendedStyle(dwStyle);
	m_lstAlarm.SetBkColor(RGB(239,246,253));
	m_lstAlarm.SetTextBkColor(RGB(0xfe,0xFF,0xc6));

	std::vector<CString> vecText;
	vecText.push_back("序号");
	vecText.push_back("变电站名称");
	vecText.push_back("安防告警次数");
	vecText.push_back("消防告警次数");
	vecText.push_back("门禁告警次数");

	AddColumnToList(&m_lstAlarm, vecText);
}

void CDlgAlarmPreview::InitStationtree()
{
	GlobalInitStationTree(&m_treeStation);
}

void CDlgAlarmPreview::OnBnClickedButtonQuery()
{
	//清空列表
	m_lstAlarm.DeleteAllItems();

	//获取所有选中的变电站
	std::vector<HTREEITEM> vecSelectedStationItems;
	m_treeStation.GetCheckItems(vecSelectedStationItems, m_treeStation.GetRootItem());
	
	//拼接出查询条件
	CString strCondition = GetQueryCondition(vecSelectedStationItems);

	//查询变电站下的未处理的告警次数
	std::vector<_T_ASS_SUBSYSTEM_ALARM_COUNT> vecSubSystemAlarmCount = GetSubSystemAlarmCount(strCondition);

	//将变电站告警次数添加到列表中
	AddStationAlarmToList(vecSubSystemAlarmCount);
}

void CDlgAlarmPreview::AddStationAlarmToList(std::vector<_T_ASS_SUBSYSTEM_ALARM_COUNT> vecSubSystemAlarmCount)
{
	int nIndex = 0;
	for (size_t t = 0; t < vecSubSystemAlarmCount.size(); t++)
	{
		CString strIndex = "";
		strIndex.Format("%d",nIndex);
		m_lstAlarm.InsertItem(nIndex,strIndex);

		m_lstAlarm.SetItemText(nIndex,1,vecSubSystemAlarmCount.at(t).szStationName);
		m_lstAlarm.SetItemText(nIndex,2,IntToCString(vecSubSystemAlarmCount.at(t).nAnFangCount));
		m_lstAlarm.SetItemText(nIndex,3,IntToCString(vecSubSystemAlarmCount.at(t).nFireCount));
		m_lstAlarm.SetItemText(nIndex,4,IntToCString(vecSubSystemAlarmCount.at(t).nDoorCount));

		nIndex++;
	}

	if (nIndex == 0)
	{
		m_lstAlarm.InsertItem(nIndex,"无数据");

		m_lstAlarm.SetItemText(nIndex,1,"无数据");
		m_lstAlarm.SetItemText(nIndex,2,"无数据");
		m_lstAlarm.SetItemText(nIndex,3,"无数据");
	}
}

//拼出选择的变电站的条件
CString CDlgAlarmPreview::GetQueryCondition(std::vector<HTREEITEM> vecSelectedItems)
{
	_T_NODE_INFO* pNodeInfo = NULL;
	CString strCondition = "1=2";

	for (size_t t = 0; t < vecSelectedItems.size(); t++)
	{
		HTREEITEM item = vecSelectedItems.at(t);
		pNodeInfo = (_T_NODE_INFO*)m_treeStation.GetItemData(item);

		//如果是变电站
		if (pNodeInfo->node_type == 2)
		{
			int nStationId = pNodeInfo->node_id;
			CString strStationIdText;
			strStationIdText.Format(" or e.station_id=%d ",nStationId);
			strCondition += strStationIdText;
		}
	}

	CString strUnProcessCondition;
	strUnProcessCondition.Format("where a.process_status=1 and (%s)", strCondition);

	return strUnProcessCondition;
}

//弹出右键菜单
void CDlgAlarmPreview::PopRightMenu()
{
	CMenu menuMain;
	menuMain.CreatePopupMenu();

	menuMain.AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,ID_DLG_ALARM_PREVIEW_VIEW_DETAIL,"查看详细");
	//menuMain.AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,ID_DLG_ALARM_PREVIEW_PROCESS_ALL,"全部处理");

	CPoint point;
	GetCursorPos(&point);

	menuMain.TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CDlgAlarmPreview::OnNMDblclkListAlarm(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	//弹出该变电站下未处理的告警框

	*pResult = 0;
}

void CDlgAlarmPreview::OnClickViewDetail()
{
	//弹出变电站未处理告警详细信息对话框
	CDlgStationAlarm dlgStationAlarm(m_nStationId);
	if (dlgStationAlarm.DoModal() == IDOK)
	{
		return;
	}
}

void CDlgAlarmPreview::OnClickProcessAll()
{
	//更新所有未处理告警为处理完成
}

void CDlgAlarmPreview::OnNMRClickListAlarm(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	POSITION pos = m_lstAlarm.GetFirstSelectedItemPosition();

	if (pos == NULL)
		return;

	int index = m_lstAlarm.GetNextSelectedItem(pos);

	if (index < 0)
		return;
	
	//保存所选条目变电站
	CString strStationName = m_lstAlarm.GetItemText(index, 1);
	m_nStationId = GetStationIdByStationName(strStationName);

	//弹出右键菜单
	PopRightMenu();

	*pResult = 0;
}

void CDlgAlarmPreview::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CWnd *pWnd = NULL;

	CRect treeRect,listRect,buttonRect;
	cx = cx-7;
	int nButtonHeight = 30;
	int nButtonWidth = 100;
	int nTreeHeight = cy;
	int nTreeWidth = cx * 0.15;
	int nListWidth = cx * 0.85;
	int nListHeight = cy - nButtonHeight;

	//变电站树
	pWnd = GetDlgItem(IDC_TREE_STATION);
	if (pWnd != NULL)
	{
		treeRect.top = 0;
		treeRect.bottom = treeRect.top + nTreeHeight;
		treeRect.left = 0;
		treeRect.right = treeRect.left + nTreeWidth;
		pWnd->MoveWindow(treeRect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_QUERY);
	if (pWnd != NULL)
	{
		buttonRect.top = 0;
		buttonRect.bottom = buttonRect.top + nButtonHeight;
		buttonRect.left = treeRect.right + 7;
		buttonRect.right = buttonRect.left + nButtonWidth;
		pWnd->MoveWindow(buttonRect);
	}

	pWnd = GetDlgItem(IDC_LIST_ALARM);
	if (pWnd != NULL)
	{
		listRect.top = buttonRect.bottom;
		listRect.bottom = listRect.top + nListHeight;
		listRect.left = buttonRect.left;
		listRect.right = listRect.left + nListWidth;
		pWnd->MoveWindow(listRect);
	}

	/*************************改颜色***************************/
	//CClientDC dc(this);
	//CBrush brush = RGB(0,255,0);
	//dc.FillRect(CRect(0,0,cx,cy), &brush);

}

BOOL CDlgAlarmPreview::OnEraseBkgnd(CDC* pDC)
{
	//SetDialogBKPic(this, pDC);
	//return TRUE;
	//CClientDC dc(this);
	//CBrush brush = RGB(0,255,0);
	//dc.FillRect(CRect(0,0,1000,1000), &brush);
	return TRUE;


	return CDialog::OnEraseBkgnd(pDC);
}

void CDlgAlarmPreview::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// 不为绘图消息调用 CDialog::OnPaint()
}
