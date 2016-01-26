#pragma once

#include "linkobjectbase.h"

class CLinkPowerGzTj :
	public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkPowerGzTj);
public:
	CLinkPowerGzTj(void);
	~CLinkPowerGzTj(void);
public:
	virtual BOOL OnEditProperties();
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
	
protected:
	char m_powerName[33];
	char m_powerName1[33];
	int m_Check;
};
