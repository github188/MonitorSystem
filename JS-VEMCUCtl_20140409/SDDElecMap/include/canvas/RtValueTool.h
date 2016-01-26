// RtValueTool.h: interface for the CRtValueTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTVALUETOOL_H__3FDFED04_D9AA_4C16_A1CE_1C92A5B41DD0__INCLUDED_)
#define AFX_RTVALUETOOL_H__3FDFED04_D9AA_4C16_A1CE_1C92A5B41DD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "canvas/TyTool.h"
#include "canvas/TyAnimate.h"
#include "canvas/Tydatetime.h"
#include "canvas/ym.h"
#include "canvas/yc.h"
#include "canvas/yx.h"
#include "canvas/ydx.h"
#include "canvas/newyc.h"


class CTyDirectionTool : public CTyTool  
{
public:
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CTyDirectionTool();
	virtual ~CTyDirectionTool();
  
	static int c_type[4];
	static COLORREF c_color1;
	static COLORREF c_color2;
	static BOOL c_blTranslate;
	static int c_nWidth;
};


class CTyHMProxyTool : public CTyTool  
{
public:
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CTyHMProxyTool();
	virtual ~CTyHMProxyTool();
  static int c_Width;
	static int c_Height;
	static int c_display;
	static char c_str1[33];
	static char c_str2[33];
	static COLORREF c_Color1;
	static COLORREF c_Color2;
	static BOOL c_bFlash;
	static BOOL c_FangXiang;
	static float c_TextHeight;
	static float c_TextWidth;
	static float c_JqWidth;
	static char c_TextFont[16];
	static BOOL c_bTranslate1;
	static BOOL c_bTranslate2;
};

class CYCTool : public CTyTool  
{
public:
	static Stu_YC m_yc;
	static BOOL canuse;
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CYCTool();
	virtual ~CYCTool();

};

class CNewYCTool : public CTyTool
{
public:
	static Stu_NewYC m_yc;
	static BOOL canuse;
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CNewYCTool() ;
	virtual ~CNewYCTool() ;
};

class CYMTool : public CTyTool  
{
public:
	static Stu_YM m_ym;
	static BOOL canuse;
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CYMTool();
	virtual ~CYMTool();

};

class CYXTool : public CTyTool  
{
public:
	static Stu_YX m_yx;
	static BOOL canuse;
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CYXTool();
	virtual ~CYXTool();

};


class CYDXTool : public CTyTool  
{
public:
	static Stu_YDX m_ydx;
	static BOOL canuse;
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CYDXTool();
	virtual ~CYDXTool();

};

class CTyDateTimeTool : public CTyTool  
{
public:
	static Stu_TyDateTime m_TyDateTime;
	static BOOL canuse;
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CTyDateTimeTool();
	virtual ~CTyDateTimeTool();

};


class CTyAnimateTool : public CTyTool  
{
public:
	static Stu_TyAnimate m_animate;
	static BOOL canuse;
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CTyAnimateTool();
	virtual ~CTyAnimateTool();

};


class CTyMonNiYKTool : public CTyTool  
{
public:
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CTyMonNiYKTool();
	virtual ~CTyMonNiYKTool();
	static int c_Width;
	static int c_Height;
	static int c_display;
	static char c_str1[33];
	static char c_str2[33];
	static COLORREF c_Color1;
	static COLORREF c_Color2;
	static BOOL c_FangXiang;
	static float c_TextHeight;
	static float c_TextWidth;
	static float c_JqWidth;
	static char c_TextFont[16];
	static BOOL c_bTranslate1;
	static BOOL c_bTranslate2;
};


class CMcCurveTool : public CTyTool 
{
	public:
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CMcCurveTool();
	virtual ~CMcCurveTool();
};


class CYMcCurveTool : public CTyTool 
{
	public:
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	CYMcCurveTool();
	virtual ~CYMcCurveTool();
};

class CYcCurveTool :
	public CTyTool
{
public:
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	 CYcCurveTool(void);
	~ CYcCurveTool(void);
};


#endif // !defined(AFX_RTVALUETOOL_H__3FDFED04_D9AA_4C16_A1CE_1C92A5B41DD0__INCLUDED_)
