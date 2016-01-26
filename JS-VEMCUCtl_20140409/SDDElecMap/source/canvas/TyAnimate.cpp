// TyAnimate.cpp: implementation of the CTyAnimate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "canvas/TyAnimate.h"
#include "canvas/TyTool.h"
#include "canvas/RtValueTool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// JDJH.cpp: implementation of the CLine class.
//
//////////////////////////////////////////////////////////////////////

#include "../../stdafx.h"
//#include "sp_draw.h"
#include "canvas/JDJH.h"
//#include "sp_drawview.h"
#include "../../ElecMapView.h"

#include "canvas/TyBase.h"
#include "canvas/lineprop.h"
#include "canvas/dashline.h"
#include "dialog/XLineDlg.h"
#include "dialog/TyAnimate_Prop.h"
#include "dialog/FillProp.h"
#include <math.h>
#include "dialog/InputText.h"
#include "dialog/FontProp.h"
#include "canvas/imageX.h"
#include "canvas/HMLayer.h"
#include "dialog/BmpSelectPage.h"
#include "dialog/PropPositionRect.h"
#include "dialog/PropPositionText.h"
#include "dialog/PropPositionLine.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CTyAnimate, CTyBase, 0)

CTyAnimate::CTyAnimate(Stu_TyAnimate * animate)
{
	UnRegistAllPic();
	memcpy(&m_animate,animate,sizeof(Stu_TyAnimate));
	RegistAllPic();
}

CTyAnimate::CTyAnimate(float x0,float y0,float x1,float y1)
{
	sprintf(m_animate.ename,"DEFAULTENAME");
		
	m_animate.rect.x0=x0;
	m_animate.rect.x1=x1;
	m_animate.rect.y0=y0;
	m_animate.rect.y1=y1;

	m_animate.disOR0Kind=0;
	m_animate.piccount=0;
	m_animate.picskind=0;
}

CTyAnimate::~CTyAnimate()
{
}

void CTyAnimate::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView!=NULL) pView->InvalidateTy(this);
	if (m_animate.rect.x0+xLen>=0) 
	{
		m_animate.rect.x0+=xLen;
		m_animate.rect.x1+=xLen;
	}
	if (m_animate.rect.y0+yLen>=0)
	{
		m_animate.rect.y0+=yLen;
		m_animate.rect.y1+=yLen;
	}

	if (pView!=NULL) pView->InvalidateTy(this);
}



void CTyAnimate::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC!=NULL);
	ASSERT(pView!=NULL);

	if(m_bDelete)	//如果已经处于删除状态
		return;

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy)) return;


	GetRect(&minx,&miny,&maxx,&maxy);
	CPoint pt1,pt2;
	pt1=pView->UPtoLP(minx,miny);
	pt2=pView->UPtoLP(maxx,maxy);
  
	if (m_animate.piccount<=0) return;
	if ((m_animate.picskind==0))
		DrawImage(m_animate.pics[0].picname,pDC,pt1.x,pt1.y,pt2.x-pt1.x,pt2.y-pt1.y,m_animate.pics[0].pickind,m_animate.pics[0].piccolor);
	else 	if ((m_animate.picskind==1))
	{
		CRect rt;
		rt.SetRect(pt1.x,pt1.y,pt2.x,pt2.y);
		DrawVector(m_animate.pics[0].picname,pDC,rt,rt,m_animate.pics[0].piccolor,m_animate.pics[0].pickind);
	}

}

void CTyAnimate::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	*minX=m_animate.rect.x0;
	*minY=m_animate.rect.y0;
	*maxX=m_animate.rect.x1;
	*maxY=m_animate.rect.y1;
}

CTyBase * CTyAnimate::Clone()
{
	ASSERT_VALID(this);

	CTyAnimate* pClone = new CTyAnimate(&m_animate);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

BOOL CTyAnimate::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

BOOL CTyAnimate::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange>=0);
	if (m_bDelete) return FALSE; 

	BOOL ret=FALSE;
	RectStruct rt,rt2;
	GetRect(&rt.x0,&rt.y0,&rt.x1,&rt.y1);

	rt2=rt;
	rt2.InflateRect(errRange,errRange);
	ret=PointInRect(x,y,rt2);

	return ret;
}

int CTyAnimate::GetHandleCount()
{
	return 8;
}

PointStruct CTyAnimate::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	nHandle--;
	PointStruct pt;
	pt.x=0;pt.y=0;
	if (nHandle==0)
	{
		pt.x=m_animate.rect.x0;
		pt.y=m_animate.rect.y0;
	}
	if (nHandle==4)
	{
		pt.x=m_animate.rect.x0;
		pt.y=m_animate.rect.y0+(m_animate.rect.y1-m_animate.rect.y0)/2;
	}
	if (nHandle==2)
	{
		pt.x=m_animate.rect.x0;
		pt.y=m_animate.rect.y1;
	}
	if (nHandle==3)
	{
		pt.x=m_animate.rect.x0+(m_animate.rect.x1-m_animate.rect.x0)/2;
		pt.y=m_animate.rect.y1;
	}
	if (nHandle==1)
	{
		pt.x=m_animate.rect.x1;
		pt.y=m_animate.rect.y1;
	}
	if (nHandle==5)
	{
		pt.x=m_animate.rect.x1;
		pt.y=m_animate.rect.y0+(m_animate.rect.y1-m_animate.rect.y0)/2;
	}
	if (nHandle==6)
	{
		pt.x=m_animate.rect.x1;
		pt.y=m_animate.rect.y0;
	}
	if (nHandle==7)
	{
		pt.x=m_animate.rect.x0+(m_animate.rect.x1-m_animate.rect.x0)/2;
		pt.y=m_animate.rect.y0;
	}
	return pt;

}


HCURSOR CTyAnimate::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);
	nHandle--;
	LPCTSTR id;
	switch (nHandle)
	{
		case 0:
			id = IDC_SIZENWSE;
			break;
		case 4:
			id = IDC_SIZEWE;
			break;
		case 2:
			id = IDC_SIZENESW;
			break;
		case 3:
			id = IDC_SIZENS;
			break;
		case 1:
			id = IDC_SIZENWSE;
			break;
		case 5:
			id = IDC_SIZEWE;
			break;
		case 6:
			id = IDC_SIZENESW;
			break;
		case 7:
			id = IDC_SIZENS;
			break;
		case 8:
			id = IDC_SIZEALL;
			break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}


void CTyAnimate::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	nHandle--;
	ASSERT(pView!=NULL);
	pView->InvalidateTy(this);
	RectStruct temprect,temprect2;
	BOOL x0move,y0move,x1move,y1move;
	x0move=false;x1move=false;
	y0move=false;y1move=false;
	temprect=m_animate.rect;
	temprect2=temprect;
	switch (nHandle)
	{
	case 0:
		temprect.x0=point.x;
		temprect.y0=point.y;
		x0move=true;y0move=true;
		break;
	case 4:
		temprect.x0=point.x;
		x0move=true;
		break;
	case 2:
		temprect.x0=point.x;
		temprect.y1=point.y;
		x0move=true;y1move=true;
		break;
	case 3:
		temprect.y1=point.y;
		y1move=true;
		break;
	case 1:
		temprect.x1=point.x;
		temprect.y1=point.y;
		x1move=true;y1move=true;
		break;
	case 5:
		temprect.x1=point.x;
		x1move=true;
		break;
	case 6:
		temprect.x1=point.x;
		temprect.y0=point.y;
		x1move=true;y0move=true;
		break;
	case 7:
		temprect.y0=point.y;
		y0move=true;
		break;
	case 8:
		temprect.x0=point.x-(temprect2.x1-temprect2.x0)/2;
		temprect.x1=temprect.x0+(temprect2.x1-temprect2.x0);
		temprect.y0=point.y-(temprect2.y1-temprect2.y0)/2;
		temprect.y1=temprect.y0+(temprect2.y1-temprect2.y0);
		break;
	}

	int textminx=1,textminy=1;
	if (temprect.x1-temprect.x0<textminx) 
	{
		if (x0move)
		{
			temprect.x0=temprect.x1-textminx;
		}
		else
		{
			temprect.x1=temprect.x0+textminx;
		}
	}

	if (temprect.y1-temprect.y0<textminy) 
	{
		if (y0move)
		{
			temprect.y0=temprect.y1-textminy;
		}
		else
		{
			temprect.y1=temprect.y0+textminy;
		}
	}

	if (temprect.x0<0) temprect.x0=0;
	if (temprect.y0<0) temprect.y0=0;
	m_animate.rect=temprect;
	RefreshIt();
	CTyAnimateTool::canuse=true;
	CTyAnimateTool::m_animate=this->m_animate;
	sprintf(CTyAnimateTool::m_animate.ename,"");
	CTyAnimateTool::m_animate.rect.x0=0;
	CTyAnimateTool::m_animate.rect.y0=0;
	CTyAnimateTool::m_animate.rect.x1=(float)(pView->ULtoLL(this->m_animate.rect.x1-this->m_animate.rect.x0));
	CTyAnimateTool::m_animate.rect.y1=(float)(pView->ULtoLL(this->m_animate.rect.y1-this->m_animate.rect.y0));
}


void CTyAnimate::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);
/*struct Stu_SinglePic
{
	char picname[33];
	BYTE pickind;
	COLORREF piccolor;
};
struct Stu_TyAnimate
{
   char ename[33];
   RectStruct rect;//范围
   BYTE disOR0Kind;//0  不显示  1  显示
   BYTE piccount;
   Stu_SinglePic pics[16];
};*/

	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_animate.rect,sizeof(m_animate.rect));
		file->Write((unsigned char *)m_animate.ename,sizeof(char)*33);
		file->Write((unsigned char *)&m_animate.disOR0Kind,sizeof(BYTE));
		file->Write((unsigned char *)&m_animate.picskind,sizeof(BYTE));
		file->Write((unsigned char *)&m_animate.piccount,sizeof(BYTE));
		if (m_animate.piccount>16) m_animate.piccount=16;
		for (int i=0;i<m_animate.piccount;i++)
		{
			file->Write((unsigned char *)m_animate.pics[i].picname,sizeof(char)*33);
			file->Write((unsigned char *)&m_animate.pics[i].pickind,sizeof(BYTE));
			file->Write((unsigned char *)&m_animate.pics[i].piccolor,sizeof(COLORREF));
		}
	} 
	else
	{
		UnRegistAllPic();
		file->Read((unsigned char *)&m_animate.rect,sizeof(m_animate.rect));
		file->Read((unsigned char *)m_animate.ename,sizeof(char)*33);
		file->Read((unsigned char *)&m_animate.disOR0Kind,sizeof(BYTE));
		file->Read((unsigned char *)&m_animate.picskind,sizeof(BYTE));
		file->Read((unsigned char *)&m_animate.piccount,sizeof(BYTE));
		if (m_animate.piccount>16) m_animate.piccount=16;
		for (int i=0;i<m_animate.piccount;i++)
		{
			file->Read((unsigned char *)m_animate.pics[i].picname,sizeof(char)*33);
			file->Read((unsigned char *)&m_animate.pics[i].pickind,sizeof(BYTE));
			file->Read((unsigned char *)&m_animate.pics[i].piccolor,sizeof(COLORREF));
		}
		RegistAllPic();
	
	}

}

void CTyAnimate::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if(ar.IsStoring())  
	{
		ar.Write((unsigned char *)&m_animate.rect,sizeof(m_animate.rect));
		ar.Write((unsigned char *)&m_animate.ename,sizeof(char)*33);
		ar.Write((unsigned char *)&m_animate.disOR0Kind,sizeof(BYTE));
		ar.Write((unsigned char *)&m_animate.picskind,sizeof(BYTE));
		ar.Write((unsigned char *)&m_animate.piccount,sizeof(BYTE));
		if (m_animate.piccount>16) m_animate.piccount=16;
		for (int i=0;i<m_animate.piccount;i++)
		{
			ar.Write((unsigned char *)&m_animate.pics[i].picname,sizeof(char)*33);
			ar.Write((unsigned char *)&m_animate.pics[i].pickind,sizeof(BYTE));
			ar.Write((unsigned char *)&m_animate.pics[i].piccolor,sizeof(COLORREF));
		}
	} 
	else
	{
		UnRegistAllPic();
		ar.Read((unsigned char *)&m_animate.rect,sizeof(m_animate.rect));
		ar.Read((unsigned char *)&m_animate.ename,sizeof(char)*33);
		ar.Read((unsigned char *)&m_animate.disOR0Kind,sizeof(BYTE));
		ar.Read((unsigned char *)&m_animate.picskind,sizeof(BYTE));
		ar.Read((unsigned char *)&m_animate.piccount,sizeof(BYTE));
		if (m_animate.piccount>16) m_animate.piccount=16;
		for (int i=0;i<m_animate.piccount;i++)
		{
			ar.Read((unsigned char *)&m_animate.pics[i].picname,sizeof(char)*33);
			ar.Read((unsigned char *)&m_animate.pics[i].pickind,sizeof(BYTE));
			ar.Read((unsigned char *)&m_animate.pics[i].piccolor,sizeof(COLORREF));
		}
		RegistAllPic();
	}
}

DRAW_TY CTyAnimate::GetTyType()
{
	return tyAnimate;
}

void CTyAnimate::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("动画对象..") );
 
	CTyAnimate_Prop dlg;
	dlg.SetPara(&m_animate);
	sheet.AddPage( (CPropertyPage*)&dlg );
	UnRegistAllPic();
	
	if (sheet.DoModal() == IDOK)
	{
		RefreshIt();	
		SetModifiedFlag();
		dlg.GetPara(&m_animate);
		RefreshIt();	
		CTyAnimateTool::canuse=true;
		CTyAnimateTool::m_animate=this->m_animate;
		sprintf(CTyAnimateTool::m_animate.ename,"");
		CTyAnimateTool::m_animate.rect.x0=0;
		CTyAnimateTool::m_animate.rect.y0=0;
		CTyAnimateTool::m_animate.rect.x1=(float)(pView->ULtoLL(this->m_animate.rect.x1-this->m_animate.rect.x0));
		CTyAnimateTool::m_animate.rect.y1=(float)(pView->ULtoLL(this->m_animate.rect.y1-this->m_animate.rect.y0));
	}
	RegistAllPic();
	
}


void CTyAnimate::RefreshIt()
{
	Invalidate();
}

void CTyAnimate::RegistAllPic()
{
	if (m_animate.piccount<=0) 
		return;
	if ((m_animate.picskind==0))
		RegisterImage(m_animate.pics[0].picname);
	else if ((m_animate.picskind==1))
		RegisterVector(m_animate.pics[0].picname);
}

void CTyAnimate::UnRegistAllPic()
{
	if (m_animate.piccount<=0) 
		return;
	if ((m_animate.picskind==0))
		UnRegisterImage(m_animate.pics[0].picname);
	else if ((m_animate.picskind==1))
		UnRegisterVector(m_animate.pics[0].picname);
}

void CTyAnimate::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=GetEname_From_ReplacRule(m_animate.ename,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_animate.ename,str);
	}
}
