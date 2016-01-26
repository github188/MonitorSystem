// JDJH.h: interface for the CLine class.
//  简单几何类
//
//
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JDJH_H__6058180F_FC99_408A_BB0E_FB47FEF9155A__INCLUDED_)
#define AFX_JDJH_H__6058180F_FC99_408A_BB0E_FB47FEF9155A__INCLUDED_
#define YCMAXCURVE 8
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "canvas/tybase.h"
#include "canvas/McCurveData.h"
#include "dialog/YMcCurvePage2.h"
#include "dialog/YcCurveDialog.h"

class CHMLayer;

//线条
class CLine : public CTyBase  
{
private:
	void SetDefaultProp(CElecMapView * pView);
	DECLARE_SERIAL(CLine);
	CLine(){};

public:
	CLine(float x0,float y0,float x1,float y1, LOGPEN *pLogPen,int vId=0,CHMLayer* pLayer=NULL);
	virtual ~CLine();

public:
	virtual void DrawRect(CDC *pDC,int orgX,int orgY,float offsetX, float offsetY, float scaleX, float scaleY, COLORREF ReplaceColor,COLORREF ToColor, BOOL bReplace = TRUE);
	virtual void Set_Line_Width(int width,CElecMapView * pView = NULL);
	virtual void Set_Ty_Color(COLORREF color,CElecMapView * pView = NULL);
	virtual BOOL Is_Net_Color_Vector();
	virtual void OnEditProperties(CElecMapView * pView = NULL);
	virtual DRAW_TY GetTyType();
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags = 0);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual PointStruct GetHandle(int nHandle);
	virtual int GetHandleCount();
	virtual BOOL PointInObj(float x, float y,float errRange);
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	virtual CTyBase * Clone();
	virtual void GetRect(float *minX, float *minY, float *maxX, float *maxY);
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView = NULL);

public:
	float m_x0,m_y0,m_x1,m_y1;	//直线的起点和终点
	LOGPEN m_logpen;
};

//虚线
class CXLine : public CTyBase  
{
private:
	void SetDefaultProp(CElecMapView *pView);
	DECLARE_SERIAL(CXLine);
	CXLine(){};

public:
	CXLine(float x0,float y0,float x1,float y1,COLORREF Color1, COLORREF Color2, BOOL blTranslate,int nWidth, int Type[4],int vId=0,CHMLayer* pLayer=NULL);
	virtual ~CXLine();

public:
	virtual void Set_Line_Width(int width,CElecMapView * pView=NULL);
	virtual BOOL Is_Net_Color_Vector();
	virtual void GetRect(float *minX, float *minY, float *maxX, float *maxY);
	virtual CTyBase * Clone();
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	virtual BOOL PointInObj(float x, float y,float errRange);
	virtual void OnEditProperties(CElecMapView * pView=NULL);
	virtual DRAW_TY GetTyType();
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags=0);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual int GetHandleCount();
	virtual PointStruct GetHandle(int nHandle);
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);

public:
	float m_x0,m_y0,m_x1,m_y1;	//直线的起点和终点
	int  m_Type[4];							//各个虚线段的宽度
	COLORREF m_color1,m_color2; //虚线颜色
	int m_iWidth;							//虚线宽度
	BOOL m_blTranslate;				//间隙是否透明
};


//矩形
class CRectangle : public CTyBase  
{
private:
	void SetDefaultProp(CElecMapView *pView);
	DECLARE_SERIAL(CRectangle);
	
	CRectangle(){};

public:
	enum RECT_TYPE
	{
		rtRect=0,
		rtRoundRect,
		rtEllipse
	};

	CRectangle(float x0,float y0,float x1,float y1, LOGPEN *pLogPen,BOOL blFill,COLORREF fillcolor,int vId=0,CHMLayer* pLayer = NULL);
	virtual ~CRectangle();

public:
	virtual void Set_Ty_Is_Fill(BOOL bFill,CElecMapView * pView=NULL);
	virtual void Set_Ty_Fill_Color(COLORREF color,CElecMapView * pView=NULL);
	virtual void DrawRect(CDC *pDC,int orgX,int orgY,float offsetX, float offsetY, float scaleX, float scaleY, COLORREF ReplaceColor,COLORREF ToColor, BOOL bReplace=TRUE);
	virtual void Set_Line_Width(int width,CElecMapView * pView=NULL);
	virtual void Set_Ty_Color(COLORREF color,CElecMapView * pView=NULL);
	virtual BOOL Is_Net_Color_Vector();
	virtual void OnEditProperties(CElecMapView * pView=NULL);
	virtual DRAW_TY GetTyType();
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags=0);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView=NULL);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual PointStruct GetHandle(int nHandle);
	virtual int GetHandleCount();
	virtual BOOL PointInObj(float x, float y,float errRange);
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	virtual CTyBase * Clone();
	virtual void GetRect(float *minX, float *minY, float *maxX, float *maxY);
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);

public:
	float m_x0,m_y0,m_x1,m_y1;	//直线的起点和终点
	LOGPEN m_logpen;
	BOOL m_bFill;						//背景是否被填充
	COLORREF m_FillColor;		//背景填充色
	RECT_TYPE m_rtType;			//分类型	
};

//多边形
class CPolygon : public CTyBase  
{
private:
	BOOL PointInLine(float x, float y,float errRange,float x0,float y0,float x1,float y1);
	void SetDefaultProp(CElecMapView *pView);
	DECLARE_SERIAL(CPolygon);

	CPolygon()
	{
		m_scale = -1;
		m_nPoints = 0; 
		m_nAllocPoints = 0;
		m_bFill = FALSE;
		m_LPoints = NULL;
		m_UPoints = NULL;
	}

public:
	enum POLYGON_TYPE
	{
		Polygon = 0,
		Polyline
	};

	CPolygon(float x0,float y0,float x1,float y1, LOGPEN *pLogPen,BOOL blFill,COLORREF fillcolor,int vId=0,CHMLayer* pLayer = NULL);
	virtual ~CPolygon();

public:
	virtual void Set_Ty_Is_Fill(BOOL bFill, CElecMapView *pView);
	virtual void Set_Ty_Fill_Color(COLORREF color, CElecMapView *pView);
	virtual void DrawRect(CDC *pDC, int orgX,int orgY,float offsetX, float offsetY, float scaleX, float scaleY, COLORREF ReplaceColor,COLORREF ToColor, BOOL bReplace=TRUE);
	virtual void Set_Ty_Color(COLORREF color,CElecMapView * pView=NULL);
	virtual void Set_Line_Width(int width,CElecMapView * pView=NULL);
	virtual BOOL Is_Net_Color_Vector();
	void ClonePoint(int id,CElecMapView *pView);
	void DeletePoint(int id,CElecMapView *pView);
	virtual void OnEditProperties(CElecMapView * pView=NULL);
	virtual DRAW_TY GetTyType();
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags=0);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView=NULL);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual PointStruct GetHandle(int nHandle);
	virtual int GetHandleCount();
	virtual BOOL PointInObj(float x, float y,float errRange);
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	virtual CTyBase * Clone();
	virtual void GetRect(float *minX, float *minY, float *maxX, float *maxY);
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);

	void CalaculateLPoints(CElecMapView *pView);
	void CalaculateBoundsRect();
	void AddPoint(PointStruct point, CElecMapView *pView);

	
public:
	float m_x0,m_y0,m_x1,m_y1;	//直线的起点和终点
	LOGPEN m_logpen;
	BOOL m_bFill;							//背景是否被填充
	COLORREF m_FillColor;			//背景填充色
	POLYGON_TYPE m_PolyType;	//分类型	
	int m_nAllocPoints;					//已分配的点数
	int m_nPoints;							//实际使用点数
	PointStruct* m_UPoints;			//用户坐标点集合
	CPoint* m_LPoints;					//逻辑坐标点集合
	float m_scale;							//当前显示比例
};


//文本
class CText : public CTyBase  
{
	DECLARE_SERIAL(CText);
	
private:
	void SetDefaultProp(CElecMapView *pView);
	void SetSize(float width,float height);

public:
	enum TEXT_TYPE
	{
		txtTime=0,
		txtDate,
		txtStatic
	};
	
	CText(){};

	CText(float startX0,float startY0,float txtwidth,float txtheight,BOOL fx,
				float jqwidth,char TextFont[],COLORREF textcolor,
				CString Text,int vId=0,CHMLayer* pLayer = NULL);

	virtual ~CText();

public:
	virtual void Set_Ty_Color(COLORREF color,CElecMapView * pView=NULL);
	virtual void SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[]);
	int GetChineseNum();
	virtual void OnEditProperties(CElecMapView * pView=NULL);
	virtual DRAW_TY GetTyType();
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags=0);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView=NULL);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual PointStruct GetHandle(int nHandle);
	virtual int GetHandleCount();
	virtual BOOL PointInObj(float x, float y,float errRange);
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	virtual CTyBase * Clone();
	virtual void GetRect(float *minX, float *minY, float *maxX, float *maxY);
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);
	virtual void ParamReplace_Ty(CStringList& ruleList);

public:
	float m_StartX;						//文本起点横坐标
	float m_StartY;						//起点纵坐标
	BOOL  m_FangXiang;			//标注角度 TRUE-横排  FALSE-竖排
	float m_TextHeight;				//字体高度
	float m_TextWidth;				//字体宽度
	float m_JqWidth;					//间隔宽度
	char	m_TextFont[16];			//字体
	TEXT_TYPE m_TextType;		//分类型	
	COLORREF m_TextColor;		//文本颜色
	CString m_Text;					//标注的文本信息
};

//位图
class CTyBmp : public CTyBase  
{
private:
	DECLARE_SERIAL(CTyBmp);

	CTyBmp()
	{	
		m_blHScale=TRUE;
		m_blVScale=TRUE;					
	};

	void SetDefaultProp(CElecMapView *pView);

public:
	CTyBmp(float x0,float y0,float x1,float y1, LPCSTR bmpname,BOOL blTranslate,COLORREF translatecolor,BOOL blHScale=TRUE,BOOL blVScale=TRUE,int vId=0,CHMLayer* pLayer=NULL);
	virtual ~CTyBmp();

public:
	virtual BOOL Is_Net_Color_Bmp();
	virtual void OnEditProperties(CElecMapView * pView=NULL);
	virtual DRAW_TY GetTyType();
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags=0);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView=NULL);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual PointStruct GetHandle(int nHandle);
	virtual int GetHandleCount();
	virtual BOOL PointInObj(float x, float y,float errRange);
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	virtual CTyBase * Clone();
	virtual void GetRect(float *minX, float *minY, float *maxX, float *maxY);
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);

public:
	float m_x0,m_y0,m_x1,m_y1;	//图象起点和终点
	BOOL m_bTranslate;				//背景是否透明
	COLORREF m_TranslateColor;	//背景透明色
	char m_bmpName[33];			//位图名称
	BOOL m_blHScale;					//是否水平方向拉伸
	BOOL m_blVScale;					//是否垂直方向拉伸
};

//矢量图
class CTyVector : public CTyBase  
{
private:
	DECLARE_SERIAL(CTyVector);
	
	CTyVector(){};
	void SetDefaultProp(CElecMapView *pView);

public:
	CTyVector(float x0,float y0,float x1,float y1, LPCSTR vectorname,BOOL blTranslate,COLORREF translatecolor,int vId = 0,CHMLayer* pLayer = NULL);
	virtual ~CTyVector();

public:
	virtual BOOL Is_Net_Color_Vector();
	virtual void OnEditProperties(CElecMapView * pView = NULL);
	virtual DRAW_TY GetTyType();
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags = 0);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView = NULL);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual PointStruct GetHandle(int nHandle);
	virtual int GetHandleCount();
	virtual BOOL PointInObj(float x, float y,float errRange);
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	virtual CTyBase * Clone();
	virtual void GetRect(float *minX, float *minY, float *maxX, float *maxY);
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);
	virtual void Set_Ty_Color(COLORREF color,CElecMapView * pView=NULL);

public:
	float m_x0,m_y0,m_x1,m_y1;	//图起点和终点
	BOOL m_bTranslate;					//动态色是否替换
	COLORREF m_TranslateColor;	//动态色的替换色
	char m_vectorName[33];			//位图名称
};

struct y_info
{
	DWORD y_min ;
	DWORD y_max ;
	DWORD y_scale ;
};

#define YMMAXCURVE	8

//累加量曲线
class CYMcCurve : public CTyBase  
{
public:
	DECLARE_SERIAL(CYMcCurve);

	CYMcCurve():m_strname(_T("")){};
	CYMcCurve(float x0,float y0,float x1,float y1,int vId=0,CHMLayer* pLayer = NULL);
	virtual ~CYMcCurve();

public:
	virtual void OnEditProperties(CElecMapView * pView);
	virtual DRAW_TY GetTyType();
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);
	virtual void GetRect(float *minX, float *minY, float *maxX, float *maxY);
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags=0);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView=NULL);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual PointStruct GetHandle(int nHandle);
	virtual int GetHandleCount();
	virtual BOOL PointInObj(float x, float y,float errRange);
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	virtual CTyBase * Clone();
	void DrawString(CDC* pDC, CElecMapView *pView);// 绘制文字
	void RecalcRects(CRect rt);// 计算绘图区大小
	void DrawTyRect(CDC* pDC, CElecMapView *pView);// 绘制图元区域
	void DrawPlotRect(CDC* pDC, CElecMapView *pView);// 绘制绘图区域
	void DrawGrid(CDC* pDC, CElecMapView *pView);// 绘制网格
	void DrawLine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color);// 绘制线段

public:
	float m_x0,m_y0,m_x1,m_y1;	     //直线的起点和终点
	CRect m_TyRect;                           // 图元区域
	CRect m_PlotRect;                         // 绘图区域	
	COLORREF m_SpaceColor;            // 空白区背景色	
	COLORREF m_BackGroundColor;  // 绘图区背景色	
	COLORREF m_GridColor;               // 网格颜色	
	int m_LeftSpace;                            // 左侧空白区域	
	int m_TopSpace;                            // 顶部空白区域
	int m_RightSpace;                          // 右侧空白区域	
	int m_BottomSpace;                      // 底部空白区域	

private:
	ymcurve_info ymcurves[YMMAXCURVE] ;
	y_info	hourinfo ;//小时曲线
	y_info  ymin_day ;
	y_info	ymin_month ;
	y_info	ymin_year ;
	BYTE	ymcurvescount ;
	char	ymc_unit[9] ;
	char	ymcname[17] ;//名称
	RectStruct rect ;//范围
	CString m_strname;
};

//毛刺曲线矩形
struct StringAlign	
{ 
	int HAlign;
	int VAlign; 
};   // 对齐方式

enum   CTextAlign
{ 
	LEFT = 0,
	CENTER = 1, 
	RIGHT = 2, 
	TOP = 0,
	BOTTOM = 2 
};

class CMcCurve : public CTyBase  
{
private:
	void SetDefaultProp(CElecMapView *pView);
	DECLARE_SERIAL(CMcCurve);
	CMcCurve(){};

public:
	CMcCurve(float x0,float y0,float x1,float y1,int vId=0,CHMLayer* pLayer=NULL);
	virtual ~CMcCurve();

public:
	virtual void OnEditProperties(CElecMapView * pView);
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags=0);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView=NULL);
	virtual DRAW_TY GetTyType();
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual PointStruct GetHandle(int nHandle);
	virtual int GetHandleCount();
	virtual BOOL PointInObj(float x, float y,float errRange);
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	virtual CTyBase * Clone();
	virtual void GetRect(float *minX, float *minY, float *maxX, float *maxY);
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);
	virtual void ParamReplace_Ty(CStringList& ruleList);
	void SetAlign(int HAlign, int VAlign);	// 设置字符对齐方式

public:
	CMcCurveData mccurvedata;
	CMcCurveData mccurvedatabackup;
	float m_x0,m_y0,m_x1,m_y1;	//直线的起点和终点

protected:
	int m_TyScale;                          // 放大比例	
	CRect m_TyRect;                         // 图元区域
	CRect m_PlotRect;                       // 绘图区域	
	int m_LeftSpace;                        // 左侧空白区域	
	int m_TopSpace;                         // 顶部空白区域
	int m_RightSpace;                       // 右侧空白区域	
	int m_BottomSpace;                      // 底部空白区域	
	CRect m_PriorButtonRect;                // 向前滚动按钮区域	
	CRect m_NextButtonRect;                 // 向后滚动按钮区域
	COLORREF m_BackGroundColor;             // 绘图区背景色	
	COLORREF m_SpaceColor;                  // 空白区背景色	
	COLORREF m_GridColor;                   // 网格颜色		
	StringAlign m_StrAlign;                 // 文字对齐方式

protected:
	// 计算绘图区大小
	void RecalcRects(CRect rt);
	// 绘制图元区域
	void DrawTyRect(CDC* pDC, CElecMapView *pView);
	// 绘制绘图区域
	void DrawPlotRect(CDC* pDC, CElecMapView *pView);
	// 绘制Y轴坐标
	void DrawYTicker(CDC* pDC, CElecMapView *pView);
	// 绘制网格
	void DrawGrid(CDC* pDC, CElecMapView *pView);
	// 绘制X轴网格
	void DrawXGrid(CDC* pDC);
	// 绘制Y轴网格
	void DrawYGrid(CDC* pDC);
	// 绘制贯通线和自闭线提示文字
	void DrawHintWords(CDC* pDC, CElecMapView *pView);
	// 绘制向前滚动和向后滚动按钮
	void DrawWayButton(CDC* pDC, CElecMapView *pView);
	// 绘制文字
	void PrintString(CDC* pDC,int x, int y, int theta, const char* fmt);
	// 绘制线段
	void DrawLine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color);
};


class CYcCurve:public CTyBase
{
	DECLARE_SERIAL(CYcCurve);

public:
	CYcCurve(){};
	CYcCurve(float x0,float y0,float x1,float y1,int vId=0,CHMLayer* pLayer = NULL);
	virtual ~CYcCurve();

public:
	virtual void OnEditProperties(CElecMapView * pView);
	virtual DRAW_TY GetTyType();
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);
	virtual void GetRect(float *minX, float *minY, float *maxX, float *maxY);
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags=0);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView=NULL);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual PointStruct GetHandle(int nHandle);
	virtual int GetHandleCount();
	virtual BOOL PointInObj(float x, float y,float errRange);
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	virtual CTyBase * Clone();
	virtual void ParamReplace_Ty(CStringList& ruleList);

	void DrawString(CDC* pDC, CElecMapView *pView);// 绘制文字
	void RecalcRects(CRect rt);// 计算绘图区大小
	void DrawTyRect(CDC* pDC, CElecMapView *pView);// 绘制图元区域
	void DrawPlotRect(CDC* pDC, CElecMapView *pView);// 绘制绘图区域
	void DrawGrid(CDC* pDC, CElecMapView *pView);// 绘制网格
	void DrawLine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color);// 绘制线段
	
public:
	float m_x0,m_y0,m_x1,m_y1;//图起点和终点
	float m_yfmin;            //y轴最小值
	float m_yfmax;            //y轴最大值
	ycurve_info yccurves[YCMAXCURVE];
	char m_tyname[17];        //图元名称
	float m_ykedu;              //y轴刻度
	char m_ydanwei[9];        //y轴单位
	DWORD m_color;            //线条颜色
	float m_ryfmin;           //预留坐标最小值
	float m_ryfmax;           //预留坐标最大值
    int m_count;              //曲线实际条数
	BYTE m_curvetype;         //默认曲线类型 0-小时曲线 1-日曲线 2-月曲线，3-年曲线
	BYTE m_showplan;          //默认是否显示计划曲线 1显示，0不显示
	BYTE m_haveplan;          //主曲线是否有计划曲线
	COLORREF m_colorPlan;	  //计划曲线的颜色	
	BYTE m_showYestorday;     //主曲线是否显示昨日曲线
	COLORREF m_colorYestorday;//昨日曲线的颜色	
    byte m_yuliu[16];

	CRect m_TyRect;                          // 图元区域
	CRect m_PlotRect;                        // 绘图区域	
	COLORREF m_SpaceColor;          // 空白区背景色	
	COLORREF m_BackGroundColor;// 绘图区背景色	
	COLORREF m_GridColor;             // 网格颜色	
	int m_LeftSpace;                           // 左侧空白区域	
	int m_TopSpace;                           // 顶部空白区域
	int m_RightSpace;                         // 右侧空白区域	
	int m_BottomSpace;                     // 底部空白区域	
};

extern BOOL GetServer(CString& );


/////////////////////////////////////////////////////////////////
//变电站图标
/////////////////////////////////////////////////////////////////
class CTyState :
	public CTyBase
{
	DECLARE_SERIAL(CTyState);

private:
	CTyState(void)
	{	
		m_blHScale=TRUE;
		m_blVScale=TRUE;					
	};
	~CTyState(void);

public:
	CTyState(float x0,float y0,float x1,float y1, \
		LPCSTR bmpname,BOOL blTranslate, \
		COLORREF translatecolor, COLORREF textColor,\
		BOOL blHScale=TRUE,BOOL blVScale=TRUE,\
		int vId=0,CHMLayer* pLayer=NULL);

public:
	//virtual BOOL Is_Net_Color_Bmp();
	//virtual void OnEditProperties(CElecMapView * pView=NULL);
	//virtual DRAW_TY GetTyType();
	//virtual void Ty_Serialize(CArchive &ar);
	//virtual void Ty_Save(CFile *file, BOOL Yn);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags=0);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView=NULL);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual PointStruct GetHandle(int nHandle);
	virtual int GetHandleCount();
	virtual BOOL PointInObj(float x, float y,float errRange);
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	virtual CTyBase * Clone();
	virtual void GetRect(float *minX, float *minY, float *maxX, float *maxY);
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);

	void setScaleConst();

private: 
	void drawText(float startX, float startY, float width, float height,\
		CString  strText,\
		CDC *pDC, CElecMapView *pView );

public:
	float m_x0,m_y0,m_x1,m_y1;		//图象起点和终点
	BOOL m_bTranslate;				//背景是否透明
	COLORREF m_TranslateColor;		//背景透明色
	char m_bmpName[33];				//位图名称
	BOOL m_blHScale;				//是否水平方向拉伸
	BOOL m_blVScale;				//是否垂直方向拉伸
	COLORREF m_TextColor;
};

#endif // !defined(AFX_JDJH_H__6058180F_FC99_408A_BB0E_FB47FEF9155A__INCLUDED_)
