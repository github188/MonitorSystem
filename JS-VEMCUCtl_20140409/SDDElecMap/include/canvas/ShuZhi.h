// ShuZhi.h: interface for the CTyDirection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHUZHI_H__2ECE7D89_CFBC_44CA_AA88_6CB9008E3FAE__INCLUDED_)
#define AFX_SHUZHI_H__2ECE7D89_CFBC_44CA_AA88_6CB9008E3FAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "canvas/TyBase.h"

class CHMLayer;

//潮流量
class CTyDirection : public CTyBase  
{
private:
	void SetDefaultProp(CElecMapView *pView);
	DECLARE_SERIAL(CTyDirection);
	CTyDirection();

public:
	CTyDirection(float x0,float y0,float x1,float y1,COLORREF Color1, COLORREF Color2, BOOL blTranslate,int nWidth, int Type[4],LPCSTR dotname=NULL,BOOL zeroshow=TRUE,int vId=0,CHMLayer* pLayer=NULL);
	virtual ~CTyDirection();

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
	virtual void ParamReplace_Ty(CStringList& ruleList);


	float m_x0,m_y0,m_x1,m_y1;	//直线的起点和终点
	int  m_Type[4];							//各个虚线段的宽度
	COLORREF m_color1,m_color2; //虚线颜色
	int m_iWidth;								//虚线宽度
	BOOL m_blTranslate;					//间隙是否透明
	char m_dotname[17];					//系统点名称
	BOOL m_bZeroShow;						//值为0时,潮流量依然显示

};



//画面代理
class CTyHMProxy : public CTyBase  
{
private:
	void Register();
	void UnRegister();
	void SetDefaultProp(CElecMapView *pView);
	void GetRect_Text(float *minX, float *minY, float *maxX, float *maxY);
	int GetChineseNum(LPCSTR str);
	void DrawDC_Text(CDC *pDC, CElecMapView *pView);
	void DrawDC_Vecter(CDC *pDC, CElecMapView *pView);
	void DrawDC_Bmp(CDC* pDC,CElecMapView * pView);
	DECLARE_SERIAL(CTyHMProxy);
	CTyHMProxy();

public:
CTyHMProxy(float x0,float y0,float x1,float y1,LPCSTR hmname,
					 int display,LPCSTR str1,LPCSTR str2,COLORREF Color1, 
					 COLORREF Color2,BOOL bFlash,BOOL FangXiang,float TextHeight,
					 float TextWidth,float JqWidth,LPCSTR TextFont,BOOL bTranslate1,
					 BOOL bTranslate2,int vId=0,CHMLayer* pLayer=NULL);
	virtual ~CTyHMProxy();

public:
	virtual void SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[]);
	virtual void Set_Ty_Color(COLORREF color,CElecMapView * pView=NULL);
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
	virtual void ParamReplace_Ty(CStringList& ruleList);



	float m_x0,m_y0,m_x1,m_y1;	//显示范围的起点和终点
  char	m_hmname[33];		//代理的画面的名称
	int		m_display;			//表现形式
												// 0：位图表现
												// 1：矢量图表现
												// 2：文字表现

	char	m_str1[33];			//正常显示的 位图名称或矢量图名称或画面链接文字提示
	char	m_str2[33];			//异常显示的 位图名称或矢量图名称或画面链接文字提示
	COLORREF	m_color1;		//正常显示的 位图的透明色或矢量图的前景色或画面链接文字颜色
	COLORREF	m_color2;		//异常显示的 位图的透明色或矢量图的前景色或画面链接文字颜色

  BOOL m_bFlash;				//报警是否闪烁
  //文字表示时特殊的字段
	BOOL  m_FangXiang;				//标注角度
														//  TRUE-横排  FALSE-竖排
	float m_TextHeight;				//字体高度
	float m_TextWidth;				//字体宽度
	float m_JqWidth;					//间隔宽度
	char	m_TextFont[16];			//字体

	//位图
	BOOL	m_bTranslate1;		//位图表现:	正常显示的位图背景是否透明
													//矢量图:		正常显示的量图活动色是否替换
  
	BOOL	m_bTranslate2;		//位图表现：异常显示的位图背景是否透明
													//矢量图:		异常显示的量图活动色是否替换

protected:
	void Set_Size_Text(float cx,float cy);
	void Ty_MoveHandleTo_Text(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags);
};


#endif // !defined(AFX_SHUZHI_H__2ECE7D89_CFBC_44CA_AA88_6CB9008E3FAE__INCLUDED_)
