#include "StdAfx.h"
#include "canvas/DeviceCanvas.h"
#include "canvas/HMLayer.h"

//ÉãÏñÍ·¼à¿Ø
IMPLEMENT_SERIAL(CCameraCanvas,CTyBase,0)

CCameraCanvas::CCameraCanvas(void)
{

}

CCameraCanvas::CCameraCanvas(PointStruct& start, PointStruct& end, \
						   LOGPEN *pLogPen,int vId,CHMLayer* pLayer)
{
	m_StartPoint = start;
	m_EndPoint   = end;
	m_logpen     = *pLogPen;
}

CCameraCanvas::~CCameraCanvas(void)
{

}

void CCameraCanvas::DrawDC(CDC *pDC, CElecMapView *pView)
{

}

void CCameraCanvas::DrawRect(CDC* pDC,int orgX,int orgY,float offsetX, float offsetY,float scaleX,float scaleY,COLORREF ReplaceColor,COLORREF ToColor,BOOL bReplace/* =TRUE */)
{

}

HCURSOR CCameraCanvas::GetHandleCursor(int nHandle)
{
	return NULL;
}

void CCameraCanvas::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{

}

void CCameraCanvas::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{

}

//µÆ¹â

//ÃÅ½û

//ÎÂ¶È
