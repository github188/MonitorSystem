#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/sz_sblooklinktool.h"
#include "canvas/sz_sblooklink.h"
//#include "Sp_drawView.h"
#include "../../ElecMapView.h"
#include "canvas/HMLayer.h"
#include "canvas/LinkTool.h"
#include "canvas/SysLinkTy.h"

CSZ_SbLookLinkTool::CSZ_SbLookLinkTool(void)
:CLinkTool(lnk_SZ_SBLOOK)
{
}

CSZ_SbLookLinkTool::~CSZ_SbLookLinkTool(void)
{
}

void CSZ_SbLookLinkTool::OnMouseMove(CElecMapView* pView, UINT nFlags, const CPoint& point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}

void CSZ_SbLookLinkTool::OnLButtonDown(CElecMapView* pView, UINT nFlags, const CPoint& point)
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
					CSZ_SbLookLink* pSbLookLink=new CSZ_SbLookLink("");
					if (pSbLookLink!=NULL)
					{
						if (pSbLookLink->OnEditProperties())
						{
							pObj->AddLinkObject(pSbLookLink);
							pObj->SetModifiedFlag();
						}
						else delete pSbLookLink;
					}
				}
			}
		}
	}

	c_drawTy=tySelect;
}
