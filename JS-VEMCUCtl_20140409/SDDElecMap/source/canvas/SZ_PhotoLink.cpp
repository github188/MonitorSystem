// SZ_PhotoLink.cpp: implementation of the CSZ_PhotoLink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/SZ_PhotoLink.h"
#include "canvas/SysLinkTy.h"
#include "canvas/TyBase.h"
#include "dialog/PropColor.h"
#include "dialog/SZ_PhotoSelDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CSZ_PhotoLink,CLinkObjectBase,0)

CSZ_PhotoLink::CSZ_PhotoLink()
:CLinkObjectBase(lnk_SZ_PHOTO,"")
{
	//strcpy(m_sPhotoName,"");
	ZeroMemory(m_sPhotoName,sizeof(char)*33);
}

CSZ_PhotoLink::~CSZ_PhotoLink()
{

}

CSZ_PhotoLink::CSZ_PhotoLink(CString strname)
:CLinkObjectBase(lnk_SZ_PHOTO,strname)
{
	ZeroMemory(m_sPhotoName,sizeof(char)*33);
}

BOOL CSZ_PhotoLink::OnEditProperties()
{
	CSZ_PhotoSelDlg Dlg;

	Dlg.SetInfo(m_name,m_sPhotoName);

	if (Dlg.DoModal()==IDOK)
	{
		Dlg.GetInfo(m_name,m_sPhotoName);

		return TRUE;
	}
	else return FALSE;
}

void CSZ_PhotoLink::Serialize_Ty(CArchive &ar)
{
	if (ar.IsStoring())
		ar.Write((unsigned char *)m_sPhotoName,sizeof(char)*33);
	else
		ar.Read((unsigned char *)m_sPhotoName,sizeof(char)*33);
}

void CSZ_PhotoLink::Save_Ty(CFile *file, BOOL Yn)
{
	if (Yn)
		file->Write((unsigned char *)m_sPhotoName,sizeof(char)*33);
	else
		file->Read((unsigned char *)m_sPhotoName,sizeof(char)*33);
}
