#pragma once

#include "linkobjectbase.h"
#include "TyBase.h"

class CLinkLineGzTj :public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkLineGzTj);
public:
	CLinkLineGzTj(void);
	~CLinkLineGzTj(void);
	public:
	virtual BOOL OnEditProperties();
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
	//virtual void ParamReplace_Ty(CStringList& ruleList);
protected:
	char m_lineName[33];
};
