#include "stdafx.h"
#include "canvas/linksupervise.h"
#include "dialog/SuperviseDlg.h"
#include "canvas/TyBase.h"

IMPLEMENT_SERIAL(CLinkSupervise,CLinkObjectBase,0)

CLinkSupervise::CLinkSupervise()
:CLinkObjectBase(lnkSupervise,"")
{
	memset(m_Ename ,0 ,17) ;
}

CLinkSupervise::~CLinkSupervise(void)
{

}

void CLinkSupervise::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar.Write((unsigned char *)m_Ename,sizeof(char)*17);
		ar.Write((unsigned char *)&m_bIsAudio, sizeof(bool));
	}
	else
	{
		ar.Read((unsigned char *)m_Ename,sizeof(char)*17);
		ar.Read((unsigned char *)&m_bIsAudio, sizeof(bool));
	}
}

BOOL CLinkSupervise::OnEditProperties()
{
	CPropertySheet sheet(_T("ÍøÂç¼à¿Ø")) ;
	CSuperviseDlg dlg ;
	dlg.SetProp(m_Ename, m_name, m_bIsAudio) ;
    
	sheet.AddPage((CPropertyPage*)&dlg) ;
	if(sheet.DoModal() == IDOK)
	{		
		dlg.GetProp(m_Ename ,m_name,m_bIsAudio) ;
		return true ;
	}
	return false ;
}

void CLinkSupervise::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)
	{
		file->Write((unsigned char*)m_Ename ,sizeof(char)*17) ;
		file->Write((unsigned char *)&m_bIsAudio, sizeof(m_bIsAudio));
	}
	else
	{
		file->Read((unsigned char*)m_Ename ,sizeof(char)*17) ;
		file->Read((unsigned char *)&m_bIsAudio, sizeof(m_bIsAudio));
	}
}
