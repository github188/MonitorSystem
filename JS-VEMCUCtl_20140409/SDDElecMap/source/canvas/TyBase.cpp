// TyBase.cpp: implementation of the CTyBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "canvas/TyBase.h"
#include <math.h>
#include <float.h>
#include "canvas/HMLayer.h"
#include "canvas/JDJH.h"
#include "canvas/ImageX.h"
#include "canvas/SysLinkTy.h"
#include "canvas/yc.h"
#include "canvas/newyc.h"
#include "canvas/shuzhi.h"
#include "canvas/YDX.h"
#include "canvas/TyDateTime.h"
#include "canvas/yx.h"
#include "canvas/ym.h"
#include "canvas/TyAnimate.h"
#include "canvas/Curve.h"
#include "canvas/SZ_SgbwLink.h"
#include "canvas/SZ_PhotoLink.h"
#include "canvas/LinkClub.h"
#include "canvas/Vector.h"
#include "canvas/App_Link_Intf.h"
#include "canvas/SZ_SbLookLink.h"
#include "canvas/TyMonNiYK.h"
#include "canvas/MNP_SHXG.h"
#include "canvas/MNP_ZJNH.h"
#include "canvas/Ticket.h"
#include "canvas/linkfile.h"
#include "canvas/Ty_YcYw.h"
#include "canvas/SZ_TimeycLink.h"
#include "canvas/linkSupervise.h"
#include "canvas/LinkPowerGzTj.h"
#include "canvas/linklineGzTj.h"
#include "canvas/TyStationAssistPng.h"
#include "canvas/YSH.h"

#include "../../ElecMapDoc.h"
#include "../../ElecMapView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_SERIAL(CTyBase, CObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTyBase::~CTyBase()
{
	int count = m_LinkArr.GetUpperBound();
	for (int i = count; i>= 0;i--)
	{
		ASSERT(m_LinkArr[i] != NULL);
		delete m_LinkArr[i];
	}
	m_LinkArr.RemoveAll();
}

#ifdef _DEBUG

void CTyBase::ASSERTValid()
{
	ASSERT(m_pHMLayer != NULL);
}
#endif

void CTyBase::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{

}

CTyBase::CTyBase(int vId,CHMLayer* pLayer)
{
	m_LinkArr.RemoveAll();
 	m_bDelete = FALSE;
	m_pHMLayer = pLayer;
	m_Id = vId;
	m_dwFlag = 0;
}

float CTyBase::PointLine(float xx, float yy, float x1, float y1, float x2, float y2)
{
	float a,b,c,ang1,ang2,ang;
  
	//计算三条边的距离
	a = CalDisp(x1,y1,xx,yy);
	if(a == 0.0)
		return 0.0;

	b = CalDisp(x2,y2,xx,yy);
	if(b == 0.0)
		return 0.0;

	c = CalDisp(x1,y1,x2,y2);

	//如果（x1,y1)和(x2,y2)是一个点直接返回距离
	if(c==0.0) return a;

	if(a < b) //如果(xx,yy)的点(x1,y1）这条边较短
	{
		if(y1 == y2)
		{
			if(x1 < x2)
				ang1 = 0;
			else
				ang1 = (float)pi;
		}
		else
		{
			ang1 = (float)acos((x2-x1)/c);
			if(y1 > y2)
				ang1 = (float)pi*2-ang1;  //直线(x1,y1)-(x2,y2)的弧度
		}

		ang2 = (float)acos((xx-x1)/a);
		if(y1 > yy)
			ang2 = (float)pi*2-ang2;  //直线(x1,y1)-(xx,yy)的弧度

		ang = ang2-ang1;
		if(ang < 0)
			ang=-ang;

		if(ang > pi) 
			ang = (float)pi*2-ang;  //交角的大小

		if(ang > pi/2)
			return a;    //如果为钝角，直接返回距离
		else 
			return (a*(float)sin(ang)); //否则返回计算得到的距离 
	}
	else //如果(xx,yy)的点(x2,y2）这条边较短
	{
		if(y1 == y2)
		{
			if(x1 < x2)
				ang1 = (float)pi;
			else
				ang1 = 0;
		}
		else
		{
			ang1 = (float)acos((x1-x2)/c);     //直线（x2,y2)-(x1,y1)的斜率的弧度
			if(y2 > y1)
				ang1 = (float)pi*2-ang1;
		}

		ang2 = (float)acos((xx-x2)/b);     //直线（x2,x1）-(xx,yy)的斜率的弧度
		if(y2 > yy)
			ang2 = (float)pi*2-ang2;

		ang = ang2-ang1;
		if(ang < 0) 
			ang=-ang;

		if(ang > pi)
			ang = (float)pi*2-ang;  //交角的大小 ?

		if(ang > pi/2)  
			return b;   //如果为钝角，直接返回距离
		else 
			return(b*(float)sin(ang));  //否则返回计算得到的距离
	}
}

float CTyBase::CalDisp(float x1, float y1, float x2, float y2)
{
	return (float)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

int CTyBase::GetID()
{
	return m_Id;
}

BOOL CTyBase::PointRgn(float x, float y, int Numble, PointStruct *PointList, float blc)
{
	CRgn rgn; 
	int x1,y1,i;
	CPoint bbcc[MAXPOINTNUM];
	
	if(Numble < 3) 
		return 0;//如果点的数目<3即不是一个区域返回不成功标志

	ASSERT(Numble <= MAXPOINTNUM);

	//将点的坐标转变成屏幕坐标
	x1 = (int)(x/blc);
	y1 = (int)(y/blc);

	//将封闭区域各点的坐标转成屏幕坐标
	for(i = 0;i < Numble;i++)
	{
		bbcc[i].x = (int)((PointList[i].x)/blc);
		bbcc[i].y = (int)((PointList[i].y)/blc);
	}

	rgn.CreatePolygonRgn(bbcc,Numble,1);//初试化一个多边形区域
	i = (rgn.PtInRegion(x1,y1)!=0);		//如果在区域内j=1,否则j=0;
	rgn.DeleteObject();					        //删除定义的rgn对象

	return i;
}

void CTyBase::Serialize(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		//ar<<m_Id<<m_iNumber<<m_dwFlag;
		ar<<m_dwFlag;
	}
	else
	{
		//ar>>m_Id>>m_iNumber>>m_dwFlag;
		ar>>m_dwFlag;
	}

	Ty_Serialize(ar);
	LinkObjects_Serialize(ar);
}

void CTyBase::Save(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_Id,sizeof(m_Id));
		file->Write((unsigned char *)&m_iNumber,sizeof(m_iNumber));
		file->Write((unsigned char *)&m_dwFlag,sizeof(m_dwFlag));
	}
	else
	{
		file->Read((unsigned char *)&m_Id,sizeof(m_Id));
		m_Id=0;
		file->Read((unsigned char *)&m_iNumber,sizeof(m_iNumber));
		file->Read((unsigned char *)&m_dwFlag,sizeof(m_dwFlag));
	}
	Ty_Save(file,Yn);
	LinkObjects_Save(file,Yn);
}

CTyBase* CTyBase::Clone()
{
  return NULL;
}

BOOL CTyBase::IntersectRect(float x0, float y0, float x1, float y1)
{
	RectStruct frt1;
	frt1.x0 = __min(x0,x1);
	frt1.x1 = __max(x0,x1);
	frt1.y0 = __min(y0,y1);
	frt1.y1 = __max(y0,y1);

	RectStruct frt2;
	float x2,y2,x3,y3;
	GetRect(&x2,&y2,&x3,&y3);

	frt2.x0 = __min(x2,x3);
	frt2.x1 = __max(x2,x3);
	frt2.y0 = __min(y2,y3);
	frt2.y1 = __max(y2,y3);

	return IntersectRect2(frt1,frt2);
}

void CTyBase::DrawDC(CDC *pDC, CElecMapView *pView)
{

}

void CTyBase::OnEditProperties(CElecMapView * pView)
{

}

void CTyBase::Move(float xLen, float yLen,CElecMapView * pView)
{
	RectStruct rt1,rt2;
	
	GetRect(&rt1.x0,&rt1.y0,&rt1.x1,&rt1.y1);
	
	Ty_Move(xLen,yLen,pView);

	if (m_pHMLayer != NULL)
	{
		GetRect(&rt2.x0,&rt2.y0,&rt2.x1,&rt2.y1);
		m_pHMLayer->OnPositionChangedTy(this,rt1,rt2);
	}
}

BOOL CTyBase::PointInObj(float x, float y,float errRange)
{
	return FALSE;
}

int CTyBase::GetHandleCount()
{
	ASSERT_VALID(this);
	return 8;
}

void CTyBase::Invalidate()
{
	ASSERT_VALID(this);
	CElecMapDoc *pDoc = m_pHMLayer->GetDocument();
	if (pDoc == NULL) return;

	pDoc->UpdateAllViews(NULL, HINT_UPDATE_TYOBJ, this);
}

PointStruct CTyBase::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	float x, y, xCenter, yCenter;
	float x0,y0,x1,y1;

	GetRect(&x0,&y0,&x1,&y1);

	xCenter = x0 + (x1-x0) / 2;
	yCenter = y0 + (y1-y0) / 2;

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		x = x0;
		y = y0;
		break;

	case 2:
		x = xCenter;
		y = y0;
		break;

	case 3:
		x = x1;
		y = y0;
		break;

	case 4:
		x = x1;
		y = yCenter;
		break;

	case 5:
		x = x1;
		y = y1;
		break;

	case 6:
		x = xCenter;
		y = y1;
		break;

	case 7:
		x = x0;
		y = y1;
		break;

	case 8:
		x = x0;
		y = yCenter;
		break;
	}

	PointStruct pt;

	pt.x = x;
	pt.y = y;

	return pt;
}

CRect CTyBase::GetHandleLogRect(int nHandleID, CElecMapView *pView)
{
	ASSERT(pView != NULL);

	PointStruct fpt = GetHandle(nHandleID);
	CPoint pt = pView->UPtoLP(fpt);
	CRect rt;

	rt.left    = pt.x-OFFSETX;
	rt.right = pt.x+OFFSETX;
	rt.top   = pt.y-OFFSETY;
	rt.bottom = pt.y+OFFSETY;
	
	return rt;
}

int CTyBase::HitTest(PointStruct point, CElecMapView *pView, BOOL bSelected)
{
	ASSERT(pView != NULL);

	if (bSelected)
	{
		CPoint pt = pView->UPtoLP(point);
		int nHandleCount = GetHandleCount();
		for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
		{
			CRect rt = GetHandleLogRect(nHandle,pView);
			
			if (rt.PtInRect(pt)) 
				return  nHandle;
		}
	}
	else
	{
		float errL;
		errL = pView->LLtoUL(OFFSETX);

		if (PointInObj(point.x,point.y,errL))
			return 1;
	}
	return 0;

}

void CTyBase::DrawTracker(CDC *pDC, CElecMapView *pView, TrackerState state)
{
	ASSERT_VALID(this);
	ASSERT(pView!=NULL);

	switch (state)
	{
	case normal:
		break;

	case selected:
	case active:
		{
			int nHandleCount = GetHandleCount();
			for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
			{
				CRect rt = GetHandleLogRect(nHandle,pView);
				pDC->PatBlt(rt.left,rt.top, rt.Width(),rt.Height(),DSTINVERT);
			}
		}
		break;
	}

}

void CTyBase::MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT_VALID(this);
	ASSERT(pView!=NULL);

	RectStruct rt1,rt2;
	
	GetRect(&rt1.x0,&rt1.y0,&rt1.x1,&rt1.y1);
	
	Ty_MoveHandleTo(nHandle,point,pView,nFlags);

	if (m_pHMLayer != NULL)
	{
		GetRect(&rt2.x0,&rt2.y0,&rt2.x1,&rt2.y1);
		m_pHMLayer->OnPositionChangedTy(this,rt1,rt2);
	}
}


HCURSOR CTyBase::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);

	LPCTSTR id;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
	case 5:
		id = IDC_SIZENWSE;
		break;

	case 2:
	case 6:
		id = IDC_SIZENS;
		break;

	case 3:
	case 7:
		id = IDC_SIZENESW;
		break;

	case 4:
	case 8:
		id = IDC_SIZEWE;
		break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}


BOOL CTyBase::IntersectRect2(RectStruct rt1,RectStruct rt2)
{
	CRect rect1,rect2;
	float bl1 = 1,bl2 = 1;

	if ((rt1.Width()+rt1.Height()) > 4096) bl1 = (rt1.Width()+rt1.Height())/4096;
	if ((rt2.Width()+rt2.Height()) > 4096) bl2 = (rt2.Width()+rt2.Height())/4096;
	bl1=__max(bl1,bl2);

	rect1.SetRect((int)((rt1.x0+bl1-1)/bl1),(int)((rt1.y0+bl1-1)/bl1),
								(int)((rt1.x1+bl1-1)/bl1),(int)((rt1.y1+bl1-1)/bl1));

	rect2.SetRect((int)((rt2.x0+bl1-1)/bl1),(int)((rt2.y0+bl1-1)/bl1),
								(int)((rt2.x1+bl1-1)/bl1),(int)((rt2.y1+bl1-1)/bl1));

	rect2.InflateRect(4,4);
	rect1.InflateRect(4,4);
	return (rect1.IntersectRect(&rect1,&rect2));
}

BOOL CTyBase::PointInRect(float x, float y, RectStruct rt)
{
	BOOL ret = TRUE; 

	if ((x < __min(rt.x0,rt.x1))||(x > __max(rt.x0,rt.x1))||
		 (y < __min(rt.y0,rt.y1))||(y > __max(rt.y0,rt.y1)))
		ret = FALSE;

	return ret;
}

DRAW_TY CTyBase::GetTyType()
{
	return tyNone;
}

CTyBase* CTyBase::CreateTy(DRAW_TY tyStyle)
{
	CTyBase* pTy = NULL;

	switch (tyStyle)
	{
	case tyLine:
		pTy = new CLine;
    break;
	case tyXX:
		pTy = new CXLine;
		break;
	case tyRect:
	case tyRoundRect:
	case tyEllipse:
		pTy = new CRectangle();
		break;
	case tyDBX:
	case tyZX:
		pTy = new CPolygon();
		break;
	case tyText:
	  pTy = new CText();
	  break;
	case tyBmp:
		pTy = new CTyBmp;
		break;
	case tyVector:
		pTy = new CTyVector;
		break;
	case tyYC:
		pTy = new CYC;
		break;
	case tyNewYC:
		pTy = new CNewYC ;
		break ;
	case tyCL:
		pTy = new CTyDirection;
		break;
	case tyHmProxy:
		pTy = new CTyHMProxy;
		break;
	case tyYDX:
		pTy = new CYDX;
		break;
	case tyDateTime:
		pTy = new CTyDateTime;
		break;
	case tyYX:
		pTy = new CYX;
		break;
	case tyCamera://add by 20151208
		pTy = new CYSH;
		break;
	case tyYM:
		pTy = new CYM;
		break;
	case tyAnimate:
		pTy = new CTyAnimate;
		break;
	case tyMonNiYK:
		pTy= new CTyMonNiYK;
		break;
	case tymccurve:
		pTy = new CMcCurve;
		break;
	case tyYcYw:
		pTy = new CTy_YcYw();
		break;
	case tyYmccurve://累加量曲线
		pTy = new CYMcCurve ;
		break;
	case tyyccurve:
		pTy = new CYcCurve; 
		break ;
	case tyStationAssistPng:
		pTy = new CTyStationAssistPng;
		break;
	default:
		{
			CString str,tempstr;
			//tempstr.LoadString(IDS_ERR_TYSTYLE);
			str.Format("%s %d",tempstr,tyStyle);
			AfxMessageBox(str);
			AfxThrowFileException(CFileException::generic);
		}
		break;
	}

	return pTy;
}

BOOL CTyBase::URectInURect(RectStruct rt1, RectStruct rt2)
{
	BOOL ret = (rt1.x0 >= rt2.x0)&&(rt1.y0 >= rt2.y0)&&(rt1.x1 <= rt2.x1)&&(rt1.y1 <= rt2.y1);
	if (ret)
		 return TRUE;
	else 
		return FALSE;
}

void CTyBase::SetModifiedFlag(BOOL bl)
{
	if (m_pHMLayer == NULL)
		return;

	CElecMapDoc * pDoc = m_pHMLayer->GetDocument();
	if (pDoc != NULL)
	{
		pDoc->SetModifiedFlag(bl);
	}
}

BOOL CTyBase::OnEditLinkObjectProperties(int pos)
{
	ASSERT(pos >= 0);
	ASSERT(pos <= m_LinkArr.GetUpperBound());

	CLinkObjectBase* pLink;
	BOOL ret = FALSE;

	pLink = m_LinkArr[pos];
	ASSERT(pLink != NULL);
	if (pLink != NULL)
	{
		ret = pLink->OnEditProperties();
		if (ret)
		{
			SetModifiedFlag();
		}
	}

	 return ret;
}

BOOL CTyBase::ExecuteLinkObject(int pos1,int pos2 )
{
	ASSERT((pos1 >= 0)&&(pos1 <= m_LinkArr.GetUpperBound()));
	ASSERT(m_LinkArr[pos1] != NULL);

	m_LinkArr[pos1]->Execute(pos2);

	return TRUE;
}

void CTyBase::LinkObjects_Save(CFile *file, BOOL Yn)
{
	ASSERT(file != NULL);

	int i,count;
	CLinkObjectBase * pLink;

	if (Yn)
	{
		count = m_LinkArr.GetUpperBound()+1;
		file->Write((unsigned char*)&count,sizeof(count));
		for (i = 0;i <= count-1;i++)	
		{
			pLink = m_LinkArr[i];
			ASSERT(pLink != NULL);
			if (pLink != NULL)
			{
				file->Write((unsigned char *)&pLink->m_key,sizeof(int));
				pLink->Save(file,Yn);
			}
		}
	}
	else 
	{
		int linkKey;
		file->Read((unsigned char*)&count,sizeof(count));
		for (int i = 0;i < count;i++)
		{
			file->Read((unsigned char *)&linkKey,sizeof(int));
			switch(linkKey)
			{
			case lnkYK:
				pLink = new CLinkYk();
				break;
			case lnkNetColorPower:
				pLink = new CNetColorPower();
				break;	 
			case lnkNetColorYx:
				pLink = new CNetColorYx();
				break;	 
			case lnkNetColorVect:
				pLink = new CNetColorVector();
				break;	 
			case lnkNetColorBmp:
				pLink = new CNetColorBitmap();
				break;	 
			case lnkYZTY:
				pLink = new CLinkYZTY();
				break;	 
			case lnkYT:
				pLink = new CLinkYT();
				break;	 
			case lnkCur:
				pLink = new CCurve();
				break;	 
			case lnk_SZ_SGBW:
				pLink = new CSZ_SgbwLink();
				break;	 
			case lnk_SZ_PHOTO:
				pLink = new CSZ_PhotoLink();
				break;
			case lnkClub:
				pLink = new CLinkClub();
				break;	 
			case lnkUnit:
				pLink = new CLinkUnit();
				break;	 
			case lnkTxt:
				pLink = new CLinkTxtFile();
				break;
			case lnkWebShot:
				pLink = new CLinkWebShot();
				break;
			case lnk_SZ_SBLOOK:
				pLink =new CSZ_SbLookLink();
				break;
			case lnkMNP_SHXG:
				pLink =new CMNP_SHXG();
				break;
			case lnkMNP_ZJNH:
				pLink =new CMNP_ZJNH();
				break;
			case lnkTicket:
				pLink = new CTicket();
			    break;
			case lnkNode:
				pLink = new CLinkNode() ;
				break; 
			case lnkLine:
				pLink = new CLinkLine() ;
				break; 
			case lnkFile:
				pLink = new CLinkFile();
				break;
			case lnk_SZ_TIMEYC:
				pLink = new CSZ_TimeycLink();
				break;
			//拨号图元
			case lnkModem:
				pLink = new CLinkModem() ;
				break;
			case lnkYT2:
				pLink = new CLinkYT2();
				break;	 
			case lnkYXBind:
				pLink = new CLinkYXBind() ;
				break ;
			case lnkYSHBind://add by wx 20151202
				pLink = new CLinkYSHBind();
				break;
			case lnkQK:
				pLink = new CLinkQK();
				break ;
			case lnkSoftRepair:
				pLink = new CLinkSoftRepair() ;
				break ;
			case lnkUnitExtend:
				pLink = new CLinkUnitExtend() ;
				break ;
			case lnkKX:
				pLink = new CLinkKX() ;
				break ;
			case lnkKXSection:
				pLink = new CLinkKXSection() ;
				break ;
			case lnkDoubleCtrl:
				pLink = new CLinkDoubleCtrl();
				break;
			case lnkSupervise:
				pLink = new CLinkSupervise();
				break;
			case lnkPowerGzTj:
				pLink = new CLinkPowerGzTj();
				break;
			case lnkLineGzTj:
				pLink = new CLinkLineGzTj();
				break;
			case lnkZK:
				pLink = new CLinkZk();
				break;
			default:
				AfxMessageBox("链接对象创建方法还没写");
				AfxThrowFileException(CFileException::generic);
				break;
			}

			ASSERT(pLink != NULL);
			pLink->Save(file,Yn); 
			pLink->m_pTy = this;
			m_LinkArr.Add(pLink);
			pLink = NULL;
		}
	}
}

void CTyBase::LinkObjects_Serialize(CArchive &ar)
{
	m_LinkArr.Serialize(ar);
}

void CTyBase::Ty_Serialize(CArchive &ar)
{

}

BOOL CTyBase::PointInEllipse(float x, float y, RectStruct rt)
{
	if ((x < __min(rt.x0,rt.x1)||x > __max(rt.x0,rt.x1))||
		 (y < __min(rt.y0,rt.y1)||y > __max(rt.y0,rt.y1)))
		 return FALSE;

	float a,b,x0,y0;
	a = (rt.x1-rt.x0)/2;
	b = (rt.y1-rt.y0)/2;

	if (a == 0)
	{
		if ((x = rt.x0)&&(y <= __max(rt.y0,rt.y1))&&(y >= __min(rt.y1,rt.y0)))
			return TRUE;
		else
			return FALSE;
	}

	if (b == 0)
	{
		if ((y = rt.y0)&&(x <= __max(rt.x0,rt.x1))&&(x >= __min(rt.x1,rt.x0)))
			return TRUE;
		else 
			return FALSE;
	}


	x0 = (rt.x1+rt.x0)/2;
	y0 = (rt.y1+rt.y0)/2;

	return ((double)(x-x0)*(double)(x-x0)/((double)a*(double)a)+(double)(y-y0)*(double)(y-y0)/((double)b*(double)b)<=1);
}


void CTyBase::Ty_Save(CFile *file, BOOL Yn)
{

}

void CTyBase::Ty_Move(float xLen, float yLen, CElecMapView *pView)
{

}

void CTyBase::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView, UINT nFlags)
{

}

void CTyBase::RegisterImage(LPCSTR bmpname)
{
	ASSERT(bmpname != NULL);
	if (strlen(bmpname) == 0) 
		return;

	g_image.RegisterImage(bmpname);
}

void CTyBase::UnRegisterImage(LPCSTR bmpname)
{
	ASSERT(bmpname != NULL);
	if (strlen(bmpname) == 0)
		return;

	g_image.UnRegisterImage(bmpname);
}

void CTyBase::DrawImage(LPCSTR bmpname, CDC *pDC, int x0, int y0, int nWidth, int nHeight, BOOL bTranslate, COLORREF color,BOOL blHScale,BOOL blVScale)
{
	ASSERT(bmpname != NULL);
	ASSERT(pDC != NULL);
	
	if (bTranslate) 
		g_image.TransparentBlt(bmpname,pDC->m_hDC,x0,y0,nWidth,nHeight,color,blHScale,blVScale);
	else 
		g_image.StretchBlt(bmpname,pDC->m_hDC,x0,y0,nWidth,nHeight,blHScale,blVScale);
}

void CTyBase::UnRegisterVector(LPCSTR vectorname)
{
	if ((vectorname == NULL)||(strlen(vectorname) == 0))
		return;
	
	g_VectorList.UnRegister_Vector(vectorname);
}

void CTyBase::RegisterVector(LPCSTR vectorname)
{
	if ((vectorname == NULL)||(strlen(vectorname) == 0)) 
		return;
	
	g_VectorList.Register_Vector(vectorname);
}

void CTyBase::DrawVector(LPCSTR vectorname,CDC* pDC,CRect viewRect,CRect clipRect,COLORREF color,BOOL blReplace)
{
	g_VectorList.DrawVector(vectorname,pDC,viewRect,clipRect,color,blReplace);
}

void CTyBase::AddLinkObject(CLinkObjectBase *pLink)
{
	ASSERT(pLink != NULL);
	ASSERT((m_LinkArr.GetUpperBound() < (MAXLINKTY_COUNT-1))); 

	m_LinkArr.Add(pLink);
}

void CTyBase::DeleteLinkObject(int pos)
{
	ASSERT(pos >= 0);
	ASSERT(pos <= m_LinkArr.GetUpperBound());

	delete m_LinkArr[pos];
	m_LinkArr.RemoveAt(pos);
}

int CTyBase::Get_LinkTy_Count()
{
	int count = m_LinkArr.GetUpperBound()+1;

	return count;
}

CString CTyBase::GetLinkObjectName(int pos)
{
	CString ret;
	ASSERT(pos >= 0);
	ASSERT(pos <= m_LinkArr.GetUpperBound());
	ASSERT(m_LinkArr[pos] != NULL);

	ret = m_LinkArr[pos]->m_name;
	
	return ret;
}

BOOL CTyBase::Is_Net_Color_Vector()
{
	return FALSE;
}

BOOL CTyBase::Is_Net_Color_Bmp()
{
	return FALSE;
}

BOOL CTyBase::Is_Net_Color_Power()
{
	BOOL ret = FALSE;

	if ((!Is_Net_Color_Bmp())&&(!Is_Net_Color_Vector())) 
		return ret;
	
	if ((Is_In_Net_Color_Bmp())||(Is_In_Net_Color_Vector()))
		ret = TRUE;

	return ret;
}

BOOL CTyBase::Is_Net_Color_Yx()
{
	return FALSE;
}

BOOL CTyBase::Is_In_Net_Color_Power()
{
	BOOL ret = FALSE;
	CLinkObjectBase* pLink;

	if (!Is_Net_Color_Power()) return ret;

	int loop = m_LinkArr.GetSize();

	CRuntimeClass* prt = RUNTIME_CLASS(CNetColorPower);

	for (int i = 0;i < loop;i++)
	{
		pLink = m_LinkArr[i];
		ASSERT(pLink != NULL);

		if (pLink != NULL)
		{
		   if (pLink->IsKindOf(prt))
		   {
			   ret = TRUE;
			   break;
		   }
		}
	}

	return ret;
}

BOOL CTyBase::Is_In_Net_Color_Yx()
{
	BOOL ret = FALSE;
	CLinkObjectBase* pLink;

	if (!Is_Net_Color_Yx()) return ret;

	int loop = m_LinkArr.GetSize();

	CRuntimeClass* prt = RUNTIME_CLASS(CNetColorYx);

	for (int i = 0;i < loop;i++)
	{
		pLink = m_LinkArr[i];
		ASSERT(pLink != NULL);
		if (pLink != NULL)
		{
			if (pLink->IsKindOf(prt))
			{
				ret = TRUE;
				break;
			}
		}
	}

	return ret;
}

BOOL CTyBase::Is_In_Net_Color_Bmp()
{
	BOOL ret = FALSE;
	CLinkObjectBase* pLink;

	if (!Is_Net_Color_Bmp()) 
		return ret;

	int loop = m_LinkArr.GetSize();

	CRuntimeClass* prt = RUNTIME_CLASS(CNetColorBitmap);

	for (int i = 0;i < loop;i++)
	{
		pLink = m_LinkArr[i];
		ASSERT(pLink != NULL);
		if (pLink != NULL)
		{
			if (pLink->IsKindOf(prt))
			{
				ret = TRUE;
				break;
			}
		}
	}

	return ret;
}

BOOL CTyBase::Is_In_Net_Color_Vector()
{
	BOOL ret=FALSE;
	CLinkObjectBase* pLink;

	if (!Is_Net_Color_Vector()) 
		return ret;

	int loop = m_LinkArr.GetSize();

	CRuntimeClass* prt = RUNTIME_CLASS(CNetColorVector);

	for (int i = 0;i < loop;i++)
	{
		pLink = m_LinkArr[i];
		ASSERT(pLink != NULL);

		if (pLink != NULL)
		{
			if (pLink->IsKindOf(prt))
			{
				ret = TRUE;
				break;
			}
		}
	}

	return ret;
}

void CTyBase::Set_Line_Width(int width,CElecMapView * pView)
{

}

void CTyBase::Set_Ty_Color(COLORREF color,CElecMapView * pView)
{

}

CSize CTyBase::Get_Size_Vector(LPCSTR vectorname)
{
	CSize sz = g_VectorList.Get_Vector_Size(vectorname);
	return sz;
}

void CTyBase::DrawRect(CDC *pDC, int orgX,int orgY,float offsetX, float offsetY, float scaleX, float scaleY, COLORREF ReplaceColor,COLORREF ToColor, BOOL bReplace)
{


}

void CTyBase::Set_Ty_Fill_Color(COLORREF color, CElecMapView *pView)
{

}

void CTyBase::Set_Ty_Is_Fill(BOOL bFill, CElecMapView *pView)
{

}


void CTyBase::SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[])
{


}

// 图元参数按规则替换
void CTyBase::ParamReplace(CStringList& ruleList)
{
	LinkObjects_ParamReplace(ruleList);
	ParamReplace_Ty(ruleList);
}

void CTyBase::LinkObjects_ParamReplace(CStringList& ruleList)
{
	int i,count;
	CLinkObjectBase * pLink;

	count = m_LinkArr.GetUpperBound();
	for (i = 0;i <= count;i++)	
	{
		pLink = m_LinkArr[i];
		ASSERT(pLink != NULL);
		if (pLink != NULL)
		{
			pLink->ParamReplace(ruleList);
		}
	}
}

// 图元参数按规则替换
void CTyBase::ParamReplace_Ty(CStringList& ruleList)
{

}


// 把英文名代入替换规则获取新的英文名
int CTyBase::GetEname_From_ReplacRule(CString vDotName,CString& vDstDotName, CStringList& ruleList)
{
	vDstDotName = vDotName;
	int ret = -1;
  	char srcStr[17],dstStr[17];
	char * token;
	char seps[] =  ",";
	int len;

	BOOL bFind = false;

	POSITION pos = ruleList.GetHeadPosition();
	CString str;
	char buf[256];
	memset(buf,0,sizeof(char)*256);

	while (pos != NULL)
	{
		memset(srcStr,0,sizeof(char)*17);
		memset(dstStr,0,sizeof(char)*17);

		str = ruleList.GetNext(pos);
		if (strlen(str) <= 0) 
			continue;

	    strcpy(buf,str);

		switch (str[0])
		{
		case '1':
			{
				token = strtok( buf, seps );
				token = strtok( NULL, seps );
				ASSERT(strlen(token) < 16);
				len = min(16,strlen(token));
				strncpy(srcStr,token,len);

				token = strtok( NULL, seps );
				ASSERT(strlen(token) < 16);
				len = min(16,strlen(token));
				strncpy(dstStr,token,len);
				
				if (_strnicmp(vDotName,srcStr,strlen(srcStr)) == 0)
				{
					bFind = true;
					vDstDotName.Delete(0,strlen(srcStr));
					vDstDotName.Insert(0,dstStr);
					ASSERT(vDstDotName.GetLength() < 17);
					if (vDstDotName.GetLength() >= 17)
						vDstDotName.Truncate(16);
					ret = 1;
				}
			}
			break;
		default:
			break;
		}

		if (bFind) 
			break;
	}
	
	return ret;
}

int CTyBase::GetStr_From_ReplacRule(CString srcStr,CString& dstStr, CStringList& ruleList)
{
	int ret = -1;
	dstStr = srcStr;
	char vsrcStr[33],vdstStr[33];
	char * token;
	char seps[] =  ",";
	int len;

	BOOL bFind = false;

	POSITION pos = ruleList.GetHeadPosition();

	CString str;
	char buf[256];
	memset(buf,0,sizeof(char)*256);

	while (pos != NULL)
	{
		str = ruleList.GetNext(pos);
		if (strlen(str) <= 0) 
			continue;

		strcpy(buf,str);
		switch (str[0])
		{
		case '0':
			{
				memset(vsrcStr,0,sizeof(char)*33);
				memset(vdstStr,0,sizeof(char)*33);

				token = strtok( buf, seps );
				token = strtok( NULL, seps );
				ASSERT(strlen(token) < 32);
				len = min(32,strlen(token));
				strncpy(vsrcStr,token,len);

				token = strtok( NULL, seps );
				ASSERT(strlen(token) < 32);
				len = min(32,strlen(token));
				strncpy(vdstStr,token,len);
				
				if (dstStr.Find(vsrcStr) >= 0)
				{
					dstStr.Replace(vsrcStr,vdstStr);
					bFind = true;
					ret = 1;
				}
			}
			break;
		default:
			break;
		}

		if (bFind)
			break;
	}

	return ret;
}

// 把点号代入替换规则获取新的点号
int CTyBase::GetDot_From_ReplaceRule(BYTE srcNode, BYTE srcLine, BYTE srcRtu, WORD srcDot, BYTE& dstNode, BYTE& dstLine, BYTE& dstRtu, WORD& dstDot, CStringList& ruleList)
{
	int ret = -1;
	int opt;
	int vNode1,vLine1,vRtu1,vDot1;
	int vNode2,vLine2,vRtu2,vDot2;
	dstNode = srcNode;
	dstLine = srcLine;
	dstRtu = srcRtu;
	dstDot = srcDot;

	POSITION pos = ruleList.GetHeadPosition();
	CString str;
	BOOL bFind = false;
	while (pos != NULL)
	{
		str = ruleList.GetNext(pos);
		if (strlen(str) <= 0) 
			continue;

		switch (str[0])
		{
		case '2':
			{
				int count = sscanf(str,"%d,%d,%d,%d,%d,%d,%d,%d,%d",&opt,&vNode1,&vLine1,&vRtu1,&vDot1,&vNode2,&vLine2,&vRtu2,&vDot2);
				if (count == 9)
				{
					if  ((vNode1 == -1) ||(vNode1 == srcNode)) dstNode = vNode2;
					if  ((vLine1 == -1)||(vLine1 == srcLine)) dstLine = vLine2;
					if  ((vRtu1 == -1) ||(vRtu1 == srcRtu)) dstRtu = vRtu2;
					if  ((vDot1 == -1) ||(vDot1 == srcDot)) dstDot = vDot2;
					
					bFind = TRUE;

					ret = 1;
				}
			}
			break;
		default:
			break;
		}

		if (bFind)
			break;
	}
	
	return ret;
}
