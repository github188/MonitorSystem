#pragma once
#include "canvas/TyBase.h"

class CElecMapView;
class CHMLayer;

class CCameraCanvas :
	public CTyBase
{
	DECLARE_SERIAL(CCameraCanvas)
private:
	CCameraCanvas(void);

public:
	CCameraCanvas(PointStruct& start, PointStruct& end, \
				  LOGPEN *pLogPen,int vId=0,CHMLayer* pLayer=NULL);
	~CCameraCanvas(void);

public:
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);
	virtual void DrawRect(CDC* pDC,int orgX,int orgY,float offsetX, float offsetY,float scaleX,float scaleY,COLORREF ReplaceColor,COLORREF ToColor,BOOL bReplace/* =TRUE */);
	
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags);

private:
	PointStruct m_StartPoint;
	PointStruct m_EndPoint;
	LOGPEN		m_logpen;
};
