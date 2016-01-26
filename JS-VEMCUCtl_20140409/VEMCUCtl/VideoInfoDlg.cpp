// VideoInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "VideoInfoDlg.h"
#include "EnBitmap.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CVideoInfoDlg 对话框

IMPLEMENT_DYNAMIC(CVideoInfoDlg, CDialog)

CVideoInfoDlg::CVideoInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoInfoDlg::IDD, pParent)
{
	m_bVideoInfoShowFlag = FALSE;
	m_bVideoArrowShowFlag = FALSE;

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

	//画笔
	LOGPEN  logpen;
	memset(&logpen,0,sizeof(logpen));

	logpen.lopnColor = RGB(255,0,0);
	logpen.lopnStyle = PS_SOLID;
	logpen.lopnWidth.x = 2;
	logpen.lopnWidth.y = 1;
	
	m_pen.CreatePenIndirect(&logpen);

	//信息
	memset(m_szVideoInfo,0,sizeof(m_szVideoInfo));
	m_ptWin.x = 0;
	m_ptWin.y = 0;
}

CVideoInfoDlg::~CVideoInfoDlg()
{
	DeleteObject(m_hBrush);
}

void CVideoInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoInfoDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CVideoInfoDlg 消息处理程序

BOOL CVideoInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL CVideoInfoDlg::SetVideoInfo(char *pszVideoInfo)
{
	if (pszVideoInfo == NULL)
		return FALSE;

	if (strlen(pszVideoInfo) >= sizeof(m_szVideoInfo))
		return FALSE;

	strcpy(m_szVideoInfo,pszVideoInfo);

	Invalidate(TRUE);

	return TRUE;
}

BOOL CVideoInfoDlg::SetArrowInfo(int nIndex,int nStartX,int nStartY,int nStopX,int nStopY)
{
	VMArrowLine *pArrowLine = NULL;

	if (nIndex == 0)
		pArrowLine = &m_OneArrowLine;
	else
		pArrowLine = &m_TwoArrowLine;

	pArrowLine->m_nArrowStartX = nStartX;
	pArrowLine->m_nArrowStartY = nStartY;
	pArrowLine->m_nArrowStopX = nStopX;
	pArrowLine->m_nArrowStopY = nStopY;

	Invalidate(TRUE);

	return TRUE;
}

BOOL CVideoInfoDlg::SetFontInfo(int nWidth,int nHeight,int nWeight)
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

BOOL CVideoInfoDlg::SetBrushColor(COLORREF color)
{
	if (m_hBrush != NULL)
	{
		DeleteObject(m_hBrush);
		m_hBrush = NULL;
	}

	m_hBrush = CreateSolidBrush(color);

	return TRUE;
}

BOOL CVideoInfoDlg::SetPenInfo(int nWidth,COLORREF color)
{
	m_pen.DeleteObject();

	LOGPEN  logpen;
	memset(&logpen,0,sizeof(logpen));

	logpen.lopnColor = color;
	logpen.lopnStyle = PS_SOLID;
	logpen.lopnWidth.x = nWidth;
	logpen.lopnWidth.y = 1;

	m_pen.CreatePenIndirect(&logpen);

	return TRUE;
}

BOOL CVideoInfoDlg::HideVideoInfo()
{
	if (IsWindowVisible())
	{
		ShowWindow(SW_HIDE);
	}

	return TRUE;
}

BOOL CVideoInfoDlg::ShowVideoInfo(int x,int y,bool bFlag)
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

BOOL CVideoInfoDlg::ShowArrowLine(CDC *pDC)
{
	ShowArrowLineOne(pDC);
	ShowArrowLineTwo(pDC);
	return TRUE;
}

BOOL CVideoInfoDlg::ShowArrowLineOne(CDC *pDC)
{
	int nLength = 0;

	double x0,y0,x1,y1,x2,y2,x3,y3;
	double theta = 3.1415926/9;
	double cos_theta = cos(theta);
	double sin_theta = sin(theta);

	if (pDC == NULL)
		return FALSE;

	if (m_OneArrowLine.m_bShowFlag)
	{
		nLength = m_OneArrowLine.GetLineLength();
		if (nLength == 0)
			return TRUE;

		x0 = m_OneArrowLine.m_nArrowStartX - m_OneArrowLine.m_nArrowStopX;
		y0 = m_OneArrowLine.m_nArrowStartY - m_OneArrowLine.m_nArrowStopY;

		x1 = x0*cos_theta-y0*sin_theta;
		y1 = x0*sin_theta+y0*cos_theta;
		x2 = x0*cos_theta+y0*sin_theta;
		y2 = -x0*sin_theta+y0*cos_theta;

		x1 = x1/4;
		y1 = y1/4;
		x2 = x2/4;
		y2 = y2/4;

		x1 += m_OneArrowLine.m_nArrowStopX;
		y1 += m_OneArrowLine.m_nArrowStopY;
		x2 += m_OneArrowLine.m_nArrowStopX;
		y2 += m_OneArrowLine.m_nArrowStopY;

		pDC->MoveTo(m_OneArrowLine.m_nArrowStartX,m_OneArrowLine.m_nArrowStartY);
		pDC->LineTo(m_OneArrowLine.m_nArrowStopX,m_OneArrowLine.m_nArrowStopY);

		pDC->MoveTo(m_OneArrowLine.m_nArrowStopX,m_OneArrowLine.m_nArrowStopY);
		pDC->LineTo(x1,y1);

		pDC->MoveTo(m_OneArrowLine.m_nArrowStopX,m_OneArrowLine.m_nArrowStopY);
		pDC->LineTo(x2,y2);

		if (strlen(m_OneArrowLine.m_szInfo) > 0)
		{
			x3 = (m_OneArrowLine.m_nArrowStartX + m_OneArrowLine.m_nArrowStopX)/2;
			y3 = (m_OneArrowLine.m_nArrowStartY + m_OneArrowLine.m_nArrowStopY)/2;
			pDC->TextOut(x3,y3,m_OneArrowLine.m_szInfo,strlen(m_OneArrowLine.m_szInfo));
		}
	}

	return TRUE;
}

BOOL CVideoInfoDlg::ShowArrowLineTwo(CDC *pDC)
{
	int nLength = 0;

	double x0,y0,x1,y1,x2,y2,x3,y3;
	double theta = 3.1415926/9;
	double cos_theta = cos(theta);
	double sin_theta = sin(theta);

	if (pDC == NULL)
		return FALSE;

	if (m_TwoArrowLine.m_bShowFlag)
	{
		nLength = m_TwoArrowLine.GetLineLength();
		if (nLength == 0)
			return TRUE;

		x0 = m_TwoArrowLine.m_nArrowStartX - m_TwoArrowLine.m_nArrowStopX;
		y0 = m_TwoArrowLine.m_nArrowStartY - m_TwoArrowLine.m_nArrowStopY;

		x1 = x0*cos_theta-y0*sin_theta;
		y1 = x0*sin_theta+y0*cos_theta;
		x2 = x0*cos_theta+y0*sin_theta;
		y2 = -x0*sin_theta+y0*cos_theta;

		x1 = x1/4;
		y1 = y1/4;
		x2 = x2/4;
		y2 = y2/4;

		x1 += m_TwoArrowLine.m_nArrowStopX;
		y1 += m_TwoArrowLine.m_nArrowStopY;
		x2 += m_TwoArrowLine.m_nArrowStopX;
		y2 += m_TwoArrowLine.m_nArrowStopY;

		pDC->MoveTo(m_TwoArrowLine.m_nArrowStartX,m_TwoArrowLine.m_nArrowStartY);
		pDC->LineTo(m_TwoArrowLine.m_nArrowStopX,m_TwoArrowLine.m_nArrowStopY);

		pDC->MoveTo(m_TwoArrowLine.m_nArrowStopX,m_TwoArrowLine.m_nArrowStopY);
		pDC->LineTo(x1,y1);

		pDC->MoveTo(m_TwoArrowLine.m_nArrowStopX,m_TwoArrowLine.m_nArrowStopY);
		pDC->LineTo(x2,y2);

		if (strlen(m_TwoArrowLine.m_szInfo) > 0)
		{
			x3 = (m_TwoArrowLine.m_nArrowStartX + m_TwoArrowLine.m_nArrowStopX)/2;
			y3 = (m_TwoArrowLine.m_nArrowStartY + m_TwoArrowLine.m_nArrowStopY)/2;
			pDC->TextOut(x3,y3,m_TwoArrowLine.m_szInfo,strlen(m_TwoArrowLine.m_szInfo));
		}
	}

	return TRUE;
}

BOOL CVideoInfoDlg::ClearVideoArrowTextInfo()
{
	m_OneArrowLine.ClearLineInfo();
	m_TwoArrowLine.ClearLineInfo();

	memset(m_szVideoInfo,0,sizeof(m_szVideoInfo));
	m_bVideoArrowShowFlag = FALSE;
	m_bVideoInfoShowFlag = FALSE;

	POINT point[4];
	point[0].x = 0;
	point[0].y = 0;
	point[1].x = 1;
	point[1].y = 0;
	point[2].x = 1;
	point[2].y = 1;
	point[3].x = 0;
	point[3].y = 1;

	HRGN hRgn = CreatePolygonRgn(point,4,ALTERNATE);
	SetWindowRgn(hRgn,TRUE);
	if (hRgn != NULL)
	{
		DeleteObject(hRgn);
		hRgn = NULL;
	}

	return TRUE;
}

void CVideoInfoDlg::OnDestroy()
{
	CDialog::OnDestroy();
	//////////////////////////////////////////////////////////////////////////
}

void CVideoInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//////////////////////////////////////////////////////////////////////////
	CDC MemDC;
	CEnBitmap bmp;
	CBitmap *pOldBmp = NULL;
	CFont * pOldFont = NULL;
	CPen *pOldPen = NULL;
	HRGN hbmpRgn = NULL;

	//////////////////////////////////////////////////////////////////////////
	bmp.LoadBitmap(IDB_BITMAP_VIDEO_INFO_BK);
	MemDC.CreateCompatibleDC(&dc);
	pOldBmp = MemDC.SelectObject(&bmp);
	pOldFont = MemDC.SelectObject(&m_font);
	pOldPen = MemDC.SelectObject(&m_pen);

	//////////////////////////////////////////////////////////////////////////
	MemDC.SetBkMode(TRANSPARENT);
	if (m_bVideoInfoShowFlag&&strlen(m_szVideoInfo) > 0)
		MemDC.TextOut(0,0,m_szVideoInfo,strlen(m_szVideoInfo));
	if (m_bVideoArrowShowFlag)
		ShowArrowLine(&MemDC);//显示箭头
	MemDC.SelectObject(pOldBmp);
	MemDC.SelectObject(pOldFont);
	MemDC.SelectObject(pOldPen);

	//////////////////////////////////////////////////////////////////////////
	hbmpRgn = bmp.BitmapToRegion(RGB(255,0,255),RGB(230,200,230));
	SetWindowRgn(hbmpRgn,TRUE);
	
	if (hbmpRgn != NULL)
	{
		DeleteObject(hbmpRgn);
		hbmpRgn = NULL;
	}
}

BOOL CVideoInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}

LRESULT CVideoInfoDlg::OnNcHitTest(CPoint point)
{
//	return HTCAPTION;
	return CDialog::OnNcHitTest(point);
}

HBRUSH CVideoInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return m_hBrush;
}

BOOL CVideoInfoDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_VISIBLE;
	return CDialog::PreCreateWindow(cs);
}
