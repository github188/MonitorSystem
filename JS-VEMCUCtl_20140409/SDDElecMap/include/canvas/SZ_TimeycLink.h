#pragma once

#include "canvas/linkobjectbase.h"

class CSZ_TimeycLink :
	public CLinkObjectBase
{
	DECLARE_SERIAL(CSZ_TimeycLink);
public:
	CSZ_TimeycLink(void);
public:
	CSZ_TimeycLink(CString name);
	virtual BOOL OnEditProperties();
	virtual ~CSZ_TimeycLink(void);
};
