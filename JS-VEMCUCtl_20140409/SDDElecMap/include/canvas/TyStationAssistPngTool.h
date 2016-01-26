#pragma once

#include "canvas/TyTool.h"
//#include "sp_drawView.h"
#include "../../ElecMapView.h"

class CTyStationAssistPngTool :public CTyTool
{
public:
	CTyStationAssistPngTool(void);
	virtual ~CTyStationAssistPngTool(void);

public:
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);

public:
	static int m_nWidth;
	static int m_nHeight;
	static char m_szPngName[256];
	static float m_fRotateAngle;
	static BOOL m_bHScale;
	static BOOL m_bVScale;	
};
