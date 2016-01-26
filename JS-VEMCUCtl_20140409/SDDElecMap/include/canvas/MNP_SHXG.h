#pragma once

#include "linkobjectbase.h"

class CMNP_SHXG :
	public CLinkObjectBase
{
	DECLARE_SERIAL(CMNP_SHXG);
public:	
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
	BOOL OnEditProperties();
	CMNP_SHXG(void);
	~CMNP_SHXG(void);
protected:
	BYTE Node,Line,Unit;
};
