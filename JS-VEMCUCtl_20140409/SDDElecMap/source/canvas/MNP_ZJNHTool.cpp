#include "stdafx.h"
#include "canvas/mnp_zjnh.h"
#include "canvas/mnp_zjnhtool.h"
//#include "Sp_drawView.h"
#include "../../ElecMapView.h"
#include "canvas/HMLayer.h"

CMNP_ZJNHTool::CMNP_ZJNHTool(void)
:CLinkTool(lnkMNP_ZJNH)
{

}

CMNP_ZJNHTool::~CMNP_ZJNHTool(void)
{

}

void CMNP_ZJNHTool::OnLButtonDown(CElecMapView *pView, UINT nFlags,const CPoint &point)
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
					CMNP_ZJNH * pMNP=new CMNP_ZJNH;
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

void CMNP_ZJNHTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

}

void CMNP_ZJNHTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}
