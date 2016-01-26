// DlgHistoryLogAlarm.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgHistoryLogAlarm.h"
#include "DlgStationSelect.h"


// CDlgHistoryLogAlarm 对话框

IMPLEMENT_DYNAMIC(CDlgHistoryLogAlarm, CDialog)

CDlgHistoryLogAlarm::CDlgHistoryLogAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHistoryLogAlarm::IDD, pParent)
	, m_StartDay(0)
	, m_StartTime(0)
	, m_StopDay(0)
	, m_StopTime(0)
{
}

CDlgHistoryLogAlarm::~CDlgHistoryLogAlarm()
{
}

void CDlgHistoryLogAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_HL_CHOOSE_STATION, m_btnSelectNode);
	DDX_Control(pDX, IDC_LIST_ALARM_LOG, m_listAlarmLog);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HL_STARTDAY, m_StartDay);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HL_STARTTIME, m_StartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HL_STOPDAY, m_StopDay);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HL_STOPTIME, m_StopTime);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_comboDevice);
	DDX_Control(pDX, IDC_BUTTON_QUERY, m_btnQuery);
}


BEGIN_MESSAGE_MAP(CDlgHistoryLogAlarm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CDlgHistoryLogAlarm::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_HL_CHOOSE_STATION, &CDlgHistoryLogAlarm::OnBnClickedButtonHlChooseStation)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgHistoryLogAlarm 消息处理程序

void CDlgHistoryLogAlarm::UpdateAlarmReplayDeviceCombox(int nStationId)
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

void CDlgHistoryLogAlarm::SearchAlarmReplayHistoryVideo()
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
	szYear[4] = 0;

	int nStationId = m_tStationNode.node_id;//变电站id
	CString strDevName = "";
	GetDlgItem(IDC_COMBO_DEVICE)->GetWindowText(strDevName);//设备名称

	//根据变电站id和设备名称获取告警信息并添加到listcontrol中
	SearchAndAddToListControl(nStationId,strDevName.GetBuffer(),szYear,szStartLinkageTime,szStopLinkageTime);
}

BOOL CDlgHistoryLogAlarm::InitControlListAlarm()
{
	m_listAlarmLog.DeleteAllItems();
	while(m_listAlarmLog.DeleteColumn(0));

	UpdateData(FALSE);

	int nWidth = 1024;

	m_listAlarmLog.InsertColumn(0,"电压等级");
	m_listAlarmLog.SetColumnWidth(0,nWidth*0.1);
	m_listAlarmLog.InsertColumn(1,"变电站名称");
	m_listAlarmLog.SetColumnWidth(1,nWidth*0.1);
	m_listAlarmLog.InsertColumn(2,"告警设备名称");
	m_listAlarmLog.SetColumnWidth(2,nWidth*0.1);
	m_listAlarmLog.InsertColumn(3,"告警类型");
	m_listAlarmLog.SetColumnWidth(3,nWidth*0.1);
	m_listAlarmLog.InsertColumn(4,"告警发生时间");
	m_listAlarmLog.SetColumnWidth(4,nWidth*0.1);
	m_listAlarmLog.InsertColumn(5,"告警处理时间");
	m_listAlarmLog.SetColumnWidth(5,nWidth*0.1);
	m_listAlarmLog.InsertColumn(6,"是否关联视频");
	m_listAlarmLog.SetColumnWidth(6,nWidth*0.1);
	m_listAlarmLog.InsertColumn(7,"优先级");
	m_listAlarmLog.SetColumnWidth(7,nWidth*0.1);
	m_listAlarmLog.InsertColumn(8,"处理状态");
	m_listAlarmLog.SetColumnWidth(8,nWidth*0.1);
	m_listAlarmLog.InsertColumn(9,"处理人员");
	m_listAlarmLog.SetColumnWidth(9,nWidth*0.1);

	UpdateData(FALSE);

	return TRUE;
}

void CDlgHistoryLogAlarm::SearchAndAddToListControl(int nStationId, char* strDevName, char* szYear,char* szStartTime,char* szEndTime)
{
	//listControl初始化
	InitControlListAlarm();

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
		sprintf_s(sql_buf, "select d.voltage_class,d.station_name_videoplant,c.`name`, "
			" a.type,a.start_time,a.process_time,e.id as preset_id,a.alarm_level,a.process_status,a.process_person from ass_alarm_%s a  "
			" left join ass_rvu b on a.rvu_id=b.rvu_id "
			" left join ass_rvu_sm c on a.sm_id=c.sm_id and a.rvu_id=c.rvu_id  "
			" left join ob_d5000_station d on b.station_id=d.station_id " 
			" left join ass_camerapreset e on e.dev_id=c.id " 
			" where a.start_time>'%s' and a.start_time<'%s' ",szYear,szStartTime,szEndTime);
	}
	else if (strcmp(strDevName,"所有设备") == 0 && nStationId != 0)
	{
		sprintf_s(sql_buf, "select d.station_name_videoplant,d.voltage_class,c.`name`, "
			" a.type,a.start_time,a.process_time,e.id as preset_id,a.alarm_level,a.process_status,a.process_person from ass_alarm_%s a  "
			" left join ass_rvu b on a.rvu_id=b.rvu_id "
			" left join ass_rvu_sm c on a.sm_id=c.sm_id and a.rvu_id=c.rvu_id  "
			" left join ob_d5000_station d on b.station_id=d.station_id " 
			" left join ass_camerapreset e on e.dev_id=c.id " 
			" where a.start_time>'%s' and a.start_time<'%s' and d.station_id=%d",szYear,szStartTime,szEndTime,nStationId);
	}
	else if (strcmp(strDevName,"所有设备") != 0 && nStationId != 0)
	{
		sprintf_s(sql_buf, "select d.station_name_videoplant,d.voltage_class,c.`name`, "
			" a.type,a.start_time,a.process_time,e.id as preset_id,a.alarm_level,a.process_status,a.process_person from ass_alarm_%s a  "
			" left join ass_rvu b on a.rvu_id=b.rvu_id "
			" left join ass_rvu_sm c on a.sm_id=c.sm_id and a.rvu_id=c.rvu_id  "
			" left join ob_d5000_station d on b.station_id=d.station_id " 
			" left join ass_camerapreset e on e.dev_id=c.id " 
			" where a.start_time>'%s' and a.start_time<'%s' and c.`name`='%s' and d.station_id=%d",szYear,szStartTime,szEndTime,strDevName,nStationId);
	}

	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);

		while (row = mysql_fetch_row(res))
		{
			m_listAlarmLog.InsertItem(nCount,row[0]);
			m_listAlarmLog.SetItemText(nCount,1,row[1]);
			m_listAlarmLog.SetItemText(nCount,2,row[2]);
			m_listAlarmLog.SetItemText(nCount,3,row[3]);
			m_listAlarmLog.SetItemText(nCount,4,row[4]);
			m_listAlarmLog.SetItemText(nCount,5,row[5]);

			char szPresetId[32] = {0};
			sprintf_s(szPresetId,"%s",row[6]);
			if (strcmp(szPresetId,"(null)")==0)
			{
				m_listAlarmLog.SetItemText(nCount,6,"未关联视频");
			}
			else
			{
				m_listAlarmLog.SetItemText(nCount,6,"已关联视频");
			}

			m_listAlarmLog.SetItemText(nCount,7,row[7]);
			m_listAlarmLog.SetItemText(nCount,8,row[8]);
			m_listAlarmLog.SetItemText(nCount,9,row[9]);

			m_listAlarmLog.SetRowColor(nCount,BKColor);
			m_listAlarmLog.SetRowTextColor(nCount,TextColor);

			nCount++;
		}
		mysql_free_result(res) ;
	}

	if (nCount == 0)
	{
		m_listAlarmLog.InsertItem(nCount,"无数据");
		m_listAlarmLog.SetItemText(nCount,1,"无数据");
		m_listAlarmLog.SetItemText(nCount,2,"无数据");
		m_listAlarmLog.SetItemText(nCount,3,"无数据");
		m_listAlarmLog.SetItemText(nCount,4,"无数据");
		m_listAlarmLog.SetItemText(nCount,5,"无数据");
		m_listAlarmLog.SetItemText(nCount,6,"无数据");
		m_listAlarmLog.SetItemText(nCount,7,"无数据");
		m_listAlarmLog.SetItemText(nCount,8,"无数据");
		m_listAlarmLog.SetItemText(nCount,9,"无数据");
	}
}

void CDlgHistoryLogAlarm::OnBnClickedButtonQuery()
{
	SearchAlarmReplayHistoryVideo();
}

BOOL CDlgHistoryLogAlarm::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControl();

	LoadSkin();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgHistoryLogAlarm::LoadSkin()
{
	m_btnSelectNode.LoadBitmap(IDB_BITMAP_SELECT_NODE);
	m_btnQuery.LoadBitmap(IDB_BITMAP_QUERY);

	m_listAlarmLog.SetHeaderHeight(1.2f);          //设置头部高度
	m_listAlarmLog.SetHeaderFontHW(12,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_listAlarmLog.SetHeaderTextColor(RGB(58,77,93)); //设置头部字体颜色
	m_listAlarmLog.SetHeaderBKColor(195,248,250,3); //设置头部背景色

	m_listAlarmLog.SetTextColor(RGB(36, 162, 146));
	m_listAlarmLog.SetBkColor(RGB(239, 246, 253));
	m_listAlarmLog.SetTextBkColor(RGB(239, 246, 253));
	m_listAlarmLog.SetRowHeigt(15);               //设置行高度
	m_listAlarmLog.SetFontHW(12,0);               //设置字体高度，和宽度,0表示缺省宽度
}

void CDlgHistoryLogAlarm::InitControl()
{
	DWORD dwStyle;
	dwStyle = m_listAlarmLog.GetStyle();
	dwStyle = LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT;
	m_listAlarmLog.SetExtendedStyle(dwStyle);

	InitControlListAlarm();

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
}

void CDlgHistoryLogAlarm::OnBnClickedButtonHlChooseStation()
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

void CDlgHistoryLogAlarm::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect(0,0,0,0), rc;
	CWnd *pWnd;

	GetClientRect(rc);

	// *****************************************最上面的按钮
	pWnd = GetDlgItem(IDC_LIST_ALARM_LOG); //系统日志
	if(pWnd)
	{
		rect.left = rc.left + 5;
		rect.right = rc.right - 5;
		rect.top = rc.top + 150;
		rect.bottom = rc.bottom - 5;

		pWnd->MoveWindow(rect);
	}
}
