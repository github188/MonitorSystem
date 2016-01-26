// DlgCameraAndControl.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgCameraAndControl.h"
#include "ViewChannel.h"
#include "AssListControlOperate.h"


// CDlgCameraAndControl 对话框

IMPLEMENT_DYNAMIC(CDlgCameraAndControl, CDialog)

CDlgCameraAndControl::CDlgCameraAndControl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCameraAndControl::IDD, pParent)
{
	m_bOpenFlag = false;
	m_nSpeed = 5;
	m_pViewChannel = NULL;
}

CDlgCameraAndControl::~CDlgCameraAndControl()
{
}

void CDlgCameraAndControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CAMERA, m_staticCameraWnd);
	DDX_Control(pDX, IDC_COMBO_SPEED, m_comboSpeed);
	DDX_Control(pDX, IDC_LIST_ALARM, m_lstAlarm);
}


BEGIN_MESSAGE_MAP(CDlgCameraAndControl, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UP, &CDlgCameraAndControl::OnBnClickedButtonUp)
	ON_MESSAGE(OM_MESSAGE_CAMERA_CONTROL,&CDlgCameraAndControl::OnStartYTControl)
	ON_MESSAGE(OM_ELECTREE_TO_CAMERAANDCONTROL_STATIONID, &CDlgCameraAndControl::GetAlarmInfoByStationIdHandle)
	ON_COMMAND(ID_DLG_CAMERA_AND_CONTROL_CLOSE_VIDEO,&CDlgCameraAndControl::OnClickDlgCameraAndControlCloseVideo)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED, &CDlgCameraAndControl::OnCbnSelchangeComboSpeed)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgCameraAndControl 消息处理程序

BOOL CDlgCameraAndControl::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_resizeHelper.Init(m_hWnd);

	////告警详细信息
	//m_resizeHelper.Fix(IDC_STATIC_ALARM_INFO, DlgResizeHelper::kLeftRight, DlgResizeHelper::kTop);
	//m_resizeHelper.Fix(IDC_STATIC_CAMERA, DlgResizeHelper::kLeftRight, DlgResizeHelper::kTop);
	//m_resizeHelper.Fix("Button", DlgResizeHelper::kNoHFix, DlgResizeHelper::kHeight);
	//m_resizeHelper.Fix(IDC_STATIC_SPEED, DlgResizeHelper::kNoHFix, DlgResizeHelper::kHeightBottom);
	//m_resizeHelper.Fix(IDC_COMBO_SPEED, DlgResizeHelper::kNoHFix, DlgResizeHelper::kHeightBottom);

	InitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCameraAndControl::InitCtrl()
{
	m_pViewChannel = new CViewChannel();
	CRect Rect(400, 400, 400, 400);
	m_pViewChannel->Create(NULL, NULL, WS_TABSTOP|WS_CHILD|WS_VISIBLE, Rect, this, 1000);
	//初始化预置位编号
	CString strText = "";
	for (int i=5; i<15; i++)
	{
		strText.Format("%d",i);
		m_comboSpeed.AddString(strText);
	}
	m_comboSpeed.SetCurSel(0);

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
}

void CDlgCameraAndControl::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	/*m_resizeHelper.OnSize();*/
	int nHeight = cy;
	int nWidth = cx;

	int nVideoHeight = cy * 0.4;
	int nVideoWidth = cx;

	int nButtonHeight = 30;
	int nButtonWidth = cx/3;

	int nStaticAlarmInfoHeight = 30;
	int nStaticAlarmInfoWidth = cx;

	int nStaticSpeedHeight = 30;
	int nStaticSpeedWidht = cx/2;

	int nComboSpeedHeight = 30;
	int nComboSpeedWidht = cx/5;

	int nListHeight = 0.6 * cy - 30 *4;
	int nListWidth = cx;

	CRect static_rect;
	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_STATIC_ALARM_INFO);
	if (pWnd != NULL)
	{
		static_rect.top = 0;
		static_rect.bottom = nStaticAlarmInfoHeight;
		static_rect.left = 0;
		static_rect.right = nStaticAlarmInfoWidth;
		pWnd->MoveWindow(static_rect);
	}

	//CRect video_rect;
	//pWnd = GetDlgItem(IDC_STATIC_CAMERA);
	//if (pWnd != NULL)
	//{
	//	video_rect.top = static_rect.bottom;
	//	video_rect.bottom = video_rect.top + nVideoHeight;
	//	video_rect.left = 0;
	//	video_rect.right = video_rect.left + nVideoWidth;
	//	pWnd->MoveWindow(video_rect);
	//}

	CRect video_rect;
	video_rect.top = static_rect.bottom;
	video_rect.bottom = video_rect.top + nVideoHeight;
	video_rect.left = 0;
	video_rect.right = video_rect.left + nVideoWidth;
	if (m_pViewChannel != NULL)
	{
		m_pViewChannel->MoveWindow(video_rect);
	}
	

	CRect button_rect;
	pWnd = GetDlgItem(IDC_BUTTON_UP);
	if (pWnd != NULL)
	{
		button_rect.top = video_rect.bottom;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = 0 + nButtonWidth;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd ->MoveWindow(button_rect);
	}
	int nStaticTop = button_rect.top;

	pWnd = GetDlgItem(IDC_BUTTON_LEFT);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.bottom;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = 0;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd ->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_LOCK);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd ->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_RIGHT);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd ->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_ZOOM_IN);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.bottom;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = 0;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd ->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_DOWN);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd ->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_ZOOM_OUT);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd ->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_STATIC_SPEED);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.bottom;
		button_rect.bottom = button_rect.top + nStaticSpeedHeight;
		button_rect.left = 0;
		button_rect.right = button_rect.left + nStaticSpeedWidht;
		pWnd ->MoveWindow(button_rect);
	}
	int nStaticBottom = button_rect.bottom;

	pWnd = GetDlgItem(IDC_COMBO_SPEED);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nComboSpeedHeight + 100;
		button_rect.left = button_rect.right;
		button_rect.right = button_rect.left + nComboSpeedWidht;
		pWnd ->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_LIST_ALARM);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top + nComboSpeedHeight;
		button_rect.bottom = button_rect.top + nListHeight;
		button_rect.left = 0;
		button_rect.right = button_rect.left + nListWidth;
		pWnd ->MoveWindow(button_rect);
	}
	int nStaticLeft = button_rect.left;
	int nStaticRight = button_rect.right;

	pWnd = GetDlgItem(IDC_STATIC_KUANG);
	if (pWnd != NULL)
	{
		button_rect.top = nStaticTop;
		button_rect.bottom = nStaticBottom;
		button_rect.left = nStaticLeft;
		button_rect.right = nStaticRight;
		pWnd ->MoveWindow(button_rect);
	}
}

void CDlgCameraAndControl::OpenCamera(char *szCameraCode)
{
	if (m_bOpenFlag)
	{
		CloseCamera(m_nOpenCameraId);
		m_pViewChannel->Invalidate();
		Sleep(1000);
	}

	//获取Camera信息
	ASS_CAMERA_INFO tCameraInfo;
	GetCameraInfoByCameraCode(&tCameraInfo, szCameraCode);
	
	//获取ID
	m_nOpenCameraId = GetID_DevSdk();

	//获取dvr类型
	int nDvrType = GetDvrTypeByDvrText(tCameraInfo.tDvrInfo.szDvrType);
	if (nDvrType == -1)
	{
		MessageBox("未知的DVR类型");
		return;
	}

	//打开摄像头
	bool ret = AssOpenCamera(tCameraInfo.tDvrInfo.szDvrIp,
		tCameraInfo.tDvrInfo.nDvrPort,
		tCameraInfo.tDvrInfo.szDvrUserName,
		tCameraInfo.tDvrInfo.szDvrPassword,
		nDvrType,
		1,
		tCameraInfo.nChannel,
		m_nOpenCameraId,
		m_pViewChannel->GetSafeHwnd());

	//设置摄像头信息
	memcpy(&m_tAssCameraInfo, &tCameraInfo, sizeof(ASS_CAMERA_INFO));

	if (ret)
		SetCameraOpenStatus(true);
	else
		SetCameraOpenStatus(false);
}

//设置控件背景色
void CDlgCameraAndControl::SetControlBackgroundColor()
{
	CDC *pDC = m_staticCameraWnd.GetDC();
	CRect rct;
	m_staticCameraWnd.GetWindowRect(&rct);
	CBrush brs;
	brs.CreateSolidBrush(RGB(0,0,0));
	CRect picrct;
	picrct.top = 0;
	picrct.left = 0;
	picrct.bottom = rct.Height();
	picrct.right = rct.Width();
	pDC->FillRect(&picrct, &brs);
}

void CDlgCameraAndControl::CloseCamera(int nID)
{
	int nRet = 0;
	if (m_pViewChannel->m_video_opened)
	{
		nRet = AssCloseCamera(nID);
		m_pViewChannel->Invalidate();
	}
	 
	if (nRet)
	{
		//SetControlBackgroundColor();
		SetCameraOpenStatus(false);
	}
}

void CDlgCameraAndControl::SetCameraOpenStatus(bool bFlag)
{
	m_bOpenFlag = bFlag;
	m_pViewChannel->m_video_opened = bFlag;
}

void CDlgCameraAndControl::OnBnClickedButtonUp()
{
}

LRESULT CDlgCameraAndControl::OnStartYTControl(WPARAM wParam,LPARAM lParam)
{
	int ytcommand = 0;

	TRACE("-----iMove=%d,iCommandID=%d------\n\n",lParam,wParam);

	if (m_bOpenFlag == false)
	{
		return 0;
	}

	if(lParam == 0)
	{
		switch (wParam)
		{
		case 1://云台控制上-结束
			{
				DvrPtzControl_Devsdk(m_tAssCameraInfo.tDvrInfo.szDvrIp,
					m_tAssCameraInfo.tDvrInfo.nDvrPort,
					m_tAssCameraInfo.tDvrInfo.szDvrUserName,
					m_tAssCameraInfo.tDvrInfo.szDvrPassword,
					m_tAssCameraInfo.tDvrInfo.nDvrType,
					m_tAssCameraInfo.tDvrInfo.nDvrId,
					m_tAssCameraInfo.nChannel,TILTUPSTOP,m_nSpeed);
			}
			break;
		case 2://云台控制下-结束
			{
				DvrPtzControl_Devsdk(m_tAssCameraInfo.tDvrInfo.szDvrIp,
					m_tAssCameraInfo.tDvrInfo.nDvrPort,
					m_tAssCameraInfo.tDvrInfo.szDvrUserName,
					m_tAssCameraInfo.tDvrInfo.szDvrPassword,
					m_tAssCameraInfo.tDvrInfo.nDvrType,
					m_tAssCameraInfo.tDvrInfo.nDvrId,
					m_tAssCameraInfo.nChannel,TILTDOWNSTOP,m_nSpeed);
			}
			break;
		case 3://云台控制左-结束
			{
				DvrPtzControl_Devsdk(m_tAssCameraInfo.tDvrInfo.szDvrIp,
					m_tAssCameraInfo.tDvrInfo.nDvrPort,
					m_tAssCameraInfo.tDvrInfo.szDvrUserName,
					m_tAssCameraInfo.tDvrInfo.szDvrPassword,
					m_tAssCameraInfo.tDvrInfo.nDvrType,
					m_tAssCameraInfo.tDvrInfo.nDvrId,
					m_tAssCameraInfo.nChannel,PANLEFTSTOP,m_nSpeed);
			}
			break;
		case 4://云台控制右-结束
			{
				DvrPtzControl_Devsdk(
					m_tAssCameraInfo.tDvrInfo.szDvrIp,
					m_tAssCameraInfo.tDvrInfo.nDvrPort,
					m_tAssCameraInfo.tDvrInfo.szDvrUserName,
					m_tAssCameraInfo.tDvrInfo.szDvrPassword,
					m_tAssCameraInfo.tDvrInfo.nDvrType,
					m_tAssCameraInfo.tDvrInfo.nDvrId,
					m_tAssCameraInfo.nChannel,PANRIGHTSTOP,m_nSpeed);
			}
			break;
		case 5://拉近-结束
			{
				DvrPtzControl_Devsdk(
					m_tAssCameraInfo.tDvrInfo.szDvrIp,
					m_tAssCameraInfo.tDvrInfo.nDvrPort,
					m_tAssCameraInfo.tDvrInfo.szDvrUserName,
					m_tAssCameraInfo.tDvrInfo.szDvrPassword,
					m_tAssCameraInfo.tDvrInfo.nDvrType,
					m_tAssCameraInfo.tDvrInfo.nDvrId,
					m_tAssCameraInfo.nChannel,ZOOMOUTSTOP,m_nSpeed);
			}
			break;
		case 6://拉远-结束
			{
				DvrPtzControl_Devsdk(
					m_tAssCameraInfo.tDvrInfo.szDvrIp,
					m_tAssCameraInfo.tDvrInfo.nDvrPort,
					m_tAssCameraInfo.tDvrInfo.szDvrUserName,
					m_tAssCameraInfo.tDvrInfo.szDvrPassword,
					m_tAssCameraInfo.tDvrInfo.nDvrType,
					m_tAssCameraInfo.tDvrInfo.nDvrId,
					m_tAssCameraInfo.nChannel,ZOOMINSTOP,m_nSpeed);
			}
			break;
		case 7://云台控制焦距近-结束
			{

			}
			break;
		case 8://云台控制焦距远-结束
			{

			}
			break;
		case 9://云台控制光圈小-结束
			{

			}
			break;
		case 10://云台控制光圈大-结束
			{

			}
			break;
		case 11://红外开
			{

			}
			break;
		case 12://红外关
			{

			}
			break;
		case 13://加热开
			{

			}
			break;
		case 14:// 加热关
			{
				//ytcommand = 0x0c02;
			}
			break;
		case 15:// 灯光开
			{
				//ytcommand = 0x0b01;
			}
			break;
		case 16:// 灯光关
			{
				//ytcommand = 0x0b02;
			}
			break;
		case 17:// 雨刷开
			{
				//ytcommand = 0x0a01;
			}
			break;
		case 18:// 雨刷关
			{
				//ytcommand = 0x0a02;
			}
			break;
		case 19:
			{
				return 0;
			}
			break;
		}
	}
	else
	{
		switch (wParam)
		{
		case 1://云台控制上-开始
			{
				DvrPtzControl_Devsdk(
					m_tAssCameraInfo.tDvrInfo.szDvrIp,
					m_tAssCameraInfo.tDvrInfo.nDvrPort,
					m_tAssCameraInfo.tDvrInfo.szDvrUserName,
					m_tAssCameraInfo.tDvrInfo.szDvrPassword,
					m_tAssCameraInfo.tDvrInfo.nDvrType,
					m_tAssCameraInfo.tDvrInfo.nDvrId,
					m_tAssCameraInfo.nChannel,TILTUP,m_nSpeed);
			}
			break;
		case 2://云台控制下-开始
			{
				DvrPtzControl_Devsdk(
					m_tAssCameraInfo.tDvrInfo.szDvrIp,
					m_tAssCameraInfo.tDvrInfo.nDvrPort,
					m_tAssCameraInfo.tDvrInfo.szDvrUserName,
					m_tAssCameraInfo.tDvrInfo.szDvrPassword,
					m_tAssCameraInfo.tDvrInfo.nDvrType,
					m_tAssCameraInfo.tDvrInfo.nDvrId,
					m_tAssCameraInfo.nChannel,TILTDOWN,m_nSpeed);
			}
			break;
		case 3://云台控制左-开始
			{
				DvrPtzControl_Devsdk(
					m_tAssCameraInfo.tDvrInfo.szDvrIp,
					m_tAssCameraInfo.tDvrInfo.nDvrPort,
					m_tAssCameraInfo.tDvrInfo.szDvrUserName,
					m_tAssCameraInfo.tDvrInfo.szDvrPassword,
					m_tAssCameraInfo.tDvrInfo.nDvrType,
					m_tAssCameraInfo.tDvrInfo.nDvrId,
					m_tAssCameraInfo.nChannel,PANLEFT,m_nSpeed);
			}
			break;
		case 4://云台控制右-开始
			{
				DvrPtzControl_Devsdk(
					m_tAssCameraInfo.tDvrInfo.szDvrIp,
					m_tAssCameraInfo.tDvrInfo.nDvrPort,
					m_tAssCameraInfo.tDvrInfo.szDvrUserName,
					m_tAssCameraInfo.tDvrInfo.szDvrPassword,
					m_tAssCameraInfo.tDvrInfo.nDvrType,
					m_tAssCameraInfo.tDvrInfo.nDvrId,
					m_tAssCameraInfo.nChannel,PANRIGHT,m_nSpeed);
			}
			break;
		case 5://拉近-开始
			{
				DvrPtzControl_Devsdk(
					m_tAssCameraInfo.tDvrInfo.szDvrIp,
					m_tAssCameraInfo.tDvrInfo.nDvrPort,
					m_tAssCameraInfo.tDvrInfo.szDvrUserName,
					m_tAssCameraInfo.tDvrInfo.szDvrPassword,
					m_tAssCameraInfo.tDvrInfo.nDvrType,
					m_tAssCameraInfo.tDvrInfo.nDvrId,
					m_tAssCameraInfo.nChannel,ZOOMOUT,m_nSpeed);
			}
			break;
		case 6://拉远-开始
			{
				DvrPtzControl_Devsdk(
					m_tAssCameraInfo.tDvrInfo.szDvrIp,
					m_tAssCameraInfo.tDvrInfo.nDvrPort,
					m_tAssCameraInfo.tDvrInfo.szDvrUserName,
					m_tAssCameraInfo.tDvrInfo.szDvrPassword,
					m_tAssCameraInfo.tDvrInfo.nDvrType,
					m_tAssCameraInfo.tDvrInfo.nDvrId,
					m_tAssCameraInfo.nChannel,ZOOMIN,m_nSpeed);
			}
			break;
		case 7://云台控制焦距近-开始
			{
				//ytcommand = 0x0302;
			}
			break;
		case 8://云台控制焦距远-开始
			{
				//ytcommand = 0x0304;
			}
			break;
		case 9://云台控制光圈小-开始
			{
				//ytcommand = 0x0102;
			}
			break;
		case 10://云台控制光圈大-开始
			{
				//ytcommand = 0x0103;
			}
			break;
		case 11:// 红外开
			{
				//ytcommand = 0x0d01;
			}
			break;
		case 12:// 红外关
			{
				//ytcommand = 0x0d02;
			}
			break;
		case 13:// 加热开
			{
				//ytcommand = 0x0c01;
			}
			break;
		case 14:// 加热关
			{
				//ytcommand = 0x0c02;
			}
			break;
		case 15:// 灯光开
			{
				//ytcommand = 0x0b01;
			}
			break;
		case 16:// 灯光关
			{
				//ytcommand = 0x0b02;
			}
			break;
		case 17:// 雨刷开
			{
				//ytcommand = 0x0a01;

			}
			break;
		case 18:// 雨刷关
			{
				//ytcommand = 0x0a02;
			}
			break;
		case 19:
			{
				return 0;
			}
			break;
		}
	}

	return 1;
}

void CDlgCameraAndControl::ProcessRClickCamera()
{
	PopClickCameraRightMenu();
}

void CDlgCameraAndControl::PopClickCameraRightMenu()
{
	CMenu menuMain;
	menuMain.CreatePopupMenu();

	menuMain.AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,ID_DLG_CAMERA_AND_CONTROL_CLOSE_VIDEO,"关闭视频");
	
	CPoint point;
	GetCursorPos(&point);

	menuMain.TrackPopupMenu(TPM_RIGHTBUTTON,point.x,point.y,this);
}

void CDlgCameraAndControl::OnClickDlgCameraAndControlCloseVideo()
{
	CloseCamera(m_nOpenCameraId);
}

BOOL CDlgCameraAndControl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_RBUTTONDOWN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_STATIC_CAMERA)->GetSafeHwnd())
		{
			ProcessRClickCamera();
		}
	}
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		//上开始
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_UP)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,1,1);
		}

		//下
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_DOWN)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,2,1);
		}

		//左
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_LEFT)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,3,1);
		}

		//右
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_RIGHT)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,4,1);
		}

		//拉近
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOM_IN)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,5,1);
		}

		//拉远
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOM_OUT)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,6,1);
		}
	}

	if (pMsg->message == WM_LBUTTONUP)
	{
		//上停
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_UP)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,1,0);
		}

		//下
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_DOWN)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,2,0);
		}

		//左
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_LEFT)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,3,0);
		}

		//右
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_RIGHT)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,4,0);
		}

		//拉近
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOM_IN)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,5,0);
		}

		//拉远
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_ZOOM_OUT)->GetSafeHwnd())
		{
			this->PostMessage(OM_MESSAGE_CAMERA_CONTROL,6,0);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgCameraAndControl::OnCbnSelchangeComboSpeed()
{
	CString strSpeed;
	m_comboSpeed.GetLBText(m_comboSpeed.GetCurSel(), strSpeed);
	m_nSpeed = atoi(strSpeed.GetBuffer());
}

BOOL CDlgCameraAndControl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void CDlgCameraAndControl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	//CRect rect;
	//GetClientRect(&rect);
	//dc.FillSolidRect(rect,RGB(250,250,250));
	// 不为绘图消息调用 CDialog::OnPaint()
}

LRESULT CDlgCameraAndControl::GetAlarmInfoByStationIdHandle( WPARAM wParam,LPARAM lParam )
{
	int nStationId = wParam;

	//清空数据
	m_lstAlarm.DeleteAllItems();

	//拼出查询告警条件
	CString strCondition;
	strCondition = SpellCondition(nStationId);

	//从数据库中获取告警信息根据某个变电站ID
	std::vector<ACS_ALARM_INFO> vecAlarmInfo = GetAlarmInfoByStationId(strCondition);

	//将数据添加到列表中
	AddAlarmInfoToList(vecAlarmInfo);

	return NULL;
}

void CDlgCameraAndControl::AddAlarmInfoToList(std::vector<ACS_ALARM_INFO> vecAlarmInfo)
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

CString CDlgCameraAndControl::SpellCondition(int nStationId)
{
	CString strSubSystemTypeCondition = " 1=2 ";

	//取安防类型告警
	strSubSystemTypeCondition += " or f.f_parent_id=4 ";

	//取消防类型告警
	strSubSystemTypeCondition += " or f.f_parent_id=6 ";

	//取门禁类型告警
	strSubSystemTypeCondition += " or f.f_parent_id=2 ";

	CString strCondition = "";
	strCondition.Format(" c.station_id=%d and a.process_status=1 and (%s) ", nStationId, strSubSystemTypeCondition);

	return strCondition;
}
HBRUSH CDlgCameraAndControl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
