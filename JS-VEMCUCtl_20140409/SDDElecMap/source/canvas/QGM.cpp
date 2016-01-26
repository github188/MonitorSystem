// QGM.cpp: implementation of the CQGM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/QGM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CQGM, CObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQGM::CQGM(float x0,float y0,float x1,float y1)
{
	m_fX0 = x0;
	m_fY0 = y0;
	m_fX1 = x1;
	m_fY1 = y1;
}

CQGM::~CQGM()
{

}

BOOL CQGM::IntersectRect(float x0, float y0, float x1, float y1)
{
	if ((x0 >= m_fX0)&&(x0 < m_fX1)&&(y0 >= m_fY0)&&(y1 >= m_fY1)) 
		return TRUE;
	else 
		return FALSE;
}

void CQGM::SetBounds(float x0, float y0, float x1, float y1)
{
	m_fX0 = x0;
	m_fY0 = y0;
	m_fX1 = x1;
	m_fY1 = y1;
}

void CQGM::Clear()
{
	m_TyBaseList.RemoveAll();
}

void CQGM::RemoveTy(CTyBase *pTy)
{
	ASSERT(pTy != NULL);
	POSITION pos = m_TyBaseList.Find(pTy);
	if (pos == NULL) 
		return;

	m_TyBaseList.RemoveAt(pos);
}

void CQGM::Back(CTyBase *pTy)
{
	ASSERT(pTy != NULL);
	POSITION pos = m_TyBaseList.Find(pTy);
	if (pos == NULL)
		return;

	m_TyBaseList.RemoveAt(pos);
	m_TyBaseList.AddHead(pTy);
}

void CQGM::Front(CTyBase *pTy)
{
	ASSERT(pTy != NULL);
	POSITION pos = m_TyBaseList.Find(pTy);
	if (pos == NULL)
		return;

	m_TyBaseList.RemoveAt(pos);
	m_TyBaseList.AddTail(pTy);
}

CTyBase * CQGM::ObjectAt(PointStruct pt,float errRange)
{
	CTyBase * pTy;
	POSITION pos = m_TyBaseList.GetTailPosition();
	while (pos != NULL)
	{ 
		pTy = m_TyBaseList.GetPrev(pos);

		if (pTy->PointInObj(pt.x,pt.y,errRange)) 
			return pTy;
	}

	return NULL;
}

void CQGM::ZhenLi()
{
	CTyBase * pTy;
	POSITION prePos;
	POSITION pos = m_TyBaseList.GetTailPosition();
	while (pos != NULL)
	{
		prePos = pos;
		pTy = m_TyBaseList.GetPrev(pos);
		ASSERT(pTy != NULL);
		if (pTy->m_bDelete)
		{
			m_TyBaseList.RemoveAt(prePos);
		}
	}
}

void CQGM::AddTy(CTyBase *pTy)
{
	ASSERT(pTy != NULL);
	CTyBase * pObj;
	POSITION pos,prePos = NULL;
	pos = m_TyBaseList.GetTailPosition();
	prePos = pos;

	while (pos != NULL)
	{
	   pObj = m_TyBaseList.GetPrev(pos);
	   ASSERT((pObj != NULL));
	   if (pObj->m_iNumber < pTy->m_iNumber)
	   {
		   m_TyBaseList.InsertAfter(prePos,pTy);
		   return;
	   } 
	   else if (pObj->m_iNumber == pTy->m_iNumber)
	   {
		   ASSERT((pObj->m_iNumber != pTy->m_iNumber));
		   m_TyBaseList.InsertAfter(prePos,pTy);
		   return;
	   }	 
	   prePos = pos;
	}

	if (prePos == NULL) m_TyBaseList.AddHead(pTy);

}
