// DlgHistoryLogVideoError.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgHistoryLogVideoError.h"


// CDlgHistoryLogVideoError 对话框

IMPLEMENT_DYNAMIC(CDlgHistoryLogVideoError, CDialog)

CDlgHistoryLogVideoError::CDlgHistoryLogVideoError(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHistoryLogVideoError::IDD, pParent)
{
	m_Brush.CreateSolidBrush(RGB(210,224,237)); 
}

CDlgHistoryLogVideoError::~CDlgHistoryLogVideoError()
{
}

void CDlgHistoryLogVideoError::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HL_NODE_NAME, m_strNodeName);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HL_STARTDAY, m_StartDay);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HL_STARTTIME, m_StartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HL_STOPDAY, m_StopDay);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HL_STOPTIME, m_StopTime);
	DDX_Control(pDX, IDC_LIST_HISTORYLOG_VIDEO_ERROR, m_ListCtrl_VidoeErrorLog);
	DDX_Control(pDX, IDC_COMBO_HL_TYPE, m_ComboType);
	DDX_Control(pDX, IDC_COMBO_HL_SUB_TYPE, m_ComboSubType);
	DDX_Control(pDX, IDC_BUTTON_HL_CHOOSENODE, m_btnSelectNode);
	DDX_Control(pDX, IDC_BUTTON_HL_QUERY, m_btnQuery);
}


BEGIN_MESSAGE_MAP(CDlgHistoryLogVideoError, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_HL_QUERY, &CDlgHistoryLogVideoError::OnBnClickedButtonHlQuery)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_HL_CHOOSENODE, &CDlgHistoryLogVideoError::OnBnClickedButtonHlChoosenode)
END_MESSAGE_MAP()

// CDlgHistoryLogVideoError 消息处理程序
BOOL CDlgHistoryLogVideoError::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	InitControl();

	LoadSkin();

	return TRUE; 
}

BOOL CDlgHistoryLogVideoError::InitControl()
{
	// 查询结果列表中需要显示的列包括了：开始时间、结束时间、摄像头名称、
	DWORD dwStyle;
	dwStyle = m_ListCtrl_VidoeErrorLog.GetStyle();
	dwStyle = LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT;
	m_ListCtrl_VidoeErrorLog.SetExtendedStyle(dwStyle);

	m_ListCtrl_VidoeErrorLog.InsertColumn(0,"摄像头名称");
	m_ListCtrl_VidoeErrorLog.SetColumnWidth(0,120);
	m_ListCtrl_VidoeErrorLog.InsertColumn(1,"摄像头号码");
	m_ListCtrl_VidoeErrorLog.SetColumnWidth(1,160);
	m_ListCtrl_VidoeErrorLog.InsertColumn(2,"站点名称");
	m_ListCtrl_VidoeErrorLog.SetColumnWidth(2,120);
	m_ListCtrl_VidoeErrorLog.InsertColumn(3,"站点号码");
	m_ListCtrl_VidoeErrorLog.SetColumnWidth(3,160);
	m_ListCtrl_VidoeErrorLog.InsertColumn(4,"状态");
	m_ListCtrl_VidoeErrorLog.SetColumnWidth(4,80);
	m_ListCtrl_VidoeErrorLog.InsertColumn(5,"原因");
	m_ListCtrl_VidoeErrorLog.SetColumnWidth(5,60);
	m_ListCtrl_VidoeErrorLog.InsertColumn(6,"检测时间");
	m_ListCtrl_VidoeErrorLog.SetColumnWidth(6,160);
	m_ListCtrl_VidoeErrorLog.InsertColumn(7,"流量");
	m_ListCtrl_VidoeErrorLog.SetColumnWidth(7,60);
	m_ListCtrl_VidoeErrorLog.InsertColumn(8,"结点名称");
	m_ListCtrl_VidoeErrorLog.SetColumnWidth(8,120);
	m_ListCtrl_VidoeErrorLog.InsertColumn(9,"客户端IP");
	m_ListCtrl_VidoeErrorLog.SetColumnWidth(9,100);

	m_ComboType.InsertString(0, "所有类型");
	m_ComboType.SetCurSel(0);

	m_ComboSubType.InsertString(0, "所有分类型");
	m_ComboSubType.SetCurSel(0);

	CTime	nowtime	= CTime::GetCurrentTime();
	CTime	StartTime(nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(), 0, 0, 0);
	CTime	StopTime(nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(), 23, 59, 59);

	//开始时间初始化
	m_StartDay = StartTime;
	m_StartTime = StartTime;
	m_StopDay  = StopTime;
	m_StopTime  = StopTime;

	UpdateData(FALSE);

	return TRUE;
}

BOOL CDlgHistoryLogVideoError::LoadSkin()
{
	m_btnSelectNode.LoadBitmap(IDB_BITMAP_SELECT_NODE);
	m_btnQuery.LoadBitmap(IDB_BITMAP_QUERY);


	m_ListCtrl_VidoeErrorLog.SetHeaderHeight(1.2f);          //设置头部高度
	m_ListCtrl_VidoeErrorLog.SetHeaderFontHW(12,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_ListCtrl_VidoeErrorLog.SetHeaderTextColor(RGB(58,77,93)); //设置头部字体颜色
	m_ListCtrl_VidoeErrorLog.SetHeaderBKColor(195,248,250,3); //设置头部背景色

	m_ListCtrl_VidoeErrorLog.SetTextColor(RGB(36, 162, 146));
	m_ListCtrl_VidoeErrorLog.SetBkColor(RGB(239, 246, 253));
	m_ListCtrl_VidoeErrorLog.SetTextBkColor(RGB(239, 246, 253));
	m_ListCtrl_VidoeErrorLog.SetRowHeigt(15);               //设置行高度
	m_ListCtrl_VidoeErrorLog.SetFontHW(12,0);               //设置字体高度，和宽度,0表示缺省宽度

// 	m_ListCtrl_VidoeErrorLog.SetHeaderHeight(1.2f);          //设置头部高度
// 	m_ListCtrl_VidoeErrorLog.SetHeaderFontHW(16,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
// 	m_ListCtrl_VidoeErrorLog.SetHeaderTextColor(RGB(58,77,93)); //设置头部字体颜色
// 	m_ListCtrl_VidoeErrorLog.SetHeaderBKColor(162,197,227,3); //设置头部背景色
// 
// 	m_ListCtrl_VidoeErrorLog.SetTextColor(RGB(36, 162, 146));
// 	m_ListCtrl_VidoeErrorLog.SetBkColor(RGB(239, 246, 253));
// 	m_ListCtrl_VidoeErrorLog.SetTextBkColor(RGB(239, 246, 253));
// 	m_ListCtrl_VidoeErrorLog.SetRowHeigt(25);               //设置行高度
// 	m_ListCtrl_VidoeErrorLog.SetFontHW(15,0);               //设置字体高度，和宽度,0表示缺省宽度

	return TRUE;
}

void CDlgHistoryLogVideoError::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//////////////////////////////////////////////////////////////////////////


}

BOOL CDlgHistoryLogVideoError::OnEraseBkgnd(CDC* pDC)
{
	//////////////////////////////////////////////////////////////////////////
	//CRect   rect;
	//GetClientRect(&rect);

	//pDC->FillRect (rect, &m_Brush);

	return TRUE;
}

void CDlgHistoryLogVideoError::OnBnClickedButtonHlQuery()
{
	UpdateData(true);

	int i = 0, j = 0;
	char s_starttime[128]={0};
	char s_stoptime[128]={0};

	CTime starttime(m_StartDay.GetYear(), m_StartDay.GetMonth(), m_StartDay.GetDay(),
		m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond());
	CTime stoptime(m_StopDay.GetYear(), m_StopDay.GetMonth(), m_StopDay.GetDay(),
		m_StopTime.GetHour(), m_StopTime.GetMinute(), m_StopTime.GetSecond());

	if (stoptime <= starttime)
	{
		MessageBox("时间选择错误：开始时间大于结束时间","视频监视");
		return;
	}

	sprintf(s_starttime, "%04d-%02d-%02d %02d:%02d:%02d",
		m_StartDay.GetYear(), m_StartDay.GetMonth(), m_StartDay.GetDay(),
		m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond());
	sprintf(s_stoptime, "%04d-%02d-%02d %02d:%02d:%02d",
		m_StopDay.GetYear(), m_StopDay.GetMonth(), m_StopDay.GetDay(),
		m_StopTime.GetHour(), m_StopTime.GetMinute(), m_StopTime.GetSecond());

	//鼠标为等待状态
	AfxGetApp()->DoWaitCursor(1); 

	m_ListCtrl_VidoeErrorLog.DeleteAllItems();

	int nSelectIndex = m_ComboType.GetCurSel();

	SearchAndSetHistoryListInfo("所有结点",s_starttime,s_stoptime);

	//恢复鼠标为正常状态
	AfxGetApp()->DoWaitCursor(0);
}

BOOL CDlgHistoryLogVideoError::SearchAndSetHistoryListInfo(char *szNode,char *szStartTime,char *szStopTime)
{
	if (szStartTime == NULL||szStopTime == NULL)
		return FALSE;

	try
	{
		char sql_buf[512] = {0};
		bool result=false;

		MYSQL_RES * res = NULL ;
		MYSQL_ROW	row ;

		char szYear[8] = {0};
		char szMonth[4] = {0};
		char szDay[4] = {0};
		int nYear = 2013;
		int nMonth = 1;
		int nDay = 1;

		char szType[64] = {0};
		char szSubType[64] = {0};
		char szStationName[256] = {0};
		char szStationCode[256] = {0};
		char szCameraName[256] = {0};
		char szCameraCode[256] = {0};
		char szState[64] = {0};
		char szReason[64] = {0};
		char szCheckTime[64] = {0};
		char szFlowValue[64] = {0};
		char szNodeName[256] = {0};
		char szClientIp[64] = {0};

		COLORREF BKColor;
		COLORREF TextColor;

		szYear[0] = szStartTime[0];
		szYear[1] = szStartTime[1];
		szYear[2] = szStartTime[2];
		szYear[3] = szStartTime[3];
		szYear[4] = 0;

		szMonth[0] = szStartTime[5];
		szMonth[1] = szStartTime[6];
		szMonth[2] = 0;

		szDay[0] = szStartTime[8];
		szDay[1] = szStartTime[9];
		szDay[2] = 0;

		nYear = atoi(szYear);
		nMonth = atoi(szMonth);
		nDay = atoi(szDay);

		BKColor = RGB(207,235,250);
		TextColor = RGB(168,0,0);

		int nCount = 0;

		int nState = 0;
		int nReason = 0;

		//读视频检测信息
		sprintf_s(sql_buf, sizeof(sql_buf),"SELECT node_name, station_name, station_code, camera_name, camera_code, state, reason,"
			"flow, time,client_ip FROM ct_history_video_error_%04d "
			"where time >= '%s' AND time <= '%s' ORDER BY id",nYear,szStartTime,szStopTime);

		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);

			while ( row = mysql_fetch_row( res ) )
			{
				if (nCount >= 100)
					break;

				if (strlen(row[0]) == 0||strlen(row[1]) == 0
					||strlen(row[3]) == 0||strlen(row[4]) == 0)
					continue;

				sprintf_s(szNodeName,"%s", row[0]);
				sprintf_s(szStationName, "%s", row[1]);
				sprintf_s(szStationCode, "%s", row[2]);
				sprintf_s(szCameraName,"%s",row[3]);
				sprintf_s(szCameraCode,"%s",row[4]);
				sprintf_s(szState,"%s",row[5]);
				sprintf_s(szReason,"%s",row[6]);
				sprintf_s(szFlowValue,"%s",row[7]);
				sprintf_s(szCheckTime, "%s", row[8]);
				sprintf_s(szClientIp,"%s",row[9]);

				//////////////////////////////////////////////////////////////////////////

				nState = atoi(szState);
				nReason = atoi(szReason);

				switch (nState)
				{
				case 2:
					{
						sprintf_s(szState,"呼叫终止");
					}
					break;

				default:
					break;
				}

				switch(nReason)
				{
				case E_RT_CALL_REASON_NORMAL:
					{
						sprintf_s(szReason,"正常状态迁移");
					}
					break;

				case E_RT_CALL_REASON_LOCAL_CANCEL:
					{
						sprintf_s(szReason,"本地取消呼叫");
					}
					break;

				case E_RT_CALL_REASON_REMOTE_CANCEL:
					{
						sprintf_s(szReason,"远端取消呼叫");
					}
					break;

				case E_RT_CALL_REASON_LOCAL_HANGUP:
					{
						sprintf_s(szReason,"本地挂机");
					}
					break;

				case E_RT_CALL_REASON_REMOTE_HANGUP:
					{
						sprintf_s(szReason,"远端挂机");
					}
					break;

				case E_RT_CALL_REASON_FAIL_REDIRECTED:
					{
						sprintf_s(szReason,"要求向另一地址呼叫");
					}
					break;

				case E_RT_CALL_REASON_FAIL_REJECT:
					{
						sprintf_s(szReason,"远端拒绝");
					}
					break;

				case  E_RT_CALL_REASON_FAIL_SERVER:
					{
						sprintf_s(szReason,"视频平台故障");
					}
					break;

				case E_RT_CALL_REASON_FAIL_GLOBAL:
					{
						sprintf_s(szReason,"网络故障");
					}
					break;

				case E_RT_CALL_REASON_NETWORK_ERROR:
					{
						sprintf_s(szReason,"网络故障");
					}
					break;

				case E_RT_CALL_REASON_WAIT_PICKUP_TIMEOUT:
					{
						sprintf_s(szReason,"等待远端摘机超时");
					}
					break;

				case E_RT_CALL_REASON_LOCAL_TIMEOUT:
					{
						sprintf_s(szReason,"本端处理超时");
					}
					break;

				case E_RT_CALL_REASON_LOCAL_FAILURE:
					{
						sprintf_s(szReason,"本端逻辑错误");
					}
					break;

				default:
					break;
				}
				//////////////////////////////////////////////////////////////////////////

				nCount++;

				m_ListCtrl_VidoeErrorLog.InsertItem(0,szCameraName);
				m_ListCtrl_VidoeErrorLog.SetItemText(0,1,szCameraCode);
				m_ListCtrl_VidoeErrorLog.SetItemText(0,2,szStationName);
				m_ListCtrl_VidoeErrorLog.SetItemText(0,3,szStationCode);
				m_ListCtrl_VidoeErrorLog.SetItemText(0,4,szState);
				m_ListCtrl_VidoeErrorLog.SetItemText(0,5,szReason);
				m_ListCtrl_VidoeErrorLog.SetItemText(0,6,szCheckTime);
				m_ListCtrl_VidoeErrorLog.SetItemText(0,7,szFlowValue);
				m_ListCtrl_VidoeErrorLog.SetItemText(0,8,szNodeName);
				m_ListCtrl_VidoeErrorLog.SetItemText(0,9,szClientIp);

				m_ListCtrl_VidoeErrorLog.SetRowColor(0,BKColor);
				m_ListCtrl_VidoeErrorLog.SetRowTextColor(0,TextColor);
			}
			mysql_free_result( res ) ;
		}

		return TRUE;
	}
	catch(...)
	{

	}

	return FALSE;
}

void CDlgHistoryLogVideoError::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	//////////////////////////////////////////////////////////////////////////
	CRect rect(0,0,0,0), rc;
	CWnd *pWnd;

	GetClientRect(rc);

	// *****************************************最上面的按钮
	pWnd = GetDlgItem(IDC_LIST_HISTORYLOG_VIDEO_ERROR); //系统日志
	if(pWnd)
	{
		rect.left = rc.left + 5;
		rect.right = rc.right - 5;
		rect.top = rc.top + 150;
		rect.bottom = rc.bottom - 5;

		pWnd->MoveWindow(rect);
	}
}

HBRUSH CDlgHistoryLogVideoError::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	//////////////////////////////////////////////////////////////////////////

	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(10,10,10));
		return m_Brush;
	}

	//////////////////////////////////////////////////////////////////////////
	return hbr;
}


void CDlgHistoryLogVideoError::OnDtnDatetimechangeDatetimepickerHlStartday(NMHDR *pNMHDR, LRESULT *pResult)
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
		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CDlgHistoryLogVideoError::OnDtnDatetimechangeDatetimepickerHlStarttime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);


	*pResult = 0;
}


void CDlgHistoryLogVideoError::OnDtnDatetimechangeDatetimepickerHlStopday(NMHDR *pNMHDR, LRESULT *pResult)
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
		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CDlgHistoryLogVideoError::OnDtnDatetimechangeDatetimepickerHlStoptime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);


	*pResult = 0;
}

void CDlgHistoryLogVideoError::OnBnClickedButtonHlChoosenode()
{

}

