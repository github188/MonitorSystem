#if !defined(AFX_YDX_H__0A1CA82E_C42F_47FD_A153_47E78E1FC6FC__INCLUDED_)
#define AFX_YDX_H__0A1CA82E_C42F_47FD_A153_47E78E1FC6FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// YDX.h : header file
#include "TyBase.h"
#include "TYTool.h"
//

/////////////////////////////////////////////////////////////////////////////
// CYDX window

#define YDX_STRING       "YXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx"
#define YDX_MINDISWIDTH   1.5
#define YDX_MINDISHEIGHT  2
#define YDX_FONTSIZEPERCENT .92
#define YDX_FINDDOTDLLNAME "c:\\sunpac\\dll\\Finddot.dll"


struct Stu_YDX
{
   RectStruct rect;//范围
   float fontwidth;
   float fontheight;
   float fontjj;
   char mainename[17];//英文名
   char slaveename[17];//英文名
   LOGFONT lgfont;//1字体
   COLORREF color1;//1颜色（图/文字）
   COLORREF color0;//0颜色（图/文字）
   COLORREF colordis;//无效颜色（图/文字）
   char picname1[33],picname0[33],picnamedis[33];//图名/文字
   BYTE pickind1,pickind0,pickinddis;//
       //0 位图 1 矢量图 2 文字 
   int diskind1,diskind0,diskinddis;//显示分项
       //0 无 1 透明色 2 统一色 4 闪烁
   BYTE warn1enable;  //0->1是否报警
   BYTE warn1levels; //0->1报警级别 //0：5s自动清 1：30s清 2：复归自动清 3：手动清
   BYTE warn1kind; //0->1报警类型 //0位：推图 1位：闪烁 2位：语音
   char warn1wave[33]; //0->1报警语音
   BYTE warn0enable;  //1->0是否报警
   BYTE warn0levels; //1->0报警级别 0：5s自动清 1：30s清 2：复归自动清 3：手动清
   BYTE warn0kind; //1->0报警类型 //0位：推图 1位：闪烁 2位：语音
   char warn0wave[33]; //1->0报警语音
   BYTE handit;//手动开合
};

class CYDX : public CTyBase
{
// Construction
	DECLARE_SERIAL(CYDX);//支持序列化
public:
	Stu_YDX m_ydx;
	BOOL m_recalrect,m_recalfont;
	CYDX(float x0=0,float y0=0,float x1=30,float y1=10);
	CYDX(Stu_YDX * ydx);
	~CYDX();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual void SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[]);
	virtual BOOL Is_Net_Color_Yx();
	void DrawItText(CDC *pDC, char *s, int count, int jj, int width, int x0, int y0, COLORREF itcolor, int *x1);
	void GetRectFromFont();
	void UnRegisterAllPic();
	void RegisterAllPic();
	void DrawVec(CDC *pDC, CElecMapView *pView,CPoint pt1,CPoint pt3);
	void DrawBmp(CDC *pDC, CElecMapView *pView,CPoint pt1,CPoint pt3);
	void RefreshIt();
	void GetFontExtractWidth(CDC * pDC,CElecMapView *pView);
	virtual int GetHandleCount();
	virtual PointStruct GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	virtual DRAW_TY GetTyType();
	virtual void GetRect(float *minX, float *minY, float *maxX, float *maxY);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView);
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags);
	virtual BOOL PointInObj(float x, float y,float errRange);
	virtual void OnEditProperties(CElecMapView * pView=NULL);
	int GetCharLen(char *schar,int maxlen,int * hzcount);
	virtual CTyBase * Clone();
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);
	virtual void ParamReplace_Ty(CStringList& ruleList);




	// Generated message map functions
protected:
};


#endif // !defined(AFX_YDX_H__0A1CA82E_C42F_47FD_A153_47E78E1FC6FC__INCLUDED_)
