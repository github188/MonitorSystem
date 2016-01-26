#include "stdafx.h"
#include "canvas/sz_timeyclinktool.h"
//#include "sp_draw.h"
#include "canvas/sz_timeyclink.h"
//#include "Sp_drawView.h"
#include "../../ElecMapView.h"
#include "canvas/HMLayer.h"
#include "canvas/LinkTool.h"
#include "canvas/SysLinkTy.h"

CSZ_TimeycLinkTool::CSZ_TimeycLinkTool(void)
:CLinkTool(lnk_SZ_TIMEYC)
{
}

CSZ_TimeycLinkTool::~CSZ_TimeycLinkTool(void)
{
}

void CSZ_TimeycLinkTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}

void CSZ_TimeycLinkTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CSZ_TimeycLink* pTimeycLink=new CSZ_TimeycLink("");
					if (pTimeycLink!=NULL)
					{
						if (pTimeycLink->OnEditProperties())
						{
							pObj->AddLinkObject(pTimeycLink);
							pObj->SetModifiedFlag();
						}
						else delete pTimeycLink;
					}
				}
			}
		}
	}

	c_drawTy=tySelect;
}
