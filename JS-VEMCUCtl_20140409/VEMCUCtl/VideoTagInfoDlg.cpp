// VideoTagInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "VideoTagInfoDlg.h"
#include "EnBitmap.h"


// CVideoTagInfoDlg 对话框

IMPLEMENT_DYNAMIC(CVideoTagInfoDlg, CDialog)

CVideoTagInfoDlg::CVideoTagInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoTagInfoDlg::IDD, pParent)
{
	m_bVideoInfoShowFlag = FALSE;

	//刷子
	m_hBrush = CreateSolidBrush(RGB(255,0,0));

	//字体
	LOGFONT   logfont;   
	memset(&logfont,0,sizeof(LOGFONT));   
	logfont.lfHeight  = 32;                  //字体高度   
	logfont.lfWidth   = 24;                  //字体宽度
	logfont.lfWeight = 1000;              //加粗
	strcpy(logfont.lfFaceName,"宋体"); //字体名
	m_font.CreateFontIndirect(&logfont);

	//信息
	memset(m_szVideoInfo,0,sizeof(m_szVideoInfo));
	m_nVideoInfoIndex = 0;
	m_ptWin.x = 0;
	m_ptWin.y = 0;
}

CVideoTagInfoDlg::~CVideoTagInfoDlg()
{
	DeleteObject(m_hBrush);
}

void CVideoTagInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoTagInfoDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CVideoTagInfoDlg 消息处理程序

BOOL CVideoTagInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL CVideoTagInfoDlg::SetVideoInfo(char *pszVideoInfo,int nIndex)
{
	if (pszVideoInfo == NULL)
		return FALSE;

	if (nIndex < 0||nIndex >= 3)
		return FALSE;

	if (strlen(pszVideoInfo) >= sizeof(m_szVideoInfo[nIndex]))
		return FALSE;

	strcpy(m_szVideoInfo[nIndex],pszVideoInfo);

	Invalidate(TRUE);

	return TRUE;
}

BOOL CVideoTagInfoDlg::SetVideoInfoCurrentIndex(int nIndex)
{
	if (nIndex < 0||nIndex >= 3)
		return FALSE;

	m_nVideoInfoIndex = nIndex;
	
	return TRUE;
}

BOOL CVideoTagInfoDlg::SetFontInfo(int nWidth,int nHeight,int nWeight)
{
	m_font.DeleteObject();

	LOGFONT logfont;   
	memset(&logfont,0,sizeof(LOGFONT));   

	logfont.lfHeight  = nWidth;                //字体高度   
	logfont.lfWidth   = nHeight;               //字体宽度
	logfont.lfWeight = nWeight;              //加粗
	strcpy(logfont.lfFaceName,"宋体");    //字体名

	m_font.CreateFontIndirect(&logfont);

	return TRUE;
}

BOOL CVideoTagInfoDlg::SetBrushColor(COLORREF color)
{
	if (m_hBrush != NULL)
	{
		DeleteObject(m_hBrush);
		m_hBrush = NULL;
	}

	m_hBrush = CreateSolidBrush(color);

	return TRUE;
}


BOOL CVideoTagInfoDlg::HideVideoInfo()
{
	if (IsWindowVisible())
	{
		ShowWindow(SW_HIDE);
	}

	return TRUE;
}

BOOL CVideoTagInfoDlg::ShowVideoInfo(int x,int y,bool bFlag)
{
	if (!bFlag)
	{
		if (m_ptWin.x == x&&m_ptWin.y == y&&IsWindowVisible())
		{
			return TRUE;
		}
	}

	m_ptWin.x = x;
	m_ptWin.y = y;

	::SetWindowPos(m_hWnd,HWND_TOPMOST,m_ptWin.x,m_ptWin.y,0,0,SWP_NOSIZE);
	ShowWindow(SW_SHOW);

	return TRUE;
}

void CVideoTagInfoDlg::OnDestroy()
{
	CDialog::OnDestroy();
	//////////////////////////////////////////////////////////////////////////
}

void CVideoTagInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//////////////////////////////////////////////////////////////////////////
	CDC MemDC;
	CEnBitmap bmp;
	CBitmap *pOldBmp = NULL;
	CFont * pOldFont = NULL;
	HRGN hbmpRgn = NULL;

	RECT rect;
	int nXPos = 0;
	int nYPos = 0;
	GetWindowRect(&rect);

	if (rect.right - rect.left > 300)
	{
		nXPos = (rect.right-rect.left)/2-150;
		if (m_nVideoInfoIndex == 0)
			nXPos += 22;
	}

	if (rect.bottom - rect.top > 40)
	{
		nYPos = (rect.bottom-rect.top)/2 -20;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bmp.LoadBitmap(IDB_BITMAP_VIDEO_INFO_BK);
	MemDC.CreateCompatibleDC(&dc);
	pOldBmp = MemDC.SelectObject(&bmp);
	pOldFont = MemDC.SelectObject(&m_font);

	//////////////////////////////////////////////////////////////////////////
	MemDC.SetBkMode(TRANSPARENT);

	if (m_nVideoInfoIndex == 0||m_nVideoInfoIndex == 2)
	{
		if (m_bVideoInfoShowFlag&&strlen(m_szVideoInfo[m_nVideoInfoIndex]) > 0)
			MemDC.TextOut(nXPos,nYPos,m_szVideoInfo[m_nVideoInfoIndex],strlen(m_szVideoInfo[m_nVideoInfoIndex]));
	}

	MemDC.SelectObject(pOldBmp);
	MemDC.SelectObject(pOldFont);

	//////////////////////////////////////////////////////////////////////////
	hbmpRgn = bmp.BitmapToRegion(RGB(255,0,255),RGB(230,200,230));
	SetWindowRgn(hbmpRgn,TRUE);

	if (hbmpRgn != NULL)
	{
		DeleteObject(hbmpRgn);
		hbmpRgn = NULL;
	}
}

BOOL CVideoTagInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}

LRESULT CVideoTagInfoDlg::OnNcHitTest(CPoint point)
{
	//return HTCAPTION;
	return CDialog::OnNcHitTest(point);
}

HBRUSH CVideoTagInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return m_hBrush;
}

BOOL CVideoTagInfoDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_VISIBLE;
	return CDialog::PreCreateWindow(cs);
}

