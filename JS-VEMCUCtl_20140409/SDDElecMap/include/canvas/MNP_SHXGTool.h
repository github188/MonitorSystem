#pragma once

#include "canvas/Tybase.h"
#include "canvas/linktool.h"

class CMNP_SHXGTool :
	public CLinkTool
{
public:
	CMNP_SHXGTool(void);
	~CMNP_SHXGTool(void);
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags,const CPoint &point);
};
