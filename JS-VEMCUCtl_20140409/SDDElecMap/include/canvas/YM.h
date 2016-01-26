#if !defined(AFX_YM_H__271EBDCA_2B78_4AEF_8D28_9CFE1BC0378A__INCLUDED_)
#define AFX_YM_H__271EBDCA_2B78_4AEF_8D28_9CFE1BC0378A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TyBase.h"
//#include "TYTool.h"

// YM.h : header file
//

#define YM_STRING       "YMXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx"
#define YM_MINDISWIDTH   1.5
#define YM_MINDISHEIGHT  2
#define YM_FONTSIZEPERCENT .92
#define YM_FINDDOTDLLNAME "c:\\sunpac\\dll\\Finddot.dll"

/////////////////////////////////////////////////////////////////////////////
// CYM window
struct Stu_YM
{
   RectStruct rect;//范围
   float fontwidth;
   float fontheight;
   float fontjj;
   float hintdis;
   float unitdis;
   char ename[17];//英文名
   int digitlength;//数据总长度
   char hint[33];//提示
   char unit[17];//单位
   LOGFONT lgfont;//字体
   COLORREF hintcolor;//提示颜色;
   COLORREF unitcolor;//单位颜色
   COLORREF normalcolor;//正常颜色
   COLORREF disablecolor;//无效颜色
   BYTE hintenable;//是否有提示
};

class CYM : public CTyBase
{
// Construction
	DECLARE_SERIAL(CYM);//支持序列化
public:
	Stu_YM m_ym;
	BOOL m_recalrect;//重新计算范围
	BOOL m_recalfont;//重新计算范围
	CYM(float x0=0,float y0=0,float fw=5,float fh=10,float fjj=0);
	CYM(Stu_YM * ym);
	virtual ~CYM();
	virtual void ParamReplace_Ty(CStringList& ruleList);

// Attributes
public:

// Operations
public:

// Implementation
public:
	void DrawText(CDC * pDC,char * s,int count,int jj,int width,int x0,int y0,COLORREF itcolor,int * x1);
	void RefreshIt();
	void GetFontExtractWidth(CDC *pDC,CElecMapView *pView);

	virtual void SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[]);
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
	void GetRectFromFont();
	virtual CTyBase * Clone();
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);

	// Generated message map functions
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YM_H__271EBDCA_2B78_4AEF_8D28_9CFE1BC0378A__INCLUDED_)
