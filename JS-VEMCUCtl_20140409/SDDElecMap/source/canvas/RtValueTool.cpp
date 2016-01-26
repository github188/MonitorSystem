// RtValueTool.cpp: implementation of the CRtValueTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_drawview.h" 
#include "../../ElecMapView.h"
#include "canvas/HMLayer.h"

#include "canvas/RtValueTool.h"
#include "canvas/YC.h"
#include "canvas/ShuZhi.h"
#include "canvas/YM.h"
#include "canvas/YX.h"
#include "canvas/YDX.h"
#include "canvas/TyDateTime.h"
#include "canvas/TyAnimate.h"
#include "canvas/TyMonNiYK.h"
#include "canvas/JDJH.h"

//潮流量默认参数
int CTyDirectionTool::c_type[4]={48,48,48,48};
COLORREF CTyDirectionTool::c_color1=RGB(0,128,255);
COLORREF CTyDirectionTool::c_color2=RGB(255,128,64);
BOOL CTyDirectionTool::c_blTranslate=FALSE;
int CTyDirectionTool::c_nWidth=4;

//	(local.x,local.y,local.x+h,local.y+h,"",2,"空白画面","空白画面",0xff00,0xff,TRUE,TRUE,h,h,jq,"宋体",FALSE,FALSE);

//画面代理默认参数
int CTyHMProxyTool::c_Height=16;
int CTyHMProxyTool::c_Width=16;
int CTyHMProxyTool::c_display=2;
char CTyHMProxyTool::c_str1[33];
char CTyHMProxyTool::c_str2[33];
COLORREF CTyHMProxyTool::c_Color1=RGB(0,255,0);
COLORREF CTyHMProxyTool::c_Color2=RGB(255,0,0);
BOOL CTyHMProxyTool::c_bFlash=FALSE;
BOOL CTyHMProxyTool::c_FangXiang=FALSE;
float CTyHMProxyTool::c_TextHeight=16;
float CTyHMProxyTool::c_TextWidth=16;
float CTyHMProxyTool::c_JqWidth=2;
char CTyHMProxyTool::c_TextFont[16];
BOOL CTyHMProxyTool::c_bTranslate1=FALSE;
BOOL CTyHMProxyTool::c_bTranslate2=FALSE;


BOOL CNewYCTool::canuse=false;
BOOL CYCTool::canuse=false;
BOOL CYXTool::canuse=false;
BOOL CYMTool::canuse=false;
BOOL CYDXTool::canuse=false;
BOOL CTyAnimateTool::canuse=false;
BOOL CTyDateTimeTool::canuse=false;
Stu_YC CYCTool::m_yc;
Stu_NewYC CNewYCTool::m_yc ;


Stu_YX CYXTool::m_yx;
Stu_YM CYMTool::m_ym;
Stu_YDX CYDXTool::m_ydx;
Stu_TyAnimate CTyAnimateTool::m_animate;
Stu_TyDateTime CTyDateTimeTool::m_TyDateTime;


//模拟控制图元默认参数
int CTyMonNiYKTool::c_Height=16;
int CTyMonNiYKTool::c_Width=16;
int CTyMonNiYKTool::c_display=2;
char CTyMonNiYKTool::c_str1[33];
char CTyMonNiYKTool::c_str2[33];
COLORREF CTyMonNiYKTool::c_Color1=RGB(0,255,0);
COLORREF CTyMonNiYKTool::c_Color2=RGB(255,0,0);
BOOL CTyMonNiYKTool::c_FangXiang=FALSE;
float CTyMonNiYKTool::c_TextHeight=16;
float CTyMonNiYKTool::c_TextWidth=16;
float CTyMonNiYKTool::c_JqWidth=2;
char CTyMonNiYKTool::c_TextFont[16];
BOOL CTyMonNiYKTool::c_bTranslate1=FALSE;
BOOL CTyMonNiYKTool::c_bTranslate2=FALSE;


//////////////////////////////////////////////////////////////////////
// Class CTyDirectionTool
//////////////////////////////////////////////////////////////////////

CTyDirectionTool::CTyDirectionTool()
	:CTyTool(tyCL)
{

}

CTyDirectionTool::~CTyDirectionTool()
{

}

void CTyDirectionTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local =pView->LPtoUP(point);
//	int type[4]={48,48,48,48};

	CTyDirection* pTy = new CTyDirection(local.x,local.y,local.x,local.y,c_color1,c_color2,c_blTranslate,c_nWidth,c_type);

	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);

	selectMode = size;
	nDragHandle = 2;
	lastPoint = local;

}

void CTyDirectionTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    pTool->OnMouseMove(pView, nFlags, point);
}

void CTyDirectionTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);

	if (point == c_down)
	{
		// Don't create empty objects...
		CTyBase *pTy = pView->m_selection.GetTail();
		CHMLayer* pLayer=pView->GetActiveHMLayer();
		ASSERT(pLayer!=NULL);
		pLayer->Remove(pTy);
		pView->Select(NULL,FALSE);
		delete pTy;

		pTool->OnLButtonUp(pView, nFlags, point); // try a select!
	}

	pTool->OnLButtonUp(pView, nFlags, point);

}



//////////////////////////////////////////////////////////////////////
// Class CTyHMProxyTool
//////////////////////////////////////////////////////////////////////

CTyHMProxyTool::CTyHMProxyTool()
	:CTyTool(tyHmProxy)
{
  ZeroMemory(c_str1,sizeof(char)*33);
  ZeroMemory(c_str1,sizeof(char)*33);
  ZeroMemory(c_TextFont,sizeof(char)*16);
  strcpy(c_TextFont,"宋体");
}


CTyHMProxyTool::~CTyHMProxyTool()
{

}

void CTyHMProxyTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local =pView->LPtoUP(point);

	float w,h,textH,textW;
	w=pView->LLtoUL(c_Width);
	h=pView->LLtoUL(c_Height);
	textH=pView->LLtoUL((int)c_TextHeight);
	textW=pView->LLtoUL((int)c_TextWidth);
	float jq=pView->LLtoUL((int)c_JqWidth);

	CTyHMProxy* pTy = new CTyHMProxy(local.x,local.y,local.x+w,local.y+h,"",
												c_display,c_str1,c_str2,c_Color1,c_Color2,c_bFlash,
												c_FangXiang,textH,textW,jq,c_TextFont,c_bTranslate1,c_bTranslate2);

	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);
	pTy->OnEditProperties(pView);
	c_drawTy = tySelect;

}








//////////////////////////////////////////////////////////////////////
// Class CYCTool
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CYCTool::CYCTool()
	:CTyTool(tyYC)
{
	canuse=false;
}

CYCTool::~CYCTool()
{

}

void CYCTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
    ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local =pView->LPtoUP(point);
	CYC* pTy = new CYC(local.x,local.y,pView->LLtoUL(5),pView->LLtoUL(10));

	if (canuse)
	{
		pTy->m_yc=m_yc;
		pTy->m_yc.rect.x0=local.x;
		pTy->m_yc.rect.y0=local.y;
		/* gzj
		pTy->m_yc.fontwidth=pView->LLtoUL(int(m_yc.fontwidth+0.4));
		pTy->m_yc.fontheight=pView->LLtoUL(int(m_yc.fontheight+0.4));
		pTy->m_yc.fontjj=pView->LLtoUL(int(m_yc.fontjj+0.4));
		pTy->m_yc.hintdis=pView->LLtoUL(int(m_yc.hintdis+0.4));
		*/
		pTy->GetRectFromFont();
	}
	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);
	pTy->OnEditProperties(pView);

	selectMode = size;
	nDragHandle = 2;
	lastPoint.x = local.x+100;
	lastPoint.y = local.y+100;
}

void CYCTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
  ASSERT(pView!=NULL);
	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    pTool->OnLButtonUp(pView, nFlags, point);
}

void CYCTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
		pTool->OnMouseMove(pView, nFlags, point);
}

//////////////////////////////////////////////////////////////////////
// Class CYCTool
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CNewYCTool::CNewYCTool()
:CTyTool(tyNewYC)
{
	canuse=false;
}

CNewYCTool::~CNewYCTool()
{

}

void CNewYCTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
    ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local =pView->LPtoUP(point);
	CNewYC* pTy = new CNewYC(local.x,local.y,pView->LLtoUL(5),pView->LLtoUL(10));

	if (canuse)
	{
		pTy->m_yc=m_yc;
		pTy->m_yc.rect.x0=local.x;
		pTy->m_yc.rect.y0=local.y;
		pTy->m_yc.fontwidth=pView->LLtoUL(int(m_yc.fontwidth+0.4));
		pTy->m_yc.fontheight=pView->LLtoUL(int(m_yc.fontheight+0.4));
		pTy->m_yc.fontjj=pView->LLtoUL(int(m_yc.fontjj+0.4));
		pTy->m_yc.hintdis=pView->LLtoUL(int(m_yc.hintdis+0.4));

		pTy->GetRectFromFont();
	}
	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);
	pTy->OnEditProperties(pView);

	selectMode = size;
	nDragHandle = 2;
	lastPoint.x = local.x+100;
	lastPoint.y = local.y+100;
}

void CNewYCTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
  ASSERT(pView!=NULL);
	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    pTool->OnLButtonUp(pView, nFlags, point);
}

void CNewYCTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
		pTool->OnMouseMove(pView, nFlags, point);
}
///////////////////////////////////////////////////////////////
CYMTool::CYMTool()
	:CTyTool(tyYM)

{
	canuse=false;

}

CYMTool::~CYMTool()
{

}

void CYMTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
  ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	
	PointStruct local =pView->LPtoUP(point);
	CYM* pTy = new CYM(local.x,local.y,pView->LLtoUL(5),pView->LLtoUL(10));
	if (canuse)
	{
		pTy->m_ym=m_ym;
		pTy->m_ym.rect.x0=local.x;
		pTy->m_ym.rect.y0=local.y;
		pTy->m_ym.fontwidth=pView->LLtoUL(int(m_ym.fontwidth+0.4));
		pTy->m_ym.fontheight=pView->LLtoUL(int(m_ym.fontheight+0.4));
		pTy->m_ym.fontjj=pView->LLtoUL(int(m_ym.fontjj+0.4));
		pTy->m_ym.hintdis=pView->LLtoUL(int(m_ym.hintdis+0.4));

		pTy->GetRectFromFont();
	}

	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);

	pTy->OnEditProperties(pView);
    c_drawTy = tySelect;

	selectMode = size;
	nDragHandle = 2;
	lastPoint.x = local.x+100;
	lastPoint.y = local.y+100;
}

void CYMTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
    ASSERT(pView!=NULL);

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    	pTool->OnLButtonUp(pView, nFlags, point);

}

void CYMTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    	pTool->OnMouseMove(pView, nFlags, point);
}


CYXTool::CYXTool()
	:CTyTool(tyYX)

{
	canuse=false;

}

CYXTool::~CYXTool()
{

}

void CYXTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
    ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	
	PointStruct local =pView->LPtoUP(point);
	CYX* pTy = new CYX(local.x,local.y,pView->LLtoUL(5),pView->LLtoUL(10));

	if (canuse)
	{
		pTy->m_yx=m_yx;
		pTy->m_yx.rect.x0=local.x;
		pTy->m_yx.rect.y0=local.y;
		if (pTy->m_yx.pickind0==2)
		{
			pTy->m_yx.fontwidth=pView->LLtoUL(int(m_yx.fontwidth+0.4));
			pTy->m_yx.fontheight=pView->LLtoUL(int(m_yx.fontheight+0.4));
			pTy->m_yx.fontjj=pView->LLtoUL(int(m_yx.fontjj+0.4));
			pTy->GetRectFromFont();
		}else{
			pTy->m_yx.rect.x1=local.x+pView->LLtoUL((int)(m_yx.rect.x1+0.4));
			pTy->m_yx.rect.y1=local.y+pView->LLtoUL((int)(m_yx.rect.y1+0.4));
		}
		pTy->RegisterAllPic();
	}

	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);

	pTy->OnEditProperties(pView);
    c_drawTy = tySelect;

	selectMode = size;
	nDragHandle = 2;
	lastPoint.x = local.x+100;
	lastPoint.y = local.y+100;
}

void CYXTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
    ASSERT(pView!=NULL);

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    	pTool->OnLButtonUp(pView, nFlags, point);

}

void CYXTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    	pTool->OnMouseMove(pView, nFlags, point);
}




CYDXTool::CYDXTool()
	:CTyTool(tyYDX)

{
	canuse=false;

}

CYDXTool::~CYDXTool()
{

}

void CYDXTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
    ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	
	PointStruct local =pView->LPtoUP(point);
	CYDX* pTy = new CYDX(local.x,local.y,pView->LLtoUL(5),pView->LLtoUL(10));
	
	if (canuse)
	{
		pTy->m_ydx=m_ydx;
		pTy->m_ydx.rect.x0=local.x;
		pTy->m_ydx.rect.y0=local.y;
		if (pTy->m_ydx.pickind0==2)
		{
			pTy->m_ydx.fontwidth=pView->LLtoUL((int)(m_ydx.fontwidth+0.4));
			pTy->m_ydx.fontheight=pView->LLtoUL((int)(m_ydx.fontheight+0.4));
			pTy->m_ydx.fontjj=pView->LLtoUL((int)(m_ydx.fontjj+0.4));
			pTy->GetRectFromFont();
		}else{
			pTy->m_ydx.rect.x1=local.x+pView->LLtoUL((int)(m_ydx.rect.x1+0.4));
			pTy->m_ydx.rect.y1=local.y+pView->LLtoUL((int)(m_ydx.rect.y1+0.4));
		}
		pTy->RegisterAllPic();
	}

	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);

	pTy->OnEditProperties(pView);
    c_drawTy = tySelect;

	selectMode = size;
	nDragHandle = 2;
	lastPoint.x = local.x+100;
	lastPoint.y = local.y+100;
}

void CYDXTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
    ASSERT(pView!=NULL);

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    	pTool->OnLButtonUp(pView, nFlags, point);

}

void CYDXTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    	pTool->OnMouseMove(pView, nFlags, point);
}


CTyDateTimeTool::CTyDateTimeTool()
	:CTyTool(tyDateTime)

{
	canuse=false;
}

CTyDateTimeTool::~CTyDateTimeTool()
{

}

void CTyDateTimeTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
    ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	
	PointStruct local =pView->LPtoUP(point);
	CTyDateTime* pTy = new CTyDateTime(local.x,local.y,pView->LLtoUL(5),pView->LLtoUL(10));
	if (canuse)
	{
		pTy->m_TyDateTime=m_TyDateTime;
		pTy->m_TyDateTime.rect.x0=local.x;
		pTy->m_TyDateTime.rect.y0=local.y;
		pTy->m_TyDateTime.fontwidth=pView->LLtoUL(int(m_TyDateTime.fontwidth+0.4));
		pTy->m_TyDateTime.fontheight=pView->LLtoUL(int(m_TyDateTime.fontheight+0.4));
		pTy->m_TyDateTime.fontjj=pView->LLtoUL(int(m_TyDateTime.fontjj+0.4));

		pTy->GetRectFromFont();
	}

	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);

	pTy->OnEditProperties(pView);
    c_drawTy = tySelect;

	selectMode = size;
	nDragHandle = 2;
	lastPoint.x = local.x+100;
	lastPoint.y = local.y+100;
}

void CTyDateTimeTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
    ASSERT(pView!=NULL);

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    	pTool->OnLButtonUp(pView, nFlags, point);

}

void CTyDateTimeTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    	pTool->OnMouseMove(pView, nFlags, point);
}


CTyAnimateTool::CTyAnimateTool()
	:CTyTool(tyAnimate)

{
    canuse=false;
}

CTyAnimateTool::~CTyAnimateTool()
{

}

void CTyAnimateTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
    ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local =pView->LPtoUP(point);
	CTyAnimate* pTy = new CTyAnimate(local.x,local.y,local.x+pView->LLtoUL(30),local.y+pView->LLtoUL(30));

	if (canuse)
	{
		pTy->m_animate=m_animate;
		pTy->m_animate.rect.x0=local.x;
		pTy->m_animate.rect.y0=local.y;
		pTy->m_animate.rect.x1=local.x+pView->LLtoUL((int)(m_animate.rect.x1+0.4));
		pTy->m_animate.rect.y1=local.y+pView->LLtoUL((int)(m_animate.rect.y1+0.4));
		pTy->RegistAllPic();
	}
	
	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);
	
	pTy->OnEditProperties(pView);
    c_drawTy = tySelect;

	selectMode = size;
	nDragHandle = 2;
	lastPoint.x = local.x+100;
	lastPoint.y = local.y+100;
}

void CTyAnimateTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
    ASSERT(pView!=NULL);

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    	pTool->OnLButtonUp(pView, nFlags, point);

}

void CTyAnimateTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    	pTool->OnMouseMove(pView, nFlags, point);
}



//////////////////////////////////////////////////////////////////////
// Class CTyMonNiYKTool
//////////////////////////////////////////////////////////////////////

CTyMonNiYKTool::CTyMonNiYKTool()
	:CTyTool(tyMonNiYK)
{
  ZeroMemory(c_str1,sizeof(char)*33);
  ZeroMemory(c_str1,sizeof(char)*33);
  ZeroMemory(c_TextFont,sizeof(char)*16);
  strcpy(c_TextFont,"宋体");
}


CTyMonNiYKTool::~CTyMonNiYKTool()
{

}

void CTyMonNiYKTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local =pView->LPtoUP(point);

	float w,h,textH,textW;
	w=pView->LLtoUL(c_Width);
	h=pView->LLtoUL(c_Height);
	textH=pView->LLtoUL((int)c_TextHeight);
	textW=pView->LLtoUL((int)c_TextWidth);
	float jq=pView->LLtoUL((int)c_JqWidth);

	CTyMonNiYK* pTy = new CTyMonNiYK(local.x,local.y,local.x+w,local.y+h,
												c_display,c_str1,c_str2,c_Color1,c_Color2,
												c_FangXiang,textH,textW,jq,c_TextFont,c_bTranslate1,c_bTranslate2);

	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);
	pTy->OnEditProperties(pView);
	c_drawTy = tySelect;

}

//////////////////////////////////////////////////////////////////////
// CMcCurveTool
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMcCurveTool::CMcCurveTool()
	:CTyTool(tymccurve)

{

}

CMcCurveTool::~CMcCurveTool()
{
	
}

void CMcCurveTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
  ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);
	
	PointStruct local =pView->LPtoUP(point);
	
	int extension=pView->LLtoUL(100);

	PointStruct destclocal;
	destclocal.x=local.x+extension;
	destclocal.y=local.x+extension;
	CMcCurve* pTy = new CMcCurve(local.x,local.y,destclocal.x,destclocal.y);
	
		CString strmodefile;
		strmodefile="C:\\sunpac\\par\\global\\mod\\McCurveMode.ini";
		pTy->mccurvedata.SetPath(strmodefile);
		pTy->mccurvedata.strMcename="名称";
		
		pTy->mccurvedata.ReadCurveData();


	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);

	selectMode = size;
	nDragHandle = 5;
	lastPoint = local;
}

void CMcCurveTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
  ASSERT(pView!=NULL);
   ASSERT(pView!=NULL);

  c_drawTy = tySelect;

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    	pTool->OnLButtonUp(pView, nFlags, point);

}

void CMcCurveTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
		pTool->OnMouseMove(pView, nFlags, point);
}

//////////////////////////////////////////////////////////////////////
// CYMcCurveTool
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CYMcCurveTool::CYMcCurveTool()
:CTyTool(tyYmccurve)
{

}

CYMcCurveTool::~CYMcCurveTool()
{
	
}

void CYMcCurveTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);
	
	PointStruct local =pView->LPtoUP(point);
	
	int extension=pView->LLtoUL(100);

	PointStruct destclocal;
	destclocal.x=local.x+extension;
	destclocal.y=local.x+extension;
	CYMcCurve* pTy = new CYMcCurve(local.x,local.y,destclocal.x,destclocal.y);
	

	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);

	selectMode = size;
	nDragHandle = 5;
	lastPoint = local;
}

void CYMcCurveTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
  ASSERT(pView!=NULL);
   ASSERT(pView!=NULL);

  c_drawTy = tySelect;

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    	pTool->OnLButtonUp(pView, nFlags, point);

}

void CYMcCurveTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
		pTool->OnMouseMove(pView, nFlags, point);
}

 CYcCurveTool:: CYcCurveTool(void)
:CTyTool(tyyccurve)
{
}

 CYcCurveTool::~ CYcCurveTool(void)
{
}
void  CYcCurveTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
  ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);
	
	PointStruct local =pView->LPtoUP(point);
	
	int extension=pView->LLtoUL(100);

	PointStruct destclocal;
	destclocal.x=local.x+extension;
	destclocal.y=local.x+extension;
	CYcCurve* pTy = new CYcCurve(local.x,local.y,destclocal.x,destclocal.y);
	

	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);

	selectMode = size;
	nDragHandle = 5;
	lastPoint = local;
}

void  CYcCurveTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
  ASSERT(pView!=NULL);
   ASSERT(pView!=NULL);

  c_drawTy = tySelect;

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
    	pTool->OnLButtonUp(pView, nFlags, point);

}

void  CYcCurveTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
		pTool->OnMouseMove(pView, nFlags, point);
}
