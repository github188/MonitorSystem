#pragma once

#include "canvas/linktool.h"

class CLinkSupvTool :
	public CLinkTool
{
public:
	CLinkSupvTool(void);
	virtual ~CLinkSupvTool(void);

	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
};
