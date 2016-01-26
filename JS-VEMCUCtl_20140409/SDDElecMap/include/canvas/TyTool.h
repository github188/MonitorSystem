// TyTool.h: interface for the CTyTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TYTOOL_H__38F468AE_FE9D_4844_8C91_2A64595FA553__INCLUDED_)
#define AFX_TYTOOL_H__38F468AE_FE9D_4844_8C91_2A64595FA553__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "canvas/Tybase.h"


class CElecMapView;
class CTyTool 
{
public:
	CTyTool(DRAW_TY type);
	virtual ~CTyTool();

public:
	virtual void OnMouseMove(CElecMapView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CElecMapView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDown(CElecMapView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CElecMapView* pView, UINT nFlags, const CPoint& point);

public:
	enum SelectMode
	{
		none,
		netSelect,
		move,
		size
	};

	DRAW_TY m_drawTy;				//对象自己所属工具类型

	static SelectMode selectMode;
	static int nDragHandle;

	static PointStruct lastPoint;

	static CTyTool* FindTool(DRAW_TY type);

	static LOGBRUSH c_LogBrush;
	static LOGPEN		c_LogPen;

	static CPoint		c_down;
	static CPoint		c_last;

	static DRAW_TY	c_drawTy;

	static UINT			c_nDownFlags;
	static CPtrList       c_tools;  //绘图工具集
};


class CSelectTool : public CTyTool  
{
public:
	CSelectTool();
	virtual ~CSelectTool();

public:
	virtual void  OnLButtonDblClk(CElecMapView * pView, UINT nFlags, const CPoint& point);
	virtual void  OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void  OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual	void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
};

class CLineTool : public CTyTool  
{
public:
	CLineTool();
	virtual ~CLineTool();

public:
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
};

class CRectangleTool : public CTyTool  
{
public:
	CRectangleTool(DRAW_TY type);
	virtual ~CRectangleTool();

public:
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);

public:
	static BOOL c_bFill;						//背景是否被填充
	static COLORREF c_FillColor;		//背景填充色
};


class CZoomOutTool : public CTyTool  
{
public:
	CZoomOutTool();
	virtual ~CZoomOutTool();

public:
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
};

class CZoomInTool : public CTyTool  
{
public:
	CZoomInTool();
	virtual ~CZoomInTool();

public:
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
};

class CXLineTool : public CTyTool  
{
public:
	CXLineTool();
	virtual ~CXLineTool();

public:
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);


public:
	static int c_type[4];
	static COLORREF c_color1;
	static COLORREF c_color2;
	static BOOL c_blTranslate;
	static int c_nWidth;
};


class CPolygon;
 
class CPolygonTool : public CTyTool  
{
public:
	CPolygonTool(DRAW_TY type);
	virtual ~CPolygonTool();

public:
	virtual void OnLButtonDblClk(CElecMapView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);

public:
	static BOOL c_bFill;						//背景是否被填充
	static COLORREF c_FillColor;		//背景填充色

	CPolygon* m_pTy;

};


class CTextTool : public CTyTool  
{
public:
	CTextTool(DRAW_TY type);
	virtual ~CTextTool();

public:
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);

public:
	static BOOL  c_FangXiang;				//标注角度，TRUE-横排  FALSE-竖排
	static int c_TextHeight;				//字体高度
	static int c_TextWidth;					//字体宽度
	static int c_JqWidth;						//间隔宽度
	static char	c_TextFont[16];			//字体
	static COLORREF c_TextColor;		//文本颜色
	static CString c_Text;					//文本内容
};


class CTyBmpTool : public CTyTool  
{
public:
	CTyBmpTool();
	virtual ~CTyBmpTool();

public:
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDblClk(CElecMapView *pView, UINT nFlags, const CPoint &point);

public:
	static int c_Width;
	static int c_Height;
	static char c_bmpName[33];
	static BOOL c_bTranslate;
	static COLORREF c_TranslateColor;
	static BOOL c_blHScale;
	static BOOL c_blVScale;					
};

class CTyVectorTool : public CTyTool  
{
public:
	CTyVectorTool();
	virtual ~CTyVectorTool();

public:
	virtual void OnMouseMove(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonUp(CElecMapView *pView, UINT nFlags, const CPoint &point);
	virtual void OnLButtonDown(CElecMapView *pView, UINT nFlags, const CPoint &point);

public:
	static int c_Width;
	static int c_Height;
	static char c_vectorName[33];
	static BOOL c_bTranslate;
	static COLORREF c_TranslateColor;
};

//add by wx 2015-09-25

class	CTyStateTool : public CTyTool
{
public:
	CTyStateTool();
	virtual ~CTyStateTool();

	virtual void OnLButtonDown(CElecMapView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CElecMapView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CElecMapView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CElecMapView* pView, UINT nFlags, const CPoint& point);

public:
	static int c_Width;
	static int c_Height;
	static CString c_strTextName;
	static BOOL    c_bTranslate;
	static COLORREF c_TranslateColor;
	static COLORREF c_TextColor;
};

#endif // !defined(AFX_TYTOOL_H__38F468AE_FE9D_4844_8C91_2A64595FA553__INCLUDED_)
