#pragma once

#include "canvas/linktool.h"

class CSZ_SbLookLinkTool :
	public CLinkTool
{
public:
	CSZ_SbLookLinkTool(void);
	~CSZ_SbLookLinkTool(void);
	void OnMouseMove(CElecMapView* pView, UINT nFlags, const CPoint& point);
	void OnLButtonDown(CElecMapView* pView, UINT nFlags, const CPoint& point);
};
