#include "stdafx.h"
#include "canvas/ticket.h"
#include "canvas/TyTool.h"
#include "canvas/TyBase.h"
//#include "sp_draw.h"
#include "dialog/SelTicketDlg.h"

IMPLEMENT_SERIAL(CTicket,CLinkObjectBase,0)

CTicket::CTicket(void)
:CLinkObjectBase(lnkTicket,"")
{
	strcpy(m_TicketName,"");
}

CTicket::~CTicket(void)
{
}

BOOL CTicket::OnEditProperties()
{	
	CSelTicketDlg dlg;
	dlg.SetTicketName(m_name);
	if(dlg.DoModal()==IDOK)
	{
		strcpy(m_name,dlg.GetTicketName());
		strcpy(m_TicketName,m_name);
		return TRUE;
	}
	return FALSE;	
}

void CTicket::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{		
		ar.Write((unsigned char *)m_TicketName,sizeof(char)*33);
	}
	else
	{	
		ar.Read((unsigned char *)m_TicketName,sizeof(char)*33);
	}
}

void CTicket::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)m_TicketName,sizeof(char)*33);
	}
	else
	{
		file->Read((unsigned char *)m_TicketName,sizeof(char)*33);
	}
}
