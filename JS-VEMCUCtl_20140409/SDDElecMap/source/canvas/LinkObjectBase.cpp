// LinkObjectBase.cpp: implementation of the CLinkObjectBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "canvas/LinkObjectBase.h"
#include "canvas/TyBase.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CLinkObjectBase, CObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkObjectBase::CLinkObjectBase(int linktype,CString strname)
{
	m_key = linktype;
	m_pTy = NULL;

	ASSERT(strname.GetLength() <= 32);

	if (!strname.IsEmpty())
	{
	    strcpy(m_name,(LPSTR)strname.GetBuffer());
	}
	else 
	{
		ZeroMemory(m_name,sizeof(char)*33);
	}
}

CLinkObjectBase::~CLinkObjectBase()
{

}


BOOL CLinkObjectBase::OnEditProperties()
{
	return FALSE;
}

void CLinkObjectBase::Execute(UINT pos)
{

}

void CLinkObjectBase::Serialize(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar<<m_key;
		ar.Write((unsigned char *)m_name,sizeof(char)*33);
	}
	else
	{
		ar>>m_key;
		ar.Read((unsigned char *)m_name,sizeof(char)*33);
	}

	Serialize_Ty(ar);
}

void CLinkObjectBase::Save(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)m_name,sizeof(char)*33);
	}
	else
	{
		file->Read((unsigned char *)m_name,sizeof(char)*33);
	}

	Save_Ty(file,Yn);
}

void CLinkObjectBase::SetModifiedFlag(BOOL bl)
{
	if(m_pTy != NULL)
	{
		m_pTy->SetModifiedFlag(bl);
	}
}

CLinkObjectBase * CLinkObjectBase::Clone()
{
	return NULL;
}

void CLinkObjectBase::Serialize_Ty(CArchive &ar)
{

}

void CLinkObjectBase::Save_Ty(CFile *file, BOOL Yn)
{

}

void CLinkObjectBase::ParamReplace(CStringList& ruleList)
{
    CString str;
	int ret = CTyBase::GetStr_From_ReplacRule((LPCTSTR)m_name,str,ruleList);
	if (ret > 0)
	{
		strcpy(m_name,(LPSTR)str.GetBuffer());
		SetModifiedFlag(true);
	}

	ParamReplace_Ty(ruleList);
}

void CLinkObjectBase::ParamReplace_Ty(CStringList& ruleList)
{

}

