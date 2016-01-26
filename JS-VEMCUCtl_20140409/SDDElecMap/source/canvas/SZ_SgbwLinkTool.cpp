// SZ_SgbwLinkTool.cpp: implementation of the CSZ_SgbwLinkTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/SZ_SgbwLinkTool.h"
#include "canvas/SZ_SgbwLink.h"
//#include "Sp_drawView.h"
#include "../../ElecMapView.h"
#include "canvas/HMLayer.h"
#include "canvas/LinkTool.h"
#include "canvas/SysLinkTy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSZ_SgbwLinkTool::CSZ_SgbwLinkTool()
:CLinkTool(lnk_SZ_SGBW)
{

}

CSZ_SgbwLinkTool::~CSZ_SgbwLinkTool()
{

}

void CSZ_SgbwLinkTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}

void CSZ_SgbwLinkTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);
	CHMLayer* pLayer;
	PointStruct local;
	CTyBase* pObj=NULL;

	if (pView!=NULL)
	{
		pLayer=pView->GetActiveHMLayer();
		if (pLayer!=NULL)
		{
			local=pView->LPtoUP(point);
			pObj=pLayer->ObjectAt(local,pView->LLtoUL(OFFSETX));
			if (pObj!=NULL)
			{
				if (pObj->Get_LinkTy_Count()<MAXLINKTY_COUNT)				
				{
					CSZ_SgbwLink* pSgbwLink=new CSZ_SgbwLink("");
					if (pSgbwLink!=NULL)
					{
						if (pSgbwLink->OnEditProperties())
						{
							pObj->AddLinkObject(pSgbwLink);
							pObj->SetModifiedFlag();
						}
						else delete pSgbwLink;
					}
				}
			}
		}
	}

	c_drawTy=tySelect;
}
