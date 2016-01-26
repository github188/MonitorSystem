// Vector.h: interface for the CVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR_H__9262E3D0_E518_4738_BD63_522D9EDD341D__INCLUDED_)
#define AFX_VECTOR_H__9262E3D0_E518_4738_BD63_522D9EDD341D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TyBase.h"
#include <afxmt.h>

class CVector: public CObject 
{
public:
	CSize GetSize();
	void Draw(CDC* pDC,CRect viewRect,CRect clipRect,COLORREF color,BOOL blReplace=TRUE);
	void Load(LPCSTR filepath);
  
	CVector(){ClearAll();}
	CVector(LPCSTR filepath);
	virtual ~CVector();

	float m_x0;
	float m_y0;
	float m_x1;
	float m_y1;

	static DWORD m_version;		//°æ±¾

private:
	void Calculate_Bound_Rect();
	void LoadFromStream(CFile* file);

	COLORREF m_replaceColor;	//Ìæ»»É«	
	CTyBaseList  m_list;

protected:
	void ClearAll();
};


typedef CTypedPtrMap<CMapStringToOb,CString,CVector*> CMapStringtoVector;

class CVectorList  
{
private:
	
	CMapStringtoVector m_map_vector;
	CMapStringToPtr    m_map_count;
	CMutex m_mutex;	

public:
	void DrawVector(LPCSTR vectorname,CDC* pDC,CRect viewRect,CRect clipRect,COLORREF color,BOOL blReplace=TRUE);
	CSize Get_Vector_Size(LPCSTR vecName);
	void UnRegister_Vector(LPCSTR vecName);
	void Register_Vector(LPCSTR vecName);
	CVectorList();
	virtual ~CVectorList();

protected:
	void Unlock();
	BOOL Lock();
	void ClearAll();
};

extern CVectorList g_VectorList; 

#endif // !defined(AFX_VECTOR_H__9262E3D0_E518_4738_BD63_522D9EDD341D__INCLUDED_)
