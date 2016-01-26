#pragma once

#include "linkobjectbase.h"

class CLinkSupervise :
	public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkSupervise);

public:
	CLinkSupervise(void);
	~CLinkSupervise(void);

	char m_Ename[17];
	bool m_bIsAudio;

	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
};
