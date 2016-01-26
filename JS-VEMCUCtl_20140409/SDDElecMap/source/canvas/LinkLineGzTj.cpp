#include "stdafx.h"
#include "canvas/linklinegztj.h"
#include "dialog/PowerNameDlg.h"
#include "dialog/PowerGzTJSetDlg.h"

IMPLEMENT_SERIAL(CLinkLineGzTj,CLinkObjectBase,0)

CLinkLineGzTj::CLinkLineGzTj(void)
:CLinkObjectBase(lnkLineGzTj,"")
{
	memset(m_lineName,0,33);
}

CLinkLineGzTj::~CLinkLineGzTj(void)
{

}

BOOL CLinkLineGzTj::OnEditProperties()
{
	CPowerNameDlg dlg;
	dlg.SetName(m_name,m_lineName);
	if(dlg.DoModal()==IDOK)
	{
	
		dlg.GetName(m_name,m_lineName);
		return true;
				
	}
	return false;
}

void CLinkLineGzTj::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)
	{
		file->Write((unsigned char *)m_lineName,sizeof(char)*33);
	}
	else
	{
		file->Read((unsigned char *)m_lineName,sizeof(char)*33);
	}
}

void CLinkLineGzTj::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar.Write((unsigned char *)m_lineName,sizeof(char)*33);
	}
	else
	{
		ar.Read((unsigned char *)m_lineName,sizeof(char)*33);
	}
}

