#pragma once

#include "canvas/linktool.h"

class CTicketTool :
	public CLinkTool
{
public:
	CTicketTool(void);
	~CTicketTool(void);
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags,const CPoint &point);
};
#include "Tybase.h"

class CElecMapView;
