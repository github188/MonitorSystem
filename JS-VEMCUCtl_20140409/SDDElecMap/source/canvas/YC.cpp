// YC.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/YC.h"
//#include "Sp_drawView.h"
#include "../../ElecMapView.h"
#include "dialog/YC_PROP_INFO.h"
#include "dialog/YC_PROP_DIS.h"
#include "dialog/YC_PROP_WARN.h"
#include "canvas/TYTool.h"
#include "canvas/RtValueTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYC
IMPLEMENT_SERIAL(CYC, CTyBase, 0)

CYC::CYC(float x0,float y0,float fw,float fh,float jj)
{
	if (true)
	{
	   	sprintf(m_yc.ename,"DEFAULTENAME");
		m_yc.digitlength=6;
		sprintf(m_yc.unit,"");
		m_yc.unitcolor=0xffffff;
		sprintf(m_yc.hint,"");
		m_yc.hintcolor=0xffffff;
		m_yc.lgfont.lfHeight=15;
		m_yc.lgfont.lfWidth=5;
		m_yc.lgfont.lfEscapement=0;//旋转角度
		m_yc.lgfont.lfOrientation=0;
		m_yc.lgfont.lfWeight=FW_MEDIUM;
		m_yc.lgfont.lfItalic=false;
//		m_yc.lgfont.lfWeight=700;
//		m_yc.lgfont.lfItalic=true;
		m_yc.lgfont.lfUnderline=false;
		m_yc.lgfont.lfStrikeOut=false;//强调线
		m_yc.lgfont.lfCharSet=GB2312_CHARSET;
		m_yc.lgfont.lfOutPrecision=OUT_DEFAULT_PRECIS;//输出精度
		m_yc.lgfont.lfClipPrecision=CLIP_DEFAULT_PRECIS;//剪裁精度
		m_yc.lgfont.lfQuality=DEFAULT_QUALITY;
		m_yc.lgfont.lfPitchAndFamily=DEFAULT_PITCH;//间距和字体族
		sprintf(m_yc.lgfont.lfFaceName,"宋体");
		m_yc.levelupcolor=0x0000ff;
		m_yc.leveldowncolor=0x0000ff;
		m_yc.normalcolor=0xff0000;
		m_yc.disablecolor=0xffffff;
		m_yc.warnupenable=0;
		m_yc.warnuplevels=0;
		m_yc.warnupkind=0;
		sprintf(m_yc.warnupwave,"");
		m_yc.warndownenable=0;
		m_yc.warndownlevels=0;
		m_yc.warndownkind=0;
		sprintf(m_yc.warndownwave,"");
		m_yc.hintenable=0;
		
		m_yc.rect.x0=x0;
		m_yc.rect.x1=x0;
		m_yc.rect.y0=y0;
		m_yc.rect.y1=y0;
		m_yc.fontwidth=fw;
		m_yc.fontheight=fh;
		m_yc.fontjj=0;
		m_yc.hintdis=0;
		m_yc.unitdis=0;
		m_recalrect=true;
		m_recalfont=false;
	
	}
}

CYC::CYC(Stu_YC * yc)
{
	m_yc=*yc;
	memcpy(m_yc.ename,yc->ename,sizeof(char)*17);
	memcpy(m_yc.hint,yc->hint,sizeof(char)*33);
	memcpy(m_yc.unit,yc->unit,sizeof(char)*17);
	memcpy(m_yc.warnupwave,yc->warnupwave,sizeof(char)*33);
	memcpy(m_yc.warndownwave,yc->warndownwave,sizeof(char)*33);
	m_recalrect=false;
	m_recalfont=false;
}

CYC::~CYC()
{
}




/////////////////////////////////////////////////////////////////////////////
// CYC message handlers

int CYC::GetHandleCount()
{
	return 8;
}

PointStruct CYC::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	nHandle--;
	PointStruct pt;
	pt.x=0;pt.y=0;
	if (nHandle==0)
	{
		pt.x=m_yc.rect.x0;
		pt.y=m_yc.rect.y0;
	}
	if (nHandle==4)
	{
		pt.x=m_yc.rect.x0;
		pt.y=m_yc.rect.y0+(m_yc.rect.y1-m_yc.rect.y0)/2;
	}
	if (nHandle==2)
	{
		pt.x=m_yc.rect.x0;
		pt.y=m_yc.rect.y1;
	}
	if (nHandle==3)
	{
		pt.x=m_yc.rect.x0+(m_yc.rect.x1-m_yc.rect.x0)/2;
		pt.y=m_yc.rect.y1;
	}
	if (nHandle==1)
	{
		pt.x=m_yc.rect.x1;
		pt.y=m_yc.rect.y1;
	}
	if (nHandle==5)
	{
		pt.x=m_yc.rect.x1;
		pt.y=m_yc.rect.y0+(m_yc.rect.y1-m_yc.rect.y0)/2;
	}
	if (nHandle==6)
	{
		pt.x=m_yc.rect.x1;
		pt.y=m_yc.rect.y0;
	}
	if (nHandle==7)
	{
		pt.x=m_yc.rect.x0+(m_yc.rect.x1-m_yc.rect.x0)/2;
		pt.y=m_yc.rect.y0;
	}
	return pt;

}

HCURSOR CYC::GetHandleCursor(int nHandle)
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

BOOL CYC::IntersectRect(float x0, float y0, float x1, float y1)
{

	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

DRAW_TY CYC::GetTyType()
{
  return tyYC;
}

int CYC::GetCharLen(char *schar,int maxlen,int * hzcount)
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

void CYC::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	if (m_recalrect)
	{
		GetRectFromFont();
		m_recalrect=false;
	}
	*minX=m_yc.rect.x0;
	*minY=m_yc.rect.y0;
	*maxX=m_yc.rect.x1;
	*maxY=m_yc.rect.y1;
}

void CYC::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView!=NULL) pView->InvalidateTy(this);
	if (m_yc.rect.x0+xLen>=0) 
	{
		m_yc.rect.x0+=xLen;
		m_yc.rect.x1+=xLen;
	}
	if (m_yc.rect.y0+yLen>=0)
	{
		m_yc.rect.y0+=yLen;
		m_yc.rect.y1+=yLen;
	}
	if (pView!=NULL) pView->InvalidateTy(this);
}

BOOL CYC::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange>=0);
	if (m_bDelete) {return false;}
	if ((x>=max(0,m_yc.rect.x0-errRange))&&
		(y>=max(0,m_yc.rect.y0-errRange))&&
		(x<=m_yc.rect.x1+errRange)&&
		(y<=m_yc.rect.y1+errRange))
  {
    	return true;
  }

  return false;
	
}

CTyBase * CYC::Clone()
{
	ASSERT_VALID(this);
	Stu_YC  tempyc;
	tempyc=m_yc;
	memcpy(tempyc.ename,m_yc.ename,17*sizeof(char));
	memcpy(tempyc.unit,m_yc.unit,17*sizeof(char));
	memcpy(tempyc.hint,m_yc.hint,33*sizeof(char));
	memcpy(tempyc.warndownwave,m_yc.warndownwave,33*sizeof(char));
	memcpy(tempyc.warnupwave,m_yc.warnupwave,33*sizeof(char));

	CYC * pClone = new CYC(&tempyc);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

void CYC::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	nHandle--;
	ASSERT(pView!=NULL);
	pView->InvalidateTy(this);
	RectStruct temprect,temprect2;
	BOOL x0move,y0move,x1move,y1move;
	x0move=false;x1move=false;
	y0move=false;y1move=false;
	temprect=m_yc.rect;
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
	m_yc.rect=temprect;
	m_recalfont=true;
	m_recalrect=false;
	RefreshIt();
}

void CYC::DrawDC(CDC *pDC, CElecMapView *pView)
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
	pt1=pView->UPtoLP(m_yc.rect.x0,m_yc.rect.y0);
	pt2=pView->UPtoLP(m_yc.rect.x0,m_yc.rect.y1);
	pt3=pView->UPtoLP(m_yc.rect.x1,m_yc.rect.y1);
	pt4=pView->UPtoLP(m_yc.rect.x1,m_yc.rect.y0);

	if ((pt3.x-pt1.x<=2)&&(pt3.y-pt1.y<=2)) return;

	int hinthz,unithz;int hintdis,unitdis;
	int hintcount=GetCharLen(m_yc.hint,33,&hinthz);
	int unitcount=GetCharLen(m_yc.unit,17,&unithz);
	
	hintdis=0;unitdis=0;
	if (hintcount>0) hintdis=pView->ULtoLL(m_yc.hintdis);
	if (unitcount>0) unitdis=pView->ULtoLL(m_yc.unitdis);
	
	if (m_recalfont)
	{
		m_recalfont=false;
		GetFontExtractWidth(pDC,pView);
	}

	if ((pt3.x-pt1.x>(YC_MINDISWIDTH*(m_yc.digitlength+hintcount+unitcount+1)
		+pView->ULtoLL(m_yc.fontjj)*(m_yc.digitlength+hintcount+unitcount-hinthz-unithz)))&&
		(pt3.y-pt1.y>YC_MINDISHEIGHT))
	{
		CString ttstr=m_yc.ename;
		for (int kk2=0;kk2<m_yc.digitlength;kk2++)
		{
			ttstr=ttstr+" ";
		}
		char s[256];
		sprintf(s,"%s",ttstr);
		s[m_yc.digitlength]='\0';
		m_yc.lgfont.lfHeight=pView->ULtoLL(m_yc.fontheight);
		m_yc.lgfont.lfWidth=pView->ULtoLL(m_yc.fontwidth);
		LOGFONT tempfont=m_yc.lgfont;
		CFont m_font;
		m_font.CreateFontIndirect(&tempfont);
		CFont * m_oldfont=pDC->SelectObject(&m_font);
		int oldmode=pDC->GetBkMode();
		pDC->SetBkMode(TRANSPARENT);
		int x1=pt1.x;int backx;
		int ljj=pView->ULtoLL(m_yc.fontjj);
		int lwidth=pView->ULtoLL(m_yc.fontwidth);
	
		if (hintcount!=0)
		{
			DrawText(pDC,m_yc.hint,hintcount,ljj,lwidth,x1,pt1.y,m_yc.hintcolor,&backx);
			x1=backx;
		}
		
		x1=x1+hintdis;
		DrawText(pDC,s,m_yc.digitlength,ljj,lwidth,x1,pt1.y,m_yc.normalcolor,&backx);
		x1=backx;
		x1=x1+unitdis;
		if (unitcount!=0)
		{
			DrawText(pDC,m_yc.unit,unitcount,ljj,lwidth,x1,pt1.y,m_yc.unitcolor,&backx);
			x1=backx;
		}
		pDC->SetBkMode(oldmode);
		pDC->SelectObject(m_oldfont);
	}

	CPen pen(PS_DOT ,1,COLORREF(0x0000ff));
    CPen * pOldPen=pDC->SelectObject(&pen);
	int holdDrawMode=pDC->SetROP2(R2_NOTXORPEN);

	pDC->SetROP2(holdDrawMode);
	pDC->SelectObject(pOldPen);
}

void CYC::GetFontExtractWidth(CDC *pDC,CElecMapView *pView)
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
	pt1=pView->UPtoLP(m_yc.rect.x0,m_yc.rect.y0);
	pt2=pView->UPtoLP(m_yc.rect.x0,m_yc.rect.y1);
	pt3=pView->UPtoLP(m_yc.rect.x1,m_yc.rect.y1);
	pt4=pView->UPtoLP(m_yc.rect.x1,m_yc.rect.y0);
	int hintcount,unitcount,hinthz,unithz;int hintdis,unitdis;
	hintcount=GetCharLen(m_yc.hint,33,&hinthz);
	unitcount=GetCharLen(m_yc.unit,17,&unithz);
	hintdis=0;unitdis=0;
	if (hintcount>0) hintdis=pView->ULtoLL(m_yc.hintdis);
	if (unitcount>0) unitdis=pView->ULtoLL(m_yc.unitdis);
	char s[256];
	memcpy(s,YC_STRING,sizeof(char)*m_yc.digitlength);
	s[m_yc.digitlength]='\0';
	CString tmpstr;
	tmpstr="";
	tmpstr=tmpstr+m_yc.hint;
	tmpstr=tmpstr+s;
	tmpstr=tmpstr+m_yc.unit;
	if ((pt3.x-pt1.x-hintdis-unitdis>(YC_MINDISWIDTH*(m_yc.digitlength+hintcount+unitcount+1)
		+pView->ULtoLL(m_yc.fontjj)*(m_yc.digitlength+hintcount+unitcount-hinthz-unithz)))&&
		(pt3.y-pt1.y>YC_MINDISHEIGHT))
	{
		m_yc.lgfont.lfHeight=pt3.y-pt1.y;
		m_yc.lgfont.lfWidth=(LONG)(pt3.x-pt1.x-hintdis-unitdis
			-pView->ULtoLL(m_yc.fontjj)*(m_yc.digitlength+hintcount+unitcount-hinthz-unithz+1))/
			(m_yc.digitlength+hintcount+unitcount);

		{
			CFont m_font;
			LOGFONT tempfont;
			tempfont=m_yc.lgfont;
			m_font.CreateFontIndirect(&tempfont);
			CFont * m_oldfont=memdc.SelectObject(&m_font);
			CSize size;
			size=memdc.GetOutputTextExtent(tmpstr,m_yc.digitlength+hintcount+unitcount);//+2);
			size.cx=size.cx;//-m_yc.lgfont.lfWidth*1;//+qq;
			memdc.SelectObject(m_oldfont);

			m_yc.fontheight=pView->LLtoUL(m_yc.lgfont.lfHeight);
			m_yc.fontwidth=pView->LLtoUL(m_yc.lgfont.lfWidth);
	
		}
	}

	CYCTool::canuse=true;
	CYCTool::m_yc=this->m_yc;
	sprintf(CYCTool::m_yc.ename,"");
	CYCTool::m_yc.fontwidth=(float)(pView->ULtoLL(m_yc.fontwidth));
	CYCTool::m_yc.fontheight=(float)(pView->ULtoLL(m_yc.fontheight));
	CYCTool::m_yc.fontjj=(float)(pView->ULtoLL(m_yc.fontjj));
	CYCTool::m_yc.hintdis=(float)(pView->ULtoLL(m_yc.hintdis));

}


void CYC::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("模拟量对象..") );

    CYC_PROP_INFO dlg0;
	dlg0.SetPara(m_yc.ename,m_yc.digitlength,m_yc.hint,m_yc.unit,
	    m_yc.unitcolor,m_yc.hintcolor,m_yc.hintenable,m_yc.hintdis,m_yc.unitdis);
	sheet.AddPage( &dlg0);
    CYC_PROP_DIS dlg1;
	dlg1.SetPara(m_yc.disablecolor,m_yc.normalcolor,m_yc.leveldowncolor,
		m_yc.levelupcolor,m_yc.lgfont,m_yc.rect.x0,m_yc.rect.y0,
		m_yc.fontwidth,m_yc.fontheight,m_yc.fontjj);
	sheet.AddPage( &dlg1);
	CYC_PROP_WARN dlg2;
	dlg2.SetPara(m_yc.warnupenable,m_yc.warnupkind,m_yc.warnuplevels,
		m_yc.warnupwave,m_yc.warndownenable,m_yc.warndownkind,
		m_yc.warndownlevels,m_yc.warndownwave);
	sheet.AddPage( &dlg2);
	   
	if (sheet.DoModal() != IDOK)
		return;
	RefreshIt();	
	dlg0.GetPara(m_yc.ename,&m_yc.digitlength,m_yc.hint,m_yc.unit,
	    &m_yc.unitcolor,&m_yc.hintcolor,&m_yc.hintenable,&m_yc.hintdis,&m_yc.unitdis);
	dlg1.GetPara(&m_yc.disablecolor,&m_yc.normalcolor,&m_yc.leveldowncolor,
		&m_yc.levelupcolor,&m_yc.lgfont,&m_yc.rect.x0,&m_yc.rect.y0,
		&m_yc.fontwidth,&m_yc.fontheight,&m_yc.fontjj);
	dlg2.GetPara(&m_yc.warnupenable,&m_yc.warnupkind,&m_yc.warnuplevels,
		m_yc.warnupwave,&m_yc.warndownenable,&m_yc.warndownkind,
		&m_yc.warndownlevels,m_yc.warndownwave);
	SetModifiedFlag();
	
	m_recalrect=true;
	RefreshIt();	
	CYCTool::canuse=true;
	CYCTool::m_yc=this->m_yc;
	sprintf(CYCTool::m_yc.ename,"");
	CYCTool::m_yc.fontwidth=(float)(pView->ULtoLL(m_yc.fontwidth));
	CYCTool::m_yc.fontheight=(float)(pView->ULtoLL(m_yc.fontheight));
	CYCTool::m_yc.fontjj=(float)(pView->ULtoLL(m_yc.fontjj));
	CYCTool::m_yc.hintdis=(float)(pView->ULtoLL(m_yc.hintdis));
}

void CYC::RefreshIt()
{
	Invalidate();
}

void CYC::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_yc.rect,sizeof(m_yc.rect));
		file->Write((unsigned char *)&m_yc.fontwidth,sizeof(m_yc.fontwidth));
		file->Write((unsigned char *)&m_yc.fontheight,sizeof(m_yc.fontheight));
		file->Write((unsigned char *)&m_yc.fontjj,sizeof(m_yc.fontjj));
		file->Write((unsigned char *)&m_yc.hintdis,sizeof(m_yc.hintdis));
		file->Write((unsigned char *)&m_yc.unitdis,sizeof(m_yc.unitdis));
		file->Write((unsigned char *)m_yc.ename,sizeof(char)*17);
		file->Write((unsigned char *)&m_yc.digitlength,sizeof(m_yc.digitlength));
		file->Write((unsigned char *)m_yc.hint,sizeof(char)*33);
		file->Write((unsigned char *)m_yc.unit,sizeof(char)*17);
		file->Write((unsigned char *)&m_yc.lgfont,sizeof(m_yc.lgfont));
		file->Write((unsigned char *)&m_yc.hintcolor,sizeof(m_yc.hintcolor));
		file->Write((unsigned char *)&m_yc.unitcolor,sizeof(m_yc.unitcolor));
		file->Write((unsigned char *)&m_yc.levelupcolor,sizeof(m_yc.levelupcolor));
		file->Write((unsigned char *)&m_yc.leveldowncolor,sizeof(m_yc.leveldowncolor));
		file->Write((unsigned char *)&m_yc.normalcolor,sizeof(m_yc.normalcolor));
		file->Write((unsigned char *)&m_yc.disablecolor,sizeof(m_yc.disablecolor));
		file->Write((unsigned char *)&m_yc.warnupenable,sizeof(m_yc.warnupenable));
		file->Write((unsigned char *)&m_yc.warnuplevels,sizeof(m_yc.warnuplevels));
		file->Write((unsigned char *)&m_yc.warnupkind,sizeof(m_yc.warnupkind));
		file->Write((unsigned char *)m_yc.warnupwave,sizeof(char)*33);
		file->Write((unsigned char *)&m_yc.warndownenable,sizeof(m_yc.warndownenable));
		file->Write((unsigned char *)&m_yc.warndownlevels,sizeof(m_yc.warndownlevels));
		file->Write((unsigned char *)&m_yc.warndownkind,sizeof(m_yc.warndownkind));
		file->Write((unsigned char *)m_yc.warndownwave,sizeof(char)*33);
		file->Write((unsigned char *)&m_yc.hintenable,sizeof(m_yc.hintenable));
	} 
	else
	{
		m_recalrect=false;
		m_recalfont=false;
		file->Read((unsigned char *)&m_yc.rect,sizeof(m_yc.rect));
		file->Read((unsigned char *)&m_yc.fontwidth,sizeof(m_yc.fontwidth));
		file->Read((unsigned char *)&m_yc.fontheight,sizeof(m_yc.fontheight));
		file->Read((unsigned char *)&m_yc.fontjj,sizeof(m_yc.fontjj));
		file->Read((unsigned char *)&m_yc.hintdis,sizeof(m_yc.hintdis));
		file->Read((unsigned char *)&m_yc.unitdis,sizeof(m_yc.unitdis));
		file->Read((unsigned char *)m_yc.ename,sizeof(char)*17);
		file->Read((unsigned char *)&m_yc.digitlength,sizeof(m_yc.digitlength));
		file->Read((unsigned char *)m_yc.hint,sizeof(char)*33);
		file->Read((unsigned char *)m_yc.unit,sizeof(char)*17);
		file->Read((unsigned char *)&m_yc.lgfont,sizeof(m_yc.lgfont));
		file->Read((unsigned char *)&m_yc.hintcolor,sizeof(m_yc.hintcolor));
		file->Read((unsigned char *)&m_yc.unitcolor,sizeof(m_yc.unitcolor));
		file->Read((unsigned char *)&m_yc.levelupcolor,sizeof(m_yc.levelupcolor));
		file->Read((unsigned char *)&m_yc.leveldowncolor,sizeof(m_yc.leveldowncolor));
		file->Read((unsigned char *)&m_yc.normalcolor,sizeof(m_yc.normalcolor));
		file->Read((unsigned char *)&m_yc.disablecolor,sizeof(m_yc.disablecolor));
		file->Read((unsigned char *)&m_yc.warnupenable,sizeof(m_yc.warnupenable));
		file->Read((unsigned char *)&m_yc.warnuplevels,sizeof(m_yc.warnuplevels));
		file->Read((unsigned char *)&m_yc.warnupkind,sizeof(m_yc.warnupkind));
		file->Read((unsigned char *)m_yc.warnupwave,sizeof(char)*33);
		file->Read((unsigned char *)&m_yc.warndownenable,sizeof(m_yc.warndownenable));
		file->Read((unsigned char *)&m_yc.warndownlevels,sizeof(m_yc.warndownlevels));
		file->Read((unsigned char *)&m_yc.warndownkind,sizeof(m_yc.warndownkind));
		file->Read((unsigned char *)m_yc.warndownwave,sizeof(char)*33);
		file->Read((unsigned char *)&m_yc.hintenable,sizeof(m_yc.hintenable));
	}
}

void CYC::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if(ar.IsStoring())  
	{
//		ar<<m_yc;
		ar.Write(&m_yc,sizeof(Stu_YC));
	}
	else
	{
//		ar>>m_yc;
		m_recalrect=false;
		m_recalfont=false;
		ar.Read(&m_yc,sizeof(Stu_YC));
		m_yc.ename[0]=0x00;
	}
}

 void CYC::DrawText(CDC * pDC,char *s, int count, int jj, int width,int x0, int y0, COLORREF itcolor, int *x1)
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

/* 
void CYC::GetRectFromFont()
{
	int hinthz,unithz;
	int hintcount=GetCharLen(m_yc.hint,33,&hinthz);
	int unitcount=GetCharLen(m_yc.unit,17,&unithz);
	m_recalrect=false;
	float tempx0=m_yc.rect.x0;
	float tempy0=m_yc.rect.y0;
	if (hintcount>0) tempx0=tempx0+m_yc.hintdis;
	if (unitcount>0) tempx0=tempx0+m_yc.unitdis;
	m_yc.rect.x1=tempx0+m_yc.fontwidth*(m_yc.digitlength+hintcount+unitcount+1)
		+m_yc.fontjj*(m_yc.digitlength+hintcount+unitcount-hinthz-unithz);
	m_yc.rect.y1=m_yc.rect.y0+m_yc.fontheight*(1);
}
*/

 
 void CYC::GetRectFromFont()
 {
	 int hinthz,unithz;
	 int hintcount=GetCharLen(m_yc.hint,33,&hinthz);
	 int unitcount=GetCharLen(m_yc.unit,17,&unithz);
	 m_recalrect=false;
	 float tempx0=m_yc.rect.x0;
	 float tempy0=m_yc.rect.y0;
	 if (hintcount>0) tempx0=tempx0+m_yc.hintdis;
	 if (unitcount>0) tempx0=tempx0+m_yc.unitdis;
	 m_yc.rect.x1=tempx0+m_yc.fontwidth*(m_yc.digitlength+hintcount+unitcount)
	 +m_yc.fontjj*(m_yc.digitlength+hintcount+unitcount-hinthz-unithz-1);
	 m_yc.rect.y1=m_yc.rect.y0+m_yc.fontheight*(1);
 }


void CYC::Set_Ty_Color(COLORREF color, CElecMapView *pView)
{
	if (m_yc.normalcolor==color) return;
	m_yc.normalcolor=color;
	SetModifiedFlag();

	ASSERT(pView!=NULL);
	if (pView==NULL) return;

	pView->InvalidateTy(this);

}

void CYC::SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[])
{
	m_yc.fontwidth=width;
	m_yc.fontheight=height;
	m_yc.fontjj=jqwidth;
	m_yc.normalcolor=color;
	if (strlen(font)<32)
		strcpy(m_yc.lgfont.lfFaceName,font);
	m_recalrect=true;
	Invalidate();
}

void CYC::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=GetEname_From_ReplacRule(m_yc.ename,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_yc.ename,str);
	}
}
