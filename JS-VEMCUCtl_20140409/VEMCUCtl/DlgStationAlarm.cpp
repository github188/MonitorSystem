// DlgStationAlarm.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgStationAlarm.h"
#include "AssListControlOperate.h"


// CDlgStationAlarm 对话框

IMPLEMENT_DYNAMIC(CDlgStationAlarm, CDialog)

CDlgStationAlarm::CDlgStationAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStationAlarm::IDD, pParent)
{
	
}

CDlgStationAlarm::CDlgStationAlarm( int nStationId, CWnd* pParent /*= NULL*/ )
	: CDialog(CDlgStationAlarm::IDD, pParent)
{
	m_nStationId = nStationId;
}

CDlgStationAlarm::~CDlgStationAlarm()
{

}

void CDlgStationAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALARM, m_lstAlarm);
	DDX_Control(pDX, IDC_CHECK_ALL, m_checkAll);
	DDX_Control(pDX, IDC_BUTTON_QUERY, m_btnQuery);
	DDX_Control(pDX, IDC_CHECK_ANFANG1, m_checkAnFang1);
	DDX_Control(pDX, IDC_CHECK_FIRE1, m_checkFire1);
	DDX_Control(pDX, IDC_CHECK_DOOR1, m_checkDoor1);
	DDX_Control(pDX, IDC_BUTTON_PROCESS_ALL, m_btnProcessAll);
}


BEGIN_MESSAGE_MAP(CDlgStationAlarm, CDialog)
	ON_BN_CLICKED(IDC_CHECK_ALL, &CDlgStationAlarm::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CDlgStationAlarm::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS_ALL, &CDlgStationAlarm::OnBnClickedButtonProcessAll)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ALARM, &CDlgStationAlarm::OnNMRClickListAlarm)
	ON_COMMAND(ID_DLG_STATION_ALARM_DELETE_SELECTED_ITEMS, &CDlgStationAlarm::OnClickDeleteSelectedItems)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgStationAlarm 消息处理程序

BOOL CDlgStationAlarm::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化控件
	InitControl();

	//初始化列表数据
	InitListData(m_nStationId);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgStationAlarm::InitListData(int nStationId)
{
	//清空数据
	m_lstAlarm.DeleteAllItems();

	//拼出查询告警条件
	CString strCondition;
	strCondition = SpellCondition(nStationId);

	//从数据库中获取告警信息根据某个变电站ID
	std::vector<ACS_ALARM_INFO> vecAlarmInfo = GetAlarmInfoByStationId(strCondition);

	//将数据添加到列表中
	AddAlarmInfoToList(vecAlarmInfo);
}

void CDlgStationAlarm::AddAlarmInfoToList(std::vector<ACS_ALARM_INFO> vecAlarmInfo)
{
	for (int i=0; i<vecAlarmInfo.size(); i++)
	{
		m_lstAlarm.InsertItem(i, IntToCString(vecAlarmInfo.at(i).nId));
		m_lstAlarm.SetItemText(i, 1, vecAlarmInfo.at(i).szStationName);
		m_lstAlarm.SetItemText(i, 2, vecAlarmInfo.at(i).szDevName);
		m_lstAlarm.SetItemText(i, 3, vecAlarmInfo.at(i).szAlarmTypeName);
		m_lstAlarm.SetItemText(i, 4, vecAlarmInfo.at(i).szAlarmStartTime);
		m_lstAlarm.SetItemText(i, 5, GetPriorLevelText(vecAlarmInfo.at(i).nPriorLevel));
		m_lstAlarm.SetItemText(i, 6, GetProcessStatusText(vecAlarmInfo.at(i).nStatus));
	}
}

CString CDlgStationAlarm::SpellCondition(int nStationId)
{
	CString strSubSystemTypeCondition = " 1=2 ";

	if (m_checkAnFang1.GetCheck())
	{
		strSubSystemTypeCondition += " or f.f_parent_id=4 ";
	}
	if (m_checkFire1.GetCheck())
	{
		strSubSystemTypeCondition += " or f.f_parent_id=6 ";
	}
	if (m_checkDoor1.GetCheck())
	{
		strSubSystemTypeCondition += " or f.f_parent_id=2 ";
	}

	CString strCondition = "";
	strCondition.Format(" c.station_id=%d and a.process_status=1 and (%s) ", nStationId, strSubSystemTypeCondition);

	return strCondition;
}

void CDlgStationAlarm::InitControl()
{
	/****************列表*******************************/
	DWORD dwStyle;
	dwStyle = m_lstAlarm.GetStyle();
	dwStyle = LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT ;

	m_lstAlarm.SetExtendedStyle(dwStyle);
	m_lstAlarm.SetBkColor(RGB(239,246,253));
	m_lstAlarm.SetTextBkColor(RGB(0xfe,0xFF,0xc6));

	//添加列表字段,告警ID,变电站名称,告警设备名称,告警类型,告警发生时间,优先级,处理状态
	std::vector<CString> vecstrText;
	vecstrText.push_back("告警ID");
	vecstrText.push_back("变电站名称");
	vecstrText.push_back("告警设备名称");
	vecstrText.push_back("告警类型");
	vecstrText.push_back("告警发生时间");
	vecstrText.push_back("优先级");
	vecstrText.push_back("处理状态");

	AddColumnToList(&m_lstAlarm, vecstrText);

	/************************button****************************/
	InitButtonColor(GetDlgItem(IDC_BUTTON_QUERY));
	InitButtonColor(GetDlgItem(IDC_BUTTON_PROCESS_ALL));

	/************************checkbox**************************/
	m_checkAll.SetCheck(TRUE);
	m_checkAnFang1.SetCheck(TRUE);
	m_checkDoor1.SetCheck(TRUE);
	m_checkFire1.SetCheck(TRUE);
}

void CDlgStationAlarm::OnBnClickedCheckAll()
{
	if (m_checkAll.GetCheck())
	{
		m_checkAnFang1.SetCheck(TRUE);
		m_checkDoor1.SetCheck(TRUE);
		m_checkFire1.SetCheck(TRUE);
	}
}

void CDlgStationAlarm::OnBnClickedButtonQuery()
{
	InitListData(m_nStationId);
}

void CDlgStationAlarm::OnBnClickedCheckAnfang()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgStationAlarm::OnBnClickedCheckFire()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgStationAlarm::OnBnClickedCheckDoor()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgStationAlarm::OnBnClickedButtonProcessAll()
{
	int nNum = m_lstAlarm.GetItemCount();
	for (int i=0; i<nNum; i++)
	{
		int nAlarmId = atoi(m_lstAlarm.GetItemText(0, 0));
		
		//更新该告警ID的处理状态为2,表示已完成处理
		UpdateAlarmInfoToCompleted(nAlarmId, 2);

		m_lstAlarm.DeleteItem(0);
	}
}

void CDlgStationAlarm::OnNMRClickListAlarm(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	PopRightMenu();

	*pResult = 0;
}

void CDlgStationAlarm::PopRightMenu()
{
	CMenu menuMain;
	menuMain.CreatePopupMenu();

	menuMain.AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,ID_DLG_STATION_ALARM_DELETE_SELECTED_ITEMS,"处理选中报警");

	CPoint point;
	GetCursorPos(&point);

	menuMain.TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CDlgStationAlarm::OnClickDeleteSelectedItems()
{
	POSITION p = m_lstAlarm.GetFirstSelectedItemPosition();

	while(p)
	{
		int nSelected = m_lstAlarm.GetNextSelectedItem(p);

		int nAlarmId = atoi(m_lstAlarm.GetItemText(nSelected, 0));

		//更新该告警ID的处理状态为2,表示已完成处理
		UpdateAlarmInfoToCompleted(nAlarmId, 2);

		m_lstAlarm.DeleteItem(nSelected);

		p = m_lstAlarm.GetFirstSelectedItemPosition();
	}
}
BOOL CDlgStationAlarm::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}
