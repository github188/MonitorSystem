// QGM.h: interface for the CQGM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QGM_H__EFCBD5FC_63A4_445B_B21F_29CC0756F5D5__INCLUDED_)
#define AFX_QGM_H__EFCBD5FC_63A4_445B_B21F_29CC0756F5D5__INCLUDED_

#include "canvas/TyBase.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------------
	--------------------		对象类：	切割面CQGM 
 	--------------------		设计者：	高正军
 	--------------------    日期：		2001-9-18		
	-----------------------------------------------------------------------------
	BOOL IntersectRect(float x0,float y0,float x1,float y1);
  功能:
			判断该区域是否和切割面相交
	参数:
	void SetBounds(float x0,float y0,float x1,float y1);
  功能:
			设置切割面范围
  void RemoveTy(CTyBase *pTy)
  功能:
			移走图元pTy
	参数:
		pTy-指向图元的指针 


	void Front(CTyBase* pTy);
	功能: 图元移到前端
	参数:
		pTy-指向图元的指针 

	void Back(CTyBase * pTy);
	功能: 图元移到后端
	参数:
		pTy-指向图元的指针 


	-----------------------------------------------------------------------------
*/


class CQGM : public CObject  
{
	DECLARE_SERIAL(CQGM);

protected:
	CQGM(){};

public:
	void AddTy(CTyBase* pTy);
	void ZhenLi();
	CTyBase * ObjectAt(PointStruct pt,float errRange);
	void Front(CTyBase* pTy);
	void Back(CTyBase* pTy);
	void RemoveTy(CTyBase * pTy);
	void Clear();
	void SetBounds(float x0,float y0,float x1,float y1);
	BOOL IntersectRect(float x0,float y0,float x1,float y1);

	CTyBaseList m_TyBaseList;		//切割层面所有的图元
	CQGM(float x0,float y0,float x1,float y1);
	virtual ~CQGM();

	//切割面的范围
	float m_fX0;
	float m_fY0;
	float m_fX1;
	float m_fY1;
};

#endif // !defined(AFX_QGM_H__EFCBD5FC_63A4_445B_B21F_29CC0756F5D5__INCLUDED_)
