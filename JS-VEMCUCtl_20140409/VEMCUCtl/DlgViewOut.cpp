// DlgViewOut.cpp : 实现文件
//

#include "stdafx.h"
#include "xappany.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"
#include "DlgViewOut.h"
#include "DlgReplay.h"
#include "CameraVideoOperate.h"
#include "SipCall.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDlgViewOut 对话框
IMPLEMENT_DYNAMIC(CDlgViewOut, CDialog)

CDlgViewOut::CDlgViewOut(CWnd* pParent /*=NULL*/)
: CDialog(CDlgViewOut::IDD, pParent)
{
	m_brush.CreateSolidBrush(RGB(77, 188, 176)); 

	//窗口序号
	m_iWinID = -1;
	
	//录像
	m_bRecordFlag = FALSE;
	m_hSaveFile = INVALID_HANDLE_VALUE;
	memset(m_Headerbuf,0,sizeof(m_Headerbuf));
	m_nHeaderLen = 0;
	memset(m_szReplayLocalFileName,0,sizeof(m_szReplayLocalFileName));
	m_nReplayLocalFileTypeFlag = 0;
	m_nReplayLocalFileIDByDirectDevice = -1;
	m_nReplayLocalFileDvrTypeyDirectDevice = 0;

	//线开始、结束值
	m_nLine1_from_x = 0;
	m_nLine1_from_y = 0;
	m_nLine1_to_x = 0;
	m_nLine1_to_y = 0;
	m_nLine2_from_x = 0;
	m_nLine2_from_y = 0;
	m_nLine2_to_x = 0;
	m_nLine2_to_y = 0;
	
	m_CallidVoice = -1;
	m_bShowVoice = false;
	m_bShowCapture = false;
	m_bShowRecord = false;
	m_bSelectedFlag = FALSE;

	//云台锁定
	m_bYtLockFlag = FALSE;
	m_YtLockTime = time(NULL);

	m_byt_ctl = false;
	m_iyt_pos = 0;

	//组包、解码
	m_pFUA = NULL;
	m_pPlayChannelInfo = NULL;

	m_hCallResultNotifyEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	try
	{
		m_pPlayChannelInfo = new GW_PLAY_CHANNEL_INFO();
	}
	catch(...)
	{

	}
}

CDlgViewOut::~CDlgViewOut()
{
	if (m_pPlayChannelInfo != NULL)
	{
		delete m_pPlayChannelInfo;
		m_pPlayChannelInfo = NULL;
	}

	if (m_hCallResultNotifyEvent != NULL)
	{
		CloseHandle(m_hCallResultNotifyEvent);
		m_hCallResultNotifyEvent = NULL;
	}
}

void CDlgViewOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_STATION, m_strStation);
	DDX_Control(pDX, IDC_STATIC_RELAINFO, m_strRelaInfo);
	DDX_Control(pDX, IDC_STATIC_VIDEO_FLOW, m_strVideoFlow);
	DDX_Control(pDX, IDC_STATIC_WEATHER, m_strWeather);
	DDX_Control(pDX, IDC_BUTTON_OPENVOICE_VO, m_btnVoice);
	DDX_Control(pDX, IDC_BUTTON_CAPTURE_VO, m_btnCapture);
	DDX_Control(pDX, IDC_BUTTON_RECORD_VO, m_btnRecord);
	DDX_Control(pDX, IDC_BTN_TYPE_FLAG, m_btnTypeFlag);
	DDX_Control(pDX, IDC_AXISMEDIACONTROL1, m_AxisView);
}


BEGIN_MESSAGE_MAP(CDlgViewOut, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_BUTTON_OPENVOICE_VO, &CDlgViewOut::OnBnClickedButtonOpenvoice)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE_VO, &CDlgViewOut::OnBnClickedButtonCaptureVo)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_VO, &CDlgViewOut::OnBnClickedButtonRecordVo)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgViewOut 消息处理程序
BOOL CDlgViewOut::OnInitDialog()
{
	CDialog::OnInitDialog();
	//////////////////////////////////////////////////////////////////////////
	InitControl();
	InitInfo();
	LoadSkin();

	try
	{
		m_AxisView.put_StretchToFit(TRUE);
		m_AxisView.put_MaintainAspectRatio(TRUE);
		m_AxisView.put_ShowStatusBar(FALSE);
		m_AxisView.put_BackgroundColor(RGB(0,0,0)); 
		m_AxisView.put_VideoRenderer(AMC_VIDEO_RENDERER_VMR9);
		m_AxisView.put_EnableOverlays(FALSE);
		m_AxisView.put_EnableContextMenu(FALSE); 

		m_AxisView.put_Popups(AMC_POPUPS_LOGIN_DIALOG|AMC_POPUPS_NO_VIDEO);

		return TRUE;
	}
	catch (...)
	{

	}	
	return FALSE; 
}

BOOL CDlgViewOut::PreTranslateMessage(MSG* pMsg)
{
	m_TPtool.RelayEvent(pMsg);

	//////////////////////////////////////////////////////////////////////////
	if(pMsg->message==0x100   &&   pMsg->wParam==27)//屏住ESC
		return	TRUE;
	if(pMsg->message==0x100   &&   pMsg->wParam==13)//屏住ENTER   
		return	TRUE; 
	if(pMsg->message==0x100   &&   pMsg->wParam==VK_SPACE)
		return	TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgViewOut::OnEraseBkgnd(CDC* pDC)
{
	//////////////////////////////////////////////////////////////////////////
	CRect rect,recttop;
	GetClientRect (&rect);

	if (m_bSelectedFlag)
	{
		DrawRect(pDC,rect);
		recttop.top = rect.top+1;
		recttop.left = rect.left+1;
		recttop.right = rect.right-1;
		recttop.bottom = rect.top+34;
	}
	else
	{
		recttop.top = rect.top;
		recttop.left = rect.left;
		recttop.right = rect.right;
		recttop.bottom = rect.top+34;
	}

	pDC->FillRect (recttop, &m_brush);

	CDC   dcMem;
	CBitmap*   pOldBitmap;

	dcMem.CreateCompatibleDC(pDC);

	// 上部
	pOldBitmap = dcMem.SelectObject(&m_TopBmp);   

	pDC->StretchBlt(rect.left,rect.top,
		rect.right-rect.left,m_TopBm.bmHeight,
		&dcMem,
		0,0,
		m_TopBm.bmWidth,m_TopBm.bmHeight,
		SRCCOPY);

	dcMem.SelectObject(pOldBitmap);

	return true;
}

void CDlgViewOut::DrawRect(CDC *pDC,RECT &rect)
{
	if (pDC == NULL)
		return;

	CPen Pen(PS_SOLID, 1, RGB(255,0,0));
	CPen *pOldPen = pDC->SelectObject(&Pen);

	pDC->MoveTo(rect.left,rect.top);
	pDC->LineTo(rect.right-1,rect.top);
	pDC->LineTo(rect.right-1,rect.bottom-1);
	pDC->LineTo(rect.left,rect.bottom-1);
	pDC->LineTo(rect.left,rect.top);
	pDC->SelectObject(pOldPen);

}

void CDlgViewOut::ModifyControlSizeBySelectFlag(BOOL bSelectedFlag,BOOL bForceFlag)
{
	if ((m_bSelectedFlag == bSelectedFlag)&&(bForceFlag == FALSE))
		return;

	RECT rc,rc1;
	CWnd *pWnd;

	GetClientRect(&rc);

	rc1.top = rc.top;
	rc1.left = rc.left;
	rc1.bottom = rc.bottom;
	rc1.right = rc.right;
	
	m_bSelectedFlag = bSelectedFlag;
	InvalidateRect(&rc1);

	pWnd = GetDlgItem(IDC_STATIC_STATION);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;

		if(m_bSelectedFlag != FALSE)
		{
			rect.left=rc.left+1;
			rect.right=rc.right-1;
			rect.top=rc.top+34;
			rect.bottom=rc.bottom-1;
		}
		else
		{
			rect.left=rc.left;
			rect.right=rc.right;
			rect.top=rc.top+34;
			rect.bottom=rc.bottom;
		}

		m_Pic.MoveWindow(rect);
		m_AxisView.MoveWindow(rect);
		//m_Pic.ShowWindow(SW_SHOW);
	}

	pWnd = GetDlgItem(IDC_STATIC_STATION);	//区域名称
	if (pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;
		
		if(m_bSelectedFlag != FALSE)
		{
			rect.left = rc.left+1;
			rect.right = rc.right-20;
			rect.top = rc.top+1;
			rect.bottom = rc.top + 16;
		}
		else
		{
			rect.left = rc.left;
			rect.right = rc.right-20;
			rect.top = rc.top;
			rect.bottom = rc.top + 16;
		}

		pWnd->MoveWindow(rect);
	}

	pWnd = GetDlgItem(IDC_STATIC_RELAINFO);	//区域名称
	if (pWnd)
	{
		CRect rect;

		if(m_bSelectedFlag != FALSE)
		{
			rect.left = rc.left+1;
			rect.right = rc.right-2;
			rect.top = rc.top+17;
			rect.bottom = rc.top + 34;
		}
		else
		{
			rect.left = rc.left;
			rect.right = rc.right-2;
			rect.top = rc.top+17;
			rect.bottom = rc.top + 34;
		}

		pWnd->MoveWindow(rect);
	}

	pWnd = GetDlgItem(IDC_STATIC_WEATHER);	//气象信息
	if (pWnd)
	{
		CRect rect;

		if(m_bSelectedFlag != FALSE)
		{
			rect.left = rc.left+1;
			rect.right = rc.right-2;
			rect.top = rc.top+17;
			rect.bottom = rc.top+34;
		}
		else
		{
			rect.left = rc.left;
			rect.right = rc.right-2;
			rect.top = rc.top+17;
			rect.bottom = rc.top+34;
		}

		pWnd->MoveWindow(rect);
	}

	// 抓图
	pWnd = GetDlgItem(IDC_BUTTON_CAPTURE_VO);
	if (pWnd)
	{
		CRect rect;
		if(m_bSelectedFlag != FALSE)
		{
			rect.left = rc.right-21;
			rect.right = rc.right-1;
			rect.top = rc.top+1;
			rect.bottom = rc.top + 21;
		}
		else
		{
			rect.left = rc.right-20;
			rect.right = rc.right;
			rect.top = rc.top;
			rect.bottom = rc.top + 20;
		}

		pWnd->MoveWindow(rect);
	}

	// 录像
	pWnd = GetDlgItem(IDC_BUTTON_RECORD_VO);
	if (pWnd)
	{
		CRect rect;
		if(m_bSelectedFlag != FALSE)
		{
			rect.left = rc.right-41;
			rect.right = rc.right-21;
			rect.top = rc.top+1;
			rect.bottom = rc.top + 21;
		}
		else
		{
			rect.left = rc.right-40;
			rect.right = rc.right-20;
			rect.top = rc.top;
			rect.bottom = rc.top + 20;
		}
		pWnd->MoveWindow(rect);
	}

	// 语音
	pWnd = GetDlgItem(IDC_BUTTON_OPENVOICE_VO);
	if (pWnd)
	{
		CRect rect;
		if(m_bSelectedFlag != FALSE)
		{
			rect.left = rc.right-61;
			rect.right = rc.right-41;
			rect.top = rc.top+1;
			rect.bottom = rc.top + 21;
		}
		else
		{
			rect.left = rc.right-60;
			rect.right = rc.right-40;
			rect.top = rc.top;
			rect.bottom = rc.top + 20;
		}
		pWnd->MoveWindow(rect);
	}

	//流量
	pWnd = GetDlgItem(IDC_STATIC_VIDEO_FLOW);
	if (pWnd)
	{
		CRect rect;
		if(m_bSelectedFlag != FALSE)
		{
			rect.left = rc.right-77;
			rect.right = rc.right-27;
			rect.top = rc.top+1;
			rect.bottom = rc.top + 21;
		}
		else
		{
			rect.left = rc.right-76;
			rect.right = rc.right-26;
			rect.top = rc.top;
			rect.bottom = rc.top + 20;
		}

		pWnd->MoveWindow(rect);
	}

	// 标志
	pWnd = GetDlgItem(IDC_BTN_TYPE_FLAG);
	if (pWnd)
	{
		CRect rect;
		if(m_bSelectedFlag != FALSE)
		{
			rect.left = rc.right-21;
			rect.right = rc.right-1;
			rect.top = rc.top+1;
			rect.bottom = rc.top + 21;
		}
		else
		{
			rect.left = rc.right-20;
			rect.right = rc.right;
			rect.top = rc.top;
			rect.bottom = rc.top + 20;
		}
		pWnd->MoveWindow(rect);
	}
}

void CDlgViewOut::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//////////////////////////////////////////////////////////////////////////
	ModifyControlSizeBySelectFlag(m_bSelectedFlag,TRUE);	
}

void CDlgViewOut::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (::GetCapture() == m_hWnd)
	{
		::ReleaseCapture();
	}

	if (m_byt_ctl == true && CheckViewVideoIsOpen(m_iWinID) > 0)
	{
		switch (m_iyt_pos)
		{
		case 1:
			m_iyt_pos = 0;
			m_byt_ctl = false;
			::SetCursor(m_hCursor_up);
			g_pMainDlg->PostMessage(OM_YTCONTROL, 1, 0);
			break;
		case 2:
			m_iyt_pos = 0;
			m_byt_ctl = false;
			::SetCursor(m_hCursor_down);
			g_pMainDlg->PostMessage(OM_YTCONTROL, 2, 0);
			break;
		case 3:
			m_iyt_pos = 0;
			m_byt_ctl = false;
			::SetCursor(m_hCursor_left);
			g_pMainDlg->PostMessage(OM_YTCONTROL, 3, 0);
			break;
		case 4:
			m_iyt_pos = 0;
			m_byt_ctl = false;
			::SetCursor(m_hCursor_right);
			g_pMainDlg->PostMessage(OM_YTCONTROL, 4, 0);
			break;
		default:
			break;
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgViewOut::OnLButtonDown(UINT nFlags, CPoint point)
{
	g_pMainDlg->SendMessage(OM_CHANNLECHANGE, m_iWinID * 0x1000 + 0);	//m_iYTID);
	
	Sleep(50);

	::SetCapture(m_hWnd);

	if (CheckViewVideoIsOpen(m_iWinID) > 0)
	{
		CRect	rc;
		CPoint pt;
		int x0,x1,y0,y1;
		int xp,yp;
		int posA,posB;

		GetWindowRect(&rc);
		GetCursorPos(&pt);
		y0 = 16;
		y1 = rc.bottom - rc.top - 16;
		x0 = 0;
		x1 = rc.right - rc.left;
		xp = pt.x - rc.left;
		yp = pt.y - rc.top;
		if (yp <= 16)
			return;
		
		posA = yp - (y1 - xp*y1/x1);
		posB = yp - xp*y1/x1;

		if (posA < 0 && posB < 0 && yp < y1/4)//up
		{
			m_byt_ctl = true;
			m_iyt_pos = 1;
			::SetCursor(m_hCursor_up_on);
			g_pMainDlg->PostMessage(OM_YTCONTROL, 1, 1);
			return;
		}

		if (posA > 0 && posB > 0 && yp > 3*y1/4)//down
		{
			m_byt_ctl = true;
			m_iyt_pos = 2;
			::SetCursor(m_hCursor_down_on);
			g_pMainDlg->PostMessage(OM_YTCONTROL, 2, 1);
			return;
		}

		if (posA < 0 && posB > 0 && xp < x1/4)//left
		{
			m_byt_ctl = true;
			m_iyt_pos = 3;
			::SetCursor(m_hCursor_left_on);
			g_pMainDlg->PostMessage(OM_YTCONTROL, 3, 1);
			return;
		}

		if (posA > 0 && posB < 0 && xp > 3*x1/4)//right
		{
			m_byt_ctl = true;
			m_iyt_pos = 4;
			::SetCursor(m_hCursor_right_on);
			g_pMainDlg->PostMessage(OM_YTCONTROL, 4, 1);
			return;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgViewOut::OnRButtonUp(UINT nFlags, CPoint point)
{
	g_pMainDlg->SendMessage(OM_RIGHTCLICKCHANNEL, m_iWinID);

	CDialog::OnRButtonUp(nFlags, point);
}

void CDlgViewOut::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	g_pMainDlg->SendMessage(OM_DBLCHANNEL, m_iWinID);

	CDialog::OnLButtonDblClk(nFlags, point);
}

int CDlgViewOut::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	//////////////////////////////////////////////////////////////////////////
	m_hCursor_up = AfxGetApp()->LoadCursor(IDC_CURSOR_YTUP);
	m_hCursor_down = AfxGetApp()->LoadCursor(IDC_CURSOR_YTDOWN);
	m_hCursor_left = AfxGetApp()->LoadCursor(IDC_CURSOR_YTLEFT);
	m_hCursor_right = AfxGetApp()->LoadCursor(IDC_CURSOR_YTRIGHT);
	m_hCursor_up_on = AfxGetApp()->LoadCursor(IDC_CURSOR_YTUP_ON);
	m_hCursor_down_on = AfxGetApp()->LoadCursor(IDC_CURSOR_YTDOWN_ON);
	m_hCursor_left_on = AfxGetApp()->LoadCursor(IDC_CURSOR_YTLEFT_ON);
	m_hCursor_right_on = AfxGetApp()->LoadCursor(IDC_CURSOR_YTRIGHT_ON);

	m_iWinID = -1;
	m_byt_ctl = false;
	m_iyt_pos = 0;

	return 0;
}

BOOL CDlgViewOut::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_byt_ctl == false)
	{
		if (CheckViewVideoIsOpen(m_iWinID) > 0)
		{
			CRect	rc;
			CPoint pt;
			int x0,x1,y0,y1;
			int xp,yp;
			int posA,posB;
			GetWindowRect(&rc);
			GetCursorPos(&pt);
			y0 = 16;
			y1 = rc.bottom - rc.top - 16;
			x0 = 0;
			x1 = rc.right - rc.left;
			xp = pt.x - rc.left;
			yp = pt.y - rc.top;
			if (yp <= 16)
				return true;
			posA = yp - (y1 - xp*y1/x1);
			posB = yp - xp*y1/x1;

			if (posA < 0 && posB < 0 && yp < y1/4)//up
			{
				::SetCursor(m_hCursor_up);
				return true;
			}

			if (posA > 0 && posB > 0 && yp > 3*y1/4)//down
			{
				::SetCursor(m_hCursor_down);
				return true;
			}

			if (posA < 0 && posB > 0 && xp < x1/4)//left
			{
				::SetCursor(m_hCursor_left);
				return true;
			}

			if (posA > 0 && posB < 0 && xp > 3*x1/4)//right
			{
				::SetCursor(m_hCursor_right);
				return true;
			}
		}

		return CDialog::OnSetCursor(pWnd, nHitTest, message);
	}
	else
	{
		return true;
	}
}

void CDlgViewOut::FlashChannel()
{
	try
	{
		m_strStation.SetTransparent(TRUE);					//设置背景透明色
		m_strStation.SetTextColor(RGB(23, 92, 85));		//设置字体颜色
		m_strStation.SetText(" ");
		m_strRelaInfo.SetTransparent(TRUE);				//设置背景透明色
		m_strRelaInfo.SetTextColor(RGB(23, 92, 85));	//设置字体颜色
		m_strRelaInfo.SetText(" ");
		m_strVideoFlow.SetText(" ");
		m_strWeather.SetText(" ");
		m_btnTypeFlag.ShowWindow(SW_HIDE);
		m_Pic.m_video_opened = false;
		m_Pic.FlashChannel();
	}
	catch(...)
	{

	}
}

BOOL  CDlgViewOut::InitControl()
{
	m_strStation.SetTransparent(TRUE);						//设置背景透明色
	m_strStation.SetTextColor(RGB(23, 92, 85));		    //设置字体颜色
	m_strStation.SetText(" ");
	m_strRelaInfo.SetTransparent(TRUE);				    //设置背景透明色
	m_strRelaInfo.SetTextColor(RGB(23, 92, 85));		//设置字体颜色
	m_strRelaInfo.SetText("无相关信息");
	m_strVideoFlow.SetTransparent(TRUE);				    //设置背景透明色
	m_strVideoFlow.SetTextColor(RGB(23, 92, 85));		//设置字体颜色
	m_strVideoFlow.SetText("0k/s");

	CRect Rect(400, 400, 400, 400);
	m_Pic.Create(NULL, NULL, WS_TABSTOP|WS_CHILD|WS_VISIBLE, Rect, this, 1000);

	GetDlgItem(IDC_STATIC_RELAINFO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_OPENVOICE_VO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_CAPTURE_VO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_RECORD_VO)->ShowWindow(SW_HIDE);

	m_btnVoice.LoadBitmap(IDB_BITMAP_VIEWOUT_VOICE, 0);
	m_btnVoice.LoadBitmap(IDB_BITMAP_VIEWOUT_VOICE_OPENED, 1);
	m_btnVoice.SetIndex(0);
	m_btnVoice.Invalidate(true);

	m_btnRecord.LoadBitmap(IDB_BITMAP_VIEWOUT_RECORD, 0);
	m_btnCapture.LoadBitmap(IDB_BITMAP_VIEWOUT_CAPTURE, 0);

	m_btnTypeFlag.LoadBitmap(IDB_BITMAP_VIEWOUT_FLAG_NORMAL,0);
	m_btnTypeFlag.LoadBitmap(IDB_BITMAP_VIEWOUT_FLAG_ALARM_LINKAGE,1);
	m_btnTypeFlag.LoadBitmap(IDB_BITMAP_VIEWOUT_FLAG_MANUAL_LINKAGE,2);
	m_btnTypeFlag.SetIndex(0);
	m_btnTypeFlag.Invalidate(TRUE);

	m_TPtool.Create(this);
	m_TPtool.Activate(TRUE);   
	m_TPtool.SetTipBkColor(RGB(255,255,255));//背景颜色   
	m_TPtool.SetTipTextColor(RGB(100,100,100));//字体颜色   
	m_TPtool.SetDelayTime(100);   //显示时间   
	m_TPtool.AddTool(GetDlgItem(IDC_BUTTON_OPENVOICE_VO),"语音对讲");
	m_TPtool.AddTool(GetDlgItem(IDC_BUTTON_RECORD_VO),"录像");
	m_TPtool.AddTool(GetDlgItem(IDC_BUTTON_CAPTURE_VO),"抓图");

	return TRUE;
}

BOOL  CDlgViewOut::InitInfo()
{
	if(m_pFUA == NULL)
	{
		m_pFUA = new CRtUtilH264ReFUA(512*1024);
	}

	m_pPlayChannelInfo->Lock();
	m_pPlayChannelInfo->m_hPlayWnd = m_Pic.GetSafeHwnd();
	m_pPlayChannelInfo->Unlock();

	return TRUE;
}

BOOL  CDlgViewOut::LoadSkin()
{
	m_TopBmp.LoadBitmap(IDB_BITMAP_VIEW_OUT_TOP_BK);
	m_TopBmp.GetObject(sizeof(BITMAP),(LPVOID)&m_TopBm);

	return TRUE;
}

//设置线开始值、结束值
BOOL CDlgViewOut::SetLineInfo(int line1_from_x,int line1_from_y,int line1_to_x,int line1_to_y,int line2_from_x,int line2_from_y,int line2_to_x,int line2_to_y)
{
	if (line1_from_x < 0||line1_from_x > 10000||line1_from_y < 0||line1_from_y > 10000)
		return FALSE;

	if (line1_to_x < 0||line1_to_x > 10000||line1_to_y < 0||line1_to_y > 10000)
		return FALSE;

	if (line2_from_x < 0||line2_from_x > 10000||line2_from_y < 0||line2_from_y > 10000)
		return FALSE;

	if (line2_to_x < 0||line2_to_x > 10000||line2_to_y < 0||line2_to_y > 10000)
		return FALSE;


	m_nLine1_from_x = line1_from_x;
	m_nLine1_from_y = line1_from_y;
	m_nLine1_to_x = line1_to_x;
	m_nLine1_to_y = line1_to_y;

	m_nLine2_from_x = line2_from_x;
	m_nLine2_from_y = line2_from_y;
	m_nLine2_to_x = line2_to_x;
	m_nLine2_to_y = line2_to_y;

	return TRUE;
}

BOOL CDlgViewOut::StartYtLock()
{
	m_bYtLockFlag = TRUE;
	m_YtLockTime = time(NULL);
	return TRUE;
}

BOOL CDlgViewOut::StopYtLock()
{
	m_bYtLockFlag = FALSE;
	m_YtLockTime = time(NULL);
	return TRUE;
}

BOOL CDlgViewOut::IsYtLock()
{
	if (m_bYtLockFlag == TRUE&&time(NULL) < m_YtLockTime + 30)
		return TRUE;
	return FALSE;
}

BOOL CDlgViewOut::RecvVideoFlow(int nVideoFlow)
{
	DWORD dwVideoFlowTime = GetTickCount();

	m_VideoViewOutInfo.Lock();

	if (dwVideoFlowTime - m_VideoViewOutInfo.m_dwVideoFlowTime <= 1000)
	{
		m_VideoViewOutInfo.m_nRealVideoFlow += nVideoFlow;
	}
	else if (dwVideoFlowTime - m_VideoViewOutInfo.m_dwVideoFlowTime > 1000 && dwVideoFlowTime - m_VideoViewOutInfo.m_dwVideoFlowTime <= 2*1000)
	{
		m_VideoViewOutInfo.m_nVideoFlow = m_VideoViewOutInfo.m_nRealVideoFlow;
		m_VideoViewOutInfo.m_nRealVideoFlow = nVideoFlow;
		m_VideoViewOutInfo.m_dwVideoFlowTime = dwVideoFlowTime;
	}
	else if (dwVideoFlowTime - m_VideoViewOutInfo.m_dwVideoFlowTime > 2*1000)
	{
		m_VideoViewOutInfo.m_nVideoFlow = 0;
		m_VideoViewOutInfo.m_nRealVideoFlow = nVideoFlow;
		m_VideoViewOutInfo.m_dwVideoFlowTime = dwVideoFlowTime;
	}

	m_VideoViewOutInfo.UnLock();

	return TRUE;
}

BOOL CDlgViewOut::ShowCurrentVideoFlow()
{
	try
	{
		CWnd *pWnd = GetDlgItem(IDC_STATIC_VIDEO_FLOW);
		if (pWnd != NULL)
			pWnd->ShowWindow(SW_SHOW);

		m_VideoViewOutInfo.Lock();
		m_VideoViewOutInfo.m_nVideoFlow = 0;
		m_VideoViewOutInfo.m_nRealVideoFlow = 0;
		m_VideoViewOutInfo.m_dwVideoFlowTime = GetTickCount();
		m_VideoViewOutInfo.UnLock();

		return TRUE;
	}
	catch(...)
	{

	}
	return FALSE;
}

BOOL CDlgViewOut::HideCurrentVideoFlow()
{
	try
	{
		CWnd *pWnd = GetDlgItem(IDC_STATIC_VIDEO_FLOW);
		if (pWnd != NULL)
			pWnd->ShowWindow(SW_HIDE);

		return TRUE;
	}
	catch(...)
	{

	}
	return FALSE;
}

BOOL CDlgViewOut::ShowCurrentVideoFlowInfo()
{
	try
	{
		int nCallID = -1;

		m_VideoViewOutInfo.Lock();

		if (m_VideoViewOutInfo.m_nCallid < 0)
		{
			
			CWnd *pWnd = GetDlgItem(IDC_STATIC_VIDEO_FLOW);
			if (pWnd)
				pWnd->ShowWindow(SW_HIDE);

			m_VideoViewOutInfo.UnLock();

			return FALSE;
		}

		DWORD dwVideoFlowTime = GetTickCount();

		char buffer[256] = {0};
		memset(buffer,0,sizeof(buffer));

		char szCameraCallNum[64] = {0};
		memset(szCameraCallNum,0,sizeof(szCameraCallNum));

		int nStreamless = 0;

		float fVideoFlow = 0.0f;

		if (dwVideoFlowTime - m_VideoViewOutInfo.m_dwVideoFlowTime <= 1000)
		{
			if (m_VideoViewOutInfo.m_nVideoFlow < 0)
			{
				sprintf(buffer,"0/s");
				m_VideoViewOutInfo.m_nVideoFlow = 0;
				m_VideoViewOutInfo.m_nVideoFlowCount++;
			}
			else if (m_VideoViewOutInfo.m_nVideoFlow == 0)
			{
				sprintf(buffer,"0/s");
				m_VideoViewOutInfo.m_nVideoFlowCount++;
			}
			else if (m_VideoViewOutInfo.m_nVideoFlow < 1024)
			{
				sprintf(buffer,"%d/s",m_VideoViewOutInfo.m_nVideoFlow);
				m_VideoViewOutInfo.m_nVideoFlowCount = 0;
			}
			else if (m_VideoViewOutInfo.m_nVideoFlow < 1024*1024)
			{
				fVideoFlow = (float)m_VideoViewOutInfo.m_nVideoFlow/1024;
				sprintf(buffer,"%.2fK/s",fVideoFlow);
				m_VideoViewOutInfo.m_nVideoFlowCount = 0;
			}
			else if(m_VideoViewOutInfo.m_nVideoFlow >= 1024*1024)
			{
				fVideoFlow = (float)m_VideoViewOutInfo.m_nVideoFlow/(1024*1024);
				sprintf(buffer,"%.2fM/s",fVideoFlow);
				m_VideoViewOutInfo.m_nVideoFlowCount = 0;
			}
		}
		else if (dwVideoFlowTime - m_VideoViewOutInfo.m_dwVideoFlowTime <= 2*1000)
		{
			if (m_VideoViewOutInfo.m_nRealVideoFlow < 0)
			{
				sprintf(buffer,"0/s");
				m_VideoViewOutInfo.m_nRealVideoFlow = 0;
				m_VideoViewOutInfo.m_nVideoFlowCount++;
			}
			else if (m_VideoViewOutInfo.m_nRealVideoFlow == 0)
			{
				sprintf(buffer,"0/s");
				m_VideoViewOutInfo.m_nVideoFlowCount++;
			}
			else if (m_VideoViewOutInfo.m_nRealVideoFlow < 1024)
			{
				sprintf(buffer,"%d/s",m_VideoViewOutInfo.m_nRealVideoFlow);
				m_VideoViewOutInfo.m_nVideoFlowCount = 0;
			}
			else if (m_VideoViewOutInfo.m_nRealVideoFlow < 1024*1024)
			{
				fVideoFlow = (float)m_VideoViewOutInfo.m_nRealVideoFlow/1024;
				sprintf(buffer,"%.2fK/s",fVideoFlow);
				m_VideoViewOutInfo.m_nVideoFlowCount = 0;
			}
			else if (m_VideoViewOutInfo.m_nRealVideoFlow >= 1024*1024)
			{
				fVideoFlow = (float)m_VideoViewOutInfo.m_nRealVideoFlow/(1024*1024);
				sprintf(buffer,"%.2fM/s",fVideoFlow);
				m_VideoViewOutInfo.m_nVideoFlowCount = 0;
			}
		}
		else
		{
			m_VideoViewOutInfo.m_nVideoFlow = 0;
			sprintf(buffer,"0K/s");
			m_VideoViewOutInfo.m_nVideoFlowCount++;
		}

		m_strVideoFlow.SetText(buffer);

		if (m_VideoViewOutInfo.m_nVideoFlowCount > 3)
		{
			m_VideoViewOutInfo.m_bVideoFlowProblemFlag = TRUE;
			m_VideoViewOutInfo.m_nVideoFlowCount = 0;

			CWnd *pWnd = GetDlgItem(IDC_STATIC_VIDEO_FLOW);
			if (pWnd != NULL)
				pWnd->ShowWindow(SW_HIDE);

			nCallID = m_VideoViewOutInfo.m_nCallid;
		}

		if (m_VideoViewOutInfo.m_nVideoFlow > 5*1024)
		{
			if (m_VideoViewOutInfo.m_nDeviceVideoFlag == 0)
			{
				m_VideoViewOutInfo.m_nDeviceVideoFlag = 1;

				strcpy_s(szCameraCallNum,sizeof(szCameraCallNum),m_VideoViewOutInfo.m_szCameraCallNum);
				nStreamless = m_VideoViewOutInfo.m_nCameraStreamless;

				m_VideoViewOutInfo.UnLock();

				if(g_pMainDlg != NULL&&g_pMainDlg->m_pDlgPageServer != NULL&&strlen(szCameraCallNum) > 10)
				{
					g_pMainDlg->m_pDlgPageServer->AutoSetTreeCameraHandleTreeItemAndDatabaseStatus(VM_CAMERA_STATUS_ONLINE_BY_STATUS,szCameraCallNum,NULL,NULL,FALSE);
				}
				return TRUE;
			}
		}

		m_VideoViewOutInfo.UnLock();

		if (nCallID >= 0&&g_pMainDlg != NULL)
		{
			g_pMainDlg->ThreadPoolDispatchTask(ThreadStopPlatformVideoByCallId,(void *)&nCallID,sizeof(nCallID),4);
		}

		return TRUE;
	}
	catch(...)
	{

	}
	return FALSE;
}

// 初始化播放通道
int CDlgViewOut::InitViewChannel()
{
	return 0;
}

void CDlgViewOut::ClearViewChannelInfo()
{
	g_VMLog.WriteVmLog("ClearViewChannelInfo---m_iWinID=%d",m_iWinID);

	m_VideoViewOutInfo.Copy(m_PreVideoViewOutInfo);
	m_VideoViewOutInfo.Clear2();

	m_Pic.m_nViewChannelInfoShowFlag = 0;
	
	return;
}

// 关闭播放通道
void CDlgViewOut::CloseViewChannel()
{
	g_VMLog.WriteVmLog("CloseViewChannel---m_iWinID=%d",m_iWinID);

	m_VideoViewOutInfo.Copy(m_PreVideoViewOutInfo);
	m_VideoViewOutInfo.Clear2();
	
	m_Pic.m_nViewChannelInfoShowFlag = 0;
	FlashChannel();

	m_Pic.ShowWindow(SW_SHOW);
	m_AxisView.Stop();
	m_AxisView.ShowWindow(SW_HIDE);

	return;
}

// 设置默认视频参数
bool CDlgViewOut::SetDefaultViewParameter()
{
	if (m_pPlayChannelInfo == NULL)
		return false;

	bool bResult = false;

	m_pPlayChannelInfo->Lock();
	bResult = m_pPlayChannelInfo->SetVideoParams(128,128,128,128,true);
	m_pPlayChannelInfo->Unlock();

	return bResult;
}

// 设置当前视频参数
bool CDlgViewOut::SetCurrentViewParameter(int liangdu, int duibidu, int baohedu, int sedu)
{
	if (m_pPlayChannelInfo == NULL)
		return false;

	bool bResult = false;

	m_pPlayChannelInfo->Lock();
	bResult = m_pPlayChannelInfo->SetVideoParams(liangdu,duibidu,baohedu,sedu,false);
	m_pPlayChannelInfo->Unlock();

	return bResult;
}

// 打开语音
void CDlgViewOut::OnBnClickedButtonOpenvoice()
{

}

void CDlgViewOut::OnBnClickedButtonCaptureVo()
{
	BOOL bDirectDeviceFlag = FALSE;
	BOOL bRet = FALSE;

	int nResult = CheckViewVideoIsOpen(m_iWinID);
	if(nResult <= 0)
	{
		MessageBox("该通道没有打开的视频","视频监视");
		return;
	}
	else if (nResult == 2)
	{
		bDirectDeviceFlag = TRUE;
	}

	CString  strFilePath;
	char	     szFilePath[256] = {0};
	CFile	     clsFile;

	CTime nowtime = CTime::GetCurrentTime();

	CreateDirectory("C:\\capture",NULL);

	if (g_pMainDlg == NULL||g_pMainDlg->m_pDlgPicFilePath == NULL||g_pMainDlg->m_pDlgPicFilePath->DoModal() == IDCANCEL)
		return;

	strcpy_s(g_PictureSaveFilepath,sizeof(g_PictureSaveFilepath),g_pMainDlg->m_pDlgPicFilePath->m_FilePath);
	
	//写文件
	if (bDirectDeviceFlag == FALSE)
	{
		strFilePath.Format("%s\\%04d-%02d-%02d-%02d-%02d-%02d.jpg", g_PictureSaveFilepath,
			nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(),
			nowtime.GetHour(), nowtime.GetMinute(), nowtime.GetSecond());

		sprintf(szFilePath, "%s", strFilePath);

		if (m_pPlayChannelInfo == NULL)
			return;

		bRet = g_GWPlay.PlayCapturePic(m_pPlayChannelInfo->m_nDecodeType,m_pPlayChannelInfo->m_hDecodeHandle,1, 90, szFilePath);
	}
	else
	{
		strFilePath.Format("%s\\%04d-%02d-%02d-%02d-%02d-%02d.bmp", g_PictureSaveFilepath,
			nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(),
			nowtime.GetHour(), nowtime.GetMinute(), nowtime.GetSecond());

		sprintf(szFilePath, "%s", strFilePath);
		
		if (m_VideoViewOutInfo.m_nDvrType == DEVTYPE_AXIS)
		{
			m_AxisView.SaveCurrentImage(CAxismediacontrol1::AMC_IMAGE_FORMAT_BMP,szFilePath);
			bRet = TRUE;
		}
		else 
		{
			if (m_VideoViewOutInfo.m_nVideoSdkId >= 0)
			{
				bRet = CaptureLocalPicture(m_VideoViewOutInfo.m_nVideoSdkId,szFilePath);
			}
		}
	}
	
	char szInfo[256] ={0};

	sprintf_s(szInfo,sizeof(szInfo),"抓图成功，存放在路径%s下",g_PictureSaveFilepath);

	if (bRet == TRUE)
	{
		MessageBox(szInfo,"视频监视");
	}
	else
	{
		MessageBox("抓图失败","视频监视");
	}
}


void CDlgViewOut::OnBnClickedButtonRecordVo()
{
	//关闭录像通道
	if (m_bRecordFlag)
	{
		OnStopRecord();
		return;
	}

	OnStartRecord();
}

BOOL CDlgViewOut::OnStartRecord()
{
	BOOL bDirectDeviceFlag = FALSE;
	BOOL bRet = FALSE;

	int nResult = CheckViewVideoIsOpen(m_iWinID);
	if (nResult <= 0)//判断是否当前通道在预览图像
	{
		MessageBox("该通道没有打开的视频","视频监视");
		return FALSE;
	}
	else if (nResult == 2)
	{
		bDirectDeviceFlag = TRUE;
	}

	if (bDirectDeviceFlag == FALSE)
	{
		bRet = OnStartRecordByVideoPlatform();
	}
	else
	{
		bRet = OnStartRecordByDirectDevice();
	}

	return bRet;
}

BOOL CDlgViewOut::OnStopRecord()
{
	if (m_bRecordFlag != FALSE)
		m_bRecordFlag = FALSE;

	BOOL bDirectDeviceFlag = FALSE;
	BOOL bRet = FALSE;

	try
	{
		m_VideoViewOutInfo.Lock();
		if (m_VideoViewOutInfo.m_nVideoViewType == 1)
		{
			bDirectDeviceFlag = FALSE;
		}
		else if (m_VideoViewOutInfo.m_nVideoViewType == 2)
		{
			bDirectDeviceFlag = TRUE;
		}
		m_VideoViewOutInfo.UnLock();

		if (bDirectDeviceFlag == FALSE)
		{
			bRet = OnStopRecordByVideoPlatform();
		}
		else
		{
			bRet = OnStopRecordByDirectDevice();
		}

		return bRet;
	}
	catch(...)
	{

	}

	return FALSE;
}

BOOL  CDlgViewOut::OnStartRecordByVideoPlatform()
{
	if (m_bRecordFlag == TRUE)
		return TRUE;

	if (g_pMainDlg->m_pDlgRecFilePath == NULL||g_pMainDlg->m_pDlgRecFilePath->DoModal() == IDCANCEL)
		return FALSE;

	m_bRecordFlag = TRUE;

	CTime nowtime = CTime::GetCurrentTime();

	char file_name[100] = {0x0};
	DWORD dwDataLen = 0;
	char szCameraCallNum[32] = {0};
	int    nDvrType = 0;

	m_VideoViewOutInfo.Lock();
	nDvrType = m_VideoViewOutInfo.m_nDecodeTag;
	strcpy_s(szCameraCallNum,sizeof(szCameraCallNum),m_VideoViewOutInfo.m_szCameraCallNum);
	m_VideoViewOutInfo.UnLock();

	sprintf(file_name, "%s\\%s00%3d%04d%02d%02d%02d%02d%02d.mp4",
		g_pMainDlg->m_pDlgRecFilePath->m_Filepath,
		szCameraCallNum,
		nDvrType,
		nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(),
		nowtime.GetHour(), nowtime.GetMinute(), nowtime.GetSecond());

	m_hSaveFile = CreateFile(file_name,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (m_hSaveFile == INVALID_HANDLE_VALUE)
	{
		m_bRecordFlag = FALSE;
		return FALSE;
	}

	if (m_pPlayChannelInfo != NULL)
	{
		WriteFile(m_hSaveFile,m_pPlayChannelInfo->m_Sps,m_pPlayChannelInfo->m_nSpsLen,&dwDataLen,NULL);
		FlushFileBuffers(m_hSaveFile);
	}

	return TRUE;
}

BOOL CDlgViewOut::SaveRecordFile(void *pData,int nDataLen)
{
	DWORD dwDataLen = 0;

	try
	{
		if (m_bRecordFlag != FALSE&&m_hSaveFile != INVALID_HANDLE_VALUE)
		{
			WriteFile(m_hSaveFile,pData,(DWORD)nDataLen,&dwDataLen,NULL);
		}
		return TRUE;
	}
	catch(...)
	{

	}

	return FALSE;
}

BOOL   CDlgViewOut::OnStopRecordByVideoPlatform()
{
	if (m_bRecordFlag != FALSE)
		m_bRecordFlag = FALSE;

	if (m_hSaveFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSaveFile);
		m_hSaveFile = INVALID_HANDLE_VALUE;
	}

	return TRUE;
}

BOOL   CDlgViewOut::OnStartRecordByDirectDevice()
{
	bool bRet = false;
	char szFilePath[128] = {0};
	char szDevNum[32] = {0};
	int    nDvrType = 0;
	int    nSdkId = -1;

	try
	{
		if (m_bRecordFlag == TRUE)
			return TRUE;

		m_VideoViewOutInfo.Lock();
		nDvrType = m_VideoViewOutInfo.m_nDvrType;
		nSdkId = m_VideoViewOutInfo.m_nVideoSdkId;
		strcpy_s(szDevNum,sizeof(szDevNum),m_VideoViewOutInfo.m_szCameraCallNum);
		m_VideoViewOutInfo.UnLock();

		if (nSdkId < 0)
			return FALSE;

		if (g_pMainDlg->m_pDlgRecFilePath == NULL||g_pMainDlg->m_pDlgRecFilePath->DoModal() == IDCANCEL)
			return FALSE;

		m_bRecordFlag = TRUE;

		CTime nowtime = CTime::GetCurrentTime();

		if (m_VideoViewOutInfo.m_nDvrType == DEVTYPE_AXIS)
		{
			sprintf(szFilePath, "%s\\dev_%s00%3d%04d%02d%02d%02d%02d%02d.asf",
				g_pMainDlg->m_pDlgRecFilePath->m_Filepath,
				szDevNum,
				nDvrType,
				nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(),
				nowtime.GetHour(), nowtime.GetMinute(), nowtime.GetSecond());

			m_AxisView.StartRecordMedia(szFilePath,CAxismediacontrol1::AMC_RECORD_FLAG_AUDIO_VIDEO,(CString)"");
			bRet = TRUE;
		}
		else
		{
			sprintf(szFilePath, "%s\\dev_%s00%3d%04d%02d%02d%02d%02d%02d.mp4",
				g_pMainDlg->m_pDlgRecFilePath->m_Filepath,
				szDevNum,
				nDvrType,
				nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(),
				nowtime.GetHour(), nowtime.GetMinute(), nowtime.GetSecond());

			bRet = StartRecordLocalFile(nSdkId,szFilePath);
		}
	
		return bRet;
	}
	catch(...)
	{

	}

	return FALSE;
}

BOOL   CDlgViewOut::OnStopRecordByDirectDevice()
{
	bool bRet = false;
	int    nSdkId = -1;
	int    nDvrType = 0;

	try
	{
		if (m_bRecordFlag != FALSE)
			m_bRecordFlag = FALSE;

		m_VideoViewOutInfo.Lock();
		nSdkId = m_VideoViewOutInfo.m_nVideoSdkId;
		nDvrType = m_VideoViewOutInfo.m_nDvrType;
		m_VideoViewOutInfo.UnLock();

		if (nDvrType == DEVTYPE_AXIS)
		{
			m_AxisView.StopRecordMedia();
			bRet = TRUE;
		}
		else
		{
			if (nSdkId < 0)
				return FALSE;

			bRet = StopRecordLocalFile(nSdkId);
		}

		return bRet;
	}
	catch(...)
	{

	}

	return FALSE;
}

int CDlgViewOut::ReplayGetFileDecodeTypeByVideoPlatform(char *szFileName)
{
	if (szFileName == NULL)
		return 0;

	CString str = szFileName;
	int pos = str.ReverseFind(_T('\\'));
	CString sFilename = str.Mid(pos + 1);

	CString tmp = "";
	tmp = sFilename.Mid(20, 3);
	int decodetype = atoi(tmp);
	return decodetype;
}

BOOL CDlgViewOut::ReplayOpenFileByVideoPlatform(char *szFileName)
{
	if (szFileName == NULL)
		return FALSE;

	strcpy_s(m_szReplayLocalFileName,sizeof(m_szReplayLocalFileName),szFileName);

	return TRUE;
}

BOOL CDlgViewOut::ReplayStartFileByVideoPlatform()
{
	bool bResult = false;

	if (strlen(m_szReplayLocalFileName) == 0)
		return FALSE;

	int nDecodeTag = ReplayGetFileDecodeTypeByVideoPlatform(m_szReplayLocalFileName);
	if (nDecodeTag <= 0)
		return FALSE;

	if (m_pPlayChannelInfo != NULL)
	{
		m_pPlayChannelInfo->Lock();
		bResult = m_pPlayChannelInfo->SetFileName(m_szReplayLocalFileName);
		if (!bResult)
		{
			m_pPlayChannelInfo->Unlock();
			return FALSE;
		}
		m_pPlayChannelInfo->SetDecodeType(nDecodeTag);
		m_pPlayChannelInfo->FileStopPlay();
		bResult = m_pPlayChannelInfo->FileStartPlay();
		m_pPlayChannelInfo->Unlock();
	}

	HANDLE hReplayInfoFile = INVALID_HANDLE_VALUE;
	DWORD dwReadFileCount = 0;     
	char szReplayInfoFileName[256] = {0};
	memset(szReplayInfoFileName,0,sizeof(szReplayInfoFileName));
	char *p = strrchr(m_szReplayLocalFileName,'.');
	if (p != NULL)
	{
		memcpy(szReplayInfoFileName,m_szReplayLocalFileName,p-m_szReplayLocalFileName);
		sprintf(szReplayInfoFileName+strlen(szReplayInfoFileName),".info");

		hReplayInfoFile = CreateFile(szReplayInfoFileName,GENERIC_READ,FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hReplayInfoFile != INVALID_HANDLE_VALUE)
		{
			if(ReadFile(hReplayInfoFile,&g_VideoReplayDownloadInfo,sizeof(g_VideoReplayDownloadInfo),&dwReadFileCount,NULL) != FALSE)
			{
				CloseHandle(hReplayInfoFile);
				hReplayInfoFile = INVALID_HANDLE_VALUE;
				g_HistoryWeatherDataWinInfo[0] = g_VideoReplayDownloadInfo.weather_data_info;
				g_HistoryWeatherDataWinInfo[0].nFlag = 1;
				g_HistoryWeatherDataWinInfo[0].nIndex = 0;
			}
		}
	}

	return (bResult?TRUE:FALSE);
}

BOOL CDlgViewOut::ReplayStopFileByVideoPlatform()
{
	bool bResult = false;

	memset(m_szReplayLocalFileName,0,sizeof(m_szReplayLocalFileName));

	if (m_pPlayChannelInfo != NULL)
	{
		m_pPlayChannelInfo->Lock();
		bResult = m_pPlayChannelInfo->FileStopPlay();
		m_pPlayChannelInfo->Unlock();
	}

	if (g_HistoryWeatherDataWinInfo[0].nFlag == 1)
	{
		g_HistoryWeatherDataWinInfo[0].nFlag = 0;
	}

	return (bResult?TRUE:FALSE);
}

BOOL CDlgViewOut::ReplayStartStreamByVideoPlatform(int nDecodeType,char *pSps,int nSpsLen)
{
	bool bResult = false;

	if (m_pPlayChannelInfo != NULL)
	{
		m_pPlayChannelInfo->Lock();
		m_pPlayChannelInfo->StopPlay();
		m_pPlayChannelInfo->SetData(NULL,NULL,-1,nDecodeType,pSps,nSpsLen,NULL,NULL,-1);
		bResult = m_pPlayChannelInfo->StartPlay();
		m_pPlayChannelInfo->m_nCid = -1;
		m_pPlayChannelInfo->Unlock();
	}

	return (bResult?TRUE:FALSE);
}

BOOL CDlgViewOut::ReplayStopStreamByVideoPlatform()
{
	bool bResult = false;

	if (m_pPlayChannelInfo != NULL)
	{
		m_pPlayChannelInfo->Lock();
		bResult = m_pPlayChannelInfo->StopPlay();
		m_pPlayChannelInfo->m_nCid = -1;
		m_pPlayChannelInfo->Unlock();
	}

	return (bResult?TRUE:FALSE);
}

BOOL CDlgViewOut::ReplayPauseByVideoPlatform()
{
	bool bResult = false;

	if (m_pPlayChannelInfo != NULL)
	{
		m_pPlayChannelInfo->Lock();
		bResult = m_pPlayChannelInfo->PausePlay();
		m_pPlayChannelInfo->Unlock();
	}

	return (bResult?TRUE:FALSE);
}

BOOL CDlgViewOut::ReplayResumeByVideoPlatform()
{
	bool bResult = false;

	if (m_pPlayChannelInfo != NULL)
	{
		m_pPlayChannelInfo->Lock();
		bResult = m_pPlayChannelInfo->ResumePlay();
		m_pPlayChannelInfo->Unlock();
		return TRUE;
	}

	return (bResult?TRUE:FALSE);
}

float CDlgViewOut::ReplayGetFilePosByVideoPlatform()
{
	float fPos = 0.0;

	if (m_pPlayChannelInfo != NULL)
	{
		m_pPlayChannelInfo->Lock();
		m_pPlayChannelInfo->GetPos(&fPos);
		m_pPlayChannelInfo->Unlock();
	}

	return fPos;
}

BOOL CDlgViewOut::ReplaySetFilePosByVideoPlatform(float fPos)
{
	bool bResult = false;

	if (m_pPlayChannelInfo != NULL)
	{
		m_pPlayChannelInfo->Lock();
		bResult = m_pPlayChannelInfo->SetPos(fPos);
		m_pPlayChannelInfo->Unlock();
	}

	return (bResult?TRUE:FALSE);
}

BOOL CDlgViewOut::ReplayFastByVideoPlatform()
{
	bool bResult = false;

	if (m_pPlayChannelInfo != NULL)
	{
		m_pPlayChannelInfo->Lock();
		bResult = m_pPlayChannelInfo->FastPlay();
		m_pPlayChannelInfo->Unlock();
	}

	return (bResult?TRUE:FALSE);
}

BOOL CDlgViewOut::ReplaySlowByVideoPlatform()
{
	bool bResult = false;

	if (m_pPlayChannelInfo != NULL)
	{
		m_pPlayChannelInfo->Lock();
		bResult = m_pPlayChannelInfo->SlowPlay();
		m_pPlayChannelInfo->Unlock();
	}

	return (bResult?TRUE:FALSE);
}

BOOL CDlgViewOut::ReplayOpenFileByDirectDevice(char *szFileName)
{
	if (szFileName == NULL)
		return FALSE;

	char *p = strrchr(szFileName,'\\');
	if (p == NULL)
		return FALSE;

	char Buffer[256] = {0};
	strcpy_s(Buffer,sizeof(Buffer),p);

	if (strlen(Buffer) < 32)
		return FALSE;

	memset(Buffer,0,sizeof(Buffer));
	memcpy(Buffer,p+25,3);

	int nDvrTyp = atoi(Buffer);

	if (nDvrTyp < DEVTYPE_HK||nDvrTyp > DEVTYPE_AXIS)
		return FALSE;

	strcpy_s(m_szReplayLocalFileName,sizeof(m_szReplayLocalFileName),szFileName);

	m_nReplayLocalFileDvrTypeyDirectDevice = nDvrTyp;

	return TRUE;
}

BOOL CDlgViewOut::ReplayStartFileByDirectDevice()
{
	if (strlen(m_szReplayLocalFileName) == 0||m_nReplayLocalFileDvrTypeyDirectDevice <= 0)
		return FALSE;

	m_nReplayLocalFileIDByDirectDevice = StartPlayLocalFile_DevSdk(m_nReplayLocalFileDvrTypeyDirectDevice,m_szReplayLocalFileName,m_Pic.GetSafeHwnd());
	if (m_nReplayLocalFileIDByDirectDevice < 0)
		return FALSE;

	HANDLE hReplayInfoFile = INVALID_HANDLE_VALUE;
	DWORD dwReadFileCount = 0;     
	char szReplayInfoFileName[256] = {0};
	memset(szReplayInfoFileName,0,sizeof(szReplayInfoFileName));
	
	char Buf[1024] = {0};
	memset(Buf,0,sizeof(Buf));

	char *p = strrchr(m_szReplayLocalFileName,'.');
	if (p != NULL)
	{
		memcpy(szReplayInfoFileName,m_szReplayLocalFileName,p-m_szReplayLocalFileName);
		sprintf(szReplayInfoFileName+strlen(szReplayInfoFileName),".info");

		hReplayInfoFile = CreateFile(szReplayInfoFileName,GENERIC_READ,FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hReplayInfoFile != INVALID_HANDLE_VALUE)
		{
			if(ReadFile(hReplayInfoFile,&g_VideoReplayDownloadInfo,sizeof(g_VideoReplayDownloadInfo),&dwReadFileCount,NULL) != FALSE)
			{
				CloseHandle(hReplayInfoFile);
				hReplayInfoFile = INVALID_HANDLE_VALUE;
				g_HistoryWeatherDataWinInfo[0] = g_VideoReplayDownloadInfo.weather_data_info;
				g_HistoryWeatherDataWinInfo[0].nFlag = 1;
				g_HistoryWeatherDataWinInfo[0].nIndex = 0;

				if (strlen(g_VideoReplayDownloadInfo.szCameraName) > 0)
				{
					if(strlen(g_VideoReplayDownloadInfo.szStationName) > 0)
					{
						sprintf_s(Buf,sizeof(Buf),"{本地回放}-{%s}-{%s}",g_VideoReplayDownloadInfo.szStationName,g_VideoReplayDownloadInfo.szCameraName);
						m_strStation.SetText(Buf);
					}
					else
					{
						sprintf_s(Buf,sizeof(Buf),"{本地回放}-{%s}",g_VideoReplayDownloadInfo.szCameraName);
						m_strStation.SetText(Buf);
					}
				}
			}
		}
		else
		{
			sprintf_s(Buf,sizeof(Buf),"%s",m_szReplayLocalFileName);
			m_strStation.SetText(Buf);
		}
	}

	return TRUE;
}

BOOL CDlgViewOut::ReplayStopFileByDirectDevice()
{
	if (m_nReplayLocalFileIDByDirectDevice < 0)
		return FALSE;
	
	return StopPlayLocalFile_DevSdk(m_nReplayLocalFileIDByDirectDevice);
}

BOOL CDlgViewOut::ReplayPauseByDirectDevice()
{
	if (m_nReplayLocalFileIDByDirectDevice < 0)
		return FALSE;

	return PausePlayLocalFile_DevSdk(m_nReplayLocalFileIDByDirectDevice);
}

BOOL CDlgViewOut::ReplayResumeByDirectDevice()
{
	if (m_nReplayLocalFileIDByDirectDevice < 0)
		return FALSE;

	return ReStartPlayLocalFile_DevSdk(m_nReplayLocalFileIDByDirectDevice);
}

BOOL CDlgViewOut::ReplayFastByDirectDevice()
{
	if (m_nReplayLocalFileIDByDirectDevice < 0)
		return FALSE;

	return FastPlayLocalFile_DevSdk(m_nReplayLocalFileIDByDirectDevice);
}

BOOL CDlgViewOut::ReplaySlowByDirectDevice()
{
	if (m_nReplayLocalFileIDByDirectDevice < 0)
		return FALSE;

	return SlowPlayLocalFile_DevSdk(m_nReplayLocalFileIDByDirectDevice);
}

BOOL CDlgViewOut::ReplaySetFilePosByDirectDevice(int nPos)
{
	if (m_nReplayLocalFileIDByDirectDevice < 0||nPos < 0||nPos > 100)
		return FALSE;

	return SetPosPlayLocalFile_DevSdk(m_nReplayLocalFileIDByDirectDevice,nPos);
}

int CDlgViewOut::ReplayGetFileDvrTypeByDirectDevice(char *szFileName)
{
	return 0;
}

void CDlgViewOut::OnDestroy()
{
	CDialog::OnDestroy();
	
	//////////////////////////////////////////////////////////////////////////
	if (m_pFUA != NULL)
	{
		delete m_pFUA;
		m_pFUA = NULL;
	}
}

BEGIN_EVENTSINK_MAP(CDlgViewOut, CDialog)
ON_EVENT(CDlgViewOut, IDC_AXISMEDIACONTROL1, 5, CDlgViewOut::OnMouseDownAxismediacontrol1, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CDlgViewOut, IDC_AXISMEDIACONTROL1, 4, CDlgViewOut::OnDoubleClickAxismediacontrol1, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CDlgViewOut, IDC_AXISMEDIACONTROL1, 1, CDlgViewOut::OnErrorAxismediacontrol1, VTS_I4 VTS_BSTR)
END_EVENTSINK_MAP()

void CDlgViewOut::OnMouseDownAxismediacontrol1(long nButton, long nShiftState, long fX, long fY)
{
	if(nButton==2)
	{
		g_pMainDlg->SendMessage(OM_RIGHTCLICKCHANNEL, m_iWinID);
	}
	else
	{
		g_pMainDlg->SendMessage(OM_CHANNLECHANGE, m_iWinID * 0x1000 + 0);	//m_iYTID);
	}
}

void CDlgViewOut::OnDoubleClickAxismediacontrol1(long nButton, long nShiftState, long fX, long fY)
{
	g_pMainDlg->SendMessage(OM_DBLCHANNEL, m_iWinID);
}

void CDlgViewOut::OnErrorAxismediacontrol1(long theErrorCode, LPCTSTR theErrorInfo)
{
	ClearViewPlatformAndDeviceInfoByWinID(m_iWinID);

	m_strStation.SetText("离线");

	g_VMLog.WriteVmLog("%d窗口Axis打开错误,错误码:0x%x,错误原因:%s",m_iWinID,theErrorCode,theErrorInfo);

	if (m_hCallResultNotifyEvent != NULL)
	{
		SetEvent(m_hCallResultNotifyEvent);
	}

	if (g_pMainDlg != NULL)
	{
		g_pMainDlg->SetTimer(TIMER_VIDEO_ERROR_REASON_VIDEO_CHANNEL_0_TIMER+m_iWinID,2*1000,NULL);
	}
}
