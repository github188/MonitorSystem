// DlgTempFavorite.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgTempFavorite.h"
#include "VEMCUCtlDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
CDlgTempFavorite*	g_pDlgTempFavorite;
long g_TempFavoritePort[4];
// CDlgTempFavorite 对话框

IMPLEMENT_DYNAMIC(CDlgTempFavorite, CDialog)

CDlgTempFavorite::CDlgTempFavorite(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTempFavorite::IDD, pParent)
{
	m_CurrentCh = 0;
	
	g_TempFavoritePort[0] = TEMPFAVORITECHA;
	g_TempFavoritePort[1] = TEMPFAVORITECHB;
	g_TempFavoritePort[2] = TEMPFAVORITECHC;
	g_TempFavoritePort[3] = TEMPFAVORITECHD;
}

CDlgTempFavorite::~CDlgTempFavorite()
{
}

void CDlgTempFavorite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_TF_QUIT, m_btnQuit);
}


BEGIN_MESSAGE_MAP(CDlgTempFavorite, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_NCHITTEST()
	ON_MESSAGE(OM_DBLCHANNEL, OnDBLChannel)
	ON_MESSAGE(OM_CHANNLECHANGE, OnChannelChange)
	ON_MESSAGE(OM_RIGHTCLICKCHANNEL, OnRightClickChannel)
	ON_COMMAND(ID_MENUITEM_DELETEFAVORITE, &CDlgTempFavorite::OnMenuitemDeleteFavorite)
	ON_COMMAND(ID_MENUITEM_PLAYTOMAIN, &CDlgTempFavorite::OnMenuitemPlay2Main)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_TF_QUIT, &CDlgTempFavorite::OnBnClickedButtonTfQuit)
END_MESSAGE_MAP()


// CDlgTempFavorite 消息处理程序

BOOL CDlgTempFavorite::OnEraseBkgnd(CDC* pDC)
{
	CRect   rect;
	GetClientRect(&rect);
	CBitmap   m_pBmp;
	BITMAP   bm;
	CDC   dcMem;
	CBitmap*   pOldBitmap;

	// 左上角
	m_pBmp.LoadBitmap(IDB_BITMAP_BG_LEFTUPCORNER);
	m_pBmp.GetObject(sizeof(BITMAP),(LPVOID)&bm);
	dcMem.CreateCompatibleDC(pDC);
	pOldBitmap = dcMem.SelectObject(&m_pBmp);

	pDC->StretchBlt(
		rect.left,
		rect.top,
		bm.bmWidth,
		bm.bmHeight,
		&dcMem,
		0,
		0,
		bm.bmWidth,
		bm.bmHeight,
		SRCCOPY);
	(dcMem.SelectObject(pOldBitmap))->DeleteObject();
	dcMem.DeleteDC();

	// 右上角
	m_pBmp.LoadBitmap(IDB_BITMAP_BG_RIGHTUPCORNER);
	m_pBmp.GetObject(sizeof(BITMAP),(LPVOID)&bm);
	dcMem.CreateCompatibleDC(pDC);
	pOldBitmap = dcMem.SelectObject(&m_pBmp);

	pDC->StretchBlt(
		rect.right-5,
		rect.top,
		bm.bmWidth,
		bm.bmHeight,
		&dcMem,
		0,
		0,
		bm.bmWidth,
		bm.bmHeight,
		SRCCOPY);
	(dcMem.SelectObject(pOldBitmap))->DeleteObject();
	dcMem.DeleteDC();

	// 上边
	m_pBmp.LoadBitmap(IDB_BITMAP_BG_UPCORNER);
	m_pBmp.GetObject(sizeof(BITMAP),(LPVOID)&bm);
	dcMem.CreateCompatibleDC(pDC);
	pOldBitmap = dcMem.SelectObject(&m_pBmp);

	pDC->StretchBlt(
		rect.left + 5,
		rect.top,
		rect.right - 10,
		bm.bmHeight,
		&dcMem,
		0,
		0,
		bm.bmWidth,
		bm.bmHeight,
		SRCCOPY);
	(dcMem.SelectObject(pOldBitmap))->DeleteObject();
	dcMem.DeleteDC();

	// 左边
	m_pBmp.LoadBitmap(IDB_BITMAP_BG_LEFTCORNER);
	m_pBmp.GetObject(sizeof(BITMAP),(LPVOID)&bm);
	dcMem.CreateCompatibleDC(pDC);
	pOldBitmap = dcMem.SelectObject(&m_pBmp);

	pDC->StretchBlt(
		rect.left,
		rect.top + 5,
		bm.bmWidth,
		rect.bottom,
		&dcMem,
		0,
		0,
		bm.bmWidth,
		bm.bmHeight,
		SRCCOPY);
	(dcMem.SelectObject(pOldBitmap))->DeleteObject();
	dcMem.DeleteDC();

	// 右边
	m_pBmp.LoadBitmap(IDB_BITMAP_BG_RIGHTCORNER);
	m_pBmp.GetObject(sizeof(BITMAP),(LPVOID)&bm);
	dcMem.CreateCompatibleDC(pDC);
	pOldBitmap = dcMem.SelectObject(&m_pBmp);

	pDC->StretchBlt(
		rect.right - 2,
		rect.top + 5,
		bm.bmWidth,
		rect.bottom - 5,
		&dcMem,
		0,
		0,
		bm.bmWidth,
		bm.bmHeight,
		SRCCOPY);
	(dcMem.SelectObject(pOldBitmap))->DeleteObject();
	dcMem.DeleteDC();

	// 中间
	m_pBmp.LoadBitmap(IDB_BITMAP_TOTAL_BG);
	m_pBmp.GetObject(sizeof(BITMAP),(LPVOID)&bm);
	dcMem.CreateCompatibleDC(pDC);
	pOldBitmap = dcMem.SelectObject(&m_pBmp);

	pDC->StretchBlt(
		rect.left + 2,
		rect.top + 5,
		rect.right - 4,
		rect.bottom - 5,
		&dcMem,
		0,
		0,
		bm.bmWidth,
		bm.bmHeight,
		SRCCOPY);
	(dcMem.SelectObject(pOldBitmap))->DeleteObject();
	dcMem.DeleteDC();

	return true;
}

BOOL CDlgTempFavorite::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==0x100   &&   pMsg->wParam==27)//屏住ESC
		return	TRUE;
	if(pMsg->message==0x100   &&   pMsg->wParam==13)//屏住ENTER   
		return	TRUE; 
	if(pMsg->message==0x100   &&   pMsg->wParam==VK_SPACE)
		return	TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgTempFavorite::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化四个区域
	RECT rc;
	CRect rc_ch;
	GetClientRect(&rc);
	rc.top = rc.top + 25;
	for (int i = 0; i < 4; i++)
	{
		m_ch[i].Create(IDD_DIALOG_VIEWOUT, this);
		m_ch[i].m_iWinID = i;
		m_ch[i].m_Pic.m_iWinID = i;
	}

	rc_ch.top = rc.top + 4;
	rc_ch.bottom = rc.top + (rc.bottom - rc.top)/2 - 4;
	rc_ch.left = rc.left + 4;
	rc_ch.right = rc.left + (rc.right - rc.left)/2 - 4;
	m_ch[0].MoveWindow(rc_ch);
	m_ch[0].ShowWindow(SW_SHOW);
	rc_ch.top = rc.top + 4;
	rc_ch.bottom = rc.top + (rc.bottom - rc.top)/2 - 4;
	rc_ch.left = rc.left + (rc.right - rc.left)/2 + 4;
	rc_ch.right = rc.right - 4;
	m_ch[1].MoveWindow(rc_ch);
	m_ch[1].ShowWindow(SW_SHOW);
	rc_ch.top = rc.top + (rc.bottom - rc.top)/2 + 4;
	rc_ch.bottom = rc.bottom - 4;
	rc_ch.left = rc.left + 4;
	rc_ch.right = rc.left + (rc.right - rc.left)/2 - 4;
	m_ch[2].MoveWindow(rc_ch);
	m_ch[2].ShowWindow(SW_SHOW);
	rc_ch.top = rc.top + (rc.bottom - rc.top)/2 + 4;
	rc_ch.bottom = rc.bottom - 4;
	rc_ch.left = rc.left + (rc.right - rc.left)/2 + 4;
	rc_ch.right = rc.right - 4;
	m_ch[3].MoveWindow(rc_ch);
	m_ch[3].ShowWindow(SW_SHOW);
	
	DrawRect(m_CurrentCh, 1);
	m_BgBrush.CreateSolidBrush(RGB(14,84,76));         // 背景的颜色

	m_btnQuit.LoadBitmap(IDB_BITMAP_QUIT2, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTempFavorite::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int i = 0;
	for (i = 0; i < 4; i ++)
	{
		if (m_ch[i].m_Callid >= 0)
		{
			AppHangupCallid(m_ch[i].m_Callid);
			Sleep(50);
			PlayClose(i);
		}
	}

	CDialog::OnClose();
}

//右键单击，弹出菜单
LRESULT CDlgTempFavorite::OnRightClickChannel(WPARAM wParam, LPARAM lParam)
{
	int iServerID = (int)wParam;	//窗口标识
	int iWinID = lParam / 0x100;
	int ich = lParam & 0xff;
		
	int iYTID = wParam % 0x1000;
	wParam = wParam / 0x1000;

	if (iServerID >= 4 || iServerID < 0)
		return 1;

	if (m_CurrentCh != (int)iServerID)
	{
		CRect rc;
		if (m_CurrentCh >= 0 && m_CurrentCh < 4)
		{
			DrawRect(m_CurrentCh, 0);
		}
		
		if ((iServerID >= 0) && (iServerID < 4))
			m_CurrentCh = iServerID;
		else
			m_CurrentCh = 0;
		
		DrawRect(m_CurrentCh, 1);
	}

	CMenu  popMenu;

	popMenu.LoadMenu(IDR_MENU_TEMPFAVORITE);
	CMenu *pMenu = popMenu.GetSubMenu(0); 

	CPoint posMouse;
	GetCursorPos(&posMouse);

	popMenu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_RIGHTALIGN,  posMouse.x, posMouse.y, this); 

	return 1;
}

//根据主画面的id呼叫摄像头
void CDlgTempFavorite::MakeCallCameraByMainID(int id)
{
	//关闭之前的会话
	if (m_ch[m_CurrentCh].m_Callid >= 0)
	{
		AppHangupCallid(m_ch[m_CurrentCh].m_Callid);
		Sleep(100);
		PlayClose(m_CurrentCh);
		m_ch[m_CurrentCh].m_Pic.DeleteTempMap();
		m_ch[m_CurrentCh].m_Pic.FlashWindow(true);
		m_ch[m_CurrentCh].Invalidate(true);
	}
	//初始化值
	m_ch[m_CurrentCh].m_CameraStatus = g_DlgVideoView[id].m_CameraStatus;
	strcpy(m_ch[m_CurrentCh].m_CameraCallNum, g_DlgVideoView[id].m_CameraCallNum);
	m_ch[m_CurrentCh].m_DvrType = g_DlgVideoView[id].m_DvrType;
	strcpy(m_ch[m_CurrentCh].m_Notes, g_DlgVideoView[id].m_Notes);
	//初始化播放窗口
	InitViewChannel(m_CurrentCh, m_ch[m_CurrentCh].m_DvrType);
	//发起呼叫
	AppMakeCallWithTempFavorite(m_ch[m_CurrentCh].m_CameraCallNum, m_CurrentCh);
	Sleep(100);
	//显示画面的名称
	//....................
	strcpy(m_ch[m_CurrentCh].m_Notes, g_DlgVideoView[id].m_Notes);
	m_ch[m_CurrentCh].m_strStation.SetText(m_ch[m_CurrentCh].m_Notes);
}

/************************************************************************
*视频通道窗口变化
*鼠标点击某个窗口
*wParam / 0x1000 : 焦点窗口
*lParam : 焦点窗口
************************************************************************/
LRESULT CDlgTempFavorite::OnChannelChange(WPARAM wParam, LPARAM lParam)
{
	int iYTID = wParam % 0x1000;
	wParam = wParam / 0x1000;

	if (m_CurrentCh != (int)wParam)
	{
		CRect rc;
		if (m_CurrentCh >= 0 && m_CurrentCh < 4)
		{
			DrawRect(m_CurrentCh, 0);
		}

		if ((wParam >= 0) && (wParam < MAXVIEWCH))
			m_CurrentCh = wParam;
		else
			m_CurrentCh = 0;

		DrawRect(m_CurrentCh, 1);
	}
	if (m_CurrentCh >= 0)
		DrawRect(m_CurrentCh, 1);

	return 1;
}

/************************************************************************
*  DrawRect
*  在指定视频通道窗口画方框 / 红框(选中)，或黑框(未选)
*
************************************************************************/
void CDlgTempFavorite::DrawRect(int iWinID, int bHighLight)
{
	COLORREF m_color;
	CDC *dc = GetDC();
	switch (bHighLight)
	{
	case 0:
		m_color = RGB(14,84,76);
		break;
	case 1:
		m_color = RGB(116,236,250);
		break;
	case 2:
		m_color = RGB(0,200,0);
		break;
	case 3:
		m_color = RGB(14,84,76);
		break;
	}
	CRect Rect;
	m_ch[iWinID].GetWindowRect(&Rect);
	ScreenToClient(&Rect);

	//rc.InflateRect(1, 1, 0, 0); 
	Rect.left--;
	//Rect.right++;
	Rect.top--;
	//Rect.bottom++;

	CPen pPen(PS_SOLID, 1, m_color);
	CPen *oldPen = dc->SelectObject(&pPen);
	dc->MoveTo(Rect.left,Rect.top);
	dc->LineTo(Rect.right,Rect.top);
	dc->LineTo(Rect.right,Rect.bottom);
	dc->LineTo(Rect.left,Rect.bottom);
	dc->LineTo(Rect.left,Rect.top);
	dc->SelectObject(oldPen);

	ReleaseDC(dc);
}

//////////////////
// Non-client hit-test handler to move window by its client area.
// If the user clicks anywhere on the client area, pretend it's the
// caption. Windows does the rest!
//
LRESULT CDlgTempFavorite::OnNcHitTest(CPoint pt)
{
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);
	return rc.PtInRect(pt) ? HTCAPTION : CDialog::OnNcHitTest(pt);
}

//初始化视频区域解码
void CDlgTempFavorite::InitViewChannel(int t_ch, int t_dvrtype)
{

}

//通过菜单项删除指定收藏内容
void CDlgTempFavorite::OnMenuitemDeleteFavorite()
{
	if (m_ch[m_CurrentCh].m_Callid > 0)
	{
		AppHangupCallid(m_ch[m_CurrentCh].m_Callid);
		Sleep(100);

		PlayClose(m_CurrentCh);
	}
}

//关闭指定窗口播放控件
void CDlgTempFavorite::PlayClose(int i)
{
	m_ch[i].CloseViewChannel();
}

//响应双击事件，在多画面中显示图像
LRESULT CDlgTempFavorite::OnDBLChannel(WPARAM wParam, LPARAM lParam)
{
	return 1;
}

//响应菜单项，在制定的多画面中打开图像
void CDlgTempFavorite::OnMenuitemPlay2Main()
{
	if (m_ch[m_CurrentCh].m_Callid <= 0)
	{
		MessageBox("没有选择收藏夹画面或指定画面无视频","视频监视");
	}
	if (g_pMainDlg->m_iFocuseWindowID < 0 || g_pMainDlg->m_iFocuseWindowID > 25)
	{
		MessageBox("多画面中未指定显示画面","视频监视");
	}

	// 去掉notes的“（”后的内容
	char notes[255] = {0x0};
	sprintf(notes, "%s", m_ch[m_CurrentCh].m_Notes);

	char *token = strtok(notes, "(");

	// 呼叫摄像头
	g_pMainDlg->MakeCallCamera(m_ch[m_CurrentCh].m_CameraStatus,
		m_ch[m_CurrentCh].m_CameraCallNum,
		g_pMainDlg->m_iFocuseWindowID, 
		m_ch[m_CurrentCh].m_CameraName,
		m_ch[m_CurrentCh].m_DvrType,
		NULL,
		notes);
}

HBRUSH CDlgTempFavorite::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
    if (pWnd->GetDlgCtrlID() == IDC_STATIC_TILE)
    {
       // 背景色透明
       pDC->SetBkMode(TRANSPARENT);
	   pDC->SetTextColor(RGB(200,200,200));
       // 返回背景色的画刷
       return m_BgBrush;
    }

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CDlgTempFavorite::OnBnClickedButtonTfQuit()
{
	g_pMainDlg->m_bShowTempFavorite = !g_pMainDlg->m_bShowTempFavorite;
	g_pDlgTempFavorite->ShowWindow(SW_HIDE);
}
