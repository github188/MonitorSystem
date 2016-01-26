// DlgHistoryLogAlarm.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"
#include "DlgHistoryLogRelation.h"

// CDlgHistoryLogRelation 对话框

IMPLEMENT_DYNAMIC(CDlgHistoryLogRelation, CDialog)

CDlgHistoryLogRelation::CDlgHistoryLogRelation(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHistoryLogRelation::IDD, pParent)
	, m_sChooseNodeName(_T(""))
{
	m_Brush.CreateSolidBrush(RGB(218,255,251)); 
}

CDlgHistoryLogRelation::~CDlgHistoryLogRelation()
{
}

void CDlgHistoryLogRelation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HL_RELATION_NODE, m_sChooseNodeName);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HL_STARTDAY, m_StartDay);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HL_STARTTIME, m_StartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HL_STOPDAY, m_StopDay);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HL_STOPTIME, m_StopTime);
	DDX_Control(pDX, IDC_LIST_HISTORYLOG_RELATION, m_ListCtrl_RelationLog);
	DDX_Control(pDX, IDC_COMBO_HL_RELATION_TYPE, m_ComboRelationType);
	DDX_Control(pDX, IDC_COMBO_HL_RELATION_SUB_TYPE, m_ComboRelationSubType);
	DDX_Control(pDX, IDC_BUTTON_HL_CHOOSENODE, m_btnSelectNode);
	DDX_Control(pDX, IDC_BUTTON_HL_QUERY, m_btnQuery);
}

BEGIN_MESSAGE_MAP(CDlgHistoryLogRelation, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_HL_CHOOSENODE, &CDlgHistoryLogRelation::OnBnClickedButtonHlChooseNode)
	ON_BN_CLICKED(IDC_BUTTON_HL_QUERY, &CDlgHistoryLogRelation::OnBnClickedButtonHlQuery)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_HL_STARTDAY, &CDlgHistoryLogRelation::OnDtnDatetimechangeDatetimepickerHlStartday)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_HL_STARTTIME, &CDlgHistoryLogRelation::OnDtnDatetimechangeDatetimepickerHlStarttime)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_HL_STOPDAY, &CDlgHistoryLogRelation::OnDtnDatetimechangeDatetimepickerHlStopday)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_HL_STOPTIME, &CDlgHistoryLogRelation::OnDtnDatetimechangeDatetimepickerHlStoptime)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgHistoryLogRelation 消息处理程序
BOOL CDlgHistoryLogRelation::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControl();
	LoadSkin();

	return TRUE;
}

BOOL CDlgHistoryLogRelation::LoadSkin()
{
	m_btnSelectNode.LoadBitmap(IDB_BITMAP_SELECT_NODE);
	m_btnQuery.LoadBitmap(IDB_BITMAP_QUERY_LOG);

	m_ListCtrl_RelationLog.SetHeaderHeight(1.2f);          //设置头部高度
	m_ListCtrl_RelationLog.SetHeaderFontHW(12,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_ListCtrl_RelationLog.SetHeaderTextColor(RGB(58,77,93)); //设置头部字体颜色
	m_ListCtrl_RelationLog.SetHeaderBKColor(195,248,250,3); //设置头部背景色

	m_ListCtrl_RelationLog.SetTextColor(RGB(36, 162, 146));
	m_ListCtrl_RelationLog.SetBkColor(RGB(239, 246, 253));
	m_ListCtrl_RelationLog.SetTextBkColor(RGB(239, 246, 253));
	m_ListCtrl_RelationLog.SetRowHeigt(15);               //设置行高度
	m_ListCtrl_RelationLog.SetFontHW(12,0);               //设置字体高度，和宽度,0表示缺省宽度

	return TRUE;
}

BOOL CDlgHistoryLogRelation::InitControl()
{
	// 查询结果列表中需要显示的列包括了：开始时间、结束时间、摄像头名称、
	DWORD dwStyle;
	dwStyle = m_ListCtrl_RelationLog.GetStyle();
	dwStyle = LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT;
	m_ListCtrl_RelationLog.SetExtendedStyle(dwStyle);

	m_ListCtrl_RelationLog.InsertColumn(0,"联动类型");
	m_ListCtrl_RelationLog.SetColumnWidth(0,80);
	m_ListCtrl_RelationLog.InsertColumn(1,"设备名称");
	m_ListCtrl_RelationLog.SetColumnWidth(1,120);
	m_ListCtrl_RelationLog.InsertColumn(2,"站点名称");
	m_ListCtrl_RelationLog.SetColumnWidth(2,120);
	m_ListCtrl_RelationLog.InsertColumn(3,"联动时间");
	m_ListCtrl_RelationLog.SetColumnWidth(3,100);
	m_ListCtrl_RelationLog.InsertColumn(4,"分类型");
	m_ListCtrl_RelationLog.SetColumnWidth(4,80);
	m_ListCtrl_RelationLog.InsertColumn(5,"状态");
	m_ListCtrl_RelationLog.SetColumnWidth(5,80);
	m_ListCtrl_RelationLog.InsertColumn(6,"屏号");
	m_ListCtrl_RelationLog.SetColumnWidth(6,60);
	m_ListCtrl_RelationLog.InsertColumn(7,"备注");
	m_ListCtrl_RelationLog.SetColumnWidth(7,100);
	m_ListCtrl_RelationLog.InsertColumn(8,"Value1");
	m_ListCtrl_RelationLog.SetColumnWidth(8,60);
	m_ListCtrl_RelationLog.InsertColumn(9,"Value2");
	m_ListCtrl_RelationLog.SetColumnWidth(9,60);
	m_ListCtrl_RelationLog.InsertColumn(10,"联动站点");
	m_ListCtrl_RelationLog.SetColumnWidth(10,100);
	m_ListCtrl_RelationLog.InsertColumn(11,"联动数量");
	m_ListCtrl_RelationLog.SetColumnWidth(11,80);

	m_ComboRelationType.InsertString(0, "所有类型");
	m_ComboRelationType.InsertString(1, "手动联动");
	m_ComboRelationType.InsertString(2, "告警联动");
	m_ComboRelationType.InsertString(3, "状态联动");
	m_ComboRelationType.InsertString(4, "其它联动");

	m_ComboRelationType.SetCurSel(0);

	m_ComboRelationSubType.InsertString(0, "所有分类型");
	m_ComboRelationSubType.SetCurSel(0);

	CTime	nowtime	= CTime::GetCurrentTime();
	CTime	StartTime(nowtime.GetYear(), nowtime.GetMonth(), 1, 0, 0, 0);
	CTime	StopTime(nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(), 23, 59, 59);

	//开始时间初始化
	m_StartDay = StartTime;
	m_StartTime = StartTime;
	m_StopDay  = StopTime;
	m_StopTime  = StopTime;

	UpdateData(FALSE);

	return TRUE;
}

void CDlgHistoryLogRelation::OnBnClickedButtonHlChooseNode()
{

}

void CDlgHistoryLogRelation::OnBnClickedButtonHlQuery()
{
	UpdateData(true);

	int i = 0, j = 0;
	char s_starttime[128]={0x0};
	char s_stoptime[128]={0x0};

	CTime starttime(m_StartDay.GetYear(), m_StartDay.GetMonth(), m_StartDay.GetDay(),
		m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond());
	CTime stoptime(m_StopDay.GetYear(), m_StopDay.GetMonth(), m_StopDay.GetDay(),
		m_StopTime.GetHour(), m_StopTime.GetMinute(), m_StopTime.GetSecond());

	if (stoptime <= starttime)
	{
		MessageBox("时间选择错误：开始时间大于结束时间","视频监视");
		return;
	}

	sprintf(s_starttime, "%04d-%02d-%02dT%02d:%02d:%02dZ",
		m_StartDay.GetYear(), m_StartDay.GetMonth(), m_StartDay.GetDay(),
		m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond());
	sprintf(s_stoptime, "%04d-%02d-%02dT%02d:%02d:%02dZ",
		m_StopDay.GetYear(), m_StopDay.GetMonth(), m_StopDay.GetDay(),
		m_StopTime.GetHour(), m_StopTime.GetMinute(), m_StopTime.GetSecond());

	//鼠标为等待状态
	AfxGetApp()->DoWaitCursor(1); 

	m_ListCtrl_RelationLog.DeleteAllItems();

	int nSelectIndex = m_ComboRelationType.GetCurSel();

	switch(nSelectIndex)
	{
	case 0:
		{
			SearchAndSetHistoryListInfo("视频联动",s_starttime,s_stoptime);
			SearchAndSetHistoryListInfo("告警联动",s_starttime,s_stoptime);
			SearchAndSetHistoryListInfo("状态联动",s_starttime,s_stoptime);
		}
		break;
	case 1:
		{
			SearchAndSetHistoryListInfo("视频联动",s_starttime,s_stoptime);
		}
		break;
	case 2:
		{
			SearchAndSetHistoryListInfo("告警联动",s_starttime,s_stoptime);
		}
		break;
	case 3:
		{
			SearchAndSetHistoryListInfo("状态联动",s_starttime,s_stoptime);
		}
		break;
	}

	//恢复鼠标为正常状态
	AfxGetApp()->DoWaitCursor(0);
}

BOOL CDlgHistoryLogRelation::SearchAndSetHistoryListInfo(char *szType,char *szStartTime,char *szStopTime)
{
	if (szStartTime == NULL||szStopTime == NULL)
		return FALSE;

	char sql_buf[512] = {0};
	bool result=false;

	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;

	char szYear[8] = {0};
	int nYear = 2013;

	char szLinkageType[64] = {0};
	char szStationName[256] = {0};
	char szDeviceName[256] = {0};
	char szSubType[64] = {0};
	char szState[64] = {0};
	char szScreenId[64] = {0};
	char szLinkageRealTime[64] = {0};
	char szContent[256] = {0};
	char szValue1[64] = {0};
	char szValue2[64] = {0};
	char szLinkageStationName[256] = {0};
	char szLinkageNum[16] = {0};

	COLORREF BKColor;
	COLORREF TextColor;

	szYear[0] = szStartTime[0];
	szYear[1] = szStartTime[1];
	szYear[2] = szStartTime[2];
	szYear[3] = szStartTime[3];
	szYear[4] = 0;

	nYear = atoi(szYear);

	//读联动信息
	sprintf_s(sql_buf, sizeof(sql_buf),"SELECT event, station, device, type, state, scree_id, time,"
		"content, yx_value,yc_value,station,link_num FROM history_relation_%04d "
		"where event='%s' AND time >= '%s' AND time <= '%s' ORDER BY id",nYear,szType,szStartTime,szStopTime);

	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);

		while ( row = mysql_fetch_row( res ) )
		{
			sprintf_s(szLinkageType, "%s", row[0]);

			if (strcmp(szLinkageType,"视频联动") == 0)
			{
				BKColor = RGB(236,230,255);
				TextColor = RGB(0,168,0);
			}
			else if (strcmp(szLinkageType,"告警联动") == 0)
			{
				BKColor = RGB(207,235,250);
				TextColor = RGB(168,0,0);
			}
			else if (strcmp(szLinkageType,"状态联动") == 0)
			{
				BKColor = RGB(197,228,249);
				TextColor = RGB(64,128,128);
			}
			else
			{
				BKColor = RGB(128,128,128);
				TextColor = RGB(30,30,30);
			}

			sprintf_s(szStationName, "%s", row[1]);
			sprintf_s(szDeviceName, "%s", row[2]);
			sprintf_s(szSubType,"%s",row[3]);
			sprintf_s(szState,"%s",row[4]);
			sprintf_s(szScreenId,"%s",row[5]);
			sprintf_s(szLinkageRealTime,"%s",row[6]);
			sprintf_s(szContent,"%s",row[7]);
			sprintf_s(szValue1, "%s", row[8]);
			sprintf_s(szValue2,"%s",row[9]);
			sprintf_s(szLinkageStationName,"%s",row[10]);
			sprintf_s(szLinkageNum,"%s",row[11]);

			m_ListCtrl_RelationLog.InsertItem(0,szLinkageType);
			m_ListCtrl_RelationLog.SetItemText(0,1,szDeviceName);
			m_ListCtrl_RelationLog.SetItemText(0,2,szStationName);
			m_ListCtrl_RelationLog.SetItemText(0,3,szLinkageRealTime);
			m_ListCtrl_RelationLog.SetItemText(0,4,szSubType);
			m_ListCtrl_RelationLog.SetItemText(0,5,szState);
			m_ListCtrl_RelationLog.SetItemText(0,6,szScreenId);
			m_ListCtrl_RelationLog.SetItemText(0,7,szContent);
			m_ListCtrl_RelationLog.SetItemText(0,8,szValue1);
			m_ListCtrl_RelationLog.SetItemText(0,9,szValue2);
			m_ListCtrl_RelationLog.SetItemText(0,10,szLinkageStationName);
			m_ListCtrl_RelationLog.SetItemText(0,11,szLinkageNum);

			m_ListCtrl_RelationLog.SetRowColor(0,BKColor);
			m_ListCtrl_RelationLog.SetRowTextColor(0,TextColor);
		}
		mysql_free_result( res ) ;
	}

	return TRUE;
}

BOOL CDlgHistoryLogRelation::OnEraseBkgnd(CDC* pDC)
{
	CRect   rect;
	GetClientRect(&rect);

	pDC->FillRect (rect, &m_Brush);

	return TRUE;
}

HBRUSH CDlgHistoryLogRelation::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//////////////////////////////////////////////////////////////////////////
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(10,10,10));
		return m_Brush;
	}

	return hbr;
}

void CDlgHistoryLogRelation::OnDtnDatetimechangeDatetimepickerHlStartday(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	
	char str[64];
	CTime	nowtime;
	GetDlgItemText(IDC_DATETIMEPICKER_HL_STARTDAY,str,64);
	if ((memcmp(str,"1971-",5) < 0)||(memcmp(str,"2037-",5) > 0))
	{
		nowtime	= CTime::GetCurrentTime();
		CTime	StartTime(nowtime.GetYear(), nowtime.GetMonth(), 1, 0, 0, 0);
		m_StartDay = StartTime;
		m_StartTime = StartTime;
		UpdateData(false);
	}

	*pResult = 0;
}


void CDlgHistoryLogRelation::OnDtnDatetimechangeDatetimepickerHlStarttime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);


	*pResult = 0;
}


void CDlgHistoryLogRelation::OnDtnDatetimechangeDatetimepickerHlStopday(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	char str[64];
	CTime	nowtime;
	GetDlgItemText(IDC_DATETIMEPICKER_HL_STOPDAY,str,64);
	if ((memcmp(str,"1971-",5) < 0)||(memcmp(str,"2037-",5) > 0))
	{	
		nowtime	= CTime::GetCurrentTime();
		CTime	StopTime(nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(), 23, 59, 59);
		m_StopDay  = StopTime;
		m_StopTime  = StopTime;
		UpdateData(false);
	}


	*pResult = 0;
}


void CDlgHistoryLogRelation::OnDtnDatetimechangeDatetimepickerHlStoptime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);


	*pResult = 0;
}

void CDlgHistoryLogRelation::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect(0,0,0,0);
	CWnd *pWnd;

	// *****************************************最上面的按钮
	pWnd = GetDlgItem(IDC_BUTTON_HL_SYSTEM); //系统日志
	if(pWnd)
	{
		rect.left = 1;
		rect.right = rect.left+90;
		rect.top = 1;
		rect.bottom = 27;

		pWnd->MoveWindow(rect);
	}
}
