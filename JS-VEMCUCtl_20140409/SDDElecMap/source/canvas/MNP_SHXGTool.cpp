#include "stdafx.h"
#include "canvas/mnp_shxg.h"
#include "canvas/mnp_shxgtool.h"
//#include "Sp_drawView.h"
#include "../../ElecMapView.h"
#include "canvas/HMLayer.h"

CMNP_SHXGTool::CMNP_SHXGTool(void)
:CLinkTool(lnkMNP_SHXG)
{
}

CMNP_SHXGTool::~CMNP_SHXGTool(void)
{
}

void CMNP_SHXGTool::OnLButtonDown(CElecMapView *pView, UINT nFlags,const CPoint &point)
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
					CMNP_SHXG * pMNP=new CMNP_SHXG;
					if (pMNP!=NULL)
					{
						if (pMNP->OnEditProperties())
						{
							pObj->AddLinkObject(pMNP);
							pObj->SetModifiedFlag();
						}
						else delete pMNP;						
					}
				}
			}
		}
	}
	
	c_drawTy = tySelect;
}

void CMNP_SHXGTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

}

void CMNP_SHXGTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}
