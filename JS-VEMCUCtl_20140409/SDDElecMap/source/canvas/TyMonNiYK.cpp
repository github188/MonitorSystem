#include "stdafx.h"
#include "canvas/tymonniyk.h"
#include "dialog/TyHMProxyProp.h"
#include "dialog/PropPositionRect.h"
#include "dialog/PropShowOption.h"

//////////////////////////////////////////////////////////////////////
// CTyMonNiYK Class
//////////////////////////////////////////////////////////////////////


IMPLEMENT_SERIAL(CTyMonNiYK, CTyBase, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CTyMonNiYK::CTyMonNiYK(void)
{
	ZeroMemory(m_str1,sizeof(char)*33);
	ZeroMemory(m_str2,sizeof(char)*33);
	ZeroMemory(m_TextFont,sizeof(char)*16);
}

CTyMonNiYK::~CTyMonNiYK(void)
{

}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTyMonNiYK::CTyMonNiYK(float x0,float y0,float x1,float y1,int display,
											 LPCSTR str1,LPCSTR str2,COLORREF Color1,COLORREF Color2, 
											 BOOL FangXiang,float TextHeight,float TextWidth,
											 float JqWidth,LPCSTR TextFont,BOOL bTranslate1,
											 BOOL bTranslate2,int vId,CHMLayer* pLayer)
	:CTyBase(vId,pLayer)
{
	m_x0=x0;
	m_y0=y0;
	m_x1=x1;
	m_y1=y1;

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


void CTyMonNiYK::Set_Ty_Color(COLORREF color, CElecMapView *pView)
{
	if (m_display==0) return;
	if (m_color1==color) return;
	m_color1=color;
	SetModifiedFlag();

	ASSERT(pView!=NULL);
	if (pView==NULL) return;

	pView->InvalidateTy(this);
}


void CTyMonNiYK::DrawDC(CDC *pDC, CElecMapView *pView)
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

PointStruct CTyMonNiYK::GetHandle(int nHandle)
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

int CTyMonNiYK::GetHandleCount()
{
	return 8;
}

HCURSOR CTyMonNiYK::GetHandleCursor(int nHandle)
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

void CTyMonNiYK::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
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


void CTyMonNiYK::Ty_MoveHandleTo_Text(int nHandle, PointStruct point, CElecMapView *pView, UINT nFlags)
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

void CTyMonNiYK::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);

	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_x0,sizeof(m_x0));
		file->Write((unsigned char *)&m_y0,sizeof(m_y0));
		file->Write((unsigned char *)&m_x1,sizeof(m_x1));
		file->Write((unsigned char *)&m_y1,sizeof(m_y1));

		file->Write((unsigned char *)&m_display,sizeof(m_display));
		file->Write((unsigned char *)m_str1,sizeof(char)*33);
		file->Write((unsigned char *)m_str2,sizeof(char)*33);
		file->Write((unsigned char *)&m_color1,sizeof(m_color1));
		file->Write((unsigned char *)&m_color2,sizeof(m_color2));
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
	
		file->Read((unsigned char *)&m_display,sizeof(m_display));
		file->Read((unsigned char *)m_str1,sizeof(char)*33);
		file->Read((unsigned char *)m_str2,sizeof(char)*33);
		file->Read((unsigned char *)&m_color1,sizeof(m_color1));
		file->Read((unsigned char *)&m_color2,sizeof(m_color2));
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

void CTyMonNiYK::Ty_Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar<<m_x0<<m_y0<<m_x1<<m_y1;
		ar<<m_display;
		ar.Write((unsigned char *)m_str1,sizeof(char)*33);
		ar.Write((unsigned char *)m_str2,sizeof(char)*33);
		ar<<m_color1<<m_color2<<m_FangXiang<<m_TextHeight<<m_TextWidth<<m_JqWidth;
		ar.Write((unsigned char *)m_TextFont,sizeof(char)*16);
		ar<<m_bTranslate1<<m_bTranslate2;
	} 
	else 
	{
		UnRegister();
		ar>>m_x0>>m_y0>>m_x1>>m_y1;
		ar>>m_display;
		ar.Read((unsigned char *)m_str1,sizeof(char)*33);
		ar.Read((unsigned char *)m_str2,sizeof(char)*33);
		ar>>m_color1>>m_color2>>m_FangXiang>>m_TextHeight>>m_TextWidth>>m_JqWidth;
		ar.Read((unsigned char *)m_TextFont,sizeof(char)*16);
		ar>>m_bTranslate1>>m_bTranslate2;
		Register();
	}
}

DRAW_TY CTyMonNiYK::GetTyType()
{
	return tyMonNiYK;
}

void CTyMonNiYK::SetDefaultProp(CElecMapView * pView)
{
	ASSERT(pView!=NULL);
	if (pView==NULL) return;
	/*
	CTyMonNiYKTool::c_bTranslate1=m_bTranslate1;
	CTyMonNiYKTool::c_bTranslate2=m_bTranslate2;
	CTyMonNiYKTool::c_Color1=m_color1;
	CTyMonNiYKTool::c_Color2=m_color2;
	CTyMonNiYKTool::c_display=m_display;
	CTyMonNiYKTool::c_FangXiang=m_FangXiang;
	
	CTyMonNiYKTool::c_Height=pView->ULtoLL(m_y1-m_y0);
	CTyMonNiYKTool::c_Width=pView->ULtoLL(m_x1-m_x0);
	CTyMonNiYKTool::c_JqWidth=pView->ULtoLL(m_JqWidth);
	strcpy(CTyMonNiYKTool::c_str1,m_str1);
	strcpy(CTyMonNiYKTool::c_str2,m_str2);
	strcpy(CTyMonNiYKTool::c_TextFont,m_TextFont);
	CTyMonNiYKTool::c_TextHeight=pView->ULtoLL(m_TextHeight);
	CTyMonNiYKTool::c_TextWidth=pView->ULtoLL(m_TextWidth);
	*/
}


void CTyMonNiYK::UnRegister()
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

void CTyMonNiYK::Register()
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


void CTyMonNiYK::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("模拟控制对象..") );

	CPropShowOption dlg;

	dlg.SetProp(m_display,m_str1,
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
		
		dlg.GetProp(m_display,m_str1,m_str2,
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

BOOL CTyMonNiYK::PointInObj(float x, float y, float errRange)
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

BOOL CTyMonNiYK::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

CTyBase * CTyMonNiYK::Clone()
{
	ASSERT_VALID(this);


	CTyMonNiYK* pClone = new CTyMonNiYK(m_x0,m_y0,m_x1,m_y1,m_display,
																			m_str1,m_str2,m_color1,m_color2,
																			m_FangXiang,m_TextHeight,m_TextWidth,
																			m_JqWidth,m_TextFont,m_bTranslate1,m_bTranslate2);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;

}

void CTyMonNiYK::GetRect(float *minX, float *minY, float *maxX, float *maxY)
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

void CTyMonNiYK::Ty_Move(float xLen, float yLen, CElecMapView *pView)
{
	if (pView!=NULL) pView->InvalidateTy(this);
	m_x0+=xLen;
	m_x1+=xLen;
	m_y0+=yLen;
	m_y1+=yLen;
	if (pView!=NULL) pView->InvalidateTy(this);
}

void CTyMonNiYK::DrawDC_Bmp(CDC *pDC, CElecMapView *pView)
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

void CTyMonNiYK::DrawDC_Vecter(CDC *pDC, CElecMapView *pView)
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

void CTyMonNiYK::DrawDC_Text(CDC *pDC, CElecMapView *pView)
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

int CTyMonNiYK::GetChineseNum(LPCSTR str)
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

void CTyMonNiYK::GetRect_Text(float *minX, float *minY, float *maxX, float *maxY)
{
	float maxX0,maxX1,maxY0,maxY1;

	*minX=m_x0;
	*minY=m_y0;

	//求正常显示的字符串的所占的区域
	int chinesenum=GetChineseNum(m_str1);
	int len=strlen(m_str1);

	if (m_FangXiang) 
	{
		maxX0=m_x0+(float)((m_TextWidth+m_JqWidth)*len-chinesenum*m_JqWidth-m_JqWidth);
		maxY0=m_y0+m_TextHeight;
	}
	else
	{
		maxX0=m_x0+m_TextWidth*2;
		maxY0=m_y0+(float)((len-chinesenum)*(m_TextHeight+m_JqWidth)-m_JqWidth);
	}

	//求异常显示的字符串的所占的区域
	chinesenum=GetChineseNum(m_str2);
	len=strlen(m_str2);

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

	*maxX=max(maxX0,maxX1);
	*maxY=max(maxY0,maxY1);
}


void CTyMonNiYK::Set_Size_Text(float cx, float cy)
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

void CTyMonNiYK::SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[])
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
