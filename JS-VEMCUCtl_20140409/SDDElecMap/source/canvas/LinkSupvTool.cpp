#include "stdafx.h"
#include "canvas/linksupvtool.h"
#include "canvas/linksupervise.h"
//#include "sp_drawView.h"
#include "../../ElecMapView.h"
#include "canvas/HMLayer.h"

CLinkSupvTool::CLinkSupvTool()
:CLinkTool(lnkSupervise)
{
	
}

CLinkSupvTool::~CLinkSupvTool(void)
{

}

void CLinkSupvTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView!=NULL);

	CHMLayer *pLayer;
	PointStruct local;
	CTyBase* pObj=NULL;

	if(pView!=NULL)
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
					CLinkSupervise * pLink=new CLinkSupervise();
					if (pLink!=NULL)
					{
						if (pLink->OnEditProperties())
						{
							pObj->AddLinkObject(pLink);
							pObj->SetModifiedFlag();
						}
						else delete pLink;
					}
				}
			}
		}
	}
	c_drawTy = tySelect;
}

void CLinkSupvTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}
