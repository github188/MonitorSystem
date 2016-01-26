#pragma once

#include "linktool.h"

class CLinkFileTool :
	public CLinkTool
{
public:
	CLinkFileTool(void);
	virtual ~CLinkFileTool(void);
    virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);

};
