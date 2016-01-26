#include "stdafx.h"
//#include "sp_draw.h"
//#include "Sp_drawView.h"
#include "../../ElecMapView.h"
#include "canvas/HMLayer.h"
#include "canvas/Ticket.h"
#include "canvas/TicketTool.h"
#include "canvas/TyTool.h"
#include "canvas/linkTool.h"


CTicketTool::CTicketTool(void)
:CLinkTool(lnkTicket)
{
}

CTicketTool::~CTicketTool(void)
{
}

void CTicketTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}

void CTicketTool::OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
}

void CTicketTool::OnLButtonDown(CElecMapView *pView, UINT nFlags,const CPoint &point)
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
					CTicket * pTicket=new CTicket;
					if (pTicket!=NULL)
					{
						if (pTicket->OnEditProperties())
						{
							pObj->AddLinkObject(pTicket);
							pObj->SetModifiedFlag();
						}
						else delete pTicket;						
					}
				}
			}
		}
	}
	
	c_drawTy = tySelect;
}

