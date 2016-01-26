#include "stdafx.h"
//#include "sp_drawview.h" 
#include "../../ElecMapView.h"
#include "canvas/HMLayer.h"
#include "canvas/ty_ycywtool.h"
#include "canvas/ty_ycyw.h"

CTy_YcYwTool::CTy_YcYwTool(void)
	:CTyTool(tyYcYw)
{
}

CTy_YcYwTool::~CTy_YcYwTool(void)
{
}

void CTy_YcYwTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));

	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
		pTool->OnMouseMove(pView, nFlags, point);
}

void CTy_YcYwTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);
	c_drawTy = tySelect;
	CSelectTool* pTool=(CSelectTool*)CTyTool::FindTool(tySelect);
	ASSERT(pTool!=NULL);
	if (pTool!=NULL)
		pTool->OnLButtonUp(pView, nFlags, point);
}

void CTy_YcYwTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);
	CTyTool::OnLButtonDown(pView, nFlags, point);

	PointStruct local =pView->LPtoUP(point);
	float x0=local.x;
	float y0=local.y;
	float x1=local.x+pView->LLtoUL(10);
	float y1=local.y+pView->LLtoUL(10);
	CTy_YcYw* pTy = new CTy_YcYw(x0,y0,x1,y1,"");

	CHMLayer* pLayer=pView->GetActiveHMLayer();
	ASSERT(pLayer!=NULL);
	pLayer->Add(pTy);
	
	pView->Select(pTy,FALSE);

	pTy->OnEditProperties(pView);
	c_drawTy = tySelect;
}
