#pragma once

#include "canvas/tytool.h"

class CTy_YcYwTool :
	public CTyTool
{
public:
	CTy_YcYwTool(void);
	virtual ~CTy_YcYwTool(void);
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
};
