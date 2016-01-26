// ShuZhi.cpp: implementation of the CTyDirection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/ShuZhi.h"
//#include "sp_drawview.h"
#include "../../ElecMapView.h"
#include "canvas/DashLine.h"
#include "dialog/PropPositionRect.h"
#include "dialog/DirectionProp.h"
#include "dialog/TyHMProxyProp.h"
#include "dialog/PropPositionLine.h"
#include "canvas/RtValueTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CTyDirection Class
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CTyDirection, CTyBase, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTyDirection::CTyDirection()
{
	ZeroMemory(m_dotname,sizeof(char)*17);

}

CTyDirection::~CTyDirection()
{
 
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTyDirection::CTyDirection(float x0,float y0,float x1,float y1,COLORREF Color1, COLORREF Color2, BOOL blTranslate,int nWidth, int Type[4],LPCSTR dotname,BOOL zeroshow,int vId,CHMLayer* pLayer)
	:CTyBase(vId,pLayer)
{
	m_x0=x0;
	m_y0=y0;
	m_x1=x1;
	m_y1=y1;
	m_iWidth=nWidth;
	m_color1=Color1;
	m_color2=Color2;
	memcpy(m_Type,Type,4 * sizeof(int));
	m_blTranslate=blTranslate;	

	m_bZeroShow=zeroshow;
	ZeroMemory(m_dotname,sizeof(char)*17);
	if (dotname!=NULL)
	{
		ASSERT((strlen(dotname)<=16));
		strcpy(m_dotname,dotname);
	}

}

void CTyDirection::Set_Line_Width(int width, CElecMapView *pView)
{
	if (m_iWidth==width) return;	
	ASSERT(pView!=NULL);
	if (pView==NULL) return;

	pView->InvalidateTy(this);
	m_iWidth=width;
	SetModifiedFlag();
	pView->InvalidateTy(this);

}


void CTyDirection::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC!=NULL);
	ASSERT(pView!=NULL);
	CPoint pt0,pt1;

	if(m_bDelete)	//如果已经处于删除状态
	  return;

	pt0=pView->UPtoLP(m_x0,m_y0);
	pt1=pView->UPtoLP(m_x1,m_y1);
  
	CPen *pOldPen = pDC->GetCurrentPen();

	CDashLine ln(pDC,m_color1,m_color2,m_blTranslate,m_iWidth,m_Type);
	ln.MoveTo(pt0.x,pt0.y);
	ln.LineTo(pt1.x,pt1.y);

	pDC->SelectObject(pOldPen);

}

PointStruct CTyDirection::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	PointStruct pt;

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		pt.x = m_x0;
		pt.y = m_y0;
		break;

	case 2:
		pt.x = m_x1;
		pt.y = m_y1;
		break;
	}

	return pt;
}

int CTyDirection::GetHandleCount()
{
  return 2;
}

HCURSOR CTyDirection::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);

	LPCTSTR id;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
	case 2:
		id = IDC_SIZENWSE;
		break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}

void CTyDirection::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView!=NULL);

	pView->InvalidateTy(this);
	
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);
    break;
	case 1:
    if ((nFlags&MK_SHIFT)!=0)
		{
			if (fabs(point.x-m_x1)>fabs(point.y-m_y1)) 
			{
				m_y0=m_y1;
				m_x0=point.x;
			}
			else 
			{
				m_x0=m_x1;
				m_y0=point.y;
			}
		}
		else 
		{
			m_x0=point.x;
			m_y0=point.y;
		}
    break;
	case 2:
		if ((nFlags&MK_SHIFT)!=0)
		{
			if (fabs(point.x-m_x0)>fabs(point.y-m_y0)) 
			{
				m_y1=m_y0;
				m_x1=point.x;
			}
			else 
			{
				m_x1=m_x0;
				m_y1=point.y;
			}
		} 
		else
		{ 
			m_x1=point.x;
			m_y1=point.y;
		}
		break;
	}

	pView->InvalidateTy(this);
}

void CTyDirection::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);

	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_x0,sizeof(m_x0));
		file->Write((unsigned char *)&m_y0,sizeof(m_y0));
		file->Write((unsigned char *)&m_x1,sizeof(m_x1));
		file->Write((unsigned char *)&m_y1,sizeof(m_y1));

		file->Write((unsigned char *)&m_iWidth,sizeof(m_iWidth));
		file->Write((unsigned char *)&m_color1,sizeof(m_color1));
		file->Write((unsigned char *)&m_color2,sizeof(m_color2));
		file->Write((unsigned char *)&m_blTranslate,sizeof(m_blTranslate));
		file->Write((unsigned char *)m_Type,sizeof(int)*4);
		file->Write((unsigned char *)m_dotname,sizeof(char)*17);
		file->Write((unsigned char *)&m_bZeroShow,sizeof(BOOL));

	} 
	else
	{
		file->Read((unsigned char *)&m_x0,sizeof(m_x0));
		file->Read((unsigned char *)&m_y0,sizeof(m_y0));
		file->Read((unsigned char *)&m_x1,sizeof(m_x1));
		file->Read((unsigned char *)&m_y1,sizeof(m_y1));
	
		file->Read((unsigned char *)&m_iWidth,sizeof(m_iWidth));
		file->Read((unsigned char *)&m_color1,sizeof(m_color1));
		file->Read((unsigned char *)&m_color2,sizeof(m_color2));
		file->Read((unsigned char *)&m_blTranslate,sizeof(m_blTranslate));
		file->Read((unsigned char *)m_Type,sizeof(int)*4);
		file->Read((unsigned char *)m_dotname,sizeof(char)*17);
		file->Read((unsigned char *)&m_bZeroShow,sizeof(BOOL));
	}
}

void CTyDirection::Ty_Serialize(CArchive &ar)
{
	int i;
	if (ar.IsStoring())
	{
		ar<<m_x0<<m_y0<<m_x1<<m_y1<<m_iWidth<<m_color1<<m_color2<<m_blTranslate;
		for (i=0;i<4;i++)
			 ar<<m_Type[i];
		//ar.Write((unsigned char *)m_dotname,sizeof(char)*17);
		ar<<m_bZeroShow;
	} 
	else 
	{
		ar>>m_x0>>m_y0>>m_x1>>m_y1>>m_iWidth>>m_color1>>m_color2>>m_blTranslate;
		for (i=0;i<4;i++)
			 ar>>m_Type[i];
		//ar.Read((unsigned char *)m_dotname,sizeof(char)*17);
		ar>>m_bZeroShow;
	}
}

DRAW_TY CTyDirection::GetTyType()
{
  return tyCL;
}


void CTyDirection::SetDefaultProp(CElecMapView *pView)
{
	ASSERT(pView!=NULL);
	if (pView==NULL) return;

	CTyDirectionTool::c_color1=m_color1;
	CTyDirectionTool::c_color2=m_color2;
	CTyDirectionTool::c_blTranslate=m_blTranslate;
	CTyDirectionTool::c_nWidth=m_iWidth;
	memcpy(CTyDirectionTool::c_type,m_Type,sizeof(int)*4);
}


void CTyDirection::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("对象..") );

	CDirectionProp dlg;
	int secLen=m_Type[0]/4;

	dlg.SetProp(secLen,m_iWidth,m_color1,m_color2,m_blTranslate,m_dotname,m_bZeroShow);
	sheet.AddPage( (CPropertyPage*)&dlg );

	CPropPositionLine dlgpos;
	dlgpos.SetPosition(m_x0,m_y0,m_x1,m_y1);
	sheet.AddPage( (CPropertyPage*)&dlgpos );


	if (sheet.DoModal() == IDOK)
	{
		Invalidate();
	//	dlg.GetXLine(m_color1,m_color2,m_blTranslate,m_iWidth,m_Type)
		dlg.GetProp(secLen,m_iWidth,m_color1,m_color2,m_blTranslate,m_dotname,m_bZeroShow);
		for (int i=0;i<4;i++)
		{
			m_Type[i]=secLen*4;
		}
		dlgpos.GetPosition(m_x0,m_y0,m_x1,m_y1);
		SetModifiedFlag();
		Invalidate();
		SetDefaultProp(pView);
		return;
	}
}

BOOL CTyDirection::PointInObj(float x, float y, float errRange)
{
	ASSERT(errRange>=0);
	if (m_bDelete) return FALSE; 
	if ((x<max(0,__min(m_x0,m_x1)-errRange))&&
			(y<max(0,__min(m_y0,m_y1)-errRange))&&
			(x>__max(m_x0,m_x1)+errRange)&&
			(y>__max(m_y0,m_y1)+errRange)) return FALSE;


  float dist=PointLine(x,y,m_x0,m_y0,m_x1,m_y1);
  if (dist>errRange) return FALSE;
  else return TRUE; 
}

BOOL CTyDirection::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

CTyBase * CTyDirection::Clone()
{
	ASSERT_VALID(this);

	CTyDirection* pClone = new CTyDirection(m_x0,m_y0,m_x1,m_y1,m_color1,m_color2,m_blTranslate,m_iWidth,m_Type,m_dotname);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;

}

void CTyDirection::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	*minX=__min(m_x0,m_x1);
	*minY=__min(m_y0,m_y1);
	*maxX=__max(m_x0,m_x1);
	*maxY=__max(m_y0,m_y1);
}

void CTyDirection::Ty_Move(float xLen, float yLen, CElecMapView *pView)
{
	if (pView!=NULL) pView->InvalidateTy(this);
	m_x0+=xLen;
	m_x1+=xLen;
	m_y0+=yLen;
	m_y1+=yLen;
	if (pView!=NULL) pView->InvalidateTy(this);
}


BOOL CTyDirection::Is_Net_Color_Vector()
{
	return TRUE;
}

void CTyDirection::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=GetEname_From_ReplacRule(m_dotname,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag();
		strcpy(m_dotname,str);
	}
}

////////////////////////////////////////





//////////////////////////////////////////////////////////////////////
// CTyDirection Class
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CTyHMProxy, CTyBase, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTyHMProxy::CTyHMProxy()
{
	ZeroMemory(m_hmname,sizeof(char)*33);
	ZeroMemory(m_str1,sizeof(char)*33);
	ZeroMemory(m_str2,sizeof(char)*33);
	ZeroMemory(m_TextFont,sizeof(char)*16);

	m_TextHeight=16;
	m_TextWidth=16;
	m_JqWidth=8;
}

CTyHMProxy::~CTyHMProxy()
{

}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTyHMProxy::CTyHMProxy(float x0,float y0,float x1,float y1,LPCSTR hmname,
											 int display,LPCSTR str1,LPCSTR str2,COLORREF Color1, 
											 COLORREF Color2,BOOL bFlash,BOOL FangXiang,float TextHeight,
											 float TextWidth,float JqWidth,LPCSTR TextFont,BOOL bTranslate1,
											 BOOL bTranslate2,int vId,CHMLayer* pLayer)
	:CTyBase(vId,pLayer)
{
	m_x0=x0;
	m_y0=y0;
	m_x1=x1;
	m_y1=y1;

	ZeroMemory(m_hmname,sizeof(char)*33);
	if ((hmname!=NULL)&&(strlen(hmname)>0))
	{
		ASSERT(strlen(hmname)<33);
		strcpy(m_hmname,hmname);
	}

	m_display=display;

	ZeroMemory(m_str1,sizeof(char)*33);
	if ((str1!=NULL)&&(strlen(str1)>0))
	{
		ASSERT(strlen(str1)<33);
		strcpy(m_str1,str1);
	}

	ZeroMemory(m_str2,sizeof(char)*33);
	if ((str2=NULL)&&(strlen(str2)>0))
	{
		ASSERT(strlen(str2)<33);
		strcpy(m_str2,str2);
	}

	m_color1=Color1;
	m_color2=Color2;
	m_bFlash=bFlash;
	m_FangXiang=FangXiang;
	m_TextHeight=TextHeight;
	m_TextWidth=TextWidth;
	m_JqWidth=JqWidth;

	ZeroMemory(m_TextFont,sizeof(char)*16);
	if ((TextFont!=NULL)&&(strlen(TextFont)>0))
	{
		ASSERT(strlen(TextFont)<16);
		strcpy(m_TextFont,TextFont);
	}

	m_bTranslate1=bTranslate1;
	m_bTranslate2=bTranslate2;
	Register();
}

void CTyHMProxy::Set_Ty_Color(COLORREF color, CElecMapView *pView)
{
	if (m_display==0) return;
	if (m_color1==color) return;
	m_color1=color;
	SetModifiedFlag();

	ASSERT(pView!=NULL);
	if (pView==NULL) return;

	pView->InvalidateTy(this);

}

void CTyHMProxy::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC!=NULL);
	ASSERT(pView!=NULL);
	CPoint pt0,pt1;

	if(m_bDelete)	//如果已经处于删除状态
	  return;

 	pt0=pView->UPtoLP(m_x0,m_y0);
	pt1=pView->UPtoLP(m_x1,m_y1);

	CRect rt;
	rt.SetRect(pt0.x,pt0.y,pt1.x,pt1.y);
	rt.NormalizeRect();
	if ((rt.Width()<1)||(rt.Height()<1)) return;

	switch (m_display)
	{
	case 0:
		DrawDC_Bmp(pDC,pView);
		break;
	case 1:
		DrawDC_Vecter(pDC,pView);
		break;
	case 2:
		DrawDC_Text(pDC,pView);
		break;
	}

}

PointStruct CTyHMProxy::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	PointStruct pt;

	float x0,y0,x1,y1;

	GetRect(&x0,&y0,&x1,&y1);
  
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		pt.x = x0;
		pt.y = y0;
		break;

	case 2:
		pt.x = (x1+x0)/2;
		pt.y = y0;
		break;

	case 3:
		pt.x = x1;
		pt.y = y0;
		break;

	case 4:
		pt.x = x1;
		pt.y = (y0+y1)/2;
		break;

	case 5:
		pt.x = x1;
		pt.y = y1;
		break;

	case 6:
		pt.x = (x1+x0)/2;
		pt.y = y1;
		break;

	case 7:
		pt.x = x0;
		pt.y = y1;
		break;

	case 8:
		pt.x = x0;
		pt.y = (y0+y1)/2;
		break;

	}

	return pt;

}

int CTyHMProxy::GetHandleCount()
{
	return 8;
}

HCURSOR CTyHMProxy::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);

	LPCTSTR id;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
	case 5:
		id=IDC_SIZENWSE;
		break;

	case 2:
	case 6:
		id=IDC_SIZENS;
		break;

	case 3:
	case 7:
		id=IDC_SIZENESW;
		break;

	case 4:
	case 8:
		id=IDC_SIZEWE;
		break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}

void CTyHMProxy::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView!=NULL);

	float tmp;

	pView->InvalidateTy(this);

	if (m_display==2)
	{
		Ty_MoveHandleTo_Text(nHandle,point,pView,nFlags);			
		return; 
	}

  switch (nHandle)
	{
	default:
		ASSERT(FALSE);
    break;
	case 1:
		if ((point.x>=m_x1)||(point.y>=m_y1))
		  break;
		
		if ((nFlags&MK_SHIFT)!=0)
		{
			if (fabs(point.x-m_x1)>fabs(point.y-m_y1)) 
			{
				m_y0=point.y;
				if (point.x<m_x1)
					tmp=m_x1-(float)fabs(m_y1-m_y0);
				else
					tmp=m_x1+(float)fabs(m_y1-m_y0);
				if (tmp<m_x1)
					m_x0=tmp;
			} 
			else
			{
				m_x0=point.x;
				if (point.y<m_y1)
					tmp=m_y1-(float)fabs(m_x1-m_x0);
				else
					tmp=m_y1+(float)fabs(m_x1-m_x0);
	
				if (tmp<m_y1)
					m_y1=tmp;
			}
		}
		else 
		{
			m_x0=point.x;
			m_y0=point.y;
		}
    break;

	case 2:
		if (point.y>=m_y1)
		  break;
		m_y0=point.y;
		break;

	case 3:
		if ((point.x<=m_x0)||(point.y>=m_y1))
		  break;

		if ((nFlags&MK_SHIFT)!=0)
		{
			if (fabs(point.x-m_x0)>fabs(point.y-m_y1)) 
			{
				m_y0=point.y;
				if (point.x<m_x0)
					m_x1=m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1=m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1=point.x;
				if (point.y<m_y1)
					m_y0=m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0=m_y1+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x1=point.x;
			m_y0=point.y;
		}
		break;

	case 4:
		if ((point.x<=m_x0))
		  break;
		m_x1=point.x;
		break;

	case 5:
		if ((point.x<=m_x0)||(point.y<=m_y0))
		  break;

		if ((nFlags&MK_SHIFT)!=0)
		{
			if (fabs(point.x-m_x0)>fabs(point.y-m_y0)) 
			{
				m_y1=point.y;
				if (point.x<m_x0)
					m_x1=m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1=m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1=point.x;
				if (point.y<m_y0)
					m_y1=m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1=m_y0+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x1=point.x;
			m_y1=point.y;
		}
		break;

	case 6:
		{
			if (point.y<=m_y0)
				break;

			m_y1=point.y;
		}
		break;

	case 7:
		if ((point.x>=m_x1)||(point.y<=m_y0))
		  break;

		if ((nFlags&MK_SHIFT)!=0)
		{
			if (fabs(point.x-m_x1)>fabs(point.y-m_y0)) 
			{
				m_y1=point.y;
				if (point.x<m_x1)
					m_x0=m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0=m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0=point.x;
				if (point.y<m_y0)
					m_y1=m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1=m_y0+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x0=point.x;
			m_y1=point.y;
		}
		break;

	case 8:
		if (point.x>=m_x1)
		  break;

		m_x0=point.x;
		break;

	}

	pView->InvalidateTy(this);

 	SetDefaultProp(pView);

}

void CTyHMProxy::Ty_MoveHandleTo_Text(int nHandle, PointStruct point, CElecMapView *pView, UINT nFlags)
{
	ASSERT(pView!=NULL);

	if (m_display!=2) return;

	pView->InvalidateTy(this);
  
	float x0,y0,x1,y1;
	
	GetRect(&x0,&y0,&x1,&y1);
	
	float width=(x1-x0);
	float height=(y1-y0);

  float distX=0,distY=0;
	
	
  switch (nHandle)
	{
	default:
		ASSERT(FALSE);
    break;
	case 1:
		{
			distX=m_x0-point.x;
			distY=m_y0-point.y;
			
			if ((distX<=(-width))||(distY<=(-height)))
			  return;

			m_x0=point.x;
			m_y0=point.y;
		}
    break;

	case 2:
		{
			distY=m_y0-point.y;
			if (distY<=(-height))
			  return;

			m_y0=point.y;
		}
		break;

	case 3:
		{
			distX=point.x-x1;
			distY=m_y0-point.y;
      if ((distX<=(-width))||(distY<=(-height)))
			  return;

			m_y0=point.y;
		}
		break;

	case 4:
		distX=point.x-x1;
		if (distX<=(-width))
		  return;

		break;

	case 5:
		{
			distX=point.x-x1;
			distY=point.y-y1;
			if ((distX<=(-width))||(distY<=(-height)))
			  return;

		}
		break;

	case 6:
		distY=point.y-y1;
		if (distY<=(-height))
		  return;

		break;

	case 7:
		{
			distX=m_x0-point.x;
			distY=point.y-y1;
			if ((distX<=(-width))||(distY<=(-height)))
			  return;
			m_x0=point.x;
		}
		break;

	case 8:
		{
			distX=m_x0-point.x;
			if (distX<=(-width))
			  return;
		  m_x0=point.x;
		}
		break;

	}
	
	height=height+distY;
	width=width+distX;
	if (height<0) height=0;
	if (width<0)  width=0;
	Set_Size_Text(width,height);

	GetRect(&x0,&y0,&x1,&y1);

	m_x0=x0;
	m_y0=y0;

	pView->InvalidateTy(this);

	SetDefaultProp(pView);
}


void CTyHMProxy::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);

	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_x0,sizeof(m_x0));
		file->Write((unsigned char *)&m_y0,sizeof(m_y0));
		file->Write((unsigned char *)&m_x1,sizeof(m_x1));
		file->Write((unsigned char *)&m_y1,sizeof(m_y1));

		file->Write((unsigned char *)m_hmname,sizeof(char)*33);
		file->Write((unsigned char *)&m_display,sizeof(m_display));
		file->Write((unsigned char *)m_str1,sizeof(char)*33);
		file->Write((unsigned char *)m_str2,sizeof(char)*33);
		file->Write((unsigned char *)&m_color1,sizeof(m_color1));
		file->Write((unsigned char *)&m_color2,sizeof(m_color2));
		file->Write((unsigned char *)&m_bFlash,sizeof(m_bFlash));
		file->Write((unsigned char *)&m_FangXiang,sizeof(m_FangXiang));

		file->Write((unsigned char *)&m_TextHeight,sizeof(m_TextHeight));
		file->Write((unsigned char *)&m_TextWidth,sizeof(m_TextWidth));
		file->Write((unsigned char *)&m_JqWidth,sizeof(m_JqWidth));
		file->Write((unsigned char *)m_TextFont,sizeof(char)*16);

		file->Write((unsigned char *)&m_bTranslate1,sizeof(m_bTranslate1));
		file->Write((unsigned char *)&m_bTranslate2,sizeof(m_bTranslate2));

	} 
	else
	{
		file->Read((unsigned char *)&m_x0,sizeof(m_x0));
		file->Read((unsigned char *)&m_y0,sizeof(m_y0));
		file->Read((unsigned char *)&m_x1,sizeof(m_x1));
		file->Read((unsigned char *)&m_y1,sizeof(m_y1));
	
		file->Read((unsigned char *)m_hmname,sizeof(char)*33);
		file->Read((unsigned char *)&m_display,sizeof(m_display));
		file->Read((unsigned char *)m_str1,sizeof(char)*33);
		file->Read((unsigned char *)m_str2,sizeof(char)*33);
		file->Read((unsigned char *)&m_color1,sizeof(m_color1));
		file->Read((unsigned char *)&m_color2,sizeof(m_color2));
		file->Read((unsigned char *)&m_bFlash,sizeof(m_bFlash));
		file->Read((unsigned char *)&m_FangXiang,sizeof(m_FangXiang));

		file->Read((unsigned char *)&m_TextHeight,sizeof(m_TextHeight));
		file->Read((unsigned char *)&m_TextWidth,sizeof(m_TextWidth));
		file->Read((unsigned char *)&m_JqWidth,sizeof(m_JqWidth));
		file->Read((unsigned char *)m_TextFont,sizeof(char)*16);
		file->Read((unsigned char *)&m_bTranslate1,sizeof(m_bTranslate1));
		file->Read((unsigned char *)&m_bTranslate2,sizeof(m_bTranslate2));

		if (m_display==0)
		{
			RegisterImage(m_str1);
			RegisterImage(m_str2);
		
		} 
		else if (m_display==1)
		{
			RegisterVector(m_str1);
			RegisterVector(m_str2);
		}

	}

}

void CTyHMProxy::Ty_Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar<<m_x0<<m_y0<<m_x1<<m_y1;
		ar.Write((unsigned char *)m_hmname,sizeof(char)*33);
		ar<<m_display;
		ar.Write((unsigned char *)m_str1,sizeof(char)*33);
		ar.Write((unsigned char *)m_str2,sizeof(char)*33);
		ar<<m_color1<<m_color2<<m_bFlash<<m_FangXiang<<m_TextHeight<<m_TextWidth<<m_JqWidth;
		ar.Write((unsigned char *)m_TextFont,sizeof(char)*16);
		ar<<m_bTranslate1<<m_bTranslate2;
	} 
	else 
	{
		UnRegister();
		ar>>m_x0>>m_y0>>m_x1>>m_y1;
		ar.Read((unsigned char *)m_hmname,sizeof(char)*33);
		ar>>m_display;
		ar.Read((unsigned char *)m_str1,sizeof(char)*33);
		ar.Read((unsigned char *)m_str2,sizeof(char)*33);
		ar>>m_color1>>m_color2>>m_bFlash>>m_FangXiang>>m_TextHeight>>m_TextWidth>>m_JqWidth;
		ar.Read((unsigned char *)m_TextFont,sizeof(char)*16);
		ar>>m_bTranslate1>>m_bTranslate2;
		Register();
	}
}


DRAW_TY CTyHMProxy::GetTyType()
{
	return tyHmProxy;
}

void CTyHMProxy::SetDefaultProp(CElecMapView * pView)
{
	ASSERT(pView!=NULL);
	if (pView==NULL) return;

	CTyHMProxyTool::c_bFlash=m_bFlash;
	CTyHMProxyTool::c_bTranslate1=m_bTranslate1;
	CTyHMProxyTool::c_bTranslate2=m_bTranslate2;
	CTyHMProxyTool::c_Color1=m_color1;
	CTyHMProxyTool::c_Color2=m_color2;
	CTyHMProxyTool::c_display=m_display;
	CTyHMProxyTool::c_FangXiang=m_FangXiang;
	
	CTyHMProxyTool::c_Height=pView->ULtoLL(m_y1-m_y0);
	CTyHMProxyTool::c_Width=pView->ULtoLL(m_x1-m_x0);
	CTyHMProxyTool::c_JqWidth=(float)(pView->ULtoLL(m_JqWidth));
	strcpy(CTyHMProxyTool::c_str1,m_str1);
	strcpy(CTyHMProxyTool::c_str2,m_str2);
	strcpy(CTyHMProxyTool::c_TextFont,m_TextFont);
	CTyHMProxyTool::c_TextHeight=pView->ULtoLL(m_TextHeight);
	CTyHMProxyTool::c_TextWidth=pView->ULtoLL(m_TextWidth);
}


void CTyHMProxy::UnRegister()
{
  if (m_display==0)
	{
		UnRegisterImage(m_str1);
		UnRegisterImage(m_str2);
	} 
	else if (m_display==1)
	{
		UnRegisterVector(m_str1);
		UnRegisterVector(m_str2);
	}
}

void CTyHMProxy::Register()
{
  if (m_display==0)
	{
		RegisterImage(m_str1);
		RegisterImage(m_str2);
	} 
	else if (m_display==1)
	{
		RegisterVector(m_str1);
		RegisterVector(m_str2);
	}
}


void CTyHMProxy::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("对象..") );

	CTyHMProxyProp dlg;

	dlg.SetProp(m_hmname,m_display,m_bFlash,m_str1,
		          m_str2,m_color1,m_color2,m_FangXiang,
							m_TextHeight,m_TextWidth,m_JqWidth,
							m_TextFont,m_bTranslate1,m_bTranslate2);
	sheet.AddPage( (CPropertyPage*)&dlg );

	CPropPositionRect dlgpos;
	dlgpos.SetContent(m_x0,m_y0,m_x1,m_y1);
	sheet.AddPage( (CPropertyPage*)&dlgpos );


	if (sheet.DoModal() == IDOK)
	{
		Invalidate();
		UnRegister();
		dlg.GetProp(m_hmname,m_display,m_bFlash,m_str1,m_str2,
			          m_color1,m_color2,m_FangXiang,m_TextHeight,
								m_TextWidth,m_JqWidth,m_TextFont,m_bTranslate1,
								m_bTranslate2);
    if (strlen(m_str1)==0) strcpy(m_str1," ");
    if (strlen(m_str2)==0) strcpy(m_str2," ");

		dlgpos.GetContent(m_x0,m_y0,m_x1,m_y1);
		SetModifiedFlag();
		Invalidate();
		Register(); 
		return;
	}
}

BOOL CTyHMProxy::PointInObj(float x, float y, float errRange)
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

BOOL CTyHMProxy::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

CTyBase * CTyHMProxy::Clone()
{
	ASSERT_VALID(this);


	CTyHMProxy* pClone = new CTyHMProxy(m_x0,m_y0,m_x1,m_y1,m_hmname,m_display,
																			m_str1,m_str2,m_color1,m_color2,m_bFlash,
																			m_FangXiang,m_TextHeight,m_TextWidth,
																			m_JqWidth,m_TextFont,m_bTranslate1,m_bTranslate2);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;

}

void CTyHMProxy::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	if (m_display==2)
	{
		GetRect_Text(minX,minY,maxX,maxY);
	}
	else
	{
		*minX=__min(m_x0,m_x1);
		*minY=__min(m_y0,m_y1);
		*maxX=__max(m_x0,m_x1);
		*maxY=__max(m_y0,m_y1);
	}
}

void CTyHMProxy::Ty_Move(float xLen, float yLen, CElecMapView *pView)
{
	if (pView!=NULL) pView->InvalidateTy(this);
	m_x0+=xLen;
	m_x1+=xLen;
	m_y0+=yLen;
	m_y1+=yLen;
	if (pView!=NULL) pView->InvalidateTy(this);
}

void CTyHMProxy::DrawDC_Bmp(CDC *pDC, CElecMapView *pView)
{
	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy)) return;


	GetRect(&minx,&miny,&maxx,&maxy);
	CPoint pt1,pt2;
	pt1=pView->UPtoLP(minx,miny);
	pt2=pView->UPtoLP(maxx,maxy);
	CRect rt;
	rt.SetRect(pt1,pt2);
	rt.NormalizeRect();
  
	DrawImage(m_str1,pDC,pt1.x,pt1.y,rt.Width(),rt.Height(),m_bTranslate1,m_color1);

}

void CTyHMProxy::DrawDC_Vecter(CDC *pDC, CElecMapView *pView)
{
	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy)) return;


	GetRect(&minx,&miny,&maxx,&maxy);
	CPoint pt1,pt2;
	pt1=pView->UPtoLP(minx,miny);
	pt2=pView->UPtoLP(maxx,maxy);
	CRect rt;
	rt.SetRect(pt1,pt2);
	rt.NormalizeRect();
  //DrawVector(LPCSTR vectorname,CDC* pDC,CRect viewRect,CRect clipRect,COLORREF color,BOOL blReplace)
	DrawVector(m_str1,pDC,rt,rt,m_color1,m_bTranslate1);
}

void CTyHMProxy::DrawDC_Text(CDC *pDC, CElecMapView *pView)
{

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy)) return;

	GetRect(&minx,&miny,&maxx,&maxy);
	CPoint pt1,pt2;
	pt1=pView->UPtoLP(minx,miny);
	pt2=pView->UPtoLP(maxx,maxy);
	CRect rt;
	rt.SetRect(pt1,pt2);
	rt.NormalizeRect();

	int high,width;
	high=pView->ULtoLL(m_TextHeight);
	width=pView->ULtoLL(m_TextWidth);

	if((high<1)||(high>1024)||(width<1)||(width>1024))
		return;                 //字体太小或太大将不显示

  CBrush brush,*pOldBrush;
	LOGBRUSH logbrush;

	CFont font; 
	//创建字模  
	if (font.CreateFont(high,width,0,0,50,0,0,0,GB2312_CHARSET,OUT_TT_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FIXED_PITCH,m_TextFont)==FALSE) return;


	logbrush.lbStyle=BS_NULL;
	brush.CreateBrushIndirect(&logbrush);
	pOldBrush=pDC->SelectObject(&brush);
	COLORREF oldTextColor=pDC->SetTextColor(m_color1);
	int oldmode=pDC->SetBkMode(TRANSPARENT);

	CFont* pOldFont=pDC->SelectObject(&font);

	CPoint startPoint;
	startPoint=pView->UPtoLP(m_x0,m_y0);
	const char * p;
	int len;
	unsigned char c1,c2;
	char bz[4];
	float x1=m_x0,y1=m_y0;

	p=(const char *)m_str1;
	len=strlen(p);
	while (len>0)
	{
		c1=*p;
		c2=*(p+1);
		
		if((c1>127)&&(c2>127))  //如果是一个汉字
		{
			strncpy(bz,p,2);//拷贝一个汉字到bz中
			bz[2]=0x00;
			p=p+2;        //跳过汉字指向下一个字符位置
			pDC->TextOut(startPoint.x,startPoint.y,bz); //在屏幕上写这个汉字
			len=len-2;                  //字符数减2
			
			//下一个字符的显示位置
			if (m_FangXiang) 
			  x1=x1+m_TextWidth*2+m_JqWidth;
			else 
				y1=y1+m_TextHeight+m_JqWidth;

			startPoint=pView->UPtoLP(x1,y1);
		} 
		else
		{
			strncpy(bz,p,1);       //拷贝一个字符到bz中
			bz[1]=0x00;
			p++;                   //跳过这一个字符到下一个字符
			if (m_FangXiang)
				pDC->TextOut(startPoint.x,startPoint.y,bz);//在屏幕上写字符
			else
				pDC->TextOut(startPoint.x+(int)(width/2),startPoint.y,bz);//在屏幕上写字符

			len--;                 //字符数减1

			if (m_FangXiang) 
			  x1=x1+m_TextWidth+m_JqWidth;
			else 
				y1=y1+m_TextHeight+m_JqWidth;

			startPoint=pView->UPtoLP(x1,y1);
		}
	}

	pDC->SetBkMode(oldmode);
	pDC->SetTextColor(oldTextColor);
	pDC->SelectObject(pOldBrush);

}

int CTyHMProxy::GetChineseNum(LPCSTR str)
{
	int len;
	const char * p;
	unsigned char c1,c2;
  int ret=0;

	p=(const char *)str;
	len=strlen(p);
	while (len>0)
	{
		c1=*p;
		c2=*(p+1);
		
		if((c1>127)&&(c2>127))  
		{
			p=p+2;       
			len=len-2;   
			ret++;
		} 
		else 
		{
			p=p+1;       
			len--;       
		}
	}

	return ret;
}

void CTyHMProxy::GetRect_Text(float *minX, float *minY, float *maxX, float *maxY)
{

	float maxX0,maxX1,maxY0,maxY1;

	*minX=m_x0;
	*minY=m_y0;

	//求正常显示的字符串的所占的区域
	int chinesenum1=GetChineseNum(m_str1);
	int len1=strlen(m_str1);

	if (m_FangXiang) 
	{
		maxX0=m_x0+(float)((m_TextWidth+m_JqWidth)*len1-chinesenum1*m_JqWidth-m_JqWidth);
		maxY0=m_y0+m_TextHeight;
	}
	else
	{
		maxX0=m_x0+m_TextWidth*2;
		maxY0=m_y0+(float)((len1-chinesenum1)*(m_TextHeight+m_JqWidth)-m_JqWidth);
	}

	//求异常显示的字符串的所占的区域
	int chinesenum=GetChineseNum(m_str2);
	int len=strlen(m_str2);

	if (m_FangXiang) 
	{
		maxX1=m_x0+(float)((m_TextWidth+m_JqWidth)*len-chinesenum*m_JqWidth-m_JqWidth);
		maxY1=m_y0+m_TextHeight;
	}
	else
	{
		maxX1=m_x0+m_TextWidth*2;
		maxY1=m_y0+(float)((len-chinesenum)*(m_TextHeight+m_JqWidth)-m_JqWidth);
	}

	//add for fix bug: maxY0, maxY1 is -1#IND000.
	if ( (len ==0) || (len1 ==0) ){
		*maxX =m_x0;
		*maxY =m_y0;

		return;
	}
	*maxX=max(maxX0,maxX1);
	*maxY=max(maxY0,maxY1);
}


void CTyHMProxy::SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[])
{
	if( m_display!=2) return;
	m_TextHeight=height;
	m_TextWidth=width;
	m_JqWidth=jqwidth;

	m_FangXiang=pl;
	m_color1=color;
	if (strlen(font)<16)
		strcpy(m_TextFont,font);
	Invalidate();
}


void CTyHMProxy::Set_Size_Text(float cx, float cy)
{
	if (m_display!=2) return;
  
	float w1,w2,h1,h2;

	//求正常显示的字符串的字符大小
	int chinesenum=GetChineseNum(m_str1);
	int len=strlen(m_str1);
  
	if (m_FangXiang) 
	{
		h1=cy;
		w1=(cx+(chinesenum+1)*m_JqWidth)/len-m_JqWidth;
	} 
	else 
	{
		w1=cx/2;
		h1=(cy+m_JqWidth)/(len-chinesenum)-m_JqWidth;
	}

	//求异常显示的字符串的字符大小
	chinesenum=GetChineseNum(m_str2);
	len=strlen(m_str2);
  
	if (m_FangXiang) 
	{
		h2=cy;
		w2=(cx+(chinesenum+1)*m_JqWidth)/len-m_JqWidth;
	} 
	else 
	{
		w2=cx/2;
		h2=(cy+m_JqWidth)/(len-chinesenum)-m_JqWidth;
	}

	m_TextHeight=__min(h1,h2);
	m_TextWidth=__min(w1,w2);

}


void CTyHMProxy::ParamReplace_Ty(CStringList& ruleList)
{
	//m_Text
	CString str;
	if (GetStr_From_ReplacRule(m_hmname,str,ruleList)>0)
	{
		int len=str.GetLength();
		if ((len>0)&&(len<=32))
		  strcpy(m_hmname,str);
		SetModifiedFlag();
	}

	if ((m_display==2)&&(strlen(m_str1)>0))
	{
		if (GetStr_From_ReplacRule(m_str1,str,ruleList)>0)
		{
			int len=str.GetLength();
			if ((len>0)&&(len<=32))
				strcpy(m_str1,str);
			SetModifiedFlag();
		}
	}

	if ((m_display==2)&&(strlen(m_str2)>0))
	{
		if (GetStr_From_ReplacRule(m_str2,str,ruleList)>0)
		{
			int len=str.GetLength();
			if ((len>0)&&(len<=32))
				strcpy(m_str2,str);
			SetModifiedFlag();
		}
	}

}




