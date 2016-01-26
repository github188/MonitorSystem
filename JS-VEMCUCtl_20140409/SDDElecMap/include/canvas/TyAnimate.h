// TyAnimate.h: interface for the CTyAnimate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TYANIMATE_H__93A78E28_FB02_4BB3_849B_AB6E1F7950EC__INCLUDED_)
#define AFX_TYANIMATE_H__93A78E28_FB02_4BB3_849B_AB6E1F7950EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "canvas/TyBase.h"

struct Stu_SinglePic
{
	char picname[33];
	BYTE pickind;//1有透明/填充色 0无
	COLORREF piccolor;
};
struct Stu_TyAnimate
{
   char ename[33];
   RectStruct rect;//范围
   BYTE disOR0Kind;//0  不显示  1  显示
   BYTE picskind;//0 bmp 1 vec
   BYTE piccount;
   Stu_SinglePic pics[16];
};

class CTyAnimate : public CTyBase  
{
private:
	DECLARE_SERIAL(CTyAnimate);
public:
	Stu_TyAnimate m_animate;

	CTyAnimate(float x0=0,float y0=0,float x1=30,float y1=30);
	CTyAnimate(Stu_TyAnimate * animate);

	virtual ~CTyAnimate();
public:
	void UnRegistAllPic();
	void RegistAllPic();
	void RefreshIt();
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
};

#endif // !defined(AFX_TYANIMATE_H__93A78E28_FB02_4BB3_849B_AB6E1F7950EC__INCLUDED_)
