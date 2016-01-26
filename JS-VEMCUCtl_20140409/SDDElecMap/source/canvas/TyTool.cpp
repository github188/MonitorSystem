// TyTool.cpp: implementation of the CTyTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../SDDElecMapDlg.h"
#include "../../ElecMapDoc.h"
#include "../../ElecMapView.h"
#include "canvas/TyTool.h"
#include "canvas/jdjh.h"
#include "canvas/ShuZhi.h"
#include "canvas/hmlayer.h"
#include "canvas/HMProject.h"
#include "canvas/huamian.h"
#include "canvas/LinkTool.h"
#include "canvas/rtvaluetool.h"
#include "canvas/SZ_PhotoLinkTool.h"
#include "canvas/SZ_SgbwLinkTool.h"
#include "canvas/CurveTool.h"
#include "canvas/SZ_SbLookLinkTool.h"
#include "canvas/MNP_SHXGTOOL.H"
#include "canvas/MNP_ZJNHTOOL.H"
#include "canvas/TicketTool.h"
#include "canvas/LinkFileTool.h"
#include "canvas/Ty_YcYwTool.h"
#include "canvas/SZ_TimeycLinkTool.h"
#include "canvas/LinkSupvTool.h"
#include "canvas/TyStationAssistPngTool.h"

//#include "canvas/TyState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPtrList CTyTool::c_tools;

//普通图元工具
CSelectTool		    SelectTool;
CZoomOutTool	ZoomOutTool;
CZoomInTool		ZoomInTool;
CTyStationAssistPngTool TyStationAssistPngTool;
static CLineTool			lineTool;
static CRectangleTool  rectTool(tyRect);
static CRectangleTool  roundrectTool(tyRoundRect);
static CRectangleTool  EllipseTool(tyEllipse); 
static CXLineTool			XLineTool;
static CPolygonTool		PolygonTool(tyDBX);
static CPolygonTool		PolyLineTool(tyZX);
static CTextTool			TextTool(tyText);
static CTyBmpTool		TyBmpTool;
static CTyVectorTool	TyVectorTool;

//链接图元工具
static CLinkYkTool				LinkYkTool;
static CLinkZkTool                LinkZkTool;
static CNetColorPowerTool	NetColorPowerTool;
static CNetColorVectorTool	NetColorVectorTool;
static CNetColorBmpTool	NetColorBmpTool;
static CNetColorYxTool		NetColorYxTool;
static CLinkYTTool				LinkYtTool;
static CLinkYZTYTool			LinkYZTYTool;
static CLinkUnitTool				LinkUnitTool;
static CSZ_PhotoLinkTool		SZ_PhotoLinkTool;
static CSZ_SgbwLinkTool		SZ_SgbwLinkTool;
static CCurveTool					CurveTool;
static CLinkClubTool             LinkClubTool;
static CMNP_SHXGTool        LinkMNP_SHXG;
static CMNP_ZJNHTool         LinkMNP_ZJNH;
static CTicketTool                 TicketTool;
static CLinkNodeTool			 LinkNodeTool;
static CLinkLineTool				 LinkLineTool;
static CLinkFileTool				 LinkFileTool;

//modem
static CLinkModemTool		LinkModemTool ;
static CLinkYT2Tool				LinkYt2Tool;
//YXbind
static CLinkXYBindTool		LinkYXBindTool ;
//QK
static CLinkQKTool			LinkQKTool ;
//后台检修牌
static CLinkSoftRepairTool	LinkSoftRepairTool ;
//单元扩展
static CLinkUnitExtendTool	LinkUnitExtendTool ;
//馈线开关
static CLinkKXTool			LinkKXTool ;
//馈线区段
static CLinkKXSectionTool	LinkKXSectionTool ;
//水行业控制
static CLinkDoubleCtrlTool  LinkDoubleCtrlTool;

static CLinkPowerGzTjTool          LinkPowerGzTjTool;
static CLinkLineGzTjTool            LinkLineGzTjTool;

//第三方应用程序接口图元工具
static CLinkTxtFileTool     LinkTxtFileTool;
static CTyMonNiYKTool       TyMonNiYKTool;
static CSZ_SbLookLinkTool   c_SZ_SbLookLinkTool;
static CLinkWebShotTool		 LinkWebShotTool;
static CSZ_TimeycLinkTool   c_SZ_TimeycLinkTool;
static CLinkSupvTool			LinkSupvTool;

//实时数据图元工具
static CYCTool						YcTool;
static CNewYCTool				NewYcTool ;
static CTyDirectionTool		TyDirectionTool;
static CTyHMProxyTool		TyHMProxyTool;
static CYXTool						YxTool;
static CYDXTool					YdxTool;
static CYMTool						YmTool;
static CTyDateTimeTool		TyDateTimeTool;
static CTyAnimateTool			TyAnimateTool;
static CMcCurveTool			McCurveTool;
static CTy_YcYwTool				Ty_YcYwTool;
static CYMcCurveTool			YMcCurveTool;
static  CYcCurveTool            MyTool; 

static CTyStateTool             StateTool;

//普通图元默认属性
CPoint CTyTool::c_down;
UINT CTyTool::c_nDownFlags;
CPoint CTyTool::c_last;
DRAW_TY CTyTool::c_drawTy = tySelect;
LOGBRUSH CTyTool::c_LogBrush = {BS_NULL,0,0};
LOGPEN	CTyTool::c_LogPen = {PS_SOLID,{1,0},0xff};

CTyTool::SelectMode CTyTool::selectMode = none;
int CTyTool::nDragHandle;
PointStruct CTyTool::lastPoint;

//矩形默认属性
BOOL CRectangleTool::c_bFill = TRUE;
COLORREF CRectangleTool::c_FillColor = RGB(255,0,0);

//多边形默认属性
BOOL CPolygonTool::c_bFill = TRUE;
COLORREF CPolygonTool::c_FillColor = RGB(255,0,0);


//文本默认属性
BOOL CTextTool::c_FangXiang = TRUE;
int CTextTool::c_TextHeight = 32;
int CTextTool::c_TextWidth = 32;
int CTextTool::c_JqWidth = 2;
char	CTextTool::c_TextFont[16];
CString CTextTool::c_Text = "文本";
COLORREF CTextTool::c_TextColor = RGB(255,0,0);

//虚线默认属性
int CXLineTool::c_type[4] = {48,48,48,48};
COLORREF CXLineTool::c_color1 = RGB(0,128,255);
COLORREF CXLineTool::c_color2 = RGB(255,128,64);
BOOL CXLineTool::c_blTranslate = FALSE;
int CXLineTool::c_nWidth = 4;

//位图默认属性
int CTyBmpTool::c_Width = 32;
int CTyBmpTool::c_Height = 32;
char CTyBmpTool::c_bmpName[33];
BOOL CTyBmpTool::c_bTranslate;
COLORREF CTyBmpTool::c_TranslateColor=RGB(0,0,0);
BOOL CTyBmpTool::c_blHScale=TRUE;
BOOL CTyBmpTool::c_blVScale=TRUE;					

//矢量图默认属性
int CTyVectorTool::c_Width = 32;
int CTyVectorTool::c_Height = 32;
char CTyVectorTool::c_vectorName[33];
BOOL CTyVectorTool::c_bTranslate;
COLORREF CTyVectorTool::c_TranslateColor = RGB(0,0,0);

//state property
int			CTyStateTool::c_Width = 136;
int			CTyStateTool::c_Height =32;
CString		CTyStateTool::c_strTextName =NULL;
BOOL		CTyStateTool::c_bTranslate = TRUE;
COLORREF	CTyStateTool::c_TextColor = RGB(0,0,0);
COLORREF	CTyStateTool::c_TranslateColor =RGB(128,45,36);

CTyTool::CTyTool(DRAW_TY type)
{
	m_drawTy = type;
	c_tools.AddTail(this);
}

CTyTool::~CTyTool()
{

}

void CTyTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	pView->SetCapture();
	c_nDownFlags = nFlags;
	c_down = point;
	c_last = point;
}

void CTyTool::OnLButtonDblClk(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

}

void CTyTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ReleaseCapture();

	if (point == c_down)
	{
		c_drawTy = tySelect;
	}
}

void CTyTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	c_last = point;
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

CTyTool* CTyTool::FindTool(DRAW_TY type)
{
	POSITION pos = c_tools.GetHeadPosition();
	while (pos != NULL)
	{
		CTyTool* pTool = (CTyTool*)c_tools.GetNext(pos);
		if (pTool->m_drawTy == type)
			return pTool;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
// CSelectTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelectTool::CSelectTool():CTyTool(tySelect)
{

}

CSelectTool::~CSelectTool()
{

}


void CSelectTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);

	PointStruct local = pView->LPtoUP(point);

	CTyBase* pObj = NULL;
	selectMode = none;

	// Check for resizing (only allowed on single selections)
	if (pView->m_selection.GetCount() == 1){
		pObj = pView->m_selection.GetHead();
		nDragHandle = pObj->HitTest(local, pView, TRUE);
		if (nDragHandle != 0){
			selectMode = size;
		}
	}

	// See if the click was on an object, select and start move if so
	if (selectMode == none)
	{
		CHuaMian* pHuamian = CHMProject::getInstance()->getCurActiveHuamianInstance();
		if (pHuamian==NULL){
			return;
		}
		CHMLayer* pLayer =NULL;
		POSITION pos= pHuamian->m_HMLayerList.GetHeadPosition();
		while (pos!=NULL){
			pLayer = pHuamian->m_HMLayerList.GetNext(pos);
			if (pLayer != NULL){
				pObj = pLayer->ObjectAt(local,pView->LLtoUL(OFFSETX));
				if (!pObj){
					continue;
				}
				CTyHMProxy* pProxy = dynamic_cast<CTyHMProxy*>(pObj);
				if (pProxy!=NULL){
					CString strName;
					strName.Format("%s",pProxy->m_hmname);
					CHMProject::getInstance()->OpenHM(FALSE,strName);
					//adjust for fix bug: can not open hua mian.
					break;
				}
			}
		}
	}
	lastPoint = local;
	CTyTool::OnLButtonDown(pView, nFlags, point);
}

void CSelectTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	if (pView->GetCapture() == pView)
	{
		if (selectMode == netSelect)
		{
			CClientDC dc(pView);
			CRect rect(c_down.x, c_down.y, c_last.x, c_last.y);
			rect.NormalizeRect();
			dc.DrawFocusRect(rect);

			pView->SelectWithinRect(rect, TRUE);
		}
		else if (selectMode != none)
		{
			pView->GetDocument()->UpdateAllViews(pView);
		}
	}

	selectMode = none;

	CTyTool::OnLButtonUp(pView, nFlags, point);
}

void CSelectTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	PointStruct local = pView->LPtoUP(point);
	CTyBase * pObj;

	if (pView->GetCapture() != pView)
	{
		if (c_drawTy == tySelect && pView->m_selection.GetCount() == 1)
		{
			pObj = pView->m_selection.GetHead();
			int nHandle = pObj->HitTest(local, pView, TRUE);
			if (nHandle != 0)
			{
				SetCursor(pObj->GetHandleCursor(nHandle));
				return;
			}
		}
		if (c_drawTy == tySelect)
		{
			CTyTool::OnMouseMove(pView, nFlags, point);
		}

		return;
	}

	if (selectMode == netSelect)
	{
		CClientDC dc(pView);
		CRect rect(c_down.x, c_down.y, c_last.x, c_last.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		rect.SetRect(c_down.x, c_down.y, point.x, point.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);

		CTyTool::OnMouseMove(pView, nFlags, point);

		return;
	}

	float distX,distY;
	distX = local.x-lastPoint.x;
	distY = local.y-lastPoint.y;

	POSITION pos = pView->m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pObj = pView->m_selection.GetNext(pos);

		if (selectMode == move)
		{
			pObj->Move(distX,distY,pView);
		}
		else if (nDragHandle != 0)
		{
			pObj->MoveHandleTo(nDragHandle, local, pView,nFlags);
		}
	}

	if (pView->m_selection.GetCount()>0)
	{
		pView->GetDocument()->SetModifiedFlag(TRUE);
	}

	lastPoint = local;

	if (selectMode == size && c_drawTy == tySelect)
	{
		c_last = point;
		SetCursor(pView->m_selection.GetHead()->GetHandleCursor(nDragHandle));
		return;
	}

	c_last = point;

	if (c_drawTy == tySelect)
	{
		CTyTool::OnMouseMove(pView, nFlags, point);
	}
}

void CSelectTool::OnLButtonDblClk(CElecMapView  *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);

	CTyBase* pTy = NULL;
	PointStruct local = pView->LPtoUP(point);

	if (pView->m_selection.GetCount() == 1)
	{
		pTy = pView->m_selection.GetHead();
		if (pTy->IsKindOf(RUNTIME_CLASS(CPolygon)))
		{
			CPolygon * pPoly = (CPolygon *)pTy;
			int nDragHandle = pTy->HitTest(local, pView, TRUE);
			if (nDragHandle != 0)
			{
				if ((nFlags & MK_SHIFT) != 0)
				{
					pPoly->ClonePoint(nDragHandle,pView);
				}
				else 
				{
					pPoly->DeletePoint(nDragHandle,pView);
				}
				return;
			}
		}

		if (pTy->IsKindOf(RUNTIME_CLASS(CTyState)))
		{
			CHuaMian* pHuamian = CHMProject::getInstance()->GetHMObject("NanJing CityHM");
			if (!pHuamian){
				return;
			}
			CHMProject::getInstance()->setCurActiveHuamianInstance(pHuamian);
			CHMProject::getInstance()->OpenHM(FALSE, "NanJing CityHM", "NanJing CityHMLayer");
			pView->Invalidate(TRUE);
		}
	}

	if ((nFlags & MK_SHIFT) == 0)
	{
		// Shift+DblClk deselects object...
		CHMLayer *pLayer = pView->GetActiveHMLayer();
		if (pLayer != NULL)
		{
			pTy = pLayer->ObjectAt(local,pView->LLtoUL(OFFSETX));
		}

		if (pTy != NULL)
		{
			pView->Deselect(pTy);
		}
	}
	else
	{
		// "Normal" DblClk opens properties, or OLE server...
		if (pView->m_selection.GetCount() == 1)
		{
			pView->m_selection.GetHead()->OnEditProperties(pView);
		}
	}

	CTyTool::OnLButtonDblClk(pView, nFlags, point);
}


//////////////////////////////////////////////////////////////////////
// CLineTool
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineTool::CLineTool():CTyTool(tyLine)
{

}

CLineTool::~CLineTool()
{

}

void CLineTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local = pView->LPtoUP(point);
	CLine* pTy = new CLine(local.x,local.y,local.x,local.y,&c_LogPen);

	CHMLayer* pLayer = pView->GetActiveHMLayer();
	ASSERT(pLayer != NULL);

	pLayer->Add(pTy);
	pView->Select(pTy,FALSE);

	selectMode = size;
	nDragHandle = 2;
	lastPoint = local;
}

void CLineTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);

	if (point == c_down)
	{
		// Don't create empty objects...
		CTyBase *pTy = pView->m_selection.GetTail();
		CHMLayer* pLayer = pView->GetActiveHMLayer();
		ASSERT(pLayer!=NULL);
		
		pLayer->Remove(pTy);
		pView->Select(NULL,FALSE);

		delete pTy;

		SelectTool.OnLButtonDown(pView, nFlags, point);
	}

	SelectTool.OnLButtonUp(pView, nFlags, point);
}

void CLineTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	SelectTool.OnMouseMove(pView, nFlags, point);
}



//////////////////////////////////////////////////////////////////////
// CRectangleTool
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRectangleTool::CRectangleTool(DRAW_TY type):CTyTool(type)
{

}

CRectangleTool::~CRectangleTool()
{

}

void CRectangleTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local = pView->LPtoUP(point);
	CRectangle* pTy = new CRectangle(local.x,local.y,local.x,local.y,&c_LogPen,c_bFill,c_FillColor);
	
	switch (c_drawTy)
	{
	case tyRect:
		pTy->m_rtType = CRectangle::rtRect;
		break;
	case tyRoundRect:
		pTy->m_rtType = CRectangle::rtRoundRect;
		break;
	case tyEllipse:
		pTy->m_rtType = CRectangle::rtEllipse;
		break;
	default:
		ASSERT(FALSE);
	}

	CHMLayer* pLayer = pView->GetActiveHMLayer();
	ASSERT(pLayer != NULL);

	pLayer->Add(pTy);
	pView->Select(pTy,FALSE);

	selectMode = size;
	nDragHandle = 5;
	lastPoint = local;
}

void CRectangleTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);

	if (point == c_down)
	{
		// Don't create empty objects...
		CTyBase *pTy = pView->m_selection.GetTail();
		CHMLayer* pLayer = pView->GetActiveHMLayer();
		ASSERT(pLayer != NULL);
		pLayer->Remove(pTy);
		pView->Select(NULL,FALSE);
		delete pTy;

		SelectTool.OnLButtonDown(pView, nFlags, point);
	}

	SelectTool.OnLButtonUp(pView, nFlags, point);
}

void CRectangleTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	SelectTool.OnMouseMove(pView, nFlags, point);
}

//////////////////////////////////////////////////////////////////////
// CZoomOutTool Class
//////////////////////////////////////////////////////////////////////
float maxviewscale;	//最大比例
float minviewscale;	//最小比例

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZoomOutTool::CZoomOutTool():CTyTool(tyZoomOut)
{

}

CZoomOutTool::~CZoomOutTool()
{

}

//////////////////////////////////////////////////////////////////////
// CZoomInTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZoomInTool::CZoomInTool():CTyTool(tyZoomIn)
{

}

CZoomInTool::~CZoomInTool()
{

}

void CZoomOutTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);

	selectMode = none;
	
	CHMLayer* pLayer = pView->GetActiveHMLayer();
	if (pLayer != NULL)
	{
		maxviewscale = pLayer->m_endViewBL;
		minviewscale = pLayer->m_startViewBL;
	}
	else 
	{
		CHMProject::getInstance()->getCurActiveHuamianInstance()->GetScaleRange(&minviewscale,&maxviewscale);
	}

	if (pView->getCurrentScale() >= maxviewscale) 
		return;

	selectMode = netSelect;

	CClientDC dc(pView);
	CRect rect(point.x, point.y, point.x, point.y);
	rect.NormalizeRect();
	dc.DrawFocusRect(rect);

	CTyTool::OnLButtonDown(pView, nFlags, point);
	SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOM_OUT));
}

void CZoomOutTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);

	if (selectMode == netSelect) 
	{
		CClientDC dc(pView);
		CRect rect(c_down.x, c_down.y, c_last.x, c_last.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		rect.SetRect(c_down.x, c_down.y, point.x, point.y);
		rect.NormalizeRect();	
		dc.DrawFocusRect(rect);
	}

	c_last = point;
	SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOM_OUT));
}

void CZoomOutTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);

	float scale,k=1.5;
	CRect rect,rt;

	if (selectMode == netSelect) 
	{
		CClientDC dc(pView);
		rect.SetRect(c_down.x, c_down.y, c_last.x, c_last.y);	
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
    
		pView->GetClientRect(&rt);

		CSize sz = c_last-c_down;

 		if ((sz.cx > 4)||(sz.cy > 4))
		{
			pView->GetClientRect(&rt);
			k = __min((float)rt.Width()/(float)rect.Width(),(float)rt.Height()/(float)rect.Height());
		} 

		scale = pView->getCurrentScale()*k;//>m_blc

		if (scale>maxviewscale) 
			scale=maxviewscale;

		PointStruct pt = pView->LPtoUP(CPoint((rect.left+rect.right)/2,(rect.top+rect.bottom)/2));

		pView->setCurrentScale(scale);// = scale;
		PointStruct ct = pView->LPtoUP(CPoint((rt.left+rt.right)/2,(rt.top+rt.bottom)/2));

		pView->setStartPointX(pView->getStartPointX() + pt.x-ct.x);//m_xStart
		pView->setStartPointY(pView->getStartPointY() + pt.y-ct.y);//m_yStart

		pView->OnHMSizeChanged();
	}

	selectMode = none;
	ReleaseCapture();

	SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOM_OUT));
}

void CZoomInTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);

	selectMode = none;
	
	CHMLayer* pLayer = pView->GetActiveHMLayer();
	if (pLayer != NULL)
	{
		maxviewscale = pLayer->m_endViewBL;
		minviewscale = pLayer->m_startViewBL;
	}
	else 
	{
		CHMProject::getInstance()->getCurActiveHuamianInstance()->GetScaleRange(&minviewscale,&maxviewscale);
		//pView->getHuamianInstance()->GetScaleRange(&minviewscale,&maxviewscale);
	}

	if (pView->getCurrentScale() <= minviewscale) //m_blc
		return;

	float scale,k = 0.75;

	scale = pView->getCurrentScale()*k;//m_blc
	if (scale < minviewscale) scale = minviewscale;

	CRect rt;
	pView->GetClientRect(&rt);
	PointStruct pt = pView->LPtoUP(point);

	//pView->m_blc = scale;
	pView->setCurrentScale(scale);
	PointStruct ct = pView->LPtoUP(CPoint((rt.left+rt.right)/2,(rt.top+rt.bottom)/2));

	//pView->m_xStart += pt.x-ct.x;
	//pView->m_yStart += pt.y-ct.y;
	pView->setStartPointX(pView->getStartPointX() + pt.x - ct.x);
	pView->setStartPointY(pView->getStartPointY() + pt.y - ct.y);

	pView->OnHMSizeChanged();

	SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOM_IN));

}

void CZoomInTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOM_IN));
}

void CZoomInTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOM_IN));
}

//////////////////////////////////////////////////////////////////////
// CXLineTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXLineTool::CXLineTool()
	:CTyTool(tyXX)
{

}

CXLineTool::~CXLineTool()
{

}

void CXLineTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);

	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local = pView->LPtoUP(point);

	CXLine* pTy = new CXLine(local.x,local.y,local.x,local.y,c_color1,c_color2,c_blTranslate,c_nWidth,c_type);

	CHMLayer* pLayer = pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	
	pLayer->Add(pTy);
	pView->Select(pTy,FALSE);

	selectMode = size;
	nDragHandle = 2;
	lastPoint = local;
}

void CXLineTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	SelectTool.OnMouseMove(pView, nFlags, point);
}

void CXLineTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);

	if (point == c_down)
	{
		// Don't create empty objects...
		CTyBase *pTy = pView->m_selection.GetTail();
		CHMLayer* pLayer = pView->GetActiveHMLayer();
		ASSERT(pLayer != NULL);
		pLayer->Remove(pTy);
		pView->Select(NULL,FALSE);
		delete pTy;

		SelectTool.OnLButtonDown(pView, nFlags, point); // try a select!
	}

	SelectTool.OnLButtonUp(pView, nFlags, point);
}



//////////////////////////////////////////////////////////////////////
// CPolygonTool
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPolygonTool::CPolygonTool(DRAW_TY type):CTyTool(type)
{
	m_pTy = NULL;
}

CPolygonTool::~CPolygonTool()
{

}

void CPolygonTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);

	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local = pView->LPtoUP(point);
	if (m_pTy == NULL)
	{
	  m_pTy = new CPolygon(local.x,local.y,local.x,local.y,&c_LogPen,c_bFill,c_FillColor);
		switch (c_drawTy)
		{
		case tyDBX:
			m_pTy->m_PolyType = CPolygon::Polygon;
			break;
		case tyZX:
			m_pTy->m_PolyType = CPolygon::Polyline;
			break;
		default:
			ASSERT(FALSE);
			break;
		}

		CHMLayer* pLayer = pView->GetActiveHMLayer();
		ASSERT(pLayer != NULL);

		pLayer->Add(m_pTy);
		pView->Select(m_pTy,FALSE);

	} 
	else 
	{
		m_pTy->AddPoint(local,pView);     
	}

	selectMode = size;
	nDragHandle = m_pTy->GetHandleCount();
	lastPoint = local;
}

void CPolygonTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

}


void CPolygonTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	SelectTool.OnMouseMove(pView, nFlags, point);
}


void CPolygonTool::OnLButtonDblClk(CElecMapView* pView, UINT nFlags, const CPoint& point)
{
	ReleaseCapture();
	m_pTy->m_nPoints--;
	pView->InvalidateTy(m_pTy);

	if (m_pTy->m_nPoints <= 2)
	{
		CHMLayer* pLayer = pView->GetActiveHMLayer();
		ASSERT(pLayer != NULL);
		pLayer->Remove(m_pTy);
		pView->Select(NULL,FALSE);
		delete m_pTy;
	}

	m_pTy = NULL;
	c_drawTy = tySelect;
}


//////////////////////////////////////////////////////////////////////
// CTextTool
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextTool::CTextTool(DRAW_TY type):CTyTool(type)
{
	strcpy(c_TextFont,"宋体");
}

CTextTool::~CTextTool()
{

}

void CTextTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local = pView->LPtoUP(point);
	float xLen,yLen,jq;

	xLen = pView->LLtoUL(c_TextWidth);
	yLen = pView->LLtoUL(c_TextHeight);
	jq = pView->LLtoUL(c_JqWidth);

	CText* pTy = new CText(local.x,local.y,xLen,yLen,c_FangXiang,jq,c_TextFont,c_TextColor,c_Text);
	
	switch (c_drawTy)
	{
	case tyText:
		pTy->m_TextType=CText::txtStatic;
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	CHMLayer* pLayer = pView->GetActiveHMLayer();
	ASSERT(pLayer != NULL);

	pLayer->Add(pTy);
	pView->Select(pTy,FALSE);

	pTy->OnEditProperties(pView);
	c_drawTy = tySelect;

}

void CTextTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);

	SelectTool.OnLButtonUp(pView, nFlags, point);
	c_drawTy = tySelect;
}

void CTextTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	SelectTool.OnMouseMove(pView, nFlags, point);
}

//////////////////////////////////////////////////////////////////////
// CTyBmpTool
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTyBmpTool::CTyBmpTool():CTyTool(tyBmp)
{
	ZeroMemory(CTyBmpTool::c_bmpName,sizeof(char)*33);
}

CTyBmpTool::~CTyBmpTool()
{

}

void CTyBmpTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local =pView->LPtoUP(point);
	CPoint pt(point.x+c_Width,point.y+c_Height);
	PointStruct fp = pView->LPtoUP(pt);
	CTyBmp* pTy = new CTyBmp(local.x,local.y,fp.x,fp.y,c_bmpName,c_bTranslate,c_TranslateColor,c_blHScale,c_blVScale);

	CHMLayer* pLayer = pView->GetActiveHMLayer();
	ASSERT(pLayer != NULL);

	pLayer->Add(pTy);
	pView->Select(pTy,FALSE);
	pTy->OnEditProperties(pView);
	c_drawTy = tySelect;
}

void CTyBmpTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);

	c_drawTy = tySelect;

	SelectTool.OnLButtonUp(pView, nFlags, point);
}

void CTyBmpTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	SelectTool.OnMouseMove(pView, nFlags, point);
}

void CTyBmpTool::OnLButtonDblClk(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView);
	SelectTool.OnLButtonDblClk(pView,nFlags,point);
}

//////////////////////////////////////////////////////////////////////
// CTyVectorTool
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTyVectorTool::CTyVectorTool():CTyTool(tyVector)
{
	ZeroMemory(CTyVectorTool::c_vectorName,sizeof(char)*33);
}

CTyVectorTool::~CTyVectorTool()
{

}

void CTyVectorTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);

	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local = pView->LPtoUP(point);
	CPoint pt(point.x+c_Width,point.y+c_Height);
	PointStruct fp = pView->LPtoUP(pt);
	CTyVector* pTy = new CTyVector(local.x,local.y,fp.x,fp.y,c_vectorName,c_bTranslate,c_TranslateColor);

	CHMLayer* pLayer = pView->GetActiveHMLayer();
	ASSERT(pLayer != NULL);

	pLayer->Add(pTy);
	pView->Select(pTy,FALSE);

	pTy->OnEditProperties(pView);
	c_drawTy = tySelect;
}

void CTyVectorTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL);
	c_drawTy = tySelect;
	SelectTool.OnLButtonUp(pView, nFlags, point);
}

void CTyVectorTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	SelectTool.OnMouseMove(pView, nFlags, point);
}

//////////////////////////////////////////////////////////////////////
// log: add by wx 20150925    for state map
// CTyStateTool                    
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTyStateTool::CTyStateTool()
:CTyTool(tyTransSubstation)
{
	
}

CTyStateTool::~CTyStateTool()
{

}

void CTyStateTool::OnLButtonDown(CElecMapView* pView, UINT nFlags, const CPoint& point)
{
	ASSERT(pView != NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local =pView->LPtoUP(point);
	CPoint pt(point.x+c_Width,point.y+c_Height);
	PointStruct fp = pView->LPtoUP(pt);
	CTyState* pTy = new CTyState(local.x,local.y,fp.x,fp.y,c_strTextName,c_bTranslate\
									,c_TranslateColor,c_TextColor);
	pTy->setScaleConst();

	CHMLayer* pLayer = pView->GetActiveHMLayer();
	ASSERT(pLayer != NULL);

	pLayer->Add(pTy);
	pView->Select(pTy,FALSE);
	pTy->OnEditProperties(pView);
	c_drawTy = tySelect;
}
	
void CTyStateTool::OnLButtonUp(CElecMapView* pView, UINT nFlags, const CPoint& point)
{
	ASSERT(pView != NULL);
	c_drawTy = tySelect;
	SelectTool.OnLButtonUp(pView, nFlags, point);
}

void CTyStateTool::OnMouseMove(CElecMapView* pView, UINT nFlags, const CPoint& point)
{
	SelectTool.OnMouseMove(pView, nFlags, point);
}

void CTyStateTool::OnLButtonDblClk(CElecMapView* pView, UINT nFlags, const CPoint& point)
{
	SelectTool.OnLButtonDblClk(pView,nFlags,point);
	
}