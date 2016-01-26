// YC.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/YM.h"
//#include "Sp_drawView.h"
#include "../../ElecMapView.h"
#include "dialog/YC_PROP_INFO.h"
#include "dialog/YC_PROP_DIS.h"
#include "canvas/RtValueTool.h"
#include "canvas/TyTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYM
IMPLEMENT_SERIAL(CYM, CTyBase, 0)

CYM::CYM(float x0,float y0,float fw,float fh,float jj)
{
	if (true)
	{
	   	sprintf(m_ym.ename,"DEFAULTENAME");
		m_ym.digitlength=6;
		sprintf(m_ym.unit,"");
		m_ym.unitcolor=0xffffff;
		sprintf(m_ym.hint,"");
		m_ym.hintcolor=0xffffff;
		m_ym.lgfont.lfHeight=15;
		m_ym.lgfont.lfWidth=5;
		m_ym.lgfont.lfEscapement=0;//旋转角度
		m_ym.lgfont.lfOrientation=0;
		m_ym.lgfont.lfWeight=FW_MEDIUM;
		m_ym.lgfont.lfItalic=false;
//		m_ym.lgfont.lfWeight=700;
//		m_ym.lgfont.lfItalic=true;
		m_ym.lgfont.lfUnderline=false;
		m_ym.lgfont.lfStrikeOut=false;//强调线
		m_ym.lgfont.lfCharSet=GB2312_CHARSET;
		m_ym.lgfont.lfOutPrecision=OUT_DEFAULT_PRECIS;//输出精度
		m_ym.lgfont.lfClipPrecision=CLIP_DEFAULT_PRECIS;//剪裁精度
		m_ym.lgfont.lfQuality=DEFAULT_QUALITY;
		m_ym.lgfont.lfPitchAndFamily=DEFAULT_PITCH;//间距和字体族
		sprintf(m_ym.lgfont.lfFaceName,"宋体");
		m_ym.normalcolor=0xff0000;
		m_ym.disablecolor=0xffffff;
		m_ym.hintenable=0;
		
		m_ym.rect.x0=x0;
		m_ym.rect.x1=x0;
		m_ym.rect.y0=y0;
		m_ym.rect.y1=y0;
		m_ym.fontwidth=fw;
		m_ym.fontheight=fh;
		m_ym.fontjj=0;
		m_ym.hintdis=0;
		m_ym.unitdis=0;
		m_recalrect=true;
		m_recalfont=false;
	}
}

CYM::CYM(Stu_YM * ym)
{
	m_ym=*ym;
	memcpy(m_ym.ename,ym->ename,sizeof(char)*17);
	memcpy(m_ym.hint,ym->hint,sizeof(char)*33);
	memcpy(m_ym.unit,ym->unit,sizeof(char)*17);
	m_recalrect=false;
	m_recalfont=false;
}

CYM::~CYM()
{
}




/////////////////////////////////////////////////////////////////////////////
// CYM message handlers

int CYM::GetHandleCount()
{
	return 8;
}

PointStruct CYM::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	nHandle--;
	PointStruct pt;
	pt.x=0;pt.y=0;
	if (nHandle==0)
	{
		pt.x=m_ym.rect.x0;
		pt.y=m_ym.rect.y0;
	}
	if (nHandle==4)
	{
		pt.x=m_ym.rect.x0;
		pt.y=m_ym.rect.y0+(m_ym.rect.y1-m_ym.rect.y0)/2;
	}
	if (nHandle==2)
	{
		pt.x=m_ym.rect.x0;
		pt.y=m_ym.rect.y1;
	}
	if (nHandle==3)
	{
		pt.x=m_ym.rect.x0+(m_ym.rect.x1-m_ym.rect.x0)/2;
		pt.y=m_ym.rect.y1;
	}
	if (nHandle==1)
	{
		pt.x=m_ym.rect.x1;
		pt.y=m_ym.rect.y1;
	}
	if (nHandle==5)
	{
		pt.x=m_ym.rect.x1;
		pt.y=m_ym.rect.y0+(m_ym.rect.y1-m_ym.rect.y0)/2;
	}
	if (nHandle==6)
	{
		pt.x=m_ym.rect.x1;
		pt.y=m_ym.rect.y0;
	}
	if (nHandle==7)
	{
		pt.x=m_ym.rect.x0+(m_ym.rect.x1-m_ym.rect.x0)/2;
		pt.y=m_ym.rect.y0;
	}
	return pt;

}

HCURSOR CYM::GetHandleCursor(int nHandle)
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

BOOL CYM::IntersectRect(float x0, float y0, float x1, float y1)
{

	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

//DEL int CYM::GetCharLen(char *schar)
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

DRAW_TY CYM::GetTyType()
{
	return tyYM;
}

int CYM::GetCharLen(char *schar,int maxlen,int * hzcount)
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
void CYM::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	if (m_recalrect)
	{
		GetRectFromFont();
		m_recalrect=false;
	}
	*minX=m_ym.rect.x0;
	*minY=m_ym.rect.y0;
	*maxX=m_ym.rect.x1;
	*maxY=m_ym.rect.y1;
}

void CYM::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView!=NULL) pView->InvalidateTy(this);
	if (m_ym.rect.x0+xLen>=0) 
	{
		m_ym.rect.x0+=xLen;
		m_ym.rect.x1+=xLen;
	}
	if (m_ym.rect.y0+yLen>=0)
	{
		m_ym.rect.y0+=yLen;
		m_ym.rect.y1+=yLen;
	}
	if (pView!=NULL) pView->InvalidateTy(this);
}

BOOL CYM::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange>=0);
	if (m_bDelete) {return false;}
	if ((x>=max(0,m_ym.rect.x0-errRange))&&
		(y>=max(0,m_ym.rect.y0-errRange))&&
		(x<=m_ym.rect.x1+errRange)&&
		(y<=m_ym.rect.y1+errRange))
	{
		return true;
	}

	return false;
}

CTyBase * CYM::Clone()
{
	ASSERT_VALID(this);
	Stu_YM  tempym;
	tempym=m_ym;
	memcpy(tempym.ename,m_ym.ename,17*sizeof(char));
	memcpy(tempym.unit,m_ym.unit,17*sizeof(char));
	memcpy(tempym.hint,m_ym.hint,33*sizeof(char));

	CYM * pClone = new CYM(&tempym);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

void CYM::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	nHandle--;
	ASSERT(pView!=NULL);
	pView->InvalidateTy(this);
	RectStruct temprect,temprect2;
	BOOL x0move,y0move,x1move,y1move;
	x0move=false;x1move=false;
	y0move=false;y1move=false;
	temprect=m_ym.rect;
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
	m_ym.rect=temprect;
	m_recalfont=true;
	m_recalrect=false;
	RefreshIt();
}

void CYM::DrawDC(CDC *pDC, CElecMapView *pView)
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
	pt1=pView->UPtoLP(m_ym.rect.x0,m_ym.rect.y0);
	pt2=pView->UPtoLP(m_ym.rect.x0,m_ym.rect.y1);
	pt3=pView->UPtoLP(m_ym.rect.x1,m_ym.rect.y1);
	pt4=pView->UPtoLP(m_ym.rect.x1,m_ym.rect.y0);

	if ((pt3.x-pt1.x<=2)&&(pt3.y-pt1.y<=2)) return;

	int hinthz,unithz;int hintdis,unitdis;
	int hintcount=GetCharLen(m_ym.hint,33,&hinthz);
	int unitcount=GetCharLen(m_ym.unit,17,&unithz);
	hintdis=0;unitdis=0;
	if (hintcount>0) hintdis=pView->ULtoLL(m_ym.hintdis);
	if (unitcount>0) unitdis=pView->ULtoLL(m_ym.unitdis);
	if (m_recalfont)
	{
		m_recalfont=false;
		GetFontExtractWidth(pDC,pView);
	}

	//if ((pt3.x-pt1.x>1024)||(pt3.y-pt1.y>768)) return;

	if ((pt3.x-pt1.x>(YM_MINDISWIDTH*(m_ym.digitlength+hintcount+unitcount+1)
		+pView->ULtoLL(m_ym.fontjj)*(m_ym.digitlength+hintcount+unitcount-hinthz-unithz)))&&
		(pt3.y-pt1.y>YM_MINDISHEIGHT))
	{
		CString ttstr=m_ym.ename;
		for (int kk2=0;kk2<m_ym.digitlength;kk2++)
		{
			ttstr=ttstr+" ";
		}
		char s[256];
		sprintf(s,"%s",ttstr);
		s[m_ym.digitlength]='\0';
		m_ym.lgfont.lfHeight=pView->ULtoLL(m_ym.fontheight);
		m_ym.lgfont.lfWidth=pView->ULtoLL(m_ym.fontwidth);
		LOGFONT tempfont=m_ym.lgfont;
		CFont m_font;
		m_font.CreateFontIndirect(&tempfont);
		CFont * m_oldfont=pDC->SelectObject(&m_font);
		int oldmode=pDC->GetBkMode();
		pDC->SetBkMode(TRANSPARENT);
		int x1=pt1.x;int backx;
		int ljj=pView->ULtoLL(m_ym.fontjj);
		int lwidth=pView->ULtoLL(m_ym.fontwidth);
		if (hintcount!=0)
		{
			DrawText(pDC,m_ym.hint,hintcount,ljj,lwidth,x1,pt1.y,m_ym.hintcolor,&backx);
			x1=backx;
		}
		x1=x1+hintdis;
		DrawText(pDC,s,m_ym.digitlength,ljj,lwidth,x1,pt1.y,m_ym.normalcolor,&backx);
		x1=backx;
		x1=x1+unitdis;
		if (unitcount!=0)
		{
			DrawText(pDC,m_ym.unit,unitcount,ljj,lwidth,x1,pt1.y,m_ym.unitcolor,&backx);
			x1=backx;
		}
		pDC->SetBkMode(oldmode);
		pDC->SelectObject(m_oldfont);
		/*if ((pt3.x-pt1.x<size.cx))
		{
			pt3.x=pt1.x+size.cx;
			float x,y;
			pView->LPtoUP(CPoint(size.cx),&x,&y);
			m_ym.rect.x1=m_ym.rect.x0+x;
		}*/
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

void CYM::GetFontExtractWidth(CDC *pDC,CElecMapView *pView)
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
	pt1=pView->UPtoLP(m_ym.rect.x0,m_ym.rect.y0);
	pt2=pView->UPtoLP(m_ym.rect.x0,m_ym.rect.y1);
	pt3=pView->UPtoLP(m_ym.rect.x1,m_ym.rect.y1);
	pt4=pView->UPtoLP(m_ym.rect.x1,m_ym.rect.y0);
	int hintcount,unitcount,hinthz,unithz;int hintdis,unitdis;
	hintcount=GetCharLen(m_ym.hint,33,&hinthz);
	unitcount=GetCharLen(m_ym.unit,17,&unithz);
	hintdis=0;unitdis=0;
	if (hintcount>0) hintdis=pView->ULtoLL(m_ym.hintdis);
	if (unitcount>0) unitdis=pView->ULtoLL(m_ym.unitdis);
	char s[256];
	memcpy(s,YM_STRING,sizeof(char)*m_ym.digitlength);
	s[m_ym.digitlength]='\0';
	CString tmpstr;
	tmpstr="";
	tmpstr=tmpstr+m_ym.hint;
	tmpstr=tmpstr+s;
	tmpstr=tmpstr+m_ym.unit;
	if ((pt3.x-pt1.x-hintdis-unitdis>(YM_MINDISWIDTH*(m_ym.digitlength+hintcount+unitcount+1)
		+pView->ULtoLL(m_ym.fontjj)*(m_ym.digitlength+hintcount+unitcount-hinthz-unithz)))&&
		(pt3.y-pt1.y>YM_MINDISHEIGHT))
	{
		m_ym.lgfont.lfHeight=pt3.y-pt1.y;
		m_ym.lgfont.lfWidth=(LONG)(pt3.x-pt1.x-hintdis-unitdis
			-pView->ULtoLL(m_ym.fontjj)*(m_ym.digitlength+hintcount+unitcount-hinthz-unithz))/
			(m_ym.digitlength+hintcount+unitcount);
		while (true) 
		{
			CFont m_font;
			LOGFONT tempfont;
			tempfont=m_ym.lgfont;
			m_font.CreateFontIndirect(&tempfont);
			CFont * m_oldfont=memdc.SelectObject(&m_font);
			CSize size;
			size=memdc.GetOutputTextExtent(tmpstr,m_ym.digitlength+hintcount+unitcount);//+2);
			size.cx=size.cx;//-m_ym.lgfont.lfWidth*1;//+qq;
			memdc.SelectObject(m_oldfont);
			//if ((minlen<=(pt3.x-pt1.x)*FontSizePercent)||(m_ym.lgfont.lfWidth<=0))
			if (((size.cx*(m_ym.digitlength+hintcount+unitcount+1)/(m_ym.digitlength+hintcount+unitcount))<=
				(pt3.x-pt1.x-hintdis-unitdis
				-pView->ULtoLL(m_ym.fontjj)*(m_ym.digitlength+hintcount+unitcount-hinthz-unithz)))||(m_ym.lgfont.lfWidth<=0))
			{
				m_ym.fontheight=pView->LLtoUL(m_ym.lgfont.lfHeight);
				m_ym.fontwidth=pView->LLtoUL(m_ym.lgfont.lfWidth);
				break;
			}else{
				m_ym.lgfont.lfWidth--;
			}
		}
	}
	CYMTool::canuse=true;
	CYMTool::m_ym=this->m_ym;
	sprintf(CYMTool::m_ym.ename,"");
	CYMTool::m_ym.fontwidth=(float)(pView->ULtoLL(m_ym.fontwidth));
	CYMTool::m_ym.fontheight=(float)(pView->ULtoLL(m_ym.fontheight));
	CYMTool::m_ym.fontjj=(float)(pView->ULtoLL(m_ym.fontjj));
	CYMTool::m_ym.hintdis=(float)(pView->ULtoLL(m_ym.hintdis));
}
void CYM::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("累加量对象..") );

    CYC_PROP_INFO dlg0(2);
	dlg0.SetPara(m_ym.ename,m_ym.digitlength,m_ym.hint,m_ym.unit,
	    m_ym.unitcolor,m_ym.hintcolor,m_ym.hintenable,m_ym.hintdis,m_ym.unitdis);
	sheet.AddPage( &dlg0);
    CYC_PROP_DIS dlg1(2);
	dlg1.SetPara2(m_ym.disablecolor,m_ym.normalcolor,
		m_ym.lgfont,m_ym.rect.x0,m_ym.rect.y0,
		m_ym.fontwidth,m_ym.fontheight,m_ym.fontjj);
	sheet.AddPage( &dlg1);
	   
	if (sheet.DoModal() != IDOK)
		return;
	RefreshIt();	
	dlg0.GetPara(m_ym.ename,&m_ym.digitlength,m_ym.hint,m_ym.unit,
	    &m_ym.unitcolor,&m_ym.hintcolor,&m_ym.hintenable,&m_ym.hintdis,&m_ym.unitdis);
	dlg1.GetPara2(&m_ym.disablecolor,&m_ym.normalcolor,
		&m_ym.lgfont,&m_ym.rect.x0,&m_ym.rect.y0,
		&m_ym.fontwidth,&m_ym.fontheight,&m_ym.fontjj);
	SetModifiedFlag();
	
	m_recalrect=true;
	RefreshIt();	
	CYMTool::canuse=true;
	CYMTool::m_ym=this->m_ym;
	sprintf(CYMTool::m_ym.ename,"");
	CYMTool::m_ym.fontwidth=(float)(pView->ULtoLL(m_ym.fontwidth));
	CYMTool::m_ym.fontheight=(float)(pView->ULtoLL(m_ym.fontheight));
	CYMTool::m_ym.fontjj=(float)(pView->ULtoLL(m_ym.fontjj));
	CYMTool::m_ym.hintdis=(float)(pView->ULtoLL(m_ym.hintdis));
}

void CYM::RefreshIt()
{
	Invalidate();
}

void CYM::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_ym.rect,sizeof(m_ym.rect));
		file->Write((unsigned char *)&m_ym.fontwidth,sizeof(m_ym.fontwidth));
		file->Write((unsigned char *)&m_ym.fontheight,sizeof(m_ym.fontheight));
		file->Write((unsigned char *)&m_ym.fontjj,sizeof(m_ym.fontjj));
		file->Write((unsigned char *)&m_ym.hintdis,sizeof(m_ym.hintdis));
		file->Write((unsigned char *)&m_ym.unitdis,sizeof(m_ym.unitdis));
		file->Write((unsigned char *)m_ym.ename,sizeof(char)*17);
		file->Write((unsigned char *)&m_ym.digitlength,sizeof(m_ym.digitlength));
		file->Write((unsigned char *)m_ym.hint,sizeof(char)*33);
		file->Write((unsigned char *)m_ym.unit,sizeof(char)*17);
		file->Write((unsigned char *)&m_ym.lgfont,sizeof(m_ym.lgfont));
		file->Write((unsigned char *)&m_ym.hintcolor,sizeof(m_ym.hintcolor));
		file->Write((unsigned char *)&m_ym.unitcolor,sizeof(m_ym.unitcolor));
		file->Write((unsigned char *)&m_ym.normalcolor,sizeof(m_ym.normalcolor));
		file->Write((unsigned char *)&m_ym.disablecolor,sizeof(m_ym.disablecolor));
		file->Write((unsigned char *)&m_ym.hintenable,sizeof(m_ym.hintenable));
	} 
	else
	{
		m_recalrect=false;
		m_recalfont=false;
		file->Read((unsigned char *)&m_ym.rect,sizeof(m_ym.rect));
		file->Read((unsigned char *)&m_ym.fontwidth,sizeof(m_ym.fontwidth));
		file->Read((unsigned char *)&m_ym.fontheight,sizeof(m_ym.fontheight));
		file->Read((unsigned char *)&m_ym.fontjj,sizeof(m_ym.fontjj));
		file->Read((unsigned char *)&m_ym.hintdis,sizeof(m_ym.hintdis));
		file->Read((unsigned char *)&m_ym.unitdis,sizeof(m_ym.unitdis));
		file->Read((unsigned char *)m_ym.ename,sizeof(char)*17);
		file->Read((unsigned char *)&m_ym.digitlength,sizeof(m_ym.digitlength));
		file->Read((unsigned char *)m_ym.hint,sizeof(char)*33);
		file->Read((unsigned char *)m_ym.unit,sizeof(char)*17);
		file->Read((unsigned char *)&m_ym.lgfont,sizeof(m_ym.lgfont));
		file->Read((unsigned char *)&m_ym.hintcolor,sizeof(m_ym.hintcolor));
		file->Read((unsigned char *)&m_ym.unitcolor,sizeof(m_ym.unitcolor));
		file->Read((unsigned char *)&m_ym.normalcolor,sizeof(m_ym.normalcolor));
		file->Read((unsigned char *)&m_ym.disablecolor,sizeof(m_ym.disablecolor));
		file->Read((unsigned char *)&m_ym.hintenable,sizeof(m_ym.hintenable));
	}
}

void CYM::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if(ar.IsStoring())  
	{
//		ar<<m_ym;
		ar.Write(&m_ym,sizeof(Stu_YM));
	}
	else
	{
//		ar>>m_ym;
		m_recalrect=false;
		m_recalfont=false;
		ar.Read(&m_ym,sizeof(Stu_YM));
		m_ym.ename[0]=0x00;
	}
}

void CYM::DrawText(CDC * pDC,char *s, int count, int jj, int width,int x0, int y0, COLORREF itcolor, int *x1)
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

void CYM::GetRectFromFont()
{
	int hinthz,unithz;
	int hintcount=GetCharLen(m_ym.hint,33,&hinthz);
	int unitcount=GetCharLen(m_ym.unit,17,&unithz);
	m_recalrect=false;
	float tempx0=m_ym.rect.x0;
	float tempy0=m_ym.rect.y0;
	if (hintcount>0) tempx0=tempx0+m_ym.hintdis;
	if (unitcount>0) tempx0=tempx0+m_ym.unitdis;
	m_ym.rect.x1=tempx0+m_ym.fontwidth*(m_ym.digitlength+hintcount+unitcount+1)
		+m_ym.fontjj*(m_ym.digitlength+hintcount+unitcount-hinthz-unithz);
	m_ym.rect.y1=m_ym.rect.y0+m_ym.fontheight*(1);
}


void CYM::SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[])
{
	m_ym.fontwidth=width;
	m_ym.fontheight=height;
	m_ym.fontjj=jqwidth;
	m_ym.normalcolor=color;
	
	if (strlen(font)<32)
		strcpy(m_ym.lgfont.lfFaceName,font);

	m_recalrect=true;
	Invalidate();
}

void CYM::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=GetEname_From_ReplacRule(m_ym.ename,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_ym.ename,str);
	}
}
