#pragma once

#include "Tybase.h"
#include "linktool.h"

class CMNP_ZJNHTool :
	public CLinkTool
{
public:
	CMNP_ZJNHTool(void);
	~CMNP_ZJNHTool(void);
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags,const CPoint &point);
};
