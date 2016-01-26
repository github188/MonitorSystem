// SZ_PhotoLink.h: interface for the CSZ_PhotoLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SZ_PHOTOLINK_H__6FA3D7E1_3D8D_11D6_A5ED_0080C8F60823__INCLUDED_)
#define AFX_SZ_PHOTOLINK_H__6FA3D7E1_3D8D_11D6_A5ED_0080C8F60823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "canvas/LinkObjectBase.h"

class CSZ_PhotoLink : public CLinkObjectBase  
{
	DECLARE_SERIAL(CSZ_PhotoLink);
public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
	virtual BOOL OnEditProperties();
	CSZ_PhotoLink(CString strname);
	CSZ_PhotoLink();
	virtual ~CSZ_PhotoLink();
private:
	char m_sPhotoName[33];
};

#endif // !defined(AFX_SZ_PHOTOLINK_H__6FA3D7E1_3D8D_11D6_A5ED_0080C8F60823__INCLUDED_)
