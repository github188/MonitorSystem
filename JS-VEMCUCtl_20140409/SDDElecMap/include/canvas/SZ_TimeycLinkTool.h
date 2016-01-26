#pragma once

#include "canvas/linktool.h"

class CSZ_TimeycLinkTool :
	public CLinkTool
{
public:
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CSZ_TimeycLinkTool(void);
	virtual ~CSZ_TimeycLinkTool(void);
};
