// CurveTool.cpp: implementation of the CCurveTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_draw.h"
//#include "Sp_drawView.h"
#include "../../ElecMapView.h"
#include "canvas/HMLayer.h"
#include "canvas/CurveTool.h"
#include "canvas/TyTool.h"
#include "canvas/curve.h"
#include "dialog/curconfig.h"
#include "dialog/firstcurconfig.h"
#include "dialog/secondcurconfig.h"
#include "canvas/linkTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CCurveTool::CCurveTool()
           :CLinkTool(lnkCur)
{

}

CCurveTool::~CCurveTool()
{

}

void CCurveTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}

void CCurveTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

}

void CCurveTool::OnLButtonDown(CElecMapView *pView, UINT nFlags,const CPoint &point)
{
	ASSERT(pView != NULL);

	CHMLayer *pLayer;
	PointStruct local;
	CTyBase* pObj=NULL;

	if(pView != NULL)
	{
		pLayer = pView->GetActiveHMLayer();
		if (pLayer != NULL)
		{
			local = pView->LPtoUP(point);
			pObj = pLayer->ObjectAt(local,pView->LLtoUL(OFFSETX));
			if (pObj != NULL)
			{
				if (pObj->Get_LinkTy_Count() < MAXLINKTY_COUNT)
				{
					CCurve * pCur=new CCurve;
					if (pCur != NULL)
					{
						if (pCur->OnEditProperties())
						{
							pObj->AddLinkObject(pCur);
							pObj->SetModifiedFlag();
						}
						else 
						{
							delete pCur;		
						}
					}
				}
			}
		}
	}
	
	c_drawTy = tySelect;
}
