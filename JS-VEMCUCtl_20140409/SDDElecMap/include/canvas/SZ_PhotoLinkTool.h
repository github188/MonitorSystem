// SZ_PhotoLinkTool.h: interface for the CSZ_PhotoLinkTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SZ_PHOTOLINKTOOL_H__6FA3D7E2_3D8D_11D6_A5ED_0080C8F60823__INCLUDED_)
#define AFX_SZ_PHOTOLINKTOOL_H__6FA3D7E2_3D8D_11D6_A5ED_0080C8F60823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LinkTool.h"

class CSZ_PhotoLinkTool : public CLinkTool  
{
public:
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CSZ_PhotoLinkTool();
	virtual ~CSZ_PhotoLinkTool();

};

#endif // !defined(AFX_SZ_PHOTOLINKTOOL_H__6FA3D7E2_3D8D_11D6_A5ED_0080C8F60823__INCLUDED_)
