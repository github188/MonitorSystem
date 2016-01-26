// DlgHLDevice.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgHLDevice.h"
#include "DlgStationSelect.h"


// CDlgHLDevice 对话框

IMPLEMENT_DYNAMIC(CDlgHLDevice, CDialog)

CDlgHLDevice::CDlgHLDevice(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHLDevice::IDD, pParent)
	, m_StartDay(0)
	, m_StartTime(0)
	, m_StopDay(0)
	, m_StopTime(0)
{

}

CDlgHLDevice::~CDlgHLDevice()
{
}

void CDlgHLDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HL_DEVICE, m_lstHlDevice);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_comboDevice);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTDAY, m_StartDay);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTTIME, m_StartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STOPDAY, m_StopDay);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STOPTIME, m_StopTime);
	DDX_Control(pDX, IDC_BUTTON_SELECT_STATION, m_btnSelectStation);
	DDX_Control(pDX, IDC_BUTTON_QUERY, m_btnQuery);
}


BEGIN_MESSAGE_MAP(CDlgHLDevice, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SELECT_STATION, &CDlgHLDevice::OnBnClickedButtonSelectStation)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CDlgHLDevice::OnBnClickedButtonQuery)
END_MESSAGE_MAP()


// CDlgHLDevice 消息处理程序

BOOL CDlgHLDevice::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControl();

	LoadSkin();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgHLDevice::LoadSkin()
{
	m_btnSelectStation.LoadBitmap(IDB_BITMAP_SELECT_NODE);
	m_btnQuery.LoadBitmap(IDB_BITMAP_QUERY);

	m_lstHlDevice.SetHeaderHeight(1.2f);          //设置头部高度
	m_lstHlDevice.SetHeaderFontHW(12,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_lstHlDevice.SetHeaderTextColor(RGB(58,77,93)); //设置头部字体颜色
	m_lstHlDevice.SetHeaderBKColor(195,248,250,3); //设置头部背景色

	m_lstHlDevice.SetTextColor(RGB(36, 162, 146));
	m_lstHlDevice.SetBkColor(RGB(239, 246, 253));
	m_lstHlDevice.SetTextBkColor(RGB(239, 246, 253));
	m_lstHlDevice.SetRowHeigt(15);               //设置行高度
	m_lstHlDevice.SetFontHW(12,0);               //设置字体高度，和宽度,0表示缺省宽度

	return TRUE;
}


BOOL CDlgHLDevice::InitControl()
{
	// 查询结果列表中需要显示的列包括了：开始时间、结束时间、摄像头名称、
	DWORD dwStyle;
	dwStyle = m_lstHlDevice.GetStyle();
	dwStyle = LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT;
	m_lstHlDevice.SetExtendedStyle(dwStyle);

	InitControlList();

	m_comboDevice.InsertString(0, "所有设备");
	m_comboDevice.SetCurSel(0);

	CTime	nowtime	= CTime::GetCurrentTime();
	CTime	StartTime(nowtime.GetYear(), nowtime.GetMonth(),nowtime.GetDay(), 0, 0, 0);
	CTime	StopTime(nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(), 23, 59, 59);

	//开始时间初始化
	m_StartDay = StartTime;
	m_StartTime = StartTime;
	m_StopDay  = StopTime;
	m_StopTime  = StopTime;

	UpdateData(FALSE);

	return TRUE;
}

BOOL CDlgHLDevice::InitControlList()
{
	m_lstHlDevice.DeleteAllItems();
	while(m_lstHlDevice.DeleteColumn(0));

	m_lstHlDevice.InsertColumn(0,"变电站名称");
	m_lstHlDevice.SetColumnWidth(0,100);
	m_lstHlDevice.InsertColumn(1,"设备名称");
	m_lstHlDevice.SetColumnWidth(1,100);
	m_lstHlDevice.InsertColumn(2,"值");
	m_lstHlDevice.SetColumnWidth(2,140);
	m_lstHlDevice.InsertColumn(3,"时间");
	m_lstHlDevice.SetColumnWidth(3,140);

	return TRUE;
}

void CDlgHLDevice::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect(0,0,0,0), rc;
	CWnd *pWnd;

	GetClientRect(rc);

	// *****************************************最上面的按钮
	pWnd = GetDlgItem(IDC_LIST_HL_DEVICE); //系统日志
	if(pWnd)
	{
		rect.left = rc.left + 5;
		rect.right = rc.right - 5;
		rect.top = rc.top + 150;
		rect.bottom = rc.bottom - 5;

		pWnd->MoveWindow(rect);
	}
}

void CDlgHLDevice::OnBnClickedButtonSelectStation()
{
	CDlgStationSelect dlgStationSelect;

	if (dlgStationSelect.DoModal() == IDOK)
	{
		memcpy(&m_tStationNode,&dlgStationSelect.m_tStationNode,sizeof(_T_NODE_INFO));

		//设置edit文本
		GetDlgItem(IDC_EDIT_STATION)->SetWindowText(m_tStationNode.node_station);

		//更新combox设备
		if (strcmp(m_tStationNode.node_station,"全部")==0)
		{
			//添加combox控件为所有设备
			CComboBox* pComboAlarmDevice = (CComboBox*)GetDlgItem(IDC_COMBO_DEVICE);
			pComboAlarmDevice->ResetContent();
			pComboAlarmDevice->AddString("所有设备");
			pComboAlarmDevice->SetCurSel(0);
			return;
		}
		else
		{
			//设置combox控件数据
			UpdateAlarmReplayDeviceCombox(m_tStationNode.node_id);
		}
	}
}

void CDlgHLDevice::UpdateAlarmReplayDeviceCombox(int nStationId)
{
	CComboBox* pComboAlarmDevice = (CComboBox*)GetDlgItem(IDC_COMBO_DEVICE);
	pComboAlarmDevice->ResetContent();
	pComboAlarmDevice->AddString("所有设备");

	char sql_buf[1024] = {0};
	MYSQL_RES *res;
	MYSQL_ROW row ;
	sprintf_s(sql_buf, "select a.name from ass_rvu_sm a where a.rvu_id in (select b.rvu_id from ass_rvu b where b.station_id=%d)",nStationId);
	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);

		while (row = mysql_fetch_row(res))
		{
			pComboAlarmDevice->AddString(row[0]);
		}
		mysql_free_result(res) ;
	}
	pComboAlarmDevice->SetCurSel(0);
}

void CDlgHLDevice::OnBnClickedButtonQuery()
{
	SearchDeviceHistoryData();
}

void CDlgHLDevice::SearchDeviceHistoryData()
{
	UpdateData(TRUE);

	char szStartLinkageTime[128] = {0};
	char szStopLinkageTime[128] = {0};

	CTime starttime(m_StartDay.GetYear(), m_StartDay.GetMonth(), m_StartDay.GetDay(),
		m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond());

	CTime stoptime(m_StopDay.GetYear(), m_StopDay.GetMonth(), m_StopDay.GetDay(),
		m_StopTime.GetHour(), m_StopTime.GetMinute(), m_StopTime.GetSecond());

	if (stoptime <= starttime)
	{
		MessageBox("时间选择错误：开始时间大于结束时间","视频监视");
		return ;
	}

	sprintf(szStartLinkageTime, "%04d-%02d-%02d %02d:%02d:%02d",
		m_StartDay.GetYear(), m_StartDay.GetMonth(), m_StartDay.GetDay(),
		m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond());

	sprintf(szStopLinkageTime, "%04d-%02d-%02d %02d:%02d:%02d",
		m_StopDay.GetYear(), m_StopDay.GetMonth(), m_StopDay.GetDay(),
		m_StopTime.GetHour(), m_StopTime.GetMinute(), m_StopTime.GetSecond());

	//取变电站id，设备名称，时间参数
	char szYear[8] = {0};
	szYear[0] = szStartLinkageTime[0];
	szYear[1] = szStartLinkageTime[1];
	szYear[2] = szStartLinkageTime[2];
	szYear[3] = szStartLinkageTime[3];
	szYear[4] = szStartLinkageTime[5];
	szYear[5] = szStartLinkageTime[6];
	szYear[6] = 0;

	int nStationId = m_tStationNode.node_id;//变电站id
	CString strDevName = "";
	GetDlgItem(IDC_COMBO_DEVICE)->GetWindowText(strDevName);//设备名称

	//根据变电站id和设备名称获取告警信息并添加到listcontrol中
	SearchAndAddToListControl(nStationId,strDevName.GetBuffer(),szYear,szStartLinkageTime,szStopLinkageTime);
}

void CDlgHLDevice::SearchAndAddToListControl(int nStationId, char* strDevName, char* szYear,char* szStartTime,char* szEndTime)
{
	//listControl初始化
	InitControlList();

	COLORREF BKColor;
	COLORREF TextColor;

	BKColor = RGB(207,235,250);
	TextColor = RGB(168,0,0);

	char sql_buf[4096] = {0};
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	int nCount = 0;

	if (nStationId == 0)//表示全部变电站
	{
		sprintf_s(sql_buf, "select d.station_name_videoplant,b.name,a.value,a.time from ass_rvu_sm_history_data_%s a "
			"left join ass_rvu_sm b on a.dev_id=b.id "
			"left join ass_rvu c on a.rvu_id=c.rvu_id "
			"left join ob_d5000_station d on c.station_id=d.station_id "
		"where a.time>'%s' and a.time<'%s'",szYear,szStartTime,szEndTime);
	}
	else if (strcmp(strDevName,"所有设备") == 0 && nStationId != 0)
	{
		sprintf_s(sql_buf, "select d.station_name_videoplant,b.name,a.value,a.time from ass_rvu_sm_history_data_%s a "
			"left join ass_rvu_sm b on a.dev_id=b.id "
			"left join ass_rvu c on a.rvu_id=c.rvu_id "
			"left join ob_d5000_station d on c.station_id=d.station_id "
			"where a.time>'%s' and a.time<'%s' and d.station_id=%d",szYear,szStartTime,szEndTime,nStationId);
	}
	else if (strcmp(strDevName,"所有设备") != 0 && nStationId != 0)
	{
		sprintf_s(sql_buf, "select d.station_name_videoplant,b.name,a.value,a.time from ass_rvu_sm_history_data_%s a "
			"left join ass_rvu_sm b on a.dev_id=b.id "
			"left join ass_rvu c on a.rvu_id=c.rvu_id "
			"left join ob_d5000_station d on c.station_id=d.station_id "
			"where a.time>'%s' and a.time<'%s' and d.station_id=%d and b.name='%s'",szYear,szStartTime,szEndTime,nStationId,strDevName);
	}

	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);

		while (row = mysql_fetch_row(res))
		{
			m_lstHlDevice.InsertItem(nCount,row[0]);
			m_lstHlDevice.SetItemText(nCount,1,row[1]);
			m_lstHlDevice.SetItemText(nCount,2,row[2]);
			m_lstHlDevice.SetItemText(nCount,3,row[3]);

			m_lstHlDevice.SetRowColor(nCount,BKColor);
			m_lstHlDevice.SetRowTextColor(nCount,TextColor);

			nCount++;
		}
		mysql_free_result(res) ;
	}

	if (nCount == 0)
	{
		m_lstHlDevice.InsertItem(nCount,"无数据");
		m_lstHlDevice.SetItemText(nCount,1,"无数据");
		m_lstHlDevice.SetItemText(nCount,2,"无数据");
		m_lstHlDevice.SetItemText(nCount,3,"无数据");
	}
}

