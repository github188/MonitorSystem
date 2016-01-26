#include "stdafx.h"
#include "canvas/TyStationAssistPngTool.h"
#include "canvas/TyStationAssistPng.h"
#include "canvas/HMLayer.h"
#include "canvas/TyTool.h"

//////////////////////////////////////////////////////////////////////////
//图元工具
extern CSelectTool		    SelectTool;
extern CZoomOutTool	    ZoomOutTool;
extern CZoomInTool		ZoomInTool;
extern CTyStationAssistPngTool TyStationAssistPngTool;

//////////////////////////////////////////////////////////////////////////
int CTyStationAssistPngTool::m_nWidth = 32;
int CTyStationAssistPngTool::m_nHeight = 32;
char CTyStationAssistPngTool::m_szPngName[256] = {0};
BOOL CTyStationAssistPngTool::m_bHScale = TRUE;
BOOL CTyStationAssistPngTool::m_bVScale = TRUE;	
float  CTyStationAssistPngTool::m_fRotateAngle = 0.0f;

//////////////////////////////////////////////////////////////////////////
//CTyStationAssistPngTool 构造函数
CTyStationAssistPngTool::CTyStationAssistPngTool(void):CTyTool(tyStationAssistPng)
{

}

CTyStationAssistPngTool::~CTyStationAssistPngTool(void)
{

}

void CTyStationAssistPngTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	try
	{
		ASSERT(pView != NULL);

		if (pView == NULL)
			return;

		CTyTool::OnLButtonDown(pView, nFlags, point);

		PointStruct Point_LT = pView->LPtoUP(point);
		CPoint pt(point.x+m_nWidth,point.y+m_nHeight);
		PointStruct Point_RB = pView->LPtoUP(pt);

		CTyStationAssistPng* pTy = new CTyStationAssistPng(Point_LT.x,Point_LT.y,Point_RB.x,Point_RB.y,m_szPngName,m_fRotateAngle,m_bHScale,m_bVScale);
		if (pTy == NULL)
			return;

		CHMLayer* pLayer = pView->GetActiveHMLayer();
		ASSERT(pLayer != NULL);

		if (pLayer == NULL) 
			return;

		pLayer->Add(pTy);
		pView->Select(pTy,FALSE);
		pTy->OnEditProperties(pView);
		c_drawTy = tySelect;
	}
	catch(...)
	{

	}
}

void CTyStationAssistPngTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	try
	{
		ASSERT(pView != NULL);

		if (pView == NULL)
			return;

		c_drawTy = tySelect;

		SelectTool.OnLButtonUp(pView, nFlags, point);
	}
	catch(...)
	{

	}
}

void CTyStationAssistPngTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	try
	{
		ASSERT(pView != NULL);

		if (pView == NULL)
			return;

		SelectTool.OnMouseMove(pView, nFlags, point);
	}
	catch (...)
	{
		
	}
}




