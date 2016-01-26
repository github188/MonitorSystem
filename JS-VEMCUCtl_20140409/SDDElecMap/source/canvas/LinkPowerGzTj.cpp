#include "stdafx.h"
#include "canvas/linkpowergztj.h"
#include "canvas/TyBase.h"
#include "dialog/PowerNameDlg.h"
#include "dialog/PowerGzTJSetDlg.h"

IMPLEMENT_SERIAL(CLinkPowerGzTj,CLinkObjectBase,0)

CLinkPowerGzTj::CLinkPowerGzTj(void)
:CLinkObjectBase(lnkPowerGzTj,"")
{
	memset(m_powerName,0,33);
	memset(m_powerName1,0,33);
	m_Check=0;
}

CLinkPowerGzTj::~CLinkPowerGzTj(void)
{
}

BOOL CLinkPowerGzTj::OnEditProperties()
{
	CPowerGzTJSetDlg dlg;
	BOOL Flag;
	if(m_Check==0)
	    Flag=FALSE;
	else Flag=TRUE;
	dlg.SetName(m_name,m_powerName,m_powerName1,Flag);
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetName(m_name,m_powerName,m_powerName1,&Flag);
		if(Flag)m_Check=1;
		else
			m_Check=0;
			
		return true;
	}

	return false;
}

void CLinkPowerGzTj::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)
	{
		file->Write((unsigned char *)m_powerName,sizeof(char)*33);
		file->Write((unsigned char *)&m_Check,sizeof(m_Check));
		file->Write((unsigned char *)m_powerName1,sizeof(char)*33);
	}
	else
	{
		file->Read((unsigned char *)m_powerName,sizeof(char)*33);
		file->Read((unsigned char *)&m_Check,sizeof(m_Check));
		file->Read((unsigned char *)m_powerName1,sizeof(char)*33);
	}
}

void CLinkPowerGzTj::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar.Write((unsigned char *)m_powerName,sizeof(char)*33);
		ar.Write((unsigned char *)&m_Check,sizeof(m_Check));
		ar.Write((unsigned char *)m_powerName1,sizeof(char)*33);
	}
	else
	{
		ar.Read((unsigned char *)m_powerName,sizeof(char)*33);
		ar.Read((unsigned char *)&m_Check,sizeof(m_Check));
		ar.Read((unsigned char *)m_powerName1,sizeof(char)*33);
	}
}

