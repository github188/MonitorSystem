// SZ_PhotoLinkTool.cpp: implementation of the CSZ_PhotoLinkTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/SZ_PhotoLinkTool.h"
#include "canvas/SZ_PhotoLink.h"
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

CSZ_PhotoLinkTool::CSZ_PhotoLinkTool()
:CLinkTool(lnk_SZ_PHOTO)
{

}

CSZ_PhotoLinkTool::~CSZ_PhotoLinkTool()
{

}

void CSZ_PhotoLinkTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CSZ_PhotoLink* pPhotoLink=new CSZ_PhotoLink("");
					if (pPhotoLink!=NULL)
					{
						if (pPhotoLink->OnEditProperties())
						{
							pObj->AddLinkObject(pPhotoLink);
							pObj->SetModifiedFlag();
						}
						else delete pPhotoLink;
					}
				}
			}
		}
	}

	c_drawTy=tySelect;
}

void CSZ_PhotoLinkTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}
