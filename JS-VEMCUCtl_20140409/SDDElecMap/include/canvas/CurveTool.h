// CurveTool.h: interface for the CCurveTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURVETOOL_H__308DCFE1_D86E_11D5_BFC5_0050BABDFD68__INCLUDED_)
#define AFX_CURVETOOL_H__308DCFE1_D86E_11D5_BFC5_0050BABDFD68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "canvas/LinkTool.h"
#include "canvas/Curve.h"
class CElecMapView;
class CCurveTool : public CLinkTool  
{
public:
	CCurveTool();
	virtual ~CCurveTool();
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags,const CPoint &point);
};
#endif // !defined(AFX_CURVETOOL_H__308DCFE1_D86E_11D5_BFC5_0050BABDFD68__INCLUDED_)
