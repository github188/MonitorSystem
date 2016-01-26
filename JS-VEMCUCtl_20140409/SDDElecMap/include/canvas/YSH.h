#pragma once
#include <string>
using namespace std;

#include "TyBase.h"


struct stu_YSH
{
	RectStruct rect;
	char	ename[17];
	string	strBmpName_normal;
	string	strBmpName_warining;
	string	strBmpName_none;
};

class CElecMapView;
class CYSH : public CTyBase
{
	DECLARE_SERIAL(CYSH)
public:
	CYSH(float x0=0,float y0=0,float w=30,float h=10);
	~CYSH();

public:
	stu_YSH	m_stuYSH;

public:
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);
	virtual void Ty_Serialize(CArchive &ar);
	virtual void Ty_Save(CFile *file, BOOL Yn);

	virtual	void OnEditProperties(CElecMapView * pView = NULL);

	virtual	void GetRect(float *minX,float *minY,float *maxX,float *maxY);
	virtual DRAW_TY GetTyType();

	virtual void MoveHandleTo(int nHandle, PointStruct point, CElecMapView* pView,UINT nFlags =0);
	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView=NULL);

	virtual PointStruct GetHandle(int nHandle);
	virtual int GetHandleCount();	
	virtual HCURSOR	GetHandleCursor(int nHandle);
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	virtual BOOL PointInObj(float x,float y,float errRange);

	virtual CTyBase* Clone();

protected:
	void DrawBmp(CDC *pDC, CElecMapView *pView, CPoint pt1, CPoint pt3);
};