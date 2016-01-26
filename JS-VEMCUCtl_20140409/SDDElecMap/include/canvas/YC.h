#if !defined(AFX_YC_H__3D4F5736_CB3C_4C8A_B176_8EF5D05CE364__INCLUDED_)
#define AFX_YC_H__3D4F5736_CB3C_4C8A_B176_8EF5D05CE364__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "canvas/TYBase.h"
//#include "TYTool.h"

#define YC_STRING       "YCXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx"
#define YC_MINDISWIDTH   1.5
#define YC_MINDISHEIGHT  2
#define YC_FINDDOTDLLNAME "c:\\sunpac\\dll\\Finddot.dll"

// YC.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CYC window
struct Stu_YC
{
   RectStruct rect;//范围
   float fontwidth;//字体宽度
   float fontheight;//字体高度
   float fontjj;//字体间距
   float hintdis;
   float unitdis;
   char ename[17];//英文名
   int digitlength;//数据总长度
   char hint[33];//提示
   char unit[17];//单位
   LOGFONT lgfont;//字体
   COLORREF hintcolor;//提示颜色;
   COLORREF unitcolor;//单位颜色
   COLORREF levelupcolor;//越上限颜色
   COLORREF leveldowncolor;//越下限颜色
   COLORREF normalcolor;//正常颜色
   COLORREF disablecolor;//无效颜色
   BYTE warnupenable;  //越下限是否报警
   BYTE warnuplevels; //越上限报警级别 //0：5s自动清 1：30s清 2：复归自动清 3：手动清
   BYTE warnupkind; //越上限报警类型 //0位：推图 1位：闪烁 2位：语音
   char warnupwave[33]; //越上限报警语音
   BYTE warndownenable;  //越下限是否报警
   BYTE warndownlevels; //越下限报警级别 0：5s自动清 1：30s清 2：复归自动清 3：手动清
   BYTE warndownkind; //越下限报警类型 //0位：推图 1位：闪烁 2位：语音
   char warndownwave[33]; //越下限报警语音
   BYTE hintenable;//是否有提示
};


class CYC : public CTyBase
{
// Construction
	DECLARE_SERIAL(CYC);//支持序列化
public:
	Stu_YC m_yc;
	BOOL m_recalrect;//重新计算范围
	BOOL m_recalfont;//重新计算范围
	CYC(float x0=0,float y0=0,float fw=5,float fh=10,float fjj=0);
	CYC(Stu_YC * yc);
	virtual ~CYC();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual void SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[]);
	virtual void Set_Ty_Color(COLORREF color,CElecMapView * pView=NULL);
	virtual void DrawText(CDC * pDC,char *s, int count, int jj, int width,int x0, int y0, COLORREF itcolor, int *x1);
	void GetRectFromFont();
	void RefreshIt();
	void GetFontExtractWidth(CDC *pDC,CElecMapView *pView);
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
	int  GetCharLen(char *schar,int maxlen,int * hzcount);
	virtual CTyBase * Clone();
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);
	virtual void ParamReplace_Ty(CStringList& ruleList);

	// Generated message map functions
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YC_H__3D4F5736_CB3C_4C8A_B176_8EF5D05CE364__INCLUDED_)
