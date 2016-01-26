#include "stdafx.h"
#include "canvas/linkfiletool.h"
#include "canvas/HMLayer.h"
//#include "sp_drawView.h"
#include "../../ElecMapView.h"
#include "canvas/LinkFile.h"

CLinkFileTool::CLinkFileTool()
:CLinkTool(lnkFile)
{
	
}

CLinkFileTool::~CLinkFileTool()
{
}

void CLinkFileTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	CLinkTool::OnMouseMove(pView,nFlags,point);	
}

void CLinkFileTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkFile * pFile=new CLinkFile;
					if (pFile!=NULL)
					{
						if (pFile->OnEditProperties())
						{
							pObj->AddLinkObject(pFile);
							pObj->SetModifiedFlag();
						}
						else 
							delete pFile;
						}
				}
			}
		}
	}
	c_drawTy = tySelect;
}
