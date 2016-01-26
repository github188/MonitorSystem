// DlgLinkageServer.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgLinkageServer.h"
#include "VEMCUCtlDlg.h"
#include "SipCall.h"


// CDlgLinkageServer 对话框

IMPLEMENT_DYNAMIC(CDlgLinkageServer, CDialog)

CDlgLinkageServer::CDlgLinkageServer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLinkageServer::IDD, pParent)
{
	m_bRelationListExtendFlag = FALSE;
	m_Brush.CreateSolidBrush(RGB(245,250,253)); 
}

CDlgLinkageServer::~CDlgLinkageServer()
{

}

void CDlgLinkageServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RELATION, m_listRelation);
	DDX_Control(pDX, IDC_LIST_CAMERA_RELATION, m_listCameraRelation);
	DDX_Control(pDX, IDC_BTN_HIDE_LINKAGE_SERVER, m_btnHideLinkageServer);
	DDX_Control(pDX, IDC_BTN_EXTEND, m_btnExtend);
	DDX_Control(pDX, IDC_CHECK_RELATE_VIDEO, m_CheckRelateVideo);
	DDX_Control(pDX, IDC_CHECK_RELATE_ACCIDENT, m_CheckRelateAccident);
	DDX_Control(pDX, IDC_CHECK_RELATE_ABNORMAL, m_CheckRelateAbnormal);
	DDX_Control(pDX, IDC_CHECK_RELATE_STATE, m_CheckRelateState);
	DDX_Control(pDX, IDC_CHECK_RELATE_SWITCHSTATE, m_CheckRelateSwitchState);
	DDX_Control(pDX, IDC_CHECK_RELATE_LIMIT, m_CheckRelateLimit);
	DDX_Control(pDX, IDC_CHECK_RELATE_NOTIFY, m_CheckRelateNotify);
	DDX_Control(pDX, IDC_CHECK_RELATE_METRO_ALARM, m_CheckRelateMetroAlarm);
	DDX_Control(pDX, IDC_CHECK_RELATE_METRO_PRE, m_CheckRelateMetroPre);
	DDX_Control(pDX, IDC_CHECK_RELATE_TYPHOON_PRE, m_CheckRelateTyphoonPre);
	DDX_Control(pDX, IDC_CHECK_RELATE_TYPHOON_ALARM, m_CheckRelateTyphoonAlarm);
	DDX_Control(pDX, IDC_CHECK_RELATE_THUNDER_PRE, m_CheckRelateThunderPre);
	DDX_Control(pDX, IDC_CHECK_RELATE_THUNDER_ALARM, m_CheckRelateThunderAlarm);
}


BEGIN_MESSAGE_MAP(CDlgLinkageServer, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RELATION, &CDlgLinkageServer::OnNMClickListRelation)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RELATION, &CDlgLinkageServer::OnNMDblclkListRelation)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CAMERA_RELATION, &CDlgLinkageServer::OnNMDblclkListCameraRelation)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_EXTEND, &CDlgLinkageServer::OnBnClickedBtnExtend)
	ON_BN_CLICKED(IDC_CHECK_RELATE_VIDEO, &CDlgLinkageServer::OnBnClickedCheckRelateVideo)
	ON_BN_CLICKED(IDC_CHECK_RELATE_ACCIDENT, &CDlgLinkageServer::OnBnClickedCheckRelateAccident)
	ON_BN_CLICKED(IDC_CHECK_RELATE_ABNORMAL, &CDlgLinkageServer::OnBnClickedCheckRelateAbnormal)
	ON_BN_CLICKED(IDC_CHECK_RELATE_STATE, &CDlgLinkageServer::OnBnClickedCheckRelateState)
	ON_BN_CLICKED(IDC_CHECK_RELATE_SWITCHSTATE, &CDlgLinkageServer::OnBnClickedCheckRelateSwitchstate)
	ON_BN_CLICKED(IDC_CHECK_RELATE_LIMIT, &CDlgLinkageServer::OnBnClickedCheckRelateLimit)
	ON_BN_CLICKED(IDC_CHECK_RELATE_NOTIFY, &CDlgLinkageServer::OnBnClickedCheckRelateNotify)
	ON_BN_CLICKED(IDC_CHECK_RELATE_METRO_ALARM, &CDlgLinkageServer::OnBnClickedCheckRelateMetroAlarm)
	ON_BN_CLICKED(IDC_CHECK_RELATE_METRO_PRE, &CDlgLinkageServer::OnBnClickedCheckRelateMetroPre)
	ON_BN_CLICKED(IDC_CHECK_RELATE_TYPHOON_PRE, &CDlgLinkageServer::OnBnClickedCheckRelateTyphoonPre)
	ON_BN_CLICKED(IDC_CHECK_RELATE_TYPHOON_ALARM, &CDlgLinkageServer::OnBnClickedCheckRelateTyphoonAlarm)
	ON_BN_CLICKED(IDC_CHECK_RELATE_THUNDER_PRE, &CDlgLinkageServer::OnBnClickedCheckRelateThunderPre)
	ON_BN_CLICKED(IDC_CHECK_RELATE_THUNDER_ALARM, &CDlgLinkageServer::OnBnClickedCheckRelateThunderAlarm)
	ON_COMMAND_RANGE(1000, 1152, OnControlButton)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgLinkageServer 消息处理程序

BOOL CDlgLinkageServer::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	LoadSkin();//加载皮肤

	InitControl();//初始化控件

	ReadRelateActionIni();//读取是否执行联动视频的动作

	//////////////////////////////////////////////////////////////////////////

	return TRUE;  
}

BOOL CDlgLinkageServer::LoadSkin()
{
	m_LinkageServerBmp.LoadBitmap(IDB_BITMAP_PAGESERVER);
	m_btnHideLinkageServer.LoadBitmap(IDB_BITMAP_HIDE_LINKAGE_SERVER,0);
	m_btnExtend.LoadBitmap(IDB_BITMAP_NORMAL_LINKAGE,1);
	m_btnExtend.LoadBitmap(IDB_BITMAP_EXTEND_LINKAGE,0);

	m_listRelation.SetHeaderHeight(1.2f);          //设置头部高度
	m_listRelation.SetHeaderFontHW(12,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_listRelation.SetHeaderTextColor(RGB(58,77,93)); //设置头部字体颜色
	m_listRelation.SetHeaderBKColor(195,248,250,3); //设置头部背景色

	m_listRelation.SetTextColor(RGB(36, 162, 146));
	m_listRelation.SetBkColor(RGB(239, 246, 253));
	m_listRelation.SetTextBkColor(RGB(239, 246, 253));
	m_listRelation.SetRowHeigt(15);               //设置行高度
	m_listRelation.SetFontHW(12,0);               //设置字体高度，和宽度,0表示缺省宽度

	m_listCameraRelation.SetHeaderHeight(1.2f);          //设置头部高度
	m_listCameraRelation.SetHeaderFontHW(12,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_listCameraRelation.SetHeaderTextColor(RGB(58,77,93)); //设置头部字体颜色
	m_listCameraRelation.SetHeaderBKColor(195,248,250,3); //设置头部背景色

	m_listCameraRelation.SetBkColor(RGB(239, 246, 253));
	m_listCameraRelation.SetTextBkColor(RGB(239, 246, 253));
	m_listCameraRelation.SetTextColor(RGB(36, 162, 146));
	m_listCameraRelation.SetRowHeigt(15);               //设置行高度
	m_listCameraRelation.SetFontHW(12,0);               //设置字体高度，和宽度,0表示缺省宽度

	//////////////////////////////////////////////////////////////////////////
	m_LeftBmp.LoadBitmap(IDB_BITMAP_PAGESERVER_RIGHT_BK);
	m_LeftBmp.GetObject(sizeof(BITMAP),(LPVOID)&m_LeftBm);

	return TRUE;
}

BOOL CDlgLinkageServer::InitControl()
{
	try
	{
		DWORD dwStyle = 0;
		dwStyle = m_listRelation.GetStyle();
		dwStyle = LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT ;
		m_listRelation.SetExtendedStyle(dwStyle);

		m_listRelation.InsertColumn(0,"联动类型");
		m_listRelation.SetColumnWidth(0,80);
		m_listRelation.InsertColumn(1,"设备名称");
		m_listRelation.SetColumnWidth(1,120);
		m_listRelation.InsertColumn(2,"站点名称");
		m_listRelation.SetColumnWidth(2,120);
		m_listRelation.InsertColumn(3,"联动时间");
		m_listRelation.SetColumnWidth(3,100);
		m_listRelation.InsertColumn(4,"分类型");
		m_listRelation.SetColumnWidth(4,80);
		m_listRelation.InsertColumn(5,"状态");
		m_listRelation.SetColumnWidth(5,0);
		m_listRelation.InsertColumn(6,"屏号");
		m_listRelation.SetColumnWidth(6,0);
		m_listRelation.InsertColumn(7,"备注");
		m_listRelation.SetColumnWidth(7,0);
		m_listRelation.InsertColumn(8,"Value1");
		m_listRelation.SetColumnWidth(8,0);
		m_listRelation.InsertColumn(9,"Value2");
		m_listRelation.SetColumnWidth(9,0);
		m_listRelation.InsertColumn(10,"联动站点");
		m_listRelation.SetColumnWidth(10,100);
		m_listRelation.InsertColumn(11,"联动数量");
		m_listRelation.SetColumnWidth(11,80);

		m_bRelationListExtendFlag = FALSE;

		dwStyle = m_listCameraRelation.GetStyle();
		dwStyle = LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT;
		m_listCameraRelation.SetExtendedStyle(dwStyle);

		m_listCameraRelation.InsertColumn(0,"摄像头名称");
		m_listCameraRelation.SetColumnWidth(0,200);
		m_listCameraRelation.InsertColumn(1,"摄像头号码");
		m_listCameraRelation.SetColumnWidth(1,200);
		m_listCameraRelation.InsertColumn(2,"解码标签");
		m_listCameraRelation.SetColumnWidth(2,150);
		m_listCameraRelation.InsertColumn(3,"站点名称");
		m_listCameraRelation.SetColumnWidth(3,200);
		m_listCameraRelation.InsertColumn(4,"PresetId");
		m_listCameraRelation.SetColumnWidth(4,60);
		m_listCameraRelation.InsertColumn(5,"FromX1");
		m_listCameraRelation.SetColumnWidth(5,60);
		m_listCameraRelation.InsertColumn(6,"FromY1");
		m_listCameraRelation.SetColumnWidth(6,60);
		m_listCameraRelation.InsertColumn(7,"ToX1");
		m_listCameraRelation.SetColumnWidth(7,60);
		m_listCameraRelation.InsertColumn(8,"ToY1");
		m_listCameraRelation.SetColumnWidth(8,60);
		m_listCameraRelation.InsertColumn(9,"FromX2");
		m_listCameraRelation.SetColumnWidth(9,60);
		m_listCameraRelation.InsertColumn(10,"FromY2");
		m_listCameraRelation.SetColumnWidth(10,60);
		m_listCameraRelation.InsertColumn(11,"ToX2");
		m_listCameraRelation.SetColumnWidth(11,60);
		m_listCameraRelation.InsertColumn(12,"ToY2");
		m_listCameraRelation.SetColumnWidth(12,60);
		m_listCameraRelation.InsertColumn(13,"预置位名称");
		m_listCameraRelation.SetColumnWidth(13,0);

		time_t now_time = time(NULL);

		time_t history_time = now_time + 8*60*60 - 24*60*60;

		tm* history_tm =  gmtime(&history_time);

		char szLinkageTime[64] = {0};

		sprintf_s(szLinkageTime,"%04d-%02d-%02d %02d:%02d:%02d",history_tm->tm_year+1900,history_tm->tm_mon+1,history_tm->tm_mday,history_tm->tm_hour,history_tm->tm_min,history_tm->tm_sec);

		ReadAndSetRelationListInfo2(szLinkageTime);

		return TRUE;
	}
	catch(...)
	{

	}

	return FALSE;
}

void CDlgLinkageServer::OnControlButton(UINT nID)
{
	if (nID == IDC_BTN_HIDE_LINKAGE_SERVER)
	{
		g_pMainDlg->PostMessage(OM_CONTROLBUTTON, nID,VEM_CONTROL_BUTTON_MESSAGE_TYPE_LINKAGESERVER);
	}
}

BOOL CDlgLinkageServer::HideLinkageServer()
{
	return TRUE;
}

void CDlgLinkageServer::OnNMClickListRelation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CameraRelationInsertCameraInfoByListRelation();
	
	*pResult = 0;
}

void CDlgLinkageServer::OnNMDblclkListRelation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CameraRelationInsertCameraInfoByListRelation();

	try
	{
		//////////////////////////////////////////////////////////////////////////
		// 打开所有摄像头视频

		char szName[256] = {0};
		char szCode[64] = {0};
		char szDecodeTag[16] = {0};
		char szStationName[256] = {0};
		char szReason[256] = {0};

		char szPresetId[64] = {0};
		char szFromX1[64] = {0};
		char szFromY1[64] = {0};
		char szToX1[64] = {0};
		char szToY1[64] = {0};
		char szFromX2[64] = {0};
		char szFromY2[64] = {0};
		char szToX2[64] = {0};
		char szToY2[64] = {0};
		char szPresetName[256] = {0};

		int nLength = 0;

		int nScreenId = 0;

		int nCount = m_listCameraRelation.GetItemCount();

		if (nCount > MAX_PATH)
			nCount = MAXVIEWCH;

		if (g_pMainDlg != NULL)
			g_pMainDlg->SetLinkageForms(nCount);

		POSITION ps =m_listRelation.GetFirstSelectedItemPosition();
		char szLinkageType[64] = {0};
		int nIndex = m_listRelation.GetNextSelectedItem(ps);
		nLength = m_listRelation.GetItemText(nIndex,0,szLinkageType,sizeof(szLinkageType));

		if (strcmp(szLinkageType,"告警联动") == 0)
		{
			g_nViewOutVideoTypeFlag = VIDEO_ALARM_LINKAGE_INFO_TYPE;
		}
		else if (strcmp(szLinkageType,"热点设备联动") == 0)
		{
			g_nViewOutVideoTypeFlag = VIDEO_LINKAGE_INFO_TYPE;
		}
		else if (strcmp(szLinkageType,"气象报警") == 0)
		{
			g_nViewOutVideoTypeFlag = VIDEO_WEATHER_WARNING_INFO_TYPE;
		}
		else if (strcmp(szLinkageType,"气象预警") == 0)
		{
			g_nViewOutVideoTypeFlag = VIDEO_WEATHER_FORECAST_INFO_TYPE;
		}

		_T_NODE_PRESET_INFO *pPresetInfo = NULL;
		_T_NODE_PRESET_INFO * pPresetNodeInfo = NULL;
		_T_NODE_INFO * pNodeInfo = NULL;
		HTREEITEM hInfoTreeItem = NULL;

		pPresetInfo = new _T_NODE_PRESET_INFO;
		if (pPresetInfo != NULL)
		{
			memset(pPresetInfo,0,sizeof(_T_NODE_PRESET_INFO));
		}
		else
		{
			return;
		}

		for (int nIndex = 0;nIndex < nCount;nIndex++)
		{
			nLength = m_listCameraRelation.GetItemText(nIndex,0,szName,sizeof(szName));
			nLength = m_listCameraRelation.GetItemText(nIndex,1,szCode,sizeof(szCode));
			nLength = m_listCameraRelation.GetItemText(nIndex,2,szDecodeTag,sizeof(szDecodeTag));
			nLength = m_listCameraRelation.GetItemText(nIndex,3,szStationName,sizeof(szStationName));
			nLength = m_listCameraRelation.GetItemText(nIndex,4,szPresetId,sizeof(szPresetId));
			nLength = m_listCameraRelation.GetItemText(nIndex,5,szFromX1,sizeof(szFromX1));
			nLength = m_listCameraRelation.GetItemText(nIndex,6,szFromY1,sizeof(szFromY1));
			nLength = m_listCameraRelation.GetItemText(nIndex,7,szToX1,sizeof(szToX1));
			nLength = m_listCameraRelation.GetItemText(nIndex,8,szToY1,sizeof(szToY1));
			nLength = m_listCameraRelation.GetItemText(nIndex,9,szFromX2,sizeof(szFromX2));
			nLength = m_listCameraRelation.GetItemText(nIndex,10,szFromY2,sizeof(szFromY2));
			nLength = m_listCameraRelation.GetItemText(nIndex,11,szToX2,sizeof(szToX2));
			nLength = m_listCameraRelation.GetItemText(nIndex,12,szToY2,sizeof(szToY2));
			nLength = m_listCameraRelation.GetItemText(nIndex,13,szPresetName,sizeof(szPresetName));

			if (g_pMainDlg != NULL&&g_pMainDlg->m_pDlgPageServer != NULL)
			{
				g_pMainDlg->m_pDlgPageServer->CameraTreelistAddCameraByStastion(szStationName);
				g_pMainDlg->m_pDlgPageServer->PresetTreelistAddPresetByStastion(szStationName);
			}

			if (pPresetInfo != NULL)
			{
				memset(pPresetInfo,0,sizeof(_T_NODE_PRESET_INFO));

				strcpy_s(pPresetInfo->node_station,sizeof(pPresetInfo->node_station),szStationName);
				strcpy_s(pPresetInfo->node_num,sizeof(pPresetInfo->node_num),szCode);

				pPresetInfo->preset_id = atoi(szPresetId);

				pPresetInfo->line1_from_x = atoi(szFromX1);
				pPresetInfo->line1_from_y = atoi(szFromY1);
				pPresetInfo->line1_to_x = atoi(szToX1);
				pPresetInfo->line1_to_y = atoi(szToY1);

				pPresetInfo->line2_from_x = atoi(szFromX2);
				pPresetInfo->line2_from_y = atoi(szFromY2);
				pPresetInfo->line2_to_x = atoi(szToX2);
				pPresetInfo->line2_to_y = atoi(szToY2);

				if (g_pMainDlg != NULL&&g_pMainDlg->m_pDlgPageServer != NULL)
				{
					hInfoTreeItem = g_pMainDlg->m_pDlgPageServer->SearchPresetTreePresetHandleTreeItem(NULL,szCode,szPresetName);
					if (hInfoTreeItem != NULL)
					{
						pPresetNodeInfo = (_T_NODE_PRESET_INFO *)g_pMainDlg->m_pDlgPageServer->m_trPreset.GetItemData(hInfoTreeItem);
						if (pPresetNodeInfo != NULL)
						{
							memcpy(&pPresetInfo->camera_info,&pPresetNodeInfo->camera_info,sizeof(pPresetInfo->camera_info));
						}
					}
					else
					{
						hInfoTreeItem = g_pMainDlg->m_pDlgPageServer->SearchCameraTreeCameraHandleTreeItem(NULL,szCode,NULL,NULL);
						if (hInfoTreeItem != NULL)
						{
							pNodeInfo = (_T_NODE_INFO *)g_pMainDlg->m_pDlgPageServer->m_trServer.GetItemData(hInfoTreeItem);
							if (pNodeInfo != NULL)
							{
								memcpy(&pPresetInfo->camera_info,&pNodeInfo->camera_info,sizeof(pPresetInfo->camera_info));
							}
						}
					}
				}
			}
			else
			{
				continue;
			}

			sprintf(szReason, "{%s}-{%s}-{%s}",szLinkageType,szName, szStationName);
			if (g_pMainDlg != NULL)
			{
				VIDEO_SIP_CALL_PRESET_INFO VideoSipCallPresetInfo;
				memset(&VideoSipCallPresetInfo,0,sizeof(VideoSipCallPresetInfo));

				if (IsCameraVideoLinkByVideoPlatform(g_nClientVideoLinkType,atoi(szDecodeTag)))
				{
					VideoSipCallPresetInfo.nType = 2;
					VideoSipCallPresetInfo.nDecodeTag = atoi(szDecodeTag);
				}
				else
				{
					VideoSipCallPresetInfo.nType = 4;
					VideoSipCallPresetInfo.nDecodeTag = pPresetInfo->camera_info.dvr_info.dvr_type;
				}

				strcpy_s(VideoSipCallPresetInfo.szCode,sizeof(VideoSipCallPresetInfo.szCode),szCode);
				strcpy_s(VideoSipCallPresetInfo.szName,sizeof(VideoSipCallPresetInfo.szName),szName);
				VideoSipCallPresetInfo.nScreenId = nScreenId+nIndex;
				strcpy_s(VideoSipCallPresetInfo.szReason,sizeof(VideoSipCallPresetInfo.szReason),szReason);
				memcpy(&VideoSipCallPresetInfo.preset_node_info,pPresetInfo,sizeof(VideoSipCallPresetInfo.preset_node_info));

				g_ThreadVideoOperateNumberInfo.DeviceVideoInNumerAdd();
				g_pMainDlg->ThreadPoolDispatchTask(ThreadVideoLinkageMakeCall,(void *)&VideoSipCallPresetInfo,sizeof(VideoSipCallPresetInfo),2);
			}

			Sleep(100);

			char szInfo[1024] = {0};
			if (pPresetInfo != NULL)
			{
				sprintf(szInfo, "<ctrlcamera user=\"admin\" station=\"%s\" name=\"%s\" code=\"%s\" CommandName=\"预置位调用\" command=\"1538\" CommandPara1=\"%d\" CommandPara2=\"0\" CommandPara3=\"0\" srcip=\"%s\"/>",
					szStationName,szName,pPresetInfo->node_num, pPresetInfo->preset_id,app_StackCfg.szLocalUdpAddress);

				SendVideoYTControlInfo(szInfo,strlen(szInfo));
			}
		}

		if (pPresetInfo != NULL)
		{
			delete pPresetInfo;
			pPresetInfo = NULL;
		}
	}
	catch(...)
	{

	}

	*pResult = 0;
}

void CDlgLinkageServer::OnNMDblclkListCameraRelation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	try
	{
		//////////////////////////////////////////////////////////////////////////
		// 在摄像头列表中显示关联摄像头
		POSITION ps =m_listCameraRelation.GetFirstSelectedItemPosition();
		int nIndex  = -1;

		char szName[256] = {0};
		char szCode[64] = {0};
		char szDecodeTag[16] = {0};
		char szStationName[256] = {0};
		char szReason[256] = {0};
		char szPresetId[64] = {0};
		char szFromX1[64] = {0};
		char szFromY1[64] = {0};
		char szToX1[64] = {0};
		char szToY1[64] = {0};
		char szFromX2[64] = {0};
		char szFromY2[64] = {0};
		char szToX2[64] = {0};
		char szToY2[64] = {0};
		char szPresetName[256] = {0};

		int nLength = 0;

		int nScreenId = 0;

		if (ps != NULL)
		{
			nIndex = m_listCameraRelation.GetNextSelectedItem(ps);
			nLength = m_listCameraRelation.GetItemText(nIndex,0,szName,sizeof(szName));
			nLength = m_listCameraRelation.GetItemText(nIndex,1,szCode,sizeof(szCode));
			nLength = m_listCameraRelation.GetItemText(nIndex,2,szDecodeTag,sizeof(szDecodeTag));
			nLength = m_listCameraRelation.GetItemText(nIndex,3,szStationName,sizeof(szStationName));
			nLength = m_listCameraRelation.GetItemText(nIndex,4,szPresetId,sizeof(szPresetId));
			nLength = m_listCameraRelation.GetItemText(nIndex,5,szFromX1,sizeof(szFromX1));
			nLength = m_listCameraRelation.GetItemText(nIndex,6,szFromY1,sizeof(szFromY1));
			nLength = m_listCameraRelation.GetItemText(nIndex,7,szToX1,sizeof(szToX1));
			nLength = m_listCameraRelation.GetItemText(nIndex,8,szToY1,sizeof(szToY1));
			nLength = m_listCameraRelation.GetItemText(nIndex,9,szFromX2,sizeof(szFromX2));
			nLength = m_listCameraRelation.GetItemText(nIndex,10,szFromY2,sizeof(szFromY2));
			nLength = m_listCameraRelation.GetItemText(nIndex,11,szToX2,sizeof(szToX2));
			nLength = m_listCameraRelation.GetItemText(nIndex,12,szToY2,sizeof(szToY2));
			nLength = m_listCameraRelation.GetItemText(nIndex,13,szPresetName,sizeof(szPresetName));

			if (g_pMainDlg != NULL&&g_pMainDlg->m_pDlgPageServer != NULL)
			{
				g_pMainDlg->m_pDlgPageServer->CameraTreelistAddCameraByStastion(szStationName);
				g_pMainDlg->m_pDlgPageServer->PresetTreelistAddPresetByStastion(szStationName);
			}

			_T_NODE_PRESET_INFO *pPresetInfo = NULL;
			_T_NODE_PRESET_INFO * pPresetNodeInfo = NULL;
			_T_NODE_INFO * pNodeInfo = NULL;
			HTREEITEM hInfoTreeItem = NULL;

			pPresetInfo = new _T_NODE_PRESET_INFO;
			if (pPresetInfo != NULL)
			{
				memset(pPresetInfo,0,sizeof(_T_NODE_PRESET_INFO));

				strcpy_s(pPresetInfo->node_num,sizeof(pPresetInfo->node_num),szCode);
				strcpy_s(pPresetInfo->node_station,sizeof(pPresetInfo->node_station),szStationName);
	
				pPresetInfo->preset_id = atoi(szPresetId);

				pPresetInfo->line1_from_x = atoi(szFromX1);
				pPresetInfo->line1_from_y = atoi(szFromY1);
				pPresetInfo->line1_to_x = atoi(szToX1);
				pPresetInfo->line1_to_y = atoi(szToY1);

				pPresetInfo->line2_from_x = atoi(szFromX2);
				pPresetInfo->line2_from_y = atoi(szFromY2);
				pPresetInfo->line2_to_x = atoi(szToX2);
				pPresetInfo->line2_to_y = atoi(szToY2);
				strcpy_s(pPresetInfo->preset_name,sizeof(pPresetInfo->preset_name),szPresetName);

				if (g_pMainDlg != NULL&&g_pMainDlg->m_pDlgPageServer != NULL)
				{
					hInfoTreeItem = g_pMainDlg->m_pDlgPageServer->SearchPresetTreePresetHandleTreeItem(NULL,szCode,szPresetName);
					if (hInfoTreeItem != NULL)
					{
						pPresetNodeInfo = (_T_NODE_PRESET_INFO *)g_pMainDlg->m_pDlgPageServer->m_trPreset.GetItemData(hInfoTreeItem);
						if (pPresetNodeInfo != NULL)
						{
							memcpy(&pPresetInfo->camera_info,&pPresetNodeInfo->camera_info,sizeof(pPresetInfo->camera_info));
						}
					}
					else
					{
						hInfoTreeItem = g_pMainDlg->m_pDlgPageServer->SearchCameraTreeCameraHandleTreeItem(NULL,szCode,NULL,NULL);
						if (hInfoTreeItem != NULL)
						{
							pNodeInfo = (_T_NODE_INFO *)g_pMainDlg->m_pDlgPageServer->m_trServer.GetItemData(hInfoTreeItem);
							if (pNodeInfo != NULL)
							{
								memcpy(&pPresetInfo->camera_info,&pNodeInfo->camera_info,sizeof(pPresetInfo->camera_info));
							}
						}
					}
				}
			}
			else
			{
				return;
			}

			sprintf(szReason, "{热点区域监视}-{%s}-{%s}",szName, szStationName);
			if (g_pMainDlg != NULL)
			{
				if (g_pMainDlg->m_iFocuseWindowID >= 0&&g_pMainDlg->m_iFocuseWindowID < 4)
					nScreenId = g_pMainDlg->m_iFocuseWindowID;

				VIDEO_SIP_CALL_PRESET_INFO VideoSipCallPresetInfo;
				memset(&VideoSipCallPresetInfo,0,sizeof(VideoSipCallPresetInfo));

				if (IsCameraVideoLinkByVideoPlatform(g_nClientVideoLinkType,atoi(szDecodeTag)))
				{
					VideoSipCallPresetInfo.nType = 2;
					VideoSipCallPresetInfo.nDecodeTag = atoi(szDecodeTag);
				}
				else
				{
					VideoSipCallPresetInfo.nType = 4;
					VideoSipCallPresetInfo.nDecodeTag = pPresetInfo->camera_info.dvr_info.dvr_type;
				}

				strcpy_s(VideoSipCallPresetInfo.szCode,sizeof(VideoSipCallPresetInfo.szCode),szCode);
				strcpy_s(VideoSipCallPresetInfo.szName,sizeof(VideoSipCallPresetInfo.szName),szName);
				VideoSipCallPresetInfo.nScreenId = nScreenId;
				strcpy_s(VideoSipCallPresetInfo.szReason,sizeof(VideoSipCallPresetInfo.szReason),szReason);
				memcpy(&VideoSipCallPresetInfo.preset_node_info,pPresetInfo,sizeof(VideoSipCallPresetInfo.preset_node_info));

				g_ThreadVideoOperateNumberInfo.DeviceVideoInNumerAdd();
				g_pMainDlg->ThreadPoolDispatchTask(ThreadVideoLinkageMakeCall,(void *)&VideoSipCallPresetInfo,sizeof(VideoSipCallPresetInfo),2);
			}

			Sleep(100);

			if (pPresetInfo != NULL)
			{
				char szInfo[1024] = {0};

				sprintf(szInfo, "<ctrlcamera user=\"admin\" station=\"%s\" name=\"%s\" code=\"%s\" CommandName=\"预置位调用\" command=\"1538\" CommandPara1=\"%d\" CommandPara2=\"0\" CommandPara3=\"0\" srcip=\"%s\"/>",
					szStationName,szName,pPresetInfo->node_num, pPresetInfo->preset_id,app_StackCfg.szLocalUdpAddress);

				SendVideoYTControlInfo(szInfo,strlen(szInfo));

				delete pPresetInfo;
				pPresetInfo = NULL;
			}
		}
	}
	catch(...)
	{

	}

	*pResult = 0;
}

BOOL CDlgLinkageServer::ReadAndSetRelationListInfo(char *szLinkageTime)
{
	try
	{
		if (szLinkageTime == NULL)
			return FALSE;

		m_listRelation.DeleteAllItems();

		char sql_buf[512] = {0};
		bool result=false;

		MYSQL_RES * res = NULL ;
		MYSQL_ROW	row ;

		char szYear[8] = {0};
		int nYear = 2013;

		int nLinkageType = 0;
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

		szYear[0] = szLinkageTime[0];
		szYear[1] = szLinkageTime[1];
		szYear[2] = szLinkageTime[2];
		szYear[3] = szLinkageTime[3];
		szYear[4] = 0;

		nYear = atoi(szYear);

		//读联动信息
		sprintf_s(sql_buf,sizeof(sql_buf),"SELECT linkage_type, station_name, device_name, sub_type, state, screen_id, linkage_time,"
			"content, value1,value2,linkage_station_name,linkage_num FROM ct_linkage_history_log_%s_%04d "
			"where linkage_time >= '%s' ORDER BY id",g_userpower.username,nYear,szLinkageTime);

		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);

			while ( row = mysql_fetch_row( res ) )
			{
				nLinkageType = atoi(row[0]);

				switch(nLinkageType)
				{
				case VIDEO_LINKAGE_INFO_TYPE:
					{
						sprintf_s(szLinkageType,"热点设备联动");

						//					BKColor = RGB(136,16,215);
						TextColor = RGB(0,168,0);
					}
					break;

				case VIDEO_ALARM_LINKAGE_INFO_TYPE:
					{
						sprintf_s(szLinkageType,"告警联动");

						//					BKColor = RGB(136,16,215);
						TextColor = RGB(168,0,0);
					}
					break;

				case VIDEO_STATE_LINKAGE_INFO_TYPE:
					{
						sprintf_s(szLinkageType,"状态联动");
						//					BKColor = RGB(197,228,249);
						TextColor = RGB(64,128,128);
					}
					break;

				case VIDEO_WEATHER_WARNING_INFO_TYPE:
					{
						sprintf_s(szLinkageType,"气象报警");
						TextColor = RGB(116,15,195);
					}
					break;

				case VIDEO_WEATHER_FORECAST_INFO_TYPE:
					{
						sprintf_s(szLinkageType,"气象预警");
						TextColor = RGB(116,15,195);
					}
					break;

				default:
					{
						sprintf_s(szLinkageType,"未知");
						BKColor = RGB(128,128,128);
						TextColor = RGB(30,30,30);
					}
					break;
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

				m_listRelation.InsertItem(0,szLinkageType);
				m_listRelation.SetItemText(0,1,szDeviceName);
				m_listRelation.SetItemText(0,2,szStationName);
				m_listRelation.SetItemText(0,3,szLinkageRealTime);
				m_listRelation.SetItemText(0,4,szSubType);
				m_listRelation.SetItemText(0,5,szState);
				m_listRelation.SetItemText(0,6,szScreenId);
				m_listRelation.SetItemText(0,7,szContent);
				m_listRelation.SetItemText(0,8,szValue1);
				m_listRelation.SetItemText(0,9,szValue2);
				m_listRelation.SetItemText(0,10,szLinkageStationName);
				m_listRelation.SetItemText(0,11,szLinkageNum);

				m_listRelation.SetRowColor(0,BKColor);
				m_listRelation.SetRowTextColor(0,TextColor);
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

BOOL CDlgLinkageServer::ReadAndSetCameraRelationListInfo(int nNodeId,char *szStationName,char *szLinkageTime)
{
	try
	{
		if(nNodeId < 0)
			return FALSE;

		m_listCameraRelation.DeleteAllItems();

		char sql_buf[512] = {0};
		bool result=false;

		MYSQL_RES * res = NULL ;
		MYSQL_ROW	row ;

		char szYear[8] = {0};
		int nYear = 2013;

		char szName[256] = {0};
		char szCode[256] = {0};
		char szDecodeTag[16] = {0};
		char szNodeId[16] = {0};
		char szPresetId[32] = {0};
		char szFromX1[32] = {0};
		char szFromY1[32] = {0};
		char szToX1[32] = {0};
		char szToY1[32] = {0};
		char szFromX2[32] = {0};
		char szFromY2[32] = {0};
		char szToX2[32] = {0};
		char szToY2[32] = {0};

		szYear[0] = szLinkageTime[0];
		szYear[1] = szLinkageTime[1];
		szYear[2] = szLinkageTime[2];
		szYear[3] = szLinkageTime[3];
		szYear[4] = 0;

		nYear = atoi(szYear);
		if (nYear == 0)
			nYear = 2013;

		//读摄像头联动信息
		sprintf_s(sql_buf,sizeof(sql_buf),"SELECT name, code, decode_tag, "
			"preset_id, from_x1, from_y1, to_x1, to_y1, from_x2, from_y2, to_x2, to_y2, node_id "
			"FROM ct_linkage_camera_history_log_%s_%04d "
			"where node_id=%d ORDER BY id",g_userpower.username,nYear,nNodeId);

		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);

			while ( row = mysql_fetch_row( res ) )
			{
				sprintf_s(szName, sizeof(szName),"%s", row[0]);
				sprintf_s(szCode,sizeof(szCode),"%s", row[1]);
				sprintf_s(szDecodeTag,sizeof(szDecodeTag),"%s",row[2]);
				sprintf_s(szPresetId,sizeof(szPresetId),"%s",row[3]);
				sprintf_s(szFromX1,sizeof(szFromX1),"%s",row[4]);
				sprintf_s(szFromY1,sizeof(szFromY1),"%s",row[5]);
				sprintf_s(szToX1,sizeof(szToX1),"%s",row[6]);
				sprintf_s(szToY1,sizeof(szToY1),"%s",row[7]);
				sprintf_s(szFromX2,sizeof(szFromX2),"%s",row[8]);
				sprintf_s(szFromY2,sizeof(szFromY2),"%s",row[9]);
				sprintf_s(szToX2,sizeof(szToX2),"%s",row[10]);
				sprintf_s(szToY2,sizeof(szToY2),"%s",row[11]);
				sprintf_s(szNodeId,sizeof(szNodeId),"%s",row[12]);

				m_listCameraRelation.InsertItem(0,szName);
				m_listCameraRelation.SetItemText(0,1,szCode);
				m_listCameraRelation.SetItemText(0,2,szDecodeTag);
				m_listCameraRelation.SetItemText(0,3,szStationName);
				m_listCameraRelation.SetItemText(0,4,szPresetId);
				m_listCameraRelation.SetItemText(0,5,szFromX1);
				m_listCameraRelation.SetItemText(0,6,szFromY1);
				m_listCameraRelation.SetItemText(0,7,szToX1);
				m_listCameraRelation.SetItemText(0,8,szToY1);
				m_listCameraRelation.SetItemText(0,9,szFromX2);
				m_listCameraRelation.SetItemText(0,10,szFromY2);
				m_listCameraRelation.SetItemText(0,11,szToX2);
				m_listCameraRelation.SetItemText(0,12,szToY2);
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

int  CDlgLinkageServer::GetRelationListInfoId(char *szLinkageType,char *szStationName,char *szDeviceName,char *szSubType,char *szLinkageTime)
{
	try
	{
		char sql_buf[255] = {0};
		bool result=false;

		int nResult = -1;

		int nLinkageType = -1;

		MYSQL_RES * res = NULL ;
		MYSQL_ROW	row ;

		char szYear[8] = {0};
		int nYear = 2013;

		szYear[0] = szLinkageTime[0];
		szYear[1] = szLinkageTime[1];
		szYear[2] = szLinkageTime[2];
		szYear[3] = szLinkageTime[3];
		szYear[4] = 0;

		nYear = atoi(szYear);

		if (nYear == 0)
			nYear = 2013;

		if (strcmp(szLinkageType,"热点设备联动") == 0)
		{
			nLinkageType = VIDEO_LINKAGE_INFO_TYPE;
		}
		else if (strcmp(szLinkageType,"告警联动") == 0)
		{
			nLinkageType = VIDEO_ALARM_LINKAGE_INFO_TYPE;
		}
		else if (strcmp(szLinkageType,"状态联动") == 0)
		{
			nLinkageType = VIDEO_STATE_LINKAGE_INFO_TYPE;
		}
		else if (strcmp(szLinkageType,"气象报警") == 0)
		{
			nLinkageType = VIDEO_WEATHER_WARNING_INFO_TYPE;
		}
		else if (strcmp(szLinkageType,"气象预警") == 0)
		{
			nLinkageType = VIDEO_WEATHER_FORECAST_INFO_TYPE;
		}
		else
		{
			nLinkageType = VIDEO_LINKAGE_INFO_UNDEFINE_TYPE;
		}

		if (nLinkageType == VIDEO_WEATHER_WARNING_INFO_TYPE||nLinkageType == VIDEO_WEATHER_FORECAST_INFO_TYPE)
		{
			sprintf_s(sql_buf, "SELECT id FROM ct_linkage_history_log_%s_%04d "
				"where linkage_type=%d AND device_name='%s' AND value1='%s' AND linkage_time='%s' ORDER BY id",
				g_userpower.username,nYear,nLinkageType,szStationName,szSubType,szLinkageTime);
		}
		else
		{
			sprintf_s(sql_buf, "SELECT id FROM ct_linkage_history_log_%s_%04d "
				"where linkage_type=%d AND station_name='%s' AND device_name='%s' AND sub_type='%s' AND linkage_time='%s' ORDER BY id",
				g_userpower.username,nYear,nLinkageType,szStationName,szDeviceName,szSubType,szLinkageTime);
		}

		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);

			while ( row = mysql_fetch_row( res ) )
			{
				nResult = atoi(row[0]);
			}

			mysql_free_result( res ) ;
		}

		return nResult;
	}
	catch(...)
	{

	}
	return -1;
}

BOOL CDlgLinkageServer::ReadAndSetRelationListInfo2(char *szLinkageTime)
{
	try
	{
		if (szLinkageTime == NULL)
			return FALSE;

		m_listRelation.DeleteAllItems();

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
		int    nLinkageNum = 0;

		COLORREF BKColor;
		COLORREF TextColor;

		szYear[0] = szLinkageTime[0];
		szYear[1] = szLinkageTime[1];
		szYear[2] = szLinkageTime[2];
		szYear[3] = szLinkageTime[3];
		szYear[4] = 0;

		nYear = atoi(szYear);

		int nItemNum = 0;
		//读联动信息
		sprintf_s(sql_buf, sizeof(sql_buf),"SELECT event, station, device, type, state, screen_id, time,"
			"content, yx_value,yc_value,station,link_num,weather_name FROM history_relation_%04d "
			"where time >= '%s' AND (event='视频联动'OR event='告警联动' OR event='状态联动' OR event='气象报警' OR event='气象预警') ORDER BY id desc",nYear,szLinkageTime);

		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);

			while ( row = mysql_fetch_row( res ) )
			{
				if (nItemNum > 1000)
				{
					return TRUE;
				}
				sprintf_s(szLinkageType, "%s", row[0]);
				sprintf_s(szLinkageNum,"%s",row[11]);
				nLinkageNum = atoi(szLinkageNum);
				if (nLinkageNum <= 0)
					continue;

				sprintf_s(szStationName, "%s", row[1]);
				sprintf_s(szDeviceName, "%s", row[2]);
				sprintf_s(szSubType,"%s",row[3]);

				if (strcmp(szLinkageType,"热点设备联动") == 0)
				{
					//				BKColor = RGB(136,16,215);
					TextColor = RGB(0,168,0);
				}
				else if (strcmp(szLinkageType,"视频联动") == 0)
				{
					strcpy(szLinkageType,"热点设备联动");
					TextColor = RGB(0,168,0);
				}
				else if (strcmp(szLinkageType,"告警联动") == 0)
				{
					//				BKColor = RGB(136,16,215);
					TextColor = RGB(168,0,0);
				}
				else if (strcmp(szLinkageType,"状态联动") == 0)
				{
					BKColor = RGB(197,228,249);
					TextColor = RGB(64,128,128);
				}
				else if (strcmp(szLinkageType,"气象报警"))
				{
					BKColor = RGB(207,235,250);
					TextColor = RGB(116,15,195);
					sprintf_s(szSubType,"%s",row[12]);
					sprintf_s(szDeviceName, "%s", row[1]);
				}
				else if (strcmp(szLinkageType,"气象预警"))
				{
					BKColor = RGB(207,235,250);
					TextColor = RGB(116,15,195);
					sprintf_s(szSubType,"%s",row[12]);
					sprintf_s(szDeviceName, "%s", row[1]);
				}
				else
				{
					BKColor = RGB(128,128,128);
					TextColor = RGB(30,30,30);
					break;
				}

				sprintf_s(szState,"%s",row[4]);
				sprintf_s(szScreenId,"%s",row[5]);
				sprintf_s(szLinkageRealTime,"%s",row[6]);
				sprintf_s(szContent,"%s",row[7]);
				sprintf_s(szValue1, "%s", row[8]);
				sprintf_s(szValue2,"%s",row[9]);
				sprintf_s(szLinkageStationName,"%s",row[10]);

				m_listRelation.InsertItem(nItemNum,szLinkageType);
				m_listRelation.SetItemText(nItemNum,1,szDeviceName);
				m_listRelation.SetItemText(nItemNum,2,szStationName);
				m_listRelation.SetItemText(nItemNum,3,szLinkageRealTime);
				m_listRelation.SetItemText(nItemNum,4,szSubType);
				m_listRelation.SetItemText(nItemNum,5,szState);
				m_listRelation.SetItemText(nItemNum,6,szScreenId);
				m_listRelation.SetItemText(nItemNum,7,szContent);
				m_listRelation.SetItemText(nItemNum,8,szValue1);
				m_listRelation.SetItemText(nItemNum,9,szValue2);
				m_listRelation.SetItemText(nItemNum,10,szLinkageStationName);
				m_listRelation.SetItemText(nItemNum,11,szLinkageNum);

				m_listRelation.SetRowColor(nItemNum,BKColor);
				m_listRelation.SetRowTextColor(nItemNum,TextColor);

				nItemNum ++;
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

BOOL CDlgLinkageServer::ReadAndSetCameraRelationListInfo2(int nNodeId,char *szStationName,char *szLinkageTime)
{
	try
	{
		if(nNodeId < 0)
			return FALSE;

		m_listCameraRelation.DeleteAllItems();

		char sql_buf[512] = {0};
		bool result=false;

		MYSQL_RES * res = NULL ;
		MYSQL_ROW	row ;

		char szYear[8] = {0};
		int nYear = 2013;

		char szName[256] = {0};
		char szCode[256] = {0};
		char szDecodeTag[16] = {0};
		char szNodeId[16] = {0};
		char szPresetId[32] = {0};
		char szFromX1[32] = {0};
		char szFromY1[32] = {0};
		char szToX1[32] = {0};
		char szToY1[32] = {0};
		char szFromX2[32] = {0};
		char szFromY2[32] = {0};
		char szToX2[32] = {0};
		char szToY2[32] = {0};

		szYear[0] = szLinkageTime[0];
		szYear[1] = szLinkageTime[1];
		szYear[2] = szLinkageTime[2];
		szYear[3] = szLinkageTime[3];
		szYear[4] = 0;

		nYear = atoi(szYear);

		//读摄像头联动信息
		sprintf_s(sql_buf,sizeof(sql_buf),"SELECT device, code, decoder_tag, "
			"act_para1, from_x_1, from_y_1, to_x_1, to_y_1, from_x_2, from_y_2, to_x_2, to_y_2, relate_id "
			"FROM history_relation_%04d "
			"where relate_id=%d AND event='控制云台' ORDER BY id",nYear,nNodeId);

		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);

			while ( row = mysql_fetch_row( res ) )
			{
				sprintf_s(szName, sizeof(szName),"%s", row[0]);
				sprintf_s(szCode,sizeof(szCode),"%s", row[1]);
				sprintf_s(szDecodeTag,sizeof(szDecodeTag),"%s",row[2]);
				sprintf_s(szPresetId,sizeof(szPresetId),"%s",row[3]);
				sprintf_s(szFromX1,sizeof(szFromX1),"%s",row[4]);
				sprintf_s(szFromY1,sizeof(szFromY1),"%s",row[5]);
				sprintf_s(szToX1,sizeof(szToX1),"%s",row[6]);
				sprintf_s(szToY1,sizeof(szToY1),"%s",row[7]);
				sprintf_s(szFromX2,sizeof(szFromX2),"%s",row[8]);
				sprintf_s(szFromY2,sizeof(szFromY2),"%s",row[9]);
				sprintf_s(szToX2,sizeof(szToX2),"%s",row[10]);
				sprintf_s(szToY2,sizeof(szToY2),"%s",row[11]);
				sprintf_s(szNodeId,sizeof(szNodeId),"%s",row[12]);

				m_listCameraRelation.InsertItem(0,szName);
				m_listCameraRelation.SetItemText(0,1,szCode);
				m_listCameraRelation.SetItemText(0,2,szDecodeTag);
				m_listCameraRelation.SetItemText(0,3,szStationName);
				m_listCameraRelation.SetItemText(0,4,szPresetId);
				m_listCameraRelation.SetItemText(0,5,szFromX1);
				m_listCameraRelation.SetItemText(0,6,szFromY1);
				m_listCameraRelation.SetItemText(0,7,szToX1);
				m_listCameraRelation.SetItemText(0,8,szToY1);
				m_listCameraRelation.SetItemText(0,9,szFromX2);
				m_listCameraRelation.SetItemText(0,10,szFromY2);
				m_listCameraRelation.SetItemText(0,11,szToX2);
				m_listCameraRelation.SetItemText(0,12,szToY2);
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

int  CDlgLinkageServer::GetRelationListInfoId2(char *szLinkageType,char *szStationName,char *szDeviceName,char *szSubType,char *szLinkageTime)
{
	try
	{
		char sql_buf[255] = {0};
		bool result=false;

		int nResult = -1;
		int nLinkageType = VIDEO_LINKAGE_INFO_UNDEFINE_TYPE;

		MYSQL_RES * res = NULL ;
		MYSQL_ROW	row ;

		char szYear[8] = {0};
		int nYear = 2013;

		szYear[0] = szLinkageTime[0];
		szYear[1] = szLinkageTime[1];
		szYear[2] = szLinkageTime[2];
		szYear[3] = szLinkageTime[3];
		szYear[4] = 0;

		nYear = atoi(szYear);

		if (strcmp(szLinkageType,"热点设备联动") == 0)
		{
			strcpy(szLinkageType,"视频联动");
			nLinkageType = VIDEO_LINKAGE_INFO_TYPE;
		}
		else if (strcmp(szLinkageType,"告警联动") == 0)
		{
			nLinkageType = VIDEO_ALARM_LINKAGE_INFO_TYPE;
		}
		else if (strcmp(szLinkageType,"状态联动") == 0)
		{
			nLinkageType = VIDEO_STATE_LINKAGE_INFO_TYPE;
		}
		else if (strcmp(szLinkageType,"气象报警"))
		{
			nLinkageType = VIDEO_WEATHER_WARNING_INFO_TYPE;
		}
		else if (strcmp(szLinkageType,"气象预警"))
		{
			nLinkageType = VIDEO_WEATHER_FORECAST_INFO_TYPE;
		}

		if (nLinkageType == VIDEO_WEATHER_WARNING_INFO_TYPE||nLinkageType == VIDEO_WEATHER_FORECAST_INFO_TYPE)
		{
			sprintf_s(sql_buf, "SELECT id FROM history_relation_%04d "
				"where event='%s' AND station='%s' AND weather_name='%s' AND time='%s' ORDER BY id",
				nYear,szLinkageType,szStationName,szSubType,szLinkageTime);
		}
		else
		{
			sprintf_s(sql_buf, "SELECT id FROM history_relation_%04d "
				"where event='%s' AND station='%s' AND device='%s' AND type='%s' AND time='%s' ORDER BY id",
				nYear,szLinkageType,szStationName,szDeviceName,szSubType,szLinkageTime);
		}

		if (!mysql_query(g_mySqlData, sql_buf))
		{
			res = mysql_store_result(g_mySqlData);

			while ( row = mysql_fetch_row( res ) )
			{
				nResult = atoi(row[0]);
			}

			mysql_free_result( res ) ;
		}

		return nResult;
	}
	catch(...)
	{

	}

	return -1;
}

BOOL CDlgLinkageServer::OnEraseBkgnd(CDC* pDC)
{
	//////////////////////////////////////////////////////////////////////////
	if (pDC == NULL)
		return TRUE;

	CRect   rect;
	GetClientRect(&rect);

	CDC   dcMem;
	CBitmap*   pOldBitmap = NULL;

	dcMem.CreateCompatibleDC(pDC);   
	pOldBitmap = dcMem.SelectObject(&m_LinkageServerBmp);   

	pDC->BitBlt(
		0,
		0,
		rect.right-rect.left,
		rect.bottom-rect.top,
		&dcMem,
		0,
		0,
		SRCCOPY);

	//右边
	dcMem.SelectObject(&m_LeftBmp);   

	pDC->StretchBlt(rect.left,rect.top,
		m_LeftBm.bmWidth+3,rect.bottom-rect.top,
		&dcMem,
		0,0,
		m_LeftBm.bmWidth,m_LeftBm.bmHeight,
		SRCCOPY);

	dcMem.SelectObject(pOldBitmap);

	return TRUE;
}

void CDlgLinkageServer::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//////////////////////////////////////////////////////////////////////////
	if (GetDlgItem(IDC_LIST_RELATION)->GetSafeHwnd())
	{
		RECT rc;		
		CWnd *pWnd;
		CRect win_rc;

		if (g_pMainDlg == NULL)
			return;

	   g_pMainDlg->GetWindowRect(&win_rc);

		//布置
		rc.top = 2;
		rc.bottom = cy-2;
		rc.left = 0;
		rc.right = cx-2;

		pWnd = GetDlgItem(IDC_BTN_HIDE_LINKAGE_SERVER);//显示按钮
		if (pWnd)
		{
			CRect rect;
			POINT pt;

			pt.x = 0;
			pt.y = (win_rc.top+win_rc.bottom)/2-50;

			ScreenToClient(&pt);

			rect.left=0 ;			
			rect.right=20;
			rect.top= pt.y;
			rect.bottom=rect.top+200;

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDC_LIST_RELATION);     //联动列表
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			CRect rect;					

			rect.left=rc.left+13;			
			rect.right=rc.right-3;
			rect.top=rc.top;
			rect.bottom=(rc.top + rc.bottom - 10)/2 - 5;

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDC_LIST_CAMERA_RELATION);     //联动摄像头列表
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			CRect rect;					

			rect.left=rc.left+13;			
			rect.right=rc.right-3;
			rect.top=(rc.top + rc.bottom - 10)/2 + 5;
			rect.bottom=rc.bottom - 10 - 150;

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDC_BTN_EXTEND);     //联动显示按钮
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			CRect rect;					

			rect.left=0;			
			rect.right=20;
			rect.top=rc.top;
			rect.bottom=rc.top + 40;

			pWnd->MoveWindow(rect);		
		}

		rc.left+=20;

		//pWnd = GetDlgItem(IDC_CHECK_RELATE_NOTIFY);  //D5000告知-联动显示按钮
		//if(pWnd)
		//{
		//	CRect rect;					

		//	rect.left=rc.left-3;			
		//	rect.right=rc.left+108;
		//	rect.bottom=rc.bottom - 3;
		//	rect.top=rect.bottom - 20;

		//	pWnd->MoveWindow(rect);		
		//}

		//pWnd = GetDlgItem(IDC_CHECK_RELATE_LIMIT);     //D5000越限-联动显示按钮
		//if(pWnd)
		//{
		//	CRect rect;					

		//	rect.left=rc.left-3;			
		//	rect.right=rc.left+108;
		//	rect.bottom=rc.bottom - 3 - 20*1;
		//	rect.top=rect.bottom - 20;

		//	pWnd->MoveWindow(rect);		
		//}

		//pWnd = GetDlgItem(IDC_CHECK_RELATE_SWITCHSTATE);     //D5000开关变位-联动显示按钮
		//if(pWnd)
		//{
		//	CRect rect;					

		//	rect.left=rc.left-3;			
		//	rect.right=rc.left+108;
		//	rect.bottom=rc.bottom - 3 - 20*2;
		//	rect.top=rect.bottom - 20;

		//	pWnd->MoveWindow(rect);		
		//}

		//pWnd = GetDlgItem(IDC_CHECK_RELATE_STATE);     //D5000变位-联动显示按钮
		//if(pWnd)
		//{
		//	CRect rect;					

		//	rect.left=rc.left-3;			
		//	rect.right=rc.left+108;
		//	rect.bottom=rc.bottom - 3 - 20*3;
		//	rect.top=rect.bottom - 20;

		//	pWnd->MoveWindow(rect);		
		//}

		pWnd = GetDlgItem(IDC_CHECK_RELATE_ABNORMAL); //D5000异常-联动显示按钮
		if(pWnd)
		{
			CRect rect;					

			rect.left=rc.left-3;			
			rect.right=rc.left+108;
			rect.bottom=rc.bottom - 3 - 20*1;
			rect.top=rect.bottom - 20;

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDC_CHECK_RELATE_ACCIDENT);     //D5000事故-联动显示按钮
		if(pWnd)
		{
			CRect rect;					

			rect.left=rc.left-3;			
			rect.right=rc.left+108;
			rect.bottom=rc.bottom - 3 - 20*3;
			rect.top=rect.bottom - 20;

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDC_CHECK_RELATE_VIDEO);     //热点视频监视-联动显示按钮
		if(pWnd)
		{
			CRect rect;					

			rect.left=rc.left-3;			
			rect.right=rc.left+108;
			rect.bottom=rc.bottom - 3 - 20*5;
			rect.top=rect.bottom - 20;

			pWnd->MoveWindow(rect);		
		}


		//pWnd = GetDlgItem(IDC_CHECK_RELATE_THUNDER_PRE);     //雷电预警-联动显示按钮
		//if(pWnd)
		//{
		//	CRect rect;					

		//	rect.left=rc.left+110;			
		//	rect.right=rc.right;
		//	rect.bottom=rc.bottom - 3 - 20*1;
		//	rect.top=rect.bottom - 20;

		//	pWnd->MoveWindow(rect);		
		//}

		pWnd = GetDlgItem(IDC_CHECK_RELATE_THUNDER_ALARM);     //雷电告警-联动显示按钮
		if(pWnd)
		{
			CRect rect;					

			rect.left=rc.left+110;
			rect.right=rc.right;
			rect.bottom=rc.bottom - 3 - 20*3;
			rect.top=rect.bottom - 20;

			pWnd->MoveWindow(rect);		
		}

		//pWnd = GetDlgItem(IDC_CHECK_RELATE_TYPHOON_PRE);     //台风预警-联动显示按钮
		//if(pWnd)
		//{
		//	CRect rect;					

		//	rect.left=rc.left+110;
		//	rect.right=rc.right;
		//	rect.bottom=rc.bottom - 3 - 20*3;
		//	rect.top=rect.bottom - 20;

		//	pWnd->MoveWindow(rect);		
		//}

		//pWnd = GetDlgItem(IDC_CHECK_RELATE_TYPHOON_ALARM);     //台风告警-联动显示按钮
		//if(pWnd)
		//{
		//	CRect rect;					

		//	rect.left=rc.left+110;
		//	rect.right=rc.right;
		//	rect.bottom=rc.bottom - 3 - 20*4;
		//	rect.top=rect.bottom - 20;

		//	pWnd->MoveWindow(rect);		
		//}

		//pWnd = GetDlgItem(IDC_CHECK_RELATE_METRO_PRE);     //气象预警-联动显示按钮
		//if(pWnd)
		//{
		//	CRect rect;					

		//	rect.left=rc.left+110;
		//	rect.right=rc.right;
		//	rect.bottom=rc.bottom - 3 - 20*5;
		//	rect.top=rect.bottom - 20;

		//	pWnd->MoveWindow(rect);		
		//}

		pWnd = GetDlgItem(IDC_CHECK_RELATE_METRO_ALARM);     //气象告警-联动显示按钮
		if(pWnd)
		{
			CRect rect;					

			rect.left=rc.left+110;
			rect.right=rc.right;
			rect.bottom=rc.bottom - 3 - 20*5;
			rect.top=rect.bottom - 20;

			pWnd->MoveWindow(rect);		
		}

		pWnd = GetDlgItem(IDC_STATIC_TYPE_SELECT);     //热点类型选择
		if(pWnd)
		{
			CRect rect;					

			rect.left=rc.left-8;
			rect.right=rc.right-1;
			rect.bottom=rc.bottom - 3 ;
			rect.top=rect.bottom - 20*6 - 30;

			pWnd->MoveWindow(rect);		
		}
	}
}

void CDlgLinkageServer::OnBnClickedBtnExtend()
{
	try
	{
		if (m_bRelationListExtendFlag == FALSE)
		{
			// 之前显示缩小的
			g_pMainDlg->ShowLargeRelationList();
			m_bRelationListExtendFlag = TRUE;
			m_btnExtend.SetIndex(1);
			m_btnExtend.Invalidate(TRUE);
		}
		else
		{
			// 之前显示放大的
			g_pMainDlg->ShowSmallRelationList();
			m_bRelationListExtendFlag = FALSE;
			m_btnExtend.SetIndex(0);
			m_btnExtend.Invalidate(TRUE);
		}
	}
	catch(...)
	{

	}
}
void CDlgLinkageServer::OnBnClickedCheckRelateVideo()
{
	try
	{
		UpdateData(true);
		//	m_CheckRelateVideo.SetCheck(!m_CheckRelateVideo.GetCheck());

		char szValue[10] = {0x0};
		char szPathName [256] = {0x0};
		sprintf(szValue, "%d", m_CheckRelateVideo.GetCheck());
		memset(szPathName, 0, sizeof(szPathName));

		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");
		WritePrivateProfileString(methodid, "video", szValue, szPathName);
	}
	catch(...)
	{

	}
}

void CDlgLinkageServer::OnBnClickedCheckRelateAccident()
{
	try
	{
		UpdateData(true);
		//	m_CheckRelateAccident.SetCheck(!m_CheckRelateAccident.GetCheck());

		char szValue[16] = {0};
		char szPathName [256] = {0};
		sprintf(szValue, "%d", m_CheckRelateAccident.GetCheck());
		memset(szPathName, 0, sizeof(szPathName));

		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");
		WritePrivateProfileString(methodid, "accident", szValue, szPathName);
	}
	catch(...)
	{

	}
}

void CDlgLinkageServer::OnBnClickedCheckRelateAbnormal()
{
	try
	{
		UpdateData(true);
		//	m_CheckRelateAbnormal.SetCheck(!m_CheckRelateAbnormal.GetCheck());

		char szValue[10] = {0x0};
		char szPathName [256] = {0x0};
		sprintf(szValue, "%d", m_CheckRelateAbnormal.GetCheck());
		memset(szPathName, 0, sizeof(szPathName));
		
		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");
		WritePrivateProfileString(methodid, "abnormal", szValue, szPathName);
	}
	catch(...)
	{

	}
}

void CDlgLinkageServer::OnBnClickedCheckRelateState()
{
	try
	{
		UpdateData(true);
		//	m_CheckRelateState.SetCheck(!m_CheckRelateState.GetCheck());

		char szValue[10] = {0x0};
		char szPathName [256] = {0x0};
		sprintf(szValue, "%d", m_CheckRelateState.GetCheck());
		memset(szPathName, 0, sizeof(szPathName));
		
		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");
		WritePrivateProfileString(methodid, "state", szValue, szPathName);
	}
	catch(...)
	{

	}
}

void CDlgLinkageServer::OnBnClickedCheckRelateSwitchstate()
{
	try
	{
		UpdateData(true);
		//	m_CheckRelateSwitchState.SetCheck(!m_CheckRelateSwitchState.GetCheck());

		char szValue[10] = {0x0};
		char szPathName [256] = {0x0};
		sprintf(szValue, "%d", m_CheckRelateSwitchState.GetCheck());
		memset(szPathName, 0, sizeof(szPathName));
		
		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");
		WritePrivateProfileString(methodid, "switchstate", szValue, szPathName);
	}
	catch(...)
	{

	}
}

void CDlgLinkageServer::OnBnClickedCheckRelateLimit()
{
	try
	{
		UpdateData(true);
		//	m_CheckRelateLimit.SetCheck(!m_CheckRelateLimit.GetCheck());

		char szValue[10] = {0x0};
		char szPathName [256] = {0x0};
		sprintf(szValue, "%d", m_CheckRelateLimit.GetCheck());
		memset(szPathName, 0, sizeof(szPathName));
		
		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");
		WritePrivateProfileString(methodid, "limit", szValue, szPathName);
	}
	catch(...)
	{

	}
}

void CDlgLinkageServer::OnBnClickedCheckRelateNotify()
{
	try
	{
		UpdateData(true);
		//	m_CheckRelateNotify.SetCheck(!m_CheckRelateNotify.GetCheck());

		char szValue[10] = {0x0};
		char szPathName [256] = {0x0};
		sprintf(szValue, "%d", m_CheckRelateNotify.GetCheck());
		memset(szPathName, 0, sizeof(szPathName));
		
		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");
		WritePrivateProfileString(methodid, "notify", szValue, szPathName);
	}
	catch(...)
	{

	}
}

void CDlgLinkageServer::OnBnClickedCheckRelateMetroAlarm()
{
	try
	{
		UpdateData(true);
		//	m_CheckRelateMetroAlarm.SetCheck(!m_CheckRelateMetroAlarm.GetCheck());

		char szValue[10] = {0x0};
		char szPathName [256] = {0x0};
		sprintf(szValue, "%d", m_CheckRelateMetroAlarm.GetCheck());
		memset(szPathName, 0, sizeof(szPathName));
		
		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");
		WritePrivateProfileString(methodid, "metroalarm", szValue, szPathName);
	}
	catch(...)
	{

	}
}

void CDlgLinkageServer::OnBnClickedCheckRelateMetroPre()
{
	try
	{
		UpdateData(true);
		//	m_CheckRelateMetroPre.SetCheck(!m_CheckRelateMetroPre.GetCheck());

		char szValue[10] = {0x0};
		char szPathName [256] = {0x0};
		sprintf(szValue, "%d", m_CheckRelateMetroPre.GetCheck());
		memset(szPathName, 0, sizeof(szPathName));
		
		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");
		WritePrivateProfileString(methodid, "metropre", szValue, szPathName);
	}
	catch(...)
	{

	}
}

void CDlgLinkageServer::OnBnClickedCheckRelateTyphoonPre()
{
	try
	{
		UpdateData(true);
		//	m_CheckRelateTyphoonPre.SetCheck(!m_CheckRelateTyphoonPre.GetCheck());

		char szValue[10] = {0x0};
		char szPathName [256] = {0x0};
		sprintf(szValue, "%d", m_CheckRelateTyphoonPre.GetCheck());
		memset(szPathName, 0, sizeof(szPathName));
		
		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");
		WritePrivateProfileString(methodid, "typhoonpre", szValue, szPathName);
	}
	catch(...)
	{

	}
}

void CDlgLinkageServer::OnBnClickedCheckRelateTyphoonAlarm()
{
	try
	{
		UpdateData(true);
		//	m_CheckRelateTyphoonAlarm.SetCheck(!m_CheckRelateTyphoonAlarm.GetCheck());

		char szValue[10] = {0x0};
		char szPathName [256] = {0x0};
		sprintf(szValue, "%d", m_CheckRelateTyphoonAlarm.GetCheck());
		memset(szPathName, 0, sizeof(szPathName));
		
		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");
		WritePrivateProfileString(methodid, "typhoonalarm", szValue, szPathName);
	}
	catch(...)
	{

	}
}

void CDlgLinkageServer::OnBnClickedCheckRelateThunderPre()
{
	try
	{
		UpdateData(true);
		//	m_CheckRelateTyphoonPre.SetCheck(!m_CheckRelateTyphoonPre.GetCheck());

		char szValue[10] = {0x0};
		char szPathName [256] = {0x0};
		sprintf(szValue, "%d", m_CheckRelateThunderPre.GetCheck());
		memset(szPathName, 0, sizeof(szPathName));
		
		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");
		WritePrivateProfileString(methodid, "thunderpre", szValue, szPathName);
	}
	catch(...)
	{

	}
}

void CDlgLinkageServer::OnBnClickedCheckRelateThunderAlarm()
{
	try
	{
		UpdateData(true);
		//	m_CheckRelateTyphoonAlarm.SetCheck(!m_CheckRelateTyphoonAlarm.GetCheck());

		char szValue[10] = {0x0};
		char szPathName [256] = {0x0};
		sprintf(szValue, "%d", m_CheckRelateThunderAlarm.GetCheck());
		memset(szPathName, 0, sizeof(szPathName));
		
		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");
		WritePrivateProfileString(methodid, "thunderalarm", szValue, szPathName);
	}
	catch(...)
	{

	}
}

HBRUSH CDlgLinkageServer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL CDlgLinkageServer::ReadRelateActionIni()
{
	try
	{
		char szPathName[256];
		memset(szPathName, 0, sizeof(szPathName));
		
		if (strlen(g_szAppPath) > 0)
		{
			strcpy_s(szPathName,sizeof(szPathName)-1,g_szAppPath);
		}
		else
		{
			GetCurrentDirectory(sizeof(szPathName)-1, szPathName);
		}

		strcat(szPathName, "\\RelateAction.ini");
		char methodid[20] = {0x0};
		sprintf(methodid, "RelateControl");

		CFileFind tempFind;
		BOOL bIsFinded = (BOOL)tempFind.FindFile(szPathName);
		if (!bIsFinded)
		{
			// 新建配置文件
			CFile file;
			file.Open(szPathName,CFile::modeWrite | CFile::modeCreate | CFile::typeBinary);
			file.Close();

			WritePrivateProfileString(methodid, "video", "1", szPathName);
			WritePrivateProfileString(methodid, "accident", "1", szPathName);
			WritePrivateProfileString(methodid, "abnormal", "1", szPathName);
			WritePrivateProfileString(methodid, "state", "0", szPathName);
			WritePrivateProfileString(methodid, "switchstate", "0", szPathName);
			WritePrivateProfileString(methodid, "limit", "0", szPathName);
			WritePrivateProfileString(methodid, "notify", "0", szPathName);
			WritePrivateProfileString(methodid, "metroalarm", "0", szPathName);
			WritePrivateProfileString(methodid, "metropre", "0", szPathName);
			WritePrivateProfileString(methodid, "typhoonpre", "0", szPathName);
			WritePrivateProfileString(methodid, "typhoonalarm", "0", szPathName);
			WritePrivateProfileString(methodid, "thunderpre", "0", szPathName);
			WritePrivateProfileString(methodid, "thunderalarm", "0", szPathName);

			m_CheckRelateVideo.SetCheck(true);
			m_CheckRelateAccident.SetCheck(true);
			m_CheckRelateAbnormal.SetCheck(true);
		}
		else
		{
			char szValue[20] = {0x0};
			int nRet = GetPrivateProfileString(methodid,"video","1",szValue,50,szPathName);
			m_CheckRelateVideo.SetCheck(atoi(szValue));
			nRet = GetPrivateProfileString(methodid,"accident","1",szValue,50,szPathName);
			m_CheckRelateAccident.SetCheck(atoi(szValue));
			nRet = GetPrivateProfileString(methodid,"abnormal","1",szValue,50,szPathName);
			m_CheckRelateAbnormal.SetCheck(atoi(szValue));
			nRet = GetPrivateProfileString(methodid,"state","0",szValue,50,szPathName);
			m_CheckRelateState.SetCheck(atoi(szValue));
			nRet = GetPrivateProfileString(methodid,"switchstate","0",szValue,50,szPathName);
			m_CheckRelateSwitchState.SetCheck(atoi(szValue));
			nRet = GetPrivateProfileString(methodid,"limit","0",szValue,50,szPathName);
			m_CheckRelateLimit.SetCheck(atoi(szValue));
			nRet = GetPrivateProfileString(methodid,"notify","0",szValue,50,szPathName);
			m_CheckRelateNotify.SetCheck(atoi(szValue));
			nRet = GetPrivateProfileString(methodid,"metroalarm","0",szValue,50,szPathName);
			m_CheckRelateMetroAlarm.SetCheck(atoi(szValue));
			nRet = GetPrivateProfileString(methodid,"metropre","0",szValue,50,szPathName);
			m_CheckRelateMetroPre.SetCheck(atoi(szValue));
			nRet = GetPrivateProfileString(methodid,"typhoonpre","0",szValue,50,szPathName);
			m_CheckRelateTyphoonPre.SetCheck(atoi(szValue));
			nRet = GetPrivateProfileString(methodid,"typhoonalarm","0",szValue,50,szPathName);
			m_CheckRelateTyphoonAlarm.SetCheck(atoi(szValue));
			nRet = GetPrivateProfileString(methodid,"thunderpre","0",szValue,50,szPathName);
			m_CheckRelateThunderPre.SetCheck(atoi(szValue));
			nRet = GetPrivateProfileString(methodid,"thunderalarm","0",szValue,50,szPathName);
			m_CheckRelateThunderAlarm.SetCheck(atoi(szValue));
		}
		return TRUE;
	}
	catch(...)
	{

	}

	return FALSE;
}

BOOL CDlgLinkageServer::DeleteRelationListWeatherInfo(char *szLinkageType,char szStationName,char *szDeviceName,char *szSubType)
{
	int nItemCount = m_listRelation.GetItemCount();

	for (int i = 0;i < nItemCount;i++)
	{

	}

	return TRUE;
}

void CDlgLinkageServer::CameraRelationInsertCameraInfoByListRelation()
{
	try
	{
		//////////////////////////////////////////////////////////////////////////
		// 在摄像头列表中显示关联摄像头
		POSITION ps =m_listRelation.GetFirstSelectedItemPosition();
		int nIndex  = -1;

		char szLinkageType[64] = {0};
		char szStationName[256] = {0};
		char szDeviceName[256] = {0};
		char szSubType[64] = {0};
		char szLinkageTime[64] = {0};

		int nNodeId = -1;
		int nLength = 0;

		if (ps != NULL)
		{
			nIndex = m_listRelation.GetNextSelectedItem(ps);
			nLength = m_listRelation.GetItemText(nIndex,0,szLinkageType,sizeof(szLinkageType));
			nLength = m_listRelation.GetItemText(nIndex,1,szDeviceName,sizeof(szDeviceName));
			nLength = m_listRelation.GetItemText(nIndex,2,szStationName,sizeof(szStationName));
			nLength = m_listRelation.GetItemText(nIndex,3,szLinkageTime,sizeof(szLinkageTime));
			nLength = m_listRelation.GetItemText(nIndex,4,szSubType,sizeof(szSubType));

			if (strcmp(szLinkageType,"告警联动") == 0)
			{
				g_nViewOutVideoTypeFlag = VIDEO_ALARM_LINKAGE_INFO_TYPE;
			}
			else if (strcmp(szLinkageType,"热点设备联动") == 0||strcmp(szLinkageType,"视频联动") == 0)
			{
				g_nViewOutVideoTypeFlag = VIDEO_LINKAGE_INFO_TYPE;
				if (strcmp(szSubType,"手动") == 0)
				{
					szSubType[0] = '1';
					szSubType[1] = 0;
				}
				else if (strcmp(szSubType,"巡检") == 0)
				{
					szSubType[0] = '2';
					szSubType[1] = 0;
				}
			}
			else if (strcmp(szLinkageType,"气象报警") == 0)
			{
				g_nViewOutVideoTypeFlag = VIDEO_WEATHER_WARNING_INFO_TYPE;
			}
			else if (strcmp(szLinkageType,"气象预警") == 0)
			{
				g_nViewOutVideoTypeFlag = VIDEO_WEATHER_FORECAST_INFO_TYPE;
			}

			m_listCameraRelation.DeleteAllItems();

			nNodeId = GetRelationListInfoId(szLinkageType,szStationName,szDeviceName,szSubType,szLinkageTime);

			if (nNodeId >= 0)
			{
				ReadAndSetCameraRelationListInfo(nNodeId,szStationName,szLinkageTime);
			}
			else if (nNodeId < 0)
			{
				if (szSubType[0] == '1'&&szSubType[1] == 0)
					strcpy_s(szSubType,sizeof(szSubType),"手动");
				else if (szSubType[0] == '2'&&szSubType[1] == 0)
					strcpy_s(szSubType,sizeof(szSubType),"巡检");
				nNodeId = GetRelationListInfoId2(szLinkageType,szStationName,szDeviceName,szSubType,szLinkageTime);
				if (nNodeId >= 0)
					ReadAndSetCameraRelationListInfo2(nNodeId,szStationName,szLinkageTime);
			}
		}
	}
	catch(...)
	{

	}
}