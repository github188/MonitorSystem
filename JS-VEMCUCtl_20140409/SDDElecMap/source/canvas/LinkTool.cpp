// LinkTool.cpp: implementation of the CLinkYkTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_draw.h"
//#include "Sp_drawView.h"
#include "../../ElecMapView.h"
#include "canvas/HMLayer.h"
#include "canvas/LinkTool.h"
#include "canvas/SysLinkTy.h"
#include "canvas/LinkClub.h"
#include "canvas/PropLinkFile.h"
#include "canvas/App_Link_Intf.h"
#include "canvas/linkpowergztj.h"
#include "canvas/linklinegztj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

bool CLinkYkTool::c_bWatchMan=false;							//开关是否采用监护人监护控制方式
bool CLinkYkTool::c_bGroup=false;								//开关是否采用分组控制方式
DWORD CLinkYkTool::c_dwFlag=0;										//开关允许那些组控制

DWORD CLinkWebShotTool::c_webIP=0;
DWORD CLinkWebShotTool::c_serverIP=0;
int   CLinkWebShotTool::c_serverPort=8190;
DWORD CLinkWebShotTool::c_groupIP=0;
int   CLinkWebShotTool::c_groupPort=8191;
int   CLinkWebShotTool::c_unitID=0;
char  CLinkWebShotTool::c_unitName[33];


//////////////////////////////////////////////////////////////////////
// CLinkTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkTool::CLinkTool(DRAW_TY type)
:CTyTool(type)
{

}

CLinkTool::~CLinkTool()
{

}

void CLinkTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CHMLayer *pLayer;
	PointStruct local;
	CTyBase* pObj=NULL;
	BOOL ret=FALSE;
	
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
				   ret=TRUE;
		   }
	   }
	}
	
	if (!ret)
	  SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	else
	  SetCursor(AfxGetApp()->LoadCursor(IDC_LINK));

}

void CLinkTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

}


//////////////////////////////////////////////////////////////////////
// CLinkYkTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkYkTool::CLinkYkTool()
:CLinkTool(lnkYK)
{

}

CLinkYkTool::~CLinkYkTool()
{

}

void CLinkYkTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkYk * pYK=new CLinkYk("",c_bWatchMan,c_bGroup,c_dwFlag);
					if (pYK!=NULL)
					{
						if (pYK->OnEditProperties())
						{
							pObj->AddLinkObject(pYK);
							pObj->SetModifiedFlag();
						}
						else delete pYK;
					}
				}
			}
		}
	}

  c_drawTy = tySelect;

}


void CLinkYkTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkZkTool::CLinkZkTool()
:CLinkTool(lnkZK)
{

}

CLinkZkTool::~CLinkZkTool()
{

}

void CLinkZkTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					 CLinkZk * pZK=new CLinkZk("",false,false,0);
					 if (pZK!=NULL)
					 {
						 if (pZK->OnEditProperties())
						 {
							 pObj->AddLinkObject(pZK);
							 pObj->SetModifiedFlag();
						 }
						 else delete pZK;
					 }
				 }
			 }
		 }
	}

	c_drawTy = tySelect;
}


void CLinkZkTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}


//////////////////////////////////////////////////////////////////////
// CNetColorPowerTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetColorPowerTool::CNetColorPowerTool()
:CLinkTool(lnkNetColorPower)
{

}

CNetColorPowerTool::~CNetColorPowerTool()
{

}

void CNetColorPowerTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					if (pObj->Is_Net_Color_Power()&&(!pObj->Is_In_Net_Color_Power()))
					{
						CNetColorPower * pTy=new CNetColorPower();
						if (pTy!=NULL)
						{
							if (pTy->OnEditProperties())
							{
								pObj->AddLinkObject(pTy);
								pObj->SetModifiedFlag();
							}
							else delete pTy;
						}
					}
				}
			}
		}
	}

	c_drawTy = tySelect;
}


void CNetColorPowerTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CHMLayer *pLayer;
	PointStruct local;
	CTyBase* pObj=NULL;
	BOOL ret=FALSE;
	
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
					if (pObj->Is_Net_Color_Power()&&(!pObj->Is_In_Net_Color_Power()))
						ret=TRUE;
				}
			}
		}
	}
	
	if (!ret)
	  SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	else
	  SetCursor(AfxGetApp()->LoadCursor(IDC_LINK));
}


//////////////////////////////////////////////////////////////////////
// CNetColorVectorTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetColorVectorTool::CNetColorVectorTool()
:CLinkTool(lnkNetColorVect)
{

}

CNetColorVectorTool::~CNetColorVectorTool()
{

}

void CNetColorVectorTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
				   if (pObj->Is_Net_Color_Vector()&&(!pObj->Is_In_Net_Color_Vector()))
				   {
					   CNetColorVector * pTy=new CNetColorVector();
					   if (pTy!=NULL)
					   {
						   if (pTy->OnEditProperties())
						   {
							   pObj->AddLinkObject(pTy);
							   pObj->SetModifiedFlag();
						   }
						   else delete pTy;
					   }
				   }
			   }
		   }
	   }
	}
	c_drawTy = tySelect;
}


void CNetColorVectorTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CHMLayer *pLayer;
	PointStruct local;
	CTyBase* pObj=NULL;
	BOOL ret=FALSE;
	
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
					if (pObj->Is_Net_Color_Vector()&&(!pObj->Is_In_Net_Color_Vector()))
						ret=TRUE;
				}
			}
		}
	}
	
	if (!ret)
	  SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	else
	  SetCursor(AfxGetApp()->LoadCursor(IDC_LINK));
}


//////////////////////////////////////////////////////////////////////
// CNetColorBmpTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetColorBmpTool::CNetColorBmpTool()
:CLinkTool(lnkNetColorBmp)
{

}

CNetColorBmpTool::~CNetColorBmpTool()
{

}

void CNetColorBmpTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					if (pObj->Is_Net_Color_Bmp()&&(!pObj->Is_In_Net_Color_Bmp()))
					{
						CNetColorBitmap * pTy=new CNetColorBitmap();
						if (pTy!=NULL)
						{
							if (pTy->OnEditProperties())
							{
								pObj->AddLinkObject(pTy);
								pObj->SetModifiedFlag();
							}
							else delete pTy;
						}
					}
				}
			}
		}
	}
	c_drawTy = tySelect;
}


void CNetColorBmpTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CHMLayer *pLayer;
	PointStruct local;
	CTyBase* pObj=NULL;
	BOOL ret=FALSE;
	
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
					if (pObj->Is_Net_Color_Bmp()&&(!pObj->Is_In_Net_Color_Bmp()))
						ret=TRUE;
				}
			}
		}
	}
	
	if (!ret)
	  SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	else
	  SetCursor(AfxGetApp()->LoadCursor(IDC_LINK));
}


//////////////////////////////////////////////////////////////////////
// CNetColorYxTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetColorYxTool::CNetColorYxTool()
:CLinkTool(lnkNetColorYx)
{

}

CNetColorYxTool::~CNetColorYxTool()
{

}

void CNetColorYxTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					if (pObj->Is_Net_Color_Yx()&&(!pObj->Is_In_Net_Color_Yx()))
					{
						CNetColorYx * pTy=new CNetColorYx();
						if (pTy!=NULL)
						{
							if (AfxMessageBox("真的要把这个开关作为网络着色层开关吗",MB_YESNO)==IDYES)
							{
								if (pTy->OnEditProperties())
								{
									pObj->AddLinkObject(pTy);
									pObj->SetModifiedFlag();
								}
								else delete pTy;
							} 
							else delete pTy;
						}
					}
				}
			}
		}
	}
	c_drawTy = tySelect;
}


void CNetColorYxTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CHMLayer *pLayer;
	PointStruct local;
	CTyBase* pObj=NULL;
	BOOL ret=FALSE;
	
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
					if (pObj->Is_Net_Color_Yx()&&(!pObj->Is_In_Net_Color_Yx()))
						ret=TRUE;
				}
			}
		}
	}
	
	if (!ret)
	  SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	else
	  SetCursor(AfxGetApp()->LoadCursor(IDC_LINK));
}

//////////////////////////////////////////////////////////////////////
// CLinkYTTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkYTTool::CLinkYTTool()
:CLinkTool(lnkYT)
{

}

CLinkYTTool::~CLinkYTTool()
{

}

void CLinkYTTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);	
}

void CLinkYTTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkYT * pYT=new CLinkYT;
					if (pYT!=NULL)
					{
						if (pYT->OnEditProperties())
						{
							pObj->AddLinkObject(pYT);
							pObj->SetModifiedFlag();
						}
						else delete pYT;
					}
				}
			}
		}
	}
	c_drawTy = tySelect;
}

//////////////////////////////////////////////////////////////////////
// CLinkYZTYTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkYZTYTool::CLinkYZTYTool()
:CLinkTool(lnkYZTY)
{

}

CLinkYZTYTool::~CLinkYZTYTool()
{

}

void CLinkYZTYTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);	
}

void CLinkYZTYTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkYZTY * pYZTY=new CLinkYZTY;
					if (pYZTY!=NULL)
					{
						if (pYZTY->OnEditProperties())
						{
							pObj->AddLinkObject(pYZTY);
							pObj->SetModifiedFlag();
						}
						else delete pYZTY;
					}
				}
			}
		}
	}
	c_drawTy = tySelect;
}

//////////////////////////////////////////////////////////////////////
// CLinkUnitTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkUnitTool::CLinkUnitTool()
:CLinkTool(lnkUnit)
{

}

CLinkUnitTool::~CLinkUnitTool()
{

}

void CLinkUnitTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);	
}

void CLinkUnitTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkUnit * pUnit=new CLinkUnit;
					if (pUnit!=NULL)
					{
						if (pUnit->OnEditProperties())
						{
							pObj->AddLinkObject(pUnit);
							pObj->SetModifiedFlag();
						}
						else delete pUnit;
					}
				}
			}
		}
	}
	c_drawTy = tySelect;
}

CLinkClubTool::CLinkClubTool()
:CLinkTool(lnkClub)
{

}

CLinkClubTool::~CLinkClubTool()
{

}

void CLinkClubTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkClub * pClub=new CLinkClub;
					if (pClub!=NULL)
					{
						if (pClub->OnEditProperties())
						{
							pObj->AddLinkObject(pClub);
							pObj->SetModifiedFlag();
						}
						else delete pClub;
					}
				}
			}
		}
	}
	
	c_drawTy = tySelect;
}


void CLinkClubTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}

//////////////////////////////////////////////////////////////////////
// CLinkNodeTool  Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkNodeTool::CLinkNodeTool()
:CLinkTool(lnkNode)
{

}

CLinkNodeTool::~CLinkNodeTool()
{

}

void CLinkNodeTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL) ;
	CHMLayer *pLayer ;
	PointStruct local ;
	CTyBase *pObj = NULL ;
	
	if(pView != NULL)
	{
		pLayer = pView->GetActiveHMLayer() ;
		if(pLayer != NULL)
		{
			local = pView->LPtoUP(point) ;
			pObj = pLayer->ObjectAt(local ,pView->LLtoUL(OFFSETX)) ;
            if (pObj!=NULL)
			{
				if (pObj->Get_LinkTy_Count()<MAXLINKTY_COUNT)
				{
					CLinkNode * pNode=new CLinkNode ;
					if (pNode!=NULL)
					{
						if (pNode->OnEditProperties())
						{
							pObj->AddLinkObject(pNode);
							pObj->SetModifiedFlag();
						}
						else delete pNode;
					}
				}
			}
		}
	}

	c_drawTy = tySelect;
}

void CLinkNodeTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView ,nFlags ,point) ;
}


//////////////////////////////////////////////////////////////////////
// CLinkNodeTool  Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkLineTool::CLinkLineTool()
:CLinkTool(lnkLine)
{
}

CLinkLineTool::~CLinkLineTool()
{
}

void CLinkLineTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	ASSERT(pView != NULL) ;
	CHMLayer *pLayer ;
	PointStruct local ;
	CTyBase *pObj = NULL ;
	
	if(pView != NULL)
	{
		pLayer = pView->GetActiveHMLayer() ;
		if(pLayer != NULL)
		{
			local = pView->LPtoUP(point) ;
			pObj = pLayer->ObjectAt(local ,pView->LLtoUL(OFFSETX)) ;
            if (pObj!=NULL)
			{
				if (pObj->Get_LinkTy_Count()<MAXLINKTY_COUNT)
				{
					CLinkLine * pLine=new CLinkLine ;
					if (pLine!=NULL)
					{
						if (pLine->OnEditProperties())
						{
							pObj->AddLinkObject(pLine);
							pObj->SetModifiedFlag();
						}
						else delete pLine;
					}
				}
			}
		}
	}
	c_drawTy = tySelect;
}

void CLinkLineTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView ,nFlags ,point) ;
}



//////////////////////////////////////////////////////////////////////
// CLinkTxtFileTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkTxtFileTool::CLinkTxtFileTool()
:CLinkTool(lnkTxt)
{

}

CLinkTxtFileTool::~CLinkTxtFileTool()
{

}

void CLinkTxtFileTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkTxtFile * pLink=new CLinkTxtFile("");
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


void CLinkTxtFileTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	CLinkTool::OnMouseMove(pView,nFlags,point);
}



//////////////////////////////////////////////////////////////////////
// CLinkWebShotTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkWebShotTool::CLinkWebShotTool()
:CLinkTool(lnkWebShot)
{
	memset((unsigned char *)c_unitName,0,sizeof(char)*33);
}

CLinkWebShotTool::~CLinkWebShotTool()
{

}

void CLinkWebShotTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkWebShot * pLink=new CLinkWebShot("",c_webIP,c_serverIP,c_serverPort,c_groupIP,c_groupPort,c_unitID,c_unitName);
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


void CLinkWebShotTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}

//////////////////////////////////////////////////////////////////////
//  CLinkModemTool
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkModemTool::CLinkModemTool()
:CLinkTool(lnkModem)
{
	
}

CLinkModemTool::~CLinkModemTool()
{

}

void CLinkModemTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkModem * pLink=new CLinkModem();
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

void CLinkModemTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}

//////////////////////////////////////////////////////////////////////
// CLinkYT2Tool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkYT2Tool::CLinkYT2Tool()
:CLinkTool(lnkYT2)
{

}

CLinkYT2Tool::~CLinkYT2Tool()
{

}

void CLinkYT2Tool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);	
}

void CLinkYT2Tool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkYT2 * pYT=new CLinkYT2;
					if (pYT!=NULL)
					{
						if (pYT->OnEditProperties())
						{
							pObj->AddLinkObject(pYT);
							pObj->SetModifiedFlag();
						}
						else delete pYT;
					}
				}
			}
		}
	}

	c_drawTy = tySelect;
}

//////////////////////////////////////////////////////////////////////
//  CLinkXYBindTool
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkXYBindTool::CLinkXYBindTool()
:CLinkTool(lnkYXBind)
{
	
}

CLinkXYBindTool::~CLinkXYBindTool()
{

}

void CLinkXYBindTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkYXBind * pLink=new CLinkYXBind();
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


void CLinkXYBindTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	CLinkTool::OnMouseMove(pView,nFlags,point);
}

//////////////////////////////////////////////////////////////////////
//  CLinkQKTool
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkQKTool::CLinkQKTool()
:CLinkTool(lnkQK)
{
	
}

CLinkQKTool::~CLinkQKTool()
{

}

void CLinkQKTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkQK * pLink=new CLinkQK();
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


void CLinkQKTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);
}

//////////////////////////////////////////////////////////////////////
//  CLinkSoftRepairTool
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkSoftRepairTool::CLinkSoftRepairTool()
:CLinkTool(lnkSoftRepair)
{
	
}

CLinkSoftRepairTool::~CLinkSoftRepairTool()
{

}

void CLinkSoftRepairTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkSoftRepair * pLink=new CLinkSoftRepair();
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


void CLinkSoftRepairTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	CLinkTool::OnMouseMove(pView,nFlags,point);
}

//////////////////////////////////////////////////////////////////////
// CLinkUnitExtendTool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkUnitExtendTool::CLinkUnitExtendTool()
:CLinkTool(lnkUnitExtend)
{

}

CLinkUnitExtendTool::~CLinkUnitExtendTool()
{

}

void CLinkUnitExtendTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);	
}

void CLinkUnitExtendTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkUnitExtend * pUnit=new CLinkUnitExtend;
					if (pUnit!=NULL)
					{
						if (pUnit->OnEditProperties())
						{
							pObj->AddLinkObject(pUnit);
							pObj->SetModifiedFlag();
						}
						else delete pUnit;
					}
				}
			}
		}
	}

	c_drawTy = tySelect;
}


//////////////////////////////////////////////////////////////////////
//  CLinkKXTool
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkKXTool::CLinkKXTool()
:CLinkTool(lnkKX)
{
	
}

CLinkKXTool::~CLinkKXTool()
{

}

void CLinkKXTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkKX * pLink=new CLinkKX();
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


void CLinkKXTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	CLinkTool::OnMouseMove(pView,nFlags,point);
}

//////////////////////////////////////////////////////////////////////
//  CLinkKXSectionTool
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkKXSectionTool::CLinkKXSectionTool()
:CLinkTool(lnkKXSection)
{
	
}

CLinkKXSectionTool::~CLinkKXSectionTool()
{

}

void CLinkKXSectionTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkKXSection * pLink=new CLinkKXSection();
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


void CLinkKXSectionTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{

	CLinkTool::OnMouseMove(pView,nFlags,point);
}

/////////////////////////////////////////////////////////////
//水行业控制链接图元
CLinkDoubleCtrlTool::CLinkDoubleCtrlTool()
:CLinkTool(lnkDoubleCtrl)
{

}

CLinkDoubleCtrlTool::~CLinkDoubleCtrlTool()
{

}

void CLinkDoubleCtrlTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);	
}

void CLinkDoubleCtrlTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkDoubleCtrl * pDoubleCtrl = new CLinkDoubleCtrl;
					if (pDoubleCtrl!=NULL)
					{
						if (pDoubleCtrl->OnEditProperties())
						{
							pObj->AddLinkObject(pDoubleCtrl);
							pObj->SetModifiedFlag();
						}
						else delete pDoubleCtrl;
					}
				}
			}
		 }
	}

	c_drawTy = tySelect;
}

CLinkPowerGzTjTool::CLinkPowerGzTjTool()
:CLinkTool(lnkPowerGzTj)
{

}

CLinkPowerGzTjTool::~CLinkPowerGzTjTool()
{

}

void CLinkPowerGzTjTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);	
}

void CLinkPowerGzTjTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkPowerGzTj * pDoubleCtrl = new CLinkPowerGzTj;
					if (pDoubleCtrl!=NULL)
					{
						if (pDoubleCtrl->OnEditProperties())
						{
							pObj->AddLinkObject(pDoubleCtrl);
							pObj->SetModifiedFlag();
						}
						else delete pDoubleCtrl;
					}
				}
			}
		}
	}
	
	c_drawTy = tySelect;
}


CLinkLineGzTjTool::CLinkLineGzTjTool()
:CLinkTool(lnkLineGzTj)
{

}

CLinkLineGzTjTool::~CLinkLineGzTjTool()
{

}

void CLinkLineGzTjTool::OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point)
{
	CLinkTool::OnMouseMove(pView,nFlags,point);	
}

void CLinkLineGzTjTool::OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point)
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
					CLinkLineGzTj * pDoubleCtrl = new CLinkLineGzTj;
					if (pDoubleCtrl!=NULL)
					{
						if (pDoubleCtrl->OnEditProperties())
						{
							pObj->AddLinkObject(pDoubleCtrl);
							pObj->SetModifiedFlag();
						}
						else delete pDoubleCtrl;
					}
				}
			}
		}
	}
	
	c_drawTy = tySelect;
}

