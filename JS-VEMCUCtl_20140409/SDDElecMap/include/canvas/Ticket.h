#pragma once

#include "linkobjectbase.h"

class CTicket :
	public CLinkObjectBase
{
	DECLARE_SERIAL(CTicket);
public:
	CTicket(void);
	~CTicket(void);
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
	BOOL OnEditProperties();
	char m_TicketName[33];
};
