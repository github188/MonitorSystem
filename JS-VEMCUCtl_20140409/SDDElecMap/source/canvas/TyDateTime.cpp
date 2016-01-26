// TyDateTime.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/TyDateTime.h"
//#include "Sp_drawView.h"
#include "../../ElecMapView.h"
#include "dialog/TyDateTime_PROP.h"
#include "canvas/TYTool.h"
#include "canvas/RtValueTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTyDateTime
IMPLEMENT_SERIAL(CTyDateTime, CTyBase, 0)

CTyDateTime::CTyDateTime(float x0,float y0,float fw,float fh,float fjj)
{
	SYSTEMTIME tm;

	GetLocalTime(&tm);
	wYear=tm.wYear;
	bMonth=(BYTE)tm.wMonth;
	bDay=(BYTE)tm.wDay;
	bHour=(BYTE)tm.wHour;
	bMin=(BYTE)tm.wMinute;
	bSec=(BYTE)tm.wSecond;
	wMSec=tm.wMilliseconds;

	if (true)
	{
		m_TyDateTime.datekind=0;
		m_TyDateTime.timekind=1;
		m_TyDateTime.diskind=0;
		m_TyDateTime.itcolor=0xffffff;
		m_TyDateTime.lgfont.lfHeight=15;
		m_TyDateTime.lgfont.lfWidth=5;
		m_TyDateTime.lgfont.lfEscapement=0;//旋转角度
		m_TyDateTime.lgfont.lfOrientation=0;
		m_TyDateTime.lgfont.lfWeight=FW_MEDIUM;
		m_TyDateTime.lgfont.lfItalic=false;
//		m_TyDateTime.lgfont.lfWeight=700;
//		m_TyDateTime.lgfont.lfItalic=true;
		m_TyDateTime.lgfont.lfUnderline=false;
		m_TyDateTime.lgfont.lfStrikeOut=false;//强调线
		m_TyDateTime.lgfont.lfCharSet=GB2312_CHARSET;
		m_TyDateTime.lgfont.lfOutPrecision=OUT_DEFAULT_PRECIS;//输出精度
		m_TyDateTime.lgfont.lfClipPrecision=CLIP_DEFAULT_PRECIS;//剪裁精度
		m_TyDateTime.lgfont.lfQuality=DEFAULT_QUALITY;
		m_TyDateTime.lgfont.lfPitchAndFamily=DEFAULT_PITCH;//间距和字体族
		sprintf(m_TyDateTime.lgfont.lfFaceName,"宋体");
		m_TyDateTime.rect.x0=x0;
		m_TyDateTime.rect.x1=x0;
		m_TyDateTime.rect.y0=y0;
		m_TyDateTime.rect.y1=y0;
		m_TyDateTime.fontwidth=fw;
		m_TyDateTime.fontheight=fh;
		m_TyDateTime.fontjj=0;
		m_recalrect=true;
		m_recalfont=false;
	
	}
	TransDateTimeToStr();
}

CTyDateTime::CTyDateTime(Stu_TyDateTime * TyDateTime)
{
	SYSTEMTIME tm;

	GetLocalTime(&tm);
	wYear=tm.wYear;
	bMonth=(BYTE)tm.wMonth;
	bDay=(BYTE)tm.wDay;
	bHour=(BYTE)tm.wHour;
	bMin=(BYTE)tm.wMinute;
	bSec=(BYTE)tm.wSecond;
	wMSec=tm.wMilliseconds;

	m_TyDateTime=*TyDateTime;
	m_recalrect=false;
	m_recalfont=false;
	TransDateTimeToStr();
}

CTyDateTime::~CTyDateTime()
{
}




/////////////////////////////////////////////////////////////////////////////
// CTyDateTime message handlers

int CTyDateTime::GetHandleCount()
{
	return 8;
}

PointStruct CTyDateTime::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	nHandle--;
	PointStruct pt;
	pt.x=0;pt.y=0;
	if (nHandle==0)
	{
		pt.x=m_TyDateTime.rect.x0;
		pt.y=m_TyDateTime.rect.y0;
	}
	if (nHandle==4)
	{
		pt.x=m_TyDateTime.rect.x0;
		pt.y=m_TyDateTime.rect.y0+(m_TyDateTime.rect.y1-m_TyDateTime.rect.y0)/2;
	}
	if (nHandle==2)
	{
		pt.x=m_TyDateTime.rect.x0;
		pt.y=m_TyDateTime.rect.y1;
	}
	if (nHandle==3)
	{
		pt.x=m_TyDateTime.rect.x0+(m_TyDateTime.rect.x1-m_TyDateTime.rect.x0)/2;
		pt.y=m_TyDateTime.rect.y1;
	}
	if (nHandle==1)
	{
		pt.x=m_TyDateTime.rect.x1;
		pt.y=m_TyDateTime.rect.y1;
	}
	if (nHandle==5)
	{
		pt.x=m_TyDateTime.rect.x1;
		pt.y=m_TyDateTime.rect.y0+(m_TyDateTime.rect.y1-m_TyDateTime.rect.y0)/2;
	}
	if (nHandle==6)
	{
		pt.x=m_TyDateTime.rect.x1;
		pt.y=m_TyDateTime.rect.y0;
	}
	if (nHandle==7)
	{
		pt.x=m_TyDateTime.rect.x0+(m_TyDateTime.rect.x1-m_TyDateTime.rect.x0)/2;
		pt.y=m_TyDateTime.rect.y0;
	}
	return pt;

}

HCURSOR CTyDateTime::GetHandleCursor(int nHandle)
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

BOOL CTyDateTime::IntersectRect(float x0, float y0, float x1, float y1)
{

	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

//DEL int CTyDateTime::GetCharLen(char *schar)
//DEL {
//DEL 	int len=0;
//DEL 	for (int i=0;i<=255;i++){
//DEL 		if (schar[i]!='\0'){
//DEL 			len=i+1;
//DEL 		}else{
//DEL 			break;
//DEL 		}
//DEL 	}
//DEL 	return len;
//DEL }

DRAW_TY CTyDateTime::GetTyType()
{
  return tyDateTime;
}

int CTyDateTime::GetCharLen(char *schar,int maxlen,int * hzcount)
{
	int len=0;
	*hzcount=0;
	for (int i=0;i<maxlen-1;i++)
	{
		if (schar[i]!='\0')
		{
			len=i+1;
		    if ((schar[i]&0x80)>0) *hzcount=*hzcount+1;
		}
		else
		{
			break;
		}
	}
	*hzcount=*hzcount/2;
	return len;
}
void CTyDateTime::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	if (m_recalrect)
	{
		GetRectFromFont();
		m_recalrect=false;
	}
	*minX=m_TyDateTime.rect.x0;
	*minY=m_TyDateTime.rect.y0;
	*maxX=m_TyDateTime.rect.x1;
	*maxY=m_TyDateTime.rect.y1;
}

void CTyDateTime::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView!=NULL) pView->InvalidateTy(this);
	if (m_TyDateTime.rect.x0+xLen>=0) 
	{
		m_TyDateTime.rect.x0+=xLen;
		m_TyDateTime.rect.x1+=xLen;
	}
	if (m_TyDateTime.rect.y0+yLen>=0)
	{
		m_TyDateTime.rect.y0+=yLen;
		m_TyDateTime.rect.y1+=yLen;
	}
	if (pView!=NULL) pView->InvalidateTy(this);
}

BOOL CTyDateTime::PointInObj(float x, float y,float errRange)
{
  ASSERT(errRange>=0);
  if (m_bDelete) {return false;}
  if ((x>=max(0,m_TyDateTime.rect.x0-errRange))&&
		(y>=max(0,m_TyDateTime.rect.y0-errRange))&&
		(x<=m_TyDateTime.rect.x1+errRange)&&
		(y<=m_TyDateTime.rect.y1+errRange))
  {
    	return true;
  }

  return false;
	
}

CTyBase * CTyDateTime::Clone()
{
	ASSERT_VALID(this);
	Stu_TyDateTime  tempTyDateTime;
	tempTyDateTime=m_TyDateTime;

	CTyDateTime * pClone = new CTyDateTime(&tempTyDateTime);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

void CTyDateTime::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	nHandle--;
	ASSERT(pView!=NULL);
	pView->InvalidateTy(this);
	RectStruct temprect,temprect2;
	BOOL x0move,y0move,x1move,y1move;
	x0move=false;x1move=false;
	y0move=false;y1move=false;
	temprect=m_TyDateTime.rect;
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
	m_TyDateTime.rect=temprect;
	m_recalfont=true;
	m_recalrect=false;
	RefreshIt();
	CTyDateTimeTool::canuse=true;
	CTyDateTimeTool::m_TyDateTime=this->m_TyDateTime;
	CTyDateTimeTool::m_TyDateTime.fontwidth=(float)(pView->ULtoLL(m_TyDateTime.fontwidth));
	CTyDateTimeTool::m_TyDateTime.fontheight=(float)(pView->ULtoLL(m_TyDateTime.fontheight));
	CTyDateTimeTool::m_TyDateTime.fontjj=(float)(pView->ULtoLL(m_TyDateTime.fontjj));
}

void CTyDateTime::DrawDC(CDC *pDC, CElecMapView *pView)
{
	if (m_bDelete) return ;
	if (m_recalrect)
	{
		GetRectFromFont();
		m_recalrect=false;
	}

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy)) return;
	
	GetRect(&minx,&miny,&maxx,&maxy);
	CPoint pt1,pt2,pt3,pt4;
	pt1=pView->UPtoLP(m_TyDateTime.rect.x0,m_TyDateTime.rect.y0);
	pt2=pView->UPtoLP(m_TyDateTime.rect.x0,m_TyDateTime.rect.y1);
	pt3=pView->UPtoLP(m_TyDateTime.rect.x1,m_TyDateTime.rect.y1);
	pt4=pView->UPtoLP(m_TyDateTime.rect.x1,m_TyDateTime.rect.y0);

	if ((pt3.x-pt1.x<=2)&&(pt3.y-pt1.y<=2)) return;

	if (m_recalfont)
	{
		m_recalfont=false;
		GetFontExtractWidth(pDC,pView);
	}

	//if ((pt3.x-pt1.x>1024)||(pt3.y-pt1.y>768)) return;
	char sss[256];
	sprintf(sss,"%s",disstr);
	int length,hz;
	length=GetCharLen(sss,256,&hz);
	if ((pt3.x-pt1.x>(TyDateTime_MINDISWIDTH*(length+1)
		+pView->ULtoLL(m_TyDateTime.fontjj)*(length-hz)))&&
		(pt3.y-pt1.y>TyDateTime_MINDISHEIGHT))
	{
		m_TyDateTime.lgfont.lfHeight=pView->ULtoLL(m_TyDateTime.fontheight);
		m_TyDateTime.lgfont.lfWidth=pView->ULtoLL(m_TyDateTime.fontwidth);
		LOGFONT tempfont=m_TyDateTime.lgfont;
		CFont m_font;
		m_font.CreateFontIndirect(&tempfont);
		CFont * m_oldfont=pDC->SelectObject(&m_font);
		int oldmode=pDC->GetBkMode();
		pDC->SetBkMode(TRANSPARENT);
		int x1=pt1.x;int backx;
		int ljj=pView->ULtoLL(m_TyDateTime.fontjj);
		int lwidth=pView->ULtoLL(m_TyDateTime.fontwidth);
		char s[256];
		sprintf(s,"%s",disstr);
		DrawText(pDC,s,length,ljj,lwidth,x1,pt1.y,m_TyDateTime.itcolor,&backx);
		pDC->SetBkMode(oldmode);
		pDC->SelectObject(m_oldfont);
		/*if ((pt3.x-pt1.x<size.cx))
		{
			pt3.x=pt1.x+size.cx;
			float x,y;
			pView->LPtoUP(CPoint(size.cx),&x,&y);
			m_TyDateTime.rect.x1=m_TyDateTime.rect.x0+x;
		}*/
	}

	CPen pen(PS_DOT ,1,COLORREF(0x0000ff));
    CPen * pOldPen=pDC->SelectObject(&pen);
	int holdDrawMode=pDC->SetROP2(R2_NOTXORPEN);

/*	pDC->MoveTo(pt1); 
	pDC->LineTo(pt2);
	pDC->LineTo(pt3);
	pDC->LineTo(pt4);
	pDC->LineTo(pt1);*/

	pDC->SetROP2(holdDrawMode);
	pDC->SelectObject(pOldPen);

}


void CTyDateTime::GetFontExtractWidth(CDC *pDC,CElecMapView *pView)
{
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy)) 
	{
		return;
	}

	GetRect(&minx,&miny,&maxx,&maxy);
	CPoint pt1,pt2,pt3,pt4;
	pt1=pView->UPtoLP(m_TyDateTime.rect.x0,m_TyDateTime.rect.y0);
	pt2=pView->UPtoLP(m_TyDateTime.rect.x0,m_TyDateTime.rect.y1);
	pt3=pView->UPtoLP(m_TyDateTime.rect.x1,m_TyDateTime.rect.y1);
	pt4=pView->UPtoLP(m_TyDateTime.rect.x1,m_TyDateTime.rect.y0);
	char sss[256];
	sprintf(sss,"%s",disstr);
	int length,hz;
	length=GetCharLen(sss,256,&hz);
	if ((pt3.x-pt1.x>(TyDateTime_MINDISWIDTH*(length+1)
		+pView->ULtoLL(m_TyDateTime.fontjj)*(length-hz)))&&
		(pt3.y-pt1.y>TyDateTime_MINDISHEIGHT))
	{
		m_TyDateTime.lgfont.lfHeight=pt3.y-pt1.y;
		m_TyDateTime.lgfont.lfWidth=(LONG)(pt3.x-pt1.x
			-pView->ULtoLL(m_TyDateTime.fontjj)*(length-hz))/
			(length);
		while (true) 
		{
			CFont m_font;
			LOGFONT tempfont;
			tempfont=m_TyDateTime.lgfont;
			m_font.CreateFontIndirect(&tempfont);
			CFont * m_oldfont=memdc.SelectObject(&m_font);
			CSize size;
			size=memdc.GetOutputTextExtent(disstr,length);//+2);
			size.cx=size.cx;//-m_TyDateTime.lgfont.lfWidth*1;//+qq;
			memdc.SelectObject(m_oldfont);
			//if ((minlen<=(pt3.x-pt1.x)*FontSizePercent)||(m_TyDateTime.lgfont.lfWidth<=0))
			if (((size.cx*(length+1)/(length))<=
				(pt3.x-pt1.x
				-pView->ULtoLL(m_TyDateTime.fontjj)*(length-hz)))||(m_TyDateTime.lgfont.lfWidth<=0))
			{
				m_TyDateTime.fontheight=pView->LLtoUL(m_TyDateTime.lgfont.lfHeight);
				m_TyDateTime.fontwidth=pView->LLtoUL(m_TyDateTime.lgfont.lfWidth);
				break;
			}
			else
			{
				m_TyDateTime.lgfont.lfWidth--;
			}
		}
	}

	CTyDateTimeTool::canuse=true;
	CTyDateTimeTool::m_TyDateTime=this->m_TyDateTime;
	CTyDateTimeTool::m_TyDateTime.fontwidth=(float)(pView->ULtoLL(m_TyDateTime.fontwidth));
	CTyDateTimeTool::m_TyDateTime.fontheight=(float)(pView->ULtoLL(m_TyDateTime.fontheight));
	CTyDateTimeTool::m_TyDateTime.fontjj=(float)(pView->ULtoLL(m_TyDateTime.fontjj));

}

void CTyDateTime::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("日期/时间对象..") );

    CTyDateTime_Prop dlg0;
	dlg0.SetPara(m_TyDateTime.itcolor,m_TyDateTime.lgfont,m_TyDateTime.rect.x0,
		m_TyDateTime.rect.y0,m_TyDateTime.fontwidth,m_TyDateTime.fontheight,m_TyDateTime.fontjj,
		m_TyDateTime.timekind,m_TyDateTime.datekind,m_TyDateTime.diskind,wYear,
		bMonth,bDay,bHour,bMin,bSec,wMSec);
	sheet.AddPage( &dlg0);
	if (sheet.DoModal() != IDOK)
		return;
	RefreshIt();	
	dlg0.GetPara(&m_TyDateTime.itcolor,&m_TyDateTime.lgfont,&m_TyDateTime.rect.x0,&
		m_TyDateTime.rect.y0,&m_TyDateTime.fontwidth,&m_TyDateTime.fontheight,&m_TyDateTime.fontjj,&
		m_TyDateTime.timekind,&m_TyDateTime.datekind,&m_TyDateTime.diskind);
	TransDateTimeToStr();
	SetModifiedFlag();
	m_recalrect=true;
	RefreshIt();	
	CTyDateTimeTool::canuse=true;
	CTyDateTimeTool::m_TyDateTime=this->m_TyDateTime;
	CTyDateTimeTool::m_TyDateTime.fontwidth=(float)(pView->ULtoLL(m_TyDateTime.fontwidth));
	CTyDateTimeTool::m_TyDateTime.fontheight=(float)(pView->ULtoLL(m_TyDateTime.fontheight));
	CTyDateTimeTool::m_TyDateTime.fontjj=(float)(pView->ULtoLL(m_TyDateTime.fontjj));

}

void CTyDateTime::RefreshIt()
{
	Invalidate();
}

void CTyDateTime::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_TyDateTime.rect,sizeof(m_TyDateTime.rect));
		file->Write((unsigned char *)&m_TyDateTime.fontwidth,sizeof(m_TyDateTime.fontwidth));
		file->Write((unsigned char *)&m_TyDateTime.fontheight,sizeof(m_TyDateTime.fontheight));
		file->Write((unsigned char *)&m_TyDateTime.fontjj,sizeof(m_TyDateTime.fontjj));
		file->Write((unsigned char *)&m_TyDateTime.lgfont,sizeof(m_TyDateTime.lgfont));
		file->Write((unsigned char *)&m_TyDateTime.itcolor,sizeof(m_TyDateTime.itcolor));
		file->Write((unsigned char *)&m_TyDateTime.datekind,sizeof(m_TyDateTime.datekind));
		file->Write((unsigned char *)&m_TyDateTime.timekind,sizeof(m_TyDateTime.timekind));
		file->Write((unsigned char *)&m_TyDateTime.diskind,sizeof(m_TyDateTime.diskind));
	} 
	else
	{
		m_recalrect=false;
		m_recalfont=false;
		file->Read((unsigned char *)&m_TyDateTime.rect,sizeof(m_TyDateTime.rect));
		file->Read((unsigned char *)&m_TyDateTime.fontwidth,sizeof(m_TyDateTime.fontwidth));
		file->Read((unsigned char *)&m_TyDateTime.fontheight,sizeof(m_TyDateTime.fontheight));
		file->Read((unsigned char *)&m_TyDateTime.fontjj,sizeof(m_TyDateTime.fontjj));
		file->Read((unsigned char *)&m_TyDateTime.lgfont,sizeof(m_TyDateTime.lgfont));
		file->Read((unsigned char *)&m_TyDateTime.itcolor,sizeof(m_TyDateTime.itcolor));
		file->Read((unsigned char *)&m_TyDateTime.datekind,sizeof(m_TyDateTime.datekind));
		file->Read((unsigned char *)&m_TyDateTime.timekind,sizeof(m_TyDateTime.timekind));
		file->Read((unsigned char *)&m_TyDateTime.diskind,sizeof(m_TyDateTime.diskind));
		TransDateTimeToStr();
	}
}

void CTyDateTime::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if(ar.IsStoring())  
	{
//		ar<<m_TyDateTime;
		ar.Write(&m_TyDateTime,sizeof(Stu_TyDateTime));
	}
	else
	{
//		ar>>m_TyDateTime;
		m_recalrect=false;
		m_recalfont=false;
		ar.Read(&m_TyDateTime,sizeof(Stu_TyDateTime));
		TransDateTimeToStr();
	}
}

 void CTyDateTime::DrawText(CDC * pDC,char *s, int count, int jj, int width,int x0, int y0, COLORREF itcolor, int *x1)
 {
 	pDC->SetTextColor(itcolor);
 	int i;int xx=x0;CString tmp="";int hz=0;
 	for (i=0;i<count;i++)
 	{
 		if (s[i]==0) break;
		if (hz==1)
 		{
 		}
		else
		{
 			if ((s[i]&0x80)>0)
 			{
 				if (hz==0)
 				{
 					tmp=tmp+s[i];
 					hz=1;
 					continue;
 				}
 			}
 		}

 		tmp=tmp+s[i];
 		pDC->TextOut(xx,y0,tmp);
 		tmp="";
 		xx=xx+jj+width*(1+hz);
 		hz=0;
 	}

 	*x1=xx;
 }

void CTyDateTime::GetRectFromFont()
{
	m_recalrect=false;
	float tempx0=m_TyDateTime.rect.x0;
	float tempy0=m_TyDateTime.rect.y0;
	char sss[256];
	sprintf(sss,"%s",disstr);
	int length,hz;
	length=GetCharLen(sss,256,&hz);
	m_TyDateTime.rect.x1=tempx0+m_TyDateTime.fontwidth*(length+1)
		+m_TyDateTime.fontjj*(length-hz);
	m_TyDateTime.rect.y1=m_TyDateTime.rect.y0+m_TyDateTime.fontheight*(1);
}

void CTyDateTime::TransDateTimeToStr()
{
	CString datestr,timestr;
	datestr="";
	timestr="";
	if ((m_TyDateTime.datekind==0)&&(m_TyDateTime.timekind==0)) m_TyDateTime.timekind=1;
	if (m_TyDateTime.datekind!=0)
	{
		if (m_TyDateTime.datekind>7) m_TyDateTime.datekind=0;
		if (m_TyDateTime.datekind==1) datestr.Format("%04d-%02d-%02d",wYear,bMonth,bDay);//2002-3-22
		if (m_TyDateTime.datekind==2) datestr.Format("%04d/%02d/%02d",wYear,bMonth,bDay);//2002/3/22
		if (m_TyDateTime.datekind==3) datestr.Format("%02d/%02d/%04d",bMonth,bDay,wYear);//3/22/2002
		if (m_TyDateTime.datekind==4) datestr.Format("%02d.%02d.%04d",bMonth,bDay,wYear);//3.22.2002
		if (m_TyDateTime.datekind==5) datestr.Format("%02d-%02d-%02d",wYear-2000,bMonth,bDay);//02.3.22
		if (m_TyDateTime.datekind==6) datestr.Format("%04d年%02d月",wYear,bMonth);//2002年3月
		if (m_TyDateTime.datekind==7) datestr.Format("%04d年%02d月%02d日",wYear,bMonth,bDay);//2002年3月22日
	}
	if (m_TyDateTime.timekind!=0)
	{
		if (m_TyDateTime.timekind>4) m_TyDateTime.timekind=0;
		if (m_TyDateTime.timekind==1) timestr.Format("%02d:%02d:%02d",bHour,bMin,bSec);//16:13:26
		if (m_TyDateTime.timekind==2) timestr.Format("%02d:%02d",bHour,bMin);//16:13
		if (m_TyDateTime.timekind==3) timestr.Format("%02d时%02d分%02d秒",bHour,bMin,bSec);//16时13分26秒
		if (m_TyDateTime.timekind==4) timestr.Format("%02d时%02d分",bHour,bMin);//16时13分
	}
	if (m_TyDateTime.diskind==1)
	{
		disstr="";
		disstr=datestr;
		if (disstr.GetLength()!=0)
		{
			disstr=disstr+" ";
		}
		disstr=disstr+timestr;
	}
	else
	{
		disstr="";
		disstr=timestr;
		if (disstr.GetLength()!=0)
		{
			disstr=disstr+" ";
		}
		disstr=disstr+datestr;
	}
}

void CTyDateTime::SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[])
{
	m_TyDateTime.fontwidth=width;
	m_TyDateTime.fontheight=height;
	m_TyDateTime.fontjj=jqwidth;
	
	if (strlen(font)<32)
		strcpy(m_TyDateTime.lgfont.lfFaceName,font);//字体

	m_TyDateTime.itcolor=color;
	m_recalrect=true;

	Invalidate();
}

void CTyDateTime::Set_Ty_Color(COLORREF color, CElecMapView *pView)
{
	if (m_TyDateTime.itcolor==color) return;
	m_TyDateTime.itcolor=color;
	SetModifiedFlag();

	ASSERT(pView!=NULL);
	if (pView==NULL) return;

	pView->InvalidateTy(this);

}
