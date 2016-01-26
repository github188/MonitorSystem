#pragma once
#include "canvas/TyBase.h"

class CTyStationAssistPng :public CTyBase
{
	DECLARE_SERIAL(CTyStationAssistPng);

public:
	CTyStationAssistPng();
	CTyStationAssistPng(float x0,float y0,float x1,float y1,LPCSTR szPngName,float fRotateAngle = 0.0f,BOOL bHScale = TRUE,BOOL bVScale = TRUE,int vId = 0,CHMLayer* pLayer = NULL);
	virtual ~CTyStationAssistPng(void);

public:
	void SetDefaultProp(CElecMapView *pView);

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
	char m_szPngName[256];		//位图名称
	BOOL m_bHScale;					//是否水平方向拉伸
	BOOL m_bVScale;					//是否垂直方向拉伸
	float   m_fRotateAngle;            //旋转角度      
};
