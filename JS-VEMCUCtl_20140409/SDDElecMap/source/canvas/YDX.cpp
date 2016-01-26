// YDX.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/YDX.h"
//#include "Sp_drawView.h"
#include "../../ElecMapView.h"
#include "dialog/YX_PROP_WARN.h"
#include "dialog/YX_PROP_DIS.h"
#include "dialog/YDX_PROP_INFO.h"
#include "canvas/IMAGEX.h"
#include "canvas/RtValueTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYDX

/////////////////////////////////////////////////////////////////////////////
// CYDX
IMPLEMENT_SERIAL(CYDX, CTyBase, 0)

CYDX::CYDX(Stu_YDX * ydx)
{
	m_ydx=*ydx;
	memcpy(m_ydx.mainename,ydx->mainename,sizeof(char)*17);
	memcpy(m_ydx.slaveename,ydx->slaveename,sizeof(char)*17);
	memcpy(m_ydx.warn0wave,ydx->warn0wave,sizeof(char)*33);
	memcpy(m_ydx.warn1wave,ydx->warn1wave,sizeof(char)*33);
	memcpy(m_ydx.picname0,ydx->picname0,sizeof(char)*33);
	memcpy(m_ydx.picname1,ydx->picname1,sizeof(char)*33);
	memcpy(m_ydx.picnamedis,ydx->picnamedis,sizeof(char)*33);
	if (m_ydx.pickind0==2)
	{
		m_ydx.pickind1=2;
		m_ydx.pickinddis=2;
	}
	RegisterAllPic();
	m_recalrect=false;
	m_recalfont=false;
}

CYDX::CYDX(float x0,float y0,float w,float h)
{
	sprintf(m_ydx.mainename,"DEFAULTMAINENAME");
	sprintf(m_ydx.slaveename,"DEFAULTSLAVENAME");
	m_ydx.color0=0xff0000;
	m_ydx.color1=0x0000ff;
	m_ydx.colordis=0xffffff;
	m_ydx.handit=0;
	m_ydx.pickind0=2;
	m_ydx.pickind1=2;
	m_ydx.pickinddis=2;
	m_ydx.diskind0=0;
	m_ydx.diskind1=0;
	m_ydx.diskinddis=0;
	sprintf(m_ydx.picname0,"YDX量0状态   ");
	sprintf(m_ydx.picname1,"YDX量1状态   ");
	sprintf(m_ydx.picnamedis,"YDX量无效状态");

	m_ydx.lgfont.lfHeight=15;
	m_ydx.lgfont.lfWidth=5;
	m_ydx.lgfont.lfEscapement=0;//旋转角度
	m_ydx.lgfont.lfOrientation=0;
	m_ydx.lgfont.lfWeight=FW_MEDIUM;
	m_ydx.lgfont.lfItalic=false;
//	m_ydx.lgfont.lfWeight=700;
//	m_ydx.lgfont.lfItalic=true;
	m_ydx.lgfont.lfUnderline=false;
	m_ydx.lgfont.lfStrikeOut=false;//强调线
	m_ydx.lgfont.lfCharSet=GB2312_CHARSET;
	m_ydx.lgfont.lfOutPrecision=OUT_DEFAULT_PRECIS;//输出精度
	m_ydx.lgfont.lfClipPrecision=CLIP_DEFAULT_PRECIS;//剪裁精度
	m_ydx.lgfont.lfQuality=DEFAULT_QUALITY;
	m_ydx.lgfont.lfPitchAndFamily=DEFAULT_PITCH;//间距和字体族
	sprintf(m_ydx.lgfont.lfFaceName,"宋体");

	m_ydx.warn1enable=0;
	m_ydx.warn1levels=0;
	m_ydx.warn1kind=0;
	sprintf(m_ydx.warn1wave,"");
	m_ydx.warn0enable=0;
	m_ydx.warn0levels=0;
	m_ydx.warn0kind=0;
	sprintf(m_ydx.warn0wave,"");

	m_ydx.rect.x0=x0;
	m_ydx.rect.y0=y0;
	m_ydx.rect.x1=x0+2;
	m_ydx.rect.y1=y0+2;

	m_ydx.fontwidth=w;
	m_ydx.fontheight=h;
	m_ydx.fontjj=0;
	m_recalrect=true;
	m_recalfont=false;
}

CYDX::~CYDX()
{
	UnRegisterAllPic();
}


/////////////////////////////////////////////////////////////////////////////
// CYDX message handlers

int CYDX::GetHandleCount()
{
	return 8;
}

PointStruct CYDX::GetHandle(int nHandle)
{
	ASSERT_VALID(this);
	nHandle--;
	PointStruct pt;
	pt.x=0;pt.y=0;
	if (nHandle==0)
	{
		pt.x=m_ydx.rect.x0;
		pt.y=m_ydx.rect.y0;
	}
	if (nHandle==4)
	{
		pt.x=m_ydx.rect.x0;
		pt.y=m_ydx.rect.y0+(m_ydx.rect.y1-m_ydx.rect.y0)/2;
	}
	if (nHandle==2)
	{
		pt.x=m_ydx.rect.x0;
		pt.y=m_ydx.rect.y1;
	}
	if (nHandle==3)
	{
		pt.x=m_ydx.rect.x0+(m_ydx.rect.x1-m_ydx.rect.x0)/2;
		pt.y=m_ydx.rect.y1;
	}
	if (nHandle==1)
	{
		pt.x=m_ydx.rect.x1;
		pt.y=m_ydx.rect.y1;
	}
	if (nHandle==5)
	{
		pt.x=m_ydx.rect.x1;
		pt.y=m_ydx.rect.y0+(m_ydx.rect.y1-m_ydx.rect.y0)/2;
	}
	if (nHandle==6)
	{
		pt.x=m_ydx.rect.x1;
		pt.y=m_ydx.rect.y0;
	}
	if (nHandle==7)
	{
		pt.x=m_ydx.rect.x0+(m_ydx.rect.x1-m_ydx.rect.x0)/2;
		pt.y=m_ydx.rect.y0;
	}
	return pt;

}

HCURSOR CYDX::GetHandleCursor(int nHandle)
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

BOOL CYDX::IntersectRect(float x0, float y0, float x1, float y1)
{

	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

//DEL int CYDX::GetCharLen(char *schar)
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

DRAW_TY CYDX::GetTyType()
{
  return tyYDX;
}

int CYDX::GetCharLen(char *schar,int maxlen,int * hzcount)
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
void CYDX::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	if (m_ydx.pickind0==2)
	{
		if (m_recalrect)
		{
			GetRectFromFont();
		}
	}
	m_recalrect=false;
	*minX=m_ydx.rect.x0;
	*minY=m_ydx.rect.y0;
	*maxX=m_ydx.rect.x1;
	*maxY=m_ydx.rect.y1;
}

void CYDX::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView!=NULL) pView->InvalidateTy(this);
	if (m_ydx.rect.x0+xLen>=0) 
	{
		m_ydx.rect.x0+=xLen;
		m_ydx.rect.x1+=xLen;
	}
	if (m_ydx.rect.y0+yLen>=0)
	{
		m_ydx.rect.y0+=yLen;
		m_ydx.rect.y1+=yLen;
	}
	if (pView!=NULL) pView->InvalidateTy(this);
}

BOOL CYDX::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange>=0);
	if (m_bDelete) {return false;}
	if ((x>=max(0,m_ydx.rect.x0-errRange))&&
		(y>=max(0,m_ydx.rect.y0-errRange))&&
		(x<=m_ydx.rect.x1+errRange)&&
		(y<=m_ydx.rect.y1+errRange))
	{
		return true;
	}

	return false;
	
}

CTyBase * CYDX::Clone()
{
	ASSERT_VALID(this);
	Stu_YDX  tempydx;
	tempydx=m_ydx;
	memcpy(tempydx.mainename,m_ydx.mainename,17*sizeof(char));
	memcpy(tempydx.slaveename,m_ydx.slaveename,17*sizeof(char));
	memcpy(tempydx.picname0,m_ydx.picname0,33*sizeof(char));
	memcpy(tempydx.picname1,m_ydx.picname1,33*sizeof(char));
	memcpy(tempydx.picnamedis,m_ydx.picnamedis,33*sizeof(char));
	memcpy(tempydx.warn1wave,m_ydx.warn1wave,33*sizeof(char));
	memcpy(tempydx.warn0wave,m_ydx.warn0wave,33*sizeof(char));

	CYDX * pClone = new CYDX(&tempydx);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

void CYDX::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	nHandle--;
	ASSERT(pView!=NULL);
	pView->InvalidateTy(this);
	RectStruct temprect,temprect2;
	BOOL x0move,y0move,x1move,y1move;
	x0move=false;x1move=false;
	y0move=false;y1move=false;
	temprect=m_ydx.rect;
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
	m_ydx.rect=temprect;
//	GetFontExtractWidth(pView);
//	pView->InvalidateTy(this);
	if (m_ydx.pickind0==2){m_recalfont=true;}else{m_recalfont=false;}
	m_recalrect=false;
	RefreshIt();
	if ((m_ydx.pickind0==2)||(m_ydx.pickind1==2)||(m_ydx.pickinddis==2))
	{
	}
	else
	{
		CYDXTool::canuse=true;
		CYDXTool::m_ydx=this->m_ydx;
		sprintf(CYDXTool::m_ydx.mainename,"");
		sprintf(CYDXTool::m_ydx.slaveename,"");
		CYDXTool::m_ydx.rect.x0=0;
		CYDXTool::m_ydx.rect.y0=0;
		CYDXTool::m_ydx.rect.x1=(float)(pView->ULtoLL(m_ydx.rect.x1-m_ydx.rect.x0));
		CYDXTool::m_ydx.rect.y1=(float)(pView->ULtoLL(m_ydx.rect.y1-m_ydx.rect.y0));
	}
}

void CYDX::DrawDC(CDC *pDC, CElecMapView *pView)
{
	if ((m_ydx.pickind0==2)&&(m_recalrect))
	{
		GetRectFromFont();
	}
	m_recalrect=false;

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy)) return;
	
	GetRect(&minx,&miny,&maxx,&maxy);
	CPoint pt1,pt2,pt3,pt4;
	pt1=pView->UPtoLP(m_ydx.rect.x0,m_ydx.rect.y0);
	pt2=pView->UPtoLP(m_ydx.rect.x0,m_ydx.rect.y1);
	pt3=pView->UPtoLP(m_ydx.rect.x1,m_ydx.rect.y1);
	pt4=pView->UPtoLP(m_ydx.rect.x1,m_ydx.rect.y0);

	if ((pt3.x-pt1.x<=2)&&(pt3.y-pt1.y<=2)) return;

	int hzcount;
	int count=GetCharLen(m_ydx.picname0,33,&hzcount);
	if ((m_ydx.pickind0==2)&&(m_recalfont))
	{
		GetFontExtractWidth(pDC,pView);
	}
	m_recalfont=false;

	//if ((pt3.x-pt1.x>1024)||(pt3.y-pt1.y>768)) return;
	if (m_ydx.pickind0==0)
	{
		DrawBmp(pDC,pView,pt1,pt3);
	}
	if (m_ydx.pickind0==1)
	{
		DrawVec(pDC,pView,pt1,pt3);
	}
	if (m_ydx.pickind0==2)
	{
		if ((pt3.x-pt1.x>(YDX_MINDISWIDTH*(count+1)
			+pView->ULtoLL(m_ydx.fontjj)*(count-hzcount)))&&
			(pt3.y-pt1.y>YDX_MINDISHEIGHT))
		{
			m_ydx.lgfont.lfHeight=pView->ULtoLL(m_ydx.fontheight);
			m_ydx.lgfont.lfWidth=pView->ULtoLL(m_ydx.fontwidth);
			LOGFONT tempfont=m_ydx.lgfont;
			CFont m_font;
			m_font.CreateFontIndirect(&tempfont);
			CFont * m_oldfont=pDC->SelectObject(&m_font);
			int oldmode=pDC->GetBkMode();
			pDC->SetBkMode(TRANSPARENT);
			int x1=pt1.x;int backx;
			int ljj=pView->ULtoLL(m_ydx.fontjj);
			int lwidth=pView->ULtoLL(m_ydx.fontwidth);
			DrawItText(pDC,m_ydx.picname0,count,ljj,lwidth,x1,pt1.y,m_ydx.color0,&backx);
			pDC->SetBkMode(oldmode);
			pDC->SelectObject(m_oldfont);
		}
	}

	CPen pen(PS_DOT ,1,COLORREF(0x0000ff));
    CPen * pOldPen=pDC->SelectObject(&pen);
	int holdDrawMode=pDC->SetROP2(R2_NOTXORPEN);

	/*pDC->MoveTo(pt1); 
	pDC->LineTo(pt2);
	pDC->LineTo(pt3);
	pDC->LineTo(pt4);
	pDC->LineTo(pt1);*/

	pDC->SetROP2(holdDrawMode);
	pDC->SelectObject(pOldPen);
}

void CYDX::GetFontExtractWidth(CDC * pDC,CElecMapView *pView)
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
	pt1=pView->UPtoLP(m_ydx.rect.x0,m_ydx.rect.y0);
	pt2=pView->UPtoLP(m_ydx.rect.x0,m_ydx.rect.y1);
	pt3=pView->UPtoLP(m_ydx.rect.x1,m_ydx.rect.y1);
	pt4=pView->UPtoLP(m_ydx.rect.x1,m_ydx.rect.y0);
	int count,hzcount;
	count=GetCharLen(m_ydx.picname0,33,&hzcount);
	if ((pt3.x-pt1.x>(YDX_MINDISWIDTH*(count+1)
		+pView->ULtoLL(m_ydx.fontjj)*(count-hzcount)))&&
		(pt3.y-pt1.y>YDX_MINDISHEIGHT))
	{
		m_ydx.lgfont.lfHeight=pt3.y-pt1.y;
		m_ydx.lgfont.lfWidth=(LONG)((pt3.x-pt1.x
			-pView->ULtoLL(m_ydx.fontjj)*(count-hzcount))/
			(count));
		while (true) 
		{
			CFont m_font;
			LOGFONT tempfont;
			tempfont=m_ydx.lgfont;
			m_font.CreateFontIndirect(&tempfont);
			CFont * m_oldfont=memdc.SelectObject(&m_font);
			CSize size;
			size=memdc.GetOutputTextExtent(m_ydx.picname0,count);//+2);
			size.cx=size.cx;//-m_ydx.lgfont.lfWidth*1;//+qq;
			memdc.SelectObject(m_oldfont);
			//if ((minlen<=(pt3.x-pt1.x)*FontSizePercent)||(m_ydx.lgfont.lfWidth<=0))
			if (((size.cx*(count+1)/(count))<=
				(pt3.x-pt1.x
				-pView->ULtoLL(m_ydx.fontjj)*(count-hzcount)))||(m_ydx.lgfont.lfWidth<=0))
			{
				m_ydx.fontheight=pView->LLtoUL(m_ydx.lgfont.lfHeight);
				m_ydx.fontwidth=pView->LLtoUL(m_ydx.lgfont.lfWidth);
				break;
			}
			else
			{
				m_ydx.lgfont.lfWidth--;
			}
		}
	}

	if ((m_ydx.pickind0==2)||(m_ydx.pickind1==2)||(m_ydx.pickinddis==2))
	{
		CYDXTool::canuse=true;
		CYDXTool::m_ydx=this->m_ydx;
		sprintf(CYDXTool::m_ydx.mainename,"");
		sprintf(CYDXTool::m_ydx.slaveename,"");
		CYDXTool::m_ydx.fontwidth=(float)(pView->ULtoLL(m_ydx.fontwidth));
		CYDXTool::m_ydx.fontheight=(float)(pView->ULtoLL(m_ydx.fontheight));
		CYDXTool::m_ydx.fontjj=(float)(pView->ULtoLL(m_ydx.fontjj));
	}

}

void CYDX::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("双遥信对象..") );

	CYDX_PROP_INFO dlg0;
	dlg0.SetPara(m_ydx.mainename,m_ydx.slaveename,m_ydx.handit);
	sheet.AddPage( &dlg0);

	CYX_PROP_DIS dlg1;
	float inw,inh;
	if (m_ydx.pickind0==2)
	{
		inw=m_ydx.fontwidth;
		inh=m_ydx.fontheight;
	}
	else
	{
		inw=m_ydx.rect.x1-m_ydx.rect.x0;
		inh=m_ydx.rect.y1-m_ydx.rect.y0;
	}	
	dlg1.SetPara(m_ydx.lgfont,m_ydx.color0,m_ydx.color1,m_ydx.colordis,
		m_ydx.picname0,m_ydx.picname1,m_ydx.picnamedis,
		m_ydx.pickind0,m_ydx.pickind1,m_ydx.pickinddis,
		m_ydx.diskind0,m_ydx.diskind1,m_ydx.diskinddis,
		m_ydx.rect.x0,m_ydx.rect.y0,
		inw,inh,m_ydx.fontjj);
	sheet.AddPage( &dlg1);

	CYX_PROP_WARN dlg2;
	dlg2.SetPara(m_ydx.warn1enable,m_ydx.warn1kind,m_ydx.warn1levels,
		m_ydx.warn1wave,m_ydx.warn0enable,m_ydx.warn0kind,
		m_ydx.warn0levels,m_ydx.warn0wave);
	sheet.AddPage( &dlg2);
	   
	if (sheet.DoModal() != IDOK)
		return;
	RefreshIt();
	UnRegisterAllPic();
	dlg0.GetPara(m_ydx.mainename,m_ydx.slaveename,&m_ydx.handit);
	dlg1.GetPara(&m_ydx.lgfont,&m_ydx.color0,&m_ydx.color1,&m_ydx.colordis,
		m_ydx.picname0,m_ydx.picname1,m_ydx.picnamedis,
		&m_ydx.pickind0,&m_ydx.pickind1,&m_ydx.pickinddis,
		&m_ydx.diskind0,&m_ydx.diskind1,&m_ydx.diskinddis,
		&m_ydx.rect.x0,&m_ydx.rect.y0,
		&inw,&inh,&m_ydx.fontjj);
  
	if ((m_ydx.pickind0==2)&&(strlen(m_ydx.picname0)==0))
		strcpy(m_ydx.picname0," ");
	if ((m_ydx.pickind1==2)&&(strlen(m_ydx.picname1)==0))
		strcpy(m_ydx.picname1," ");
	if ((m_ydx.pickinddis==2)&&(strlen(m_ydx.picnamedis)==0))
		strcpy(m_ydx.picnamedis," ");

	dlg2.GetPara(&m_ydx.warn1enable,&m_ydx.warn1kind,&m_ydx.warn1levels,
		m_ydx.warn1wave,&m_ydx.warn0enable,&m_ydx.warn0kind,
		&m_ydx.warn0levels,m_ydx.warn0wave);
	RegisterAllPic();
	SetModifiedFlag();

	if ((m_ydx.pickind0==2)||(m_ydx.pickind1==2)||(m_ydx.pickinddis==2))
	{
		m_ydx.pickind0=2;
		m_ydx.pickind1=2;
		m_ydx.pickinddis=2;
		m_recalrect=true;
		m_ydx.fontwidth=inw;
		m_ydx.fontheight=inh;
		CYDXTool::canuse=true;
		CYDXTool::m_ydx=this->m_ydx;
		sprintf(CYDXTool::m_ydx.mainename,"");
		sprintf(CYDXTool::m_ydx.slaveename,"");
		CYDXTool::m_ydx.fontwidth=(float)(pView->ULtoLL(m_ydx.fontwidth));
		CYDXTool::m_ydx.fontheight=(float)(pView->ULtoLL(m_ydx.fontheight));
		CYDXTool::m_ydx.fontjj=(float)(pView->ULtoLL(m_ydx.fontjj));
	}
	else
	{
		m_ydx.rect.x1=m_ydx.rect.x0+inw;
		m_ydx.rect.y1=m_ydx.rect.y0+inh;
		CYDXTool::canuse=true;
		CYDXTool::m_ydx=this->m_ydx;
		sprintf(CYDXTool::m_ydx.mainename,"");
		sprintf(CYDXTool::m_ydx.slaveename,"");
		CYDXTool::m_ydx.rect.x0=0;
		CYDXTool::m_ydx.rect.y0=0;
		CYDXTool::m_ydx.rect.x1=(float)(pView->ULtoLL(m_ydx.rect.x1-m_ydx.rect.x0));
		CYDXTool::m_ydx.rect.y1=(float)(pView->ULtoLL(m_ydx.rect.y1-m_ydx.rect.y0));
	}
	
	RefreshIt();	
}

void CYDX::RefreshIt()
{
	Invalidate();

}

/////////////////////////////////////////////////////////////////////////////
// CYDX message handlers
//void Ty_Serialize(CArchive &ar);
//void Ty_Save(CFile *file, BOOL Yn);

void CYDX::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);
	if(Yn)	//如果是在进行保存
	{
		m_recalrect=false;
		m_recalfont=false;
		file->Write((unsigned char *)&m_ydx.rect,sizeof(m_ydx.rect));
		file->Write((unsigned char *)&m_ydx.fontwidth,sizeof(m_ydx.fontwidth));
		file->Write((unsigned char *)&m_ydx.fontheight,sizeof(m_ydx.fontheight));
		file->Write((unsigned char *)&m_ydx.fontjj,sizeof(m_ydx.fontjj));
		file->Write((unsigned char *)m_ydx.mainename,sizeof(char)*17);
		file->Write((unsigned char *)m_ydx.slaveename,sizeof(char)*17);
		file->Write((unsigned char *)&m_ydx.lgfont,sizeof(m_ydx.lgfont));
		file->Write((unsigned char *)&m_ydx.color0,sizeof(m_ydx.color0));
		file->Write((unsigned char *)&m_ydx.color1,sizeof(m_ydx.color1));
		file->Write((unsigned char *)&m_ydx.colordis,sizeof(m_ydx.colordis));
		file->Write((unsigned char *)m_ydx.picname0,sizeof(char)*33);
		file->Write((unsigned char *)m_ydx.picname1,sizeof(char)*33);
		file->Write((unsigned char *)m_ydx.picnamedis,sizeof(char)*33);
		file->Write((unsigned char *)&m_ydx.pickind0,sizeof(m_ydx.pickind0));
		file->Write((unsigned char *)&m_ydx.pickind1,sizeof(m_ydx.pickind1));
		file->Write((unsigned char *)&m_ydx.pickinddis,sizeof(m_ydx.pickinddis));
		file->Write((unsigned char *)&m_ydx.diskind0,sizeof(m_ydx.diskind0));
		file->Write((unsigned char *)&m_ydx.diskind1,sizeof(m_ydx.diskind1));
		file->Write((unsigned char *)&m_ydx.diskinddis,sizeof(m_ydx.diskinddis));

		file->Write((unsigned char *)&m_ydx.warn0enable,sizeof(m_ydx.warn0enable));
		file->Write((unsigned char *)&m_ydx.warn0levels,sizeof(m_ydx.warn0levels));
		file->Write((unsigned char *)&m_ydx.warn0kind,sizeof(m_ydx.warn0kind));
		file->Write((unsigned char *)m_ydx.warn0wave,sizeof(char)*33);
		file->Write((unsigned char *)&m_ydx.warn1enable,sizeof(m_ydx.warn1enable));
		file->Write((unsigned char *)&m_ydx.warn1levels,sizeof(m_ydx.warn1levels));
		file->Write((unsigned char *)&m_ydx.warn1kind,sizeof(m_ydx.warn1kind));
		file->Write((unsigned char *)m_ydx.warn1wave,sizeof(char)*33);
		file->Write((unsigned char *)&m_ydx.handit,sizeof(m_ydx.handit));

	} 
	else
	{
		m_recalrect=false;
		m_recalfont=false;
		UnRegisterAllPic();	
		file->Read((unsigned char *)&m_ydx.rect,sizeof(m_ydx.rect));
		file->Read((unsigned char *)&m_ydx.fontwidth,sizeof(m_ydx.fontwidth));
		file->Read((unsigned char *)&m_ydx.fontheight,sizeof(m_ydx.fontheight));
		file->Read((unsigned char *)&m_ydx.fontjj,sizeof(m_ydx.fontjj));
		file->Read((unsigned char *)m_ydx.mainename,sizeof(char)*17);
		file->Read((unsigned char *)m_ydx.slaveename,sizeof(char)*17);
		file->Read((unsigned char *)&m_ydx.lgfont,sizeof(m_ydx.lgfont));
		file->Read((unsigned char *)&m_ydx.color0,sizeof(m_ydx.color0));
		file->Read((unsigned char *)&m_ydx.color1,sizeof(m_ydx.color1));
		file->Read((unsigned char *)&m_ydx.colordis,sizeof(m_ydx.colordis));
		file->Read((unsigned char *)m_ydx.picname0,sizeof(char)*33);
		file->Read((unsigned char *)m_ydx.picname1,sizeof(char)*33);
		file->Read((unsigned char *)m_ydx.picnamedis,sizeof(char)*33);
		file->Read((unsigned char *)&m_ydx.pickind0,sizeof(m_ydx.pickind0));
		file->Read((unsigned char *)&m_ydx.pickind1,sizeof(m_ydx.pickind1));
		file->Read((unsigned char *)&m_ydx.pickinddis,sizeof(m_ydx.pickinddis));
		file->Read((unsigned char *)&m_ydx.diskind0,sizeof(m_ydx.diskind0));
		file->Read((unsigned char *)&m_ydx.diskind1,sizeof(m_ydx.diskind1));
		file->Read((unsigned char *)&m_ydx.diskinddis,sizeof(m_ydx.diskinddis));

		file->Read((unsigned char *)&m_ydx.warn0enable,sizeof(m_ydx.warn0enable));
		file->Read((unsigned char *)&m_ydx.warn0levels,sizeof(m_ydx.warn0levels));
		file->Read((unsigned char *)&m_ydx.warn0kind,sizeof(m_ydx.warn0kind));
		file->Read((unsigned char *)m_ydx.warn0wave,sizeof(char)*33);
		file->Read((unsigned char *)&m_ydx.warn1enable,sizeof(m_ydx.warn1enable));
		file->Read((unsigned char *)&m_ydx.warn1levels,sizeof(m_ydx.warn1levels));
		file->Read((unsigned char *)&m_ydx.warn1kind,sizeof(m_ydx.warn1kind));
		file->Read((unsigned char *)m_ydx.warn1wave,sizeof(char)*33);
		file->Read((unsigned char *)&m_ydx.handit,sizeof(m_ydx.handit));

		if (m_ydx.pickind0==2)
		{
			m_ydx.pickind1=2;
			m_ydx.pickinddis=2;
		}
		RegisterAllPic();
	}
}

void CYDX::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=GetEname_From_ReplacRule(m_ydx.mainename,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag();
		strcpy(m_ydx.mainename,str);
	}

	ret=GetEname_From_ReplacRule(m_ydx.slaveename,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag();
		strcpy(m_ydx.slaveename,str);
	}

}

void CYDX::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if(ar.IsStoring())  
	{
//		ar<<m_ydx;
		ar.Write(&m_ydx,sizeof(Stu_YDX));
	}
	else
	{
//		ar>>m_ydx;
		UnRegisterAllPic();
		ar.Read(&m_ydx,sizeof(Stu_YDX));
		m_ydx.mainename[0]=0x00;
		m_ydx.slaveename[0]=0x00;

		if (m_ydx.pickind0==2)
		{
			m_ydx.pickind1=2;
			m_ydx.pickinddis=2;
		}
		RegisterAllPic();
	}
}

//DEL void CYDX::DrawText(CDC *pDC, CElecMapView *pView,CPoint pt1,CPoint pt3)
//DEL {
//DEL 	int digitlength=GetCharLen(m_ydx.picname0,33);
//DEL 	if (digitlength==0)return;
//DEL 	if ((pt3.x-pt1.x>YDX_MINDISWIDTH*(digitlength))&&
//DEL 		(pt3.y-pt1.y>YDX_MINDISHEIGHT))
//DEL 	{
//DEL 		if (m_fontwidth==0) 
//DEL 		{
//DEL 			GetFontExtractWidth(pView);
//DEL 		}
//DEL 		m_ydx.lgfont.lfHeight=pt3.y-pt1.y;
//DEL 		m_ydx.lgfont.lfWidth=m_fontwidth;
//DEL 		LOGFONT tempfont=m_ydx.lgfont;
//DEL 		CFont m_font;
//DEL 		m_font.CreateFontIndirect(&tempfont);
//DEL 		CFont * m_oldfont=pDC->SelectObject(&m_font);
//DEL 		int oldmode=pDC->GetBkMode();
//DEL 		pDC->SetBkMode(TRANSPARENT);
//DEL 		pDC->SetTextColor(m_ydx.color0);
//DEL 		pDC->TextOut(pt1.x,pt1.y,m_ydx.picname0);
//DEL 		pDC->SetBkMode(oldmode);
//DEL 		pDC->SelectObject(m_oldfont);
//DEL 	}
//DEL 
//DEL }

void CYDX::DrawBmp(CDC *pDC, CElecMapView *pView,CPoint pt1,CPoint pt3)
{
	if ((pt3.x-pt1.x<1)||(pt3.y-pt1.y<1)) return;
	BOOL trans=false;
	if ((m_ydx.diskind0&1)!=0) trans=true;
 	DrawImage(m_ydx.picname0,pDC,pt1.x+1,pt1.y+1,pt3.x-pt1.x-2,pt3.y-pt1.y-2,trans,m_ydx.color0);
}

void CYDX::DrawVec(CDC *pDC, CElecMapView *pView,CPoint pt1,CPoint pt3)
{
	if ((pt3.x-pt1.x<1)||(pt3.y-pt1.y<1)) return;
	BOOL trans=false;
	if ((m_ydx.diskind0&1)!=0) trans=true;
	CRect rt;
	rt.SetRect(pt1.x,pt1.y,pt3.x,pt3.y);
	DrawVector(m_ydx.picname0,pDC,rt,rt,m_ydx.color0,trans);
}

void CYDX::RegisterAllPic()
{
	if ((m_ydx.pickind0)==0)
		RegisterImage(m_ydx.picname0);
	else if ((m_ydx.pickind0)==1)
		RegisterVector(m_ydx.picname0);

	if ((m_ydx.pickind1)==0)
		RegisterImage(m_ydx.picname1);
	else if ((m_ydx.pickind1)==1)
		RegisterVector(m_ydx.picname1);


	if ((m_ydx.pickinddis)==0)
		RegisterImage(m_ydx.picnamedis);
	else if ((m_ydx.pickinddis)==1)
		RegisterVector(m_ydx.picnamedis);

}

void CYDX::UnRegisterAllPic()
{
	if ((m_ydx.pickind0)==0)
		UnRegisterImage(m_ydx.picname0);
	else if ((m_ydx.pickind0)==1)
		UnRegisterVector(m_ydx.picname0);

	if ((m_ydx.pickind1)==0)
		UnRegisterImage(m_ydx.picname1);
	else if ((m_ydx.pickind1)==1)
		UnRegisterVector(m_ydx.picname1);


	if ((m_ydx.pickinddis)==0)
		UnRegisterImage(m_ydx.picnamedis);
	else if ((m_ydx.pickinddis)==1)
		UnRegisterVector(m_ydx.picnamedis);

}

void CYDX::DrawItText(CDC *pDC, char *s, int count, int jj, int width, int x0, int y0, COLORREF itcolor, int *x1)
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
void CYDX::GetRectFromFont()
{
	int hzcount;
	int count=GetCharLen(m_ydx.picname0,33,&hzcount);
	m_recalrect=false;
	float tempx0=m_ydx.rect.x0;
	float tempy0=m_ydx.rect.y0;
	m_ydx.rect.x1=tempx0+m_ydx.fontwidth*(count+1)+m_ydx.fontjj*(count-hzcount);
	m_ydx.rect.y1=m_ydx.rect.y0+m_ydx.fontheight*(1);
}

BOOL CYDX::Is_Net_Color_Yx()
{
	return TRUE;
}

void CYDX::SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[])
{
	if ((m_ydx.pickind0!=2)&&(m_ydx.pickind1!=2)
		   &&(m_ydx.pickinddis!=2)) 
	   return;

	m_ydx.fontwidth=width;
	m_ydx.fontheight=height;
	m_ydx.fontjj=jqwidth;

	if (m_ydx.pickind0==2)
		m_ydx.color0=color;

	if (strlen(font)<32)
		strcpy(m_ydx.lgfont.lfFaceName,font);

	m_recalrect=true;

	Invalidate();
}

