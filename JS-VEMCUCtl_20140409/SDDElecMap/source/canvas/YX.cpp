// YX.cpp : implementation file
//

#include "stdafx.h"
#include "canvas/YX.h"
#include "../../ElecMapView.h"
#include "dialog/YX_PROP_WARN.h"
#include "dialog/YX_PROP_DIS.h"
#include "dialog/YX_PROP_INFO.h"
#include "canvas/IMAGEX.h"
#include "canvas/RtValueTool.h"
#include "canvas/TyTool.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYX

/////////////////////////////////////////////////////////////////////////////
// CYX
IMPLEMENT_SERIAL(CYX, CTyBase, 0)

CYX::CYX(Stu_YX * yx)
{
	m_yx=*yx;
	memcpy(m_yx.ename,yx->ename,sizeof(char)*17);
	memcpy(m_yx.warn0wave,yx->warn0wave,sizeof(char)*33);
	memcpy(m_yx.warn1wave,yx->warn1wave,sizeof(char)*33);
	memcpy(m_yx.picname0,yx->picname0,sizeof(char)*33);
	memcpy(m_yx.picname1,yx->picname1,sizeof(char)*33);
	memcpy(m_yx.picnamedis,yx->picnamedis,sizeof(char)*33);
	if (m_yx.pickind0==2)
	{
		m_yx.pickind1=2;
		m_yx.pickinddis=2;
	}
	RegisterAllPic();
	m_recalrect=false;
	m_recalfont=false;

	//运行状态
	m_nStatus =0;
	m_bShow = TRUE;
}

CYX::CYX(float x0,float y0,float w,float h)
{
	sprintf(m_yx.ename,"DEFAULTENAME");
	m_yx.color0=0xff0000;
	m_yx.color1=0x0000ff;
	m_yx.colordis=0xffffff;
	m_yx.handit=0;
	m_yx.pickind0=2;
	m_yx.pickind1=2;
	m_yx.pickinddis=2;
	m_yx.diskind0=0;
	m_yx.diskind1=0;
	m_yx.diskinddis=0;
	sprintf(m_yx.picname0,"YX量0状态   ");
	sprintf(m_yx.picname1,"YX量1状态   ");
	sprintf(m_yx.picnamedis,"YX量无效状态");

	m_yx.lgfont.lfHeight=15;
	m_yx.lgfont.lfWidth=5;
	m_yx.lgfont.lfEscapement=0;//旋转角度
	m_yx.lgfont.lfOrientation=0;
	m_yx.lgfont.lfWeight=FW_MEDIUM;
	m_yx.lgfont.lfItalic=false;
//	m_yx.lgfont.lfWeight=700;
//	m_yx.lgfont.lfItalic=true;
	m_yx.lgfont.lfUnderline=false;
	m_yx.lgfont.lfStrikeOut=false;//强调线
	m_yx.lgfont.lfCharSet=GB2312_CHARSET;
	m_yx.lgfont.lfOutPrecision=OUT_DEFAULT_PRECIS;//输出精度
	m_yx.lgfont.lfClipPrecision=CLIP_DEFAULT_PRECIS;//剪裁精度
	m_yx.lgfont.lfQuality=DEFAULT_QUALITY;
	m_yx.lgfont.lfPitchAndFamily=DEFAULT_PITCH;//间距和字体族
	sprintf(m_yx.lgfont.lfFaceName,"宋体");

	m_yx.warn1enable=0;
	m_yx.warn1levels=0;
	m_yx.warn1kind=0;
	sprintf(m_yx.warn1wave,"");
	m_yx.warn0enable=0;
	m_yx.warn0levels=0;
	m_yx.warn0kind=0;
	sprintf(m_yx.warn0wave,"");

	m_yx.rect.x0=x0;
	m_yx.rect.y0=y0;
	m_yx.rect.x1=x0+2;
	m_yx.rect.y1=y0+2;

	m_yx.fontwidth=w;
	m_yx.fontheight=h;
	m_yx.fontjj=0;
	m_recalrect=true;
	m_recalfont=false;
}

CYX::~CYX()
{
	UnRegisterAllPic();
}

/////////////////////////////////////////////////////////////////////////////
// CYX message handlers

int CYX::GetHandleCount()
{
	return 8;
}

PointStruct CYX::GetHandle(int nHandle)
{
	ASSERT_VALID(this);
	nHandle--;
	PointStruct pt;
	pt.x=0;pt.y=0;
	if (nHandle==0)
	{
		pt.x=m_yx.rect.x0;
		pt.y=m_yx.rect.y0;
	}
	if (nHandle==4)
	{
		pt.x=m_yx.rect.x0;
		pt.y=m_yx.rect.y0+(m_yx.rect.y1-m_yx.rect.y0)/2;
	}
	if (nHandle==2)
	{
		pt.x=m_yx.rect.x0;
		pt.y=m_yx.rect.y1;
	}
	if (nHandle==3)
	{
		pt.x=m_yx.rect.x0+(m_yx.rect.x1-m_yx.rect.x0)/2;
		pt.y=m_yx.rect.y1;
	}
	if (nHandle==1)
	{
		pt.x=m_yx.rect.x1;
		pt.y=m_yx.rect.y1;
	}
	if (nHandle==5)
	{
		pt.x=m_yx.rect.x1;
		pt.y=m_yx.rect.y0+(m_yx.rect.y1-m_yx.rect.y0)/2;
	}
	if (nHandle==6)
	{
		pt.x=m_yx.rect.x1;
		pt.y=m_yx.rect.y0;
	}
	if (nHandle==7)
	{
		pt.x=m_yx.rect.x0+(m_yx.rect.x1-m_yx.rect.x0)/2;
		pt.y=m_yx.rect.y0;
	}
	return pt;

}

HCURSOR CYX::GetHandleCursor(int nHandle)
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

BOOL CYX::IntersectRect(float x0, float y0, float x1, float y1)
{

	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

//DEL int CYX::GetCharLen(char *schar)
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

DRAW_TY CYX::GetTyType()
{
  return tyYX;
}

int CYX::GetCharLen(char *schar,int maxlen,int * hzcount)
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
void CYX::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	if (m_yx.pickind0==2)
	{
		if (m_recalrect)
		{
			GetRectFromFont();
		}
	}
	m_recalrect=false;
	*minX=m_yx.rect.x0;
	*minY=m_yx.rect.y0;
	*maxX=m_yx.rect.x1;
	*maxY=m_yx.rect.y1;
}

void CYX::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView!=NULL) pView->InvalidateTy(this);
	if (m_yx.rect.x0+xLen>=0) 
	{
		m_yx.rect.x0+=xLen;
		m_yx.rect.x1+=xLen;
	}
	if (m_yx.rect.y0+yLen>=0)
	{
		m_yx.rect.y0+=yLen;
		m_yx.rect.y1+=yLen;
	}
	if (pView!=NULL) pView->InvalidateTy(this);
}

BOOL CYX::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange>=0);
	if (m_bDelete) {return false;}
	if ((x>=max(0,m_yx.rect.x0-errRange))&&
		(y>=max(0,m_yx.rect.y0-errRange))&&
		(x<=m_yx.rect.x1+errRange)&&
		(y<=m_yx.rect.y1+errRange))
	{
		return true;
	}

	return false;
}

CTyBase * CYX::Clone()
{
	ASSERT_VALID(this);
	Stu_YX  tempyx;
	tempyx=m_yx;
	memcpy(tempyx.ename,m_yx.ename,17*sizeof(char));
	memcpy(tempyx.picname0,m_yx.picname0,33*sizeof(char));
	memcpy(tempyx.picname1,m_yx.picname1,33*sizeof(char));
	memcpy(tempyx.picnamedis,m_yx.picnamedis,33*sizeof(char));
	memcpy(tempyx.warn1wave,m_yx.warn1wave,33*sizeof(char));
	memcpy(tempyx.warn0wave,m_yx.warn0wave,33*sizeof(char));

	CYX * pClone = new CYX(&tempyx);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

void CYX::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	nHandle--;
	ASSERT(pView!=NULL);
	pView->InvalidateTy(this);
	RectStruct temprect,temprect2;
	BOOL x0move,y0move,x1move,y1move;
	x0move=false;x1move=false;
	y0move=false;y1move=false;
	temprect=m_yx.rect;
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
	m_yx.rect=temprect;
//	GetFontExtractWidth(pView);
//	pView->InvalidateTy(this);
	if (m_yx.pickind0==2){m_recalfont=true;}else{m_recalfont=false;}
	m_recalrect=false;
	RefreshIt();
	if ((m_yx.pickind0==2)||(m_yx.pickind1==2)||(m_yx.pickinddis==2))
	{
		CYXTool::canuse=true;
		CYXTool::m_yx=this->m_yx;
		sprintf(CYXTool::m_yx.ename,"");
		CYXTool::m_yx.fontwidth=(float)(pView->ULtoLL(m_yx.fontwidth));
		CYXTool::m_yx.fontheight=(float)(pView->ULtoLL(m_yx.fontheight));
		CYXTool::m_yx.fontjj=(float)(pView->ULtoLL(m_yx.fontjj));
	}
	else
	{
		CYXTool::canuse=true;
		CYXTool::m_yx=this->m_yx;
		sprintf(CYXTool::m_yx.ename,"");
		CYXTool::m_yx.rect.x0=0;
		CYXTool::m_yx.rect.y0=0;
		CYXTool::m_yx.rect.x1=(float)(pView->ULtoLL(m_yx.rect.x1-m_yx.rect.x0));
		CYXTool::m_yx.rect.y1=(float)(pView->ULtoLL(m_yx.rect.y1-m_yx.rect.y0));
	}
}

void CYX::DrawDC(CDC *pDC, CElecMapView *pView)
{

	if ((m_yx.pickind0==2)&&(m_recalrect))//暂时不明其用途
	{
		GetRectFromFont();
	}
	m_recalrect=false;

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy)) return;
	
	GetRect(&minx,&miny,&maxx,&maxy);
	CPoint pt1,pt2,pt3,pt4;
	pt1=pView->UPtoLP(m_yx.rect.x0,m_yx.rect.y0);
	pt2=pView->UPtoLP(m_yx.rect.x0,m_yx.rect.y1);
	pt3=pView->UPtoLP(m_yx.rect.x1,m_yx.rect.y1);
	pt4=pView->UPtoLP(m_yx.rect.x1,m_yx.rect.y0);

	if ((pt3.x-pt1.x<=2)&&(pt3.y-pt1.y<=2)) return;

	if ( m_nStatus ==0 ){//abnormal
		//display pickind0
		showPickind_zero(pDC,pView,pt1,pt3);
	}
	else if ( m_nStatus ==4 ){//disble
		//display pickinddis
		showPickind_two(pDC,pView,pt1,pt3);
	}
	else{//warning
		//display pickind1		
		if (m_bShow){
			m_bShow =FALSE;
			showPickind_one(pDC,pView,pt1,pt3);
		}
		else{
			m_bShow =TRUE;
		}
	}
}

void CYX::GetFontExtractWidth(CDC * pDC,CElecMapView *pView)
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
	pt1=pView->UPtoLP(m_yx.rect.x0,m_yx.rect.y0);
	pt2=pView->UPtoLP(m_yx.rect.x0,m_yx.rect.y1);
	pt3=pView->UPtoLP(m_yx.rect.x1,m_yx.rect.y1);
	pt4=pView->UPtoLP(m_yx.rect.x1,m_yx.rect.y0);
	int count,hzcount;
	count=GetCharLen(m_yx.picname0,33,&hzcount);
	if ((pt3.x-pt1.x>(YX_MINDISWIDTH*(count+1)
		+pView->ULtoLL(m_yx.fontjj)*(count-hzcount)))&&
		(pt3.y-pt1.y>YX_MINDISHEIGHT))
	{
		m_yx.lgfont.lfHeight=pt3.y-pt1.y;
		m_yx.lgfont.lfWidth=(LONG)((pt3.x-pt1.x
			-pView->ULtoLL(m_yx.fontjj)*(count-hzcount))/
			(count));
		while (true) 
		{
			CFont m_font;
			LOGFONT tempfont;
			tempfont=m_yx.lgfont;
			m_font.CreateFontIndirect(&tempfont);
			CFont * m_oldfont=memdc.SelectObject(&m_font);
			CSize size;
			size=memdc.GetOutputTextExtent(m_yx.picname0,count);//+2);
			size.cx=size.cx;//-m_yx.lgfont.lfWidth*1;//+qq;
			memdc.SelectObject(m_oldfont);
			//if ((minlen<=(pt3.x-pt1.x)*FontSizePercent)||(m_yx.lgfont.lfWidth<=0))
			if (((size.cx*(count+1)/(count))<=
				(pt3.x-pt1.x
				-pView->ULtoLL(m_yx.fontjj)*(count-hzcount)))||(m_yx.lgfont.lfWidth<=0))
			{
				m_yx.fontheight=pView->LLtoUL(m_yx.lgfont.lfHeight);
				m_yx.fontwidth=pView->LLtoUL(m_yx.lgfont.lfWidth);
				break;
			}else{
				m_yx.lgfont.lfWidth--;
			}
		}
	}

	CYXTool::canuse=true;
	CYXTool::m_yx=this->m_yx;
	sprintf(CYXTool::m_yx.ename,"");
	if ((m_yx.pickind0==2)||(m_yx.pickind1==2)||(m_yx.pickinddis==2))
	{
		CYXTool::canuse=true;
		CYXTool::m_yx=this->m_yx;
		sprintf(CYXTool::m_yx.ename,"");
		CYXTool::m_yx.fontwidth=(float)(pView->ULtoLL(m_yx.fontwidth));
		CYXTool::m_yx.fontheight=(float)(pView->ULtoLL(m_yx.fontheight));
		CYXTool::m_yx.fontjj=(float)(pView->ULtoLL(m_yx.fontjj));
	}
}

void CYX::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("开关量对象..") );

	CYX_PROP_INFO dlg0;
	dlg0.SetPara(m_yx.ename,m_yx.handit);
	sheet.AddPage( &dlg0);

	CYX_PROP_DIS dlg1;
	float inw,inh;
	if (m_yx.pickind0==2)
	{
		inw=m_yx.fontwidth;
		inh=m_yx.fontheight;
	}
	else
	{
		inw=m_yx.rect.x1-m_yx.rect.x0;
		inh=m_yx.rect.y1-m_yx.rect.y0;
	}

	dlg1.SetPara(m_yx.lgfont,m_yx.color0,m_yx.color1,m_yx.colordis,
		m_yx.picname0,m_yx.picname1,m_yx.picnamedis,
		m_yx.pickind0,m_yx.pickind1,m_yx.pickinddis,
		m_yx.diskind0,m_yx.diskind1,m_yx.diskinddis,
		m_yx.rect.x0,m_yx.rect.y0,
		inw,inh,m_yx.fontjj);
	sheet.AddPage( &dlg1);

	CYX_PROP_WARN dlg2;
	dlg2.SetPara(m_yx.warn1enable,m_yx.warn1kind,m_yx.warn1levels,
		m_yx.warn1wave,m_yx.warn0enable,m_yx.warn0kind,
		m_yx.warn0levels,m_yx.warn0wave);
	sheet.AddPage( &dlg2);
	   
	if (sheet.DoModal() != IDOK)
		return;

	RefreshIt();
	UnRegisterAllPic();
	dlg0.GetPara(m_yx.ename,&m_yx.handit);
	dlg1.GetPara(&m_yx.lgfont,&m_yx.color0,&m_yx.color1,&m_yx.colordis,
		m_yx.picname0,m_yx.picname1,m_yx.picnamedis,
		&m_yx.pickind0,&m_yx.pickind1,&m_yx.pickinddis,
		&m_yx.diskind0,&m_yx.diskind1,&m_yx.diskinddis,
		&m_yx.rect.x0,&m_yx.rect.y0,
		&inw,&inh,&m_yx.fontjj);

	if ((m_yx.pickind0==2)&&(strlen(m_yx.picname0)==0))
		strcpy(m_yx.picname0," ");
	if ((m_yx.pickind1==2)&&(strlen(m_yx.picname1)==0))
		strcpy(m_yx.picname1," ");
	if ((m_yx.pickinddis==2)&&(strlen(m_yx.picnamedis)==0))
		strcpy(m_yx.picnamedis," ");

	dlg2.GetPara(&m_yx.warn1enable,&m_yx.warn1kind,&m_yx.warn1levels,
		m_yx.warn1wave,&m_yx.warn0enable,&m_yx.warn0kind,
		&m_yx.warn0levels,m_yx.warn0wave);

	RegisterAllPic();
	SetModifiedFlag();

	if ((m_yx.pickind0==2)||(m_yx.pickind1==2)||(m_yx.pickinddis==2))
	{
		m_yx.pickind0=2;
		m_yx.pickind1=2;
		m_yx.pickinddis=2;
		m_recalrect=true;
		m_yx.fontwidth=inw;
		m_yx.fontheight=inh;
		CYXTool::canuse=true;
		CYXTool::m_yx=this->m_yx;
		sprintf(CYXTool::m_yx.ename,"");
		CYXTool::m_yx.fontwidth=(float)(pView->ULtoLL(m_yx.fontwidth));
		CYXTool::m_yx.fontheight=(float)(pView->ULtoLL(m_yx.fontheight));
		CYXTool::m_yx.fontjj=(float)(pView->ULtoLL(m_yx.fontjj));
	}
	else
	{
		m_yx.rect.x1=m_yx.rect.x0+inw;
		m_yx.rect.y1=m_yx.rect.y0+inh;
		CYXTool::canuse=true;
		CYXTool::m_yx=this->m_yx;
		sprintf(CYXTool::m_yx.ename,"");
		CYXTool::m_yx.rect.x0=0;
		CYXTool::m_yx.rect.y0=0;
		CYXTool::m_yx.rect.x1=(float)(pView->ULtoLL(m_yx.rect.x1-m_yx.rect.x0));
		CYXTool::m_yx.rect.y1=(float)(pView->ULtoLL(m_yx.rect.y1-m_yx.rect.y0));
	}
	
	RefreshIt();	
}

void CYX::RefreshIt()
{
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// CYX message handlers
//void Ty_Serialize(CArchive &ar);
//void Ty_Save(CFile *file, BOOL Yn);

void CYX::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);
	if(Yn)	//如果是在进行保存
	{
		m_recalrect=false;
		m_recalfont=false;
		file->Write((unsigned char *)&m_yx.rect,sizeof(m_yx.rect));
		file->Write((unsigned char *)&m_yx.fontwidth,sizeof(m_yx.fontwidth));
		file->Write((unsigned char *)&m_yx.fontheight,sizeof(m_yx.fontheight));
		file->Write((unsigned char *)&m_yx.fontjj,sizeof(m_yx.fontjj));
		file->Write((unsigned char *)m_yx.ename,sizeof(char)*17);
		file->Write((unsigned char *)&m_yx.lgfont,sizeof(m_yx.lgfont));
		file->Write((unsigned char *)&m_yx.color0,sizeof(m_yx.color0));
		file->Write((unsigned char *)&m_yx.color1,sizeof(m_yx.color1));
		file->Write((unsigned char *)&m_yx.colordis,sizeof(m_yx.colordis));
		file->Write((unsigned char *)m_yx.picname0,sizeof(char)*33);
		file->Write((unsigned char *)m_yx.picname1,sizeof(char)*33);
		file->Write((unsigned char *)m_yx.picnamedis,sizeof(char)*33);
		file->Write((unsigned char *)&m_yx.pickind0,sizeof(m_yx.pickind0));
		file->Write((unsigned char *)&m_yx.pickind1,sizeof(m_yx.pickind1));
		file->Write((unsigned char *)&m_yx.pickinddis,sizeof(m_yx.pickinddis));
		file->Write((unsigned char *)&m_yx.diskind0,sizeof(m_yx.diskind0));
		file->Write((unsigned char *)&m_yx.diskind1,sizeof(m_yx.diskind1));
		file->Write((unsigned char *)&m_yx.diskinddis,sizeof(m_yx.diskinddis));

		file->Write((unsigned char *)&m_yx.warn0enable,sizeof(m_yx.warn0enable));
		file->Write((unsigned char *)&m_yx.warn0levels,sizeof(m_yx.warn0levels));
		file->Write((unsigned char *)&m_yx.warn0kind,sizeof(m_yx.warn0kind));
		file->Write((unsigned char *)m_yx.warn0wave,sizeof(char)*33);
		file->Write((unsigned char *)&m_yx.warn1enable,sizeof(m_yx.warn1enable));
		file->Write((unsigned char *)&m_yx.warn1levels,sizeof(m_yx.warn1levels));
		file->Write((unsigned char *)&m_yx.warn1kind,sizeof(m_yx.warn1kind));
		file->Write((unsigned char *)m_yx.warn1wave,sizeof(char)*33);
		file->Write((unsigned char *)&m_yx.handit,sizeof(m_yx.handit));

	} 
	else
	{
		m_recalrect=false;
		m_recalfont=false;
		UnRegisterAllPic();	
		file->Read((unsigned char *)&m_yx.rect,sizeof(m_yx.rect));
		file->Read((unsigned char *)&m_yx.fontwidth,sizeof(m_yx.fontwidth));
		file->Read((unsigned char *)&m_yx.fontheight,sizeof(m_yx.fontheight));
		file->Read((unsigned char *)&m_yx.fontjj,sizeof(m_yx.fontjj));
		file->Read((unsigned char *)m_yx.ename,sizeof(char)*17);
		file->Read((unsigned char *)&m_yx.lgfont,sizeof(m_yx.lgfont));
		file->Read((unsigned char *)&m_yx.color0,sizeof(m_yx.color0));
		file->Read((unsigned char *)&m_yx.color1,sizeof(m_yx.color1));
		file->Read((unsigned char *)&m_yx.colordis,sizeof(m_yx.colordis));
		file->Read((unsigned char *)m_yx.picname0,sizeof(char)*33);
		file->Read((unsigned char *)m_yx.picname1,sizeof(char)*33);
		file->Read((unsigned char *)m_yx.picnamedis,sizeof(char)*33);
		file->Read((unsigned char *)&m_yx.pickind0,sizeof(m_yx.pickind0));
		file->Read((unsigned char *)&m_yx.pickind1,sizeof(m_yx.pickind1));
		file->Read((unsigned char *)&m_yx.pickinddis,sizeof(m_yx.pickinddis));
		file->Read((unsigned char *)&m_yx.diskind0,sizeof(m_yx.diskind0));
		file->Read((unsigned char *)&m_yx.diskind1,sizeof(m_yx.diskind1));
		file->Read((unsigned char *)&m_yx.diskinddis,sizeof(m_yx.diskinddis));

		file->Read((unsigned char *)&m_yx.warn0enable,sizeof(m_yx.warn0enable));
		file->Read((unsigned char *)&m_yx.warn0levels,sizeof(m_yx.warn0levels));
		file->Read((unsigned char *)&m_yx.warn0kind,sizeof(m_yx.warn0kind));
		file->Read((unsigned char *)m_yx.warn0wave,sizeof(char)*33);
		file->Read((unsigned char *)&m_yx.warn1enable,sizeof(m_yx.warn1enable));
		file->Read((unsigned char *)&m_yx.warn1levels,sizeof(m_yx.warn1levels));
		file->Read((unsigned char *)&m_yx.warn1kind,sizeof(m_yx.warn1kind));
		file->Read((unsigned char *)m_yx.warn1wave,sizeof(char)*33);
		file->Read((unsigned char *)&m_yx.handit,sizeof(m_yx.handit));

		if (m_yx.pickind0==2)
		{
			m_yx.pickind1=2;
			m_yx.pickinddis=2;
		}
		RegisterAllPic();
	}

}

void CYX::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if(ar.IsStoring())  
	{
//		ar<<m_yx;
		ar.Write(&m_yx,sizeof(Stu_YX));
	}
	else
	{
//		ar>>m_yx;
		UnRegisterAllPic();
		ar.Read(&m_yx,sizeof(Stu_YX));
		m_yx.ename[0]=0x00;
		if (m_yx.pickind0==2)
		{
			m_yx.pickind1=2;
			m_yx.pickinddis=2;
		}
		RegisterAllPic();
	}
}

//DEL void CYX::DrawText(CDC *pDC, CElecMapView *pView,CPoint pt1,CPoint pt3)
//DEL {
//DEL 	int digitlength=GetCharLen(m_yx.picname0,33);
//DEL 	if (digitlength==0)return;
//DEL 	if ((pt3.x-pt1.x>YX_MINDISWIDTH*(digitlength))&&
//DEL 		(pt3.y-pt1.y>YX_MINDISHEIGHT))
//DEL 	{
//DEL 		if (m_fontwidth==0) 
//DEL 		{
//DEL 			GetFontExtractWidth(pView);
//DEL 		}
//DEL 		m_yx.lgfont.lfHeight=pt3.y-pt1.y;
//DEL 		m_yx.lgfont.lfWidth=m_fontwidth;
//DEL 		LOGFONT tempfont=m_yx.lgfont;
//DEL 		CFont m_font;
//DEL 		m_font.CreateFontIndirect(&tempfont);
//DEL 		CFont * m_oldfont=pDC->SelectObject(&m_font);
//DEL 		int oldmode=pDC->GetBkMode();
//DEL 		pDC->SetBkMode(TRANSPARENT);
//DEL 		pDC->SetTextColor(m_yx.color0);
//DEL 		pDC->TextOut(pt1.x,pt1.y,m_yx.picname0);
//DEL 		pDC->SetBkMode(oldmode);
//DEL 		pDC->SelectObject(m_oldfont);
//DEL 	}
//DEL 
//DEL }

void CYX::DrawBmp(CDC *pDC, CElecMapView *pView,CPoint pt1,CPoint pt3,char* picname)
{
	if ((pt3.x-pt1.x<1)||(pt3.y-pt1.y<1)) return;
	BOOL trans=false;
	if ((m_yx.diskind0&1)>0) trans=true;

	CString strBmpName;
	strBmpName.Format("%s",picname);
	int pos = strBmpName.ReverseFind('_');
	strBmpName = strBmpName.Left(pos);

	if (m_nStatus ==0){
		if (strcmp(strBmpName,_T("电子围栏_正面")) == 0){
			DrawImage(picname,pDC,pt1.x+1,pt1.y+1,pt3.x-pt1.x-2,pt3.y-pt1.y-2,trans,m_yx.color0,FALSE,TRUE);
			return;
		}

		if (strcmp(strBmpName,_T("电子围栏_侧面")) == 0){
			DrawImage(picname,pDC,pt1.x+1,pt1.y+1,pt3.x-pt1.x-2,pt3.y-pt1.y-2,trans,m_yx.color0,TRUE,FALSE);
			return;
		}
		DrawImage(picname,pDC,pt1.x+1,pt1.y+1,pt3.x-pt1.x-2,pt3.y-pt1.y-2,trans,m_yx.color0);
	}
}

void CYX::DrawVec(CDC *pDC, CElecMapView *pView,CPoint pt1,CPoint pt3,char* picname)
{
	if ((pt3.x-pt1.x<1)||(pt3.y-pt1.y<1)) return;
	BOOL trans=false;
	if ((m_yx.diskind0&1)>0) trans=true;
	CRect rt;
	rt.SetRect(pt1.x,pt1.y,pt3.x,pt3.y);

	DrawVector(picname,pDC,rt,rt,m_yx.color0,trans);
}

void CYX::RegisterAllPic()
{
	if ((m_yx.pickind0)==0)
		RegisterImage(m_yx.picname0);
	else if ((m_yx.pickind0)==1)
		RegisterVector(m_yx.picname0);

	if ((m_yx.pickind1)==0)
		RegisterImage(m_yx.picname1);
	else if ((m_yx.pickind1)==1)
		RegisterVector(m_yx.picname1);


	if ((m_yx.pickinddis)==0)
		RegisterImage(m_yx.picnamedis);
	else if ((m_yx.pickinddis)==1)
		RegisterVector(m_yx.picnamedis);

}

void CYX::UnRegisterAllPic()
{
	if ((m_yx.pickind0)==0)
		UnRegisterImage(m_yx.picname0);
	else if ((m_yx.pickind0)==1)
		UnRegisterVector(m_yx.picname0);

	if ((m_yx.pickind1)==0)
		UnRegisterImage(m_yx.picname1);
	else if ((m_yx.pickind1)==1)
		UnRegisterVector(m_yx.picname1);

	if ((m_yx.pickinddis)==0)
		UnRegisterImage(m_yx.picnamedis);
	else if ((m_yx.pickinddis)==1)
		UnRegisterVector(m_yx.picnamedis);

}

void CYX::DrawItText(CDC *pDC, char *s, int count, int jj, int width, int x0, int y0, COLORREF itcolor, int *x1)
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
void CYX::GetRectFromFont()
{
	int hzcount;
	int count=GetCharLen(m_yx.picname0,33,&hzcount);
	m_recalrect=false;
	float tempx0=m_yx.rect.x0;
	float tempy0=m_yx.rect.y0;
	m_yx.rect.x1=tempx0+m_yx.fontwidth*(count+1)
		+m_yx.fontjj*(count-hzcount);
	m_yx.rect.y1=m_yx.rect.y0+m_yx.fontheight*(1);
}

BOOL CYX::Is_Net_Color_Yx()
{
	return TRUE;
}

void CYX::SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[])
{
	if ((m_yx.pickind0!=2)&&(m_yx.pickind1!=2)&&(m_yx.pickinddis!=2)) 
	return;

	m_yx.fontwidth=width;
	m_yx.fontheight=height;
	m_yx.fontjj=jqwidth;
	 if (m_yx.pickind0==2)
		  m_yx.color0=color;
	 if (strlen(font)<32)
	   strcpy(m_yx.lgfont.lfFaceName,font);

	 m_recalrect=true;
	 Invalidate();
   
}

void CYX::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=GetEname_From_ReplacRule(m_yx.ename,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_yx.ename,str);
	}
}

void CYX::showPickind_zero(CDC *pDC, CElecMapView *pView,CPoint pt1,CPoint pt3)
{
	int hzcount;
	int count=GetCharLen(m_yx.picname0,33,&hzcount);
	if ((m_yx.pickind0==2)&&(m_recalfont))
	{
		GetFontExtractWidth(pDC,pView);
	}
	m_recalfont =false;

	if (m_yx.pickind0==0){
		DrawBmp(pDC,pView,pt1,pt3,m_yx.picname0);
	}

	if (m_yx.pickind0==1){
		DrawVec(pDC,pView,pt1,pt3,m_yx.picname0);
	}

	if (m_yx.pickind0==2)
	{
		if ((pt3.x-pt1.x>(YX_MINDISWIDTH*(count+1)
			+pView->ULtoLL(m_yx.fontjj)*(count-hzcount)))&&
			(pt3.y-pt1.y>YX_MINDISHEIGHT))
		{
			m_yx.lgfont.lfHeight=pView->ULtoLL(m_yx.fontheight);
			m_yx.lgfont.lfWidth=pView->ULtoLL(m_yx.fontwidth);
			LOGFONT tempfont=m_yx.lgfont;
			CFont m_font;
			m_font.CreateFontIndirect(&tempfont);
			CFont * m_oldfont=pDC->SelectObject(&m_font);
			int oldmode=pDC->GetBkMode();
			pDC->SetBkMode(TRANSPARENT);
			int x1=pt1.x;int backx;
			int ljj=pView->ULtoLL(m_yx.fontjj);
			int lwidth=pView->ULtoLL(m_yx.fontwidth);

			DrawItText(pDC,m_yx.picname0,count,ljj,lwidth,x1,pt1.y,m_yx.color0,&backx);

			pDC->SetBkMode(oldmode);
			pDC->SelectObject(m_oldfont);
		}
	}
}

void CYX::showPickind_one(CDC *pDC, CElecMapView *pView,CPoint pt1,CPoint pt3)
{
	int hzcount;
	int count=GetCharLen(m_yx.picname0,33,&hzcount);
	if ((m_yx.pickind0==2)&&(m_recalfont))
	{
		GetFontExtractWidth(pDC,pView);
	}
	m_recalfont =false;

	if (m_yx.pickind1==0){
		DrawBmp(pDC,pView,pt1,pt3,m_yx.picname1);
	}

	if (m_yx.pickind1==1){
		DrawVec(pDC,pView,pt1,pt3,m_yx.picname1);
	}

	if (m_yx.pickind1==2)
	{
		if ((pt3.x-pt1.x>(YX_MINDISWIDTH*(count+1)
			+pView->ULtoLL(m_yx.fontjj)*(count-hzcount)))&&
			(pt3.y-pt1.y>YX_MINDISHEIGHT))
		{
			m_yx.lgfont.lfHeight=pView->ULtoLL(m_yx.fontheight);
			m_yx.lgfont.lfWidth=pView->ULtoLL(m_yx.fontwidth);
			LOGFONT tempfont=m_yx.lgfont;
			CFont m_font;
			m_font.CreateFontIndirect(&tempfont);
			CFont * m_oldfont=pDC->SelectObject(&m_font);
			int oldmode=pDC->GetBkMode();
			pDC->SetBkMode(TRANSPARENT);
			int x1=pt1.x;int backx;
			int ljj=pView->ULtoLL(m_yx.fontjj);
			int lwidth=pView->ULtoLL(m_yx.fontwidth);

			DrawItText(pDC,m_yx.picname1,count,ljj,lwidth,x1,pt1.y,m_yx.color1,&backx);

			pDC->SetBkMode(oldmode);
			pDC->SelectObject(m_oldfont);
		}
	}
}

void CYX::showPickind_two(CDC *pDC, CElecMapView *pView,CPoint pt1,CPoint pt3)
{
	int hzcount;
	int count=GetCharLen(m_yx.picname0,33,&hzcount);
	if ((m_yx.pickind0==2)&&(m_recalfont))
	{
		GetFontExtractWidth(pDC,pView);
	}
	m_recalfont =false;

	if (m_yx.pickinddis==0){
		DrawBmp(pDC,pView,pt1,pt3,m_yx.picnamedis);
	}

	if (m_yx.pickinddis==1){
		DrawVec(pDC,pView,pt1,pt3,m_yx.picnamedis);
	}

	if (m_yx.pickinddis==2)
	{
		if ((pt3.x-pt1.x>(YX_MINDISWIDTH*(count+1)
			+pView->ULtoLL(m_yx.fontjj)*(count-hzcount)))&&
			(pt3.y-pt1.y>YX_MINDISHEIGHT))
		{
			m_yx.lgfont.lfHeight=pView->ULtoLL(m_yx.fontheight);
			m_yx.lgfont.lfWidth=pView->ULtoLL(m_yx.fontwidth);
			LOGFONT tempfont=m_yx.lgfont;
			CFont m_font;
			m_font.CreateFontIndirect(&tempfont);
			CFont * m_oldfont=pDC->SelectObject(&m_font);
			int oldmode=pDC->GetBkMode();
			pDC->SetBkMode(TRANSPARENT);
			int x1=pt1.x;int backx;
			int ljj=pView->ULtoLL(m_yx.fontjj);
			int lwidth=pView->ULtoLL(m_yx.fontwidth);

			DrawItText(pDC,m_yx.picnamedis,count,ljj,lwidth,x1,pt1.y,m_yx.colordis,&backx);

			pDC->SetBkMode(oldmode);
			pDC->SelectObject(m_oldfont);
		}
	}
}
