// DlgPageViewInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgPageViewInfo.h"
#include "VEMCUCtlDlg.h"
#include "DlgDeviceOnlineInfo.h"
#include "DlgPageViewSetTime.h"


//////////////////////////////////////////////////////////////////////////
//巡视一页打开的摄像头数
int g_nPageViewOnePageMaxCameraNum = 4;

//////////////////////////////////////////////////////////////////////////
//摄像机状态
VIDEO_VIEW_REPORT_STATUS_STATION_DAILY_INFO g_VideoViewReportStatusStationDailyInfo;

//流量
VIDEO_VIEW_REPORT_STATUS_STATION_STREAM_INFO g_VideoViewReportStatusStationStreamInfo;


// CDlgPageViewInfo 对话框

IMPLEMENT_DYNAMIC(CDlgPageViewInfo, CDialog)

CDlgPageViewInfo::CDlgPageViewInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPageViewInfo::IDD, pParent)
{
	m_nPageType = 0;
	m_hTreeStationItem = NULL;
	m_hTreeCameraItem = NULL;
	m_nTotalPageNum = 0;
	m_nCurrentPageIndex = -1;
	m_nTotalCameraNum = 0;
	m_nCurrentCameraIndex = -1;
	m_bOnlineRateReportFlag = false;
	memset(m_szOnlineRateReportType,0,sizeof(m_szOnlineRateReportType));

	COLORREF color= RGB(218,255,251);
	m_brush.CreateSolidBrush(color); 

	memset(m_szClientIp,0,sizeof(m_szClientIp));
	memset(m_szDBServerIp,0,sizeof(m_szDBServerIp));
	m_nDBServerPort = 0;
	memset(m_szDBName,0,sizeof(m_szDBName));
	memset(m_szDBUserName,0,sizeof(m_szDBUserName));
	memset(m_szDBUserPassword,0,sizeof(m_szDBUserPassword));
	m_pMySql = NULL;
	m_dwPageViewTickCount = GetTickCount();

	m_hDeviceOnlineRateInfoThread = NULL;
	m_uDeviceOnlineRateInfoThreadID = 0;

	InitializeCriticalSection(&m_csPageViewInfoLock);
}

CDlgPageViewInfo::~CDlgPageViewInfo()
{
	m_nPageType = 0;
	m_hTreeStationItem = NULL;
	m_hTreeCameraItem = NULL;
	m_nTotalPageNum = 0;
	m_nCurrentPageIndex = -1;
	m_nTotalCameraNum = 0;
	m_nCurrentCameraIndex = -1;

	DisConnectMySql();
	memset(m_szClientIp,0,sizeof(m_szClientIp));
	memset(m_szDBServerIp,0,sizeof(m_szDBServerIp));
	m_nDBServerPort = 0;
	memset(m_szDBName,0,sizeof(m_szDBName));
	memset(m_szDBUserName,0,sizeof(m_szDBUserName));
	memset(m_szDBUserPassword,0,sizeof(m_szDBUserPassword));
	
	DeleteCriticalSection(&m_csPageViewInfoLock);
}

void CDlgPageViewInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_PRE_PAGE, m_btnPrePage);
	DDX_Control(pDX, IDC_BTN_NEXT_PAGE, m_btnNextPage);
	DDX_Control(pDX, IDC_BTN_EXPORT_REPORT, m_btnExportReport);
	DDX_Control(pDX, IDC_STATIC_STATION_INFO, m_staticStationInfo);
	DDX_Control(pDX, IDC_STATIC_PAGE_INFO, m_staticPageInfo);
	DDX_Control(pDX, IDC_BTN_DEVICE_INFO, m_btnDeviceInfo);
	DDX_Control(pDX, IDC_BTN_ONLINE_RATE_INFO, m_btnOnlineRateInfo);
	DDX_Control(pDX, IDC_BUTTON_SET_TIME, m_btnSetTime);
	DDX_Control(pDX, IDC_BUTTON_PAUSE_VIDEO, m_btnPauseVideo);
	DDX_Control(pDX, IDC_BUTTON_RESUME_VIDEO, m_btnResumeVideo);
	DDX_Control(pDX, IDC_BUTTON_STOP_VIDEO, m_btnStopVideo);
}

BEGIN_MESSAGE_MAP(CDlgPageViewInfo, CDialog)
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CDlgPageViewInfo::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CDlgPageViewInfo::OnBnClickedBtnNextPage)
	ON_BN_CLICKED(IDC_BTN_EXPORT_REPORT, &CDlgPageViewInfo::OnBnClickedBtnExportReport)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_DEVICE_INFO, &CDlgPageViewInfo::OnBnClickedBtnDeviceInfo)
	ON_BN_CLICKED(IDC_BTN_ONLINE_RATE_INFO, &CDlgPageViewInfo::OnBnClickedBtnOnlineRateInfo)
	ON_BN_CLICKED(IDC_BUTTON_SET_TIME, &CDlgPageViewInfo::OnBnClickedButtonSetTime)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE_VIDEO, &CDlgPageViewInfo::OnBnClickedButtonPauseVideo)
	ON_BN_CLICKED(IDC_BUTTON_RESUME_VIDEO, &CDlgPageViewInfo::OnBnClickedButtonResumeVideo)
	ON_BN_CLICKED(IDC_BUTTON_STOP_VIDEO, &CDlgPageViewInfo::OnBnClickedButtonStopVideo)
END_MESSAGE_MAP()


// CDlgPageViewInfo 消息处理程序

BOOL CDlgPageViewInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//////////////////////////////////////////////////////////////////////////
	m_staticStationInfo.SetTransparent(TRUE);						//设置背景透明色
	m_staticStationInfo.SetTextColor(RGB(23, 92, 85));		    //设置字体颜色
	m_staticStationInfo.SetText("变电站名称");
	
	m_staticPageInfo.SetTransparent(TRUE);				    //设置背景透明色
	m_staticPageInfo.SetTextColor(RGB(23, 92, 85));		//设置字体颜色
	m_staticPageInfo.SetText("页面信息");

	LoadSkin();

	m_tipTool.Create(this);
	m_tipTool.Activate(TRUE);   
	m_tipTool.SetTipBkColor(RGB(255,255,255));//背景颜色
	m_tipTool.SetTipTextColor(RGB(100,100,100));//字体颜色
	m_tipTool.SetDelayTime(100);   //显示时间   

	m_tipTool.AddTool(GetDlgItem(IDC_BTN_PRE_PAGE),"快捷键ALT+PgUp");
	m_tipTool.AddTool(GetDlgItem(IDC_BTN_NEXT_PAGE),"快捷键ALT+PgDn");
	m_tipTool.AddTool(GetDlgItem(IDC_BTN_EXPORT_REPORT),"快捷键ALT+Home");

	m_btnPauseVideo.EnableWindow(FALSE);
	m_btnResumeVideo.EnableWindow(FALSE);
	m_btnStopVideo.EnableWindow(FALSE);

	return TRUE; 
}

//上一页
void CDlgPageViewInfo::OnBnClickedBtnPrePage()
{
	if (g_pMainDlg == NULL||g_pMainDlg->m_pDlgPageServer == NULL)
		return;

	//操作太快
	if(CheckPageViewTickCount() == FALSE)
		return;

	//判断打开视频线程的队列数量
	int nThreadVideoServerNumber = g_ThreadVideoOperateNumberInfo.GetThreadDeviceVideoNumer();
	if (g_pMainDlg != NULL&&nThreadVideoServerNumber > g_pMainDlg->m_FormsNum)
		return;

	if (m_nCurrentPageIndex <= 0)
	{
		if (m_hTreeStationItem != NULL)
		{
			if(OpenPreStationFirstPage() < 0)
			{
				StopAutoVideoPageView();
			}
		}
		else
		{
			StopAutoVideoPageView();
		}
		return;
	}

	if (m_nCurrentCameraIndex  < g_nPageViewOnePageMaxCameraNum)
		return;

	g_pMainDlg->SetForms(g_nPageViewOnePageMaxCameraNum);

	_T_NODE_INFO *pNodeInfo = NULL;

	int nCount = 0;

	if (m_hTreeStationItem == NULL||m_hTreeCameraItem == NULL)
		return;

	HTREEITEM hChildItem = m_hTreeCameraItem;

	while(hChildItem&&nCount < g_nPageViewOnePageMaxCameraNum)
	{
		nCount++;
		hChildItem = g_pMainDlg->m_pDlgPageServer->m_trServer.GetPrevSiblingItem(hChildItem);
	}

	if (hChildItem == NULL)
		return;

	m_nCurrentPageIndex--;
	m_nCurrentCameraIndex = m_nCurrentCameraIndex-g_nPageViewOnePageMaxCameraNum;

	m_hTreeCameraItem = hChildItem;

	char szPageInfo[256] = {0};

	if (m_nTotalPageNum == 0)
	{
		sprintf(szPageInfo,"总共0页，当前0页");
		m_staticPageInfo.SetText(szPageInfo);
	}
	else
	{
		sprintf(szPageInfo,"总共%d页，当前%d页",m_nTotalPageNum,m_nCurrentPageIndex+1);
		m_staticPageInfo.SetText(szPageInfo);

		g_pMainDlg->OpenCurPageVideo(m_hTreeCameraItem);
	}
}

//下一页
void CDlgPageViewInfo::OnBnClickedBtnNextPage()
{
	if (g_pMainDlg == NULL||g_pMainDlg->m_pDlgPageServer == NULL)
		return;

	//操作太快
	if(CheckPageViewTickCount() == FALSE)
		return;

	//判断打开视频线程的队列数量
	int nThreadVideoServerNumber = g_ThreadVideoOperateNumberInfo.GetThreadDeviceVideoNumer();
	if (g_pMainDlg != NULL&&nThreadVideoServerNumber > g_pMainDlg->m_FormsNum)
		return;

	if (m_nCurrentPageIndex >= m_nTotalPageNum-1)
	{
		if (m_hTreeStationItem != NULL)
		{
			if (OpenNextStationFirstPage() < 0)
			{
				StopAutoVideoPageView();
			}
		}
		else
		{
			StopAutoVideoPageView();
		}
		return;
	}

	if (m_nCurrentCameraIndex + g_nPageViewOnePageMaxCameraNum >= m_nTotalCameraNum)
		return;

	g_pMainDlg->SetForms(g_nPageViewOnePageMaxCameraNum);

	_T_NODE_INFO *pNodeInfo = NULL;

	int nCount = 0;

	if (m_hTreeStationItem == NULL||m_hTreeCameraItem == NULL)
		return;

	HTREEITEM hChildItem = m_hTreeCameraItem;

	while(hChildItem&&nCount < g_nPageViewOnePageMaxCameraNum)
	{
		nCount++;
		hChildItem = g_pMainDlg->m_pDlgPageServer->m_trServer.GetNextSiblingItem(hChildItem);
	}

	if (hChildItem == NULL)
		return;

	m_nCurrentPageIndex++;
	m_nCurrentCameraIndex = m_nCurrentCameraIndex+g_nPageViewOnePageMaxCameraNum;

	m_hTreeCameraItem = hChildItem;

	char szPageInfo[256] = {0};
	sprintf(szPageInfo,"总共%d页，当前%d页",m_nTotalPageNum,m_nCurrentPageIndex+1);
	m_staticPageInfo.SetText(szPageInfo);

	g_pMainDlg->OpenCurPageVideo(m_hTreeCameraItem);
}

//设置人工自动巡视时间间隔
void CDlgPageViewInfo::OnBnClickedButtonSetTime()
{
	CDlgPageViewSetTime DlgPageViewSetTime;

	if (g_VideoPageViewInfo.nStatus == AUTOVIDEO_PAGEVIEW_STATUS_RUNNING)
	{
		MessageBox("当前处于人工自动巡视状态","视频监视",MB_ICONWARNING);
		return;
	}

	if(DlgPageViewSetTime.DoModal() == IDOK)
	{

	}
}

//暂停人工自动巡视
void CDlgPageViewInfo::OnBnClickedButtonPauseVideo()
{
	PauseAutoVideoPageView();
}

//继续人工自动巡视
void CDlgPageViewInfo::OnBnClickedButtonResumeVideo()
{
	ResumeAutoVideoPageView();
}

//停止人工自动巡视
void CDlgPageViewInfo::OnBnClickedButtonStopVideo()
{
	StopAutoVideoPageView();
}

int CDlgPageViewInfo::OpenPreStationFirstPage()
{
	int nResult = -1;

	if (m_hTreeStationItem == NULL||g_pMainDlg == NULL||g_pMainDlg->m_pDlgPageServer == NULL)
		return nResult;

	_T_NODE_INFO *pNodeInfo = NULL;

	HTREEITEM hTreeParentItem = g_pMainDlg->m_pDlgPageServer->m_trServer.GetParentItem(m_hTreeStationItem);
	HTREEITEM hTreeStationItem = g_pMainDlg->m_pDlgPageServer->m_trServer.GetPrevSiblingItem(m_hTreeStationItem);
	if (hTreeStationItem == NULL)
	{
		if (hTreeParentItem != NULL)
		{
			hTreeParentItem = g_pMainDlg->m_pDlgPageServer->m_trServer.GetPrevSiblingItem(hTreeParentItem);
			if (hTreeParentItem == NULL)
				return nResult;

			hTreeStationItem = g_pMainDlg->m_pDlgPageServer->m_trServer.GetChildItem(hTreeParentItem);
			if (hTreeStationItem == NULL)
				return nResult;

			pNodeInfo = (_T_NODE_INFO *)g_pMainDlg->m_pDlgPageServer->m_trServer.GetItemData(hTreeStationItem);

			if (pNodeInfo == NULL||pNodeInfo->node_type != 2)
				return nResult;
		}
		else
		{
			return nResult;
		}
	}

	OpenFirstPageVideo(hTreeStationItem);

	return 0;
}

int CDlgPageViewInfo::OpenNextStationFirstPage()
{
	int nResult = -1;

	if (m_hTreeStationItem == NULL||g_pMainDlg == NULL||g_pMainDlg->m_pDlgPageServer == NULL)
		return nResult;

	_T_NODE_INFO *pNodeInfo = NULL;

	HTREEITEM hTreeParentItem = g_pMainDlg->m_pDlgPageServer->m_trServer.GetParentItem(m_hTreeStationItem);
	HTREEITEM hTreeStationItem = g_pMainDlg->m_pDlgPageServer->m_trServer.GetNextSiblingItem(m_hTreeStationItem);
	
	if (hTreeStationItem == NULL)
	{
		if (hTreeParentItem != NULL)
		{
			hTreeParentItem = g_pMainDlg->m_pDlgPageServer->m_trServer.GetNextSiblingItem(hTreeParentItem);
			if (hTreeParentItem == NULL)
				return nResult;

			hTreeStationItem = g_pMainDlg->m_pDlgPageServer->m_trServer.GetChildItem(hTreeParentItem);
			if (hTreeStationItem == NULL)
				return nResult;

			pNodeInfo = (_T_NODE_INFO *)g_pMainDlg->m_pDlgPageServer->m_trServer.GetItemData(hTreeStationItem);

			if (pNodeInfo == NULL||pNodeInfo->node_type != 2)
				return nResult;
		}
		else
		{
			return nResult;
		}	
	}

	OpenFirstPageVideo(hTreeStationItem);

	return 0;
}

//开始人工自动视频巡视
void CDlgPageViewInfo::StartAutoVideoPageView()
{
	try
	{
		if (g_VideoPageViewInfo.nStatus != AUTOVIDEO_PAGEVIEW_STATUS_RUNNING)
		{
			g_VideoPageViewInfo.nStatus = AUTOVIDEO_PAGEVIEW_STATUS_RUNNING;

			m_btnPrePage.EnableWindow(FALSE);
			m_btnNextPage.EnableWindow(FALSE);
			m_btnSetTime.EnableWindow(FALSE);
			m_btnPauseVideo.EnableWindow(TRUE);
			m_btnResumeVideo.EnableWindow(FALSE);
			m_btnStopVideo.EnableWindow(TRUE);

			m_btnPrePage.Invalidate(TRUE);
			m_btnNextPage.Invalidate(TRUE);
			m_btnSetTime.Invalidate(TRUE);
			m_btnPauseVideo.Invalidate(TRUE);
			m_btnResumeVideo.Invalidate(TRUE);
			m_btnStopVideo.Invalidate(TRUE);

			if (g_pMainDlg != NULL)
			{
				g_pMainDlg->SetTimer(TIMER_VIDEO_PAGEVIEW_AUTOINFO,g_VideoPageViewInfo.nTimeSpan*1000,NULL);
			}	
		}
	}
	catch(...)
	{

	}
}

//暂停人工自动视频巡视
void CDlgPageViewInfo::PauseAutoVideoPageView()
{
	try
	{
		if (g_VideoPageViewInfo.nStatus == AUTOVIDEO_PAGEVIEW_STATUS_RUNNING)
		{
			g_VideoPageViewInfo.nStatus = AUTOVIDEO_PAGEVIEW_STATUS_PAUSE;

			m_btnPrePage.EnableWindow(TRUE);
			m_btnNextPage.EnableWindow(TRUE);
			m_btnSetTime.EnableWindow(FALSE);
			m_btnPauseVideo.EnableWindow(FALSE);
			m_btnResumeVideo.EnableWindow(TRUE);
			m_btnStopVideo.EnableWindow(TRUE);

			m_btnPrePage.Invalidate(TRUE);
			m_btnNextPage.Invalidate(TRUE);
			m_btnSetTime.Invalidate(TRUE);
			m_btnPauseVideo.Invalidate(TRUE);
			m_btnResumeVideo.Invalidate(TRUE);
			m_btnStopVideo.Invalidate(TRUE);
		}

		if (g_pMainDlg != NULL)
		{
			g_pMainDlg->KillTimer(TIMER_VIDEO_PAGEVIEW_AUTOINFO);
		}
	}
	catch(...)
	{

	}
}

//继续人工自动视频巡视
void CDlgPageViewInfo::ResumeAutoVideoPageView()
{
	try
	{
		if (g_VideoPageViewInfo.nStatus == AUTOVIDEO_PAGEVIEW_STATUS_PAUSE)
		{
			g_VideoPageViewInfo.nStatus = AUTOVIDEO_PAGEVIEW_STATUS_RUNNING;

			m_btnPrePage.EnableWindow(FALSE);
			m_btnNextPage.EnableWindow(FALSE);
			m_btnSetTime.EnableWindow(FALSE);
			m_btnPauseVideo.EnableWindow(TRUE);
			m_btnResumeVideo.EnableWindow(FALSE);
			m_btnStopVideo.EnableWindow(TRUE);

			m_btnPrePage.Invalidate(TRUE);
			m_btnNextPage.Invalidate(TRUE);
			m_btnSetTime.Invalidate(TRUE);
			m_btnPauseVideo.Invalidate(TRUE);
			m_btnResumeVideo.Invalidate(TRUE);
			m_btnStopVideo.Invalidate(TRUE);

			if (g_pMainDlg != NULL)
			{
				g_pMainDlg->SetTimer(TIMER_VIDEO_PAGEVIEW_AUTOINFO,g_VideoPageViewInfo.nTimeSpan*1000,NULL);
			}
		}
	}
	catch(...)
	{

	}
}

//停止人工自动视频巡视
void CDlgPageViewInfo::StopAutoVideoPageView()
{
	try
	{
		if (g_VideoPageViewInfo.nStatus != AUTOVIDEO_PAGEVIEW_STATUS_INIT)
		{
			g_VideoPageViewInfo.nStatus = AUTOVIDEO_PAGEVIEW_STATUS_INIT;

			m_btnPrePage.EnableWindow(TRUE);
			m_btnNextPage.EnableWindow(TRUE);
			m_btnSetTime.EnableWindow(TRUE);
			m_btnPauseVideo.EnableWindow(FALSE);
			m_btnResumeVideo.EnableWindow(FALSE);
			m_btnStopVideo.EnableWindow(FALSE);

			m_btnPrePage.Invalidate(TRUE);
			m_btnNextPage.Invalidate(TRUE);
			m_btnSetTime.Invalidate(TRUE);
			m_btnPauseVideo.Invalidate(TRUE);
			m_btnResumeVideo.Invalidate(TRUE);
			m_btnStopVideo.Invalidate(TRUE);

			if (g_pMainDlg != NULL)
			{
				g_pMainDlg->KillTimer(TIMER_VIDEO_PAGEVIEW_AUTOINFO);
			}
		}
	}
	catch(...)
	{

	}
}

BOOL CDlgPageViewInfo::GetStationInfo(int &nAllStationNum,int &nOffStationNum,int nStationType)
{
	nAllStationNum = 0;
	nOffStationNum = 0;

	MYSQL_RES * res = NULL;
	MYSQL_ROW	row ;

	char sql_buf[1024] = {0};
	char Temp[256] = {0};

	switch (nStationType)
	{
	case VIEW_INFO_STATION_TYPE_ALL:
		{
			sprintf_s(sql_buf, "SELECT station_id,notes FROM ob_d5000_station ORDER BY station_id ");
		}
		break;

	case VIEW_INFO_STATION_TYPE_500KV:
		{
			sprintf_s(sql_buf, "SELECT station_id,notes FROM ob_d5000_station WHERE voltage_class=500 ORDER BY station_id ");
		}
		break;

	case VIEW_INFO_STATION_TYPE_220KV:
		{
			sprintf_s(sql_buf, "SELECT station_id,notes FROM ob_d5000_station WHERE voltage_class=220 ORDER BY station_id ");
		}
		break;

	case VIEW_INFO_STATION_TYPE_110KV:
		{
			sprintf_s(sql_buf, "SELECT station_id,notes FROM ob_d5000_station WHERE voltage_class=110 ORDER BY station_id ");
		}
		break;

	case VIEW_INFO_STATION_TYPE_35KV:
		{
			sprintf_s(sql_buf, "SELECT station_id,notes FROM ob_d5000_station WHERE voltage_class=35 ORDER BY station_id ");
		}
		break;

	default:
		return FALSE;
	}

	Lock();
	if (!mysql_query(m_pMySql, sql_buf))
	{
		res = mysql_store_result(m_pMySql);
		while (row = mysql_fetch_row(res))
		{
			strcpy_s(Temp,sizeof(Temp),row[1]);
			if (strcmp(Temp,"0") == 0)
			{
				nOffStationNum++;
			}
			nAllStationNum++;
		}
		mysql_free_result(res) ;

		Unlock();
		return TRUE;
	}

	Unlock();
	return FALSE;
}

BOOL CDlgPageViewInfo::GetStationCameraInfo(int nStationId,int &nAllCameraNum,int &nProblemCameraNum,int &nStationStatus)
{
	if (nStationId <= 0)
		return FALSE;

	nAllCameraNum = 0;
	nProblemCameraNum = 0;
	nStationStatus = 0;

	MYSQL_RES * res = NULL;
	MYSQL_ROW	row ;

	char sql_buf[1024] = {0};

	sprintf_s(sql_buf, "SELECT COUNT(camera_id) FROM ob_d5000_camera_station_%d ",nStationId);

	Lock();
	if (!mysql_query(m_pMySql, sql_buf))
	{
		res = mysql_store_result(m_pMySql);
		while (row = mysql_fetch_row(res))
		{
			nAllCameraNum = atoi(row[0]);
		}
		mysql_free_result(res) ;
		
		Unlock();
	}
	else
	{
		Unlock();
		return FALSE;
	}

	sprintf_s(sql_buf, "SELECT COUNT(camera_id) FROM ob_d5000_camera_station_%d WHERE camera_status=0 or camera_status=3 or streamless=1 ",nStationId);

	Lock();
	if (!mysql_query(m_pMySql, sql_buf))
	{
		res = mysql_store_result(m_pMySql);
		while (row = mysql_fetch_row(res))
		{
			nProblemCameraNum = atoi(row[0]);
		}
		mysql_free_result(res) ;

		Unlock();
	}
	else
	{
		Unlock();
		return FALSE;
	}

	if (nAllCameraNum > nProblemCameraNum)
	{
		nStationStatus = 1;
		sprintf_s(sql_buf, "UPDATE ob_d5000_station SET station_video_status=1 WHERE station_id=%d ",nStationId);

		Lock();
		mysql_query(m_pMySql, sql_buf);
		Unlock();
	}

	if (nAllCameraNum == 0||nAllCameraNum == nProblemCameraNum)
	{
		sprintf_s(sql_buf, "UPDATE ob_d5000_station SET station_video_status=0 WHERE station_id=%d ",nStationId);
		
		Lock();
		mysql_query(m_pMySql, sql_buf);
		Unlock();
	}

	return TRUE;
}

//导出报表
void CDlgPageViewInfo::OnBnClickedBtnExportReport()
{
	if (g_pMainDlg != NULL)
	{
		g_pMainDlg->CheckAndOpenVideoExcelReport();
	}
}

void CDlgPageViewInfo::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//////////////////////////////////////////////////////////////////////////
	if (GetDlgItem(IDC_BTN_EXPORT_REPORT)->GetSafeHwnd())
	{
		RECT rc;		
		RECT rect;
		CWnd *pWnd;

		//布置
		rc.top = 8;
		rc.bottom = cy-2;
		rc.left = 16;
		rc.right = cx-2;

		pWnd = GetDlgItem(IDC_BTN_EXPORT_REPORT);
		if (pWnd)
		{
			rect.right = rc.right-6;
			rect.left = rect.right - 80 ;			
			rect.top= rc.top;
			rect.bottom=rect.top+36;

			pWnd->MoveWindow(&rect);		
		}

		pWnd = GetDlgItem(IDC_BTN_DEVICE_INFO);
		if (pWnd)
		{
			rect.right = rect.left-6;
			rect.left = rect.right-80 ;			
			rect.top= rc.top;
			rect.bottom=rect.top + 36;
			pWnd->MoveWindow(&rect);		
		}

		pWnd = GetDlgItem(IDC_BTN_ONLINE_RATE_INFO);
		if (pWnd)
		{
			rect.right = rect.left-6;
			rect.left = rect.right-80 ;			
			rect.top= rc.top;
			rect.bottom=rect.top + 36;
			pWnd->MoveWindow(&rect);		
		}

		pWnd = GetDlgItem(IDC_STATIC_PAGE_INFO);
		if (pWnd != NULL)
		{
			rect.right = rect.left-6;
			rect.left = rect.right-160 ;			
			rect.top= rc.top;
			rect.bottom=rect.top + 36;
			pWnd->MoveWindow(&rect);	
		}
	}
}

BOOL CDlgPageViewInfo::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect (&rect);

	pDC->FillRect (rect, &m_brush);

	return true;
}

HBRUSH CDlgPageViewInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	return hbr;
}

BOOL CDlgPageViewInfo::OpenFirstPageVideo(HTREEITEM hTreeStationItem)
{
	try
	{
		if (hTreeStationItem == NULL)
			return FALSE;

		if (g_pMainDlg == NULL||g_pMainDlg->m_pDlgPageServer == NULL)
			return FALSE;

		if (g_pMainDlg != NULL)
		{
			g_pMainDlg->SetForms(g_nPageViewOnePageMaxCameraNum);
		}

		m_nTotalCameraNum = 0;
		m_nTotalPageNum = 0;

		char szPathName [256] = {0};

		_T_NODE_INFO *pNodeInfo = NULL;

		pNodeInfo = (_T_NODE_INFO *)g_pMainDlg->m_pDlgPageServer->m_trServer.GetItemData(hTreeStationItem);

		if (pNodeInfo == NULL||pNodeInfo->node_type != 2)
			return FALSE;

		//记录当前的人工巡视站点
		strcpy_s(g_VideoPageViewInfo.szStationName,sizeof(g_VideoPageViewInfo.szStationName)-1,pNodeInfo->node_station);

		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\VEMCUCtl.ini");
		WritePrivateProfileString("PAGEVIEWINFO", "PageViewPreStation", g_VideoPageViewInfo.szStationName, szPathName);

		if (g_pMainDlg->m_pDlgPageServer->m_trServer.ItemHasChildren(hTreeStationItem) == FALSE)
		{
			g_pMainDlg->m_pDlgPageServer->OnTreeServerStationFlashInfo(hTreeStationItem);
		}

		m_hTreeCameraItem = g_pMainDlg->m_pDlgPageServer->m_trServer.GetChildItem(hTreeStationItem);

		m_staticStationInfo.SetText(pNodeInfo->node_station);

		m_hTreeStationItem = hTreeStationItem;

		HTREEITEM hChildItem = m_hTreeCameraItem;

		while(hChildItem)
		{
			m_nTotalCameraNum++;
			hChildItem = g_pMainDlg->m_pDlgPageServer->m_trServer.GetNextSiblingItem(hChildItem);
		}

		m_nTotalPageNum = (m_nTotalCameraNum+g_nPageViewOnePageMaxCameraNum-1)/g_nPageViewOnePageMaxCameraNum;

		m_nCurrentPageIndex = 0;
		m_nCurrentCameraIndex = 0;

		char szPageInfo[256] = {0};

		if (m_nTotalPageNum == 0)
		{
			sprintf(szPageInfo,"总共0页，当前0页");
			m_staticPageInfo.SetText(szPageInfo);
		}
		else
		{
			sprintf(szPageInfo,"总共%d页，当前%d页",m_nTotalPageNum,m_nCurrentPageIndex+1);
			m_staticPageInfo.SetText(szPageInfo);

			g_pMainDlg->OpenCurPageVideo(m_hTreeCameraItem);
		}

		return TRUE;
	}
	catch(...)
	{

	}

	return FALSE;
}

BOOL CDlgPageViewInfo::SetPageViewOnePageMaxCameraNum(int nNum)
{
	if (g_nPageViewOnePageMaxCameraNum == nNum)
		return TRUE;

	m_hTreeStationItem = NULL;
	m_hTreeCameraItem = NULL;
	m_nTotalPageNum = 0;
	m_nCurrentPageIndex = 0;
	m_nTotalCameraNum = 0;
	m_nCurrentCameraIndex = 0;

	g_nPageViewOnePageMaxCameraNum = nNum;

	m_staticPageInfo.SetText("页面信息");
	m_staticStationInfo.SetText("变电站名称");

	return TRUE;
}

bool CDlgPageViewInfo::Lock()
{
	EnterCriticalSection(&m_csPageViewInfoLock);
	return true;
}

bool CDlgPageViewInfo::Unlock()
{
	LeaveCriticalSection(&m_csPageViewInfoLock);
	return true;
}

//成员变量赋值
bool CDlgPageViewInfo::SetPageViewInfo(char *pClientIp,char *pDBServerIp,int nDBServerPort,char *pDBName,char *pDBUserName,char *pDBUserPassword)
{
	if (pClientIp != NULL)
	{
		strcpy_s(m_szClientIp,sizeof(m_szClientIp),pClientIp);
	}

	if (pDBServerIp != NULL)
	{
		strcpy_s(m_szDBServerIp,sizeof(m_szDBServerIp),pDBServerIp);
	}

	if (nDBServerPort > 0)
	{
		m_nDBServerPort = nDBServerPort;
	}

	if (pDBName != NULL)
	{
		strcpy_s(m_szDBName,sizeof(m_szDBName),pDBName);
	}

	if (pDBUserName != NULL)
	{
		strcpy_s(m_szDBUserName,sizeof(m_szDBUserName),pDBUserName);
	}

	if (pDBUserPassword != NULL)
	{
		strcpy_s(m_szDBUserPassword,sizeof(m_szDBUserPassword),pDBUserPassword);
	}

	return true;
}

// 连接数据库
bool CDlgPageViewInfo::ConnectMySql(void)
{
	Lock();

	if (strlen(m_szDBServerIp) == 0||m_nDBServerPort == 0
		||strlen(m_szDBName) == 0||strlen(m_szDBUserName) == 0
		||strlen(m_szDBUserPassword) == 0)
	{
		Unlock();
		return false;
	}

	do 
	{
		try
		{
			if (m_pMySql != NULL)
			{
				mysql_close(m_pMySql);
				m_pMySql = NULL;
				TRACE("断开mysql连接!");
			}

			m_pMySql = mysql_init((MYSQL*)NULL);
			if (m_pMySql == NULL)
			{
				TRACE("初始化mysql失败!\n");
				break;
			}

			if (mysql_real_connect(m_pMySql, m_szDBServerIp, m_szDBUserName, m_szDBUserPassword, m_szDBName, m_nDBServerPort, NULL, 0))
			{
				if (mysql_select_db(m_pMySql, m_szDBName) < 0)
				{
					TRACE("已成功连接mysql, 但选择数据库出错!\n");
					mysql_close(m_pMySql);
					m_pMySql = NULL;
					break;
				}

				int nValue = 1;
				mysql_options(m_pMySql,MYSQL_OPT_RECONNECT,(char *)&nValue);

				mysql_query(m_pMySql,"SET NAMES GB2312");
				TRACE("已成功连接mysql!\n");

				Unlock();
				return true;
			}
			else
			{
				TRACE("无法连接到数据库: Error=%s\n",mysql_error(m_pMySql));
				mysql_close(m_pMySql);
				m_pMySql = NULL;
				TRACE("连接mysql失败!\n");
				break;
			}
		}
		catch(...)
		{
			m_pMySql = NULL;
		}
	} while (false);

	Unlock();
	return false;
}

BOOL CDlgPageViewInfo::LoadSkin()
{
	m_btnPrePage.LoadBitmap(IDB_BITMAP_PREV_PAGE_BK,0);
	m_btnNextPage.LoadBitmap(IDB_BITMAP_NEXT_PAGE_BK,0);
	m_btnExportReport.LoadBitmap(IDB_BITMAP_DEVICE_REPORT_BK,0);
	m_btnDeviceInfo.LoadBitmap(IDB_BITMAP_BTN_RECORD_REPORT_BK,0);
	m_btnOnlineRateInfo.LoadBitmap(IDB_BITMAP_BTN_ONLINE_RATE,0);
	m_btnSetTime.LoadBitmap(IDB_BITMAP_BTN_SET_TIME,0);
	m_btnPauseVideo.LoadBitmap(IDB_BITMAP_BTN_PAUSE_PAGEVIEW,0);
	m_btnResumeVideo.LoadBitmap(IDB_BITMAP_BTN_RESUME_PAGEVIEW,0);
	m_btnStopVideo.LoadBitmap(IDB_BITMAP_BTN_STOP_PAGEVIEW,0);

	return TRUE;
}

// 断开数据库
bool CDlgPageViewInfo::DisConnectMySql(void)
{
	Lock();

	try
	{
		if (m_pMySql != NULL)
		{
			mysql_close(m_pMySql);
			m_pMySql = NULL;
			TRACE("断开mysql连接!");
		}
		Unlock();
		return true;
	}
	catch(...)
	{

	}
	Unlock();
	return false;
}

BOOL CDlgPageViewInfo::PreTranslateMessage(MSG* pMsg)
{
	m_tipTool.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgPageViewInfo::OnBnClickedBtnDeviceInfo()
{
	if (g_pMainDlg != NULL)
	{
		g_pMainDlg->CheckAndOpenVideoExcelRecordReport();
	}
}

int CDlgPageViewInfo::GetStationNodeInfo(char *szTypeName,int *nNodeId,int nMaxNum,int nLineNodeId)
{
	try
	{
		if (szTypeName == NULL||strlen(szTypeName) == 0||nNodeId == NULL||nMaxNum <= 0)
			return 0;

		MYSQL_RES * res = NULL;
		MYSQL_ROW	row ;

		char sql_buf[1024] = {0};
		int nCount = 0;

		if (strcmp(szTypeName,"全部站点") == 0)
		{
			sprintf_s(sql_buf, "SELECT node_id FROM ob_d5000_node_relation WHERE node_id!=parent_id AND parent_id!=%d ORDER BY node_id ",nLineNodeId);
		}
		else
		{
			sprintf_s(sql_buf, "SELECT node_id FROM ob_d5000_node_relation WHERE node_name='%s' AND parent_id!=%d ORDER BY node_id ",szTypeName,nLineNodeId);
		}

		Lock();
		if (!mysql_query(m_pMySql, sql_buf))
		{
			res = mysql_store_result(m_pMySql);
			while (row = mysql_fetch_row(res))
			{
				if (nCount >= nMaxNum)
					break;
				nNodeId[nCount++] = atoi(row[0]);
			}
			mysql_free_result(res) ;
		}
		Unlock();
		return nCount;
	}
	catch(...)
	{

	}
	return 0;
}

int CDlgPageViewInfo::GetLineNodeInfo(char *szTypeName,int *nNodeId,int nMaxNum,int nLineNodeId)
{
	try
	{
		if (szTypeName == NULL||strlen(szTypeName) == 0||nNodeId == NULL||nMaxNum <= 0)
			return 0;

		MYSQL_RES * res = NULL;
		MYSQL_ROW	row ;

		char sql_buf[1024] = {0};
		int nCount = 0;

		if (strcmp(szTypeName,"全部站点") == 0)
		{
			sprintf_s(sql_buf, "SELECT node_id FROM ob_d5000_node_relation WHERE node_id!=parent_id AND parent_id=%d ORDER BY node_id ",nLineNodeId);
		}
		else
		{
			sprintf_s(sql_buf, "SELECT node_id FROM ob_d5000_node_relation WHERE node_name='%s' AND parent_id=%d ORDER BY node_id ",szTypeName,nLineNodeId);
		}

		Lock();
		if (!mysql_query(m_pMySql, sql_buf))
		{
			res = mysql_store_result(m_pMySql);
			while (row = mysql_fetch_row(res))
			{
				if (nCount >= nMaxNum)
					break;
				nNodeId[nCount++] = atoi(row[0]);
			}
			mysql_free_result(res) ;
		}
		Unlock();
		return nCount;
	}
	catch(...)
	{

	}
	return 0;
}

int CDlgPageViewInfo::GetLineNodeId()
{
	try
	{
		MYSQL_RES * res = NULL;
		MYSQL_ROW	row ;

		char sql_buf[1024] = {0};
		int nNodeId = 0;

		sprintf_s(sql_buf, "SELECT node_id FROM ob_d5000_node_relation WHERE node_name='输电' ORDER BY node_id ");

		Lock();
		if (!mysql_query(m_pMySql, sql_buf))
		{
			res = mysql_store_result(m_pMySql);
			if (row = mysql_fetch_row(res))
			{
				nNodeId = atoi(row[0]);
			}
			mysql_free_result(res) ;
		}
		Unlock();
		return nNodeId;
	}
	catch(...)
	{

	}
	return 0;
}

int CDlgPageViewInfo::Get500kVStationCameraInfoOnlineCameraNumFromHistory()
{
	int i = 0;
	int nOnlineCameraSum = 0;
	int nOnlineCameraNum = 0;

	char sql_buf[1204] = {0};

	MYSQL_RES * res = NULL;
	MYSQL_ROW row ;

	SYSTEMTIME sysTm;

	GetLocalTime(&sysTm);

	for (i = 0;i < g_nStation_Num;i++)
	{
		if (m_bOnlineRateReportFlag == false)
			return -1;

		g_pDlgDeviceOnlineInfo->PostMessage(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE,40+i*60/g_nStation_Num,9999);

		if (g_tStation_Info[i].voltage_class == 500)
		{
			memset(sql_buf,0,sizeof(sql_buf));
			nOnlineCameraNum = 0;

			sprintf_s(sql_buf,sizeof(sql_buf),"SELECT COUNT(id) FROM ct_user_station_view_history_18_%04d%02d WHERE station_id=%d AND day=%d AND (status=1 OR status=2) ",sysTm.wYear,sysTm.wMonth,g_tStation_Info[i].station_id,sysTm.wDay);

			Lock();
			if (!mysql_query(m_pMySql, sql_buf))
			{
				res = mysql_store_result(m_pMySql);

				while ( row = mysql_fetch_row( res ) )
				{
					nOnlineCameraNum = atoi(row[0]);
				}

				mysql_free_result(res) ;

				nOnlineCameraSum += nOnlineCameraNum;
			}
			Unlock();
		}
	}

	return nOnlineCameraSum;
}

int CDlgPageViewInfo::Get220kVStationCameraInfoOnlineCameraNumFromHistory()
{
	int i = 0;
	int nOnlineCameraSum = 0;
	int nOnlineCameraNum = 0;

	char sql_buf[1204] = {0};
	
	MYSQL_RES * res = NULL;
	MYSQL_ROW row ;

	SYSTEMTIME sysTm;

	GetLocalTime(&sysTm);

	for (i = 0;i < g_nStation_Num;i++)
	{
		if (m_bOnlineRateReportFlag == false)
			return -1;

		g_pDlgDeviceOnlineInfo->PostMessage(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE,40+i*60/g_nStation_Num,9999);

		if (g_tStation_Info[i].voltage_class == 220)
		{
			memset(sql_buf,0,sizeof(sql_buf));
			nOnlineCameraNum = 0;

			sprintf_s(sql_buf,sizeof(sql_buf),"SELECT COUNT(id) FROM ct_user_station_view_history_18_%04d%02d WHERE station_id=%d AND day=%d AND (status=1 OR status=2) ",sysTm.wYear,sysTm.wMonth,g_tStation_Info[i].station_id,sysTm.wDay);

			Lock();
			if (!mysql_query(m_pMySql, sql_buf))
			{
				res = mysql_store_result(m_pMySql);

				while ( row = mysql_fetch_row( res ) )
				{
					nOnlineCameraNum = atoi(row[0]);
				}

				mysql_free_result(res) ;

				nOnlineCameraSum += nOnlineCameraNum;
			}
			Unlock();
		}
	}

	return nOnlineCameraSum;
}

int CDlgPageViewInfo::Get110kVStationCameraInfoOnlineCameraNumFromHistory()
{
	int i = 0;
	int nOnlineCameraSum = 0;
	int nOnlineCameraNum = 0;

	char sql_buf[1204] = {0};

	MYSQL_RES * res = NULL;
	MYSQL_ROW row ;

	SYSTEMTIME sysTm;

	GetLocalTime(&sysTm);

	for (i = 0;i < g_nStation_Num;i++)
	{
		if (m_bOnlineRateReportFlag == false)
			return -1;

		g_pDlgDeviceOnlineInfo->PostMessage(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE,40+i*60/g_nStation_Num,9999);

		if (g_tStation_Info[i].voltage_class == 110)
		{
			memset(sql_buf,0,sizeof(sql_buf));
			nOnlineCameraNum = 0;

			sprintf_s(sql_buf,sizeof(sql_buf),"SELECT COUNT(id) FROM ct_user_station_view_history_18_%04d%02d WHERE station_id=%d AND day=%d AND (status=1 OR status=2) ",sysTm.wYear,sysTm.wMonth,g_tStation_Info[i].station_id,sysTm.wDay);

			Lock();
			if (!mysql_query(m_pMySql, sql_buf))
			{
				res = mysql_store_result(m_pMySql);

				while ( row = mysql_fetch_row( res ) )
				{
					nOnlineCameraNum = atoi(row[0]);
				}

				mysql_free_result(res) ;

				nOnlineCameraSum += nOnlineCameraNum;
			}
			Unlock();
		}
	}

	return nOnlineCameraSum;
}

int CDlgPageViewInfo::Get35kVStationCameraInfoOnlineCameraNumFromHistory()
{
	int i = 0;
	int nOnlineCameraSum = 0;
	int nOnlineCameraNum = 0;

	char sql_buf[1204] = {0};

	MYSQL_RES * res = NULL;
	MYSQL_ROW row ;

	SYSTEMTIME sysTm;

	GetLocalTime(&sysTm);

	for (i = 0;i < g_nStation_Num;i++)
	{
		if (m_bOnlineRateReportFlag == false)
			return -1;

		g_pDlgDeviceOnlineInfo->PostMessage(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE,40+i*60/g_nStation_Num,9999);

		if (g_tStation_Info[i].voltage_class == 35)
		{
			memset(sql_buf,0,sizeof(sql_buf));
			nOnlineCameraNum = 0;

			sprintf_s(sql_buf,sizeof(sql_buf),"SELECT COUNT(id) FROM ct_user_station_view_history_18_%04d%02d WHERE station_id=%d AND day=%d AND (status=1 OR status=2) ",sysTm.wYear,sysTm.wMonth,g_tStation_Info[i].station_id,sysTm.wDay);

			Lock();
			if (!mysql_query(m_pMySql, sql_buf))
			{
				res = mysql_store_result(m_pMySql);

				while ( row = mysql_fetch_row( res ) )
				{
					nOnlineCameraNum = atoi(row[0]);
				}

				mysql_free_result(res) ;

				nOnlineCameraSum += nOnlineCameraNum;
			}
			Unlock();
		}
	}

	return nOnlineCameraSum;
}

int CDlgPageViewInfo::Get500kVStationCameraInfoOnlineCameraNumFromViewHistory()
{
	int i = 0;
	int nCameraSum = 0;
	int nCameraNum = 0;
	int nOnlineCameraSum = 0;
	int nOfflineCameraNum = 0;
	int nStationStatus = 0;

	for (i = 0;i < g_nStation_Num;i++)
	{
		if (m_bOnlineRateReportFlag == false)
			return -1;

		g_pDlgDeviceOnlineInfo->PostMessage(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE,40+i*60/g_nStation_Num,9999);

		if (g_tStation_Info[i].voltage_class == 500)
		{
			nOfflineCameraNum = 0;
			nCameraNum = 0;

			if (StatisticsOneStationVideoDailyInfo(g_tStation_Info[i].station_id,nCameraNum,nOfflineCameraNum,nStationStatus) == TRUE)
			{
				nCameraSum += nCameraNum;
				nOnlineCameraSum += (nCameraNum-nOfflineCameraNum);
			}
		}
	}

	return nOnlineCameraSum;
}

int CDlgPageViewInfo::Get220kVStationCameraInfoOnlineCameraNumFromViewHistory()
{
	int i = 0;
	int nCameraSum = 0;
	int nCameraNum = 0;
	int nOnlineCameraSum = 0;
	int nOfflineCameraNum = 0;
	int nStationStatus = 0;

	for (i = 0;i < g_nStation_Num;i++)
	{
		if (m_bOnlineRateReportFlag == false)
			return -1;

		g_pDlgDeviceOnlineInfo->PostMessage(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE,40+i*60/g_nStation_Num,9999);

		if (g_tStation_Info[i].voltage_class == 220)
		{
			nOfflineCameraNum = 0;
			nCameraNum = 0;

			if (StatisticsOneStationVideoDailyInfo(g_tStation_Info[i].station_id,nCameraNum,nOfflineCameraNum,nStationStatus) == TRUE)
			{
				nCameraSum += nCameraNum;
				nOnlineCameraSum += (nCameraNum-nOfflineCameraNum);
			}
		}
	}

	return nOnlineCameraSum;
}

int CDlgPageViewInfo::Get110kVStationCameraInfoOnlineCameraNumFromViewHistory()
{
	int i = 0;
	int nCameraSum = 0;
	int nCameraNum = 0;
	int nOnlineCameraSum = 0;
	int nOfflineCameraNum = 0;
	int nStationStatus = 0;

	for (i = 0;i < g_nStation_Num;i++)
	{
		if (m_bOnlineRateReportFlag == false)
			return -1;

		g_pDlgDeviceOnlineInfo->PostMessage(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE,40+i*60/g_nStation_Num,9999);

		if (g_tStation_Info[i].voltage_class == 110)
		{
			nOfflineCameraNum = 0;
			nCameraNum = 0;

			if (StatisticsOneStationVideoDailyInfo(g_tStation_Info[i].station_id,nCameraNum,nOfflineCameraNum,nStationStatus) == TRUE)
			{
				nCameraSum += nCameraNum;
				nOnlineCameraSum += (nCameraNum-nOfflineCameraNum);
			}
		}
	}

	return nOnlineCameraSum;
}

int CDlgPageViewInfo::Get35kVStationCameraInfoOnlineCameraNumFromViewHistory()
{
	int i = 0;
	int nCameraSum = 0;
	int nCameraNum = 0;
	int nOnlineCameraSum = 0;
	int nOfflineCameraNum = 0;
	int nStationStatus = 0;

	for (i = 0;i < g_nStation_Num;i++)
	{
		if (m_bOnlineRateReportFlag == false)
			return -1;

		g_pDlgDeviceOnlineInfo->PostMessage(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE,40+i*60/g_nStation_Num,9999);

		if (g_tStation_Info[i].voltage_class == 35)
		{
			nOfflineCameraNum = 0;
			nCameraNum = 0;

			if (StatisticsOneStationVideoDailyInfo(g_tStation_Info[i].station_id,nCameraNum,nOfflineCameraNum,nStationStatus) == TRUE)
			{
				nCameraSum += nCameraNum;
				nOnlineCameraSum += (nCameraNum-nOfflineCameraNum);
			}
		}
	}

	return nOnlineCameraSum;
}

BOOL CDlgPageViewInfo::GetStationVideoReportCameraStreamlessStatus(int nStationId)
{
	MYSQL_RES * res = NULL ;
	MYSQL_ROW row;

	int nCameraCount = 0;
	char sql_buf[1024] = {0};

	try
	{
		memset(&g_VideoViewReportStatusStationStreamInfo,0,sizeof(g_VideoViewReportStatusStationStreamInfo));

		sprintf_s(sql_buf, "SELECT camera_id,camera_code,streamless "
			" FROM ob_d5000_camera_station_%d ORDER BY camera_id ",nStationId);

		Lock();
		if (!mysql_query(m_pMySql, sql_buf))
		{
			res = mysql_store_result(m_pMySql);
			while (row = mysql_fetch_row(res))
			{
				nCameraCount = g_VideoViewReportStatusStationStreamInfo.m_nCameraCount;
				strcpy_s(g_VideoViewReportStatusStationStreamInfo.m_CameraInfo[nCameraCount].m_szCameraNum,
					sizeof(g_VideoViewReportStatusStationStreamInfo.m_CameraInfo[nCameraCount].m_szCameraNum),row[1]);
				g_VideoViewReportStatusStationStreamInfo.m_CameraInfo[nCameraCount].m_nStreamless= atoi(row[2]);
				g_VideoViewReportStatusStationStreamInfo.m_nCameraCount++;
			}
			mysql_free_result(res) ;
		}
		Unlock();
	}
	catch(...)
	{

	}
	return FALSE;
}

BOOL CDlgPageViewInfo::GetStationVideoReportStatusCameraDailyInfo(int nUserId,int nStationId,int nYear,int nMonth,int nDay)
{
	MYSQL_RES * res = NULL ;
	MYSQL_ROW row;

	int nCameraCount = 0;
	char sql_buf[1024] = {0};

	try
	{
		memset(&g_VideoViewReportStatusStationDailyInfo,0,sizeof(g_VideoViewReportStatusStationDailyInfo));

		sprintf_s(sql_buf, "SELECT  t1.camera_id,t1.camera_name,t1.camera_code,t2.status "
			" FROM ob_d5000_camera_station_%d AS t1, ct_user_station_view_history_%d_%04d%02d AS t2 WHERE t2.station_id=%d AND t2.day=%d AND t1.camera_code=t2.camera_code ORDER BY t1.camera_id ",
			nStationId,nUserId,nYear,nMonth,nStationId,nDay);

		Lock();
		if (!mysql_query(m_pMySql, sql_buf))
		{
			res = mysql_store_result(m_pMySql);
			while (row = mysql_fetch_row(res))
			{
				nCameraCount = g_VideoViewReportStatusStationDailyInfo.m_nCameraCount;
				g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[nCameraCount].m_nCameraId = atoi(row[0]);
				strcpy_s(g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[nCameraCount].m_szCameraName,sizeof(g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[nCameraCount].m_szCameraName),row[1]);
				strcpy_s(g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[nCameraCount].m_szCameraNum,sizeof(g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[nCameraCount].m_szCameraNum),row[2]);
				g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[nCameraCount].m_nCameraStatus = atoi(row[3]);
				g_VideoViewReportStatusStationDailyInfo.m_nCameraCount++;
			}
			mysql_free_result(res) ;
		}
		Unlock();
	}
	catch(...)
	{

	}
	return FALSE;
}

BOOL CDlgPageViewInfo::GetStationVideoReportStatusCameraDailyInfo2(int nUserId,int nStationId,int nYear,int nMonth,int nDay)
{
	MYSQL_RES * res = NULL ;
	MYSQL_ROW row;

	int    i = 0;
	int    nCameraCount = 0;
	char sql_buf[1024] = {0};
	int    nCameraId = 0;
	char szCameraName[256] = {0};
	char szCameraCode[64] = {0};
	int    nOperateType = 0;
	int    nOperateType2 = 0;

	try
	{
		sprintf_s(sql_buf, "SELECT  t1.camera_id,t1.camera_name,t1.camera_code,t2.status "
			" FROM ob_d5000_camera_station_%d AS t1, ct_user_station_view_history_%d_%04d%02d AS t2 WHERE t2.station_id=%d AND t2.day=%d AND t1.camera_code=t2.camera_code ORDER BY t1.camera_id ",
			nStationId,nUserId,nYear,nMonth,nStationId,nDay);

		Lock();
		if (!mysql_query(m_pMySql, sql_buf))
		{
			res = mysql_store_result(m_pMySql);
			while (row = mysql_fetch_row(res))
			{
				nCameraId = atoi(row[0]);
				strcpy_s(szCameraName,sizeof(szCameraName),row[1]);
				strcpy_s(szCameraCode,sizeof(szCameraCode),row[2]);
				nOperateType = atoi(row[3]);

				for (i = 0;i < g_VideoViewReportStatusStationDailyInfo.m_nCameraCount;i++)
				{
					if (strcmp(g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[i].m_szCameraNum,szCameraCode) == 0)
						break;
				}

				if (i < g_VideoViewReportStatusStationDailyInfo.m_nCameraCount)
				{
					nOperateType2 = g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[i].m_nCameraStatus;
					nOperateType = JoinStationReportCameraStatus(nOperateType,nOperateType2);
					g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[i].m_nCameraStatus = nOperateType;
				}
				else
				{
					nCameraCount = g_VideoViewReportStatusStationDailyInfo.m_nCameraCount;
					g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[nCameraCount].m_nCameraId = nCameraId;
					strcpy_s(g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[nCameraCount].m_szCameraName,sizeof(g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[nCameraCount].m_szCameraName),szCameraName);
					strcpy_s(g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[nCameraCount].m_szCameraNum,sizeof(g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[nCameraCount].m_szCameraNum),szCameraCode);
					g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[nCameraCount].m_nCameraStatus = nOperateType;
					g_VideoViewReportStatusStationDailyInfo.m_nCameraCount++;
				}
			}
			mysql_free_result(res) ;
		}
		Unlock();
	}
	catch(...)
	{

	}
	return FALSE;
}

int CDlgPageViewInfo::JoinStationReportCameraStatus(int nTagStatus,int nLineStatus)
{
	int nNewStatus = 0;

	if (nTagStatus == 0||nTagStatus == 1)
	{
		if (nLineStatus == 0||nLineStatus == 3)
		{
			nNewStatus = 0;
		}
		else if(nLineStatus == 1||nLineStatus == 2)
		{
			nNewStatus = 1;
		}
		else
		{
			nNewStatus = nTagStatus;
		}
	}
	else if (nTagStatus == 2||nTagStatus == 3)
	{
		if (nLineStatus == 0||nLineStatus == 3)
		{
			nNewStatus = 3;
		}
		else if (nLineStatus == 1||nLineStatus == 2)
		{
			nNewStatus = 2;
		}
		else
		{
			nNewStatus = nTagStatus;
		}
	}
	else
	{
		nNewStatus = nLineStatus;
	}

	return nNewStatus;
}

BOOL CDlgPageViewInfo::StationReportCameraIsOfflineStatus(int nCameraStatus,int nStreamless)
{
	BOOL bOfflineFlag = FALSE;

	if (nStreamless == 1||nCameraStatus == 0||nCameraStatus == 3||nCameraStatus == 10)
	{
		bOfflineFlag = TRUE;
	}

	return bOfflineFlag;
}

BOOL CDlgPageViewInfo::StatisticsOneStationVideoDailyInfo(int nStationId,int &nCameraCount,int &nOfflineCameraCount,int &nStationStatus)
{
	try
	{
		int i = 0;
		int j = 0;
		char Buffer[1024] = {0};

		int   nCameraId = 0;
		char szCameraName[256] = {0};
		char szCameraNum[64] = {0};
		char szOperateType[64] = {0};
		int    nOperateType = 0;
		int    nOperateType2 = 0;
		int    nStreamless = 0;

		nCameraCount = 0;
		nOfflineCameraCount = 0;
		nStationStatus = 0;

		memset(Buffer,0,sizeof(Buffer));
		memset(szCameraName,0,sizeof(szCameraName));
		memset(szCameraNum,0,sizeof(szCameraNum));

		SYSTEMTIME sysTm;
		GetLocalTime(&sysTm);

		int nYear = sysTm.wYear;
		int nMonth = sysTm.wMonth;
		int nDay = sysTm.wDay;

		//////////////////////////////////////////////////////////////////////////
		GetStationVideoReportCameraStreamlessStatus(nStationId);

		GetStationVideoReportStatusCameraDailyInfo(18,nStationId,nYear,nMonth,nDay);
		GetStationVideoReportStatusCameraDailyInfo2(g_userpower.userID,nStationId,nYear,nMonth,nDay);

		//////////////////////////////////////////////////////////////////////////
		nCameraCount = g_VideoViewReportStatusStationDailyInfo.m_nCameraCount;

		for (i = 0;i < g_VideoViewReportStatusStationDailyInfo.m_nCameraCount;i++)
		{
			nCameraId = g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[i].m_nCameraId;
			strcpy_s(szCameraName,sizeof(szCameraName),g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[i].m_szCameraName);
			strcpy_s(szCameraNum,sizeof(szCameraNum),g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[i].m_szCameraNum);
			nOperateType = g_VideoViewReportStatusStationDailyInfo.m_CameraInfo[i].m_nCameraStatus;

			nStreamless = 0;

			for (j = 0;j < g_VideoViewReportStatusStationStreamInfo.m_nCameraCount;j++)
			{
				if (strcmp(g_VideoViewReportStatusStationStreamInfo.m_CameraInfo[j].m_szCameraNum,szCameraNum) == 0)
				{
					nStreamless = g_VideoViewReportStatusStationStreamInfo.m_CameraInfo[j].m_nStreamless;
					break;
				}
			}

			if (nStreamless == 1||(nOperateType != 1&&nOperateType != 2))
			{
				nOfflineCameraCount++;
			}
		}

		if (nCameraCount > nOfflineCameraCount)
		{
			nStationStatus = 1;
			sprintf_s(Buffer, "UPDATE ob_d5000_station SET station_video_status=1 WHERE station_id=%d ",nStationId);

			Lock();
			mysql_query(m_pMySql, Buffer);
			Unlock();
		}

		if (nCameraCount == 0||nCameraCount == nOfflineCameraCount)
		{
			sprintf_s(Buffer, "UPDATE ob_d5000_station SET station_video_status=0 WHERE station_id=%d ",nStationId);

			Lock();
			mysql_query(m_pMySql, Buffer);
			Unlock();
		}

		return TRUE;
	}
	catch(...)
	{

	}

	return FALSE;
}


BOOL CDlgPageViewInfo::CheckPageViewTickCount()
{
	DWORD dwTickCount = GetTickCount();

	if (dwTickCount < m_dwPageViewTickCount)
	{
		m_dwPageViewTickCount = dwTickCount;
		return TRUE;
	}

	int nCountNum = 5*1000;

	switch(g_nPageViewOnePageMaxCameraNum)
	{
	case 1:
		{
			nCountNum = 2*1000;
		}
		break;

	case 4:
		{
			nCountNum = 3*1000;
		}
		break;

	case 9:
		{
			nCountNum = 5*1000;
		}
		break;

	default:
		break;
	}

	if (dwTickCount - m_dwPageViewTickCount > nCountNum)
	{
		m_dwPageViewTickCount = dwTickCount;
		return TRUE;
	}

	return FALSE;
}

void CDlgPageViewInfo::OnBnClickedBtnOnlineRateInfo()
{
	try
	{
		if (g_pDlgDeviceOnlineInfo == NULL)
		{
			g_pDlgDeviceOnlineInfo = new CDlgDeviceOnlineInfo();
			if (g_pDlgDeviceOnlineInfo == NULL)
				return;

			g_pDlgDeviceOnlineInfo->Create(IDD_DIALOG_DEVICE_ONLINE_INFO,this);
		}

		g_pDlgDeviceOnlineInfo->ShowWindow(SW_SHOW);

		g_pDlgDeviceOnlineInfo->m_comboType.SetCurSel(0);
		StartDeviceOnlineRateReportInfo("全部站点");
	}
	catch(...)
	{

	}
}

//得到并显示在线率统计信息
BOOL CDlgPageViewInfo::GetAndShowOnlineRateReportInfo(char *szTypeName)
{
	if (g_pDlgDeviceOnlineInfo == NULL)
		return FALSE;

	g_pDlgDeviceOnlineInfo->ShowWindow(SW_SHOW);

	int nDBStationNum = 0;
	int nDBOffStationNum = 0;

	int nOffStationNum = 0;
	int nAllStationNum = 0;
	int nProblemCameraNum = 0;
	int nAllCameraNum = 0;
	int nStationStatus = 0;

	int nOffStationNum2 = 0;
	int nAllStationNum2 = 0;
	int nProblemCameraNum2 = 0;
	int nAllCameraNum2 = 0;
	int nStationStatus2 = 0;

	int nOffStationNum_500KV = 0;
	int nAllStationNum_500KV = 0;
	int nProblemCameraNum_500KV = 0;
	int nAllCameraNum_500KV = 0;

	int nOffStationNum_220KV = 0;
	int nAllStationNum_220KV = 0;
	int nProblemCameraNum_220KV = 0;
	int nAllCameraNum_220KV = 0;

	int nOffStationNum_110KV = 0;
	int nAllStationNum_110KV = 0;
	int nProblemCameraNum_110KV = 0;
	int nAllCameraNum_110KV = 0;

	int nOffStationNum_35KV = 0;
	int nAllStationNum_35KV = 0;
	int nProblemCameraNum_35KV = 0;
	int nAllCameraNum_35KV = 0;

	if(GetStationInfo(nDBStationNum,nDBOffStationNum,VIEW_INFO_STATION_TYPE_ALL) == FALSE)
		return FALSE;

	int nNodeId[256];
	memset(nNodeId,0,sizeof(nNodeId));
	int nCount = 0; 

	int nLineNodeId = GetLineNodeId();//输电线路

	nCount = GetStationNodeInfo(szTypeName,nNodeId,256,nLineNodeId);

	char buf[256] = {0};
	memset(buf,0,sizeof(buf));

	int nNum1 = 0;
	int nNum2 = 0;

	int i = 0;
	int j = 0;

	try
	{
		for (i = 0;i < nDBStationNum;i++)
		{
			if (m_bOnlineRateReportFlag == false)
				return FALSE;

			g_pDlgDeviceOnlineInfo->PostMessage(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE,i*20/nDBStationNum,9999);

			for (j = 0;j < nCount;j++)
			{
				if(g_tStation_Info[i].node_id == nNodeId[j])
					break;
			}

			if (j >= nCount)
				continue;

			if (g_tStation_Info[i].voltage_class == 500)
			{
				if(GetStationCameraInfo(g_tStation_Info[i].station_id,nNum1,nNum2,nStationStatus) != FALSE)
				{
					nAllCameraNum += nNum1;
					nProblemCameraNum += nNum2;
					nAllStationNum++;
					if (nStationStatus == 0)
						nOffStationNum++;

					nAllCameraNum_500KV += nNum1;
					nProblemCameraNum_500KV += nNum2;
					nAllStationNum_500KV++;
					if (nStationStatus == 0)
						nOffStationNum_500KV++;

					//g_VMLog.WriteVmLog("--500kV--站点%s,总摄像机数=%d,有问题摄像机=%d",g_tStation_Info[i].station_name_videoplant,nNum1,nNum2);
				}
			}
			else if (g_tStation_Info[i].voltage_class == 220)
			{
				if(GetStationCameraInfo(g_tStation_Info[i].station_id,nNum1,nNum2,nStationStatus) != FALSE)
				{
					nAllCameraNum += nNum1;
					nProblemCameraNum += nNum2;
					nAllStationNum++;
					if (nStationStatus == 0)
						nOffStationNum++;

					nAllCameraNum_220KV += nNum1;
					nProblemCameraNum_220KV += nNum2;
					nAllStationNum_220KV++;
					if (nStationStatus == 0)
						nOffStationNum_220KV++;

					//g_VMLog.WriteVmLog("--220kV--站点%s,总摄像机数=%d,有问题摄像机=%d",g_tStation_Info[i].station_name_videoplant,nNum1,nNum2);
				}
			}
			else if (g_tStation_Info[i].voltage_class == 110)
			{
				if(GetStationCameraInfo(g_tStation_Info[i].station_id,nNum1,nNum2,nStationStatus) != FALSE)
				{
					nAllCameraNum += nNum1;
					nProblemCameraNum += nNum2;
					nAllStationNum++;
					if (nStationStatus == 0)
						nOffStationNum++;

					nAllCameraNum_110KV += nNum1;
					nProblemCameraNum_110KV += nNum2;
					nAllStationNum_110KV++;
					if (nStationStatus == 0)
						nOffStationNum_110KV++;

					//g_VMLog.WriteVmLog("--110kV--站点%s,总摄像机数=%d,有问题摄像机=%d",g_tStation_Info[i].station_name_videoplant,nNum1,nNum2);
				}
			}
			else if (g_tStation_Info[i].voltage_class == 35)
			{
				if(GetStationCameraInfo(g_tStation_Info[i].station_id,nNum1,nNum2,nStationStatus) != FALSE)
				{
					nAllCameraNum += nNum1;
					nProblemCameraNum += nNum2;
					nAllStationNum++;
					if (nStationStatus == 0)
						nOffStationNum++;

					nAllCameraNum_35KV += nNum1;
					nProblemCameraNum_35KV += nNum2;
					nAllStationNum_35KV++;
					if (nStationStatus == 0)
						nOffStationNum_35KV++;

					//g_VMLog.WriteVmLog("--35kV--站点%s,总摄像机数=%d,有问题摄像机=%d",g_tStation_Info[i].station_name_videoplant,nNum1,nNum2);
				}
			}
		}

		nCount = GetLineNodeInfo(szTypeName,nNodeId,256,nLineNodeId);

		for (i = 0;i < nDBStationNum;i++)
		{
			if (m_bOnlineRateReportFlag == false)
				return FALSE;

			g_pDlgDeviceOnlineInfo->PostMessage(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE,20+i*20/nDBStationNum,9999);

			for (j = 0;j < nCount;j++)
			{
				if(g_tStation_Info[i].node_id == nNodeId[j])
					break;
			}

			if (j >= nCount)
				continue;

			if(GetStationCameraInfo(g_tStation_Info[i].station_id,nNum1,nNum2,nStationStatus) != FALSE)
			{
				nAllCameraNum2 += nNum1;
				nProblemCameraNum2 += nNum2;
				nAllStationNum2++;
				if (nStationStatus == 0)
					nOffStationNum2++;
			}
		}

		////////////////////////////////////////////////////////////////////////////
		////临时
		//int n220kVOnlineCameraNum =  Get220kVStationCameraInfoOnlineCameraNumFromHistory();
		//if (n220kVOnlineCameraNum < 0)
		//	return FALSE;

		//nProblemCameraNum_220KV = nAllCameraNum_220KV - n220kVOnlineCameraNum;
		//if (nProblemCameraNum_220KV < 0)
		//	nProblemCameraNum_220KV = 0;
		////////////////////////////////////////////////////////////////////////////

		g_pDlgDeviceOnlineInfo->PostMessage(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE,100,9999);

		g_pDlgDeviceOnlineInfo->SetAndShowOnlineInfo(nAllCameraNum,nProblemCameraNum,nAllStationNum,nOffStationNum);
		g_pDlgDeviceOnlineInfo->SetAndShowOnlineInfo500KV(nAllCameraNum_500KV,nProblemCameraNum_500KV,nAllStationNum_500KV,nOffStationNum_500KV);
		g_pDlgDeviceOnlineInfo->SetAndShowOnlineInfo220KV(nAllCameraNum_220KV,nProblemCameraNum_220KV,nAllStationNum_220KV,nOffStationNum_220KV);
		g_pDlgDeviceOnlineInfo->SetAndShowOnlineInfo110KV(nAllCameraNum_110KV,nProblemCameraNum_110KV,nAllStationNum_110KV,nOffStationNum_110KV);
		g_pDlgDeviceOnlineInfo->SetAndShowOnlineInfo2(nAllCameraNum2,nProblemCameraNum2,nAllStationNum2,nOffStationNum2);

		g_pDlgDeviceOnlineInfo->SendMessage(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE,101,9999);

		return TRUE;
	}
	catch (...)
	{

	}

	return FALSE;
}

BOOL CDlgPageViewInfo::StartDeviceOnlineRateReportInfo(char *szTypeName)
{
	try
	{
		if (m_bOnlineRateReportFlag != false)
			return TRUE;

		m_bOnlineRateReportFlag = true;

		if (m_hDeviceOnlineRateInfoThread != NULL)
		{
			CloseHandle(m_hDeviceOnlineRateInfoThread);
			m_hDeviceOnlineRateInfoThread = NULL;
			m_uDeviceOnlineRateInfoThreadID = 0;
		}

		strcpy_s(m_szOnlineRateReportType,sizeof(m_szOnlineRateReportType)-1,szTypeName);

		m_hDeviceOnlineRateInfoThread = (HANDLE)_beginthreadex(NULL, 0,_GetAndShowOnlineRateReportInfoThread,(void*)this, 0, &m_uDeviceOnlineRateInfoThreadID);
		if (m_hDeviceOnlineRateInfoThread == NULL)
		{
			m_bOnlineRateReportFlag = false;
			return FALSE;
		}

		return TRUE;
	}
	catch(...)
	{

	}
	return FALSE;
}

BOOL CDlgPageViewInfo::StopDeviceOnlineRateReportInfo()
{
	try
	{
		DWORD dwResult = 0;

		m_bOnlineRateReportFlag = false;

		if (m_hDeviceOnlineRateInfoThread != NULL)
		{
			dwResult = WaitForSingleObject(m_hDeviceOnlineRateInfoThread,6*1000);
			CloseHandle(m_hDeviceOnlineRateInfoThread);
			m_hDeviceOnlineRateInfoThread = NULL;
			m_uDeviceOnlineRateInfoThreadID = 0;
		}

		if (g_pDlgDeviceOnlineInfo != NULL)
		{
			g_pDlgDeviceOnlineInfo->SendMessage(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE,0,9999);
		}

		return TRUE;
	}
	catch(...)
	{

	}
	return FALSE;
}

//得到并显示在线率统计信息
unsigned __stdcall _GetAndShowOnlineRateReportInfoThread(void *arg)
{
	if (arg == NULL)
		return -1;

	CDlgPageViewInfo * pDlgPageViewInfo = NULL;
	pDlgPageViewInfo = (CDlgPageViewInfo *)arg;

	try
	{
		pDlgPageViewInfo->GetAndShowOnlineRateReportInfo(pDlgPageViewInfo->m_szOnlineRateReportType);
		pDlgPageViewInfo->m_bOnlineRateReportFlag = false;

		return 0;
	}
	catch(...)
	{

	}
	pDlgPageViewInfo->m_bOnlineRateReportFlag = false;
	return 1;
}
