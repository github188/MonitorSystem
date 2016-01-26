// JDJH.cpp: implementation of the CLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
//#include "sp_draw.h"
#include "canvas/JDJH.h"
//#include "sp_drawview.h"
#include "../../ElecMapView.h"
#include "canvas/TyBase.h"
#include "canvas/imageX.h"
#include "canvas/HMLayer.h"
#include "canvas/HuaMian.h"
#include "canvas/lineprop.h"
#include "canvas/TyTool.h"
#include "canvas/dashline.h"

#include "dialog/XLineDlg.h"
#include "dialog/FillProp.h"
#include "dialog/InputText.h"
#include "dialog/FontProp.h"
#include "dialog/BmpSelectPage.h"
#include "dialog/PropPositionRect.h"
#include "dialog/PropPositionText.h"
#include "dialog/PropPositionLine.h"
#include "dialog/SelectVectorDlg.h"
#include "dialog/McCurveDialog.h"
#include "dialog/ymccurvesheet.h"
#include "dialog/YcCurveDialog.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CLine, CTyBase, 0)


CLine::CLine(float x0,float y0,float x1,float y1, LOGPEN *pLogPen,int vId,CHMLayer* pLayer)
	:CTyBase(vId,pLayer)
{
	ASSERT(pLogPen != NULL);
	m_x0 = x0;
	m_y0 = y0;
	m_x1 = x1;
	m_y1 = y1;
	m_logpen = *pLogPen;
}

CLine::~CLine()
{

}

void CLine::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView != NULL) 
		pView->InvalidateTy(this);

	m_x0 += xLen;
	m_x1 += xLen;
	m_y0 += yLen;
	m_y1 += yLen;

	if (pView != NULL) 
		pView->InvalidateTy(this);
}

void CLine::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC != NULL);
	ASSERT(pView != NULL);

	if(m_bDelete)	//如果已经处于删除状态
		return;

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy))
		return;

	CPen pen(m_logpen.lopnStyle,m_logpen.lopnWidth.x,m_logpen.lopnColor);
	CPen * pOldPen = pDC->SelectObject(&pen);
	int holdDrawMode = pDC->SetROP2(R2_COPYPEN);

	GetRect(&minx,&miny,&maxx,&maxy);

	CPoint pt1,pt2;
	pt1 = pView->UPtoLP(m_x0,m_y0);
	pt2 = pView->UPtoLP(m_x1,m_y1);

	pDC->MoveTo(pt1); 
	pDC->LineTo(pt2);

	pDC->SetROP2(holdDrawMode);
	pDC->SelectObject(pOldPen);
}

void CLine::DrawRect(CDC *pDC, int orgX,int orgY,float offsetX, float offsetY,
										 float scaleX, float scaleY, COLORREF ReplaceColor, 
										 COLORREF ToColor, BOOL bReplace)
{
	COLORREF color;

	if(bReplace&&(m_logpen.lopnColor == ReplaceColor))
		color = ToColor;
	else 	
		color = m_logpen.lopnColor;

	CPen pen(m_logpen.lopnStyle,m_logpen.lopnWidth.x,color);
	CPen * pOldPen = pDC->SelectObject(&pen);
	int holdDrawMode = pDC->SetROP2(R2_COPYPEN);

	CPoint pt1,pt2;
	pt1.x = orgX+(int)((m_x0-offsetX)*scaleX);
	pt1.y = orgY+(int)((m_y0-offsetY)*scaleY);
	pt2.x = orgX+(int)((m_x1-offsetX)*scaleX);
	pt2.y = orgY+(int)((m_y1-offsetY)*scaleY);

	pDC->MoveTo(pt1); 
	pDC->LineTo(pt2);

	pDC->SetROP2(holdDrawMode);
	pDC->SelectObject(pOldPen);
}

void CLine::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	*minX = __min(m_x0,m_x1);
	*minY = __min(m_y0,m_y1);
	*maxX = __max(m_x0,m_x1);
	*maxY = __max(m_y0,m_y1);
}

CTyBase * CLine::Clone()
{
	ASSERT_VALID(this);

	CLine* pClone = new CLine(m_x0,m_y0,m_x1,m_y1,&m_logpen);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

BOOL CLine::IntersectRect(float x0, float y0, float x1, float y1)
{

	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

BOOL CLine::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange >= 0);

	if (m_bDelete)
		return FALSE; 

	if ((x < max(0,__min(m_x0,m_x1)-errRange))
		&&(y<max(0,__min(m_y0,m_y1)-errRange))
		&&(x>__max(m_x0,m_x1)+errRange)
		&&(y>__max(m_y0,m_y1)+errRange)) 
		return FALSE;


  float dist = PointLine(x,y,m_x0,m_y0,m_x1,m_y1);
  if (dist > errRange) 
	  return FALSE;
  else
	  return TRUE; 
}

int CLine::GetHandleCount()
{
	return 2;
}

PointStruct CLine::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	PointStruct pt;

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);
		break;

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

HCURSOR CLine::GetHandleCursor(int nHandle)
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


void CLine::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView != NULL);

	pView->InvalidateTy(this);
	
	switch (nHandle)
	{
	case 1:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1)>fabs(point.y-m_y1)) 
			{
				m_y0 = m_y1;
				m_x0 = point.x;
			}
			else 
			{
				m_x0 = m_x1;
				m_y0 = point.y;
			}
		}
		else 
		{
			m_x0 = point.x;
			m_y0 = point.y;
		}
    break;

	case 2:
		if ((nFlags&MK_SHIFT)!=0)
		{
			if (fabs(point.x-m_x0)>fabs(point.y-m_y0)) 
			{
				m_y1 = m_y0;
				m_x1 = point.x;
			}
			else 
			{
				m_x1 = m_x0;
				m_y1 = point.y;
			}
		} 
		else
		{ 
		  m_x1 = point.x;
		  m_y1 = point.y;
		}
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	pView->InvalidateTy(this);
}

void CLine::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);

	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_x0,sizeof(m_x0));
		file->Write((unsigned char *)&m_y0,sizeof(m_y0));
		file->Write((unsigned char *)&m_x1,sizeof(m_x1));
		file->Write((unsigned char *)&m_y1,sizeof(m_y1));

		file->Write((unsigned char *)&m_logpen.lopnStyle,sizeof(m_logpen.lopnStyle));
		file->Write((unsigned char *)&m_logpen.lopnColor,sizeof(m_logpen.lopnColor));
		file->Write((unsigned char *)&m_logpen.lopnWidth.x,sizeof(m_logpen.lopnWidth.x));
	} 
	else
	{
		file->Read((unsigned char *)&m_x0,sizeof(m_x0));
		file->Read((unsigned char *)&m_y0,sizeof(m_y0));
		file->Read((unsigned char *)&m_x1,sizeof(m_x1));
		file->Read((unsigned char *)&m_y1,sizeof(m_y1));
	
		file->Read((unsigned char *)&m_logpen.lopnStyle,sizeof(m_logpen.lopnStyle));
		file->Read((unsigned char *)&m_logpen.lopnColor,sizeof(m_logpen.lopnColor));
		file->Read((unsigned char *)&m_logpen.lopnWidth.x,sizeof(m_logpen.lopnWidth.x));
	}
}

void CLine::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if(ar.IsStoring())  
	{
		ar<<m_x0<<m_y0<<m_x1<<m_y1;
		ar.Write(&m_logpen,sizeof(m_logpen));
	}
	else
	{
		ar>>m_x0>>m_y0>>m_x1>>m_y1;
		ar.Read(&m_logpen,sizeof(m_logpen));
	}
}

DRAW_TY CLine::GetTyType()
{
	return tyLine;
}

void CLine::SetDefaultProp(CElecMapView *pView)
{
	memcpy(&(CTyTool::c_LogPen),&m_logpen,sizeof(m_logpen));
}

void CLine::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("线条对象..") );
	CLineProp dlg;
	dlg.SetLogPen(&m_logpen);
	sheet.AddPage( (CPropertyPage*)&dlg );

	CPropPositionLine dlgpos;
	dlgpos.SetPosition(m_x0,m_y0,m_x1,m_y1);
	sheet.AddPage( (CPropertyPage*)&dlgpos );

	if (sheet.DoModal() == IDOK)
	{
		Invalidate();
		dlg.GetLogPen(&m_logpen);
		dlgpos.GetPosition(m_x0,m_y0,m_x1,m_y1);
		SetModifiedFlag();
		Invalidate();
		SetDefaultProp(pView);
		return;
	}
}

BOOL CLine::Is_Net_Color_Vector()
{
	return TRUE;
}

void CLine::Set_Ty_Color(COLORREF color,CElecMapView * pView)
{
	if (m_logpen.lopnColor == color) 
		return;

	m_logpen.lopnColor = color;
	SetModifiedFlag();

	ASSERT(pView != NULL);
	if (pView == NULL) 
		return;

	pView->InvalidateTy(this);
}

void CLine::Set_Line_Width(int width, CElecMapView *pView)
{
	if (m_logpen.lopnWidth.x == width) 
		return;	

	ASSERT(pView != NULL);
	if (pView == NULL) 
		return;

	pView->InvalidateTy(this);
	m_logpen.lopnWidth.x = width;
	SetModifiedFlag();
	pView->InvalidateTy(this);
}



//////////////////////////////////////////////////////////////////////
// CXLine Class
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CXLine, CTyBase, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXLine::CXLine(float x0,float y0,float x1,float y1,COLORREF Color1, COLORREF Color2, BOOL blTranslate,int nWidth, int Type[4],int vId,CHMLayer* pLayer)
	:CTyBase(vId,pLayer)
{
	m_x0 = x0;
	m_y0 = y0;
	m_x1 = x1;
	m_y1 = y1;
	m_iWidth = nWidth;
	m_color1 = Color1;
	m_color2 = Color2;
	memcpy(m_Type,Type,4 * sizeof(int));
	m_blTranslate = blTranslate;	
}

CXLine::~CXLine()
{

}

void CXLine::Set_Line_Width(int width, CElecMapView *pView)
{
	if (m_iWidth == width) return;	
	ASSERT(pView != NULL);
	if (pView == NULL) return;

	pView->InvalidateTy(this);
	m_iWidth = width;
	SetModifiedFlag();
	pView->InvalidateTy(this);
}

void CXLine::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC != NULL);
	ASSERT(pView != NULL);
	CPoint pt0,pt1;

	if(m_bDelete)	//如果已经处于删除状态
	  return;
	
	pt0 = pView->UPtoLP(m_x0,m_y0);
	pt1 = pView->UPtoLP(m_x1,m_y1);
  
	CPen *pOldPen = pDC->GetCurrentPen();

	CDashLine ln(pDC,m_color1,m_color2,m_blTranslate,m_iWidth,m_Type);
	ln.MoveTo(pt0.x,pt0.y);
	ln.LineTo(pt1.x,pt1.y);

	pDC->SelectObject(pOldPen);
}

PointStruct CXLine::GetHandle(int nHandle)
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

int CXLine::GetHandleCount()
{
	return 2;
}

HCURSOR CXLine::GetHandleCursor(int nHandle)
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

void CXLine::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView != NULL);

	pView->InvalidateTy(this);
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);
    break;
	case 1:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1)>fabs(point.y-m_y1)) 
			{
				m_y0 = m_y1;
				m_x0 = point.x;
			}
			else 
			{
				m_x0 = m_x1;
				m_y0 = point.y;
			}
		}
		else 
		{
			m_x0 = point.x;
			m_y0 = point.y;
		}
    break;
	case 2:
		if ((nFlags&MK_SHIFT)!=0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y0)) 
			{
				m_y1 = m_y0;
				m_x1 = point.x;
			}
			else 
			{
				m_x1 = m_x0;
				m_y1 = point.y;
			}
		} 
		else
		{ 
			m_x1 = point.x;
			m_y1 = point.y;
		}
		break;
	}

	pView->InvalidateTy(this);
}

void CXLine::Ty_Save(CFile *file, BOOL Yn)
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
	}
}

void CXLine::Ty_Serialize(CArchive &ar)
{
	int i;

	if (ar.IsStoring())
	{
		ar<<m_x0<<m_y0<<m_x1<<m_y1<<m_iWidth<<m_color1<<m_color2<<m_blTranslate;
		for (i=0;i<4;i++)
		{
			ar<<m_Type[i];
		}
	} 
	else 
	{
		ar>>m_x0>>m_y0>>m_x1>>m_y1>>m_iWidth>>m_color1>>m_color2>>m_blTranslate;
		for (i=0;i<4;i++)
		{
			ar>>m_Type[i];
		}
	}
}

DRAW_TY CXLine::GetTyType()
{
	return tyXX;
}

void CXLine::SetDefaultProp(CElecMapView *pView)
{
	CXLineTool::c_blTranslate = m_blTranslate;
	CXLineTool::c_color1 = m_color1;
	CXLineTool::c_color2 = m_color2;
	CXLineTool::c_nWidth = m_iWidth;
	memcpy(CXLineTool::c_type,m_Type,sizeof(int)*4);
}

void CXLine::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("对象..") );
	CXLineProp dlg;
	dlg.SetXLine(m_color1,m_color2,m_blTranslate,m_iWidth,m_Type);
	sheet.AddPage( (CPropertyPage*)&dlg );
  
	CPropPositionLine dlgpos;
	dlgpos.SetPosition(m_x0,m_y0,m_x1,m_y1);
	sheet.AddPage( (CPropertyPage*)&dlgpos );

	if (sheet.DoModal() == IDOK)
	{
		Invalidate();
		dlg.GetXLine(m_color1,m_color2,m_blTranslate,m_iWidth,m_Type);
		dlgpos.GetPosition(m_x0,m_y0,m_x1,m_y1);
		SetModifiedFlag();
		Invalidate();
		SetDefaultProp(pView);
		return;
	}
}

BOOL CXLine::PointInObj(float x, float y, float errRange)
{
	ASSERT(errRange >= 0);
	if (m_bDelete) 
		return FALSE; 

	if ((x < max(0,__min(m_x0,m_x1)-errRange))
		&&(y < max(0,__min(m_y0,m_y1)-errRange))
		&&(x > __max(m_x0,m_x1)+errRange)
		&&(y>__max(m_y0,m_y1)+errRange)) return FALSE;

	float dist = PointLine(x,y,m_x0,m_y0,m_x1,m_y1);
	if (dist > errRange)
		return FALSE;
	else 
		return TRUE; 
}

BOOL CXLine::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

CTyBase * CXLine::Clone()
{
	ASSERT_VALID(this);

	CXLine* pClone = new CXLine(m_x0,m_y0,m_x1,m_y1,m_color1,m_color2,m_blTranslate,m_iWidth,m_Type);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

void CXLine::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	*minX = __min(m_x0,m_x1);
	*minY = __min(m_y0,m_y1);
	*maxX = __max(m_x0,m_x1);
	*maxY = __max(m_y0,m_y1);
}

void CXLine::Ty_Move(float xLen, float yLen, CElecMapView *pView)
{
	if (pView != NULL) 
		pView->InvalidateTy(this);

	m_x0 += xLen;
	m_x1 += xLen;
	m_y0 += yLen;
	m_y1 += yLen;

	if (pView != NULL) 
		pView->InvalidateTy(this);
}

BOOL CXLine::Is_Net_Color_Vector()
{
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
//  CRectangle
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CRectangle, CTyBase, 0)

CRectangle::CRectangle(float x0,float y0,float x1,float y1, LOGPEN *pLogPen,BOOL blFill,COLORREF fillcolor,int vId/*=0*/,CHMLayer* pLayer/*=NULL*/)
	:CTyBase(vId,pLayer)
{
	ASSERT(pLogPen != NULL);
	m_x0 = x0;
	m_y0 = y0;
	m_x1 = x1;
	m_y1 = y1;
	m_logpen = *pLogPen;
	m_bFill = blFill;
	m_FillColor = fillcolor;
	m_rtType = rtRect;
}

CRectangle::~CRectangle()
{

}

void CRectangle::Set_Line_Width(int width, CElecMapView *pView)
{
	if (m_logpen.lopnWidth.x == width)
		return;	

	ASSERT(pView != NULL);
	if (pView == NULL)
		return;

	pView->InvalidateTy(this);
	m_logpen.lopnWidth.x = width;
	SetModifiedFlag();
	pView->InvalidateTy(this);
}

void CRectangle::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView != NULL) 
		pView->InvalidateTy(this);

	m_x0 += xLen;
	m_x1 += xLen;
	m_y0 += yLen;
	m_y1 += yLen;

	if (pView != NULL) 
		pView->InvalidateTy(this);
}

void CRectangle::Set_Ty_Is_Fill(BOOL bFill, CElecMapView *pView)
{
	if (m_bFill == bFill) 
		return;

	m_bFill = bFill;

	SetModifiedFlag();

	ASSERT(pView != NULL);
	if (pView == NULL) 
		return;

	pView->InvalidateTy(this);
}


void CRectangle::Set_Ty_Fill_Color(COLORREF color, CElecMapView *pView)
{
  if (m_FillColor == color) 
	  return;

	m_FillColor = color;
	SetModifiedFlag();

	ASSERT(pView != NULL);
	if (pView == NULL)
		return;

	pView->InvalidateTy(this);
}

void CRectangle::Set_Ty_Color(COLORREF color, CElecMapView *pView)
{
	if (m_logpen.lopnColor == color) 
		return;

	m_logpen.lopnColor = color;
	SetModifiedFlag();

	ASSERT(pView != NULL);
	if (pView == NULL) 
		return;

	pView->InvalidateTy(this);
}

void CRectangle::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC != NULL);
	ASSERT(pView != NULL);

	if(m_bDelete)	//如果已经处于删除状态
		return;

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy)) 
		return;

	CPen pen(m_logpen.lopnStyle,m_logpen.lopnWidth.x,m_logpen.lopnColor);
	CPen * pOldPen = pDC->SelectObject(&pen);
	int holdDrawMode = pDC->SetROP2(R2_COPYPEN);

	GetRect(&minx,&miny,&maxx,&maxy);
	CPoint pt1,pt2;
	pt1 = pView->UPtoLP(minx,miny);
	pt2 = pView->UPtoLP(maxx,maxy);

	CRect rt;
	rt.SetRect(pt1,pt2);
	rt.NormalizeRect();

	CBrush brush,*pOldBrush;
	LOGBRUSH logbrush;

	if (!m_bFill)
		logbrush.lbStyle = BS_NULL;
	else 
	{
		logbrush.lbStyle  = BS_SOLID;
		logbrush.lbColor = m_FillColor;
	}

	brush.CreateBrushIndirect(&logbrush);
	pOldBrush = pDC->SelectObject(&brush);

	switch (m_rtType)
	{
	case rtRect:
		pDC->Rectangle(&rt);
		break;
  case rtRoundRect:
		pDC->RoundRect(&rt,CPoint(20,20));
		break;
  case rtEllipse:
		pDC->Ellipse(&rt);
		break;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SetROP2(holdDrawMode);
	pDC->SelectObject(pOldPen);
}

void CRectangle::DrawRect(CDC *pDC, int orgX,int orgY,float offsetX, float offsetY, float scaleX, float scaleY, COLORREF ReplaceColor, COLORREF ToColor, BOOL bReplace)
{
	COLORREF color;

	if(bReplace&&(m_logpen.lopnColor == ReplaceColor))
		color = ToColor;
	else 	
		color = m_logpen.lopnColor;

	CPen pen(m_logpen.lopnStyle,m_logpen.lopnWidth.x,color);
	CPen * pOldPen = pDC->SelectObject(&pen);
	int holdDrawMode = pDC->SetROP2(R2_COPYPEN);

	float minx,miny,maxx,maxy;
	GetRect(&minx,&miny,&maxx,&maxy);
	
	CPoint pt1,pt2;
	pt1.x = orgX+(int)((minx-offsetX)*scaleX);
	pt1.y = orgY+(int)((miny-offsetY)*scaleY);
	pt2.x = orgX+(int)((maxx-offsetX)*scaleX);
	pt2.y = orgY+(int)((maxy-offsetY)*scaleY);

	CRect rt;
	rt.SetRect(pt1,pt2);
	rt.NormalizeRect();

	CBrush brush,*pOldBrush;
	LOGBRUSH logbrush;

	if (!m_bFill)
		logbrush.lbStyle = BS_NULL;
	else 
	{
		logbrush.lbStyle = BS_SOLID;

		if(bReplace&&(m_FillColor == ReplaceColor))
			color = ToColor;
		else 	
			color = m_FillColor;

		logbrush.lbColor=color;
	}

	brush.CreateBrushIndirect(&logbrush);
	pOldBrush = pDC->SelectObject(&brush);

	switch (m_rtType)
	{
	case rtRect:
		pDC->Rectangle(&rt);
		break;
  case rtRoundRect:
		pDC->RoundRect(&rt,CPoint(20,20));
		break;
  case rtEllipse:
		pDC->Ellipse(&rt);
		break;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SetROP2(holdDrawMode);
	pDC->SelectObject(pOldPen);
}

void CRectangle::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	*minX  = __min(m_x0,m_x1);
	*minY  = __min(m_y0,m_y1);
	*maxX = __max(m_x0,m_x1);
	*maxY = __max(m_y0,m_y1);
}

CTyBase * CRectangle::Clone()
{
	ASSERT_VALID(this);

	CRectangle* pClone = new CRectangle(m_x0,m_y0,m_x1,m_y1,&m_logpen,m_bFill,m_FillColor,m_rtType);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

BOOL CRectangle::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

BOOL CRectangle::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange >= 0);

	if (m_bDelete)
		return FALSE; 

	BOOL ret =FALSE;
	RectStruct rt,rt2,rt3;
	GetRect(&rt.x0,&rt.y0,&rt.x1,&rt.y1);
	
	if (m_bFill)
	{
		rt2 = rt;
		rt2.InflateRect(errRange,errRange);

		switch (m_rtType)
		{
		case rtRect:
		case rtRoundRect:
			 ret = PointInRect(x,y,rt2);
			 break;
		case rtEllipse:
			ret = PointInEllipse(x,y,rt2);
			break;
		default:
			ASSERT(FALSE);
		}
	}
	else 
	{
		switch (m_rtType)
		{
		case rtRect:
		case rtRoundRect:
			{
				rt2 = rt;
				rt2.InflateRect(errRange,errRange);
				rt3 = rt;
				rt3.InflateRect(-errRange,-errRange);

				if (PointInRect(x,y,rt2)&&(!PointInRect(x,y,rt3)))
					ret=TRUE;
				else 
					ret=FALSE;
			}
			break;
		case rtEllipse:
			{
				rt2 = rt;
				rt2.InflateRect(errRange,errRange);
				rt3 = rt;
				rt3.InflateRect(-errRange,-errRange);

				if (PointInEllipse(x,y,rt2)&&(!PointInEllipse(x,y,rt3)))
					ret=TRUE;
				else
					ret=FALSE;
			}
			break;
		default:
			ASSERT(FALSE);
		}
	}
	return ret;
}

int CRectangle::GetHandleCount()
{
	return 8;
}

PointStruct CRectangle::GetHandle(int nHandle)
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


HCURSOR CRectangle::GetHandleCursor(int nHandle)
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


void CRectangle::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView != NULL);

	pView->InvalidateTy(this);

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);
    break;

	case 1:
		if ((point.x >= m_x1)||(point.y >= m_y1))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1) > fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;
				if (point.x < m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;

				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}

		}
		else 
		{
			m_x0 = point.x;
			m_y0 = point.y;
		}
    break;

	case 2:
		if (point.y >= m_y1)
		  break;

		m_y0 = point.y;
		break;

	case 3:
		if ((point.x <= m_x0)||(point.y >= m_y1))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;
				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;
				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x1 = point.x;
			m_y0 = point.y;
		}
		break;

	case 4:
		if (point.x <= m_x0)
		  break;

		m_x1 = point.x;
		break;

	case 5:
		if ((point.x <= m_x0)||(point.y <= m_y0))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;
				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;
				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x1 = point.x;
			m_y1 = point.y;
		}
		break;

	case 6:
		if (point.y <= m_y0)
		  break;

		m_y1 = point.y;
		break;

	case 7:
		if ((point.x >= m_x1)||(point.y <= m_y0))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;
				if (point.x < m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;
				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x0 = point.x;
			m_y1 = point.y;
		}
		break;

	case 8:
		if (point.x >= m_x1)
		  break;

		m_x0 = point.x;
		break;
	}

	pView->InvalidateTy(this);

}


void CRectangle::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);

	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_x0,sizeof(m_x0));
		file->Write((unsigned char *)&m_y0,sizeof(m_y0));
		file->Write((unsigned char *)&m_x1,sizeof(m_x1));
		file->Write((unsigned char *)&m_y1,sizeof(m_y1));

		file->Write((unsigned char *)&m_logpen.lopnStyle,sizeof(m_logpen.lopnStyle));
		file->Write((unsigned char *)&m_logpen.lopnColor,sizeof(m_logpen.lopnColor));
		file->Write((unsigned char *)&m_logpen.lopnWidth.x,sizeof(m_logpen.lopnWidth.x));

		file->Write((unsigned char *)&m_bFill,sizeof(m_bFill));
		file->Write((unsigned char *)&m_FillColor,sizeof(m_FillColor));
		file->Write((unsigned char *)&m_rtType,sizeof(m_rtType));
	} 
	else
	{
		file->Read((unsigned char *)&m_x0,sizeof(m_x0));
		file->Read((unsigned char *)&m_y0,sizeof(m_y0));
		file->Read((unsigned char *)&m_x1,sizeof(m_x1));
		file->Read((unsigned char *)&m_y1,sizeof(m_y1));
	
		file->Read((unsigned char *)&m_logpen.lopnStyle,sizeof(m_logpen.lopnStyle));
		file->Read((unsigned char *)&m_logpen.lopnColor,sizeof(m_logpen.lopnColor));
		file->Read((unsigned char *)&m_logpen.lopnWidth.x,sizeof(m_logpen.lopnWidth.x));

		file->Read((unsigned char *)&m_bFill,sizeof(m_bFill));
		file->Read((unsigned char *)&m_FillColor,sizeof(m_FillColor));
		file->Read((unsigned char *)&m_rtType,sizeof(m_rtType));
	}
}

void CRectangle::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if(ar.IsStoring())  
	{
		ar<<m_x0<<m_y0<<m_x1<<m_y1;
		ar.Write(&m_logpen,sizeof(m_logpen));
		ar<<m_bFill<<m_FillColor;
		ar.Write(&m_rtType,sizeof(m_rtType));
	}
	else
	{
		ar>>m_x0>>m_y0>>m_x1>>m_y1;
		ar.Read(&m_logpen,sizeof(m_logpen));
		ar>>m_bFill>>m_FillColor;
		ar.Read(&m_rtType,sizeof(m_rtType));
	}
}

DRAW_TY CRectangle::GetTyType()
{
	DRAW_TY ty;

	switch (m_rtType)
	{
	case rtRect:
		ty = tyRect;
		break;
	case rtRoundRect:
		ty = tyRoundRect;
		break;
	case rtEllipse:
		ty = tyEllipse;
		break;
	}

	return ty;
}

void CRectangle::SetDefaultProp(CElecMapView *pView)
{
	memcpy(&(CTyTool::c_LogPen),&m_logpen,sizeof(m_logpen));
	CRectangleTool::c_bFill = m_bFill;
	CRectangleTool::c_FillColor = m_FillColor;
}


void CRectangle::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("对象..") );
	CLineProp dlg;
	dlg.SetLogPen(&m_logpen);
	sheet.AddPage( (CPropertyPage*)&dlg );
	CFillProp dlgFill;
	dlgFill.SetFill(m_bFill,m_FillColor);
	sheet.AddPage((CPropertyPage*)&dlgFill);

	CPropPositionRect posdlg;

	float x0,y0,x1,y1;
	GetRect(&x0,&y0,&x1,&y1);
	posdlg.SetContent(x0,y0,x1,y1);
	sheet.AddPage( (CPropertyPage*)&posdlg );
	
	if (sheet.DoModal() == IDOK)
	{
		Invalidate();
		dlg.GetLogPen(&m_logpen);
		dlgFill.GetFill(m_bFill,m_FillColor);
		posdlg.GetContent(m_x0,m_y0,m_x1,m_y1);
		SetModifiedFlag();
		Invalidate();
		SetDefaultProp(pView);
		return;
	}
}

BOOL CRectangle::Is_Net_Color_Vector()
{
	BOOL ret=FALSE;

	switch (m_rtType)
	{
	case rtRect:
	case rtRoundRect:
     ret = TRUE;
     break;
	}

	return ret;
}


//////////////////////////////////////////////////////////////////////
//  CPolygon
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CPolygon, CTyBase, 0)

CPolygon::CPolygon(float x0,float y0,float x1,float y1, LOGPEN *pLogPen,BOOL blFill,COLORREF fillcolor,int vId/*=0*/,CHMLayer* pLayer/*=NULL*/)
	:CTyBase(vId,pLayer)
{
  ASSERT(pLogPen != NULL);
	m_x0 = __min(x0,x1);
	m_y0 = __min(y0,y1);
	m_x1 = __max(x0,x1);
	m_y1 = __max(y0,y1);
	m_logpen = *pLogPen;
	m_bFill = blFill;
	m_FillColor = fillcolor;
	m_PolyType = Polygon;
	m_UPoints = new PointStruct[14];
	m_LPoints = new CPoint[14];

	if ((m_UPoints == NULL)||(m_LPoints == NULL))
		AfxThrowMemoryException();

	m_nAllocPoints = 14;
	ZeroMemory(m_UPoints,sizeof(PointStruct)*14);
	m_nPoints = 2;
	m_UPoints[0].x = x0;
	m_UPoints[0].y = y0;
	m_UPoints[1].x = x1;
	m_UPoints[1].y = y1;
	m_scale = -1;
}

CPolygon::~CPolygon()
{
	if (m_nAllocPoints > 0)
	{
		if (m_UPoints != NULL)
		  delete[] m_UPoints;

		if (m_LPoints != NULL)
		  delete[] m_LPoints;
	}

	CTyBase::~CTyBase();
}

void CPolygon::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if ((xLen == 0)&&(yLen == 0)) 
		return;

	if (pView != NULL)
		pView->InvalidateTy(this);

	for (int i = 0;i < m_nPoints;i++)
	{
		m_UPoints[i].x += xLen;
		m_UPoints[i].y += yLen;
	}

	CalaculateBoundsRect();
	CalaculateLPoints(pView);

	if (pView != NULL) 
		pView->InvalidateTy(this);
}

void CPolygon::CalaculateLPoints(CElecMapView *pView)
{
	for (int i = 0;i < m_nPoints;i++)
	{
		m_LPoints[i] = pView->UPtoLP(m_UPoints[i]);
	}

	if (m_scale != pView->getCurrentScale())
		m_scale = pView->getCurrentScale();
}

void CPolygon::CalaculateBoundsRect()
{
	ASSERT(m_nPoints > 0);

	m_x0 = m_UPoints[0].x;
	m_x1 = m_x0;
	m_y0 = m_UPoints[0].y;
	m_y1 = m_y0;

	for (int i = 0;i < m_nPoints;i++)
	{
		m_x0 = __min(m_x0,m_UPoints[i].x);
		m_y0 = __min(m_y0,m_UPoints[i].y);
		m_x1 = __max(m_x1,m_UPoints[i].x);
		m_y1 = __max(m_y1,m_UPoints[i].y);
	}
}

void CPolygon::Set_Line_Width(int width, CElecMapView *pView)
{
	if (m_logpen.lopnWidth.x == width)
		return;	

	ASSERT(pView != NULL);
	if (pView == NULL)
		return;

	pView->InvalidateTy(this);
	m_logpen.lopnWidth.x = width;
	SetModifiedFlag();
	pView->InvalidateTy(this);
}

void CPolygon::Set_Ty_Fill_Color(COLORREF color, CElecMapView *pView)
{
	if (m_FillColor == color)
		return;

	m_FillColor = color;
	SetModifiedFlag();

	ASSERT(pView != NULL);
	if (pView == NULL)
		return;

	pView->InvalidateTy(this);
}

void CPolygon::Set_Ty_Is_Fill(BOOL bFill, CElecMapView *pView)
{
	if (m_bFill == bFill) return;

	m_bFill = bFill;
	SetModifiedFlag();
	ASSERT(pView != NULL);
	if (pView == NULL) 
		return;

	pView->InvalidateTy(this);
}


void CPolygon::Set_Ty_Color(COLORREF color, CElecMapView *pView)
{
	if (m_logpen.lopnColor == color) return;
	m_logpen.lopnColor = color;
	SetModifiedFlag();

	ASSERT(pView != NULL);
	if (pView == NULL)
		return;

	pView->InvalidateTy(this);
}

void CPolygon::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC != NULL);
	ASSERT(pView != NULL);

	if(m_bDelete)	//如果已经处于删除状态
		return;

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy))
		return;

	CPen pen(m_logpen.lopnStyle,m_logpen.lopnWidth.x,m_logpen.lopnColor);
	CPen * pOldPen = pDC->SelectObject(&pen);
	int holdDrawMode = pDC->SetROP2(R2_COPYPEN);

	CBrush brush,*pOldBrush;
	LOGBRUSH logbrush;

	if ((!m_bFill)||(m_PolyType == Polyline))
	{
		logbrush.lbStyle = BS_NULL;
	}
	else 
	{
		logbrush.lbStyle=BS_SOLID;
		logbrush.lbColor=m_FillColor;
	}

	brush.CreateBrushIndirect(&logbrush);
	pOldBrush = pDC->SelectObject(&brush);

    CalaculateLPoints(pView);

	switch (m_PolyType)
	{
	case Polygon:
		pDC->Polygon(m_LPoints,m_nPoints);
		break;
	case Polyline:
		pDC->Polyline(m_LPoints,m_nPoints);
		break;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SetROP2(holdDrawMode);
	pDC->SelectObject(pOldPen);
}

void CPolygon::DrawRect(CDC *pDC,int orgX,int orgY,float offsetX, float offsetY, float scaleX, float scaleY, COLORREF ReplaceColor, COLORREF ToColor, BOOL bReplace)
{
	COLORREF color;

	if(bReplace&&(m_logpen.lopnColor == ReplaceColor))
		color = ToColor;
	else 	
		color = m_logpen.lopnColor;

	CPen pen(m_logpen.lopnStyle,m_logpen.lopnWidth.x,color);
	CPen * pOldPen = pDC->SelectObject(&pen);
	int holdDrawMode = pDC->SetROP2(R2_COPYPEN);

	CBrush brush,*pOldBrush;
	LOGBRUSH logbrush;

	if ((!m_bFill)||(m_PolyType == Polyline))
	{
		logbrush.lbStyle = BS_NULL;
	}
	else 
	{
		logbrush.lbStyle=BS_SOLID;
		
		if(bReplace&&(m_FillColor == ReplaceColor))
			color = ToColor;
		else 	
			color = m_FillColor;

		logbrush.lbColor=color;		
	}

	brush.CreateBrushIndirect(&logbrush);
	pOldBrush = pDC->SelectObject(&brush);

	for (int i = 0;i < m_nPoints;i++)
	{
		m_LPoints[i].x = orgX+(int)((m_UPoints[i].x-offsetX)*scaleX);
		m_LPoints[i].y = orgY+(int)((m_UPoints[i].y-offsetY)*scaleY);
	}

	switch (m_PolyType)
	{
	case Polygon:
		pDC->Polygon(m_LPoints,m_nPoints);
		break;
	case Polyline:
		pDC->Polyline(m_LPoints,m_nPoints);
		break;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SetROP2(holdDrawMode);
	pDC->SelectObject(pOldPen);
}

void CPolygon::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	*minX = __min(m_x0,m_x1);
	*minY = __min(m_y0,m_y1);
	*maxX = __max(m_x0,m_x1);
	*maxY = __max(m_y0,m_y1);
}

CTyBase * CPolygon::Clone()
{
	ASSERT_VALID(this);
	ASSERT(m_bDelete != TRUE);

	CPolygon* pClone = new CPolygon(m_x0,m_y0,m_x1,m_y1,&m_logpen,m_bFill,m_FillColor);
	ASSERT_VALID(pClone);

	pClone->m_PolyType = m_PolyType;
	pClone->m_UPoints = new PointStruct[m_nAllocPoints];
	pClone->m_LPoints = new CPoint[m_nAllocPoints];
	if ((pClone->m_UPoints == NULL)||(pClone->m_LPoints == NULL))
		 AfxThrowMemoryException();

	memcpy(pClone->m_UPoints,m_UPoints,sizeof(PointStruct)*m_nPoints);
	pClone->m_nAllocPoints = m_nAllocPoints;
	pClone->m_nPoints = m_nPoints;
	pClone->CalaculateBoundsRect();

	return (CTyBase *)pClone;
}

BOOL CPolygon::IntersectRect(float x0, float y0, float x1, float y1)
{
 	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

BOOL CPolygon::PointInObj(float x, float y,float errRange)
{
	BOOL ret = FALSE;

	ASSERT(errRange >= 0);

	if (m_bDelete) 
		return ret; 
	
	CRgn rgn;
	int i;
	for (i = 0;i < m_nPoints;i++)
	{
		m_LPoints[i].x = (int)(m_UPoints[i].x);///scale);
		m_LPoints[i].y = (int)(m_UPoints[i].y);///scale);
	}

	rgn.CreatePolygonRgn(m_LPoints,m_nPoints,WINDING/*ALTERNATE*/);

	if ((m_PolyType==Polygon)&&(m_bFill))
	{
		CRgn rgnx0,rgnx1,rgny0,rgny1;

		//向左偏移OFFSETX像素
		rgnx0.CreatePolygonRgn(m_LPoints,m_nPoints,WINDING);
		rgnx0.OffsetRgn(-OFFSETX,0);
		rgnx0.CombineRgn(&rgnx0,&rgn,RGN_OR);
	
		//向右偏移OFFSETX像素
		rgnx1.CreatePolygonRgn(m_LPoints,m_nPoints,WINDING);
		rgnx1.OffsetRgn(OFFSETX,0);
		rgnx1.CombineRgn(&rgnx1,&rgn,RGN_OR);
	
		//向上偏移OFFSETX像素
		rgny0.CreatePolygonRgn(m_LPoints,m_nPoints,WINDING);
		rgny0.OffsetRgn(0,-OFFSETX);
		rgny0.CombineRgn(&rgny0,&rgn,RGN_OR);
	
		//向下偏移OFFSETX像素
		rgny1.CreatePolygonRgn(m_LPoints,m_nPoints,WINDING);
		rgny1.OffsetRgn(0,OFFSETX);
		rgny1.CombineRgn(&rgny1,&rgn,RGN_OR);

		rgn.CombineRgn(&rgnx0,&rgnx1,RGN_OR);
		rgn.CombineRgn(&rgn,&rgny0,RGN_OR);
		rgn.CombineRgn(&rgn,&rgny1,RGN_OR);

		rgnx0.DeleteObject();
		rgnx1.DeleteObject();
		rgny0.DeleteObject();
		rgny1.DeleteObject();
		
		if (rgn.PtInRegion(CPoint((int)x,(int)y)))
			ret = TRUE;

		rgn.DeleteObject();

		return  ret;
	} 

	for (i = 0;i < m_nPoints-1;i++)
	{
		ret = PointInLine(x,y,errRange,m_UPoints[i].x,m_UPoints[i].y,m_UPoints[i+1].x,m_UPoints[i+1].y);
		if (ret)
			break;
	}

	if ((m_PolyType == Polygon)&&(!ret))
		ret = PointInLine(x,y,errRange,m_UPoints[m_nPoints-1].x,m_UPoints[m_nPoints-1].y,m_UPoints[0].x,m_UPoints[0].y);

	rgn.DeleteObject();

	return ret;	
}

BOOL CPolygon::PointInLine(float x, float y, float errRange, float x0, float y0, float x1, float y1)
{
	ASSERT(errRange >= 0);

	if (m_bDelete) 
		return FALSE; 

	if ((x < max(0,__min(x0,x1)-errRange))&&
			(y<max(0,__min(y0,y1)-errRange))&&
			(x>__max(x0,x1)+errRange)&&
			(y>__max(y0,y1)+errRange)) return FALSE;

	float dist = PointLine(x,y,x0,y0,x1,y1);

	if (dist > errRange) 
		return FALSE;
	else 
		return TRUE; 
}

int CPolygon::GetHandleCount()
{
	return m_nPoints;
}

PointStruct CPolygon::GetHandle(int nHandle)
{
	ASSERT_VALID(this);
	ASSERT((nHandle > 0)&&(nHandle <= m_nPoints));
	ASSERT(m_UPoints != NULL);

	return m_UPoints[nHandle-1];
}

HCURSOR CPolygon::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);
	return AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE);
}

void CPolygon::AddPoint(PointStruct point, CElecMapView *pView)
{
	ASSERT_VALID(this);

	RectStruct rt0,rt1;
	GetRect(&rt0.x0,&rt0.y0,&rt0.x1,&rt0.y1);

	if (m_nPoints == m_nAllocPoints)
	{
		PointStruct* newUPoints = new PointStruct[m_nAllocPoints + 10];
		CPoint* newLPoints = new CPoint[m_nAllocPoints + 10];

		if (m_UPoints != NULL)
		{
			memcpy(newUPoints, m_UPoints, sizeof(PointStruct) * m_nAllocPoints);
			delete[] m_UPoints;
		}

		if (m_LPoints != NULL)
			delete[] m_LPoints;

		m_UPoints = newUPoints;
		m_LPoints = newLPoints;
		m_nAllocPoints += 10;
	}

 	pView->InvalidateTy(this);
	m_UPoints[m_nPoints++] = point;
	CalaculateBoundsRect();
	SetModifiedFlag();
 	pView->InvalidateTy(this);

	if (m_pHMLayer != NULL)
	{
		GetRect(&rt1.x0,&rt1.y0,&rt1.x1,&rt1.y1);
		m_pHMLayer->OnPositionChangedTy(this,rt0,rt1);
	}
}

void CPolygon::ClonePoint(int id,CElecMapView *pView)
{
	ASSERT_VALID(this);
	ASSERT((id > 0)&&(id <= m_nPoints));
	ASSERT(m_UPoints!=NULL);

	pView->InvalidateTy(this);
	if (m_nPoints == m_nAllocPoints)
	{
		PointStruct* newUPoints = new PointStruct[m_nAllocPoints + 10];
		CPoint* newLPoints = new CPoint[m_nAllocPoints + 10];

		if (m_UPoints != NULL)
		{
			memcpy(newUPoints, m_UPoints, sizeof(PointStruct) * m_nAllocPoints);
			delete[] m_UPoints;
		}

		if (m_LPoints != NULL)
			delete[] m_LPoints;

		m_UPoints = newUPoints;
		m_LPoints = newLPoints;
		m_nAllocPoints += 10;
	}

	memmove(&m_UPoints[id],&m_UPoints[id-1], sizeof(PointStruct) * (m_nPoints-id+1));
	m_nPoints++;
	m_UPoints[id].x = (m_UPoints[(id-1+m_nPoints)%m_nPoints].x+
									m_UPoints[(id+1+m_nPoints)%m_nPoints].x)/2;
	m_UPoints[id].y = (m_UPoints[(id-1+m_nPoints)%m_nPoints].y+
									m_UPoints[(id+1+m_nPoints)%m_nPoints].y)/2;

	CalaculateBoundsRect();
	SetModifiedFlag();
	pView->InvalidateTy(this);

}

void CPolygon::DeletePoint(int id,CElecMapView *pView)
{
	ASSERT_VALID(this);
	ASSERT((id > 0)&&(id <= m_nPoints));
	ASSERT(m_UPoints != NULL);
	if (m_nPoints == 3) 
		return;
  
	RectStruct rt0,rt1;
	
	GetRect(&rt0.x0,&rt0.y0,&rt0.x1,&rt0.y1);

	pView->InvalidateTy(this);
	memcpy(&m_UPoints[id-1],&m_UPoints[id], sizeof(PointStruct) * (m_nPoints-id));
	m_nPoints--;
	
	CalaculateBoundsRect();
	SetModifiedFlag();
	pView->InvalidateTy(this);

	if (m_pHMLayer != NULL)
	{
		GetRect(&rt1.x0,&rt1.y0,&rt1.x1,&rt1.y1);
		m_pHMLayer->OnPositionChangedTy(this,rt0,rt1);
	}
}

void CPolygon::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView != NULL);
	ASSERT((nHandle > 0)&&(nHandle <= m_nPoints));
	ASSERT(m_UPoints != NULL);

	pView->InvalidateTy(this);

	m_UPoints[nHandle-1].x = point.x;
	m_UPoints[nHandle-1].y = point.y;

	CalaculateBoundsRect();
	CalaculateLPoints(pView);

	pView->InvalidateTy(this);
	pView->UpdateWindow();
}

void CPolygon::Ty_Save(CFile *file, BOOL Yn)
{
	int i;

	ASSERT_VALID(this);
	
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_PolyType,sizeof(m_PolyType));
		file->Write((unsigned char *)&m_logpen.lopnStyle,sizeof(m_logpen.lopnStyle));
		file->Write((unsigned char *)&m_logpen.lopnColor,sizeof(m_logpen.lopnColor));
		file->Write((unsigned char *)&m_logpen.lopnWidth.x,sizeof(m_logpen.lopnWidth.x));

		file->Write((unsigned char *)&m_nPoints,sizeof(m_nPoints));
		ASSERT(m_nPoints >= 3);

		for (i = 0;i < m_nPoints;i++)
		{
		  file->Write((unsigned char *)&m_UPoints[i].x,sizeof(float));
		  file->Write((unsigned char *)&m_UPoints[i].y,sizeof(float));
		}

		file->Write((unsigned char *)&m_bFill,sizeof(m_bFill));
		file->Write((unsigned char *)&m_FillColor,sizeof(m_FillColor));
	} 
	else
	{
		file->Read((unsigned char *)&m_PolyType,sizeof(m_PolyType));
		file->Read((unsigned char *)&m_logpen.lopnStyle,sizeof(m_logpen.lopnStyle));
		file->Read((unsigned char *)&m_logpen.lopnColor,sizeof(m_logpen.lopnColor));
		file->Read((unsigned char *)&m_logpen.lopnWidth.x,sizeof(m_logpen.lopnWidth.x));

		file->Read((unsigned char *)&m_nPoints,sizeof(m_nPoints));

		m_nAllocPoints = m_nPoints;
		if (m_nPoints > 0)
		{
			m_UPoints = new PointStruct[m_nAllocPoints];
			m_LPoints = new CPoint[m_nAllocPoints];

			if((m_UPoints == NULL)||(m_LPoints == NULL))
				 AfxThrowMemoryException();

			ZeroMemory(m_UPoints,sizeof(PointStruct)*m_nAllocPoints);
			ZeroMemory(m_LPoints,sizeof(CPoint)*m_nAllocPoints);

			for (i = 0;i < m_nPoints;i++)
			{
				file->Read((unsigned char *)&m_UPoints[i].x,sizeof(float));
				file->Read((unsigned char *)&m_UPoints[i].y,sizeof(float));
			}
		} 

		//错误处理
		if (m_nPoints <= 2)
		{
			 m_nPoints = 3;
			 m_nAllocPoints = 3;

			 m_UPoints = new PointStruct[m_nAllocPoints];
			 m_LPoints = new CPoint[m_nAllocPoints];

			 if((m_UPoints == NULL)||(m_LPoints == NULL))
			  	 AfxThrowMemoryException();

			 ZeroMemory(m_UPoints,sizeof(PointStruct)*m_nAllocPoints);
			 ZeroMemory(m_LPoints,sizeof(CPoint)*m_nAllocPoints);
		}

		file->Read((unsigned char *)&m_bFill,sizeof(m_bFill));
		file->Read((unsigned char *)&m_FillColor,sizeof(m_FillColor));

		CalaculateBoundsRect();
	}
}

void CPolygon::Ty_Serialize(CArchive &ar)
{
	int i;

	ASSERT_VALID(this);

	if(ar.IsStoring())  
	{
		ar.Write((unsigned char *)&m_PolyType,sizeof(m_PolyType));
		ar.Write(&m_logpen,sizeof(m_logpen));

		ar<<m_nPoints;
		for (i = 0;i < m_nPoints;i++)
		  ar<<m_UPoints[i].x<<m_UPoints[i].y;

		ar<<m_bFill<<m_FillColor;
	}
	else
	{
		ar.Read((unsigned char *)&m_PolyType,sizeof(m_PolyType));
		ar.Read(&m_logpen,sizeof(m_logpen));
		ar>>m_nPoints;

		m_nAllocPoints = m_nPoints;
		if (m_nPoints > 0)
		{
			m_UPoints = new PointStruct[m_nAllocPoints];
			m_LPoints = new CPoint[m_nAllocPoints];

			if((m_UPoints == NULL)||(m_LPoints == NULL))
				 AfxThrowMemoryException();

			ZeroMemory(m_UPoints,sizeof(PointStruct)*m_nAllocPoints);
			ZeroMemory(m_LPoints,sizeof(CPoint)*m_nAllocPoints);

			for (i = 0;i < m_nPoints;i++)
				ar>>m_UPoints[i].x>>m_UPoints[i].y;
		}

		ar>>m_bFill>>m_FillColor;

		CalaculateBoundsRect();
	}
}

DRAW_TY CPolygon::GetTyType()
{
	DRAW_TY ty;
	switch (m_PolyType)
	{
	case Polygon:
		ty = tyDBX;
		break;
	case Polyline:
		ty = tyZX;
		break;
	}

	return ty;
}

void CPolygon::SetDefaultProp(CElecMapView *pView)
{
	memcpy(&(CTyTool::c_LogPen),&m_logpen,sizeof(m_logpen));

	if (m_PolyType == Polyline) 
		return;

	CPolygonTool::c_bFill = m_bFill;
	CPolygonTool::c_FillColor = m_FillColor;
}


void CPolygon::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("对象..") );
	CLineProp dlg;
	CFillProp dlgFill;

	dlg.SetLogPen(&m_logpen);
	sheet.AddPage( (CPropertyPage*)&dlg );

	if (m_PolyType == Polygon)
	{
		dlgFill.SetFill(m_bFill,m_FillColor);
		sheet.AddPage((CPropertyPage*)&dlgFill);
	}

	if (sheet.DoModal() == IDOK)
	{
		dlg.GetLogPen(&m_logpen);

		if (m_PolyType == Polygon)
			dlgFill.GetFill(m_bFill,m_FillColor);

		SetModifiedFlag();
		Invalidate();
		SetDefaultProp(pView);

		return;
	}
}


BOOL CPolygon::Is_Net_Color_Vector()
{
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
//  CText
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CText, CTyBase, 0)

CText::CText(float startX0,float startY0,float txtwidth,float txtheight,BOOL  fx,
				float jqwidth, char TextFont[],COLORREF textcolor,
				CString Text,int vId/*=0*/,CHMLayer* pLayer/*=NULL*/)
	:CTyBase(vId,pLayer)
{
	m_StartX = startX0;
	m_StartY = startY0;
	strcpy(m_TextFont,TextFont);
	m_TextHeight = txtheight;
	m_TextWidth = txtwidth;
	m_TextType = txtStatic;
	m_JqWidth = jqwidth;
	m_FangXiang = fx;
	m_Text = Text;
	m_TextColor = textcolor;
}

CText::~CText()
{

}

void CText::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView != NULL) 
		pView->InvalidateTy(this);

	m_StartX += xLen;
	m_StartY += yLen;

	if (pView!=NULL) 
		pView->InvalidateTy(this);
}

void CText::Set_Ty_Color(COLORREF color, CElecMapView *pView)
{
  if (m_TextColor == color)
	  return;

	m_TextColor = color;
	SetModifiedFlag();

	ASSERT(pView != NULL);
	if (pView == NULL)
		return;

	pView->InvalidateTy(this);
}

void CText::SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[])
{
	m_TextHeight = height;
	 m_TextWidth = width;
	 m_JqWidth = jqwidth;
	 m_FangXiang = pl;
	 m_TextColor = color;

	 if (strlen(font) < 16)
		 strcpy(m_TextFont,font);

	 Invalidate();
}

void CText::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC != NULL);
	ASSERT(pView != NULL);

	if(m_bDelete)	//如果已经处于删除状态
		return;

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy)) 
		return;

	GetRect(&minx,&miny,&maxx,&maxy);

	CPoint pt1,pt2;
	pt1 = pView->UPtoLP(minx,miny);
	pt2 = pView->UPtoLP(maxx,maxy);

	CRect rt;
	rt.SetRect(pt1,pt2);
	rt.NormalizeRect();

	int high,width;
	high = pView->ULtoLL(m_TextHeight);
	width = pView->ULtoLL(m_TextWidth);

	if((high < 1)||(high > 1024)||(width < 1)||(width > 1024))
		return;                 //字体太小或太大将不显示

	CBrush brush,*pOldBrush;
	LOGBRUSH logbrush;

	CFont font; 
	//创建字模  
	if (font.CreateFont(high,width,0,0,50,0,0,0,GB2312_CHARSET,OUT_TT_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FIXED_PITCH,m_TextFont) == FALSE)
		return;

	logbrush.lbStyle = BS_NULL;
	brush.CreateBrushIndirect(&logbrush);
	pOldBrush = pDC->SelectObject(&brush);
	COLORREF oldTextColor = pDC->SetTextColor(m_TextColor);
	int oldmode = pDC->SetBkMode(TRANSPARENT);

	CFont* pOldFont = pDC->SelectObject(&font);

	CPoint startPoint;
	startPoint = pView->UPtoLP(m_StartX,m_StartY);
	const char * p;
	int len;
	unsigned char c1,c2;
	char bz[4];
	float x1 = m_StartX,y1 = m_StartY;

	p = (const char *)m_Text;
	len = strlen(p);
	
	while (len > 0)
	{
		c1 = *p;
		c2 = *(p+1);
		if((c1 > 127)&&(c2 > 127))  //如果是一个汉字
		{
			strncpy(bz,p,2);//拷贝一个汉字到bz中
			bz[2] = 0x00;
			p = p+2;        //跳过汉字指向下一个字符位置
			pDC->TextOut(startPoint.x,startPoint.y,bz); //在屏幕上写这个汉字
			len = len-2;                  //字符数减2
			
			//下一个字符的显示位置
			if (m_FangXiang) 
				x1 = x1+m_TextWidth*2+m_JqWidth;
			else 
				y1=y1+m_TextHeight+m_JqWidth;

			startPoint = pView->UPtoLP(x1,y1);
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

int CText::GetChineseNum()
{
	int len;
	const char * p;
	unsigned char c1,c2;
	int ret = 0;

	p = (const char *)m_Text;
	len = strlen(p);

	while (len > 0)
	{
		c1 = *p;
		c2 = *(p+1);
		if((c1 > 127)&&(c2 > 127))  
		{
			p = p+2;       
			len = len-2;   
			ret++;
		} 
		else 
		{
			p = p+1;       
			len--;       
		}
	}

	return ret;
}


void CText::SetSize(float width,float height)
{
	int chinesenum = GetChineseNum();
	int len = m_Text.GetLength();

	if (m_FangXiang) 
	{
		m_TextHeight = height;
		m_TextWidth = (width+(chinesenum+1)*m_JqWidth)/len-m_JqWidth;
	} 
	else 
	{
		m_TextWidth = width/2;
		m_TextHeight = (height+m_JqWidth)/(len-chinesenum)-m_JqWidth;
	}
}

void CText::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	int chinesenum = GetChineseNum();
	int len = m_Text.GetLength();

	if (m_FangXiang) 
	{
		*minX = m_StartX;
		*minY = m_StartY;
		*maxX = m_StartX+(float)((m_TextWidth+m_JqWidth)*len-chinesenum*m_JqWidth-m_JqWidth);
		*maxY = m_StartY+m_TextHeight;
	}
	else
	{
		*minX = m_StartX;
		*minY = m_StartY;
		*maxX = m_StartX+m_TextWidth*2;
		*maxY = m_StartY+(float)((len-chinesenum)*(m_TextHeight+m_JqWidth)-m_JqWidth);
	}
}

CTyBase * CText::Clone()
{
	ASSERT_VALID(this);

	CText* pClone = new CText(m_StartX,m_StartY,m_TextWidth,m_TextHeight,
														m_FangXiang,m_JqWidth,m_TextFont,m_TextColor,
														m_Text);

	pClone->m_TextType = txtStatic;
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

BOOL CText::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

BOOL CText::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange >= 0);
	if (m_bDelete) 
		return FALSE; 

	BOOL ret = FALSE;

	RectStruct rt;
	GetRect(&rt.x0,&rt.y0,&rt.x1,&rt.y1);
	rt.InflateRect(errRange,errRange);

	ret = PointInRect(x,y,rt);

	return ret;
}

int CText::GetHandleCount()
{
	return 8;
}

PointStruct CText::GetHandle(int nHandle)
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


HCURSOR CText::GetHandleCursor(int nHandle)
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


void CText::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView != NULL);

	pView->InvalidateTy(this);
  
	float x0,y0,x1,y1;
	
	GetRect(&x0,&y0,&x1,&y1);
	
	float width = (x1-x0);
	float height = (y1-y0);

	float distX = 0,distY = 0;
	
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);
    break;
	case 1:
		{
			distX = m_StartX-point.x;
			distY = m_StartY-point.y;
			if ((distX <= (-width))||(distY <= (-height)))
			{
				distX = 0;
				distY = 0;
				return;
			}

			m_StartX = point.x;
			m_StartY = point.y;
		}
    break;

	case 2:
		{
			distY = m_StartY-point.y;
			if (distY <= (-height))
			{
				distY = 0;
				return;
			}

			m_StartY = point.y;
		}
		break;

	case 3:
		{
			distX = point.x-x1;
			distY = m_StartY-point.y;
			if ((distX <= (-width))||(distY <= (-height)))
			{
				distX = 0;
				distY = 0;
				break;
			}

			m_StartY = point.y;
		}
		break;

	case 4:
		distX = point.x-x1;
		if (distX <= (-width))
		{
			distX = 0;
			break;
		}
		break;

	case 5:
		{
			distX = point.x-x1;
			distY = point.y-y1;
			if ((distX <= (-width))||(distY <= (-height)))
			{
				distX = 0;
				distY = 0;
				break;
			}
		}
		break;

	case 6:
		distY = point.y-y1;
		if (distY <= (-height))
		{
			distY = 0;
			break;
		}
		break;

	case 7:
		{
			distX = m_StartX-point.x;
			distY = point.y-y1;
			if ((distX <= (-width))||(distY <= (-height)))
			{
				distX = 0;
				distY = 0;
				break;
			}
			m_StartX = point.x;
		}
		break;

	case 8:
		{
			distX = m_StartX-point.x;
			if (distX <= (-width))
			{
				distX = 0;
				break;
			}
			m_StartX = point.x;
		}
		break;
	}
	
	height = height+distY;
	width = width+distX;
	if (height < 0) height = -height;
	if (width < 0)  width = -width;
	SetSize(width,height);

	GetRect(&x0,&y0,&x1,&y1);

	m_StartX = x0;
	m_StartY = y0;

	pView->InvalidateTy(this);
	SetDefaultProp(pView);
}


void CText::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);
	
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_StartX,sizeof(m_StartX));
		file->Write((unsigned char *)&m_StartY,sizeof(m_StartY));
		file->Write((unsigned char *)&m_TextWidth,sizeof(m_TextWidth));
		file->Write((unsigned char *)&m_TextHeight,sizeof(m_TextHeight));

		file->Write((unsigned char *)&m_FangXiang,sizeof(m_FangXiang));
		file->Write((unsigned char *)&m_JqWidth,sizeof(m_JqWidth));
		file->Write((unsigned char *)&m_TextColor,sizeof(m_TextColor));
		file->Write((unsigned char *)&m_TextFont,sizeof(char)*16);
		file->Write((unsigned char *)&m_TextType,sizeof(m_TextType));

		int len = m_Text.GetLength();
		file->Write((unsigned char *)&len,sizeof(len));
		file->Write((LPCSTR)m_Text,sizeof(char)*len);
	} 
	else
	{
		file->Read((unsigned char *)&m_StartX,sizeof(m_StartX));
		file->Read((unsigned char *)&m_StartY,sizeof(m_StartY));
		file->Read((unsigned char *)&m_TextWidth,sizeof(m_TextWidth));
		file->Read((unsigned char *)&m_TextHeight,sizeof(m_TextHeight));

		file->Read((unsigned char *)&m_FangXiang,sizeof(m_FangXiang));
		file->Read((unsigned char *)&m_JqWidth,sizeof(m_JqWidth));
		file->Read((unsigned char *)&m_TextColor,sizeof(m_TextColor));
		file->Read((unsigned char *)&m_TextFont,sizeof(char)*16);
		file->Read((unsigned char *)&m_TextType,sizeof(m_TextType));
	
		int len;
		char * pBuf;
		file->Read((unsigned char *)&len,sizeof(len));
		
		if (len > 0)
		{
			pBuf = new char[len+1];
			ASSERT(pBuf != NULL);
			if (pBuf == NULL)
				AfxThrowMemoryException( );
   
			ZeroMemory(pBuf,len+1);
			file->Read(pBuf,sizeof(char)*len);
			m_Text = pBuf;
			delete [] pBuf;
		}
		else
			m_Text.Empty();
	}
}

void CText::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if(ar.IsStoring())  
	{
		ar<<m_StartX<<m_StartY<<m_TextWidth<<m_TextHeight<<m_TextColor;
		ar.Write((unsigned char *)m_TextFont,sizeof(char)*16);
		ar<<m_JqWidth<<m_FangXiang;
		ar.Write((unsigned char *)&m_TextType,sizeof(m_TextType));
		ar<<m_Text;
	}
	else
	{
		ar>>m_StartX>>m_StartY>>m_TextWidth>>m_TextHeight>>m_TextColor;
		ar.Read((unsigned char *)m_TextFont,sizeof(char)*16);
		ar>>m_JqWidth>>m_FangXiang;
		ar.Read((unsigned char *)&m_TextType,sizeof(m_TextType));
		ar>>m_Text;
	}
}

DRAW_TY CText::GetTyType()
{
	DRAW_TY ty;
	switch (m_TextType)
	{
	case txtStatic:
		ty = tyText;
		break;
	}
	return ty;
}

void CText::SetDefaultProp(CElecMapView *pView)
{
	CTextTool::c_TextHeight = pView->ULtoLL(m_TextHeight);
	CTextTool::c_TextWidth = pView->ULtoLL(m_TextWidth);
	CTextTool::c_JqWidth = pView->ULtoLL(m_JqWidth);
	CTextTool::c_FangXiang = m_FangXiang;
	CTextTool::c_TextColor = m_TextColor;
	strcpy(CTextTool::c_TextFont,m_TextFont);
	CTextTool::c_Text = m_Text;
}

void CText::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("对象..") );
	CInputText dlgtxt;
	dlgtxt.SetText(m_Text);
	sheet.AddPage((CPropertyPage*)&dlgtxt);
	CFontProp dlgfont;
	dlgfont.SetFontProp(m_TextHeight,m_TextWidth,m_JqWidth,m_FangXiang,m_TextColor,m_TextFont);
	sheet.AddPage((CPropertyPage*)&dlgfont);

	CPropPositionText posdlg;
	posdlg.SetContent(m_StartX,m_StartY);
	sheet.AddPage((CPropertyPage*)&posdlg);
	
 	if (sheet.DoModal() == IDOK)
	{
		Invalidate();
		dlgtxt.GetText(m_Text);
		if (strlen(m_Text) <= 0) m_Text = " ";
		dlgfont.GetFontProp(m_TextHeight,m_TextWidth,m_JqWidth,m_FangXiang,m_TextColor,m_TextFont);
		posdlg.GetContent(m_StartX,m_StartY);
		SetModifiedFlag();
		Invalidate();
		SetDefaultProp(pView);
		return;
	}
}

void CText::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	if (GetStr_From_ReplacRule(m_Text,str,ruleList)>0)
	{
		m_Text = str;
		SetModifiedFlag();
	}
}


//////////////////////////////////////////////////////////////////////
//  CTyBmp
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CTyBmp, CTyBase, 0)

CTyBmp::CTyBmp(float x0,float y0,float x1,float y1,
							 LPCSTR bmpname,BOOL blTranslate,
							 COLORREF translatecolor,BOOL blHScale,
							 BOOL blVScale,int vId/*=0*/,CHMLayer* pLayer/*=NULL*/)
	:CTyBase(vId,pLayer)
{
	m_x0 = x0;
	m_y0 = y0;
	m_x1 = x1;
	m_y1 = y1;
	m_bTranslate = blTranslate;
	m_TranslateColor = translatecolor;
	m_blHScale = blHScale;
	m_blVScale = blVScale;					
	if (bmpname != NULL)
	{
	  strcpy(m_bmpName,bmpname);
	  RegisterImage(m_bmpName);
	}
	else 
		ZeroMemory(m_bmpName,sizeof(char)*33);
}

CTyBmp::~CTyBmp()
{
	UnRegisterImage(m_bmpName);
}

void CTyBmp::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView != NULL) 
		pView->InvalidateTy(this);

	m_x0 += xLen;
	m_x1 += xLen;
	m_y0 += yLen;
	m_y1 += yLen;

	if (pView!=NULL) 
		pView->InvalidateTy(this);
}

void CTyBmp::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC != NULL);
	ASSERT(pView != NULL);

	if(m_bDelete)	//如果已经处于删除状态
		return;

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy))
		return;

	GetRect(&minx,&miny,&maxx,&maxy);

	CPoint pt1,pt2;
	pt1 = pView->UPtoLP(minx,miny);
	pt2 = pView->UPtoLP(maxx,maxy);

	CRect rt;
	rt.SetRect(pt1,pt2);
	rt.NormalizeRect();

	if ((rt.Width() < 1)||(rt.Height() < 1))
		return;

	DrawImage(m_bmpName,pDC,pt1.x,pt1.y,rt.Width(),rt.Height(),m_bTranslate,m_TranslateColor,m_blHScale,m_blVScale);
}

void CTyBmp::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	*minX  = __min(m_x0,m_x1);
	*minY  = __min(m_y0,m_y1);
	*maxX = __max(m_x0,m_x1);
	*maxY = __max(m_y0,m_y1);
}

CTyBase * CTyBmp::Clone()
{
	ASSERT_VALID(this);

	CTyBmp* pClone = new CTyBmp(m_x0,m_y0,m_x1,m_y1,m_bmpName,m_bTranslate,m_TranslateColor,m_blHScale,m_blVScale);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

BOOL CTyBmp::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

BOOL CTyBmp::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange >= 0);

	if (m_bDelete) 
		return FALSE; 

	BOOL ret = FALSE;

	RectStruct rt,rt2;
	GetRect(&rt.x0,&rt.y0,&rt.x1,&rt.y1);

	rt2 = rt;
	rt2.InflateRect(errRange,errRange);

	ret = PointInRect(x,y,rt2);

	return ret;
}

int CTyBmp::GetHandleCount()
{
	return 8;
}

PointStruct CTyBmp::GetHandle(int nHandle)
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


HCURSOR CTyBmp::GetHandleCursor(int nHandle)
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


void CTyBmp::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView != NULL);

	pView->InvalidateTy(this);

	
  switch (nHandle)
	{
	default:
		ASSERT(FALSE);
    break;
	case 1:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1)>fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;
				if (point.x < m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;
				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x0 = point.x;
			m_y0 = point.y;
		}
    break;

	case 2:
		m_y0 = point.y;
		break;

	case 3:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;
				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;
				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x1 = point.x;
			m_y0 = point.y;
		}
		break;

	case 4:
		m_x1 = point.x;
		break;

	case 5:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;
				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;
				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x1 = point.x;
			m_y1 = point.y;
		}
		break;

	case 6:
		m_y1 = point.y;
		break;

	case 7:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;
				if (point.x < m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;
				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x0 = point.x;
			m_y1 = point.y;
		}
		break;

	case 8:
		m_x0 = point.x;
		break;
	}

	pView->InvalidateTy(this);

	SetDefaultProp(pView);
}


void CTyBmp::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);

	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_x0,sizeof(m_x0));
		file->Write((unsigned char *)&m_y0,sizeof(m_y0));
		file->Write((unsigned char *)&m_x1,sizeof(m_x1));
		file->Write((unsigned char *)&m_y1,sizeof(m_y1));

		file->Write((unsigned char *)m_bmpName,sizeof(char)*33);
		file->Write((unsigned char *)&m_bTranslate,sizeof(m_bTranslate));
		file->Write((unsigned char *)&m_TranslateColor,sizeof(m_TranslateColor));
		file->Write((unsigned char *)&m_blHScale,sizeof(m_blHScale));
		file->Write((unsigned char *)&m_blVScale,sizeof(m_blVScale));
	} 
	else
	{
		file->Read((unsigned char *)&m_x0,sizeof(m_x0));
		file->Read((unsigned char *)&m_y0,sizeof(m_y0));
		file->Read((unsigned char *)&m_x1,sizeof(m_x1));
		file->Read((unsigned char *)&m_y1,sizeof(m_y1));
	
		file->Read((unsigned char *)m_bmpName,sizeof(char)*33);
		file->Read((unsigned char *)&m_bTranslate,sizeof(m_bTranslate));
		file->Read((unsigned char *)&m_TranslateColor,sizeof(m_TranslateColor));
		file->Read((unsigned char *)&m_blHScale,sizeof(m_blHScale));
		file->Read((unsigned char *)&m_blVScale,sizeof(m_blVScale));
   
		RegisterImage(m_bmpName);
	}
}

void CTyBmp::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if(ar.IsStoring())  
	{
		ar<<m_x0<<m_y0<<m_x1<<m_y1;
		ar.Write(m_bmpName,sizeof(m_bmpName));
		ar<<m_bTranslate<<m_TranslateColor<<m_blHScale<<m_blVScale;
	}
	else
	{
		ar>>m_x0>>m_y0>>m_x1>>m_y1;
		UnRegisterImage(m_bmpName);
		ar.Read(m_bmpName,sizeof(m_bmpName));
		RegisterImage(m_bmpName);
		ar>>m_bTranslate>>m_TranslateColor>>m_blHScale>>m_blVScale;
	}
}

DRAW_TY CTyBmp::GetTyType()
{
	return tyBmp;
}

void CTyBmp::SetDefaultProp(CElecMapView *pView)
{
	ASSERT(pView != NULL);
	if (pView == NULL) 
		return;

	CTyBmpTool::c_Width  = pView->ULtoLL(m_x1-m_x0);
	CTyBmpTool::c_Height = pView->ULtoLL(m_y1-m_y0);
	strcpy(CTyBmpTool::c_bmpName,m_bmpName);
	CTyBmpTool::c_bTranslate = m_bTranslate;
	CTyBmpTool::c_TranslateColor = m_TranslateColor;
	CTyBmpTool::c_blHScale = m_blHScale;
	CTyBmpTool::c_blVScale = m_blVScale;					
}

void CTyBmp::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("对象..") );
 
	CBmpSelectPage dlg;
	dlg.SetContent(m_bmpName,m_bTranslate,m_TranslateColor,m_blHScale,m_blVScale);
	sheet.AddPage( (CPropertyPage*)&dlg );

	CPropPositionRect posdlg;

	float x0,y0,x1,y1;
	GetRect(&x0,&y0,&x1,&y1);
	posdlg.SetContent(x0,y0,x1,y1);
	sheet.AddPage( (CPropertyPage*)&posdlg );

	if (sheet.DoModal() == IDOK)
	{
		Invalidate();
		CString str = dlg.GetContent(m_bTranslate,m_TranslateColor,&m_blHScale,&m_blVScale);
		UnRegisterImage(m_bmpName);
	
		if (str.IsEmpty())
		{
			ZeroMemory(m_bmpName,sizeof(char)*33);
		}
		else
		{
		  strcpy(m_bmpName,str);
		}
		
		RegisterImage(m_bmpName);

		posdlg.GetContent(m_x0,m_y0,m_x1,m_y1);
		SetModifiedFlag();
		Invalidate();
		SetDefaultProp(pView);
	}
}

BOOL CTyBmp::Is_Net_Color_Bmp()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//  CTyVector
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CTyVector, CTyBase, 0)

CTyVector::CTyVector(float x0,float y0,float x1,float y1,
							 LPCSTR vectorname,BOOL blTranslate,
							 COLORREF translatecolor,int vId/*=0*/,CHMLayer* pLayer/*=NULL*/)
	:CTyBase(vId,pLayer)
{
	m_x0 = x0;
	m_y0 = y0;
	m_x1 = x1;
	m_y1 = y1;
	m_bTranslate = blTranslate;
	m_TranslateColor = translatecolor;
	
	if (vectorname != NULL)
		strcpy(m_vectorName,vectorname);
	else 
		ZeroMemory(m_vectorName,sizeof(char)*33);

	RegisterVector(m_vectorName);
}


CTyVector::~CTyVector()
{
	UnRegisterVector(m_vectorName);
}

void CTyVector::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView != NULL) 
		pView->InvalidateTy(this);

	m_x0 += xLen;
	m_x1 += xLen;
	m_y0 += yLen;
	m_y1 += yLen;

	if (pView != NULL) 
		pView->InvalidateTy(this);
}

void CTyVector::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC != NULL);
	ASSERT(pView != NULL);

	if(m_bDelete)	//如果已经处于删除状态
		return;

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy)) 
		return;

	GetRect(&minx,&miny,&maxx,&maxy);

	CPoint pt1,pt2;
	pt1 = pView->UPtoLP(minx,miny);
	pt2 = pView->UPtoLP(maxx,maxy);

	CRect rt;
	rt.SetRect(pt1,pt2);
	rt.NormalizeRect();

	if ((rt.Width() < 1)||(rt.Height() < 1)) 
		return;

	DrawVector(m_vectorName,pDC,rt,rt,m_TranslateColor,m_bTranslate);
}

void CTyVector::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	*minX = __min(m_x0,m_x1);
	*minY = __min(m_y0,m_y1);
	*maxX = __max(m_x0,m_x1);
	*maxY = __max(m_y0,m_y1);
}

CTyBase * CTyVector::Clone()
{
	ASSERT_VALID(this);

	CTyVector* pClone = new CTyVector(m_x0,m_y0,m_x1,m_y1,m_vectorName,m_bTranslate,m_TranslateColor);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

BOOL CTyVector::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

BOOL CTyVector::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange >= 0);

	if (m_bDelete)
		return FALSE; 

	BOOL ret = FALSE;
	RectStruct rt,rt2;
	GetRect(&rt.x0,&rt.y0,&rt.x1,&rt.y1);

	rt2 = rt;
	rt2.InflateRect(errRange,errRange);
	ret = PointInRect(x,y,rt2);

	return ret;
}

int CTyVector::GetHandleCount()
{
	return 8;
}

PointStruct CTyVector::GetHandle(int nHandle)
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


HCURSOR CTyVector::GetHandleCursor(int nHandle)
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


void CTyVector::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView != NULL);

	pView->InvalidateTy(this);

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);
		break;
	case 1:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1) > fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;
				if (point.x < m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;
				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x0 = point.x;
			m_y0 = point.y;
		}
    break;

	case 2:
		m_y0 = point.y;
		break;

	case 3:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;
				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;
				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x1 = point.x;
			m_y0 = point.y;
		}
		break;

	case 4:
		m_x1 = point.x;
		break;

	case 5:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;
				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;
				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x1 = point.x;
			m_y1 = point.y;
		}
		break;

	case 6:
		m_y1=point.y;
		break;

	case 7:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;
				if (point.x < m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;
				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x0 = point.x;
			m_y1 = point.y;
		}
		break;

	case 8:
		m_x0 = point.x;
		break;

	}

	pView->InvalidateTy(this);

	SetDefaultProp(pView);
}


void CTyVector::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);

	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_x0,sizeof(m_x0));
		file->Write((unsigned char *)&m_y0,sizeof(m_y0));
		file->Write((unsigned char *)&m_x1,sizeof(m_x1));
		file->Write((unsigned char *)&m_y1,sizeof(m_y1));

		file->Write((unsigned char *)m_vectorName,sizeof(char)*33);
		file->Write((unsigned char *)&m_bTranslate,sizeof(m_bTranslate));
		file->Write((unsigned char *)&m_TranslateColor,sizeof(m_TranslateColor));
	} 
	else
	{
		file->Read((unsigned char *)&m_x0,sizeof(m_x0));
		file->Read((unsigned char *)&m_y0,sizeof(m_y0));
		file->Read((unsigned char *)&m_x1,sizeof(m_x1));
		file->Read((unsigned char *)&m_y1,sizeof(m_y1));
	
		file->Read((unsigned char *)m_vectorName,sizeof(char)*33);
		file->Read((unsigned char *)&m_bTranslate,sizeof(m_bTranslate));
		file->Read((unsigned char *)&m_TranslateColor,sizeof(m_TranslateColor));
   
		RegisterVector(m_vectorName);
	}
}

void CTyVector::Set_Ty_Color(COLORREF color, CElecMapView *pView)
{
	if (m_TranslateColor == color) 
		return;

	m_TranslateColor = color;

	SetModifiedFlag();

	ASSERT(pView != NULL);
	if (pView == NULL)
		return;

	pView->InvalidateTy(this);
}

void CTyVector::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if(ar.IsStoring())  
	{
		ar<<m_x0<<m_y0<<m_x1<<m_y1;
		ar.Write(m_vectorName,sizeof(m_vectorName));
		ar<<m_bTranslate<<m_TranslateColor;
	}
	else
	{
		UnRegisterVector(m_vectorName);
		ar>>m_x0>>m_y0>>m_x1>>m_y1;
		ar.Read(m_vectorName,sizeof(m_vectorName));
		ar>>m_bTranslate>>m_TranslateColor;
		RegisterVector(m_vectorName);
	}
}

DRAW_TY CTyVector::GetTyType()
{
	return tyVector;
}

void CTyVector::SetDefaultProp(CElecMapView *pView)
{
	ASSERT(pView != NULL);
	if (pView == NULL) 
		return;

	CTyVectorTool::c_Width = pView->ULtoLL(m_x1-m_x0);
	CTyVectorTool::c_Height = pView->ULtoLL(m_y1-m_y0);
	strcpy(CTyVectorTool::c_vectorName,m_vectorName);
	CTyVectorTool::c_bTranslate = m_bTranslate;
	CTyVectorTool::c_TranslateColor = m_TranslateColor;
}

void CTyVector::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("对象..") );
 
	CSelectVectorDlg dlg;
	dlg.SetContent(m_vectorName,m_bTranslate,m_TranslateColor);
	sheet.AddPage( (CPropertyPage*)&dlg );

	CPropPositionRect posdlg;

	float x0,y0,x1,y1;
	GetRect(&x0,&y0,&x1,&y1);
	posdlg.SetContent(x0,y0,x1,y1);
	sheet.AddPage((CPropertyPage*)&posdlg);

	if (sheet.DoModal() == IDOK)
	{
		Invalidate();
		CString str;
		dlg.GetContent(str,m_bTranslate,m_TranslateColor);
		UnRegisterVector(m_vectorName);

		if (str.IsEmpty())
		{
			ZeroMemory(m_vectorName,sizeof(char)*33);
		}
		else
		{
		  strcpy(m_vectorName,str);
		}
		
		RegisterVector(m_vectorName);

		posdlg.GetContent(m_x0,m_y0,m_x1,m_y1);
		SetModifiedFlag();
		Invalidate();
		SetDefaultProp(pView);
	}
}

BOOL CTyVector::Is_Net_Color_Vector()
{
  return TRUE;
}

//////////////////////////////////////////////////////////////////////
//  CMcCurve
// Construction/Destruction
///////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CMcCurve, CTyBase, 0)

CMcCurve::CMcCurve(float x0,float y0,float x1,float y1,int vId,CHMLayer* pLayer)
:CTyBase(vId,pLayer)
{
	m_x0 = x0;
	m_y0 = y0;
	m_x1 = x1;
	m_y1 = y1;
}

CMcCurve::~CMcCurve()
{
	
}

void CMcCurve::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	
	if(ar.IsStoring())  
	{
		ar<<m_x0<<m_y0<<m_x1<<m_y1;
		ar<<mccurvedata.nIprecision;
		ar<<mccurvedata.nVprecision;
		ar<<mccurvedata.fImin;
		ar<<mccurvedata.fImax;
		ar<<mccurvedata.fVmin;
		ar<<mccurvedata.fVmin;
		ar<<mccurvedata.nIprecision;
		ar<<mccurvedata.nVprecision;
		ar<<mccurvedata.nNode;
		ar<<mccurvedata.nLine;
		ar<<mccurvedata.nRtu;
		ar<<mccurvedata.m_realtime_style;
		
		mccurvedata.gtIcurvearray.Serialize(ar);
		mccurvedata.gtVcurvearray.Serialize(ar);
		mccurvedata.zbIcurvearray.Serialize(ar);
		mccurvedata.zbVcurvearray.Serialize(ar);
	}
	else
	{
		ar>>m_x0>>m_y0>>m_x1>>m_y1;
		ar>>mccurvedata.nIprecision;
		ar>>mccurvedata.nVprecision;
		ar>>mccurvedata.fImin;
		ar>>mccurvedata.fImax;
		ar>>mccurvedata.fVmin;
		ar>>mccurvedata.fVmin;
		ar>>mccurvedata.nIprecision;
		ar>>mccurvedata.nVprecision;
		ar>>mccurvedata.nNode;
		ar>>mccurvedata.nLine;
		ar>>mccurvedata.nRtu;
		ar>>mccurvedata.m_realtime_style;
		
		mccurvedata.nNode = 63;
		mccurvedata.nLine = 127;
		mccurvedata.nRtu = 127;
		
		mccurvedata.gtIcurvearray.Serialize(ar);
		mccurvedata.gtVcurvearray.Serialize(ar);
		mccurvedata.zbIcurvearray.Serialize(ar);
		mccurvedata.zbVcurvearray.Serialize(ar);
	}
}


 void CMcCurve::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);
	CString strfilepath;
	char szmcename[12];
	memset(szmcename,0,12);

	if(Yn)	//如果是在进行保存
	{
		int len = mccurvedata.strMcename.GetLength();
		if  (len > 0)
		{
		  strncpy(szmcename,mccurvedata.strMcename,min(11,len));	
		}

		szmcename[11] = 0x00;
		file->Write((float *)&m_x0,sizeof(m_x0));
		file->Write((float *)&m_y0,sizeof(m_y0));
		file->Write((float *)&m_x1,sizeof(m_x1));
		file->Write((float *)&m_y1,sizeof(m_y1));
		
		file->Write((unsigned char *)szmcename,12);
	
		if (::GetServer(strfilepath))
		{
			mccurvedata.SetPath(strfilepath);
			mccurvedata.WriteCurveData();
		}
	}
	else//读取
	{
		file->Read((float *)&m_x0,sizeof(m_x0));
		file->Read((float *)&m_y0,sizeof(m_y0));
		file->Read((float *)&m_x1,sizeof(m_x1));
		file->Read((float *)&m_y1,sizeof(m_y1));
		file->Read((unsigned char *)szmcename,12);
		
		szmcename[11] = 0x00;
		mccurvedata.strMcename = szmcename;

		if (::GetServer(strfilepath))
		{
			mccurvedata.SetPath(strfilepath);
			mccurvedata.ReadCurveData();
		}
	}

}

BOOL GetServer(CString& strfilepath)
{
	char chpath[MAX_PATH];  
	GetModuleFileName(NULL,(LPSTR)chpath,sizeof(chpath));  
	strfilepath.Format("%s",chpath);//= g_szAppPath;
	strfilepath += "\\sunpac\\par\\global\\pic\\McCurveLine.ini";

	return TRUE;
}

void CMcCurve::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView!=NULL);

	pView->InvalidateTy(this);

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);
		break;
	case 1:
		if ((point.x >= m_x1)||(point.y >= m_y1))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1) > fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;
				if (point.x < m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;
				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x0 = point.x;
			m_y0 = point.y;
		}
    break;

	case 2:
		if (point.y >= m_y1)
		  break;

		m_y0 = point.y;
		break;

	case 3:
		if ((point.x <= m_x0)||(point.y >= m_y1))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;
				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;
				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x1 = point.x;
			m_y0 = point.y;
		}
		break;

	case 4:
		if (point.x <= m_x0)
		  break;

		m_x1 = point.x;
		break;

	case 5:
		if ((point.x <= m_x0)||(point.y <= m_y0))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;
				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;
				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x1 = point.x;
			m_y1 = point.y;
		}
		break;

	case 6:
		if (point.y <= m_y0)
		  break;

		m_y1 = point.y;
		break;

	case 7:
		if ((point.x >= m_x1)||(point.y <= m_y0))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;
				if (point.x < m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;
				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x0 = point.x;
			m_y1 = point.y;
		}
		break;

	case 8:
		if (point.x >= m_x1)
		  break;

		m_x0 = point.x;
		break;
	}

	pView->InvalidateTy(this);
}

void CMcCurve::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView != NULL) 
		pView->InvalidateTy(this);

	m_x0 += xLen;
	m_x1 += xLen;
	m_y0 += yLen;
	m_y1 += yLen;

	if (pView != NULL) 
		pView->InvalidateTy(this);
}

HCURSOR CMcCurve::GetHandleCursor(int nHandle)
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

PointStruct CMcCurve::GetHandle(int nHandle)
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

int CMcCurve::GetHandleCount()
{
	return 8;
}

BOOL CMcCurve::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange >= 0);
	if (m_bDelete)
		return FALSE; 

	BOOL ret = FALSE;

	RectStruct rt,rt2;
	GetRect(&rt.x0,&rt.y0,&rt.x1,&rt.y1);
	
	rt2 = rt;
	rt2.InflateRect(errRange,errRange);
	ret = PointInRect(x,y,rt2);

	return ret;
}

BOOL CMcCurve::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

DRAW_TY CMcCurve::GetTyType()
{
	return tymccurve;
}

void CMcCurve::OnEditProperties(CElecMapView * pView)
{
	
	CMcCurveSheet mcsheet("毛刺曲线属性");
	mcsheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	mccurvedata.fleftcoord = m_x0;
	mccurvedata.ftopcoord = m_y0;
	mccurvedata.frightcoord = m_x1;
	mccurvedata.fbottomcoord = m_y1;
	
	mcsheet.pmccurvedata = &mccurvedata;

	mccurvedatabackup.Duplicate(&mccurvedata);

	if (mcsheet.DoModal() == IDOK)
	{
		Invalidate();
		SetModifiedFlag();
		m_x0 = mccurvedata.fleftcoord;
		m_y0 = mccurvedata.ftopcoord;
		m_x1 = mccurvedata.frightcoord;
		m_y1 = mccurvedata.fbottomcoord;
		Invalidate();
	}
	else
	{
		mcsheet.pmccurvedata->Duplicate(&mccurvedatabackup);
	}
}

CTyBase * CMcCurve::Clone()
{
	ASSERT_VALID(this);

	CMcCurve* pClone = new CMcCurve(m_x0,m_y0,m_x1,m_y1);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

void CMcCurve::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	*minX = __min(m_x0,m_x1);
	*minY = __min(m_y0,m_y1);
	*maxX = __max(m_x0,m_x1);
	*maxY = __max(m_y0,m_y1);
}

void CMcCurve::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC != NULL);
	ASSERT(pView != NULL);

	if(m_bDelete)	//如果已经处于删除状态
		return;

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	
	if (!IntersectRect(minx,miny,maxx,maxy))
	{
		return;
	}

	GetRect(&minx,&miny,&maxx,&maxy);
	CPoint pt1,pt2;
	pt1 = pView->UPtoLP(minx,miny);
	pt2 = pView->UPtoLP(maxx,maxy);
	CRect rt;
	rt.SetRect(pt1,pt2);
	rt.NormalizeRect();

    m_TyScale = pView->getCurrentScale();	
	m_TyRect.SetRect(pt1,pt2);
	RecalcRects(m_TyRect);

	DrawTyRect(pDC,pView);                            // 绘制图元区域
	DrawPlotRect(pDC,pView);                          // 绘制绘图区域
	DrawYTicker(pDC,pView);                           // 绘制Y轴坐标
	DrawGrid(pDC,pView);                              // 绘制网格
	DrawHintWords(pDC,pView);                         // 绘制贯通线和自闭线提示文字
	DrawWayButton(pDC,pView);                         // 绘制向前滚动和向后滚动按钮
}

// 计算绘图区大小
void CMcCurve::RecalcRects(CRect rt)
{
	m_TopSpace    = (rt.bottom-rt.top) /20;
	m_BottomSpace = (rt.bottom-rt.top) /5;
	m_LeftSpace   = (rt.right-rt.left) /10;
	m_RightSpace  = (rt.right-rt.left) /20;

	m_PlotRect.left	  = rt.left   + m_LeftSpace;
	m_PlotRect.right  = rt.right  - m_RightSpace;
	m_PlotRect.top	  = rt.top    + m_TopSpace;
	m_PlotRect.bottom = rt.bottom - m_BottomSpace;

	m_PriorButtonRect.left = m_PlotRect.left+7*m_PlotRect.Width()/8;
	m_PriorButtonRect.top  = m_PlotRect.bottom+m_BottomSpace/3;
	m_PriorButtonRect.right= m_PriorButtonRect.left+m_PlotRect.Width()/24;
	m_PriorButtonRect.bottom=m_PriorButtonRect.top+m_BottomSpace/3;
	m_NextButtonRect.left  = m_PlotRect.left+23*m_PlotRect.Width()/24;
	m_NextButtonRect.top   = m_PlotRect.bottom+m_BottomSpace/3;
	m_NextButtonRect.right = m_PlotRect.right;
	m_NextButtonRect.bottom = m_NextButtonRect.top+m_BottomSpace/3;

	m_BackGroundColor = RGB(115,190,198);
	m_SpaceColor = RGB(214,211,206);   
	m_GridColor = RGB(132,130,132);
}

void CMcCurve::DrawTyRect(CDC* pDC, CElecMapView *pView)
{
	CBrush newBrush(m_SpaceColor);	
	CBrush* oldBrush = pDC->SelectObject(&newBrush);
	pDC->Rectangle(m_TyRect);
    pDC->SelectObject(oldBrush);
}

// 绘制绘图区域
void CMcCurve::DrawPlotRect(CDC* pDC, CElecMapView *pView)
{
	if(!mccurvedata.bgtmode)
	{
    	CBrush newBrush(m_BackGroundColor);	
    	CBrush* oldBrush = pDC->SelectObject(&newBrush);
    	pDC->Rectangle(m_PlotRect);
        pDC->SelectObject(oldBrush);
	}
	else
	{
		CRect rt1,rt2;
		CBrush newBrush(m_BackGroundColor);	
    	CBrush* oldBrush = pDC->SelectObject(&newBrush);
		rt1.SetRect(m_PlotRect.left,m_PlotRect.top,m_PlotRect.right,m_PlotRect.top+m_PlotRect.Height()/2-4);
		rt2.SetRect(m_PlotRect.left,m_PlotRect.top+m_PlotRect.Height()/2+4,m_PlotRect.right,m_PlotRect.bottom);		
		pDC->Rectangle(rt1);
		pDC->Rectangle(rt2);
        pDC->SelectObject(oldBrush);
	}
}

// 绘制Y轴坐标
void CMcCurve::DrawYTicker(CDC* pDC, CElecMapView *pView)
{
	int	  xb, yb, xe, ye, i, j, value;          
	char  str[32];

	xe = m_PlotRect.left;
	if(!mccurvedata.bgtmode)
	{
		if(m_PlotRect.Height()/10 > 20)
			 j = 10;
		else if(m_PlotRect.Height()/5 > 20)
			 j = 5;
		else
			j = 1;

    	for(i = 0; i <= j; i ++)     
		{
			yb = ye = (int)(m_PlotRect.top + 1.0 * m_PlotRect.Height() * i / j );
			xb = m_PlotRect.left - m_LeftSpace / 10;
			value = (int)(mccurvedata.fImax - i * (mccurvedata.fImax - mccurvedata.fImin) / j);			
   			sprintf(str,"%d",value);
			SetAlign(RIGHT,CENTER);
			if(i == 0)         // 第一个标示顶部对齐
				PrintString(pDC,m_PlotRect.left - m_LeftSpace / 6, m_PlotRect.top+6, 0, str);
			else if(i == j)    // 最后一个标示底部对齐
				PrintString(pDC,m_PlotRect.left - m_LeftSpace / 6, m_PlotRect.bottom-6, 0, str);
			else             // 其它的标示中间对齐
				PrintString(pDC,m_PlotRect.left - m_LeftSpace / 6, yb, 0, str);

    		if (i > 0 && i < j)
    			DrawLine(pDC,xb, yb, xe, ye, RGB(0,0,0));
		}
	}
	else
	{
		if((m_PlotRect.Height()/2-4)/10 > 20)
			 j = 10;
		else if((m_PlotRect.Height()/2-4)/5 > 20)
			 j = 5;
		else
			j = 1;

    	for(i = 0; i <= j; i ++)     
		{
			yb = ye = (int)(m_PlotRect.top + 1.0 * (m_PlotRect.Height()/2-4) * i / j );
			xb = m_PlotRect.left - m_LeftSpace / 10;
			value = (int)(mccurvedata.fImax - i * (mccurvedata.fImax - mccurvedata.fImin) / j);			
   			sprintf(str,"%d",value);
			SetAlign(RIGHT, CENTER);
			
			if(i == 0)
				PrintString(pDC,m_PlotRect.left - m_LeftSpace / 6, m_PlotRect.top+6, 0, str);
			else if(i == j)
				PrintString(pDC,m_PlotRect.left - m_LeftSpace / 6, m_PlotRect.top+m_PlotRect.Height()/2-10, 0, str);
			else
				PrintString(pDC,m_PlotRect.left - m_LeftSpace / 6, yb, 0, str);

    		if (i > 0 && i < j)
    			DrawLine(pDC,xb, yb, xe, ye, RGB(0,0,0));
		}    

    	for(i = 0; i <= j; i ++)     
		{
			yb = ye = (int)((m_PlotRect.top+m_PlotRect.Height()/2+4) + 1.0 * (m_PlotRect.Height()/2-4) * i / j );
			xb = m_PlotRect.left - m_LeftSpace / 10;
			value = (int)(mccurvedata.fImax - i * (mccurvedata.fImax - mccurvedata.fImin) / j);			
   			sprintf(str,"%d",value);
			SetAlign(RIGHT, CENTER);
			
			if(i == 0)
				PrintString(pDC,m_PlotRect.left - m_LeftSpace / 6, m_PlotRect.top+m_PlotRect.Height()/2+10, 0, str);
			else if(i == j)
				PrintString(pDC,m_PlotRect.left - m_LeftSpace / 6, m_PlotRect.bottom-6, 0, str);
			else
				PrintString(pDC,m_PlotRect.left - m_LeftSpace / 6, yb, 0, str);

    		if (i > 0 && i < j)
    			DrawLine(pDC,xb, yb, xe, ye, RGB(0,0,0));
		}
	}
}

void CMcCurve::DrawGrid(CDC* pDC, CElecMapView *pView)
{
	DrawXGrid(pDC);
	DrawYGrid(pDC);
}

void CMcCurve::DrawYGrid(CDC* pDC)
{
	int	  xb, yb, xe, ye, i, j;          

	xe = m_PlotRect.left;
	
	if(!mccurvedata.bgtmode)
	{
		if(m_PlotRect.Height()/10 > 20)
			 j = 10;
		else if(m_PlotRect.Height()/5 > 20)
			 j = 5;
		else 
			j = 1;

    	for(i = 0; i <= j; i ++)     
		{
			yb = ye = (int)(m_PlotRect.top + 1.0 * m_PlotRect.Height() * i / j );
			xb = m_PlotRect.left;
			xe = m_PlotRect.right;

    		if (i > 0 && i < j)
    			DrawLine(pDC,xb, yb, xe, ye,m_GridColor);
		}
	}
	else
	{
		if((m_PlotRect.Height()/2-4)/10 > 20)
			 j = 10;
		else if((m_PlotRect.Height()/2-4)/5 > 20)
			 j = 5;
		else
			j = 1;

    	for(i = 0; i <= j; i ++)     
		{
			yb = ye = (int)(m_PlotRect.top + 1.0 * (m_PlotRect.Height()/2-4) * i / j );
			xb = m_PlotRect.left;
			xe = m_PlotRect.right;

    		if (i > 0 && i < j)
    			DrawLine(pDC,xb, yb, xe, ye, m_GridColor);
		}    

    	for(i = 0; i <= j; i ++)     
		{
			yb = ye = (int)((m_PlotRect.top+m_PlotRect.Height()/2+4) + 1.0 * (m_PlotRect.Height()/2-4) * i / j );
			xb = m_PlotRect.left ;
			xe = m_PlotRect.right;

    		if (i > 0 && i < j)
    			DrawLine(pDC,xb, yb, xe, ye, m_GridColor);
		}
	}
}

void CMcCurve::DrawXGrid(CDC* pDC)
{
	int i,xb, yb, xe, ye;

	if(!mccurvedata.bgtmode)
	{
    	for(i = 0;i < 12;i++)
    	{
	    	yb = m_PlotRect.top;
    		ye = m_PlotRect.bottom;
	    	xb = xe = (int)(m_PlotRect.left+m_PlotRect.Width()*i/12);
    		
			if(i>0 && i<12)
	    		DrawLine(pDC,xb,yb,xe,ye,m_GridColor);
		}
	}
	else
	{
		for(i=0;i<12;i++)
    	{
	    	yb = m_PlotRect.top;
			ye = m_PlotRect.top+m_PlotRect.Height()/2-4;
	    	xb = xe = (int)(m_PlotRect.left+m_PlotRect.Width()*i/12);
    		
			if(i>0 && i<12)
	    		DrawLine(pDC,xb,yb,xe,ye,m_GridColor);

			yb = m_PlotRect.top+m_PlotRect.Height()/2+4;
			ye = m_PlotRect.bottom;
	    	xb = xe = (int)(m_PlotRect.left+m_PlotRect.Width()*i/12);
    		
			if(i>0 && i<12)
	    		DrawLine(pDC,xb,yb,xe,ye,m_GridColor);
		}
	}
}

// 绘制贯通线和自闭线提示文字
void CMcCurve::DrawHintWords(CDC* pDC, CElecMapView *pView)
{
	if(mccurvedata.bgtmode)
	{
		SetAlign(LEFT,CENTER);
		PrintString(pDC,m_PlotRect.left, m_PlotRect.bottom+m_BottomSpace /2, 0, "电流：实时曲线");
	}
	else
	{
		
	}
}

// 绘制向前滚动和向后滚动按钮
void CMcCurve::DrawWayButton(CDC* pDC, CElecMapView *pView)
{
	CRect rt;		
	CBrush nBrush(m_BackGroundColor);	
	CBrush* noldBrush;
	noldBrush = pDC->SelectObject(&nBrush);

	rt.left   = m_NextButtonRect.left+2;
	rt.top    = m_NextButtonRect.top+2;
	rt.right  = m_NextButtonRect.right-2;
	rt.bottom = m_NextButtonRect.bottom-2;

	CPoint pointarr[3];
	pointarr[0].x = rt.left;
	pointarr[0].y = rt.top;
	pointarr[1].x = rt.left;
	pointarr[1].y = rt.bottom;
	pointarr[2].x = rt.right;
	pointarr[2].y = rt.top+rt.Height()/2;
	pDC->Polygon(pointarr,3);

	rt.left   = m_PriorButtonRect.left+2;
	rt.top    = m_PriorButtonRect.top+2;
	rt.right  = m_PriorButtonRect.right-2;
	rt.bottom = m_PriorButtonRect.bottom-2;

	pointarr[0].x = rt.right;
	pointarr[0].y = rt.top;
	pointarr[1].x = rt.right;
	pointarr[1].y = rt.bottom;
	pointarr[2].x = rt.left;
	pointarr[2].y = rt.top+rt.Height()/2;
	pDC->Polygon(pointarr,3);

    pDC->SelectObject(noldBrush);
}

void CMcCurve::DrawLine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color)
{
	CPen newPen(PS_SOLID,1,color);
	CPen* oldPen = pDC->SelectObject(&newPen);
	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y2);
	pDC->SelectObject(oldPen);
}

void CMcCurve::PrintString(CDC* pDC, int x, int y, int theta, const char* fmt)
{	
	WORD    Height, Width;	
	LOGFONT nLogFont;	
	UINT    PreSet;
	
	nLogFont.lfHeight      = -10;
	nLogFont.lfWeight      = 100;
	nLogFont.lfOrientation = 0;
	nLogFont.lfEscapement  = 0;
	strcpy(nLogFont.lfFaceName,"宋体");

	CFont nFont;
	nFont.CreateFontIndirect(&nLogFont);
	CFont* oldFont=pDC->SelectObject(&nFont);
    pDC->SetTextColor(RGB(0,0,0)); 
	pDC->SelectObject(oldFont);

	int bm  = pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,0)); 
	pDC->SelectObject(oldFont);
	PreSet = pDC->SetTextAlign(TA_LEFT|TA_TOP);

   	SIZE  size=pDC->GetTextExtent(fmt, lstrlen(fmt));
   	Height = (WORD)size.cy;
   	Width  = (WORD)size.cx;
                                             
    if(m_StrAlign.HAlign == LEFT && m_StrAlign.VAlign == TOP)
   {
   } 
   else if(m_StrAlign.HAlign == LEFT && m_StrAlign.VAlign == CENTER)
   {     
    	x = (int)(x);
   		y = (int)(y - Height/2.);
   }
   else if(m_StrAlign.HAlign == CENTER && m_StrAlign.VAlign == TOP)
   {
    	x = (int)(x - Width/2. );
   		y = (int)(y );
   }
   else if(m_StrAlign.HAlign == CENTER && m_StrAlign.VAlign == CENTER)
   {
    	x = (int)(x - Width/2.  );
   		y = (int)(y - Height/2.);
   }
   else if(m_StrAlign.HAlign == CENTER && m_StrAlign.VAlign == BOTTOM)
   {
   		x = (int)(x - Width/2.);
    	y = (int)(y - Height);
   }
   else if(m_StrAlign.HAlign == RIGHT && m_StrAlign.VAlign == TOP)
   {
    	x = (int)(x - Width );
   		y = (int)(y);
   }
   else if(m_StrAlign.HAlign == RIGHT && m_StrAlign.VAlign == CENTER)
   {
    	x = (int)(x - Width);
   		y = (int)(y - Height/2);
   }
   else if(m_StrAlign.HAlign == RIGHT && m_StrAlign.VAlign == BOTTOM)
   {
    	x = (int)(x - Width);
   		y = (int)(y - Height);
   }
   else //if(m_StrAlign.HAlign == LEFT && m_StrAlign.VAlign == BOTTOM)
   {             
    	x = (int)(x);
   		y = (int)(y - Height);
   }                 

	pDC->TextOut(x, y, fmt, lstrlen(fmt));
    pDC->SetTextAlign(PreSet);
	pDC->SetBkMode(bm);	
}

// 设置字符对齐方式
void CMcCurve::SetAlign(int HAlign, int VAlign)
{
	m_StrAlign.HAlign=HAlign;
	m_StrAlign.VAlign=VAlign;
}

void CMcCurve::ParamReplace_Ty(CStringList& ruleList)
{
  WORD srcDot = 0,dstDot;
  BYTE dstNode,dstLine,dstRtu;
  int ret,i;
  CString str;

  if (GetStr_From_ReplacRule(mccurvedata.strMcename,str,ruleList) > 0)
  {
	  mccurvedata.strMcename = str;
	  mccurvedata.stroldsection = mccurvedata.strMcename;
	  SetModifiedFlag();
  } 
  else
  {
	  if (m_pHMLayer != NULL)
	  {
		  if (m_pHMLayer->m_pHuaMian != NULL)
		  {
			   mccurvedata.strMcename = m_pHMLayer->m_pHuaMian->GetHMName();

			   if (mccurvedata.strMcename.GetLength() > 11)
				   mccurvedata.strMcename.Truncate(11);	

			   mccurvedata.stroldsection=mccurvedata.strMcename;
		  }
	  }
  }

  ret = GetDot_From_ReplaceRule(mccurvedata.nNode,mccurvedata.nLine,mccurvedata.nRtu,srcDot, dstNode,dstLine,dstRtu,dstDot,ruleList);
  if (ret > 0)
  {
	 SetModifiedFlag(true);
	 mccurvedata.nNode=dstNode;
	 mccurvedata.nLine=dstLine;
	 mccurvedata.nRtu=dstRtu;
  }

  int count=mccurvedata.gtIcurvearray.GetSize();
  for (i = 0;i < count;i++)
  {
	  ret = GetDot_From_ReplaceRule(mccurvedata.gtIcurvearray[i].nlinenode,mccurvedata.gtIcurvearray[i].nlineline,mccurvedata.gtIcurvearray[i].nlinertu,mccurvedata.gtIcurvearray[i].Bdot, dstNode,dstLine,dstRtu,dstDot,ruleList);
	  if (ret > 0)
	  {
		  SetModifiedFlag(true);
		  mccurvedata.gtIcurvearray[i].nlinenode=dstNode;
		  mccurvedata.gtIcurvearray[i].nlineline=dstLine;
		  mccurvedata.gtIcurvearray[i].nlinertu=dstRtu;
		  mccurvedata.gtIcurvearray[i].Bdot=(BYTE)dstDot;
	  }
  }

  count = mccurvedata.zbIcurvearray.GetSize();
  for (i = 0;i < count;i++)
  {
	  ret = GetDot_From_ReplaceRule(mccurvedata.zbIcurvearray[i].nlinenode,mccurvedata.zbIcurvearray[i].nlineline,mccurvedata.zbIcurvearray[i].nlinertu,mccurvedata.zbIcurvearray[i].Bdot, dstNode,dstLine,dstRtu,dstDot,ruleList);
	  if (ret > 0)
	  {
		  SetModifiedFlag(true);
		  mccurvedata.zbIcurvearray[i].nlinenode=dstNode;
		  mccurvedata.zbIcurvearray[i].nlineline=dstLine;
		  mccurvedata.zbIcurvearray[i].nlinertu=dstRtu;
		  mccurvedata.zbIcurvearray[i].Bdot=(BYTE)dstDot;
	  }
  }

  count = mccurvedata.gtVcurvearray.GetSize();
  for (i = 0;i<count;i++)
  {
	  ret = GetDot_From_ReplaceRule(mccurvedata.gtVcurvearray[i].nlinenode,mccurvedata.gtVcurvearray[i].nlineline,mccurvedata.gtVcurvearray[i].nlinertu,mccurvedata.gtVcurvearray[i].Bdot, dstNode,dstLine,dstRtu,dstDot,ruleList);
	  if (ret > 0)
	  {
		  SetModifiedFlag(true);
		  mccurvedata.gtVcurvearray[i].nlinenode=dstNode;
		  mccurvedata.gtVcurvearray[i].nlineline=dstLine;
		  mccurvedata.gtVcurvearray[i].nlinertu=dstRtu;
		  mccurvedata.gtVcurvearray[i].Bdot=(BYTE)dstDot;
	  }
  }

  count = mccurvedata.zbVcurvearray.GetSize();
  for (i = 0;i < count;i++)
  {
	  ret = GetDot_From_ReplaceRule(mccurvedata.zbVcurvearray[i].nlinenode,mccurvedata.zbVcurvearray[i].nlineline,mccurvedata.zbVcurvearray[i].nlinertu,mccurvedata.zbVcurvearray[i].Bdot, dstNode,dstLine,dstRtu,dstDot,ruleList);
	  if (ret > 0)
	  {
		  SetModifiedFlag(true);
		  mccurvedata.zbVcurvearray[i].nlinenode=dstNode;
		  mccurvedata.zbVcurvearray[i].nlineline=dstLine;
		  mccurvedata.zbVcurvearray[i].nlinertu=dstRtu;
		  mccurvedata.zbVcurvearray[i].Bdot=(BYTE)dstDot;
	  }
  }
}



//////////////////////////////////////////////////////////////////////
//  CYMcCurve
// Construction/Destruction
///////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CYMcCurve, CTyBase, 0)

CYMcCurve::CYMcCurve(float x0,float y0,float x1,float y1,int vId,CHMLayer* pLayer)
:CTyBase(vId,pLayer)
{
	strcpy(ymcname ,"") ;
	m_x0 = x0 ;
	m_y0 = y0 ;
	m_x1 = x1 ;
	m_y1 = y1 ;

	hourinfo.y_min = 0;
	hourinfo.y_max = 0;
	hourinfo.y_scale =0;

	//日曲线
	ymin_day.y_min = 0;
	ymin_day.y_max = 0;
	ymin_day.y_scale = 0;

	//月曲线
	ymin_month.y_min = 0;
	ymin_month.y_max = 0;
	ymin_month.y_scale = 0;

	//年曲线
	ymin_year.y_min = 0;
	ymin_year.y_max = 0;
	ymin_year.y_scale = 0;

	ymcurvescount = 0 ;

	for(int i = 0 ;i < YMMAXCURVE ;i++)
	{
		strcpy(ymcurves[i].ename ,"") ;
		strcpy(ymcurves[i].cname ,"") ;
		ymcurves[i].color = RGB(0,0,0) ;
	}
	
	strcpy(ymc_unit ,"") ;
	m_strname = "缺省" ;
}

CYMcCurve::~CYMcCurve()
{
	
}

void CYMcCurve::OnEditProperties(CElecMapView * pView)
{
	CYMcCurveSheet mcsheet("累加曲线属性");
	mcsheet.m_psh.dwFlags|=PSH_NOAPPLYNOW;

	mcsheet.m_ymccurvepage1.m_strmcename = ymcname ;
	mcsheet.m_ymccurvepage1.m_fLeftcoord = m_x0 ;
	mcsheet.m_ymccurvepage1.m_fTopcoord = m_y0 ;
	mcsheet.m_ymccurvepage1.m_fXlen = m_x1 ;
	mcsheet.m_ymccurvepage1.m_fYlen = m_y1 ;

	//小时曲线	
	mcsheet.m_ymccurvepage1.m_hMin = hourinfo.y_min  ;
	mcsheet.m_ymccurvepage1.m_hMax = hourinfo.y_max  ;
	mcsheet.m_ymccurvepage1.m_hScale = hourinfo.y_scale ;

	//日曲线
	mcsheet.m_ymccurvepage1.m_dMin = ymin_day.y_min  ;
	mcsheet.m_ymccurvepage1.m_dMax = ymin_day.y_max  ;
	mcsheet.m_ymccurvepage1.m_dScale = ymin_day.y_scale ;

	//月曲线
	mcsheet.m_ymccurvepage1.m_mMin = ymin_month.y_min ;
	mcsheet.m_ymccurvepage1.m_mMax = ymin_month.y_max ;
	mcsheet.m_ymccurvepage1.m_mScale= ymin_month.y_scale;

	//年曲线
	mcsheet.m_ymccurvepage1.m_yMin = ymin_year.y_min ;
	mcsheet.m_ymccurvepage1.m_yMax = ymin_year.y_max ;
	mcsheet.m_ymccurvepage1.m_yScale = ymin_year.y_scale;
	
	for(int i = 0 ;i < ymcurvescount ;i++)
	{
		mcsheet.m_ymccurvepage2.curvearray.Add(ymcurves[i])  ;
	}

	mcsheet.m_ymccurvepage1.m_unit = ymc_unit ;

	if (mcsheet.DoModal()==IDOK)
	{
		Invalidate();	
		SetModifiedFlag();
		strcpy(ymcname ,mcsheet.m_ymccurvepage1.m_strmcename) ;
		
		m_strname = ymcname ;

		m_x0 = mcsheet.m_ymccurvepage1.m_fLeftcoord ;
		m_y0 = mcsheet.m_ymccurvepage1.m_fTopcoord ;
		m_x1 = mcsheet.m_ymccurvepage1.m_fXlen ;
		m_y1 = mcsheet.m_ymccurvepage1.m_fYlen ;
	
		//小时曲线	
		hourinfo.y_min = mcsheet.m_ymccurvepage1.m_hMin ;
		hourinfo.y_max = mcsheet.m_ymccurvepage1.m_hMax ;
		hourinfo.y_scale = mcsheet.m_ymccurvepage1.m_hScale ;

		//日曲线
		ymin_day.y_min = mcsheet.m_ymccurvepage1.m_dMin ;
		ymin_day.y_max = mcsheet.m_ymccurvepage1.m_dMax ;
		ymin_day.y_scale = mcsheet.m_ymccurvepage1.m_dScale ;

		//月曲线
		ymin_month.y_min = mcsheet.m_ymccurvepage1.m_mMin ;
		ymin_month.y_max = mcsheet.m_ymccurvepage1.m_mMax ;
		ymin_month.y_scale = mcsheet.m_ymccurvepage1.m_mScale ;

		//年曲线
		ymin_year.y_min = mcsheet.m_ymccurvepage1.m_yMin ;
		ymin_year.y_max = mcsheet.m_ymccurvepage1.m_yMax ;
		ymin_year.y_scale = mcsheet.m_ymccurvepage1.m_yScale ;

		ymcurvescount = mcsheet.m_ymccurvepage2.curvearray.GetCount() ;
		
		for(int i = 0 ;i < ymcurvescount ;i++)
		{
			ymcurves[i] = mcsheet.m_ymccurvepage2.curvearray.GetAt(i) ;
		}
		
		strcpy(ymc_unit,mcsheet.m_ymccurvepage1.m_unit) ;
		Invalidate();
	}
	else
	{

	}
}
DRAW_TY CYMcCurve::GetTyType()
{
	return tyYmccurve;
}

void CYMcCurve::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC != NULL);
	ASSERT(pView != NULL);

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy))
	{
		return;
	}

	GetRect(&minx,&miny,&maxx,&maxy);

	CPoint pt1,pt2;
	pt1 = pView->UPtoLP(minx,miny);
	pt2 = pView->UPtoLP(maxx,maxy);

	CRect rt;
	rt.SetRect(pt1,pt2);
	rt.NormalizeRect();

	m_TyRect = rt ;
	
	RecalcRects(m_TyRect);

	DrawTyRect(pDC,pView);                            // 绘制图元区域
	DrawPlotRect(pDC,pView);                          // 绘制绘图区域
	
	DrawGrid(pDC,pView);                              // 绘制网格
	DrawString(pDC ,pView) ;							//绘制文字
}

void CYMcCurve::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	*minX = __min(m_x0,m_x1);
	*minY = __min(m_y0,m_y1);
	*maxX = __max(m_x0,m_x1);
	*maxY = __max(m_y0,m_y1);
}

void CYMcCurve::DrawTyRect(CDC *pDC ,CElecMapView *pView)
{
	CBrush newBrush(m_SpaceColor);	
	CBrush* oldBrush = pDC->SelectObject(&newBrush);
	pDC->Rectangle(m_TyRect);
    pDC->SelectObject(oldBrush);
}

// 绘制绘图区域
void CYMcCurve::DrawPlotRect(CDC* pDC, CElecMapView *pView)
{
	CBrush newBrush(m_BackGroundColor);	
	CBrush* oldBrush = pDC->SelectObject(&newBrush);
	pDC->Rectangle(m_PlotRect);
	pDC->SelectObject(oldBrush);
}

// 计算绘图区大小
void CYMcCurve::RecalcRects(CRect rt)
{
	m_TopSpace    = (rt.bottom-rt.top) /10;
	m_BottomSpace = (rt.bottom-rt.top) /20;
	m_LeftSpace   = (rt.right-rt.left) /20;
	m_RightSpace  = (rt.right-rt.left) /20;

	m_PlotRect.left	  = rt.left   + m_LeftSpace;
	m_PlotRect.right  = rt.right  - m_RightSpace;
	m_PlotRect.top	  = rt.top    + m_TopSpace;
	m_PlotRect.bottom = rt.bottom - m_BottomSpace;

	m_BackGroundColor = RGB(115,190,198);
	m_SpaceColor = RGB(214,211,206);   
	m_GridColor = RGB(132,130,132);
}

void CYMcCurve::DrawString(CDC* pDC, CElecMapView *pView)
{
	pDC->SetBkMode(TRANSPARENT) ;
	pDC->TextOut(m_TyRect.left + m_LeftSpace ,m_TyRect.top+m_TopSpace / 4 ,"累加量曲线") ;
	pDC->TextOut(m_PlotRect.right - 4*m_RightSpace ,m_TyRect.top+m_TopSpace / 4 ,m_strname) ;
}

void CYMcCurve::DrawGrid(CDC* pDC, CElecMapView *pView)
{
	int i,j ,xb, yb, xe, ye;
	
	//画X轴
	for(i = 0;i < 24;i++)
    {
		yb = m_PlotRect.top;
		ye = m_PlotRect.bottom;
		xb = xe = (int)(m_PlotRect.left+m_PlotRect.Width()*i/24);
		
		if(i > 0 && i < 24)
			DrawLine(pDC,xb,yb,xe,ye,m_GridColor);
	}
	
	//画Y轴
	if((m_PlotRect.Height()/2-4)/10 > 20)
		 j = 10;
	else if((m_PlotRect.Height()/2-4)/5 > 20)
		 j = 5;
	else 
		j = 1;

	for(i = 0; i <= j; i ++)     
	{
		yb = ye = (int)(m_PlotRect.top + 1.0 * (m_PlotRect.Height()-4) * i / j );
		xb = m_PlotRect.left;
		xe = m_PlotRect.right;
		
		if (i > 0 && i < j)
			DrawLine(pDC,xb, yb, xe, ye, m_GridColor);
	}
}

void CYMcCurve::DrawLine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color)
{
	CPen newPen(PS_SOLID,1,color);
	CPen* oldPen=pDC->SelectObject(&newPen);
	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y2);
	pDC->SelectObject(oldPen);
}

void CYMcCurve::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView != NULL);

	pView->InvalidateTy(this);

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);
    break;
	case 1:
		if ((point.x >= m_x1)||(point.y >= m_y1))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1)>fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;

				if (point.x<m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;
				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x0 = point.x;
			m_y0 = point.y;
		}
		break;

	case 2:
		if (point.y >= m_y1)
		  break;

		m_y0 = point.y;
		break;

	case 3:
		if ((point.x <= m_x0)||(point.y >= m_y1))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;
				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;
				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x1 = point.x;
			m_y0 = point.y;
		}
		break;

	case 4:
		if (point.x <= m_x0)
		  break;

		m_x1 = point.x;
		break;

	case 5:
		if ((point.x <= m_x0)||(point.y <= m_y0))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;
				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;
				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x1 = point.x;
			m_y1 = point.y;
		}
		break;

	case 6:
		if (point.y <= m_y0)
		  break;

		m_y1 = point.y;
		break;

	case 7:
		if ((point.x >= m_x1)||(point.y <= m_y0))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;
				if (point.x < m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;
				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x0 = point.x;
			m_y1 = point.y;
		}
		break;

	case 8:
		if (point.x >= m_x1)
		  break;

		m_x0 = point.x;
		break;
	}

	pView->InvalidateTy(this);
}


void CYMcCurve::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView != NULL) 
		pView->InvalidateTy(this);

	m_x0 += xLen;
	m_x1 += xLen;
	m_y0 += yLen;
	m_y1 += yLen;

	if (pView != NULL)
		pView->InvalidateTy(this);
}

HCURSOR CYMcCurve::GetHandleCursor(int nHandle)
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

PointStruct CYMcCurve::GetHandle(int nHandle)
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

int CYMcCurve::GetHandleCount()
{
	return 8;
}

BOOL CYMcCurve::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange >= 0);
	if (m_bDelete) 
		return FALSE; 

	BOOL ret = FALSE;

	RectStruct rt,rt2;
	GetRect(&rt.x0,&rt.y0,&rt.x1,&rt.y1);
	
	rt2 = rt;
	rt2.InflateRect(errRange,errRange);
	ret = PointInRect(x,y,rt2);

	return ret;
}

BOOL CYMcCurve::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

CTyBase * CYMcCurve::Clone()
{
	ASSERT_VALID(this);

	CYMcCurve* pClone = new CYMcCurve(m_x0,m_y0,m_x1,m_y1);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

void CYMcCurve::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	
	if(ar.IsStoring())  
	{
		ar.Write((unsigned char *)ymcname,sizeof(char)*17) ;

		ar<<m_x0<<m_y0<<m_x1<<m_y1;
		ar<<hourinfo.y_min;
		ar<<hourinfo.y_max;
		ar<<hourinfo.y_scale; 
		ar<<ymin_day.y_min ;
		ar<<ymin_day.y_max ;
		ar<<ymin_day.y_scale;
		ar<<ymin_month.y_min;
		ar<<ymin_month.y_max ;
		ar<<ymin_month.y_scale; 
		ar<<ymin_year.y_min;
		ar<<ymin_year.y_max;
		ar<<ymin_year.y_scale;

		ar<<ymcurvescount ;
	
		for(int i = 0 ;i < ymcurvescount ;i++)
		{
			ar.Write((unsigned char *)ymcurves[i].ename,sizeof(char)*17) ;
			ar.Write((unsigned char *)ymcurves[i].cname,sizeof(char)*9) ;
			ar<<ymcurves[i].color ;
		}
		ar.Write((unsigned char *)ymc_unit,sizeof(char)*9) ;
	
	}
	else
	{
		ar.Read((unsigned char *)ymcname,sizeof(char)*17) ;
		m_strname = ymcname ;
		ar>>m_x0>>m_y0>>m_x1>>m_y1;
		ar>>hourinfo.y_min;
		ar>>hourinfo.y_max;
		ar>>hourinfo.y_scale; 
		ar>>ymin_day.y_min ;
		ar>>ymin_day.y_max ;
		ar>>ymin_day.y_scale;
		ar>>ymin_month.y_min;
		ar>>ymin_month.y_max ;
		ar>>ymin_month.y_scale; 
		ar>>ymin_year.y_min;
		ar>>ymin_year.y_max;
		ar>>ymin_year.y_scale;

		ar>>ymcurvescount ;
		for(int i = 0 ;i < ymcurvescount ;i++)
		{
			ar.Read((unsigned char *)ymcurves[i].ename,sizeof(char)*17) ;
			ar.Read((unsigned char *)ymcurves[i].cname,sizeof(char)*9) ;
			ar>>ymcurves[i].color ;
		}

		ar.Read((unsigned char *)ymc_unit,sizeof(char)*9) ;
	}
}


 void CYMcCurve::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);

	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)ymcname ,sizeof(char)*17) ;

		file->Write((float *)&m_x0,sizeof(m_x0));
		file->Write((float *)&m_y0,sizeof(m_y0));
		file->Write((float *)&m_x1,sizeof(m_x1));
		file->Write((float *)&m_y1,sizeof(m_y1));
		
		//小时曲线
		file->Write((unsigned char *)&hourinfo.y_min ,sizeof(hourinfo.y_min));
		file->Write((unsigned char *)&hourinfo.y_max ,sizeof(hourinfo.y_max));
		file->Write((unsigned char *)&hourinfo.y_scale ,sizeof(hourinfo.y_scale)); 

		//日曲线
		file->Write((unsigned char *)&ymin_day.y_min ,sizeof(ymin_day.y_min));
		file->Write((unsigned char *)&ymin_day.y_max ,sizeof(ymin_day.y_max));
		file->Write((unsigned char *)&ymin_day.y_scale ,sizeof(ymin_day.y_scale));

		//月曲线
		file->Write((unsigned char *)&ymin_month.y_min ,sizeof(ymin_month.y_min));
		file->Write((unsigned char *)&ymin_month.y_max ,sizeof(ymin_month.y_max));
		file->Write((unsigned char *)&ymin_month.y_scale ,sizeof(ymin_month.y_scale));

		//年曲线
		file->Write((unsigned char *)&ymin_year.y_min ,sizeof(ymin_month.y_min));
		file->Write((unsigned char *)&ymin_year.y_max ,sizeof(ymin_month.y_max));
		file->Write((unsigned char *)&ymin_year.y_scale ,sizeof(ymin_month.y_scale));

		file->Write((unsigned char *)&ymcurvescount ,sizeof(ymcurvescount)) ;

		for(int i = 0 ;i < ymcurvescount ;i++)
		{
			file->Write((unsigned char *)ymcurves[i].ename ,sizeof(char)*17) ;
			file->Write((unsigned char *)ymcurves[i].cname ,sizeof(char)*9) ;
			file->Write((unsigned char *)&ymcurves[i].color ,sizeof(ymcurves[i].color)) ;
		}

		file->Write((unsigned char *)ymc_unit ,sizeof(char)*9) ;
	}
	else//读取
	{
		file->Read((unsigned char *)ymcname ,sizeof(char)*17) ;

		m_strname = ymcname ;
		file->Read((float *)&m_x0,sizeof(m_x0));
		file->Read((float *)&m_y0,sizeof(m_y0));
		file->Read((float *)&m_x1,sizeof(m_x1));
		file->Read((float *)&m_y1,sizeof(m_y1));
		
		//小时曲线
		file->Read((unsigned char *)&hourinfo.y_min ,sizeof(hourinfo.y_min));
		file->Read((unsigned char *)&hourinfo.y_max ,sizeof(hourinfo.y_max));
		file->Read((unsigned char *)&hourinfo.y_scale ,sizeof(hourinfo.y_scale)); 

		//日曲线
		file->Read((unsigned char *)&ymin_day.y_min ,sizeof(ymin_day.y_min));
		file->Read((unsigned char *)&ymin_day.y_max ,sizeof(ymin_day.y_max));
		file->Read((unsigned char *)&ymin_day.y_scale ,sizeof(ymin_day.y_scale));

		//月曲线
		file->Read((unsigned char *)&ymin_month.y_min ,sizeof(ymin_month.y_min));
		file->Read((unsigned char *)&ymin_month.y_max ,sizeof(ymin_month.y_max));
		file->Read((unsigned char *)&ymin_month.y_scale ,sizeof(ymin_month.y_scale));

		//年曲线
		file->Read((unsigned char *)&ymin_year.y_min ,sizeof(ymin_month.y_min));
		file->Read((unsigned char *)&ymin_year.y_max ,sizeof(ymin_month.y_max));
		file->Read((unsigned char *)&ymin_year.y_scale ,sizeof(ymin_month.y_scale));

		file->Read((unsigned char *)&ymcurvescount ,sizeof(ymcurvescount)) ;

		for(int i = 0 ;i < ymcurvescount ;i++)
		{
			file->Read((unsigned char *)ymcurves[i].ename ,sizeof(char)*17) ;
			file->Read((unsigned char *)ymcurves[i].cname ,sizeof(char)*9) ;
			file->Read((unsigned char *)&ymcurves[i].color ,sizeof(ymcurves[i].color)) ;
		}

		file->Read((unsigned char *)ymc_unit ,sizeof(char)*9) ;
	}
}

IMPLEMENT_SERIAL(CYcCurve, CTyBase, 0)
CYcCurve::CYcCurve(float x0,float y0,float x1,float y1,int vId,CHMLayer* pLayer)
:CTyBase(vId,pLayer)
{
	m_x0 = x0 ;
	m_y0 = y0 ;
	m_x1 = x1 ;
	m_y1 = y1 ;
    m_yfmin = 0;
	m_yfmax = 0;
	m_ykedu = 0;
	m_count = 0;
	strcpy(m_tyname,"") ;
    strcpy(m_ydanwei,"") ;
	m_curvetype = 1;
	m_showplan = 0;
	m_haveplan = 0;
	memset(&yccurves,0,sizeof(ycurve_info)*YCMAXCURVE);	
}

CYcCurve::~CYcCurve()
{
	
}

void CYcCurve::OnEditProperties(CElecMapView * pView)
{
	CYcCurveSheet ycsheet("遥测曲线属性");
	ycsheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	ycsheet.Page3.m_fMin = m_yfmin;
	ycsheet.Page3.m_fMax = m_yfmax;
	ycsheet.Page3.m_ykdu = m_ykedu;
	ycsheet.Page3.m_ydanwei = m_ydanwei;
	ycsheet.Page3.m_tyName = m_tyname;
	ycsheet.Page2.m_xfmin = m_x0;
	ycsheet.Page2.m_yfmin = m_y0;
	ycsheet.Page2.m_xfwide = m_x1;

	ycsheet.Page2.m_fheight = m_y1;

	for(int i = 0 ;i <m_count ;i++)
	{
		ycsheet.Page1.curvearray.Add(yccurves[i])  ;
	}

	ycsheet.Page4.SetProp(m_haveplan,m_showplan,m_colorPlan,m_showYestorday,m_colorYestorday);
	if (ycsheet.DoModal() == IDOK)
	{
		 Invalidate();
		 SetModifiedFlag();
		 ycsheet.Page4.GetProp(m_haveplan,m_showplan,m_colorPlan,m_showYestorday,m_colorYestorday);
		 m_yfmin = min(ycsheet.Page3.m_fMin,ycsheet.Page3.m_fMax);
         m_yfmax = max(ycsheet.Page3.m_fMin,ycsheet.Page3.m_fMax);
		 
		 m_ykedu = ycsheet.Page3.m_ykdu;

	     strcpy(m_ydanwei ,ycsheet.Page3.m_ydanwei) ;
		 strcpy(m_tyname ,ycsheet.Page3.m_tyName) ;

		 m_x0 = ycsheet.Page2.m_xfmin;
         m_y0 = ycsheet.Page2.m_yfmin;
		 m_x1 = ycsheet.Page2.m_xfwide;
         m_y1 = ycsheet.Page2.m_fheight;
		 m_curvetype = 1;
	
		 m_count = ycsheet.Page1.curvearray.GetCount() ;
		
		for(int i = 0 ;i < m_count ;i++)
		{
			yccurves[i] = ycsheet.Page1.curvearray.GetAt(i) ;
		}

		 Invalidate();
	 }
 }
	
DRAW_TY CYcCurve::GetTyType()
{
	return tyyccurve;
}

void CYcCurve::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC != NULL);
	ASSERT(pView != NULL);

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	
	if (!IntersectRect(minx,miny,maxx,maxy))
	{
		return;
	}

	GetRect(&minx,&miny,&maxx,&maxy);

	CPoint pt1,pt2;
	pt1 = pView->UPtoLP(minx,miny);
	pt2 = pView->UPtoLP(maxx,maxy);

	CRect rt;
	rt.SetRect(pt1,pt2);
	rt.NormalizeRect();

	m_TyRect = rt ;
	
	RecalcRects(m_TyRect);

	DrawTyRect(pDC,pView);                            // 绘制图元区域
	DrawPlotRect(pDC,pView);                          // 绘制绘图区域
	
	DrawGrid(pDC,pView);                              // 绘制网格
	DrawString(pDC ,pView) ;							//绘制文字
}

void CYcCurve::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	*minX = __min(m_x0,m_x1);
	*minY = __min(m_y0,m_y1);
	*maxX = __max(m_x0,m_x1);
	*maxY = __max(m_y0,m_y1);
}

void CYcCurve::DrawTyRect(CDC *pDC ,CElecMapView *pView)
{
	CBrush newBrush(m_SpaceColor);	
	CBrush* oldBrush = pDC->SelectObject(&newBrush);
	pDC->Rectangle(m_TyRect);
    pDC->SelectObject(oldBrush);
}

// 绘制绘图区域
void CYcCurve::DrawPlotRect(CDC* pDC, CElecMapView *pView)
{
	CBrush newBrush(m_BackGroundColor);	
	CBrush* oldBrush = pDC->SelectObject(&newBrush);
	pDC->Rectangle(m_PlotRect);
	pDC->SelectObject(oldBrush);
}

// 计算绘图区大小
void CYcCurve::RecalcRects(CRect rt)
{
	m_TopSpace    = (rt.bottom-rt.top) /10;
	m_BottomSpace = (rt.bottom-rt.top) /20;
	m_LeftSpace   = (rt.right-rt.left) /20;
	m_RightSpace  = (rt.right-rt.left) /20;

	m_PlotRect.left	  = rt.left   + m_LeftSpace;
	m_PlotRect.right  = rt.right  - m_RightSpace;
	m_PlotRect.top	  = rt.top    + m_TopSpace;
	m_PlotRect.bottom = rt.bottom - m_BottomSpace;

	m_BackGroundColor = RGB(115,190,198);
	m_SpaceColor = RGB(214,211,206);   
	m_GridColor = RGB(132,130,132);
}

void CYcCurve::DrawString(CDC* pDC, CElecMapView *pView)
{
	CFont nFont;

	COLORREF nFontColor = RGB(0,0,0);
	nFont.CreatePointFont(120,"宋体") ;
	CFont* oldFont = pDC->SelectObject(&nFont);

	int bm = pDC->SetBkMode(TRANSPARENT);
	COLORREF preColor = pDC->SetTextColor(nFontColor); 

	pDC->TextOut(m_TyRect.left + m_LeftSpace ,m_TyRect.top+m_TopSpace / 4 ,"遥测曲线") ;
	pDC->SetBkMode(bm);
	pDC->SetTextColor(preColor);
	pDC->SelectObject(oldFont);
	nFont.DeleteObject();
}

void CYcCurve::DrawGrid(CDC* pDC, CElecMapView *pView)
{
	int i,j ,xb, yb, xe, ye;
	
	//画X轴
	for(i = 0;i < 24;i++)
    {
		yb = m_PlotRect.top;
		ye = m_PlotRect.bottom;
		xb = xe = (int)(m_PlotRect.left+m_PlotRect.Width()*i/24);
	
		if(i > 0&&i < 24)
			DrawLine(pDC,xb,yb,xe,ye,m_GridColor);
	}
	
	//画Y轴
	if((m_PlotRect.Height()/2-4)/10 > 20)
		 j = 10;
	else if((m_PlotRect.Height()/2-4)/5 > 20)
		 j = 5;
	else 
		j = 1;

	for(i = 0; i <= j; i ++)     
	{
		yb = ye = (int)(m_PlotRect.top + 1.0 * (m_PlotRect.Height()-4) * i / j );
		xb = m_PlotRect.left;
		xe = m_PlotRect.right;
		
		if (i > 0 && i < j)
			DrawLine(pDC,xb, yb, xe, ye, m_GridColor);
	}
}

void CYcCurve::DrawLine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color)
{
	CPen newPen(PS_SOLID,1,color);
	CPen* oldPen = pDC->SelectObject(&newPen);
	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y2);
	pDC->SelectObject(oldPen);
}

void CYcCurve::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView != NULL);

	pView->InvalidateTy(this);

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);
    break;
	case 1:
		if ((point.x >= m_x1)||(point.y >= m_y1))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1) > fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;

				if (point.x < m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;

				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}
		}
	  else 
		{
			m_x0 = point.x;
			m_y0 = point.y;
		}
    break;

	case 2:
		if (point.y >= m_y1)
		  break;

		m_y0 = point.y;
		break;

	case 3:
		if ((point.x <= m_x0)||(point.y >= m_y1))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;

				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;
				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x1 = point.x;
			m_y0 = point.y;
		}
		break;

	case 4:
		if (point.x <= m_x0)
		  break;

		m_x1 = point.x;
		break;

	case 5:
		if ((point.x <= m_x0)||(point.y <= m_y0))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;
				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;

				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x1 = point.x;
			m_y1 = point.y;
		}
		break;

	case 6:
		if (point.y <= m_y0)
		  break;

		m_y1 = point.y;
		break;

	case 7:
		if ((point.x >= m_x1)||(point.y <= m_y0))
		  break;

		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;

				if (point.x < m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;

				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x0 = point.x;
			m_y1 = point.y;
		}
		break;

	case 8:
		if (point.x >= m_x1)
		  break;

		m_x0 = point.x;
		break;
	}

	pView->InvalidateTy(this);
}

void CYcCurve::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView != NULL) 
		pView->InvalidateTy(this);

	m_x0 += xLen;
	m_x1 += xLen;
	m_y0 += yLen;
	m_y1 += yLen;

	if (pView != NULL)
		pView->InvalidateTy(this);
}

HCURSOR CYcCurve::GetHandleCursor(int nHandle)
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

PointStruct CYcCurve::GetHandle(int nHandle)
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

int CYcCurve::GetHandleCount()
{
	return 8;
}

BOOL CYcCurve::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange >= 0);
	if (m_bDelete)
		return FALSE; 

	BOOL ret = FALSE;

	RectStruct rt,rt2;
	GetRect(&rt.x0,&rt.y0,&rt.x1,&rt.y1);
	
	rt2 = rt;
	rt2.InflateRect(errRange,errRange);
	ret = PointInRect(x,y,rt2);

	return ret;
}

BOOL CYcCurve::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

CTyBase * CYcCurve::Clone()
{
	ASSERT_VALID(this);

	CYcCurve* pClone = new CYcCurve(m_x0,m_y0,m_x1,m_y1);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

void CYcCurve::Ty_Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	
	if(ar.IsStoring())  
	{
		ar.Write((unsigned char *)m_tyname,sizeof(char)*17) ;
		
		ar<<m_x0<<m_y0<<m_x1<<m_y1;
		ar<<m_yfmin;
		ar<<m_yfmax;
		ar<<m_ykedu;
		ar<<m_curvetype;
		ar<<m_haveplan;
		ar<<m_showplan;

		ar<<m_colorPlan;
		ar<<m_showYestorday;
		ar<<m_colorYestorday;

		ar.Write((unsigned char *)m_ydanwei,sizeof(char)*9) ;
	}
	else
	{
		ar.Read((unsigned char *)m_tyname,sizeof(char)*17) ;
		
		ar>>m_x0>>m_y0>>m_x1>>m_y1;
		ar>>m_yfmin;
		ar>>m_yfmax;
		ar>>m_ykedu; 
		ar>>m_curvetype;
		ar>>m_haveplan;
		ar>>m_showplan;

		ar>>m_colorPlan;
		ar>>m_showYestorday;
		ar>>m_colorYestorday;

		ar.Read((unsigned char *)m_ydanwei,sizeof(char)*9);
	}	
}

 void CYcCurve::Ty_Save(CFile *file, BOOL Yn)
{
	ASSERT_VALID(this);

	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)m_tyname ,sizeof(char)*17) ;
       
		file->Write((float *)&m_x0,sizeof(m_x0));
		file->Write((float *)&m_y0,sizeof(m_y0));
		file->Write((float *)&m_x1,sizeof(m_x1));
		file->Write((float *)&m_y1,sizeof(m_y1));
		
		
		file->Write((float *)&m_yfmin ,sizeof(m_yfmin));
		file->Write((float *)&m_yfmax ,sizeof(m_yfmin));
		file->Write((unsigned char *)&m_ykedu ,sizeof(m_ykedu)); 
		
		file->Write((unsigned char *)&m_curvetype,sizeof(m_curvetype)) ;
		file->Write((unsigned char *)&m_haveplan,sizeof(m_haveplan)) ;
		file->Write((unsigned char *)&m_showplan,sizeof(m_haveplan)) ;

		file->Write((unsigned char *)&m_colorPlan,sizeof(m_colorPlan)) ;
		file->Write((unsigned char *)&m_showYestorday,sizeof(m_showYestorday)) ;
		file->Write((unsigned char *)&m_colorYestorday,sizeof(m_colorYestorday));


		file->Write((unsigned char *)&m_count,sizeof(m_count)) ;

		for(int i = 0 ;i < m_count ;i++)
		{
			file->Write((unsigned char *)yccurves[i].ename ,sizeof(char)*17) ;
			file->Write((unsigned char *)yccurves[i].cname ,sizeof(char)*33) ;
			file->Write((unsigned char *)&yccurves[i].color ,sizeof(yccurves[i].color)) ;
		}

		 file->Write((unsigned char *)m_ydanwei ,sizeof(char)*9) ;
	}
	else//读取
	{
		file->Read((unsigned char *)m_tyname ,sizeof(char)*17) ;
        
		file->Read((float *)&m_x0,sizeof(m_x0));
		file->Read((float *)&m_y0,sizeof(m_y0));
		file->Read((float *)&m_x1,sizeof(m_x1));
		file->Read((float *)&m_y1,sizeof(m_y1));
		
		file->Read((float *)&m_yfmin ,sizeof(m_yfmin));
		file->Read((float *)&m_yfmax ,sizeof(m_yfmin));
		file->Read((unsigned char *)&m_ykedu ,sizeof(m_ykedu)); 
		  
        file->Read((unsigned char *)&m_curvetype,sizeof(m_curvetype)) ;
		file->Read((unsigned char *)&m_haveplan,sizeof(m_haveplan)) ;
		file->Read((unsigned char *)&m_showplan,sizeof(m_showplan)) ;

		file->Read((unsigned char *)&m_colorPlan,sizeof(m_colorPlan)) ;
		file->Read((unsigned char *)&m_showYestorday,sizeof(m_showYestorday)) ;
		file->Read((unsigned char *)&m_colorYestorday,sizeof(m_colorYestorday));

		file->Read((unsigned char *)&m_count,sizeof(m_count)) ;

		for(int i = 0 ;i < m_count ;i++)
		{
			file->Read((unsigned char *)yccurves[i].ename ,sizeof(char)*17) ;
			file->Read((unsigned char *)yccurves[i].cname ,sizeof(char)*33) ;
			file->Read((unsigned char *)&yccurves[i].color ,sizeof(yccurves[i].color)) ;
		}

		file->Read((unsigned char *)m_ydanwei ,sizeof(char)*9) ;
	}
}

void CYcCurve::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret;

	for (int i = 0;i < m_count;i++)
	{
		ret = GetEname_From_ReplacRule(yccurves[i].ename,str,ruleList);
		if (ret > 0)
		{
			SetModifiedFlag(true);
			strcpy(yccurves[i].ename,str);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CTyState, CTyBase, 0)

CTyState::CTyState(float x0,float y0,float x1,float y1,\
				   LPCSTR bmpname,BOOL blTranslate,\
				   COLORREF translatecolor, COLORREF textColor,\
				   BOOL blHScale,BOOL blVScale,\
				   int vId/*=0*/,CHMLayer* pLayer/*=NULL*/)
				   :CTyBase(vId,pLayer)
{
	m_x0 = x0;
	m_y0 = y0;
	m_x1 = x1;
	m_y1 = y1;
	m_bTranslate = blTranslate;
	m_TranslateColor = translatecolor;
	m_TextColor =textColor;
	m_blHScale = blHScale;
	m_blVScale = blVScale;					

	strcpy(m_bmpName,"state4030");
	RegisterImage(m_bmpName);

}

CTyState::~CTyState(void)
{
	UnRegisterImage(m_bmpName);
}

void CTyState::setScaleConst()
{
	m_blHScale = FALSE;
	m_blVScale = FALSE;
}

void CTyState::drawText(float startX, float startY, float width, float height,\
						CString  strText,\
						CDC *pDC, CElecMapView *pView )
{
	CBrush brush,*pOldBrush;
	LOGBRUSH logbrush;

	CFont font; 

	//创建字模  
	if (font.CreateFont(32,15,0,0,50,0,0,0,\
						GB2312_CHARSET,\
						OUT_TT_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FIXED_PITCH,\
						_T("黑体")) == FALSE)
		return;

	logbrush.lbStyle = BS_NULL;
	brush.CreateBrushIndirect(&logbrush);
	pOldBrush = pDC->SelectObject(&brush);
	COLORREF oldTextColor = pDC->SetTextColor(m_TextColor);
	
	//modified  for set text background by wx 20150928 
	int oldmode = pDC->SetBkMode(OPAQUE);
	pDC->SetBkColor(RGB(128,128,128));

	CFont* pOldFont = pDC->SelectObject(&font);

	CPoint startPoint = pView->UPtoLP(startX,startY);
	const char * p;
	int len;
	unsigned char c1,c2;
	char bz[4];
	float x1 = startX,y1 = startY;

	p = (const char *)strText;
	len = strlen(p);

	BOOL bFangXiang(TRUE);
	float fTextHeight(32);				//字体高度
	float fTextWidth(15);				//字体宽度
	float fJqWidth(2);					//间隔宽度
	while (len > 0)
	{
		c1 = *p;
		c2 = *(p+1);
		if((c1 > 127)&&(c2 > 127))  //如果是一个汉字
		{
			strncpy(bz,p,2);//拷贝一个汉字到bz中
			bz[2] = 0x00;
			p = p+2;        //跳过汉字指向下一个字符位置
			pDC->TextOut(startPoint.x,startPoint.y,bz); //在屏幕上写这个汉字
			len = len-2;                  //字符数减2

			//下一个字符的显示位置
			if (bFangXiang) 
				x1 = x1+fTextWidth*2+fJqWidth;
			else 
				y1=y1+fTextHeight+fJqWidth;

			startPoint = pView->UPtoLP(x1,y1);
		} 
		else
		{
			strncpy(bz,p,1);       //拷贝一个字符到bz中
			bz[1]=0x00;
			p++;                   //跳过这一个字符到下一个字符
			if (bFangXiang)
				pDC->TextOut(startPoint.x,startPoint.y,bz);//在屏幕上写字符
			else
				pDC->TextOut(startPoint.x+(int)(width/2),startPoint.y,bz);//在屏幕上写字符

			len--;                 //字符数减1

			if (bFangXiang) 
				x1=x1+fTextWidth+fJqWidth;
			else 
				y1=y1+fTextHeight+fJqWidth;

			startPoint=pView->UPtoLP(x1,y1);
		}
	}

	pDC->SetBkMode(oldmode);
	pDC->SetTextColor(oldTextColor);
	pDC->SelectObject(pOldBrush);
}

void CTyState::DrawDC( CDC *pDC, CElecMapView *pView )
{
	ASSERT(pDC != NULL);
	ASSERT(pView != NULL);

	if(m_bDelete)	//如果已经处于删除状态
		return;

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy))
		return;

	GetRect(&minx,&miny,&maxx,&maxy);

	CPoint pt1,pt2;
	pt1 = pView->UPtoLP(minx,miny);
	pt2 = pView->UPtoLP(maxx,maxy);

	CRect rt;
	rt.SetRect(pt1,pt2);
	rt.NormalizeRect();

	if ((rt.Width() < 1)||(rt.Height() < 1))
		return;

	DrawImage(m_bmpName,pDC,pt1.x,pt1.y,rt.Width()-96,rt.Height(),m_bTranslate,m_TranslateColor,m_blHScale,m_blVScale);

	float width =128;
	float height =32;
	drawText( (minx + rt.Width() - 96) ,miny, width, height, "南京市", pDC,pView);
}


void CTyState::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	*minX  = __min(m_x0,m_x1);
	*minY  = __min(m_y0,m_y1);
	*maxX = __max(m_x0,m_x1);
	*maxY = __max(m_y0,m_y1);
}

CTyBase * CTyState::Clone()
{
	ASSERT_VALID(this);

	CTyState* pClone = new CTyState(m_x0,m_y0,m_x1,m_y1,m_bmpName,m_bTranslate,m_TranslateColor,m_blHScale,m_blVScale);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

BOOL CTyState::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

void CTyState::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView != NULL) 
		pView->InvalidateTy(this);

	m_x0 += xLen;
	m_x1 += xLen;
	m_y0 += yLen;
	m_y1 += yLen;

	if (pView!=NULL) 
		pView->InvalidateTy(this);
}

int CTyState::GetHandleCount()
{
	return 8;
}

PointStruct CTyState::GetHandle(int nHandle)
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


HCURSOR CTyState::GetHandleCursor(int nHandle)
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


void CTyState::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView != NULL);

	pView->InvalidateTy(this);


	switch (nHandle)
	{
	default:
		ASSERT(FALSE);
		break;
	case 1:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1)>fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;
				if (point.x < m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;
				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x0 = point.x;
			m_y0 = point.y;
		}
		break;

	case 2:
		m_y0 = point.y;
		break;

	case 3:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y1)) 
			{
				m_y0 = point.y;
				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;
				if (point.y < m_y1)
					m_y0 = m_y1-(float)fabs(m_x1-m_x0);
				else
					m_y0 = m_y1+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x1 = point.x;
			m_y0 = point.y;
		}
		break;

	case 4:
		m_x1 = point.x;
		break;

	case 5:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x0) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;
				if (point.x < m_x0)
					m_x1 = m_x0-(float)fabs(m_y1-m_y0);
				else
					m_x1 = m_x0+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x1 = point.x;
				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x1 = point.x;
			m_y1 = point.y;
		}
		break;

	case 6:
		m_y1 = point.y;
		break;

	case 7:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_x1) > fabs(point.y-m_y0)) 
			{
				m_y1 = point.y;
				if (point.x < m_x1)
					m_x0 = m_x1-(float)fabs(m_y1-m_y0);
				else
					m_x0 = m_x1+(float)fabs(m_y1-m_y0);
			} 
			else
			{
				m_x0 = point.x;
				if (point.y < m_y0)
					m_y1 = m_y0-(float)fabs(m_x1-m_x0);
				else
					m_y1 = m_y0+(float)fabs(m_x1-m_x0);
			}
		}
		else 
		{
			m_x0 = point.x;
			m_y1 = point.y;
		}
		break;

	case 8:
		m_x0 = point.x;
		break;
	}

	pView->InvalidateTy(this);
}

BOOL CTyState::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange >= 0);

	if (m_bDelete) 
		return FALSE; 

	BOOL ret = FALSE;

	RectStruct rt,rt2;
	GetRect(&rt.x0,&rt.y0,&rt.x1,&rt.y1);

	rt2 = rt;
	rt2.InflateRect(errRange,errRange);

	ret = PointInRect(x,y,rt2);

	return ret;
}